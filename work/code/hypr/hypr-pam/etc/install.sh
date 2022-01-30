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

OS=`grep '^ID=' /etc/os-release | sed 's/ID=//' | sed 's/"//g'`

case $OS in
   *ubuntu* | *debian* | *kali* | *mint* ) PAM_INSTALL_DIR="/usr/lib/`arch`-linux-gnu/security" ;;
   *centos* | *rhel* | *fedora* | *amzn* ) PAM_INSTALL_DIR="/usr/lib64/security"                ;;
   *opensuse* | *sles* )                   PAM_INSTALL_DIR="/lib64/security"                    ;;
   *)                                      PAM_INSTALL_DIR="/usr/lib64/security"                ;;
esac

PAM_BUILD_PATH=$DIR/src/pam/.libs/pam_hypr.so
PAM=`basename $PAM_BUILD_PATH`
PAM_INSTALL_PATH=$PAM_INSTALL_DIR/$PAM

echo ">>> Installing $PAM from $PAM_BUILD_PATH into $PAM_INSTALL_PATH ($OS)"

if [ ! -f $PAM_BUILD_PATH ] ; then
    echo "Not found: $PAM_BUILD_PATH"
    exit 1
fi

# https://stackoverflow.com/questions/32766609/libtool-installation-issue-with-make-install
# sudo make DESTDIR=/stagedir libsecuritydir=$PAM_INSTALL_DIR install
#
( cd $DIR ; sudo make prefix=/usr libsecuritydir=$PAM_INSTALL_DIR install 2>&1 | fgrep -v "warning: remember to run" ; ls -l $PAM_INSTALL_PATH )
