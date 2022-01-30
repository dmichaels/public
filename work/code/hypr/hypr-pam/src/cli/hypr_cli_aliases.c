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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "auth/hypr_auth_defaults.h"
#include "auth/hypr_pam_config.h"

static const char *api_access_token = NULL;
static const char *base_url = NULL;
static const char *endpoint_path = "/rp/api/username";
static char       *base_url_and_endpoint = NULL;
static int         timeout = 0;
static bool        verbose = false;
static bool        debug = false;
static bool        dump_header = false;
static bool        show_curl = false;

static void  print_aliases_or_email_aliases(hypr_json response_json, const char *username, bool email_aliases, bool show_type);
static char *get_username_from_alias_or_email_alias(const char *alias, bool email_alias);
static bool  username_exists(const char *username);
static char *get_unexpected_response_message(const char *response);
static void  print_unexpected_response_message(const char *response);
static int   http_fetch(hypr_http_type verb, const char *url, const char *data, char **response, int expected_response_code);

static void usage()
{
    fprintf(stderr, "usage: hypr alias list   username [--email | --all]   [--base-url url] [--access-token token] [--debug] [--debug] [--curl]\n");
    fprintf(stderr, "usage: hypr alias set    username alias               [--base-url url] [--access-token token] [--debug] [--debug] [--curl]\n");
    fprintf(stderr, "usage: hypr alias set    username email-alias --email [--base-url url] [--access-token token] [--debug] [--debug] [--curl]\n");
    fprintf(stderr, "usage: hypr alias delete username                     [--base-url url] [--access-token token] [--debug] [--debug] [--curl]\n");
    exit(1);
}

/*
 * Utility CLI to interface with the username aliases API.
 * https://why.atlassian.net/wiki/spaces/SE/pages/1862664193/FSD+User+Aliases
 */
int hypr_cli_aliases(int argc, char *argv[], const char *arg)
{
    const int ACTION_NULL = 0, ACTION_LIST = 1, ACTION_SET = 2, ACTION_DELETE = 3;
    int action = ACTION_NULL;
    const char *config_file = NULL;
    const char *username = NULL;
    bool email_aliases = false;
    bool aliases_and_email_aliases = false;
    hypr_json aliases_to_set = NULL;
    bool check_for_aliases_which_are_already_defined = true;
    bool check_for_aliases_which_already_exist_as_username = true;
    int istart = 3;

    if (argc < 2) {
        usage();
    }

    if (hypr_str_iequals(argv[2], "list") ||
        hypr_str_iequals(argv[2], "get")) {
        action = ACTION_LIST;
    }
    else if (hypr_str_iequals(argv[2], "set") ||
             hypr_str_iequals(argv[2], "add") ||
             hypr_str_iequals(argv[2], "put") ||
             hypr_str_iequals(argv[2], "patch")) {
        action = ACTION_SET;
    }
    else if (hypr_str_iequals(argv[2], "delete") ||
             hypr_str_iequals(argv[2], "remove")) {
        action = ACTION_DELETE;
    }
    if (action == ACTION_NULL) {
        action = ACTION_LIST;
        istart = 2;
    }

    int i;
    for (i = istart ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "--config") || hypr_str_iequals(argv[i], "--conf")) {
            if (++i >= argc) usage();
            config_file = argv[i];
            if (access(config_file, F_OK) != 0) {
                fprintf(stderr, "hypr aliases: Cannot open configuration file: %s\n", config_file);
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
                 hypr_str_iequals(argv[i], "-endpoint")) {
            if (++i >= argc) usage();
            endpoint_path = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--access-token") ||
                 hypr_str_iequals(argv[i], "--accesstoken") ||
                 hypr_str_iequals(argv[i], "--api-access-token") ||
                 hypr_str_iequals(argv[i], "--apiaccesstoken") ||
                 hypr_str_iequals(argv[i], "-accesstoken")) {
            if (++i >= argc) usage();
            api_access_token = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--username") ||
                 hypr_str_iequals(argv[i], "-username")) {
            if (++i >= argc) usage();
            username = argv[i];
        }
        else if ((hypr_str_iequals(argv[i], "--emails") || hypr_str_iequals(argv[i], "--email"))) {
            email_aliases = true;
        }
        else if (hypr_str_iequals(argv[i], "--all")) {
            aliases_and_email_aliases = true;
        }
        else if (hypr_str_iequals(argv[i], "--verbose")) {
            verbose = true;
        }
        else if (hypr_str_iequals(argv[i], "--debug")) {
            debug = true;
        }
        else if (hypr_str_iequals(argv[i], "--dump-header") ||
                 hypr_str_iequals(argv[i], "--dumpheader")) {
            dump_header = true;
        }
        else if (hypr_str_iequals(argv[i], "--show-curl") ||
                 hypr_str_iequals(argv[i], "--showcurl") ||
                 hypr_str_iequals(argv[i], "--curl")) {
            show_curl = true;
        }
        else if (hypr_str_iequals(argv[i], "--nocheck")) {
            //
            // By default when adding aliases we check for alias names which
            // already exist as usernames; this involves and API call for each alias.
            // Turn this off with the --nocheck option.
            //
            if (action != ACTION_SET) {
                usage();
            }
            check_for_aliases_which_are_already_defined = false;
            check_for_aliases_which_already_exist_as_username = false;
        }
        else if (hypr_str_blank(username)) {
            username = argv[i];
        }
        else if (action == ACTION_SET) {
            const char *alias = argv[i];
            if (aliases_to_set == NULL) {
                aliases_to_set = hypr_json_new_array();
            }
            hypr_json_add_string_element(aliases_to_set, alias);
        }
        else {
            usage();
        }
    }

    if (hypr_str_blank(username)) {
        usage();
    }
    if (aliases_and_email_aliases && (action == ACTION_SET)) {
        usage();
    }

    if (hypr_str_blank(config_file)) {
        config_file = HYPR_PAM_DEFAULT_CONFIG_FILE;
    }

    // Read the HYPR Linux PAM configuration file (pam_hypr.conf).

    hypr_pam_config *pam_config = hypr_pam_config_read(config_file, NULL, NULL, NULL);
    if (pam_config != NULL) {
        if (hypr_str_blank(api_access_token) && (pam_config->auth_config != NULL)) {
            api_access_token = pam_config->auth_config->api_access_token;
        }
        if (hypr_str_blank(base_url) && (pam_config->auth_config != NULL)) {
            base_url = pam_config->auth_config->base_url;
        }
        if (hypr_str_blank(endpoint_path) && (pam_config->auth_config != NULL)) {
            endpoint_path = pam_config->auth_config->endpoint_path;
        }
    }

    if (debug) {
        printf("HYPR USERNAME %sALIASES API UTILITY\n", email_aliases ? "(EMAIL) " : "");
    }
    if (debug) {
        printf("CONFIG FILE: %s (%s)\n", config_file, pam_config != NULL ? "OK" : "not found");
    }

    if (hypr_str_blank(api_access_token)) {
        fprintf(stderr, "hypr aliases: No api-access-token found.\n");
        usage();
    }
    else if (hypr_str_iequals(api_access_token, "REDACTED") || hypr_str_iequals(api_access_token, "<REDACTED>")) {
        fprintf(stderr, "hypr aliases: No api-access-token yet set (%s).\n", api_access_token);
        usage();
    }
    else if (hypr_str_blank(username)) {
        fprintf(stderr, "hypr aliases: No username found.\n");
        usage();
    }

    if (debug) {
        int api_access_token_length = strlen(api_access_token);
        printf("API ACCESS TOKEN: %c*******%c\n", api_access_token[0], api_access_token[api_access_token_length -1]);
        printf("BASE URL: %s\n", base_url);
        printf("ENDPOINT PATH: %s\n", endpoint_path);
        printf("USERNAME: %s\n", username);
        printf("LIBCURL: %s%s\n", hypr_http_static_libcurl() ? "static" : "dynamic",
                                  hypr_http_static_libcurl() ? "" : (hypr_http_libcurl_okay() ? " (OK)" : "(not loadable)")); 
    }

    // E.g. https://awsdevelop1.biometric.software/rp/api/username
    //
    base_url_and_endpoint =
        hypr_str_new("%s/%s", hypr_str_rtrim(base_url, '/', false), hypr_str_trim(endpoint_path, '/', false));

    if (action == ACTION_SET) {
        if (aliases_to_set == NULL) {
            usage();
        }
        //
        // Call the HYPR username aliases API to add the given aliases for the given username.
        // For example:
        //
        //  curl -L -X PATCH -H 'Content-Type: application/json'
        //                   -H 'Authorization: Bearer 2541c819-67cc-d3e8-5cf2-dc86f3e128b2'
        //                   -d '["some-alias-name","another-alias-name"]'
        //             https://awsdevelop1.biometric.software/rp/api/username/some-hypr-username/alias
        //
        // If username found and alias(s) added then returns HTTP status code 200 and the given payload.
        // If username not found then returns HTTP status code 404.
        // If malformed request then returns HTTP status code 400.
        //
        if (check_for_aliases_which_are_already_defined) {
            //
            // Check if any of the specified aliases is already
            // defined as an alias for another (or this) username.
            //
            // The API call will fail entirely if any of the given aliases are already defined
            // as an alias for another user (if already defined as this username then fine/silent).
            //
            hypr_json aliases_to_set_checked = hypr_json_new_array();
            int naliases_to_set = hypr_json_get_array_count(aliases_to_set); int i;
            for (i = 0 ; i < naliases_to_set ; i++) {
                const char *alias_to_set = hypr_json_get_array_string_element(aliases_to_set, i);
                char *username_for_alias_to_set = get_username_from_alias_or_email_alias(alias_to_set, email_aliases);
                if (!hypr_str_blank(username_for_alias_to_set)) {
                    if (hypr_str_iequals(username_for_alias_to_set, username)) {
                        fprintf(stderr, "WARNING: Alias is already defined for this username: %s (ignoring)\n", alias_to_set);
                    }
                    else {
                        fprintf(stderr, "WARNING: Alias is already defined for another username: %s -> %s (ignoring)\n", username_for_alias_to_set, alias_to_set);
                    }
                }
                else {
                    hypr_json_add_string_element(aliases_to_set_checked, alias_to_set);
                }
            }
            hypr_json_free(aliases_to_set);
            aliases_to_set = aliases_to_set_checked;
        }
        if (check_for_aliases_which_already_exist_as_username) {
            //
            // Check if any of the specified aliases is already defined as a (or this) username.
            //
            // The API allows setting an alias which is already defined as a username.
            // This could introduce some inconsistency, e.g. if we have usernames A and B,
            // it is possible for B define the alias A, and this could effectively hijack
            // authentications for A to use B, i.e. the real A would not be able to authenticate.
            //
            hypr_json aliases_to_set_checked = hypr_json_new_array();
            int naliases_to_set = hypr_json_get_array_count(aliases_to_set); int i;
            for (i = 0 ; i < naliases_to_set ; i++) {
                const char *alias_to_set = hypr_json_get_array_string_element(aliases_to_set, i);
                if (hypr_str_iequals(alias_to_set, username)) {
                    fprintf(stderr, "WARNING: Alias is already defined as this username: %s (ignoring)\n", alias_to_set);
                }
                else if (username_exists(alias_to_set)) {
                    fprintf(stderr, "WARNING: Alias is already defined as another username: %s (ignoring)\n", alias_to_set);
                }
                else {
                    hypr_json_add_string_element(aliases_to_set_checked, alias_to_set);
                }
            }
            hypr_json_free(aliases_to_set);
            aliases_to_set = aliases_to_set_checked;
        }
        char *url = hypr_str_new("%s/%s/%s", base_url_and_endpoint, username, email_aliases ? "email" : "alias");
        char *response = NULL; const int expected_response_code = 200;
        int response_code =
            http_fetch(HYPR_HTTP_PATCH, url, hypr_json_noformat(aliases_to_set), &response, expected_response_code);
        hypr_str_free(url);
        if (response_code < 0) {
            exit(2);
        }
        else if (response_code == 404) {
            printf("Username not found: %s\n", username); 
            exit(3);
        }
        else if (response_code != expected_response_code) {
            if (verbose) {
                //
                // See if the given one or more of the specified aliases is already exists,
                // by calling the API /rp/api/username/alias/:alias or /rp/api/username/email/:email
                //
                int naliases_to_set = hypr_json_get_array_count(aliases_to_set);
                int i;
                for (i = 0 ; i < naliases_to_set ; i++) {
                    const char *alias_to_set = hypr_json_get_array_string_element(aliases_to_set, i);
                    if (!hypr_str_blank(aliases_to_set)) {
                        char *username_for_alias_to_set = get_username_from_alias_or_email_alias(alias_to_set, email_aliases);
                        if (!hypr_str_blank(username_for_alias_to_set)) {
                            printf("%s already exists: %s -> %s\n", email_aliases ? "Email alias" : "Alias", username_for_alias_to_set, alias_to_set);
                            hypr_str_free(username_for_alias_to_set);
                        }
                    }
                }
            }
        }
        else {
            hypr_json response_json = hypr_json_parse(response);
            if (response_json == NULL) {
                printf("Cannot parse result as JSON (use --debug to see response).\n");
                exit(4);
            }
            int response_json_alias_count = hypr_json_get_array_count(response_json);
            if (debug) {
                 printf("ALIASES ADDED (%d) FOR USERNAME: %s\n", response_json_alias_count, username);
            }
        }
    }
    else if (action == ACTION_DELETE) {
        //
        // Call the HYPR username aliases API to delete all aliases for the given username.
        // For example:
        //
        //  curl -L -X DELETE -H 'Content-Type: application/json'
        //                    -H 'Authorization: Bearer 2541c819-67cc-d3e8-5cf2-dc86f3e128b2'
        //             https://awsdevelop1.biometric.software/rp/api/username/some-hypr-username/alias
        //
        // If username found and (any/all) associated aliases deleted then returns HTTP status code 204.
        // If username not found then returns HTTP status code 404 (actually, returns 204).
        //
        if (debug) {
            if (aliases_and_email_aliases) {
                printf("DELETING ALL ALIASES AND EMAIL ALIASES FOR USERNAME: %s\n", username);
            }
            else {
                printf("DELETING ALL %sALIASES FOR USERNAME: %s\n", email_aliases ? "EMAIL " : "", username);
            }
        }
        //
        // TODO
        // Allow --all for delete.
        //
        char *url = hypr_str_new("%s/%s/%s", base_url_and_endpoint, username, email_aliases ? "email" : "alias");
        const int expected_response_code = 204;
        int response_code = http_fetch(HYPR_HTTP_DELETE, url, NULL, NULL, expected_response_code);
        hypr_str_free(url);
        if (response_code < 0) {
            exit(5);
        }
        else if (response_code == 404) {
            printf("Username not found: %s\n", username); 
            exit(6);
        }
        else if (response_code != expected_response_code) {
            exit(7);
        }
        if (aliases_and_email_aliases) {
            char *url = hypr_str_new("%s/%s/%s", base_url_and_endpoint, username, !email_aliases ? "email" : "alias", true);
            const int expected_response_code = 204;
            int response_code = http_fetch(HYPR_HTTP_DELETE, url, NULL, NULL, expected_response_code);
            hypr_str_free(url);
            if (response_code < 0) {
                exit(8);
            }
            else if (response_code == 404) {
                printf("Username not found: %s\n", username); 
                exit(9);
            }
            else if (response_code != expected_response_code) {
                exit(10);
            }
        }
    }

    if (debug && (action != ACTION_LIST)) {
         printf("LISTING ALIASES FOR: %s\n", username);
    }

    // Do the GET even if (after) doing a set or a delete.

    // Call the HYPR username aliases API to get/list all aliases
    // and/or email aliases for the given username. For example:
    //
    //  curl -L -X GET -H 'Content-Type: application/json'
    //                 -H 'Authorization: Bearer 2541c819-67cc-d3e8-5cf2-dc86f3e128b2'
    //             https://awsdevelop1.biometric.software/rp/api/username/some-hypr-username
    //
    // If username found then returns HTTP status code 200 and JSON like this:
    //
    //    { "alias": ["some-alias","another-alias"],
    //        "email": [
    //            "some-email-address",
    //            "another-email-address" ],
    //        "username": "hypr-username-email-address" }
    //
    // If username not found returns HTTP status code 404.

    char *url = hypr_str_new("%s/%s", base_url_and_endpoint, username);
    char *response = NULL; int expected_response_code = 200;
    int response_code = http_fetch(HYPR_HTTP_GET, url, NULL, &response, expected_response_code);
    if (response_code <= 0) {
        exit(11);
    }
    else if (response_code == 404) {
        printf("Username not found: %s\n", username);
        char *username_from_alias_or_email_alias = get_username_from_alias_or_email_alias(username, email_aliases);
        if (!hypr_str_blank(username_from_alias_or_email_alias)) {
            printf("This is an %salias for username: %s\n", email_aliases ? "email " : "", username_from_alias_or_email_alias); 
        }
        if (aliases_and_email_aliases) {
            username_from_alias_or_email_alias = get_username_from_alias_or_email_alias(username, !email_aliases);
            if (!hypr_str_blank(username_from_alias_or_email_alias)) {
                printf("This is an %salias for username: %s\n", !email_aliases ? "email " : "", username_from_alias_or_email_alias); 
            }
        }
        hypr_str_free(username_from_alias_or_email_alias);
        exit(12);
    }
    else if (response_code != expected_response_code) {
        exit(13);
    }

    hypr_json response_json = hypr_json_parse(response);

    if (response_json == NULL) {
        printf("Cannot parse result as JSON (use --debug to see response).\n");
        exit(14);
    }

    const char *response_username = hypr_json_get_string(response_json, "username");

    if (!hypr_str_equals(username, response_username)) {
        printf("Returned username (%s) does not match given username: %s\n", response_username, username);
    }

    if (aliases_and_email_aliases) {
        print_aliases_or_email_aliases(response_json, username, false, true);
        print_aliases_or_email_aliases(response_json, username, true,  true);
    }
    else {
        print_aliases_or_email_aliases(response_json, username, email_aliases, false);
    }

    hypr_str_free(url);
    hypr_str_free(base_url_and_endpoint);

    return 0;
}

/*
 * Returns true iff the given username exists at all.
 */
static bool username_exists(const char *username)
{
    char *url = hypr_str_new("%s/%s", base_url_and_endpoint, username);
    char *response;
    int response_code = http_fetch(HYPR_HTTP_GET, url, NULL, &response, 0);
    hypr_str_free(url);
    hypr_str_free(response);
    return response_code == 200;
}

/*
 * Returns the the username which is defined as the given alias or email alias name,
 * or NULL if no username exists for the given alias or email alias.
 * Caller responsible for freeing any returned string.
 * NOTE: Uses new endpoints as of HYPR Server 6.15.0 (proposed in HYP-8820).
 */
static char *get_username_from_alias_or_email_alias(const char *alias_or_email_alias, bool email_alias)
{
    // Uses NEW endpoints first released in HYPR Server 6.15.0 (August 2021 / HYP-8820):
    //
    //   GET /rp/api/username?alias=alias
    //   GET /rp/api/username?email=email-alias
    //
    // If the endpoint itself does NOT (yet) exist then the response is 404, just
    // as-if the endpoint DOES exist and/but the given (email) alias is not found.
    //
    char *url = hypr_str_new("%s?%s=%s", base_url_and_endpoint, email_alias ? "email" : "alias", alias_or_email_alias);
    char *response;
    int response_code = http_fetch(HYPR_HTTP_GET, url, NULL, &response, 0);
    hypr_str_free(url);
    if (response_code == 200) {
        hypr_json response_json = hypr_json_parse(response);
        if (response_json != NULL) {
            hypr_json response_json_first_element = hypr_json_get_array_element(response_json, 0);
            if (response_json_first_element != NULL) {
                const char *username = hypr_json_get_string(response_json_first_element, "username");
                if (!hypr_str_blank(username)) {
                    hypr_str_free(response);
                    response = hypr_str_copy(username);
                    hypr_json_free(response_json);
                    return response;
                }
            }
            hypr_json_free(response_json);
        }
    }
    else if ((response_code == 404) && (verbose || debug)) {
        //
        // Distinguish between 404 due to (email) alias not found vs. the endpoint
        // itself is not found (i.e. not yet supported), in which (the latter) case,
        // the "error" property in the returned JSON contains "Not Found".
        //
        if (!hypr_str_blank(response)) {
            hypr_json response_json = hypr_json_parse(response);
            if (response_json != NULL) {
                const char *response_error = hypr_json_get_string(response_json, "error");
                if (hypr_str_iequals(response_error, "Not Found")) {
                    url = hypr_str_new("%s?%s=%s", base_url_and_endpoint, email_alias ? "email" : "alias", email_alias ? "email" : "alias");
                    fprintf(stderr, "NOTE: Server does not yet support endpoint: %s\n", url);
                    hypr_str_free(url);
                }
                hypr_json_free(response_json);
            }
        }
    }
    hypr_str_free(response);
    return NULL;
}

static void print_aliases_or_email_aliases(hypr_json response_json, const char *username, bool email_aliases, bool show_type)
{
    hypr_json response_array = hypr_json_get_object(response_json, email_aliases ? "email" : "alias");
    if (response_array != NULL) {
        int response_array_count = hypr_json_get_array_count(response_array);
        if (debug) {
            printf("RESPONSE %sALIAS COUNT: %d\n", email_aliases ? "EMAIL " : "", response_array_count);
        }
        int n;
        for (n = 0 ; n < response_array_count ; n++) {
        const char *value = hypr_json_get_array_string_element(response_array, n);
            if (!hypr_str_blank(value)) {
                if (verbose) {
                    printf("%s: %s%s\n", username, value, show_type ? (email_aliases ? " (email alias)" : " (alias)") : "");
                }
                else {
                    printf("%s%s\n", value, show_type ? (email_aliases ? " (email alias)" : " (alias)") : "");
                }
            }
        }
    }
}

static char *get_unexpected_response_message(const char *response)
{
    char *message = NULL;
    if (!hypr_str_blank(response)) {
        hypr_json response_json = hypr_json_parse(response);
        if (response_json != NULL) {
            const char *response_title = hypr_json_get_string(response_json, "title");
            if (!hypr_str_blank(response_title)) {
                message = hypr_str_copy(response_title);
            }
            hypr_json_free(response_json);
        }
    }
    return message;
}

static void print_unexpected_response_message(const char *response)
{
    char *unexpected_response_message = get_unexpected_response_message(response);
    if (!hypr_str_blank(unexpected_response_message )) {
        printf("Unexpected response message: %s\n", unexpected_response_message);
        hypr_str_free(unexpected_response_message);
    }
}

/**
 * Internal http_fetch function wrapping hypr_http_fetch with debugging output et cetera.
 * Global file variables used: api_access_token, timeout, debug, dump_header, show_curl.
 */
static int http_fetch(hypr_http_type verb, const char *url, const char *data, char **response, int expected_response_code)
{
    char *http_response, *http_response_header;

    if (debug) {
        printf("HTTP REQUEST: %s %s\n", hypr_http_type_name(verb), url);
        printf("HTTP AUTHORIZATION: %s\n", api_access_token);
    }
    if (show_curl) {
        if (!hypr_str_blank(data)) {
            printf("curl -L -X %s -H '%s' -H 'Authorization: Bearer %s' -d '%s' %s\n",
                hypr_http_type_name(verb), HYPR_HTTP_CONTENT_TYPE_HEADER, api_access_token, data, url);
        }
        else {
            printf("curl -L -X %s -H '%s' -H 'Authorization: Bearer %s' %s\n",
                hypr_http_type_name(verb), HYPR_HTTP_CONTENT_TYPE_HEADER, api_access_token, url);
        }
    }

    int http_response_code = hypr_http_fetch(verb, url, api_access_token, data, timeout, &http_response, &http_response_header);

    if (debug) {
        printf("HTTP RESPONSE CODE: %d\n", http_response_code);
    }
    if (debug && dump_header) {
        printf("HTTP RESPONSE HEADER:\n%s\n", http_response_header);
    }
    if (debug) {
        int http_response_length = !hypr_str_blank(http_response) ? (int) strlen(http_response) : 0;
        printf("HTTP RESPONSE SIZE: %d bytes\n", http_response_length);
        printf("HTTP RESPONSE:%s%s\n", http_response_length > 40 ? "\n" : " ", http_response);
    }
    hypr_str_free(http_response_header);
    if (response != NULL) {
        *response = http_response;
    }
    if (http_response_code <= 0) {
        fprintf(stderr, "Invalid HTTP response code: %d\n", http_response_code);
        return http_response_code;
    }
    else if (http_response_code == 401) {
        int api_access_token_length = strlen(api_access_token);
        fprintf(stderr, "Unauthorized access for API token: %c*******%c\n", api_access_token[0], api_access_token[api_access_token_length -1]);
        return -http_response_code;
    }
    else if ((expected_response_code > 0) && (http_response_code != expected_response_code)) {
        if (http_response_code != 404) {
            //
            // Caller expected to specifically handle 404 error messaging.
            //
            fprintf(stderr, "Unexpected HTTP response code: %d\n", http_response_code);
            print_unexpected_response_message(http_response);
        }
    }

    return http_response_code;
}
