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
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "util/hypr_http.h"
#include "util/hypr_json.h"
#include "util/hypr_str.h"
#include "util/hypr_uuid.h"
#include "util/hypr_datetime.h"
#include "auth/hypr_auth.h"
#include "auth/hypr_auth_devices.h"
#include "../version.h"

extern int hypr_cli_info(int argc, char *argv[], const char *);
extern int hypr_cli_auth(int argc, char *argv[], const char *);
extern int hypr_cli_devices(int argc, char *argv[]);
extern int hypr_cli_pam(int argc, char *argv[]);
extern int hypr_cli_http(int argc, char *argv[]);
extern int hypr_cli_aliases(int argc, char *argv[]);

static
void usage()
{
    printf("HYPR Linux PAM %s | %s:\n", VERSION, COPYRIGHT);
    fprintf(stderr, "usage: hypr info    [--verbose]\n");
    fprintf(stderr, "usage: hypr auth    [--login login | --username username] [--config file] [--access-token token] [--base-url url] [--app-id id] [--verbose | --debug] [--curl]\n");
    fprintf(stderr, "usage: hypr devices [--login login | --username username] [--config file] [--access-token token] [--base-url url] [--app-id id] [--verbose | --debug]\n");
    fprintf(stderr, "usage: hypr pam     [--login login] [--full]\n");
    fprintf(stderr, "usage: hypr http    [--verb { GET | POST | PUT }] [--bearer bearer] [--data @file | --data data] [--verbose] [--output file] [--dump-header file] [--json] url\n");
    fprintf(stderr, "usage: hypr alias   [list username [--email | --all] | set username alias... [--email] | delete username [--email]]\n");
    exit(1);
}

/*
 * Test command-line app for general info, and test HYPR authentication,
 * test PAM authentication, and username aliases.
 */
int main(int argc, char *argv[])
{
    int i;
    for (i = 1 ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "config") || hypr_str_iequals(argv[i], "conf") || hypr_str_iequals(argv[i], "info") ||
            hypr_str_iequals(argv[i], "--config") || hypr_str_iequals(argv[i], "--conf") || hypr_str_iequals(argv[i], "--info")) {
            return hypr_cli_info(argc, argv, NULL);
        }
        else if (hypr_str_iequals(argv[i], "version") || hypr_str_iequals(argv[i], "--version")) {
            return hypr_cli_info(argc, argv, "--version");
        }
        else if (hypr_str_iequals(argv[i], "auth") ||
                 hypr_str_iequals(argv[i], "authenticate") ||
                 hypr_str_iequals(argv[i], "-auth") ||
                 hypr_str_iequals(argv[i], "-authenticate") ||
                 hypr_str_iequals(argv[i], "--auth") ||
                 hypr_str_iequals(argv[i], "--authenticate")) {
            return hypr_cli_auth(argc, argv, NULL);
        }
        else if (hypr_str_iequals(argv[i], "-command") ||
                 hypr_str_iequals(argv[i], "--command")) {
            //
            // Just so we can use exactly like HyprJavaWebClient
            //
            if (++i >= argc) usage();
            const char *command = argv[i];
            if (hypr_str_iequals(command, "auth") ||
                hypr_str_iequals(command, "authenticate")) {
                return hypr_cli_auth(argc, argv, "--debug");
            }
        }
        else if (hypr_str_iequals(argv[i], "devices") ||
                 hypr_str_iequals(argv[i], "-devices") ||
                 hypr_str_iequals(argv[i], "--devices")) {
            return hypr_cli_auth(argc, argv, "--devices");
        }
        else if (hypr_str_iequals(argv[i], "http") || hypr_str_iequals(argv[i], "https") ||
                 hypr_str_iequals(argv[i], "--http") || hypr_str_iequals(argv[i], "--https")) {
            return hypr_cli_http(argc, argv);
        }
        else if (hypr_str_iequals(argv[i], "pam") || hypr_str_iequals(argv[i], "--pam")) {
            return hypr_cli_pam(argc, argv);
        }
        else if (hypr_str_iequals(argv[i], "aliases") || hypr_str_iequals(argv[i], "alias")) {
            return hypr_cli_aliases(argc, argv);
        }
    }
    usage();
    return 0;
}
