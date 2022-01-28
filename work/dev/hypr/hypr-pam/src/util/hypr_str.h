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

#ifndef __HYPR_STR_H
#define __HYPR_STR_H

#include <stdbool.h>

extern bool        hypr_str_empty       (const char *);
extern bool        hypr_str_blank       (const char *);
extern bool        hypr_str_equals      (const char *, const char *);
extern bool        hypr_str_iequals     (const char *, const char *);
extern bool        hypr_str_starts_with (const char *, const char *);
extern bool        hypr_str_ends_with   (const char *, const char *);
extern bool        hypr_str_istarts_with(const char *, const char *);
extern bool        hypr_str_iends_with  (const char *, const char *);
extern const char *hypr_str_find        (const char *, const char *);
extern const char *hypr_str_ifind       (const char *, const char *);
extern char       *hypr_str_alloc       (int);
extern char       *hypr_str_copy        (const char *);
extern char       *hypr_str_new         (const char *, ...);
extern char       *hypr_str_cat         (const char *, const char *, bool);
extern char       *hypr_str_replace     (const char *, const char *, const char *, bool);
extern char       *hypr_str_ltrim       (const char *, char, bool);
extern char       *hypr_str_rtrim       (const char *, char, bool);
extern char       *hypr_str_trim        (const char *, char, bool);
extern void        hypr_str_free        (char *);

#endif
