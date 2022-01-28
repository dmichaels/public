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

#ifndef __HYPR_AUTH_H
#define __HYPR_AUTH_H

#include <stdbool.h>
#include "util/hypr_json.h"

typedef struct {
    char *base_url;
    char *endpoint_path;
    char *app_id;
    char *access_token;
    int   poll_max_time_seconds;
    int   poll_interval_seconds;
    int   timeout;
    //
    // This API access-token is not authentication related.
    // It is currently used for programmatic access to the
    // username aliases APIs, access to which is supported
    // by the CLI utility (hypr) associated with this code
    // base for testing/troubleshooting purposes. It can be
    // set in the pam_hypr.config configuration file with
    // the authenticaiton access-token.
    //
    char *api_access_token;
} hypr_auth_config;

typedef struct {
    hypr_auth_config *config;
    void            (*on_authenticate)(const char *url, const char *post_data);
    void            (*on_authenticate_done)(int status, const char *response);
    void            (*on_authenticate_poll)(const char *url);
    void            (*on_authenticate_poll_done)(int status, const char *response);
} hypr_auth_request;

typedef enum hypr_auth_status
{
    HYPR_AUTH_STARTED            = 0x0001,
    HYPR_AUTH_REQUEST_SENT       = 0x0002,
    HYPR_AUTH_INITIATED          = 0x0004,
    HYPR_AUTH_INITIATED_RESPONSE = 0x0008,
    HYPR_AUTH_COMPLETED_INIT     = 0x0010,
    HYPR_AUTH_COMPLETED          = 0x0020,
    HYPR_AUTH_CANCELLED          = 0x0040
} hypr_auth_status;

typedef struct {
    hypr_auth_request *request;
    const char        *request_id;
    int                authenticate_http_status;
    int                authenticate_response_code;
    const char        *authenticate_response_message;
    int                poll_http_status;
    int                poll_count;
    hypr_auth_status   status;
} hypr_auth_response;

extern hypr_auth_config   *hypr_create_auth_config();
extern void                hypr_free_auth_config(hypr_auth_config *);
extern hypr_auth_request  *hypr_create_auth_request();
extern hypr_auth_request  *hypr_create_auth_request_with_config(hypr_auth_config *);
extern void                hypr_free_auth_request(hypr_auth_request *);
extern hypr_auth_response *hypr_auth(hypr_auth_request *, const char *username);
extern void                hypr_free_auth_response(hypr_auth_response *);

#endif
