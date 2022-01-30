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

#include "json.h"
#include "hypr_json.h"
#include <errno.h>
#include <stdio.h>

// Thin JSON library wrapper around the 'json-c' library.
// https://github.com/json-c/json-c
// https://json-c.github.io/json-c/json-c-0.12/doc/html/files.html
//
// Some more examples:
// https://linuxprograms.wordpress.com/2010/05/20/json-c-libjson-tutorial/
//
// Discussions of when/how to free JSON object memory:
// https://github.com/json-c/json-c/issues/642
// https://www.reddit.com/r/C_Programming/comments/evs2xl/need_help_understanding_memory_management_in_jsonc/

/*
 * Returns a new JSON object from the given JSON string.
 * Caller responsible for freeing the returned JSON object (via hypr_json_free).
 */
hypr_json
hypr_json_parse(const char *value)
{
    return (value != NULL) ? json_tokener_parse(value) : NULL;
}

/*
 * Returns a new JSON object.
 * Caller responsible for freeing the returned object (via hypr_json_free).
 */
hypr_json
hypr_json_new()
{
    return json_object_new_object();
}

/*
 * Returns a new JSON array object.
 * Caller responsible for freeing the returned object (via hypr_json_free).
 */
hypr_json
hypr_json_new_array()
{
    return json_object_new_array();
}

/*
 * Adds to the given JSON array object, the given string.
 */
void
hypr_json_add_string_element(hypr_json json_array, const char *value)
{
    json_object_array_add(json_array, json_object_new_string(value));
}

/*
 * Adds the given JSON object value with the given name to the given JSON object.
 */
void
hypr_json_add_object(hypr_json json, const char *name, hypr_json value)
{
    json_object_object_add(json, name, value);
}

/*
 * Adds the given string value with the given name to the given JSON object.
 */
void
hypr_json_add_string(hypr_json json, const char *name, const char *value)
{
    json_object_object_add(json, name, json_object_new_string(value));
}

/*
 * Frees (all) the memory associated with the given JSON object.
 */
void
hypr_json_free(hypr_json json)
{
    if (json != NULL) {
        json_object_put(json);
    }
}

/*
 * Returns a JSON object for the given property name in the given JSON object.
 * The returned JSON object memory is managed by the JSON object and will
 * be freed when the reference count of the JSON object drops to zero.
 */
hypr_json
hypr_json_get_object(hypr_json json, const char *name)
{
    struct json_object *json_object;
    return json_object_object_get_ex(json, name, &json_object) ? json_object : NULL;
}

/*
 * Returns the string value for the given name in the given JSON object,
 * or an empty string if not found. The returned JSON string memory is
 * managed by the JSON object and will be freed when the reference count
 * of the JSON object drops to zero.
 */
const char *
hypr_json_get_string(hypr_json json, const char *name) {
    static const char *fallback = "";
    if ((json == NULL) || (name == NULL)) return fallback;
    struct json_object *json_object = NULL;
    if (!json_object_object_get_ex(json, name, &json_object) || (json_object == NULL)) {
        return fallback;
    }
    const char *result = json_object_get_string(json_object);
    return result != NULL ? result : fallback;
}

/*
 * Returns an integer for the given name property in the given JSON object,
 * Integer type coercion is attempted if the specified property is not a integer;
 * double objects will return their integer conversion; strings will be parsed as
 * integers; if no conversion exists then -1 is returned. And -1 will also be the
 * returned if the given property name does not exist, or the given JSON is NULL.
 */
int
hypr_json_get_int(hypr_json json, const char *name)
{
    static const int fallback = -1;
    if ((json == NULL) || (name == NULL)) return fallback;
    struct json_object *json_object = NULL;
    if (!json_object_object_get_ex(json, name, &json_object) || (json_object == NULL)) {
        return fallback;
    }
    extern int errno; errno = 0;
    int result = json_object_get_int(json_object);
    return (result == 0 && errno == EINVAL) ? fallback : result;
}

/*
 * Returns the number elements in the given JSON array object.
 * Returns -1 if the given JSON object is NULL or is not a JSON array object.
 */
int
hypr_json_get_array_count(hypr_json json)
{
    return (json != NULL && json_object_get_type(json) == json_type_array) ? json_object_array_length(json) : -1;
}

/*
 * Returns the JSON array element object from the given JSON object identified
 * by the given (zero-based) index. Forces the index into range if out of range.
 * Returns NULL if the given JSON object is NULL or is not a JSON array object.
 * Caller NOT responsible for freeing the returned JSON object;
 * it is freed when the given JSON object is freed.
 */
hypr_json
hypr_json_get_array_element(hypr_json json, int index)
{
    int nelements = hypr_json_get_array_count(json);
    if (nelements < 0) return NULL;
    index = index < 0 ? 0 : (index >= nelements ? nelements - 1 : index);
    return json_object_array_get_idx(json, index);
}

/*
 * Returns the JSON array element string from the given JSON object identified
 * by the given (zero-based) index. Forces the index into range if out of range.
 * Returns NULL if the given JSON object is NULL or is not a JSON array object
 * or if the element is not a JSON string. Caller NOT responsible for freeing
 * the returned string; it is freed when the given JSON object is freed.
 */
const char *
hypr_json_get_array_string_element(hypr_json json, int index)
{
    return hypr_json_tostring(hypr_json_get_array_element(json, index));
}

/*
 * Returns a formatted JSON string for the given JSON object.
 * Caller NOT responsible for freeing the returned string;
 * the string is freed when the JSON object is freed.
 */
const char *
hypr_json_format(hypr_json json)
{
    return (json != NULL) ? json_object_to_json_string_ext(json, JSON_C_TO_STRING_SPACED | JSON_C_TO_STRING_PRETTY) : "";
}

/*
 * Returns an unformatted JSON string for the given JSON object.
 * Caller NOT responsible for freeing the returned string;
 * the string is freed when the JSON object is freed.
 */
const char *
hypr_json_noformat(hypr_json json)
{
    return (json != NULL) ? json_object_to_json_string_ext(json, 0) : "";
}

/*
 * Returns the actual string representing the given JSON string object,
 * or NULL if the given object is NULL or is not a JSON string object.
 * Caller NOT responsible for freeing the returned string;
 * the string is freed when the JSON object is freed.
 */
const char *
hypr_json_tostring(hypr_json json_string_object)
{
    return (json_string_object != NULL) &&
           (json_object_get_type(json_string_object) == json_type_string) ?
           json_object_get_string(json_string_object) : "";
}
