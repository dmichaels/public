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
 * HYPR command-line utility for testing, troublshooting, et cetera.
 */

#include <ctype.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "util/hypr_uuid.h"
#include "util/hypr_datetime.h"
#include "auth/hypr_auth.h"
#include "auth/hypr_auth_defaults.h"
#include "auth/hypr_auth_devices.h"
#include "auth/hypr_pam_config.h"

static void verbose_output(const char *format, ...);
static void debug_output(const char *format, ...);
static void (*output)(const char *format, ...);
static char *hypr_auth_status_string(int status);

static void on_authenticate(const char *url, const char *post_data);
static void on_authenticate_done(int status, const char *response);
static void on_authenticate_poll(const char *url);
static void on_authenticate_poll_done(int status, const char *response);

static bool        verbose      = false;
static bool        debug        = false;
static bool        showcurl     = false;
static const char *username     = NULL;
static const char *access_token = NULL;

static void usage()
{
    fprintf(stderr, "usage: hypr auth [--login login | --username username] [--config file] [--access-token token] [--base-url url] [--app-id id] [--verbose | --debug] [--show-curl]\n");
    exit(1);
}

/*
 * Test command-line app for HYPR authentication. Usage:
 *
 *   hypr auth --access-token token --username username [--app-id id] [--base-url] [--debug | --verbose]
 *
 * For example:
 *
 *   hypr auth --access-token REDACTED --app-id fIDO2Local --username david.michaels@hypr.com --debug
 */
int hypr_cli_auth(int argc, char *argv[], const char *arg)
{
    const char *config_file = NULL;
    const char *base_url = NULL;
    const char *endpoint_path = NULL;
    const char *app_id = NULL;
    const char *login = NULL;
    bool devices = hypr_str_iequals(arg, "--devices");
         debug   = hypr_str_iequals(arg, "--debug");

    int i;
    for (i = 1 ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "auth") || hypr_str_iequals(argv[i], "authenticate")) {
            //
            // Just part of little hack so we can use exactly like HyprJavaWebClient; see hypr.c.
            //
            ;
        }
        else if (hypr_str_iequals(argv[i], "--config") || hypr_str_iequals(argv[i], "--conf")) {
            if (++i >= argc) usage();
            config_file = argv[i];
            if (access(config_file, F_OK) != 0) {
                fprintf(stderr, "hypr: cannot open configuration file: %s\n", config_file);
                usage();
            }
        }
        else if (hypr_str_iequals(argv[i], "--base-url") ||
                 hypr_str_iequals(argv[i], "--baseurl") ||
                 hypr_str_iequals(argv[i], "-baseurl")) {
            if (++i >= argc) usage();
            base_url = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--endpoint-path") ||
                 hypr_str_iequals(argv[i], "--endpointpath") ||
                 hypr_str_iequals(argv[i], "--endpoint") ||
                 hypr_str_iequals(argv[i], "-endpoint-path") ||
                 hypr_str_iequals(argv[i], "-endpointpath") ||
                 hypr_str_iequals(argv[i], "-endpoint")) {
            if (++i >= argc) usage();
            endpoint_path = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--access-token") ||
                 hypr_str_iequals(argv[i], "--accesstoken") ||
                 hypr_str_iequals(argv[i], "--token") ||
                 hypr_str_iequals(argv[i], "-accesstoken") ||
                 hypr_str_iequals(argv[i], "-token")) {
            if (++i >= argc) usage();
            access_token = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--app-id") ||
                 hypr_str_iequals(argv[i], "--appid") ||
                 hypr_str_iequals(argv[i], "-app-id") ||
                 hypr_str_iequals(argv[i], "-appid")) {
            if (++i >= argc) usage();
            app_id = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--username") ||
                 hypr_str_iequals(argv[i], "-username")) {
            //
            // This is the HYPR (account) username.
            //
            if (++i >= argc) usage();
            username = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--login") ||
                 hypr_str_iequals(argv[i], "-login")) {
            //
            // This (Linux) login is ignored if username is specified.
            // If login is specified/user then the config file must be found;
            // i.e. to map to the HYPR (account) username.
            //
            if (++i >= argc) usage();
            login = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--devices") ||
                 hypr_str_iequals(argv[i], "-devices")) {
            devices = true;
        }
        else if (hypr_str_iequals(argv[i], "--verbose") ||
                 hypr_str_iequals(argv[i], "-verbose")) {
            verbose = true;
        }
        else if (hypr_str_iequals(argv[i], "--debug") ||
                 hypr_str_iequals(argv[i], "-debug")) {
            debug = true;
        }
        else if (hypr_str_iequals(argv[i], "--show-curl") ||
                 hypr_str_iequals(argv[i], "--showcurl") ||
                 hypr_str_iequals(argv[i], "--curl") ||
                 hypr_str_iequals(argv[i], "-show-curl") ||
                 hypr_str_iequals(argv[i], "-showcurl") ||
                 hypr_str_iequals(argv[i], "-curl")) {
            showcurl = true;
        }
    }

    if (debug) {
        output = debug_output;
    }
    else {
        output = verbose_output;
    }

    if (hypr_str_blank(config_file)) {
        config_file = HYPR_PAM_DEFAULT_CONFIG_FILE;
    }
    else if (hypr_str_iequals(config_file, "default")) {
        config_file = NULL;
    }

    if (!hypr_str_blank(login) && !hypr_str_blank(username)) {
        fprintf(stderr, "hypr: both --login and --username specified; ignoring --login\n");
        login = "";
    }

    if (hypr_str_blank(login)) {
        uid_t uid = geteuid();
        struct passwd *pw = getpwuid(uid);
        if (pw != NULL) {
            login = pw->pw_name;
        }
    }

    // Read the HYPR Linux PAM configuration file (pam_hypr.conf).

    hypr_pam_config *pam_config = hypr_pam_config_read(config_file, login, NULL, NULL);
    if (pam_config != NULL) {
        if (!hypr_str_blank(pam_config->libcurl_path)) {
            hypr_http_libcurl_path_global_set(pam_config->libcurl_path);
        }
        if (hypr_str_blank(access_token) && (pam_config->auth_config != NULL)) {
            access_token = pam_config->auth_config->access_token;
        }
        if (hypr_str_blank(base_url) && (pam_config->auth_config != NULL)) {
            base_url = pam_config->auth_config->base_url;
        }
        if (hypr_str_blank(endpoint_path) && (pam_config->auth_config != NULL)) {
            endpoint_path = pam_config->auth_config->endpoint_path;
        }
        if (hypr_str_blank(app_id) && (pam_config->auth_config != NULL)) {
            app_id = pam_config->auth_config->app_id;
        }
        if (hypr_str_blank(username)) {
            //
            // Only use username found in config (from the login name,
            // which is implicit or from the --login option) if not
            // specified in the --username option.
            //
            username = pam_config->username;
            if (hypr_str_blank(username)) {
                //
                // Username not found in pam_hypr.conf for
                // the given (or implicit, current) login name.
                //
                // If the 'on_unmapped_login' property in pam_hypr.conf
                // is not set (or is 'default') then just try using the
                // login name as the username.
                // 
                if (hypr_str_blank(pam_config->on_unmapped_login) ||
                    hypr_str_iequals(pam_config->on_unmapped_login, "default")) {
                    username = login;
                }
            }
        }
    }

    if (!debug) {
        if (!verbose) {
            output("HYPR Authentication Test Utility (use --verbose or --debug for more output) ...");
        }
        else {
            output("HYPR Authentication Test Utility (use --debug for more output) ...");
        }
    }
    else {
        output("HYPR Authentication Test Utility ...");
    }

    output("Configuration File: %s (%s)", config_file, pam_config != NULL ? "OK" : "not found");

    if (hypr_str_blank(access_token)) {
        fprintf(stderr, "hypr: access-token not found\n");
        usage();
    }
    else if (hypr_str_iequals(access_token, "REDACTED") || hypr_str_iequals(access_token, "<REDACTED>")) {
        fprintf(stderr, "hypr: access-token not yet set (%s)\n", access_token);
        usage();
    }
    else if (hypr_str_blank(username)) {
        if (!hypr_str_blank(login)) {
            fprintf(stderr, "hypr: username not found for login: %s\n", login);
        }
        else {
            fprintf(stderr, "hypr: username not found\n");
        }
        usage();
    }

    int access_token_length = strlen(access_token);
    output("Access Token: %c*******%c", access_token[0], access_token[access_token_length -1]);
    output("Base URL: %s", base_url);
    output("Endpoint Path: %s", endpoint_path);
    output("App ID: %s", app_id);
    if (!hypr_str_blank(login)) {
        output("Login: %s", login);
    }
    output("Username: %s", username);

    if (hypr_http_static_libcurl()) {
        output("Using libcurl: static");
    }
    else {
        char *libcurl_path = hypr_http_libcurl_path();
        if (!hypr_str_blank(libcurl_path)) {
            output("Using libcurl: dynamic -> %s %s", libcurl_path, hypr_http_libcurl_okay() ? "(OK)" : "(not loadable)"); 
        }
        else {
            output("Using libcurl: dynamic %s", hypr_http_libcurl_okay() ? "(OK)" : "(not loadable)"); 
        }
        hypr_str_free(libcurl_path);
    }

    if (devices) {
        hypr_auth_config *auth_config; bool free_auth_config;
        if ((pam_config != NULL) && (pam_config->auth_config != NULL)) {
            auth_config = pam_config->auth_config;
            free_auth_config = false;
        }
        else {
            auth_config = hypr_create_auth_config();
            free_auth_config = true;
        }
        char *devices_url = NULL;
        output("Getting devices for: %s", username);
        hypr_json devices_json = hypr_get_auth_devices(auth_config, username, &devices_url);
        if (showcurl && (devices_url != NULL)) {
            output("CURL: curl -X GET -L -H 'Content-Type: application/json' -H 'Authorization: Bearer %s' %s", access_token, devices_url);
        }
        int status = 0;
        if (devices_json == NULL) {
            fprintf(stderr, "hypr: null response from HYPR devices API call.\n");
            status = 1;
        }
        else {
            output(hypr_json_format(devices_json));
        }
        hypr_json_free(devices_json);
        hypr_pam_config_free(pam_config);
        if (free_auth_config) {
            hypr_free_auth_config(auth_config);
        }
        return status;
    }

    hypr_auth_request *request = hypr_create_auth_request();

    if (!hypr_str_blank(base_url)) {
        hypr_str_free(request->config->base_url);
        request->config->base_url = hypr_str_copy(base_url);
    }
    if (!hypr_str_blank(endpoint_path)) {
        hypr_str_free(request->config->endpoint_path);
        request->config->endpoint_path = hypr_str_copy(endpoint_path);
    }
    if (!hypr_str_blank(app_id)) {
        hypr_str_free(request->config->app_id);
        request->config->app_id = hypr_str_copy(app_id);
    }
    if (!hypr_str_blank(access_token)) {
        hypr_str_free(request->config->access_token);
        request->config->access_token  = hypr_str_copy(access_token);
    }

    request->on_authenticate           = on_authenticate;
    request->on_authenticate_done      = on_authenticate_done;
    request->on_authenticate_poll      = on_authenticate_poll;
    request->on_authenticate_poll_done = on_authenticate_poll_done;

    output("Authenticating for: %s", username);

    hypr_auth_response *response = hypr_auth(request, username);

    if (response == NULL) {
        output("Authentication failed (null-response) for: %s", username);
        hypr_pam_config_free(pam_config);
        return 1;
    }
    if (response->status & HYPR_AUTH_COMPLETED) {
        output("Authentication completed successfully for: %s", username);
    }
    else {
        output("Authentication did not complete for: %s", username);
    }

    if (debug) {
        char *status_string = hypr_auth_status_string(response->status);
        output("Authentication response for: %s", username);
        output("- request_id = %s", response->request_id);
        output("- authenticate_http_status = %d", response->authenticate_http_status);
        output("- authenticate_response_code = %d", response->authenticate_response_code);
        output("- authenticate_response_message = %s", response->authenticate_response_message);
        output("- status = 0x%x -> %s", response->status, status_string);
        output("- poll_count = %d", response->poll_count);
        hypr_str_free(status_string);
    }

    hypr_free_auth_response(response);
    hypr_pam_config_free(pam_config);

    return 0;
}

static void
on_authenticate(const char *url, const char *post_data)
{
    if (verbose || debug) {
        output("Initiating authentication for: %s", username);
    }
    if (showcurl) {
        char *post_data_for_curl = NULL;
        hypr_json post_data_json = hypr_json_parse(post_data);
        if (post_data_json != NULL) {
            const char *post_data_json_unformatted = hypr_json_noformat(post_data_json);
            if (post_data_json_unformatted != NULL) {
                post_data_for_curl = hypr_str_replace(post_data_json_unformatted, "'", "\\'", false);
            }
            hypr_json_free(post_data_json);
        }
        char curl_command[8192];
        snprintf(curl_command, sizeof(curl_command),
            "curl -X POST -L -H 'Content-Type: application/json' -H 'Authorization: Bearer %s' -d '%s' %s",
                access_token, post_data_for_curl != NULL ? post_data_for_curl : post_data, url);
        hypr_str_free(post_data_for_curl);
        output("CURL: %s", curl_command);
    }
    if (debug) {
        output("Authentication initiation URL (POST): %s", url);
        output("Authentication initiation POST data: %s", post_data);
    }
}

static void
on_authenticate_done(int http_status, const char *response)
{
    if (verbose || debug) {
        output("Initiated authentication for: %s", username);
    }
    if (debug) {
        output("Authenticatiion initiation response HTTP status: %d", http_status);
        output("Authenticatiion initiation response: %s", response);
    }
}

static void
on_authenticate_poll(const char *url)
{
    if (verbose || debug) {
        output("Polling authentication response for: %s", username);
    }
    if (showcurl) {
        output("CURL: curl -X GET -L -H 'Content-Type: application/json' -H 'Authorization: Bearer %s' %s", access_token, url);
    }
    if (debug) {
        output("Authentication polling URL (GET): %s", url);
    }
}

static void
on_authenticate_poll_done(int http_status, const char *response)
{
    if (verbose || debug) {
        output("Authentication polling done for: %s", username);
    }
    //
    // See if the namedUser we get back is different from the one we started with;
    // if so then the one we gave was an alias; just note this for info/troubleshooting.
    //
    hypr_json response_json = hypr_json_parse(response);
    if (response_json != NULL) {
        const char *named_user = hypr_json_get_string(response_json, "namedUser");
        if (!hypr_str_blank(named_user)) {
            if (!hypr_str_equals(named_user, username)) {
                static bool already_said_this = false;
                if (!already_said_this) {
                    output("Given username (%s) is actually an alias for: %s", username, named_user);
                    already_said_this = true;
                }
            }
        }
        hypr_json_free(response_json);
    }
    if (debug) {
        output("Authentication polling response HTTP status: %d", http_status);
        output("Authentication polling response: %s", response);
    }
}

static char *
hypr_auth_status_string(int status)
{
    char *result = NULL;
    if (status & HYPR_AUTH_STARTED) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "STARTED", true);
    }
    if (status & HYPR_AUTH_REQUEST_SENT) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "REQUEST_SENT", true);
    }
    if (status & HYPR_AUTH_INITIATED) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "INITIATED", true);
    }
    if (status & HYPR_AUTH_INITIATED_RESPONSE) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "INITIATED_RESPONSE", true);
    }
    if (status & HYPR_AUTH_COMPLETED_INIT) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "COMPLETED_INIT", true);
    }
    if (status & HYPR_AUTH_COMPLETED) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "COMPLETED", true);
    }
    if (status & HYPR_AUTH_CANCELLED) {
        if (result != NULL) result = hypr_str_cat(result, ", ", true);
        result = hypr_str_cat(result, "CANCELED", true);
    }
    return result;
}

static void verbose_output(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    fflush(stdout);
    va_end(args);
}

static void debug_output(const char *format, ...)
{
    char *datetime = hypr_datetime_current();
    va_list args;
    va_start(args, format);
    fprintf(stderr, "%s: ", datetime);
    vfprintf(stderr, format, args);
    fprintf(stderr, "\n");
    va_end(args);
    hypr_datetime_free(datetime);
}
