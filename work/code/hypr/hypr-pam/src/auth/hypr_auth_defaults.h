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

#ifndef __HYPR_AUTH_DEFAULTS_H
#define __HYPR_AUTH_DEFAULTS_H

static const char *HYPR_AUTH_DEFAULT_BASE_URL              = "https://bojandev.gethypr.com";
static const char *HYPR_AUTH_DEFAULT_ENDPOINT_PATH         = "/rp/api/oob/client/authentication";
static const char *HYPR_AUTH_DEFAULT_APP_ID                = "fIDO2Local";
static const char *HYPR_AUTH_DEFAULT_ACCESS_TOKEN          = "REDACTED";
static int         HYPR_AUTH_DEFAULT_POLL_MAX_TIME_SECONDS = 60;
static int         HYPR_AUTH_DEFAULT_POLL_INTERVAL_SECONDS = 2;
static int         HYPR_AUTH_DEFAULT_TIMEOUT_MS            = 20 * 1000;
static const char *HYPR_PAM_DEFAULT_CONFIG_FILE            = "/etc/security/pam_hypr.conf";

#endif
