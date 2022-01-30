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

DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

# Script to (GPG) sign the specified RPM file.
#
# Usage: sign.sh RPM-file-path-name
#
# Assumes that a KEYID file exists this directory; and that this
# contains the (public) GPG key ID with which to do the signing;
# and that the private key associated with this KEYID has been
# imported into the local GPG key store.
#
# It is the unique (public) GPG key identifier for the (private)
# GPG key which will be used to sign the given RPM file.
#
# GPG (private) keys should be view-able like this:
#
# > gpg --list-secret-keys --keyid-format=short
#
#   /home/dmichaels/.gnupg/pubring.kbx
#   ----------------------------------
#   sec   rsa2048/0EA166EF 2021-05-19 [SC] [expires: 2023-05-19]
#         48BF60ECC2D3BB234BAA37B442C88D530EA166EF
#         uid         [ultimate] David Michaels <david.michaels@hypr.com>
#         ssb   rsa2048/FDFCDEE4 2021-05-19 [E] [expires: 2023-05-19]
#
# In this case the public KEYID would be: 0EA166EF 
#
# Keys can be exported/imported like this:
#
# > gpg --export-secret-keys --armor -a KEYID > private-key.pem
# > gpg --import private-key.pem
#
# On checking the RPM signagure, using the 'rpm --checksig',
# the public GPG have been imported into the RPM key store, like this:
#
# > rpm --import public-key.pem
#
# View keys imported into the RPM key store like this:
#
# > rpm -qa gpg-pubkey*
#   gpg-pubkey-0ea166ef-60a55c28
#
# > rpm -qi gpg-pubkey-0ea166ef-60a55c28
#   This prints key info plus actual public key PEM data.
#
# N.B. If you get odd/unexplaned errors when importing try this:
#
# > gpgconf --kill gpg-agent
# 

if [ $# != 1 ] ; then
    echo "usage: sign.sh rpm-file"
    exit 1
fi

RPM_FILE=$1

if [ ! -f "${RPM_FILE}" ] ; then
    echo "RPM file not found: ${RPM_FILE}"
    exit 2
fi

GPG=/usr/bin/gpg

which ${GPG} > /dev/null 2>&1
if [ $? != 0 ] ; then
    echo "GPG command not found."
    exit 3
fi

if [ ! -f "${DIR}/KEYID" ] ; then
    echo "KEYID file not found: ${DIR}/KEYID"
    exit 4
fi

KEYID=`cat ${DIR}/KEYID`

if [ -z "${KEYID}" ] ; then
    echo "No value found in KEYID file: ${DIR}/KEYID"
    exit 5
fi

${GPG} --list-secret-keys -a "${KEYID}" > /dev/null 2>&1

if [ $? != 0 ] ; then
    echo "Not signing RPM file: ${RPM_FILE}"
    echo "No private GPG key found for: ${KEYID}"
    exit 6
fi

echo
echo "Signing RPM file: ${RPM_FILE}"
echo "Signing with private GPG key: ${KEYID}"
rpmsign \
  --define "_signature gpg" \
  --define "_gpg_name ${KEYID}" \
  --define "_gpgbin ${GPG}" \
  --define "__gpg ${GPG}" \
  --addsign ${RPM_FILE} \
  2>&1 | grep -v "already contains identical signature, skipping"

echo "Checking RPM signature: ${RPM_FILE}"

sudo rpm -qa gpg-pubkey* | grep gpg-pubkey-`echo ${KEYID} | tr '[:upper:]' '[:lower:]'`-

if [ $? != 0 ] ; then
    echo "Cannot check the RPM signature because public GPG key not imported into RPM key store (via rpm --import)"
    exit 7
fi

sudo rpm --checksig ${RPM_FILE}
