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

#ifndef __HYPR_HTTP_H
#define __HYPR_HTTP_H

#ifndef HYPR_HTTP_STATIC_LIBCURL
#define HYPR_HTTP_STATIC_LIBCURL 0
#endif

#include <stdbool.h>
#include "hypr_json.h"

typedef enum {
    HYPR_HTTP_GET     = 0x0001,
    HYPR_HTTP_POST    = 0x0002,
    HYPR_HTTP_PUT     = 0x0004,
    HYPR_HTTP_PATCH   = 0x0008,
    HYPR_HTTP_DELETE  = 0x0010
} hypr_http_type;

static const char *const HYPR_HTTP_CONTENT_TYPE_HEADER = "Content-Type: application/json";
static const char *const HYPR_HTTP_USER_AGENT          = "hypr-pam/1.0";

/*
 * Returns true iff this code has been compiled with HYPR_HTTP_STATIC_LIBCURL defined as 1.
 * The default build is undefined HYPR_HTTP_STATIC_LIBCURL (i.e. default load dynamically).
 */
extern bool hypr_http_static_libcurl();

/*
 * Returns true iff this code has been compiled with HYPR_HTTP_STATIC_LIBCURL defined as 1,
 * or if not, then iff libcurl can successfully be loaded dynamically.
 */
extern bool hypr_http_libcurl_okay();

/*
 * Returns NULL iff this code has been compiled with HYPR_HTTP_STATIC_LIBCURL defined as 1,
 * or if not, then the path name of the libcurl shared library file which can be dynamically loaded.
 */
extern char *hypr_http_libcurl_path();

/*
 * Globally sets the path of the libcurl shared library file used to load (dlopen) libcurl when used. 
 * If NULL then uses the default ("libcurl.so" - see hypr_http). 
 */
extern void hypr_http_libcurl_path_global_set(const char *);

/*
 * Returns the path of the libcurl shared library used to load (dlopen) libcurl when used.
 */
extern const char *hypr_http_libcurl_path_global_get();

extern bool hypr_http_libcurl_path_using_alternate();

/*
 * Functions for HTTP requests for GET, POST, and PUT.
 * Return value is the HTTP status. Response payload is set in final argument.
 * Caller responsible for freeing memory for this response.
 */
extern int hypr_http_get   (const char *url, const char *bearer, int timeout, char **response);
extern int hypr_http_post  (const char *url, const char *bearer, const char *data, int timeout, char **response);
extern int hypr_http_put   (const char *url, const char *bearer, const char *data, int timeout, char **response);
extern int hypr_http_fetch (hypr_http_type verb, const char *url, const char *bearer, const char *data, int timeout, char **response, char **response_header);

/*
 * Same as above except the response is a JSON object (hypr_json).
 */
extern int hypr_http_get_json   (const char *url, const char *bearer, int timeout, hypr_json *response);
extern int hypr_http_post_json  (const char *url, const char *bearer, const char *data, int timeout, hypr_json *response);
extern int hypr_http_put_json   (const char *url, const char *bearer, const char *data, int timeout, hypr_json *response);
extern int hypr_http_fetch_json (hypr_http_type verb, const char *url, const char *bearer, const char *data, int timeout, hypr_json *response, char **response_header);

extern const char *hypr_http_type_name(hypr_http_type verb);

#endif
