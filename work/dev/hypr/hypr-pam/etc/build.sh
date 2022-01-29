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

HYPR_HTTP_STATIC_LIBCURL=0
for i in "$@" ; do
case $i in
    --static-libcurl)
        HYPR_HTTP_STATIC_LIBCURL=1
        shift
        ;;
    *)
        shift
        ;;
esac
done

echo ">>> Cleaning out source/build directory"
$DIR/etc/clean.sh
mkdir -p $DIR/m4
echo ">>> Auto-reconfiguring source/build directory"
( cd $DIR ; autoreconf -iv )
if [ "$HYPR_HTTP_STATIC_LIBCURL" = "1" ] ; then
    echo ">>> Configuring source/build directory (with static libcurl)"
else
    echo ">>> Configuring source/build directory"
fi
(cd $DIR ; ./configure HYPR_HTTP_STATIC_LIBCURL=$HYPR_HTTP_STATIC_LIBCURL )
echo ">>> Building from source"
( cd $DIR && make ) 
echo ">>> Done "
ls -l $DIR/src/pam/.libs/pam_hypr.so
echo ">>> To install pam_hypr.so into the system directory: $DIR/etc/install.sh"