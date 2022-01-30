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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "util/hypr_url.h"

static void usage()
{
    fprintf(stderr, "usage: hypr http [--verb { GET | POST | PUT }] [--bearer bearer] [--data @file | --data data] [--verbose] [--output file] [--dump-header file] [--json] url\n");
    exit(1);
}

static char *read_file_contents(const char *file)
{
    FILE *f = fopen(file, "rb");
    if (f == NULL) {
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *string = malloc(fsize + 1);
    size_t dummy = fread(string, 1, fsize, f);
    fclose(f);
    string[fsize] = '\0';
    return string;
}

int hypr_cli_http(int argc, char *argv[])
{
    char *url = NULL;
    hypr_url *parsed_url = NULL;
    const char *data = NULL, *bearer = NULL, *data_file = NULL;
    char *response = NULL, *response_header = NULL;
    hypr_http_type verb = HYPR_HTTP_GET;
    int timeout = 0;
    const char *output = NULL;
    bool json = false, verbose = false, debug = false, dump_header = false;
    const char *dump_header_file = NULL; 
    const char *libcurl_path = NULL;

    int i;
    for (i = 2 ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "--verb") || hypr_str_equals(argv[i], "-X")) {
            if (++i >= argc) usage();
            if      (hypr_str_iequals(argv[i], "GET"))    { verb = HYPR_HTTP_GET;    }
            else if (hypr_str_iequals(argv[i], "POST"))   { verb = HYPR_HTTP_POST;   }
            else if (hypr_str_iequals(argv[i], "PUT"))    { verb = HYPR_HTTP_PUT;    }
            else if (hypr_str_iequals(argv[i], "PATCH"))  { verb = HYPR_HTTP_PATCH;  }
            else if (hypr_str_iequals(argv[i], "DELETE")) { verb = HYPR_HTTP_DELETE; }
            else usage();
        }
        else if (hypr_str_iequals(argv[i], "--bearer")) {
            if (++i >= argc) usage();
            bearer = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--data") || hypr_str_equals(argv[i], "-d")) {
            if (++i >= argc) usage();
            if (argv[i][0] == '@') {
                data_file = argv[i] + 1;
                data = read_file_contents(data_file);
                if (data == NULL) {
                    fprintf(stderr, "Cannot open data file: %s\n", data_file);
                    usage();
                }
            }
            else {
                data = argv[i];
            }
        }
        else if (hypr_str_iequals(argv[i], "--output") || hypr_str_equals(argv[i], "-o")) {
            if (++i >= argc) usage();
            output = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--json")) {
            json = true;
        }
        else if (hypr_str_iequals(argv[i], "--verbose")) {
            verbose = true;
        }
        else if (hypr_str_iequals(argv[i], "--debug")) {
            debug = true;
            verbose = true;
        }
        else if (hypr_str_iequals(argv[i], "--dump-header")) {
            if (++i >= argc) usage();
            dump_header = true;
            dump_header_file = argv[i];
            if (hypr_str_equals(dump_header_file, "-")) {
                dump_header_file = "/dev/stdout";
            }
        }
        else if (hypr_str_starts_with(argv[i], "-D")) {
            dump_header = true;
            dump_header_file = argv[i] + 2;
        }
        else if (hypr_str_iequals(argv[i], "--header") || hypr_str_equals(argv[i], "-H")) {
            fprintf(stderr, "Setting arbitrary headers not supported by this utility; only --bearer for authorization header.\n");
            usage();
        }
        else if (hypr_str_iequals(argv[i], "--libcurl")) {
            if (++i >= argc) usage();
            libcurl_path = argv[i];
        }
        else if (argv[i][0] == '-') {
            usage();
        }
        else if (parsed_url != NULL) {
            usage();
        }
        else {
            url = argv[i];
            parsed_url = hypr_url_parse(url);
            if (parsed_url == NULL) {
                url = hypr_str_cat("http://", url, false);
                parsed_url = hypr_url_parse(url);
                if (parsed_url == NULL) {
                    hypr_str_free(url);
                    usage();
                }
            }
            else {
                url = hypr_str_copy(url);
            }
        }
    }

    if (parsed_url == NULL) {
        usage();
    }

    if (!hypr_str_blank(libcurl_path)) {
        hypr_http_libcurl_path_global_set(libcurl_path);
    }

    if (verbose) {
        printf("URL: %s\n", url); 
        printf("METHOD: %s\n", hypr_http_type_name(verb)); 
        if (!hypr_str_blank(libcurl_path)) {
            printf("LIBCURL PATH: %s\n", libcurl_path);
        }
        printf("LIBCURL: %s%s\n", hypr_http_static_libcurl() ? "static" : "dynamic",
                                  hypr_http_static_libcurl() ? "" : (hypr_http_libcurl_okay() ? " (OK)" : " (not loadable)")); 
    }
    if (debug) {
        printf("PROTOCOL: %s\n", parsed_url->protocol); 
        if (!hypr_str_blank(parsed_url->user)) printf("USER: %s\n", parsed_url->user); 
        printf("HOST: %s\n", parsed_url->host); 
        printf("PORT: %d\n", parsed_url->port); 
        if (!hypr_str_blank(parsed_url->path)) printf("PATH: %s\n", parsed_url->path); 
        if (!hypr_str_blank(parsed_url->query)) printf("QUERY: %s\n", parsed_url->query); 
        hypr_url_free(parsed_url);
    }
    if (verbose) {
        if (!hypr_str_blank(bearer)) {
            printf("AUTHORIZATION: %s\n", bearer); 
        }
        if (!hypr_str_blank(data_file)) {
            printf("DATA FILE: %s\n", data_file);
        }
        if (!hypr_str_blank(data)) {
            printf("DATA SIZE: %d BYTES\n", (int) strlen(data)); 
        }
    }
    if (debug) {
        if (!hypr_str_blank(data)) {
            printf("DATA:\n%s\n", data);
        }
    }

    int http_status = hypr_http_fetch(verb, url, bearer, data, timeout, &response, &response_header);

    if (verbose) {
        printf("RESPONSE HTTP STATUS: %d\n", http_status); 
    }

    if (response != NULL) {
        if (verbose) {
            printf("RESPONSE SIZE: %d BYTES\n", (int) strlen(response)); 
        }
        if (json) {
            hypr_json json_response = hypr_json_parse(response);
            if (json_response == NULL) {
                fprintf(stderr, "Cannot parse response as JSON.");
            }
            else {
                const char *formatted_json_response = hypr_json_format(json_response);
                if (formatted_json_response == NULL) {
                    fprintf(stderr, "Cannot format response as JSON.");
                }
                else {
                    response = hypr_str_copy(formatted_json_response);
                }
                hypr_json_free(json_response);
            }
        }
        if (!hypr_str_blank(output)) {
            if (verbose) {
                printf("RESPONSE FILE: %s\n", output); 
            }
            FILE *f = fopen(output, "w");
            if (f != NULL) {
                fprintf(f, "%s", response);
                fclose(f);
            }
            else {
                fprintf(stderr, "Cannot open output file: %s\n", output);
            }
        }
        else {
            if (verbose) {
                printf("RESPONSE:\n%s\n", response);
            }
            else {
                printf("%s", response);
            }
        }
    }

    if (dump_header) {
        if (response_header != NULL) {
            if (!hypr_str_blank(dump_header_file)) {
                if (hypr_str_iequals(dump_header_file, "-") || hypr_str_iequals(dump_header_file, "/dev/stdout")) {
                    if (verbose || debug) {
                        printf("RESPONSE HEADER:\n");
                    }
                    printf("%s\n", response_header);
                }
                else {
                    if (verbose) {
                        printf("RESPONSE HEADER FILE: %s\n", dump_header_file);
                    }
                    FILE *f = fopen(dump_header_file, "w");
                    if (f != NULL) {
                        fprintf(f, "%s", response_header);
                        fclose(f);
                    }
                    else {
                        fprintf(stderr, "Cannot open header output file: %s\n", dump_header_file);
                    }
                }
            }
            else {
                printf("RESPONSE HEADER:\n%s\n", response_header);
            }
        }
        else {
            printf("RESPONSE HEADER: NULL\n");
        }
    }

    if ((data_file != NULL) && (data != NULL)) {
        free((char *) data);
    }

    hypr_str_free(response);
    hypr_str_free(url);

    return 0;
}
