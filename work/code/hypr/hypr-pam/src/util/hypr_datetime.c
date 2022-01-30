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

#include <stdlib.h>
#include <time.h>
#include "hypr_datetime.h"

/*
 * Returns a date/time string in YYYY-MM-DD hh:mm:ss format
 * for the current date/time in the local timezone.
 * Caller responsible for freeing the returned string.
 * For example, the value 1616377772 would result
 * in a return value of: 2021-03-21T21:49:32Z
 */
char *
hypr_datetime_current()
{
    time_t now; time(&now);
    return hypr_datetime_from_seconds(now);
}

/*
 * Returns a date/time string in YYYY-MM-DD hh:mm:ss format
 * for the current date/time in the UTC timezone.
 * Caller responsible for freeing the returned string.
 * For example, the value 1616377772 would result
 * in a return value of: 2021-03-21T21:49:32Z
 */
char *
hypr_datetime_current_utc()
{
    time_t now; time(&now);
    return hypr_datetime_from_seconds_utc(now);
}

/*
 * Returns a date/time string in YYYY-MM-DD hh:mm:ss format for the
 * given number of seconds since "the epoch" in the local timezone.
 * Caller responsible for freeing the returned string.
 * For example, the value 1616377772 would result
 * in a return value of: 2021-03-21T21:49:32Z
 */
char *
hypr_datetime_from_seconds(long seconds)
{
    const int MAX_LENGTH = 128;
    struct tm *timeinfo = localtime(&seconds);
    char *value = calloc(1, sizeof(char) * MAX_LENGTH);
    strftime(value, MAX_LENGTH, "%Y-%m-%d %H:%M:%S", timeinfo);
    return value;
}

/*
 * Returns a date/time string in YYYY-MM-DD hh:mm:ss format for the
 * given number of seconds since "the epoch" in the UTC timezone.
 * Caller responsible for freeing the returned string.
 * For example, the value 1616377772 would result
 * in a return value of: 2021-03-21T21:49:32Z
 */
char *
hypr_datetime_from_seconds_utc(long seconds)
{
    const int MAX_LENGTH = 128;
    struct tm *timeinfo = gmtime(&seconds);
    char *value = calloc(1, sizeof(char) * MAX_LENGTH);
    strftime(value, MAX_LENGTH, "%Y-%m-%d %H:%M:%S", timeinfo);
    return value;
}

/*
 * Frees the given string assumed to have been allocated
 * by one of the hypr_datetime functions above.
 */
void
hypr_datetime_free(char *value)
{
    if (value != NULL) {
        free((void *) value);
    }
}
