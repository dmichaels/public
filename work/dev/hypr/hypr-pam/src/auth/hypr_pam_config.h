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

#ifndef __HYPR_PAM_CONFIG_H
#define __HYPR_PAM_CONFIG_H

#include "hypr_auth.h"

typedef enum hypr_pam_config_disabled {
    HYPR_PAM_CONFIG_ENABLED          = 0x0000,
    HYPR_PAM_CONFIG_DISABLED         = 0x0001,
    HYPR_PAM_CONFIG_DISABLED_LOGIN   = 0x0002,
    HYPR_PAM_CONFIG_DISABLED_GROUP   = 0x0004,
    HYPR_PAM_CONFIG_DISABLED_COMMAND = 0x0008
} hypr_pam_config_disabled;

typedef struct {
    hypr_auth_config         *auth_config;
    const char               *username;
    hypr_pam_config_disabled  disabled;
    const char               *disabled_group;
    const char               *login_prompt;
    const char               *login_message;
    const char               *on_unmapped_login;
    int                       username_mappings;
    const char               *libcurl_path;
} hypr_pam_config;

extern hypr_pam_config *hypr_pam_config_read(const char *config_file, const char *login, const char *command, const char *remote_host);
extern void             hypr_pam_config_free(hypr_pam_config *);

#endif
