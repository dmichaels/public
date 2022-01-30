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

# Assume this here script is two directories down from our main
# project directory (hypr-pam), specifically in in: hypr-pam/etc/packaging.
#
PROJECT_DIR=`readlink -f ${DIR}/../..`
PROJECT_NAME=`basename ${PROJECT_DIR}`

PACKAGE_BASE_NAME=hypr-pam
#
# Get the version from the top-level directory VERSION file.
#
VERSION=`cat ${DIR}/../../VERSION | cut -d. -f1-2`
PACKAGE_NAME=${PACKAGE_BASE_NAME}-${VERSION}
PACKAGE_FILE=${PACKAGE_NAME}.tar.gz
SPEC_FILE=${DIR}/rpm.spec

# Create the tarball for hypr-pam in hypr-pam-{$VERSION}.tar.gz
#
echo "Creating ${PROJECT_NAME} tarball (${PACKAGE_FILE}) from: ${PROJECT_DIR}"

TMP_DIR=/tmp/${PROJECT_NAME}-$$
TMP_PROJECT_DIR=${TMP_DIR}/${PROJECT_NAME}
mkdir -p ${TMP_DIR}
cp -pR ${PROJECT_DIR} ${TMP_DIR}
${TMP_PROJECT_DIR}/etc/clean.sh
rm -rf ${TMP_PROJECT_DIR}/etc ${TMP_PROJECT_DIR}/etc/repos
find ${TMP_PROJECT_DIR} -name makefile-legacy -exec rm -f {} \;
( cd ${TMP_DIR} ; tar cfz ${PACKAGE_FILE} ${PROJECT_NAME} --transform "s/^hypr-pam/${PACKAGE_NAME}/" )

echo "Created ${PROJECT_NAME} tarball: ${TMP_DIR}/${PACKAGE_FILE}"

# Create the rpmbuild directory with the hypr-pam tarball and our hypr-pam.spec file.
#
RPM_BUILD_DIR=${DIR}/rpmbuild
RPM_SPEC_FILE=${RPM_BUILD_DIR}/SPECS/hypr-pam.spec

echo "Creating ${PROJECT_NAME} RPM build directory: ${RPM_BUILD_DIR}"
echo "Copying ${PROJECT_NAME} RPM tarball from: ${TMP_DIR}/${PACKAGE_FILE}"
#echo "Copying ${PROJECT_NAME} RPM spec file from: ${RPM_SPEC_FILE}"

rm -rf ${RPM_BUILD_DIR}
mkdir -p ${RPM_BUILD_DIR}/{BUILD,BUILDROOT,RPMS,SOURCES,SPECS,SRPMS}
cp -p ${TMP_DIR}/${PACKAGE_FILE} ${RPM_BUILD_DIR}/SOURCES
#cp -p ${RPM_SPEC_FILE} ${RPM_BUILD_DIR}/SPECS

# Create the RPM spec file from the rpm.spec file in this directory.
#
echo "Creating ${PROJECT_NAME} RPM spec file (from ${SPEC_FILE}): ${RPM_SPEC_FILE}"
cp -p ${SPEC_FILE} ${RPM_SPEC_FILE}

echo "RPM build directory ready for rpmbuild: ${RPM_BUILD_DIR}"
echo "RPM spec file: ${RPM_SPEC_FILE}"
