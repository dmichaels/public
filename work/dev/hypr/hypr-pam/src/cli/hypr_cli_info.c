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

/*
 * HYPR command-line utility for testing, troublshooting, et cetera.
 */

#include <dlfcn.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <security/pam_ext.h>
#include <security/pam_modules.h>
#include "auth/hypr_auth_defaults.h"
#include "auth/hypr_pam_config.h"
#include "util/hypr_http.h"
#include "util/hypr_str.h"
#include "../version.h"

static void  print_pam_hypr_library_info(const char *arch, bool verbose);
static void  print_pam_hypr_usage_info(bool verbose);
static void  print_pam_hypr_config_info(const char *config_file, bool verbose, bool debug, bool noping);
static void  print_version_info(bool just_version, bool verbose);
static void  print_system_info(const char *arch, bool verbose);

static bool  file_exists(const char *);
static bool  loadable_pam_library(const char *);
static char *find_system_pam_shared_library_directory(const char *arch);
static char *symbolic_link(const char *);
static bool  selinux_enabled();
static int   selinux_nis_enabled_status();
static char *simple_system_command();
static char *get_os();
static char *get_os_version();
static char *get_os_verbose();
static char *get_os_like();
static char *get_arch();
static char *get_pam_config_files_containing_pam_hypr();

static const char *pam_hypr_config_example_file            = "/etc/security/pam_hypr.conf-example";
static const char *pam_hypr_stack_example_file             = "/etc/pam.d/hypr-example";
static const char *pam_hypr_shared_library_file_name       = "pam_hypr.so";
static const char *standard_system_pam_shared_library_name = "pam_unix.so";

static const char *possible_pam_shared_library_directories[] =
{
    "/lib/security",
    "/lib64/security",
    "/lib{arch}/security",
    "/lib/{arch}/security",
    "/lib/{arch}-linux-gnu/security",
    "/usr/lib/security",
    "/usr/lib64/security",
    "/usr/lib{arch}/security",
    "/usr/lib/{arch}/security",
    "/usr/lib/{arch}-linux-gnu/security",
    "/usr/local/lib/security",
    "/usr/local/lib64/security",
    "/usr/local/lib{arch}/security",
    "/usr/local/lib/{arch}/security",
    "/usr/local/lib/{arch}-linux-gnu/security"
};

static const int npossible_pam_shared_library_directories =
                 sizeof(possible_pam_shared_library_directories) /
                 sizeof(possible_pam_shared_library_directories[0]);

static void usage()
{
    fprintf(stderr, "usage: hypr info [--verbose] [--config file]\n");
    exit(1);
}

int hypr_cli_info(int argc, char *argv[], const char *arg)
{
    bool verbose = false, debug = false, noping = false;
    const char *config_file = HYPR_PAM_DEFAULT_CONFIG_FILE;
    bool just_version = hypr_str_iequals(arg, "--version");

    int i;
    for (i = 2 ; i < argc ; i++) {
        if (hypr_str_iequals(argv[i], "--config") || hypr_str_iequals(argv[i], "--conf") ||
            hypr_str_iequals(argv[i], "-config")  || hypr_str_iequals(argv[i], "-conf")) {
            if (++i >= argc) usage();
            config_file = argv[i];
            if (access(config_file, F_OK) != 0) {
                fprintf(stderr, "hypr: cannot open configuration file: %s\n", config_file);
                usage();
            }
        }
        else if (hypr_str_iequals(argv[i], "--verbose") ||
                 hypr_str_iequals(argv[i], "-verbose")) {
            verbose = true;
        }
        else if (hypr_str_iequals(argv[i], "--debug") ||
                 hypr_str_iequals(argv[i], "-debug")) {
            debug = true;
        }
        else if (hypr_str_iequals(argv[i], "--version") ||
                 hypr_str_iequals(argv[i], "-version")) {
            just_version = true;
        }
        else if (hypr_str_iequals(argv[i], "--noping") ||
                 hypr_str_iequals(argv[i], "-noping")) {
            noping = true;
        }
    }

    printf("HYPR Linux PAM %s%s%s:\n", VERSION, verbose ? " | " : "", verbose ? COPYRIGHT : "");

    char *arch = get_arch();

    if (!just_version) {
        print_pam_hypr_library_info(arch, verbose);
    }

    if (!just_version) {
        print_pam_hypr_usage_info(verbose);
    }

    if (!just_version && verbose) {
        if (file_exists(pam_hypr_stack_example_file )) {
            printf("- HYPR Linux PAM stack example: %s\n", pam_hypr_stack_example_file );
        }
    }

    if (!just_version && verbose) {
        if (file_exists(pam_hypr_config_example_file)) {
            printf("- HYPR Linux PAM configuration example: %s\n", pam_hypr_config_example_file);
        }
    }

    if (!just_version) {
        print_pam_hypr_config_info(config_file, verbose, debug, noping);
    }

    print_version_info(just_version, verbose);

    if (!just_version) {
        print_system_info(arch, verbose);
    }

    hypr_str_free(arch);

    return 0;
}

static void print_pam_hypr_library_info(const char *arch, bool verbose)
{
    const char *pam_hypr_so_path;

    int npam_hypr_found = 0, i;
    for (i = 0 ; i < npossible_pam_shared_library_directories ; i++) {
        char *pam_hypr = hypr_str_cat(hypr_str_cat(hypr_str_replace(possible_pam_shared_library_directories[i], "{arch}", arch, false), "/", true), pam_hypr_shared_library_file_name , true);
        if (file_exists(pam_hypr)) {
            npam_hypr_found++;
            bool loadable = loadable_pam_library(pam_hypr);
            char *realpath = symbolic_link(pam_hypr);
            if (realpath != NULL) {
                printf("- HYPR Linux PAM library: %s%s -> %s\n", pam_hypr, loadable ? " (OK)" : " (unloadable)", realpath);
                hypr_str_free(realpath);
            }
            else {
                printf("- HYPR Linux PAM library: %s%s\n", pam_hypr, loadable ? " (OK)" : " (unloadable)");
                char *system_pam_shared_library_directory = find_system_pam_shared_library_directory(arch);
                if (system_pam_shared_library_directory != NULL) {
                    char *preferred_pam_hypr_location =
                          hypr_str_cat(hypr_str_cat(system_pam_shared_library_directory, "/", false), pam_hypr_shared_library_file_name , true);
                    if (!hypr_str_equals(preferred_pam_hypr_location, pam_hypr)) {
                        printf("- WARNING: The %s shared library does not seem to be installed\n", pam_hypr_shared_library_file_name);
                        printf("  in the standard system PAM library directory: %s\n", system_pam_shared_library_directory);
                        printf("  May not be loadable by PAM enabled applications.\n");
                    }
                    hypr_str_free(system_pam_shared_library_directory);
                }
                else {
                    printf("- WARNING: Cannot find the standard system PAM library directory.\n");
                    printf("  I.e. e.g. the directory containing the file: %s\n", standard_system_pam_shared_library_name);
                }
            }
        }
        hypr_str_free(pam_hypr);
    }
    if (npam_hypr_found == 0) {
        printf("- HYPR Linux PAM library: missing\n");
        printf("- WARNING: The %s shared library cannot be found.\n", pam_hypr_shared_library_file_name);
    }
}

static void print_pam_hypr_usage_info(bool verbose)
{
    char *pam_config_files_containing_pam_hypr = get_pam_config_files_containing_pam_hypr();
    printf("- HYPR Linux PAM stack usages%s: %s\n",
        verbose ? " in /etc/pam.d" : "",
        !hypr_str_blank(pam_config_files_containing_pam_hypr) ?
            pam_config_files_containing_pam_hypr : "none");
    hypr_str_free(pam_config_files_containing_pam_hypr);
}

static void print_pam_hypr_config_info(const char *config_file, bool verbose, bool debug, bool noping)
{
    if (file_exists(config_file)) {
        printf("- HYPR Linux PAM configuration: %s\n", config_file);
    }
    else {
        printf("- HYPR Linux PAM configuration: %s (not found)\n", config_file);
    }

    // Read the HYPR Linux PAM configuration file (pam_hypr.conf).

    hypr_pam_config *pam_config = hypr_pam_config_read(config_file, NULL, NULL, NULL);
    if (pam_config != NULL) {
        if (!hypr_str_blank(pam_config->libcurl_path)) {
            hypr_http_libcurl_path_global_set(pam_config->libcurl_path);
        }
        if (pam_config->disabled) {
            printf("- HYPR Linux PAM disabled: true\n");
        }
        else {
            printf("- HYPR Linux PAM enabled: true\n");
        }
        if (hypr_http_static_libcurl()) {
            if (verbose) {
                printf("- HYPR Linux PAM libcurl: static\n");
            }
        }
        else {
            if (!hypr_str_blank(pam_config->libcurl_path)) {
                printf("- HYPR Linux PAM libcurl path: %s\n", pam_config->libcurl_path);
            }
            bool libcurl_okay = hypr_http_libcurl_okay();
            char *libcurl_path = hypr_http_libcurl_path();
            if (!hypr_str_blank(libcurl_path)) {
                char *libcurl_path_symlink = symbolic_link(libcurl_path);
                if (!hypr_str_blank(libcurl_path_symlink)) {
                    hypr_str_free(libcurl_path);
                    libcurl_path = libcurl_path_symlink;
                }
                if (verbose) {
                    printf("- HYPR Linux PAM libcurl: dynamic%s -> %s%s\n",
                        hypr_http_libcurl_path_using_alternate() ? " (alternate)" : "",
                        libcurl_path, libcurl_okay ? " (OK)" : " (not loadable)"); 
                }
            }
            else {
                if (verbose) {
                    printf("- HYPR Linux PAM libcurl: dynamic %s\n", libcurl_okay ? " (OK)" : "(not loadable)"); 
                }
            }
            hypr_str_free(libcurl_path);
            if (!libcurl_okay) {
                //
                // The libcurl.so library is not found!
                //
                printf("- WARNING: Required libcurl.so shared library cannot be found or loaded.\n");
            }
        }

        if (pam_config->auth_config != NULL) {
            if (hypr_str_iequals(pam_config->auth_config->access_token, "REDACTED") ||
                hypr_str_iequals(pam_config->auth_config->access_token, "<REDACTED>")) {
                printf("- HYPR authentication access-token: unset (REDACTED)\n");
            }
            else if (hypr_str_blank(pam_config->auth_config->access_token)) {
                if (verbose) {
                    printf("- HYPR Linux PAM authentication access-token: unset\n");
                }
            }
            else {
                int access_token_length = strlen(pam_config->auth_config->access_token);
                printf("- HYPR Linux PAM authentication access-token: %c*******%c\n",
                    pam_config->auth_config->access_token[0],
                    pam_config->auth_config->access_token[access_token_length -1]);
            }
            if (!hypr_str_blank(pam_config->auth_config->api_access_token)) {
                int api_access_token_length = strlen(pam_config->auth_config->api_access_token);
                printf("- HYPR Linux PAM API access-token: %c*******%c\n",
                    pam_config->auth_config->api_access_token[0],
                    pam_config->auth_config->api_access_token[api_access_token_length -1]);
            }
            if (!hypr_str_blank(pam_config->auth_config->base_url)) {
                //
                // See if we can actually access this URL.
                //
                if (!hypr_str_istarts_with(pam_config->auth_config->base_url, "http://") &&
                    !hypr_str_istarts_with(pam_config->auth_config->base_url, "https://")) {
                    printf("- HYPR Linux PAM base-url: %s (malformed)\n", pam_config->auth_config->base_url);
                }
                else if (verbose && !noping) {
                    const int timeout_ms = 7 * 1000;
                    char *trimmed_base_url = hypr_str_rtrim(pam_config->auth_config->base_url, '/', false);
                    char *hypr_ping_url = hypr_str_cat(trimmed_base_url, "/info", false);
                    if (debug) {
                        printf("- HYPR API ping: %s\n", hypr_ping_url);
                    }
                    char *hypr_ping_response = NULL;
                    int http_status = hypr_http_get(hypr_ping_url, NULL, timeout_ms, &hypr_ping_response);
                    char *hypr_ping_version = NULL;
                    if (!hypr_str_blank(hypr_ping_response)) {
                        hypr_json hypr_ping_response_json = hypr_json_parse(hypr_ping_response);
                        hypr_ping_version = hypr_str_copy(hypr_json_get_string(hypr_ping_response_json, "Version"));
                        hypr_json_free(hypr_ping_response_json);
                    }
                    if (debug) {
                        printf("- HYPR API ping response HTTP code: %d\n", http_status);
                        printf("- HYPR API ping response size: %d\n", !hypr_str_blank(hypr_ping_response) ? (int) strlen(hypr_ping_response) : 0);
                        printf("- HYPR API ping response: [%s]\n", hypr_ping_response);
                        hypr_str_free(hypr_ping_response);
                    }
                    if (http_status == 200) {
                        //
                        // The version returns by locally running ControlCenter can be "Unavailable".
                        //
                        if (!hypr_str_blank(hypr_ping_version) && !hypr_str_iequals(hypr_ping_version, "Unavailable")) {
                            printf("- HYPR Linux PAM base-url: %s (OK) %s\n", pam_config->auth_config->base_url, hypr_ping_version);
                        }
                        else {
                            printf("- HYPR Linux PAM base-url: %s (OK)\n", pam_config->auth_config->base_url);
                        }
                    }
                    else {
                        printf("- HYPR Linux PAM base-url: %s (not reachable)\n", pam_config->auth_config->base_url);
                    }
                    hypr_str_free(hypr_ping_version);
                    hypr_str_free(trimmed_base_url);
                    hypr_str_free(hypr_ping_url);
                }
                else {
                    printf("- HYPR Linux PAM base-url: %s\n", pam_config->auth_config->base_url);
                }
            }
            else if (verbose) {
                printf("- HYPR Linux PAM base-url: unset\n");
            }
            if (!hypr_str_blank(pam_config->auth_config->endpoint_path)) {
                printf("- HYPR Linux PAM endpoint-path: %s\n", pam_config->auth_config->endpoint_path);
            }
            else if (verbose) {
                printf("- HYPR Linux PAM endpoint-path: unset\n");
            }
            if (!hypr_str_blank(pam_config->auth_config->app_id)) {
                printf("- HYPR Linux PAM app-id: %s\n", pam_config->auth_config->app_id);
            }
            else if (verbose) {
                printf("- HYPR Linux PAM app-id: unset\n");
            }
            if (verbose) {
                if (pam_config->auth_config->poll_max_time_seconds > 0) {
                    printf("- HYPR Linux PAM polling time: %d seconds\n", pam_config->auth_config->poll_max_time_seconds);
                }
                if (pam_config->auth_config->poll_max_time_seconds > 0) {
                    printf("- HYPR Linux PAM polling interval: %d seconds\n", pam_config->auth_config->poll_interval_seconds);
                }
                if (pam_config->auth_config->timeout > 0) {
                    printf("- HYPR Linux PAM timeout: %d milliseconds\n", pam_config->auth_config->timeout);
                }
            }
            if (verbose) {
                if (pam_config->username_mappings > 0) {
                    printf("- HYPR Linux PAM username mappings: %d\n", pam_config->username_mappings);
                }
                else {
                    printf("- HYPR Linux PAM username mappings: none\n");
                }
            }
        }
    }
}

static void print_version_info(bool just_version, bool verbose)
{
    if (just_version) {
        printf("- HYPR Linux PAM version: %s\n", VERSION);
    }
    if (!hypr_str_blank(GIT_REPO)) printf("- HYPR Linux PAM git repo: %s\n", GIT_REPO);
    if (!hypr_str_blank(GIT_BRANCH)) printf("- HYPR Linux PAM git branch: %s\n", GIT_BRANCH);
    if (verbose) {
        if (!hypr_str_blank(GIT_COMMIT)) printf("- HYPR Linux PAM git commit: %s\n", GIT_COMMIT);
    }
    if (!hypr_str_blank(GIT_COMMIT_DATE)) printf("- HYPR Linux PAM git date: %s\n", GIT_COMMIT_DATE);
    printf("- HYPR Linux PAM build date: %s\n", BUILD_DATE);
    if (verbose) {
        printf("- HYPR Linux PAM build OS: %s\n", BUILD_OS);
        printf("- HYPR Linux PAM build OS version: %s\n", BUILD_OS_VERSION);
        printf("- HYPR Linux PAM build architecture: %s\n", BUILD_ARCH);
    }
}

static void print_system_info(const char *arch, bool verbose)
{
    if (verbose) {
        char *os = get_os();
        char *os_version = get_os_version();
        char *os_verbose = get_os_verbose();
        char *os_like = get_os_like();
        printf("- Operating system: %s / %s", os_verbose, os);
        if (!hypr_str_blank(os_like)) {
            printf(" -> %s\n", os_like);
        }
        else {
            printf("\n");
        }
        if (!hypr_str_blank(os_version)) {
            printf("- Operating system version: %s\n", os_version);
        }
        printf("- Architecture: %s\n", arch);
        hypr_str_free(os);
        hypr_str_free(os_verbose);
        hypr_str_free(os_like);
    }

    if (selinux_enabled()) {
        if (verbose) {
            printf("- SELinux: enabled\n");
        }
        int selinux_nis_enabled = selinux_nis_enabled_status();
        if (selinux_nis_enabled == 0) {
            if (!verbose) {
                printf("- SELinux: enabled\n");
            }
            printf("- SELinux NIS: disabled\n");
            printf("- WARNING: SELinux seems to be in place and NIS is disabled.\n");
            printf("  Possibly problematic for ssh/scp with HYPR authentication.\n");
            printf("  Can be enabled using: sudo setsebool -P nis_enabled 1\n");
            printf("  Can be viewed using: sestatus -b | grep nis_enabled\n");
        }
        else if (selinux_nis_enabled == 1) {
            if (verbose) {
                printf("- SELinux NIS: enabled\n");
            }
        }
    }
}

/*
 * Returns true iff the given file is shared library loadable as an authentication PAM.
 */
static bool loadable_pam_library(const char *file)
{
    typedef int (*pam_function) (pam_handle_t *pamh, int flags, int argc, const char **argv);
    void *myso = dlopen(file, RTLD_NOW);
    if (myso == NULL) {
       return false;
    }
    pam_function *myfunction = dlsym(myso, "pam_sm_authenticate");
    if (myfunction == NULL) {
        dlclose(myso);
        return false;
    }
    dlclose(myso);
    return true;
}

/*
 * Returns the directory path name of what looks like the standard
 * system PAM shared library directory, or NULL if not found.
 * Caller responsible for freeing the returned string.
 */
static char *find_system_pam_shared_library_directory(const char *arch)
{
    int i;
    for (i = 0 ; i < npossible_pam_shared_library_directories ; i++) {
        char *pam_shared_library_directory =
              hypr_str_replace(possible_pam_shared_library_directories[i], "{arch}", arch, false);
        char *real_pam_shared_library_directory = symbolic_link(pam_shared_library_directory);
        if (real_pam_shared_library_directory != NULL) {
            hypr_str_free(pam_shared_library_directory);
            pam_shared_library_directory = real_pam_shared_library_directory;
        }
        char *standard_system_pam_shared_library =
              hypr_str_cat(hypr_str_cat(pam_shared_library_directory, "/", false), standard_system_pam_shared_library_name, true);
        if (file_exists(standard_system_pam_shared_library)) {
            hypr_str_free(standard_system_pam_shared_library);
            return pam_shared_library_directory;
        }
        hypr_str_free(standard_system_pam_shared_library);
        hypr_str_free(pam_shared_library_directory);
    }

    return NULL;
}

/*
 * Returns true iff the given file path exists.
 */
static bool file_exists(const char *path)
{
    return access(path, F_OK) == 0;
}

/*
 * If the given path is a symbolic link then return the real path to which it refers.
 * Call responsible for freeing the returned string.
 */
static char *symbolic_link(const char *path)
{
    if (hypr_str_blank(path)) {
        return NULL;
    }
    char resolved_path[PATH_MAX + 1];
    if (realpath(path, resolved_path) == NULL) {
        return NULL;
    }
    if (hypr_str_equals(path, resolved_path)) {
        return NULL;
    }
    return hypr_str_copy(resolved_path);
}

/*
 * Returns true iff this seems to an SELinux enabled system.
 */
static bool selinux_enabled()
{
    return system("selinuxenabled 2> /dev/null") == 0;
}

/*
 * Returns 1  iff the SELinux NIS flag seems to exist and is enabled.
 * Returns 0  iff the SELinux NIS flag seems to exist and is disabled.
 * Returns -1 iff the SELinux NIS flag does not seem to exist at all.
 */
static int selinux_nis_enabled_status()
{
    int status = -1;
    const char *selinux_nis_enabled_file = "/sys/fs/selinux/booleans/nis_enabled";
    FILE *f = fopen(selinux_nis_enabled_file , "r");
    if (f != NULL) {
        char result[64];
        if (fgets(result, sizeof(result), f) != NULL) {
            int n = strlen(result);
            if (n > 1) {
                if (result[n - 1] == '\n') result[n - 1] = '\0';
                if (hypr_str_equals(result, "1 1")) {
                    status = 1;
                }
                else {
                    status = 0;
                }
            }
        }
        fclose(f);
    }
    return status;
}

/*
 * Returns the short system OS name (via a 'grep' of the /etc/os-release file).
 * Caller responsible for freeing the returned string.
 */
static char *get_os()
{
    return simple_system_command("grep '^ID=' /etc/os-release | sed 's/ID=//' | sed 's/\"//g'");
}

/*
 * Returns the long system OS name (via a 'grep' of the /etc/os-release file).
 * Caller responsible for freeing the returned string.
 */
static char *get_os_verbose()
{
    return simple_system_command("grep '^PRETTY_NAME=' /etc/os-release | sed 's/PRETTY_NAME=//' | sed 's/\"//g'");
}

/*
 * Returns the system OS version name (via a 'grep' of the /etc/os-release file).
 * Caller responsible for freeing the returned string.
 */
static char *get_os_version()
{
    return simple_system_command("grep '^VERSION=' /etc/os-release | sed 's/VERSION=//' | sed 's/\"//g'");
}

/*
 * Returns the OS "like" name (via a 'grep' of the /etc/os-release file).
 * Caller responsible for freeing the returned string.
 */
static char *get_os_like()
{
    return simple_system_command("grep '^ID_LIKE=' /etc/os-release | sed 's/ID_LIKE=//' | sed 's/\"//g'");
}

/*
 * Returns the system architecture (via the 'arch' command).
 * Caller responsible for freeing the returned string.
 */
static char *get_arch()
{
    return simple_system_command("arch");
}

/*
 * Returns a spaced-separated (sorted) list of /etc/pam.d files containing usages of pam_hypr.so.
 * Caller responsible for freeing the returned string.
 */
static char *get_pam_config_files_containing_pam_hypr()
{
    char command[8192];
    snprintf(command, sizeof(command),
             "grep -v \"^\\s*#\" /etc/pam.conf /etc/pam.d/* 2> /dev/null | grep -v '%s' | grep %s | cut -d: -f1 | sort | sed 's/\\/etc\\/pam.d\\///' | tr '\n' ' '",
             pam_hypr_stack_example_file, pam_hypr_shared_library_file_name);
    return hypr_str_trim(simple_system_command(command), ' ', true);
}

/*
 * Returns the single-line result of a simple given system command.
 * Caller responsible for freeing the returned string.
 */
static char *simple_system_command(const char *command)
{
    if (hypr_str_blank(command)) {
        return NULL;
    }
    FILE *f = popen(command, "r");
    char result[8192]; result[0] = '\0';
    if (fgets(result, sizeof(result), f) != NULL) {
        int n = strlen(result);
        if (result[n - 1] == '\n') result[n - 1] = '\0';
    }
    pclose(f);
    return hypr_str_copy(result);
}
