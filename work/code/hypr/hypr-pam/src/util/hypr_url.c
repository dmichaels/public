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

#include <netdb.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "hypr_str.h"
#include "hypr_url.h"

static const char *min_pointer(const char *, const char *);

/*
 * Parses the given URL and returns a newly allocated hypr_url object containing
 * the components of the URL, or NULL if it does not look like an URL at all.
 * Caller responsible for freeing the returned hypr_url object via hypr_url_free;
 * components/members of the returned hypr_url should not be changed by caller.
 */
hypr_url *
hypr_url_parse(const char *url)
{
    if (url == NULL) return NULL;

    const char *start = hypr_str_find(url, ":/");

    if ((start == NULL) || (start == url)) return NULL;

    int protocol_length = start - url;
    char *protocol = hypr_str_alloc(protocol_length + 1);
    strncpy((char *) protocol, url, protocol_length);
    protocol[protocol_length] = '\0';

    start = (start != NULL) ? start + 2 : url;
    while (*start == '/') start++;
    if (*start == '\0') {
        hypr_str_free(protocol);
        return NULL;
    }

    hypr_url *result = (hypr_url *) malloc(sizeof(hypr_url));
    result->protocol = protocol;
    result->https    = hypr_str_iequals(result->protocol, "https");
    result->user     = NULL;
    result->host     = NULL;
    result->port     = result->https ? 443 : 80;
    result->path     = NULL;
    result->query    = NULL;

    char *colon    = strchr(start, ':');
    char *slash    = strchr(start, '/');
    char *question = strchr(start, '?');

    const char *after_host = min_pointer(min_pointer(colon, slash), question);

    if (after_host == NULL) {
        result->host = hypr_str_copy(start);
        result->path = hypr_str_copy("/");
        return result;
    }

    int host_length = after_host - start;
    result->host = hypr_str_alloc(host_length + 1);
    strncpy((char *) result->host, start, host_length);
    ((char *) result->host)[host_length] = '\0';

    char *at = strchr(result->host, '@');
    if (at != NULL) {
        int user_length = at - result->host;
        result->user = (char *) hypr_str_alloc(user_length + 1);
        strncpy((char *) result->user, result->host, user_length);
        ((char *) result->user)[user_length] = '\0';
        const char *new_host = hypr_str_copy(at + 1);
        hypr_str_free((char *) result->host);
        result->host = new_host;
    }

    if (after_host == colon) {
        const char *after_colon = min_pointer(slash, question); int port_number;
        if (after_colon != NULL) {
            int port_length = after_colon - colon - 1;
            char *port = hypr_str_alloc(port_length + 1);
            strncpy(port, colon + 1, port_length);
            port[port_length] = '\0';
            port_number = atoi(port);
            hypr_str_free(port);
        }
        else {
            port_number = atoi(colon + 1);
        }
        if (port_number != 0) result->port = port_number;
    }

    if (slash != NULL) {
        if (question != NULL) {
            if (question > slash) {
                int path_length = question - slash;
                result->path = hypr_str_alloc(path_length + 1);
                strncpy((char *) result->path, slash, path_length);
                ((char *) result->path)[path_length] = '\0';
            }
            else result->path = hypr_str_copy("/");
        }
        else result->path = hypr_str_copy(slash);
    }
    else result->path = hypr_str_copy("/");

    if (question != NULL) {
        result->query = hypr_str_copy(question + 1);
    }

    return result;
}

hypr_url *
hypr_url_parse_with_socket_address(const char *url)
{
    hypr_url *parsed_url = hypr_url_parse(url);

    if ((parsed_url != NULL) && !hypr_str_blank(parsed_url->host)) {
        struct hostent *host_entry = gethostbyname(parsed_url->host);
        if (host_entry != NULL) {
            struct sockaddr_in *socket_address = (struct sockaddr_in *) malloc(sizeof(struct sockaddr_in));
            memset(socket_address, 0, sizeof(struct sockaddr_in));
            socket_address->sin_family = AF_INET;
            socket_address->sin_port = htons(parsed_url->port);
            memcpy(&socket_address->sin_addr.s_addr, host_entry->h_addr, host_entry->h_length);
            parsed_url->socket_address = (struct sockaddr *) socket_address;
        }
    }

    return parsed_url;
}

/*
 * Frees all of the memory associated with the given hypr_url object,
 * presumed to have been created via hypr_url_parse.
 */
void
hypr_url_free(hypr_url *url)
{
    if (url != NULL) {
        hypr_str_free((char *) url->protocol);
        hypr_str_free((char *) url->user);
        hypr_str_free((char *) url->host);
        hypr_str_free((char *) url->path);
        hypr_str_free((char *) url->query);
        if (url->socket_address != NULL) {
            free(url->socket_address);
        }
    }
}

/*
 * Returns the minimum pointer value between the two given pointers,
 * where NULL is regarded as a maximum value.
 */
static const char *
min_pointer(const char *a, const char *b)
{
    return (a != NULL) ? ((b != NULL) ? (a < b ? a : b) : a) : b;
}
