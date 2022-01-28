# Copyright (c) 2021, HYPR Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
#  - Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  - Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# ------------------------------------------------------------------------------

# Makefile.am
# Main autotools based hypr-pam makefile to build the pam_hypr.so shared library.
# This implements HYPR authentication as a Linux PAM.

ACLOCAL_AMFLAGS = -I m4
AM_CPPFLAGS = -I$(top_srcdir)/src -Wno-unused-variable

# ------------------------------------------------------------------------------
# Local (to project, non-installed) libraries.
#
noinst_LTLIBRARIES = \
    src/util/json-c/libjson-c.la \
    src/util/libhypr_util.la \
    src/auth/libhypr_auth.la 

# libjson-c.a
# Internal library of JSON parsing/formatting functions used by hypr-pam;
# this is used (only) by the internal libhypr_util.a library.
# This is built from source cloned and slightly modified directly from:
# https://github.com/json-c/json-c.git, directly into our own hypr-pam source
# This copying/modification is allowed per the MIT License associated with this code.
# Script to copy/adapt this code from the json-c repo into the hypr-pam project is here:
# hypr-pam/etc/misc/copy-json-c-source-into-our-project.sh
#
src_util_json_c_libjson_c_la_SOURCES = \
	src/util/json-c/arraylist.c \
	src/util/json-c/debug.c \
	src/util/json-c/json_c_version.c \
	src/util/json-c/json_object.c \
	src/util/json-c/json_object_iterator.c \
	src/util/json-c/json_pointer.c \
	src/util/json-c/json_tokener.c \
	src/util/json-c/json_util.c \
	src/util/json-c/json_visit.c \
	src/util/json-c/libjson.c \
	src/util/json-c/linkhash.c \
	src/util/json-c/printbuf.c \
	src/util/json-c/random_seed.c \
	src/util/json-c/strerror_override.c
src_util_json_c_libjson_c_la_CPPFLAGS = 

# libhypr_util.a
# Internal library of miscellaneous functions used by hypr-pam.
#
src_util_libhypr_util_la_SOURCES = \
    src/util/hypr_datetime.c \
    src/util/hypr_http.c \
    src/util/hypr_http_libcurl.c \
    src/util/hypr_json.c \
    src/util/hypr_str.c \
    src/util/hypr_url.c \
    src/util/hypr_uuid.c
src_util_libhypr_util_la_CPPFLAGS = \
    -I$(top_srcdir)/src/util/json-c

if HYPR_HTTP_STATIC_LIBCURL
src_util_libhypr_util_la_CPPFLAGS += -DHYPR_HTTP_STATIC_LIBCURL=1
endif

# libhypr_auth.a:
# Internal library of HYPR authentication related functions used by hypr-pam.
#
src_auth_libhypr_auth_la_SOURCES = \
    src/auth/hypr_auth.c \
    src/auth/hypr_auth_devices.c \
    src/auth/hypr_pam_config.c

# ------------------------------------------------------------------------------
# The main event, the HYPR Linux PAM: pam_hypr.so
# Also include a pam_hypr.conf-example file for reference.
#
libsecurity_LTLIBRARIES = src/pam/pam_hypr.la
src_pam_pam_hypr_la_SOURCES = \
    src/pam/pam_hypr.c
src_pam_pam_hypr_la_LIBADD = \
    src/auth/libhypr_auth.la \
	src/util/libhypr_util.la \
	src/util/json-c/libjson-c.la
src_pam_pam_hypr_la_LDFLAGS = \
    -module -shared -no-undefined -avoid-version
if HYPR_HTTP_STATIC_LIBCURL
src_pam_pam_hypr_la_LDFLAGS += -lcurl
endif

pamconfdir=$(sysconfdir)/security
pamconf_DATA = src/pam/conf/etc/security/pam_hypr.conf-example

pamstackdir=$(sysconfdir)/pam.d
pamstack_DATA = src/pam/conf/etc/pam.d/hypr-example

# ------------------------------------------------------------------------------
# Utilities.
#
bin_PROGRAMS = \
    src/cli/hypr
src_cli_hypr_SOURCES = \
    src/cli/hypr.c \
	src/cli/hypr_cli_aliases.c \
	src/cli/hypr_cli_auth.c \
	src/cli/hypr_cli_http.c \
	src/cli/hypr_cli_info.c \
	src/cli/hypr_cli_pam.c
src_cli_hypr_LDADD = \
    src/auth/libhypr_auth.la \
	src/util/libhypr_util.la \
	src/util/json-c/libjson-c.la \
	-lpam_misc \
	-lpam \
	-ldl
if HYPR_HTTP_STATIC_LIBCURL
src_cli_hypr_LDADD += -lcurl
endif

# ------------------------------------------------------------------------------
# Embed git info into source header file: src/version.h
#
BUILT_SOURCES = src/version.h

src/version.h: FORCE
	@rm -f $@
	@echo -n > $@
	@echo "static const char *COPYRIGHT = \"$(shell cat COPYRIGHT)\";" >> $@
	@if [ -s .git/HEAD ] ; then \
		echo -e "static const char *GIT_REPO = \"$(shell git config --get remote.origin.url)\";" >> $@ ; \
		echo -e "static const char *GIT_BRANCH = \"$(shell git rev-parse --abbrev-ref HEAD | sed -e 's/*//' | sed -e 's/ //')\";" >> $@ ; \
		echo -e "static const char *GIT_COMMIT = \"$(shell git rev-parse HEAD)\";" >> $@ ; \
		echo -e "static const char *GIT_COMMIT_DATE = \"$(shell date -d @`git log -1 --pretty=format:%at` +"%Y-%m-%d %H:%M:%S%z")\";" >> $@ ; \
	else \
		echo -e "static const char *GIT_REPO = \"UNKNOWN\";" >> $@ ; \
		echo -e "static const char *GIT_BRANCH = \"UNKNOWN\";" >> $@ ; \
		echo -e "static const char *GIT_COMMIT = \"UNKNOWN\";" >> $@ ; \
		echo -e "static const char *GIT_COMMIT_DATE = \"UNKNOWN\";" >> $@ ; \
	fi
	@echo "static const char *BUILD_DATE = \"$(shell date +"%Y-%m-%d %H:%M:%S%z")\";" >> $@ ;
	@echo "static const char *BUILD_OS = \"$(shell grep '^PRETTY_NAME=' /etc/os-release | sed 's/PRETTY_NAME=//' | sed 's/\"//g')\";" >> $@ ;
	@echo "static const char *BUILD_OS_VERSION = \"$(shell grep '^VERSION=' /etc/os-release | sed 's/VERSION=//' | sed 's/\"//g')\";" >> $@ ;
	@echo "static const char *BUILD_ARCH = \"$(shell arch)\";" >> $@ ;

.PHONY: FORCE
FORCE:
