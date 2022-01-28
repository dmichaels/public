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
#include "hypr_str.h"

/*
 * Found that we can avoid the libuuid dependency by using the pseudo
 * file /proc/sys/kernel/random/uuid which when read (or just cat-ed)
 * generates a new UUID. Verified that this file exists and wokd on
 * Linux variants: Ubuntu, CentOS, RedHet, Debian, OpenSUSE, Kali, Mint
 * But should still make this a check at build time (TODO).
 */
static const char *UUID_PSEUDO_FILE    = "/proc/sys/kernel/random/uuid";
static const int   UUID_LENGTH         = 36;
static const int   UUID_TRIMMED_LENGTH = 32;

/*
 * Writes a newly generated UUID string to the given string buffer which is
 * ASSUMED to have at least 37-characters (UUID_LENGTH + 1) allocated to it.
 * Returns the given buffer, unless an error occurs in which case NULL is returned.
 */
static char *
hypr_uuid_raw(char *uuid_buffer)
{
    if (uuid_buffer == NULL) return NULL;
    FILE *f = fopen(UUID_PSEUDO_FILE, "r");
    if (f == NULL) {
        return NULL;
    }
    char *result = fgets(uuid_buffer, UUID_LENGTH + 1, f);
    fclose(f);
    return result;
}

/*
 * Writes a newly generated trimmed UUID (minus-dashes, upper-case) to the given string buffer
 * which is ASSUMED to have at least 33-characters (UUID_TRIMMED_LENGTH + 1) allocated to it.
 * Returns the given buffer, unless an error occurs in which case NULL is returned.
 */
static char *
hypr_uuid_trimmed(char *uuid_trimmed_buffer)
{
    if (uuid_trimmed_buffer == NULL) return NULL;
    char uuid_buffer[UUID_LENGTH + 1];
    if (hypr_uuid_raw(uuid_buffer) == NULL) return NULL;
    const char *src = uuid_buffer;
    char *dst = uuid_trimmed_buffer;
    int i;
    for (i = 0 ; *src != '\0' && i < UUID_LENGTH ; i++) {
        if (*src != '-') {
            *dst++ = toupper(*src);
        }
        src++;
    }
    *dst = '\0';
    return uuid_trimmed_buffer;
}

/*
 * Returns a newly generated "trimmed" UUID (minus-dashes, upper-case) in a newly
 * allocated string; it will be 32-characters in length. Returns NULL if an error
 * occurred. Caller responsible for freeing the returned string.
 */
extern char *
hypr_uuid()
{
    char uuid_trimmed_buffer[UUID_TRIMMED_LENGTH + 1];
    if (hypr_uuid_trimmed(uuid_trimmed_buffer) == NULL) {
         return NULL;
    }
    return hypr_str_new(uuid_trimmed_buffer);
}

/*
 * Returns a "nonce", which, in our case, currently, is two UUIDs concatenated
 * together (minus-dashes, upper-case), in a newly allocated string; it will be
 * 64-characters in length. Caller responsible for freeing the returned string.
 * FYI: HyprJavaWebClient uses SHA256 of a random integer.
 */
extern char *
hypr_nonce()
{
    char uuid1_trimmed_buffer[UUID_TRIMMED_LENGTH + 1];
    char uuid2_trimmed_buffer[UUID_TRIMMED_LENGTH + 1];
    if (hypr_uuid_trimmed(uuid1_trimmed_buffer) == NULL ||
        hypr_uuid_trimmed(uuid2_trimmed_buffer) == NULL) {
         return NULL;
    }
    return hypr_str_new("%s%s", uuid1_trimmed_buffer, uuid2_trimmed_buffer);
}
