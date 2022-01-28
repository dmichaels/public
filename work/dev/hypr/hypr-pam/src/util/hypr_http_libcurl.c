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

#define _GNU_SOURCE
#include <dlfcn.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <curl/curl.h>
#include "hypr_http.h"
#include "hypr_str.h"

static CURL  *curl_init();
static void   curl_deinit();
static bool   set_alternate_libcurl_path();
static size_t write_memory_callback(void *content, size_t size, size_t nmemb, void *userdata);

#if HYPR_HTTP_STATIC_LIBCURL == 0

/*
 * We load libcurl.so dynamically to obviate build-time linkage to increase our chances of running
 * across Linux variants, with the different versions of libcurl, et cetera. For example, we ran
 * into issues with binaries (pam_hypr.so, hypr CLI) built on Ubuntu 20 not installing on Debian 9.
 * with error (on apt install of DEB): Depends: libcurl4 (>= 7.16.2) but it is not installable
 * But loading dynamically, with no build/install-time dependency works fine.
 *
 * TODO
 * Think about trying to find libcurl.so if it can't be found, e.g. probably usually if/when
 * for some reason there is no symlink from the versioned file (e.g. libcurl.so.4) to the
 * unversioned file (libcurl.so). Didn't think would happen (at least much), but came up when
 * John Paglierani was testing, on CentOS 8 (2021-06-29). In such cases we can easily, though
 * manually, get around it by either creating the appropriate symlink, or by explicitly setting
 * path to the libcurl.so file in /etc/security/pam_hypr.config, e.g.: libcurl /usr/lib/libcurl.so.4
 */

#define DEFAULT_LIBCURL_PATH "libcurl.so"
static char                *libcurl_path_global                                         = NULL;
static void *               libcurl_so                                                  = NULL;
static bool                 libcurl_path_using_alternate                                = false;
static CURLcode           (*libcurl_so_global_init)(long)                               = NULL;
static CURL              *(*libcurl_so_easy_init)()                                     = NULL;
static CURLcode           (*libcurl_so_easy_setopt)(CURL *, CURLoption, ...)            = NULL;
static CURLcode           (*libcurl_so_easy_getinfo)(CURL *, CURLINFO, ...)             = NULL;
static struct curl_slist *(*libcurl_so_slist_append)(struct curl_slist *, const char *) = NULL;
static CURLcode           (*libcurl_so_easy_perform)(CURL *)                            = NULL;
static void               (*libcurl_so_slist_free_all)(struct curl_slist *)             = NULL;
static void               (*libcurl_so_easy_cleanup)(CURL *)                            = NULL;

#else

static CURLcode           (*libcurl_so_global_init)(long)                               = curl_global_init;
static CURL              *(*libcurl_so_easy_init)()                                     = curl_easy_init;
static CURLcode           (*libcurl_so_easy_setopt)(CURL *, CURLoption, ...)            = curl_easy_setopt;
static CURLcode           (*libcurl_so_easy_getinfo)(CURL *, CURLINFO, ...)             = curl_easy_getinfo;
static struct curl_slist *(*libcurl_so_slist_append)(struct curl_slist *, const char *) = curl_slist_append;
static CURLcode           (*libcurl_so_easy_perform)(CURL *)                            = curl_easy_perform;
static void               (*libcurl_so_slist_free_all)(struct curl_slist *)             = curl_slist_free_all;
static void               (*libcurl_so_easy_cleanup)(CURL *)                            = curl_easy_cleanup;

#endif

typedef struct {
    char *memory;
    size_t nbytes;
} http_result_data;

int
hypr_http_libcurl_fetch(hypr_http_type verb, const char *url, const char *bearer, const char *data, int timeout, char **response, char **response_header)
{
    if (response != NULL) *response = NULL;
    if (response_header != NULL) *response_header = NULL;

    if (url == NULL) return 0;

    // Initialize libcurl. 

    CURL *curl = curl_init();

    if (curl == NULL) {
        syslog(LOG_ERR, "HYPR: Error on libcurl initialization.");
        return 0;
    }

    // The aggregated/final result of the HTTP fetch goes into this 'result' http_result_data struct.

    http_result_data result;
    result.memory = (char *) malloc(1);
    result.nbytes = 0;

    // Setup basic HTTP request.

    libcurl_so_easy_setopt(curl, CURLOPT_URL,             url);
    libcurl_so_easy_setopt(curl, CURLOPT_FOLLOWLOCATION,  1L);
    libcurl_so_easy_setopt(curl, CURLOPT_WRITEFUNCTION,   write_memory_callback);
    libcurl_so_easy_setopt(curl, CURLOPT_WRITEDATA,       &result);
    libcurl_so_easy_setopt(curl, CURLOPT_USERAGENT,       HYPR_HTTP_USER_AGENT);

    http_result_data header_result;
    if (response_header != NULL) {
        header_result.memory = (char *) malloc(1);
        header_result.nbytes = 0;
        libcurl_so_easy_setopt(curl, CURLOPT_HEADERFUNCTION, write_memory_callback);
        libcurl_so_easy_setopt(curl, CURLOPT_HEADERDATA,     &header_result);
    }

    // Setup the verb of HTTP request (GET, POST, PUT, PATCH, DELETE).

    if (verb == HYPR_HTTP_PUT) {
        libcurl_so_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
    }
    else if (verb == HYPR_HTTP_PATCH) {
        libcurl_so_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
    }
    else if (verb == HYPR_HTTP_DELETE) {
        libcurl_so_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
    }
    else if (verb == HYPR_HTTP_POST) {
        libcurl_so_easy_setopt(curl, CURLOPT_POST, 1L);
    }
    else {
        libcurl_so_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
    }

    // Setup any authorization/bearer token.

    struct curl_slist *headers = NULL;
    if ((bearer != NULL) && (bearer[0] != '\0')) {
        char buffer[4096];
        snprintf(buffer, sizeof(buffer), "Authorization: Bearer %s", bearer);
        if (!hypr_str_blank(HYPR_HTTP_CONTENT_TYPE_HEADER)) {
            headers = libcurl_so_slist_append(headers, HYPR_HTTP_CONTENT_TYPE_HEADER);
        }
        headers = libcurl_so_slist_append(headers, buffer);
        libcurl_so_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    }

    // Setup any data to send (POST, PUT).

    if ((data != NULL) && (data[0] != '\0')) {
        libcurl_so_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    }

    // Setup the timeout(s), in milliseconds.
    // If timeout is zero then do not set at all; use libcurl default.

    if (timeout > 0) {
        libcurl_so_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);
    }

    // Actually make the HTTP request.

    CURLcode curl_status = libcurl_so_easy_perform(curl);

    // Check the return status.

    if (curl_status != CURLE_OK) {
        syslog(LOG_ERR, "HYPR: Bad status code (%d) for %s (%s)", curl_status, hypr_http_type_name(verb), url);
        return 0;
    }

    // Get the HTTP response/status code.

    long http_status;
    libcurl_so_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_status);

    // Cleanup.

    libcurl_so_slist_free_all(headers);
    libcurl_so_easy_cleanup(curl);

    // Return the content.
    // Caller responsible for freeing this memory.

    if (response != NULL) {
        *response = result.memory;
    }

    if (response_header != NULL) {
        const char *end_header = hypr_str_find(header_result.memory, "\r\n\r\n");
        if (end_header != NULL) {
            *((char *) end_header) = '\0';
            *response_header = strdup(header_result.memory);
            *((char *) end_header) = '\r';
            free((void *) header_result.memory);
        }
        else {
            *response_header = header_result.memory;
        }
    }

    return http_status;
}

bool hypr_http_libcurl_okay()
{
#if HYPR_HTTP_STATIC_LIBCURL == 0
    void *libcurl_so = dlopen(hypr_http_libcurl_path_global_get(), RTLD_NOW);
    if (libcurl_so == NULL) {
        if (set_alternate_libcurl_path()) {
             return true;
        }
        return false;
    }
    dlclose(libcurl_so);
#endif
    return true;
}

char *hypr_http_libcurl_path()
{
#if HYPR_HTTP_STATIC_LIBCURL == 0
    void *dl = dlopen(hypr_http_libcurl_path_global_get(), RTLD_NOW);
    if (dl == NULL) {
        return NULL;
    }
    void *dlf = dlsym(dl, "curl_global_init");
    if (dlf == NULL) {
        dlclose(dl);
        return NULL;
    }
    Dl_info dli;
    if (dladdr(dlf, &dli) == 0) {
        dlclose(dl);
        return NULL;
    }
    char *path = hypr_str_copy(dli.dli_fname);
    dlclose(dl);
    return path;
#endif
    return NULL;
}

const char *hypr_http_libcurl_path_global_get()
{
    if (!hypr_str_blank(libcurl_path_global)) {
        return libcurl_path_global;
    }
    else {
        return DEFAULT_LIBCURL_PATH;
    }
}

void hypr_http_libcurl_path_global_set(const char *value)
{
    if (!hypr_str_blank(value)) {
        hypr_str_free(libcurl_path_global);
        libcurl_path_global = hypr_str_copy(value);
    }
    else {
        libcurl_path_global = NULL;
    }
}

bool hypr_http_libcurl_path_using_alternate()
{
    return libcurl_path_using_alternate;
}

/*
 * Initializes the libcurl library; both globally which is done
 * once before first use of libcurl, and then for the current usage.
 * Should we ever call curl_global_cleanup for our purposes?
 */
static CURL *curl_init()
{
    static bool initialized = false;

    if (!initialized) {

#if HYPR_HTTP_STATIC_LIBCURL == 0

        libcurl_so = dlopen(hypr_http_libcurl_path_global_get(), RTLD_NOW);

        if (libcurl_so == NULL) {
            if (!set_alternate_libcurl_path()) {
                syslog(LOG_ERR, "HYPR: Error dynamically loading default libcurl shared library and no alternative found.");
                return NULL;
            }
            libcurl_so = dlopen(hypr_http_libcurl_path_global_get(), RTLD_NOW);
            if (libcurl_so == NULL) {
                syslog(LOG_ERR, "HYPR: Error dynamically loading libcurl shared library.");
                return NULL;
            }
        }

        libcurl_so_global_init    = (CURLcode (*)(long))                                        dlsym(libcurl_so, "curl_global_init");
        libcurl_so_easy_init      = (CURL *(*)())                                               dlsym(libcurl_so, "curl_easy_init");
        libcurl_so_easy_setopt    = (CURLcode (*)(CURL *, CURLoption, ...))                     dlsym(libcurl_so, "curl_easy_setopt");
        libcurl_so_easy_getinfo   = (CURLcode (*)(CURL *, CURLINFO, ...))                       dlsym(libcurl_so, "curl_easy_getinfo");
        libcurl_so_slist_append   = (struct curl_slist *(*)(struct curl_slist *, const char *)) dlsym(libcurl_so, "curl_slist_append");
        libcurl_so_easy_perform   = (CURLcode (*)(CURL *))                                      dlsym(libcurl_so, "curl_easy_perform");
        libcurl_so_slist_free_all = (void (*)(struct curl_slist *))                             dlsym(libcurl_so, "curl_slist_free_all");
        libcurl_so_easy_cleanup   = (void (*)(CURL *))                                          dlsym(libcurl_so, "curl_easy_cleanup");

        if ((libcurl_so_global_init    == NULL) ||
            (libcurl_so_easy_init      == NULL) ||
            (libcurl_so_easy_setopt    == NULL) ||
            (libcurl_so_easy_getinfo   == NULL) ||
            (libcurl_so_slist_append   == NULL) ||
            (libcurl_so_easy_perform   == NULL) ||
            (libcurl_so_slist_free_all == NULL) ||
            (libcurl_so_easy_cleanup   == NULL)) {
            dlclose(libcurl_so);
            syslog(LOG_ERR, "HYPR: Error dynamically loading libcurl shared library functions.");
            return NULL;
        }
#endif

        if (libcurl_so_global_init(CURL_GLOBAL_ALL) != 0) {
            syslog(LOG_ERR, "HYPR: Error on libcurl global initialization.");
#if HYPR_HTTP_STATIC_LIBCURL == 0
            dlclose(libcurl_so);
#endif
            return NULL;
        }

        initialized = true;

        atexit(curl_deinit);
    }

    CURL *init = libcurl_so_easy_init();

    if (init == NULL) {
        syslog(LOG_ERR, "HYPR: Error on libcurl initialization.");
    }

    return init;
}

static void curl_deinit()
{
#if HYPR_HTTP_STATIC_LIBCURL == 0
    if (libcurl_so != NULL) {
        dlclose(libcurl_so);
        libcurl_so = NULL;
    }
#endif
}

static bool set_alternate_libcurl_path()
{
#if HYPR_HTTP_STATIC_LIBCURL == 0
    static const char *possible_libcurl_paths[] = {
       "libcurl.so.8",
       "libcurl.so.7",
       "libcurl.so.6",
       "libcurl.so.5",
       "libcurl.so.4",
       "libcurl.so.3"
    };
    static const int npossible_libcurl_paths = sizeof(possible_libcurl_paths) /
                                               sizeof(possible_libcurl_paths[0]);
    const char *save_libcurl_path_global = hypr_http_libcurl_path_global_get();
    int i;
    for (i = 0 ; i < npossible_libcurl_paths ; i++) {
        const char *possible_libcurl_path = possible_libcurl_paths[i];
        hypr_http_libcurl_path_global_set(possible_libcurl_path);
        void *libcurl_so = dlopen(hypr_http_libcurl_path_global_get(), RTLD_NOW);
        if (libcurl_so != NULL) {
            dlclose(libcurl_so);
            char *libcurl_path = hypr_http_libcurl_path();
            if (!hypr_str_blank(libcurl_path)) {
                syslog(LOG_NOTICE, "HYPR: Using  alternate libcurl shared library; %s rather than %s.", libcurl_path, save_libcurl_path_global);
                hypr_str_free(libcurl_path);
                libcurl_path_using_alternate = true;
                return true;
            }
            hypr_str_free(libcurl_path);
        }
    }
    hypr_http_libcurl_path_global_set(save_libcurl_path_global);
    return false;
#else
    return true;
#endif
}

/*
 * Internal function/callback to collect/aggregate/concatenate the HTTP response.
 * This will be called one or (perhaps many) more times during the data fetch.
 * The 'content' is the incoming data/bytes; 'nmemb' is the number elements in
 * this array of incoming bytes; and 'size' is the size in bytes of each element;
 * this 'size' it typically just 1, though it matters not; the main point is that
 * the number of bytes comprising the incoming data is 'nmemb' * 'size'.
 * The 'userdata' is the (single) aggregated data object, setup/defined originally,
 * before that start of the fetch via the CURLOPT_WRITEDATA libcurl option.
 */
static size_t write_memory_callback(void *content, size_t size, size_t nmemb, void *userdata) {

    size_t nbytes = size * nmemb;
    http_result_data *result = (http_result_data *)userdata;

    char *new_content = (char *)realloc(result->memory, result->nbytes + nbytes + 1);

    if (new_content == NULL) {
        syslog(LOG_ERR, "HYPR: Not enough memory for HTTP fetch");
        return 0;
    }

    result->memory = new_content;
    memcpy(&(result->memory[result->nbytes]), content, nbytes);
    result->nbytes += nbytes;
    result->memory[result->nbytes] = 0;

    return nbytes;
}
