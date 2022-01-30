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

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include <unistd.h>
#include "hypr_auth.h"
#include "hypr_auth_defaults.h"
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "util/hypr_uuid.h"

/*
 * Internal functions.
 */
static char               *create_initiate_authentication_url(const hypr_auth_config *);
static char               *create_initiate_authentication_post_data(const char *app_id, const char *username);
static char               *create_poll_authentication_response_url(const hypr_auth_config *, const char *request_id);
static hypr_auth_response *initiate_authentication(hypr_auth_request *, const char *username);
static hypr_auth_response *poll_authentication_response(hypr_auth_response *);

/*
 * Creates and returns a new hypr_auth_config object,
 * with default values (see: hypr_auth_defaults.h).
 */
hypr_auth_config *
hypr_create_auth_config()
{
    hypr_auth_config *config = (hypr_auth_config *) malloc(sizeof(hypr_auth_config));
    if (config == NULL) {
        return NULL;
    }
    config->base_url              = hypr_str_new(HYPR_AUTH_DEFAULT_BASE_URL);
    config->endpoint_path         = hypr_str_new(HYPR_AUTH_DEFAULT_ENDPOINT_PATH);
    config->app_id                = hypr_str_new(HYPR_AUTH_DEFAULT_APP_ID);
    config->access_token          = hypr_str_new(HYPR_AUTH_DEFAULT_ACCESS_TOKEN);
    config->poll_max_time_seconds =              HYPR_AUTH_DEFAULT_POLL_MAX_TIME_SECONDS;
    config->poll_interval_seconds =              HYPR_AUTH_DEFAULT_POLL_INTERVAL_SECONDS;
    config->timeout               =              HYPR_AUTH_DEFAULT_TIMEOUT_MS;
    //
    // See comments in hypr_auth.h about api_access_token.
    //
    config->api_access_token = NULL;
    return config;
}

/*
 * Frees memory associated with the given hypr_auth_config.
 */
void
hypr_free_auth_config(hypr_auth_config *config)
{
    if (config != NULL) {
        if (config->base_url != NULL) {
            hypr_str_free(config->base_url);
        }
        if (config->endpoint_path != NULL) {
            hypr_str_free(config->endpoint_path);
        }
        if (config->app_id != NULL) {
            hypr_str_free(config->app_id);
        }
        if (config->access_token != NULL) {
            hypr_str_free(config->access_token);
        }
        if (config->access_token != NULL) {
            hypr_str_free(config->api_access_token);
        }
        free((void *) config);
    }
}

/*
 * Creates and returns a hypr_auth_request object,
 * with a default hypr_auth_config (see: hypr_auth_defaults.h).
 * Caller responsible for freeing this via hypr_free_auth_request;
 * or via hypr_free_auth_response with the result of a hypr_auth
 * function call where this object was passed into that call.
 */
hypr_auth_request *
hypr_create_auth_request()
{
    return hypr_create_auth_request_with_config(hypr_create_auth_config());
}

/*
 * Creates and returns a hypr_auth_request object based on the given hypr_auth_config.
 * Caller responsible for freeing this via hypr_free_auth_request,
 * or via hypr_free_auth_response with the result of a hypr_auth
 * function call where this object was passed into that call.
 */
hypr_auth_request *
hypr_create_auth_request_with_config(hypr_auth_config *config)
{
    hypr_auth_request *request = (hypr_auth_request *) malloc(sizeof(hypr_auth_request));
    if (request == NULL) {
        return NULL;
    }
    request->config = config != NULL ? config : hypr_create_auth_config();
    if (request->config == NULL) {
        free((void *) request);
        return NULL;
    }
    request->on_authenticate           = NULL;
    request->on_authenticate_done      = NULL;
    request->on_authenticate_poll      = NULL;
    request->on_authenticate_poll_done = NULL;
    return request;
}

/*
 * Frees memory associated with the given hypr_auth_request.
 * This includes the freeing of memory associated with the
 * hypr_auth_config object referenced by the the 'config' data member.
 */
void
hypr_free_auth_request(hypr_auth_request *request)
{
    if (request != NULL) {
        hypr_free_auth_config(request->config);
        free((void *) request);
    }
}

/*
 * Frees memory associated with the given hypr_auth_response;
 * This includes the freeing of memory associated with the hypr_auth_request
 * object referenced by the 'request' data member, which in turn includes the
 * freeing of the memory associated with its referenced hypr_auth_config object.
 */
void
hypr_free_auth_response(hypr_auth_response *response)
{
    if (response != NULL) {
        hypr_free_auth_request(response->request);
        free((void *) response);
    }
}

/*
 * Performs the HYPR authentication process based on the given
 * hypr_auth_request and returns a new hypr_auth_response object.
 * This both initiates the authentication and does the response polling.
 * Caller responsible for freeing the returned hypr_auth_response via hypr_free_auth_response.
 */
hypr_auth_response *
hypr_auth(hypr_auth_request *request, const char *username)
{
    hypr_auth_response *response = initiate_authentication(request, username);
    response = poll_authentication_response(response);
    return response;
}

/*
 * Initiates the HYPR authentication via the HYPR authentication API
 * based on the given hypr_auth_request and returns a new hypr_auth_response object.
 * Caller responsible for freeing the returned hypr_auth_response via hypr_free_auth_response.
 */
static
hypr_auth_response *
initiate_authentication(hypr_auth_request *request, const char *username)
{
    if (request == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication request");
        return NULL;
    }

    const hypr_auth_config *config = request->config;

    if (config == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication config");
        return NULL;
    }

    if (hypr_str_blank(username)) {
        syslog(LOG_ERR, "HYPR: Bad authentication username");
        return NULL;
    }

    // Start authentication process.

    char *url       = create_initiate_authentication_url(config);
    char *post_data = create_initiate_authentication_post_data(config->app_id, username);

    if (request->on_authenticate != NULL) {
        (*request->on_authenticate)(url, post_data);
    }

    hypr_json response_json;

    int http_status = hypr_http_post_json
    (
        url,
        config->access_token,
        post_data,
        config->timeout >= 0 ? config->timeout : HYPR_AUTH_DEFAULT_TIMEOUT_MS,
        &response_json
    );

    hypr_str_free(post_data);

    if (request->on_authenticate_done != NULL) {
        (*request->on_authenticate_done)(http_status, hypr_json_format(response_json));
    }

    hypr_str_free(url);

    if (response_json == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication response");
        return NULL;
    }

    // Parse the authentication response.

    hypr_json   status_node      = hypr_json_get_object(response_json, "status");
    hypr_json   response_node    = hypr_json_get_object(response_json, "response");
    int         response_code    = hypr_json_get_int   (status_node,   "responseCode");
    const char *response_message = hypr_json_get_string(status_node,   "responseMessage");
    const char *request_id       = hypr_json_get_string(response_node, "requestId");

    if (response_code != 200) {
        syslog(LOG_ERR, "HYPR: Bad authentication response code (%d)", response_code);
        return NULL;
    }

    hypr_auth_response *response = (hypr_auth_response *) malloc(sizeof(hypr_auth_response));

    if (response == NULL) {
        syslog(LOG_ERR, "HYPR: Cannot create authentication response");
        return NULL;
    }

    response->request                       = request;
    response->request_id                    = request_id;
    response->authenticate_http_status      = http_status;
    response->authenticate_response_code    = response_code;
    response->authenticate_response_message = response_message;
    response->poll_http_status              = 0;
    response->poll_count                    = 0;
    response->status                        = HYPR_AUTH_STARTED;

    return response;
}

/*
 * Polls the HYPR authentication API for a response from the HYPR authentication
 * initiation request made by a/the previous call to the hypr_auth function.
 * Returns a hypr_auth_response object. Caller responsible for freeing
 * this hypr_auth_response object via hypr_free_auth_response.
 */
static
hypr_auth_response *
poll_authentication_response(hypr_auth_response *authentication_response)
{
    if (authentication_response == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication poll authenticate response");
        return NULL;
    }

    hypr_auth_config *config = authentication_response->request->config;

    if (config == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication poll config");
        return NULL;
    }

    hypr_auth_request *request = authentication_response->request; 

    if (request == NULL) {
        syslog(LOG_ERR, "HYPR: Bad authentication poll request");
        return NULL;
    }

    const char *request_id   = authentication_response->request_id;
    const char *access_token = config->access_token;
    char       *url          = create_poll_authentication_response_url(config, request_id);

    int poll_max_time_seconds = config->poll_max_time_seconds > 0 ? config->poll_max_time_seconds : HYPR_AUTH_DEFAULT_POLL_MAX_TIME_SECONDS;
    int poll_interval_seconds = config->poll_interval_seconds > 0 ? config->poll_interval_seconds : HYPR_AUTH_DEFAULT_POLL_INTERVAL_SECONDS;
    int timeout               = config->timeout;

    if (poll_interval_seconds > poll_max_time_seconds) {
        poll_interval_seconds = poll_max_time_seconds;
    }

    int poll_count = poll_max_time_seconds / poll_interval_seconds;

    int i;
    for (i = 0 ; i < poll_count ; i++) {

        sleep(poll_interval_seconds);

        if (request->on_authenticate_poll != NULL) {
            (*request->on_authenticate_poll)(url);
        }

        hypr_json response_json;

        int http_status = hypr_http_get_json(url, access_token, timeout, &response_json);

        authentication_response->poll_http_status = http_status;

        if (request->on_authenticate_poll_done != NULL) {
            (*request->on_authenticate_poll_done)(http_status, hypr_json_format(response_json));
        }

        if (response_json == NULL) {
            fprintf(stderr, "Invalid authentication poll response.");
            break;
        }

        hypr_json state_nodes = hypr_json_get_object(response_json, "state");
        int state_node_count = hypr_json_get_array_count(state_nodes);

        int j;
        for (j = 0 ; j < state_node_count ; j++) {
            hypr_json state_node  = hypr_json_get_array_element(state_nodes, j);
            const char *state_value = hypr_json_get_string(state_node, "value");
            if (hypr_str_equals(state_value, "REQUEST_SENT")) {
                authentication_response->status |= HYPR_AUTH_REQUEST_SENT;
            }
            else if (hypr_str_equals(state_value, "INITIATED")) {
                authentication_response->status |= HYPR_AUTH_INITIATED;
            }
            else if (hypr_str_equals(state_value, "INITIATED_RESPONSE")) {
                authentication_response->status |= HYPR_AUTH_INITIATED_RESPONSE;
            }
            else if (hypr_str_equals(state_value, "COMPLETED_INIT")) {
                authentication_response->status |= HYPR_AUTH_COMPLETED_INIT;
            }
            else if (hypr_str_equals(state_value, "COMPLETED")) {
                authentication_response->status |= HYPR_AUTH_COMPLETED;
            }
            else if (hypr_str_equals(state_value, "CANCELED") || hypr_str_equals(state_value, "CANCELLED")) {
                //
                // In this case there are (at least) two cases, where the 'message' is one of:
                //
                // - Canceled by User
                // - Authentication request timed out
                //
                // And looking for both CANCELED and CANCELLED because easy to imagine
                // someone fixing/chaning it as a typo (both spellings are acceptable).
                //
                authentication_response->status |= HYPR_AUTH_CANCELLED;
            }
        }

        hypr_json_free(response_json);

        if ((authentication_response->status & HYPR_AUTH_COMPLETED) ||
            (authentication_response->status & HYPR_AUTH_CANCELLED)) {
            authentication_response->poll_count = i + 1;
            break;
        }
    }

    hypr_str_free(url);

    return authentication_response;
}

/*
 * Creates and returns the POST data/string required for the initial
 * HYPR authentication request based on the given app ID and username.
 * Caller responsible for freeing the returned string.
 */
static char *
create_initiate_authentication_post_data(const char *app_id, const char *username)
{
    if (hypr_str_blank(app_id) || hypr_str_blank(username)) {
        return hypr_str_new("");
    }

    // Just in case someone mistakenly thinks the app_id needs slashes.

    char *trimmed_app_id = hypr_str_trim(app_id, '/', false);

    if (hypr_str_blank(trimmed_app_id)) {
        trimmed_app_id = hypr_str_copy(HYPR_AUTH_DEFAULT_APP_ID);
    }

    char *device_nonce  = hypr_nonce();
    char *service_hmac  = hypr_nonce();
    char *service_nonce = hypr_nonce();
    char *session_nonce = hypr_nonce();

    hypr_json request_json = hypr_json_new();
    hypr_json_add_object(request_json, "actionId",          NULL);
    hypr_json_add_object(request_json, "additionalDetails", hypr_json_new());
    hypr_json_add_string(request_json, "appId",             trimmed_app_id);
    hypr_json_add_object(request_json, "clientType",        NULL);
    hypr_json_add_object(request_json, "deviceId",          NULL);
    hypr_json_add_string(request_json, "deviceNonce",       device_nonce);
    hypr_json_add_string(request_json, "machine",           "WEB");
    hypr_json_add_object(request_json, "machineId",         NULL);
    hypr_json_add_string(request_json, "namedUser",         username);
    hypr_json_add_string(request_json, "serviceHmac",       service_hmac);
    hypr_json_add_string(request_json, "serviceNonce",      service_nonce);
    hypr_json_add_string(request_json, "sessionNonce",      session_nonce);

    hypr_str_free(device_nonce);
    hypr_str_free(service_hmac);
    hypr_str_free(service_nonce);
    hypr_str_free(session_nonce);
    hypr_str_free(trimmed_app_id);

    const char *json_string = hypr_json_format(request_json);

    if (json_string == NULL) {
        return hypr_str_new("");
    }

    char *post_data = hypr_str_copy(json_string);

    hypr_json_free(request_json);

    return post_data;
}

/*
 * Returns the URL to initialiate HYPR authentication
 * based on the given hypr_auth_config.
 * Caller responsible for freeing the returned string.
 */
static char *
create_initiate_authentication_url(const hypr_auth_config *config)
{
    if ((config == NULL) || hypr_str_blank(config->base_url) || hypr_str_blank(config->endpoint_path)) {
        return "";
    }

    // Ensure we don't have double slashes in the final URL.

    char *trimmed_base_url      = hypr_str_rtrim(config->base_url,      '/', false);
    char *trimmed_endpoint_path = hypr_str_trim (config->endpoint_path, '/', false);

    if (hypr_str_blank(trimmed_base_url)) {
        trimmed_base_url = hypr_str_copy(HYPR_AUTH_DEFAULT_BASE_URL);
    }
    if (hypr_str_blank(trimmed_endpoint_path)) {
        trimmed_endpoint_path = hypr_str_copy(HYPR_AUTH_DEFAULT_ENDPOINT_PATH);
    }

    char *url = hypr_str_new("%s/%s/requests", trimmed_base_url, trimmed_endpoint_path);

    hypr_str_free(trimmed_base_url);
    hypr_str_free(trimmed_endpoint_path);

    return url;
}

/*
 * Returns the URL to poll for a HYPR authentication response
 * based on the given hypr_auth_config and request ID.
 * Caller responsible for freeing the returned string.
 */
static char *
create_poll_authentication_response_url(const hypr_auth_config *config, const char *request_id)
{
    if ((config == NULL) || hypr_str_blank(config->base_url) || hypr_str_blank(config->endpoint_path) || hypr_str_blank(request_id)) {
        return hypr_str_new("");
    }

    // Ensure we don't have double slashes in the final URL.

    char *trimmed_base_url      = hypr_str_rtrim(config->base_url,      '/', false);
    char *trimmed_endpoint_path = hypr_str_trim (config->endpoint_path, '/', false);
    char *trimmed_request_id    = hypr_str_rtrim(request_id,            '/', false);

    if (hypr_str_blank(trimmed_base_url)) {
        trimmed_base_url = hypr_str_copy(HYPR_AUTH_DEFAULT_BASE_URL);
    }
    if (hypr_str_blank(trimmed_endpoint_path)) {
        trimmed_endpoint_path = hypr_str_copy(HYPR_AUTH_DEFAULT_ENDPOINT_PATH);
    }

    char *url = hypr_str_new("%s/%s/requests/%s", trimmed_base_url, trimmed_endpoint_path, trimmed_request_id);

    hypr_str_free(trimmed_base_url);
    hypr_str_free(trimmed_endpoint_path);
    hypr_str_free(trimmed_request_id);

    return url;
}
