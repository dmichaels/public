/*
 * Copyright (c) 2021, HYPR Corporation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * HYPR Linux PAM module (pam_hypr.so)
 *
 * This is a recommended /etc/pam.d/sshd file configuration:
 *
 *   auth [success=done auth_err=die new_authtok_reqd=ok default=ignore] pam_hypr.so
 *
 * Then on ssh, the pam_sm_authenticate function (below) will be called
 * initiating the HYPR authentication workflow, prompting the user for a
 * authentication, e.g. via iPhone biometric, and letting them in, or not.
 *
 * The configuration (e.g. access-token, api-base-url) is defined in
 * the /etc/security/pam_hypr.conf file. It should look something like this:
 *
 *   access_token          REDACTED
 *   api_base_url          https://bojandev.gethypr.com
 *   api_auth_endpoint     /rp/api/oob/client/authentication
 *   app_id                fIDO2Local
 *   poll_max_time_seconds 60
 *   poll_interval_seconds 2
 *   username dmichaels david.michaels@hypr.com
 *   username another-linux-user another-hypr-account@hypr.com
 *
 * Note that the Linux login to username mapping is also currently defined here.
 *
 * And this is the recommended/assumed /etc/ssh/sshd_config configuration:
 *
 *   ChallengeResponseAuthentication yes
 *   PasswordAuthentication no
 *   PubkeyAuthentication no
 *   UsePAM yes
 *   UseDNS no
 *
 * And optionally giving listed users direct ssh-key access with something
 * like this at the end of this file:
 *
 *   Match User tseliot,prufrock
 *     PubkeyAuthentication yes
 */

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <security/pam_modules.h>
#include <security/pam_ext.h>
#include "auth/hypr_pam_config.h"
#include "auth/hypr_auth.h"
#include "util/hypr_http.h"
#include "util/hypr_str.h"

static const int   HYPR_AUTH_RETURN_OK     = PAM_SUCCESS;
static const int   HYPR_AUTH_RETURN_ERROR  = PAM_AUTH_ERR;
static const int   HYPR_AUTH_RETURN_IGNORE = PAM_IGNORE;
static const char *HYPR_PAM_CONFIG_FILE    = "/etc/security/pam_hypr.conf";

static void  log_info   (const char *format, ...);
static void  log_warning(const char *format, ...);
static void  log_error  (const char *format, ...);
static void  log_debug  (const char *format, ...);
static bool  log_stderr = false;

/*
 * This is the main HYPR authentication function to be called from our PAM module
 * function pam_sm_authentication. Returns PAM_IGNORE if disabled or misconfigured.
 * Returns PAM_SUCCESS on successful HYPR authentication. Returns PAM_AUTH_ERR on
 * unsuccessful HYPR authentication.
 */
static int hypr_authentication(pam_handle_t *pamh, int flags, int argc, const char **argv)
{
    const char *config_file = HYPR_PAM_CONFIG_FILE;

    // Read arguments to pam_hypr.so line in /etc/pam.d/{sshd,common-auth} ...

    if (argc > 0) {
        int i;
        for (i = 0 ; i < argc ; i++) {
            if (hypr_str_iequals(argv[i], "disabled")) {
                log_warning("HYPR authentication disabled per 'disabled' in pam.d config");
                return HYPR_AUTH_RETURN_IGNORE;
            }
            if (hypr_str_iequals(argv[i], "debug") || hypr_str_iequals(argv[i], "logstderr")) {
                log_stderr = true;
                log_info("HYPR logging to stderr per 'debug' in pam.d config");
            }
            else if ((argv[i] != NULL) && (argv[i][0] == '/')) {
                config_file = argv[i];
                log_info("HYPR config file defined in pam.d config: %s", config_file);
            }
        }
    }

    // Make sure we have a config file (default: /etc/security/pam_hypr.conf) defined.

    if (hypr_str_blank(config_file)) {
        log_error("HYPR authentication config file is null");
        return HYPR_AUTH_RETURN_IGNORE;
    }

    // Make sure we can get the login name.
    // Note that (according to the pam_get_user docs) the memory 
    // associated with the returned login name should not be freed.

    const char *login = NULL;
    pam_get_user(pamh, &login, NULL);

    if (hypr_str_blank(login)) {
        log_error("Cannot find login name (via pam_get_user)");
        return HYPR_AUTH_RETURN_IGNORE;
    }

    // Get the name of the comand/service which invoked us. We can use this to disable HYPR
    // authentication specifically for, say, 'sudo', in pam_hypr.conf. This the can be gotten
    // from the extern string 'program_invocation_short_name' (with #define of _GNU_SOURCE
    // before #include of errno.h); but this isn't absolutely portable. Rather than use this
    // we'll use pam_get_item of PAM_SERVICE which is probably much more reliable/portable.

    const void *command = NULL;
    if ((pam_get_item(pamh, PAM_SERVICE, (const void **)(&command)) != PAM_SUCCESS) || (command == NULL)) {
        log_info("HYPR authentication cannot get command name.");
    }
    else {
        log_info("HYPR authentication command name: '%s'", command);
    }

    // Get the remote host, which is the one *from* which we are ssh/scp-ing.
    // Not really using this, yet, but may be more generally useful.
    // But we do allow using it in 'login_message' in pam_hypr.conf.
    //
    const char *remote_host = NULL;
    if ((pam_get_item(pamh, PAM_RHOST, (const void **)(&remote_host)) != PAM_SUCCESS) || (remote_host == NULL)) {
        log_info("HYPR authentication cannot get remote host name.");
    }
    else {
        log_info("HYPR authentication remote host name: '%s'", remote_host);
    }

    log_info("Reading HYPR config and looking for HYPR username for '%s' login: %s", login, config_file);

    // Read the config file (default: /etc/security/pam_hypr.conf).

    hypr_pam_config *config = hypr_pam_config_read(config_file, login, command, remote_host);

    if (config == NULL) {
        log_error("Cannot read HYPR config: %s", config_file);
        return HYPR_AUTH_RETURN_IGNORE;
    }
    else {
        log_info("Successfully read HYPR config: %s", config_file);
    }

    // Look at the results/content of the config file.

    int status = HYPR_AUTH_RETURN_IGNORE;

    if (config->disabled != HYPR_PAM_CONFIG_ENABLED) {
        if (config->disabled & HYPR_PAM_CONFIG_DISABLED) {
            log_info("HYPR authentication is disabled in: %s", config_file);
        }
        if (config->disabled & HYPR_PAM_CONFIG_DISABLED_LOGIN) {
            log_info("HYPR authentication is disabled for login '%s' in: %s", login, config_file);
        }
        if (config->disabled & HYPR_PAM_CONFIG_DISABLED_GROUP) {
            log_info("HYPR authentication is disabled for group '%s' of login '%s' in: %s", config->disabled_group, login, config_file);
        }
        if (config->disabled & HYPR_PAM_CONFIG_DISABLED_COMMAND) {
            log_info("HYPR authentication is disabled for command '%s' in: %s", command, config_file);
        }
        status = HYPR_AUTH_RETURN_IGNORE;
    }
    else if (hypr_str_blank(config->auth_config->access_token) || hypr_str_iequals(config->auth_config->access_token, "REDACTED")) {
        log_error("HYPR access_token is not defined in: %s", config_file);
        status = HYPR_AUTH_RETURN_IGNORE;
    }
    else if (hypr_str_blank(config->auth_config->base_url)) {
        log_error("HYPR api_base_url is not defined in: %s", config_file);
        status = HYPR_AUTH_RETURN_IGNORE;
    }
    else if (hypr_str_blank(config->auth_config->endpoint_path)) {
        log_error("HYPR api_auth_endpoint is not defined in: %s", config_file);
        status = HYPR_AUTH_RETURN_IGNORE;
    }
    else if (hypr_str_blank(config->auth_config->app_id)) {
        log_error("HYPR api_auth_endpoint is not defined in: %s", config_file);
        status = HYPR_AUTH_RETURN_IGNORE;
    }
    else {

        // Here, what we have in the config file is sufficient to continue.

        log_info("HYPR access_token: %s", "REDACTED");
        log_info("HYPR api_base_url: %s", config->auth_config->base_url);
        log_info("HYPR api_auth_endpoint: %s", config->auth_config->endpoint_path);
        log_info("HYPR app_id: %s", config->auth_config->app_id);
        log_info("HYPR poll_max_time_seconds: %d", config->auth_config->poll_max_time_seconds);
        log_info("HYPR poll_interval_seconds: %d", config->auth_config->poll_interval_seconds);
        if (config->auth_config->timeout <= 0) {
            log_info("HYPR timeout: undefined (using system default)");
        }
        else if (config->auth_config->timeout < 250) {
            log_warning("HYPR timeout: %d ms (very small so timeout easily possible)", config->auth_config->timeout);
        }
        else {
            log_info("HYPR timeout: %d ms", config->auth_config->timeout);
        }

        if (hypr_str_blank(config->username)) {
            //
            // If a login/HYPR-username mapping is not found then currently we will use
            // the login name as the HYPR-username. Before we had the username aliasing
            // mapping on the server-side this would most likely fail to login the user.
            // But now with server-side alias mapping in place the proper way to map
            // Linux login names to HYPR usernames is via the HYPR username alias API:
            // https://why.atlassian.net/wiki/spaces/SE/pages/1862664193/FSD+User+Aliases
            //
            if (hypr_str_iequals(config->on_unmapped_login, "ignore")) {
                log_warning("HYPR username for login (%s) not found so bypassing HYPR authentication", login);
                hypr_pam_config_free(config);
                return HYPR_AUTH_RETURN_IGNORE;
            }
            else if (hypr_str_iequals(config->on_unmapped_login, "fail")) {
                log_warning("HYPR username for login (%s) not found so failing HYPR authentication", login);
                hypr_pam_config_free(config);
                return HYPR_AUTH_RETURN_ERROR;
            }
            else {
                //
                // This is now (with server-side username aliasing) the normal path.
                // I.e. simply use the Linux login name as the HYPR username.
                //
                log_info("HYPR username mapping for login (%s) not found in config so using login as HYPR username", login);
                config->username = hypr_str_copy(login);
            }
        }
        else {
            log_info("HYPR username for login (%s) found: %s", login, config->username);
        }

        if (!hypr_str_blank(config->libcurl_path)) {
            hypr_http_libcurl_path_global_set(config->libcurl_path);
            log_info("HYPR libcurl shared library path explicitly specified in config: %s", config->libcurl_path);
        }

        // Create the request for HYPR authentication.
 
        hypr_auth_request *request = hypr_create_auth_request_with_config(config->auth_config);

        if (request == NULL) {
            log_error("Cannot create HYPR authentication request");
        }
        else {

            if (!hypr_str_blank(config->login_prompt)) {
                //
                // EXPERIMENTAL:
                // Trying to just echo before starting authentication process.
                // So far can only get working a prompt rather than a simple message,
                // which require the user at least hit enter. An oddness with this is if HYPR
                // authentication fails, then this (somehow) tries again, a total of three times;
                // questionable if this is desirable (or even controllable) behavior.
                // Only enabled if the 'login_prompt' is defined in: /etc/security/pam_hypr.conf
                //
                char *prompt_response;
                log_debug("Prompting for HYPR authentication per 'login_prompt' in: %s", config_file);
                pam_prompt(pamh, PAM_PROMPT_ECHO_OFF, &prompt_response, "%s", config->login_prompt);
                log_debug("Back from HYPR authentication prompt (%s)", prompt_response);
            }

            // Invoke the HYPR authentication process here/now.

            log_info("HYPR authenticating username: %s", config->username);

            hypr_auth_response *response = hypr_auth(request, config->username);

            log_debug("HYPR back from authenticating username: %s", config->username);

            // HYPR authentication process done; look at the result.

            if (response == NULL) {
                log_error("Did not authenticate user (null-response)");
                status = HYPR_AUTH_RETURN_ERROR;
            }
            else if (!(response->status & HYPR_AUTH_COMPLETED)) {
                log_error("Did not authenticate user (incomplete)");
                status = HYPR_AUTH_RETURN_ERROR;
            }
            else {
                log_info("Done authenticating user: OK");
                if (!hypr_str_blank(config->login_message)) {
                    if (hypr_str_equals(command, "ssh") || hypr_str_equals(command, "sshd")) {
                        //
                        // For some reason on Fedora and Kali this message appears twice;
                        // does not happen on Ubuntu, CentOS, RedHat, Debian, OpenSUSE.
                        // 
                        pam_info(pamh, "%s", config->login_message);
                    }
                }
                status = HYPR_AUTH_RETURN_OK;
            }

            hypr_free_auth_response(response);

            // The hypr_auth_config object (auth_config) is freed as a part of
            // the above hypr_free_auth_response call; and so should not be
            // freed again below as part of the hypr_pam_config_free call.

            config->auth_config = NULL;
        }
    }

    hypr_pam_config_free(config);

    return status;
}

PAM_EXTERN int pam_sm_authenticate
(
    pam_handle_t *pamh,
    int flags,
    int argc,
    const char **argv
)
{
    log_debug("pam_sm_authenticate(flags: 0x%X, argc: %d) enter", flags, argc);
    if (argc > 0) {
        int i;
        for (i = 0 ; i < argc ; i++) {
            log_debug("arg[%d] = \'%\'\n", i, argv[i]);
        }
    }

    int status = hypr_authentication(pamh, flags, argc, argv);

    log_debug("pam_sm_authenticate exit: %d", status);

    return status;
}

PAM_EXTERN int pam_set_data
(
    pam_handle_t *pamh,
    const char   *module_data_name,
    void         *data,
    void        (*cleanup)(pam_handle_t *pamh, void *data, int error_status)
)
{
    log_debug("pam_set_data enter");
    int status = PAM_SUCCESS;
    log_debug("pam_set_data exit: %d", status);
    return status;
}

PAM_EXTERN int pam_get_data
(
    const pam_handle_t  *pamh,
    const char          *module_data_name,
    const void         **data
)
{
    log_debug("pam_get_data enter");
    int status = PAM_SUCCESS;
    log_debug("pam_get_data exit: %d", status);
    return status;
}

PAM_EXTERN int pam_set_item
(
    pam_handle_t *pamh,
    int item_type,
    const void *item
)
{
    log_debug("pam_set_item enter");
    int status = PAM_SUCCESS;
    log_debug("pam_set_item exit: %d", status);
    return status;
}

PAM_EXTERN int pam_get_item
(
    const pam_handle_t *pamh,
    int item_type,
    const void **item
)
{
    log_debug("pam_get_item enter");
    int status = PAM_SUCCESS;
    log_debug("pam_get_item exit: %d", status);
    return status;
}

PAM_EXTERN int PAM_NONNULL((1,2)) pam_get_user
(
    pam_handle_t *pamh,
    const char **user,
    const char *prompt
)
{
    log_debug("pam_get_user enter");
    int status = PAM_SUCCESS;
    log_debug("pam_get_user exit: %d", status);
    return status;
}

PAM_EXTERN int pam_putenv
(
    pam_handle_t *pamh,
    const char *name_value
)
{
    log_debug("pam_putenv enter");
    int status = PAM_SUCCESS;
    log_debug("pam_putenv exit: %d", status);
    return status;
}

PAM_EXTERN const char *pam_getenv
(
    pam_handle_t *pamh,
    const char *name
)
{
    log_debug("pam_getenv enter");
    log_debug("pam_getenv exit");
    return NULL;
}

PAM_EXTERN char **pam_getenvlist
(
    pam_handle_t *pamh
)
{
    log_debug("pam_getenvlist enter");
    log_debug("pam_getenvlist exit");
    return NULL;
}

PAM_EXTERN const char *pam_strerror
(
    pam_handle_t *pamh,
    int errnum
)
{
    log_debug("pam_strerror enter");
    log_debug("pam_strerror exit");
    return NULL;
}

PAM_EXTERN int pam_fail_delay
(
    pam_handle_t *pamh,
    unsigned int  usec
)
{
    log_debug("pam_fail_delay enter");
    int status = PAM_SUCCESS;
    log_debug("pam_fail_delay exit: %d", status);
    return status;
}

PAM_EXTERN int pam_sm_setcred
(
    pam_handle_t *pamh,
    int           flags,
    int           argc,
    const char  **argv
)
{
    log_debug("pam_sm_setcred enter");
    int status = PAM_SUCCESS;
    log_debug("pam_sm_setcred exit: %d", status);
    return status;
}

PAM_EXTERN int pam_sm_acct_mgmt
(
    pam_handle_t *pamh,
    int           flags,
    int           argc,
    const char  **argv
)
{
    log_debug("pam_sm_acct_mgmt enter");
    int status = PAM_SUCCESS;
    log_debug("pam_sm_acct_mgmt exit: %d", status);
    return status;
}

PAM_EXTERN int pam_sm_open_session
(
    pam_handle_t *pamh,
    int           flags,
    int           argc,
    const char  **argv
)
{
    log_debug("pam_sm_open_session enter");
    int status = PAM_SUCCESS;
    log_debug("pam_sm_open_session exit: %d", status);
    return status;
}

PAM_EXTERN int pam_sm_close_session
(
    pam_handle_t *pamh,
    int           flags,
    int           argc,
    const char  **argv

)
{
    log_debug("pam_sm_close_session enter");
    int status = PAM_SUCCESS;
    log_debug("pam_sm_close_session exit: %d", status);
    return status;
}

PAM_EXTERN int pam_sm_chauthtok
(
    pam_handle_t *pamh,
    int           flags,
    int           argc,
    const char  **argv
)
{
    log_debug("pam_sm_chauthtok enter");
    int status = PAM_SUCCESS;
    log_debug("pam_sm_chauthtok exit: %d", status);
    return status;
}

static void LOG(int level, const char *format, va_list args)
{
    const char *prefix;
    switch (level) {
        case LOG_EMERG:   prefix = "EMERGENCY"; break;
        case LOG_ALERT:   prefix = "ALERT";     break;
        case LOG_CRIT:    prefix = "CRITICAL";  break;
        case LOG_ERR:     prefix = "ERROR";     break;
        case LOG_WARNING: prefix = "WARNING";   break;
        case LOG_NOTICE:  prefix = "NOTICE";    break;
        case LOG_INFO:    prefix = "INFO";      break;
        case LOG_DEBUG:   prefix = "DEBUG";     break;
        default:          prefix = "UNKNOWN";   level = LOG_NOTICE; break;
    }

    char message[8192];
    vsnprintf(message, sizeof(message) - 1, format, args);
    syslog(level, "pam_hypr %s: %s", prefix, message);
    if (log_stderr) fprintf(stderr, "%s: %s\n", prefix, message);
}

static void log_info(const char *format, ...)
{
    va_list args; va_start(args, format); LOG(LOG_INFO,  format, args); va_end(args);
}

static void log_warning(const char *format, ...)
{
    va_list args; va_start(args, format); LOG(LOG_WARNING,  format, args); va_end(args);
}

static void log_error(const char *format, ...)
{
    va_list args; va_start(args, format); LOG(LOG_ERR,  format, args); va_end(args);
}

static void log_debug(const char *format, ...)
{
    va_list args; va_start(args, format); LOG(LOG_DEBUG,  format, args); va_end(args);
}
