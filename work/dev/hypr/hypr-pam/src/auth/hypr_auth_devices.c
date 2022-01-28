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
#include <string.h>
#include <syslog.h>
#include <unistd.h>
#include "hypr_auth.h"
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "util/hypr_uuid.h"

// Internal functions.

static char *create_devices_url(const hypr_auth_config *, const char *username);

hypr_json
hypr_get_auth_devices(const hypr_auth_config *config, const char *username, char **url)
{
    if (config == NULL) {
        return NULL;
    }

    char *devices_url = create_devices_url(config, username);

    if (devices_url == NULL) {
        return NULL;
    }

    if (url != NULL) {
        *url = hypr_str_copy(devices_url);
    }

    hypr_json devices_response;

    int http_status = hypr_http_get_json(devices_url, config->access_token, config->timeout, &devices_response);

    free((void *) devices_url);

    if (http_status != 200) {
        return NULL;
    }

    if (devices_response == NULL) {
        return NULL;
    }

    // Don't bother actually parsing this into some C HYPR C SDK object,
    // but parsing the JSON result would look something like the below.
    //
    // int devices_node_count = hypr_json_get_array_count(devices_response);
    // int i;
    // for (i = 0 ; i < devices_node_count ; i++) {
    //     hypr_json   device_node         = hypr_json_get_array_element(devices_response,    i);
    //     const char *device_id           = hypr_json_get_string       (device_node,         "deviceId");
    //     const char *name                = hypr_json_get_string       (device_node,         "friendlyName");
    //     hypr_json   fb_device_info_node = hypr_json_get_object       (device_node,         "fbDeviceInfo");
    //     const char *brand               = hypr_json_get_string       (fb_device_info_node, "brand");
    // }

    return devices_response;
}

static char *create_devices_url(const hypr_auth_config *config, const char *username)
{
    // Ensure we don't have double slashes in the final URL.

    char *trimmed_base_url      = hypr_str_rtrim(config->base_url,      '/', false);
    char *trimmed_endpoint_path = hypr_str_trim (config->endpoint_path, '/', false);
    char *trimmed_app_id        = hypr_str_trim (config->app_id,        '/', false);

    char *url = hypr_str_new("%s/%s/%s/%s/devices", trimmed_base_url, trimmed_endpoint_path, trimmed_app_id, username);

    hypr_str_free(trimmed_base_url);
    hypr_str_free(trimmed_endpoint_path);
    hypr_str_free(trimmed_app_id);

    return url;
}
