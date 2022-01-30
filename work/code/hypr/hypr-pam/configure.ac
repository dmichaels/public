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

AC_PREREQ([2.69])
AC_INIT([pam_hypr], m4_normalize(m4_include([VERSION])), david.michaels@hypr.com)
#
# Inititialize automake; use 'foreign' option to turn
# off complaints about missing README files, et cetera.
#
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_MACRO_DIR([m4])
AC_PROG_CC
AC_PROG_LIBTOOL
LT_PREREQ([2.4.2])
LT_INIT
AC_PROG_INSTALL

AM_CONDITIONAL([HYPR_HTTP_STATIC_LIBCURL], [test "$HYPR_HTTP_STATIC_LIBCURL" -eq 1])

# Set the target lib/pam directory.
# https://stackoverflow.com/questions/26033155/building-a-pam-module-and-install-into-lib-security
#
AC_ARG_WITH([libsecuritydir],
    [AS_HELP_STRING([--with-libsecuritydir],
        [Directory for PAM modules, pass /lib/security for live install])],
    [], [with_libsecuritydir='$(libdir)/security'])
AC_SUBST([libsecuritydir], [$with_libsecuritydir])

# The configure will generate these Makefiles.
#
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
