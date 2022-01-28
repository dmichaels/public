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

#ifndef __HYPR_JSON_H
#define __HYPR_JSON_H

/*
 * The 'hypr_json' type is an opaque JSON object type to insulate callers from
 * underlying implementation; this is currently json-c in our case, but could
 * well conceivably change, e.g. if some security vulnerability is found;
 * the jansson library is another alternative.
 */
typedef void *hypr_json;

extern hypr_json   hypr_json_parse(const char *);
extern hypr_json   hypr_json_new();
extern hypr_json   hypr_json_new_array();
extern void        hypr_json_add_object(hypr_json json, const char *name, hypr_json);
extern void        hypr_json_add_string(hypr_json json, const char *name, const char *);
extern void        hypr_json_add_string_element(hypr_json json, const char *value);
extern void        hypr_json_free(hypr_json);

extern const char *hypr_json_get_string(hypr_json, const char *name);
extern int         hypr_json_get_int(hypr_json, const char *name);
extern hypr_json   hypr_json_get_object(hypr_json, const char *name);
extern int         hypr_json_get_array_count(hypr_json);
extern hypr_json   hypr_json_get_array_element(hypr_json, int index);
extern const char *hypr_json_get_array_string_element(hypr_json, int index);

extern const char *hypr_json_format(hypr_json);
extern const char *hypr_json_noformat(hypr_json);
extern const char *hypr_json_tostring(hypr_json json_string_object);

#endif
