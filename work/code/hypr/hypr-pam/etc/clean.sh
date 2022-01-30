#!/bin/bash

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

DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")/..

rm -rf \
    $DIR/aclocal.m4 \
    $DIR/autom4te.cache \
    $DIR/compile \
    $DIR/config.guess \
    $DIR/config.log \
    $DIR/config.status \
    $DIR/config.sub \
    $DIR/configure \
    $DIR/depcomp \
    $DIR/install-sh \
    $DIR/libtool \
    $DIR/ltmain.sh \
    $DIR/m4 \
    $DIR/missing \
    $DIR/*.log \
    $DIR/test-driver \
    $DIR/src/cli/hypr \
    $DIR/src/gitinfo.h

find $DIR -name "*.lo"        -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.Plo"       -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.la"        -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.o"         -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.a"         -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.so"        -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.log"       -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "*.trs"       -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "Makefile"    -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "Makefile.in" -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "main"        -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name "a.out"       -exec rm -f  {} \; > /dev/null 2>&1
find $DIR -name ".deps"       -exec rm -rf {} \; > /dev/null 2>&1
find $DIR -name ".libs"       -exec rm -rf {} \; > /dev/null 2>&1
find $DIR -name ".log"        -exec rm -rf {} \; > /dev/null 2>&1
find $DIR -name ".dirstamp"   -exec rm -rf {} \; > /dev/null 2>&1
find $DIR -name "*.tar.gz"    -exec rm -rf {} \; > /dev/null 2>&1

find $DIR -name "hypr_auth_util" -exec rm -f {} \; > /dev/null 2>&1
find $DIR -name "hypr_pam_util"  -exec rm -f {} \; > /dev/null 2>&1
