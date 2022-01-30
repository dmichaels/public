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
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include "hypr_http.h"

extern int hypr_http_libcurl_fetch(hypr_http_type, const char *url, const char *bearer, const char *data, int timeout, char **response, char **response_header);

bool
hypr_http_static_libcurl()
{
#if HYPR_HTTP_STATIC_LIBCURL != 0
    return true;
#else
    return false;
#endif
}

int
hypr_http_get(const char *url, const char *bearer, int timeout, char **response)
{
    return hypr_http_fetch(HYPR_HTTP_GET, url, bearer, NULL, timeout, response, NULL);
}

int
hypr_http_post(const char *url, const char *bearer, const char *data, int timeout, char **response)
{
    return hypr_http_fetch(HYPR_HTTP_POST, url, bearer, data, timeout, response, NULL);
}

int
hypr_http_put(const char *url, const char *bearer, const char *data, int timeout, char **response)
{
    return hypr_http_fetch(HYPR_HTTP_PUT, url, bearer, data, timeout, response, NULL);
}

int
hypr_http_get_json(const char *url, const char *bearer, int timeout, hypr_json *response)
{
    char *content;
    int http_status = hypr_http_get(url, bearer, timeout, &content);
    hypr_json json = hypr_json_parse(content);
    free((void *) content);
    if (response != NULL) *response = json;
    return http_status;
}

int
hypr_http_post_json(const char *url, const char *bearer, const char *data, int timeout, hypr_json *response)
{
    char *content;
    int http_status = hypr_http_post(url, bearer, data, timeout, &content);
    hypr_json json = hypr_json_parse(content);
    free((void *) content);
    if (response != NULL) *response = json;
    return http_status;
}

int
hypr_http_put_json(const char *url, const char *bearer, const char *data, int timeout, hypr_json *response)
{
    char *content;
    int http_status = hypr_http_put(url, bearer, data, timeout, &content);
    hypr_json json = hypr_json_parse(content);
    free((void *) content);
    if (response != NULL) *response = json;
    return http_status;
}

int
hypr_http_fetch_json(hypr_http_type verb, const char *url, const char *bearer, const char *data, int timeout, hypr_json *response, char **response_header)
{
    char *content;
    int http_status = hypr_http_libcurl_fetch(verb, url, bearer, data, timeout, &content, response_header);
    if (response != NULL) {
        *response = hypr_json_parse(content);
    }
    free((void *) content);
    return http_status;
}

int hypr_http_fetch(hypr_http_type verb, const char *url, const char *bearer, const char *data, int timeout, char **response, char **response_header)
{
    return hypr_http_libcurl_fetch(verb, url, bearer, data, timeout, response, response_header);
}

const char *
hypr_http_type_name(hypr_http_type verb)
{
    if      ((verb & HYPR_HTTP_GET)    != 0) return "GET";
    else if ((verb & HYPR_HTTP_POST)   != 0) return "POST";
    else if ((verb & HYPR_HTTP_PUT)    != 0) return "PUT";
    else if ((verb & HYPR_HTTP_PATCH)  != 0) return "PATCH";
    else if ((verb & HYPR_HTTP_DELETE) != 0) return "DELETE";
    else                                     return "UNKNOWN";
}
