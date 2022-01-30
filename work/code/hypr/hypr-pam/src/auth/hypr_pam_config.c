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

#include <grp.h>
#include <pwd.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "hypr_pam_config.h"
#include "hypr_auth.h"
#include "hypr_auth_defaults.h"
#include "util/hypr_str.h"

/*
 * Reads given file (e.g. /etc/security/pam_hypr.conf) & looks for lines like:
 *
 *     access_token      HYPR_ACCESS_TOKEN
 *     api_base_url      https://bojandev.gethypr.com
 *     api_auth_endpoint /rp/api/oob/client/authentication
 *     app_id            fIDO2Local
 *     timeout           30000
 *     on_unmapped_login ignore | fail
 *     disabled          false
 *     disabled          login LINUX_LOGIN_NAME
 *     disabled          group LINUX_GROUP_NAME
 *     disabled          command LINUX_COMMAND_NAME # e.g. sudo
 *
 *     username LINUX_LOGIN_NAME HYPR_USERNAME
 *     username LINUX_LOGIN_NAME HYPR_USERNAME
 *     username LINUX_LOGIN_NAME HYPR_USERNAME
 *
 * Returns a new hypr_pam_config object (see pam_hypr_config.h) with the config
 * info in the specified pam_hypr.conf file. This includes the (last) HYPR username
 * associated with given Linux login name (temporary until we have a server-side
 * solution for this); and an indication of whether or not HYPR authentication is
 * disabled due to the given login or its group being disabled (and including the
 * group name in the latter case), or due to the invoking command being disabled.
 *
 * This module is here rather than in src/pam mostly so we can easily use it from
 * the src/cli/hypr module/utility.
 */
hypr_pam_config *
hypr_pam_config_read(const char *config_file, const char *login, const char *command, const char *remote_host)
{
    if (hypr_str_blank(config_file)) {
        config_file = "/etc/security/pam_hypr.conf";
    }

    FILE *f = fopen(config_file, "r");

    if (f == NULL) {
        return NULL;
    }

    hypr_pam_config *config = (hypr_pam_config *) malloc(sizeof(hypr_pam_config));

    if (config == NULL) {
        fclose(f);
        return NULL;
    }

    config->auth_config = hypr_create_auth_config();

    if (config->auth_config == NULL) {
        fclose(f);
        return NULL;
    }

    config->username = NULL;
    config->disabled = HYPR_PAM_CONFIG_ENABLED;
    config->disabled_group = NULL;
    config->login_prompt = NULL;
    config->login_message = NULL;
    config->on_unmapped_login = NULL;
    config->username_mappings = 0;
    config->libcurl_path = NULL;

    // Initialize these to NULL as they (may) have default values from
    // hypr_create_auth_config; we want to make sure everything comes
    // from our installed/specified configuration file.

    config->auth_config->base_url         = NULL;
    config->auth_config->endpoint_path    = NULL;
    config->auth_config->app_id           = NULL;
    config->auth_config->access_token     = NULL;
    config->auth_config->api_access_token = NULL;

    // Parse the configuration file.

    struct passwd *pwd = NULL;
    gid_t groups[1024 * 2];
    int ngroups = sizeof(groups) / sizeof(gid_t);
    bool grouplist = false;

    char line[1024 * 4], first[sizeof(line)], second[sizeof(line)], third[sizeof(line)];
    while (fgets(line, sizeof(line), f) != NULL) {
        int n = sscanf(line, "%s %s %s", first, second, third);
        if (hypr_str_iequals(first, "login_message")) {
            if (sscanf(line, "%s %[^\n]", first, second) == 2) {
                config->login_message = hypr_str_copy(second);
            }
        }
        else if (hypr_str_iequals(first, "login_prompt")) {
            if (sscanf(line, "%s %[^\n]", first, second) == 2) {
                config->login_prompt = hypr_str_copy(second);
            }
        }
        else if ((n == 2) && !hypr_str_blank(second)) {
            if (hypr_str_iequals(first, "api_base_url")) {
                if (second[0] != '\0') {
                    config->auth_config->base_url = hypr_str_copy(second);
                }
            }
            else if (hypr_str_iequals(first, "api_auth_endpoint")) {
                if (second[0] != '\0') {
                    config->auth_config->endpoint_path = hypr_str_copy(second);
                }
            }
            else if (hypr_str_iequals(first, "app_id")) {
                if (second[0] != '\0') {
                    config->auth_config->app_id = hypr_str_copy(second);
                }
            }
            else if (hypr_str_iequals(first, "access_token") ||
                     hypr_str_iequals(first, "auth_access_token")) {
                if (second[0] != '\0') {
                    config->auth_config->access_token = hypr_str_copy(second);
                }
            }
            else if (hypr_str_iequals(first, "api_access_token")) {
                //
                // The API access-token is used for non-auth related API,
                // most notably, for our purposes, the username 'aliases' endpoints.
                //
                if (second[0] != '\0') {
                    config->auth_config->api_access_token = hypr_str_copy(second);
                }
            }
            else if (hypr_str_iequals(first, "poll_max_time_seconds")) {
                if (second[0] != '\0') {
                    config->auth_config->poll_max_time_seconds = atoi(second);
                }
            }
            else if (hypr_str_iequals(first, "poll_interval_seconds")) {
                if (second[0] != '\0') {
                    config->auth_config->poll_interval_seconds = atoi(second);
                }
            }
            else if (hypr_str_iequals(first, "timeout_milliseconds") || hypr_str_iequals(first, "timeout")) {
                //
                // Note that the timeout is in milliseconds; should be be
                // greater-than-to zero, otherwise don't set at all, i.e.
                // libcurl default. HYPR default in hypr_auth_defaults.h.
                //
                config->auth_config->timeout = atoi(second);
            }
            else if (hypr_str_iequals(first, "libcurl")) {
                //
                // If using dynamic libcurl library loading, i.e. compiled with
                // HYPR_HTTP_STATIC_LIBCURL=0, which is the default, then allow the
                // actual path of the libcurl.so file (globally) to be specified explicitly.
                //
                config->libcurl_path = hypr_str_copy(second);
            }
            else if (hypr_str_iequals(first, "disabled")) {
                if (hypr_str_iequals(second, "true") ||
                    hypr_str_iequals(second, "yes") ||
                    hypr_str_iequals(second, "1")) {
                    config->disabled |= HYPR_PAM_CONFIG_DISABLED;
                }
            }
            else if (hypr_str_iequals(first, "on_unmapped_login")) {
                if (hypr_str_iequals(second, "ignore")) {
                    config->on_unmapped_login = "ignore";
                }
                else if (hypr_str_iequals(second, "fail") || hypr_str_iequals(second, "error")) {
                    config->on_unmapped_login = "fail";
                }
            }
        }
        else if ((n == 3) && !hypr_str_blank(third)) {
            if (hypr_str_iequals(first, "username")) {
                if (hypr_str_equals(login, second)) {
                    config->username = hypr_str_copy(third);
                }
                if (!hypr_str_blank(second) && !hypr_str_blank(third)) {
                    config->username_mappings++;
                }
            }
            else if (hypr_str_iequals(first, "disabled")) {
                if (hypr_str_iequals(second, "login")) {
                    if (hypr_str_iequals(third, login)) {
                        config->disabled |= HYPR_PAM_CONFIG_DISABLED_LOGIN;
                    }
                }
                else if (hypr_str_iequals(second, "group")) {
                    if (config->disabled & HYPR_PAM_CONFIG_DISABLED_GROUP) {
                        //
                        // Already found we are disabled for a login group.
                        //
                        continue;
                    }
                    //
                    // Take care to only make passwd/group calls once.
                    // 
                    if (pwd == NULL) {
                        pwd = getpwnam(login);
                    }
                    if (!grouplist && (pwd != NULL)) {
                        if (getgrouplist(login, pwd->pw_gid, groups, &ngroups) == -1) {
                            ngroups = -1;
                        }
                        grouplist = true;
                    }
                    if (ngroups > 0) {
                        int i;
                        for (i = 0; i < ngroups; i++) {
                            struct group *grp = getgrgid(groups[i]);
                            if (grp != NULL) {
                                if (hypr_str_equals(grp->gr_name, third)) {
                                    config->disabled |= HYPR_PAM_CONFIG_DISABLED_GROUP;
                                    config->disabled_group = hypr_str_copy(third);
                                }
                            }
                        }
                    }
                }
                else if (hypr_str_iequals(second, "command")) {
                    if (hypr_str_iequals(third, command)) {
                        config->disabled |= HYPR_PAM_CONFIG_DISABLED_COMMAND;
                    }
                }
            }
        }
    }

    fclose(f);

    if (config->auth_config->poll_max_time_seconds <= 0) {
        config->auth_config->poll_max_time_seconds = HYPR_AUTH_DEFAULT_POLL_MAX_TIME_SECONDS;
    }
    if (config->auth_config->poll_interval_seconds <= 0) {
        config->auth_config->poll_interval_seconds = HYPR_AUTH_DEFAULT_POLL_INTERVAL_SECONDS;
    }
    if (config->auth_config->poll_interval_seconds > config->auth_config->poll_max_time_seconds) {
        config->auth_config->poll_interval_seconds = config->auth_config->poll_max_time_seconds;
    }
    if (config->login_prompt != NULL) {
        config->login_prompt = hypr_str_replace(config->login_prompt, "%login", login, true);
        config->login_prompt = hypr_str_replace(config->login_prompt, "%username", config->username, true);
        config->login_prompt = hypr_str_replace(config->login_prompt, "%remotehost", remote_host, true);
        config->login_prompt = hypr_str_replace(config->login_prompt, "%command", command, true);
        config->login_prompt = hypr_str_cat(config->login_prompt, " ", true);
    }
    if (config->login_message != NULL) {
        config->login_message = hypr_str_replace(config->login_message, "%login", login, true);
        config->login_message = hypr_str_replace(config->login_message, "%username", config->username, true);
        config->login_message = hypr_str_replace(config->login_message, "%remotehost", remote_host, true);
        config->login_message = hypr_str_replace(config->login_message, "%command", command, true);
    }

    return config;
}

void hypr_pam_config_free(hypr_pam_config *config)
{
    if (config != NULL) {
        hypr_free_auth_config(config->auth_config);
        if (config->username != NULL) {
            free((void *) config->username);
        }
        if (config->disabled_group != NULL) {
            free((void *) config->disabled_group);
        }
        if (config->login_message != NULL) {
            free((void *) config->login_message);
        }
        if (config->libcurl_path != NULL) {
            free((void *) config->libcurl_path);
        }
    }
}
