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
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hypr_str.h"

/*
 * Returns true iff the two given string are equal.
 */
bool
hypr_str_equals(const char *a, const char *b)
{
    return a == NULL ? b == NULL : (b == NULL ? false : strcmp(a, b) == 0);
}

/*
 * Returns true iff the two given string are equal, ignoring case.
 */
bool
hypr_str_iequals(const char *a, const char *b)
{
    return a == NULL ? b == NULL : (b == NULL ? false : strcasecmp(a, b) == 0);
}

/*
 * Returns true iff the given string is NULL or empty.
 */
bool
hypr_str_empty(const char *value)
{
    return (value == NULL) || (value[0] == '\0');
}

/*
 * Returns true iff the given string is NULL or empty or contains only whitespace.
 */
bool
hypr_str_blank(const char *value)
{
    if ((value == NULL) || (value[0] == '\0')) {
        return true;
    }
    const char *p;
    for (p = value ; *p != '\0' ; p++) {
        if (!isspace(*p)) {
            return false;
        }
    }
    return true;
}

/*
 * Returns true iff the given string starts with the given suffix.
 */
bool
hypr_str_starts_with(const char *s, const char *prefix)
{
    if ((s == NULL) || (s[0] == '\0')) {
        return (prefix == NULL) || (prefix[0] == '\0');
    }
    else if ((prefix == NULL) || (prefix[0] == '\0')) {
        return false;
    }
    return strncmp(prefix, s, strlen(prefix)) == 0;
}

/*
 * Returns true iff the given string ends with the given suffix.
 */
bool
hypr_str_ends_with(const char *s, const char *suffix)
{
    if ((s == NULL) || (s[0] == '\0')) {
        return (suffix == NULL) || (suffix[0] == '\0');
    }
    else if ((suffix == NULL) || (suffix[0] == '\0')) {
        return false;
    }

    int s_length = strlen(s);
    int suffix_length = strlen(suffix);

    if (suffix_length > s_length) {
        return false;
    }

    return strncmp(s + s_length - suffix_length, suffix, suffix_length) == 0;
}

/*
 * Returns true iff the given string starts with the given suffix ignoring case.
 */
bool
hypr_str_istarts_with(const char *s, const char *prefix)
{
    if ((s == NULL) || (s[0] == '\0')) {
        return (prefix == NULL) || (prefix[0] == '\0');
    }
    else if ((prefix == NULL) || (prefix[0] == '\0')) {
        return false;
    }
    return strncasecmp(prefix, s, strlen(prefix)) == 0;
}

/*
 * Returns true iff the given string ends with the given suffix.
 */
bool
hypr_str_iends_with(const char *s, const char *suffix)
{
    if ((s == NULL) || (s[0] == '\0')) {
        return (suffix == NULL) || (suffix[0] == '\0');
    }
    else if ((suffix == NULL) || (suffix[0] == '\0')) {
        return false;
    }

    int s_length = strlen(s);
    int suffix_length = strlen(suffix);

    if (suffix_length > s_length) {
        return false;
    }

    return strncasecmp(s + s_length - suffix_length, suffix, suffix_length) == 0;
}

/*
 * Returns a pointer to the first occurrence of the second argument string
 * in the first argument string, or NULL if not found.
 */
const char *
hypr_str_find(const char *haystack, const char *needle)
{
    return (haystack != NULL) && (needle != NULL) ? strstr(haystack, needle) : NULL;
}

/*
 * Returns a pointer to the first occurrence of the second argument string
 * in the first argument string, ignoring case, or NULL if not found.
 */
const char *
hypr_str_ifind(const char *haystack, const char *needle)
{
    return (haystack != NULL) && (needle != NULL) ? strcasestr(haystack, needle) : NULL;
}

/*
 * Returns a newly allocated uninitialzed string of the given length.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_alloc(int length)
{
    return (length > 0) ? (char *) malloc(length) : NULL;
}

/*
 * Returns a newly allocated string which is a copy of the given string.
 * If the given string is NULL then returns a newly allocated empty string.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_copy(const char *s)
{
    if (s == NULL) s = "";
    return strdup(s);
}

/*
 * Returns a newly allocated string based on the given (printf-style) 'format' and arguments.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_new(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    char *result;
    return vasprintf(&result, format, args) != -1 ? result : NULL;
}

/*
 * Returns a newly allocated duplicate of the given 'source' string but
 * with all instances of the given character occurring both at the (left)
 * beginning of the string and at the (right) end of the string removed.
 * Returns NULL if the given source string is NULL. If the 'free_source'
 * argument is true then actually *frees* the given source string.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_trim(const char *source, char c, bool free_source)
{
    return hypr_str_rtrim(hypr_str_ltrim(source, c, free_source), c, true);
}

/*
 * Returns a newly allocated duplicate of the given 'source' string but with all instances
 * of the given character occurring at the (left) beginning of the string removed.
 * Returns NULL if the given source string is NULL. If the 'free_source'
 * argument is true then actually *frees* the given source string.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_ltrim(const char *source, char c, bool free_source)
{
    if (source == NULL) return NULL;
    if ((c == '\0') || (source[0] == '\0')) {
        char *new_string = strdup(source);
        if (free_source) {
            free((void *) source);
        }
        return new_string;
    }

    const char *p;
    for (p = source ; *p != '\0' ; p++) {
        if (*p != c) {
            break;
        }
    }

    char *new_string = hypr_str_copy(p);

    if (free_source) {
        free((void *) source);
    }

    return new_string;
}

/*
 * Returns a newly allocated duplicate of the given 'source' string but with all instances
 * of the given character occurring at the (right) end of the string removed.
 * Returns NULL if the given source string is NULL. If the 'free_source'
 * argument is true then actually *frees* the given source string.
 * Caller responsible for freeing the returned string.
 */
char *
hypr_str_rtrim(const char *source, char c, bool free_source)
{
    if (source == NULL) return NULL;
    if ((c == '\0') || (source[0] == '\0')) {
        char *new_string = strdup(source);
        if (free_source) {
            free((void *) source);
        }
        return new_string;
    }

    int n = strlen(source), i;
    for (i = n - 1 ; i >= 0 ; i--) {
        if (source[i] != c) {
            break;
        }
    }

    int new_string_length = i + 1;
    char *new_string = (char  *) malloc(new_string_length +  1);

    if (new_string == NULL) {
        if (free_source) {
            free((void *) source);
        }
        return NULL;
    }

    strncpy(new_string, source, new_string_length);
    new_string[new_string_length] = '\0';

    if (free_source) {
        free((void *) source);
    }

    return new_string;
}

/*
 * Returns a newly allocated string which is the concatentation of the 'source' and
 * the 'appendage' string arguments. If given source is NULL then assumes empty.
 * If the 'free_source' argument is true then actually *frees* the given source
 * string (if not NULL). Caller responsible for freeing the returned string.
 */
char *hypr_str_cat(const char *source, const char *appendage, bool free_source)
{
    if (source == NULL) source = hypr_str_copy("");
    if (appendage == NULL) appendage = "";
    char *result = hypr_str_new("%s%s", source, appendage);
    if (free_source) free((void *) source);
    return result;
}

/*
 * Returns a newly allocated duplicate of the given 'source' string with all occurrences
 * of the given 'search' string substituted with the given 'replacement' string.
 * Returns NULL if the given source string is NULL. If the 'free_source' argument
 * is true then actually *frees* the given source string.
 * Caller responsible for freeing the returned string.
 */
char *hypr_str_replace(const char *source, const char *search, const char *replacement, bool free_source)
{
    if (source == NULL) {
         return NULL;
    }

    char *new_string;

    if ((source[0] == '\0') || (search == NULL) || (search[0] == '\0')) {
        new_string = strdup(source);
        if (free_source) free((void *) source);
        return new_string;
    }

    if (replacement == NULL) {
        replacement = "";
    }

    int source_length = strlen(source);
    int search_length = strlen(search);
    int replacement_length = strlen(replacement);
    int n = 0;

    // Note that we traverse string twice; once to get the number of occurrences
    // of the search string in the source string, so we know how much to allocate
    // for the new string, and then again to do the actual replacement.

    const char *s = source;
    while (true) {
        char *p = strstr(s, search);
        if (p == NULL) break;
        s = p + search_length; n++;
    }
    if (n == 0) {
        new_string = strdup(source);
        if (free_source) free((void *) source);
        return new_string;
    }

    int new_string_length = source_length - (n * search_length) + (n * replacement_length);
    new_string = (char *) malloc(new_string_length + 1); new_string[0] = '\0';
    char *new_string_end = new_string;

    s = source;
    while (true) {
        const char *p = strstr(s, search);
        if (p == NULL) {
            strcat(new_string_end, s);
            if (free_source) free((void *) source);
            return new_string;
        }
        int search_index = p - s;
        strncat(new_string_end, s, search_index);
        strcat(new_string_end, replacement);
        new_string_end += search_index + replacement_length;
        s += search_index + search_length;
    }

    // Should not get here.

    if (free_source) free((void *) source);
    return strdup(source);
}

/*
 * Frees the given string.
 */
void
hypr_str_free(char *s)
{
    if (s != NULL) free(s);
}
