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

#include <security/pam_appl.h>
#include <security/pam_misc.h>
#include <pwd.h>
#include <sys/types.h>
#include <stdio.h>
#include "util/hypr_str.h"

/*
 * Mini PAM test application.
 * 
 * Create this file /etc/pam.d/hypr containing:
 *
 *   auth required pam_hypr.so
 *
 * Where pam_hypr.so is in/usr/lib/x86_64-linux-gnu/security or /usr/lib64/security
 * depending on the specific Linux variant.
 *
 * And the /etc/security/pam_hypr.conf file is also required, something like this:
 *
 *   access_token REDACTED
 *   api_base_url https://bojandev.gethypr.com
 *   api_auth_endpoint /rp/api/oob/client/authentication
 *   app_id fIDO2Local
 *   poll_max_time_seconds 30
 *   poll_interval_seconds 1
 *   timeout 30000
 *   disabled command sudo
 *   username some-linux-user some-hypr-account@some-domain.com
 *   username dmichaels david.michaels@hypr.com
 */

static const char *MY_APPLICATION_NAME = "hypr-example";

static void run_my_application()
{
    printf("Running test PAM-enabled authentication app (does nothing but print this line): OK\n");
}

static int my_conversation_function(int num_msg, const struct pam_message **msgm, struct pam_response **response, void *appdata_ptr)
{
    fprintf(stderr, "HYPR Linux PAM conversation function ...\n");
    fprintf(stderr, "  num_msg            = %d\n",     num_msg);
    fprintf(stderr, "  (*msgm)->msg_style = %d\n",     (*msgm)->msg_style);
    fprintf(stderr, "  (*msgm)->msg       = \"%s\"\n", (*msgm)->msg);
    fprintf(stderr, "  appdata_ptr        = 0x%lX\n",  (long)appdata_ptr);

    int r = misc_conv(num_msg, msgm, response, appdata_ptr);

    fprintf(stderr, "   (*response)->resp_retcode = %d\n",     (*response)->resp_retcode);
    fprintf(stderr, "   (*response)->resp         = \"%s\"\n", (*response)->resp);

    return r;
}

static struct pam_conv conv = { my_conversation_function, NULL };

static
void usage()
{
    fprintf(stderr, "hypr pam [--login login] [--full]\n");
    exit(1);
}

int hypr_cli_pam(int argc, char *argv[])
{
    pam_handle_t *pamh;
    int result;
    struct passwd *pw;
    const char *login = NULL;
    bool full = false;

    int i;
    for (i = 2 ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "--login") || hypr_str_iequals(argv[i], "-login") ||
            hypr_str_iequals(argv[i], "--username") || hypr_str_iequals(argv[i], "-username")) {
            if (++i >= argc) usage();
            login = argv[i];
        }
        else if (hypr_str_iequals(argv[i], "--full")) {
            full = true;
        }
        else if (hypr_str_iequals(argv[i], "--help")
              || hypr_str_iequals(argv[i], "-help")
              || hypr_str_iequals(argv[i], "help")
              || hypr_str_iequals(argv[i], "-h")
              || hypr_str_iequals(argv[i], "-h")
              || hypr_str_iequals(argv[i], "--version")
              || hypr_str_iequals(argv[i], "-version")
              || hypr_str_iequals(argv[i], "version")) {
            usage();
        }
    }

    fprintf(stderr, "HYPR Linux PAM mini-test command-line application ...\n");

    if (hypr_str_blank(login)) {
        if ((pw = getpwuid(getuid())) == NULL) {
            perror("getpwuid");
            return 1;
        }
        login = pw->pw_name;
    }

    fprintf(stderr, "Calling pam_start (app: %s, login: %s)\n", MY_APPLICATION_NAME, login);
    if ((result = pam_start(MY_APPLICATION_NAME, login, &conv, &pamh)) != PAM_SUCCESS) {
        fprintf(stderr, "Call to pam_start failed (%d)\n", result);
        return 1;
    }
    fprintf(stderr, "Back from pam_start (%d)\n", result);

    fprintf(stderr, "Calling pam_authenticate\n");
    if ((result = pam_authenticate(pamh, 0)) != PAM_SUCCESS) {
        fprintf(stderr, "Call to pam_authenticate failed (%d)\n", result);
        return 1;
    }
    fprintf(stderr, "Back from pam_authenticate (%d)\n", result);

    if (full) {

        fprintf(stderr, "Calling pam_acct_mgmt\n");
        if ((result = pam_acct_mgmt(pamh, 0)) != PAM_SUCCESS) {
            fprintf(stderr, "Call to pam_acct_mgmt failed (%d)\n", result);
            return 1;
        }
        fprintf(stderr, "Back from pam_acct_mgmt (%d)\n", result);

        fprintf(stderr, "Calling pam_open_session\n");
        if ((result = pam_open_session(pamh, 0)) != PAM_SUCCESS) {
            fprintf(stderr, "Call to pam_open_session failed (%d)\n", result);
            return 1;
        }
        fprintf(stderr, "Back from pam_open_session (%d)\n", result);

        fprintf(stderr, "Calling pam_close_session\n");
        if ((result = pam_close_session(pamh, 0)) != PAM_SUCCESS) {
            fprintf(stderr, "Call to pam_close_session failed (%d)\n", result);
            return 1;
        }
        fprintf(stderr, "Back from pam_close_session (%d)\n", result);
    }

    fprintf(stderr, "Calling pam_end\n");
    if ((result = pam_end(pamh, result)) != PAM_SUCCESS) {
        fprintf(stderr, "Call to pam_end failed (%d)\n", result);
        return 1;
    }
    fprintf(stderr, "Back from pam_end (%d)\n", result);

    run_my_application();

    return 0;
}
