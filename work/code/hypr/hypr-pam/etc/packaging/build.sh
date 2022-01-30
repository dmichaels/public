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
PROJECT_REPOS_DIR=${PROJECT_DIR}/etc/repos
PROJECT_NAME=`basename ${PROJECT_DIR}`

# OS info.
#
OS=`grep '^ID=' /etc/os-release | sed 's/ID=//' | sed 's/"//g'`
ARCH=`arch`

case $OS in
   *ubuntu* | *debian* | *kali* | *mint* )
       PAM_INSTALL_DIR="/usr/lib/${ARCH}-linux-gnu/security"
       PACKAGE_TYPE="deb"
       ;;
   *centos* | *rhel* | *fedora* | *amzn* )
       PAM_INSTALL_DIR="/usr/lib64/security"
       PACKAGE_TYPE="rpm"
       ;;
   *opensuse* | *sles* )
       PAM_INSTALL_DIR="/lib64/security"
       PACKAGE_TYPE="rpm"
       OS=opensuse
       ;;
   *)
       PAM_INSTALL_DIR="/usr/lib64/security"
       PACKAGE_TYPE="rpm"
       ;;
esac

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

${DIR}/prepare.sh

RPM_SPEC_FILE=${DIR}/rpmbuild/SPECS/hypr-pam.spec

if [ "$HYPR_HTTP_STATIC_LIBCURL" = "1" ] ; then
    echo "Running rpmbuild with spec file (with static libcurl): ${RPM_SPEC_FILE}"
else
    echo "Running rpmbuild with spec file: ${RPM_SPEC_FILE}"
fi

rpmbuild --define "_libsecuritydir ${PAM_INSTALL_DIR}" --define "_topdir ${DIR}/rpmbuild" --define "HYPR_HTTP_STATIC_LIBCURL $HYPR_HTTP_STATIC_LIBCURL" -ba ${RPM_SPEC_FILE}

if [ $? != 0 ] ; then
    echo "The rpmbuild command failed."
    exit 1
fi

echo "Created these RPM files:"
find ${DIR} -name "*.rpm"

RPM_FILE=`find ${DIR}/rpmbuild/RPMS -name "hypr-pam-*.rpm" | fgrep -v debug | head -1`

${DIR}/sign.sh ${RPM_FILE}

if [ $PACKAGE_TYPE = "deb" ] ; then
    #
    # For Ubuntu/Debian like Linux variants we create a DEB file from the RPM.
    #
    which alien > /dev/null
    if [ $? != 0 ] ; then
        echo
        echo "Need the 'alien' command-line utility to convert RPM to DEB for this Linux variant ($OS): sudo apt install alien"
        exit 1
    fi
    TMP_DIR=/tmp/${PROJECT_NAME}-$$
    mkdir ${TMP_DIR}
    SAVE_PWD=`pwd`
    echo "Changing directory to: ${TMP_DIR}"
    cd ${TMP_DIR}
    if [ ${ARCH} = "aarch64" ] ; then
        echo
        echo "Converting RPM file to DEB (aarch64/arm64) file: $RPM_FILE"
        DEB_PROJECT_VERSION=`rpm -q --info ${RPM_FILE} | grep -i version | awk '{print $NF}' | cut -d. -f1-2`
        DEB_PACKAGE_NAME=${PROJECT_NAME}-${DEB_PROJECT_VERSION}
        cp ${RPM_FILE} ${TMP_DIR}
        echo "Executing: sudo alien -g --scripts ${RPM_FILE}"
        sudo alien -g --scripts ${RPM_FILE}
        if [ $? != 0 ] ; then
            echo "Conversion of RPM file to DEB (aarch64/arm64) file failed: $RPM_FILE (sudo alien -g --scripts ${RPM_FILE})"
            exit 1
        fi
        echo "Changing directory to: ${TMP_DIR}/${DEB_PACKAGE_NAME}"
        cd ${DEB_PACKAGE_NAME}
        if [ ! -f debian/control ] ; then
            echo "The debian/control file does not seem to exist in: ${TMP_DIR}/${DEB_PACKAGE_NAME}"
            exit 1
        fi
        if [ ! -f debian/rules ] ; then
            echo "The debian/rules file does not seem to exist in: ${TMP_DIR}/${DEB_PACKAGE_NAME}"
            exit 1
        fi
        grep aarch64 debian/control > /dev/null 2>&1
        if [ $? = 0 ] ; then
            #
            # For some reason the debian/control file ends up with 'aarch64' as the Architecture and
            # for some reason this is not recognized by the DEB builder (debian/rules), so we change
            # it in place to 'arm64'. The errors looked like this:
            # hypr-pam-1.0-1.aarch64.rpm is for architecture aarch64 ; the package cannot be built on this system
            #
            echo "Changing 'aarch64' to 'arm64' in debian/control file."
            sudo sed -i -e 's/aarch64/arm64/' debian/control
        fi
        sudo debian/rules binary
        if [ $? != 0 ] ; then
            echo "Conversion of RPM file to DEB (aarch64/arm64) file failed: $RPM_FILE (sudo debian/rules binary)"
            exit 1
        fi
        DEB_FILE=`ls ${TMP_DIR}/${PROJECT_NAME}*.deb`
        echo "Converted RPM to DEB (aarch64/arm64) file: ${DEB_FILE}"
        echo "Moving DEB (aarch64/arm64) file to ${PROJECT_REPOS_DIR}: ${PROJECT_REPOS_DIR}/`basename ${DEB_FILE}`"
        mv -f ${DEB_FILE} ${PROJECT_REPOS_DIR}
        DEB_FILE=${PROJECT_REPOS_DIR}/`basename ${DEB_FILE}`
    else
        echo
        echo "Converting RPM file to DEB file: $RPM_FILE"
        sudo alien --scripts ${RPM_FILE}
        if [ $? != 0 ] ; then
            echo "Conversion of RPM file to DEB file failed: $RPM_FILE (sudo alien --scripts ${RPM_FILE})"
            exit 1
        fi
        DEB_FILE=${TMP_DIR}/`ls *.deb` 
        echo "Converted RPM to DEB file: ${DEB_FILE}"
        echo "Moving DEB file to ${PROJECT_REPOS_DIR}: ${PROJECT_REPOS_DIR}/`basename ${DEB_FILE}`"
        mv -f ${DEB_FILE} ${PROJECT_REPOS_DIR}
        DEB_FILE=${PROJECT_REPOS_DIR}/`basename ${DEB_FILE}`
    fi
    echo "Changing directory back to: ${SAVE_PWD}"
    cd ${SAVE_PWD}
    echo
    echo "DEB file:"
    echo "${DEB_FILE}"
    echo
    echo "DEB file contents: ${DEB_FILE}"
    dpkg -c ${DEB_FILE} | grep -v '/.build-id' | grep -v '/$' | awk '{print $(NF)}' | sed 's/^\.\//\//' | sed -e 's/^/+ /'
    echo
    echo "System installation: sudo apt install ${DEB_FILE}" 
    echo "                 or: sudo dpkg -i ${DEB_FILE}" 
    echo "Installation status: sudo apt list ${PROJECT_NAME}" 
    echo "                 or: dpkg -l ${PROJECT_NAME}" 
    echo "Remove installation: sudo apt remove ${PROJECT_NAME}" 
    echo "                 or: sudo apt purge ${PROJECT_NAME}" 
    echo "                 or: sudo dpkg --remove ${PROJECT_NAME}" 
    echo "                 or: sudo dpkg --purge ${PROJECT_NAME}" 
else
    echo
    echo "Copying RPM to ${PROJECT_REPOS_DIR}: ${PROJECT_REPOS_DIR}/`basename ${RPM_FILE}`"
    cp -pf ${RPM_FILE} ${PROJECT_REPOS_DIR}/`basename ${RPM_FILE}`
    RPM_FILE=${PROJECT_REPOS_DIR}/`basename ${RPM_FILE}`
    echo
    echo "RPM file:"
    echo "${RPM_FILE}"
    echo
    echo "RPM file contents: ${RPM_FILE}"
    rpm -qlp ${RPM_FILE} | grep -v '/.build-id' | sed -e 's/^/+ /' # rpm -qlp --noartifact hypr-pam-1.0-1.x86_64.rpm
    echo
    if [ $OS = "opensuse" ] ; then
        echo "System installation: sudo zypper install ${RPM_FILE}" 
        echo "                 or: sudo rpm -ihv ${RPM_FILE}" 
        echo "Installation status: sudo zypper search ${PROJECT_NAME}" 
        echo "                 or: rpm -q ${PROJECT_NAME}" 
        echo "Remove installation: sudo zypper remove ${PROJECT_NAME}" 
        echo "                 or: sudo rpm -e ${PROJECT_NAME}" 
    else
        echo "System installation: sudo yum localinstall ${RPM_FILE}" 
        echo "                 or: sudo yum reinstall ${RPM_FILE}" 
        echo "                 or: sudo rpm -ihv ${RPM_FILE}" 
        echo "Installation status: sudo yum list ${PROJECT_NAME}" 
        echo "                 or: rpm -q ${PROJECT_NAME}" 
        echo "Remove installation: sudo yum remove ${PROJECT_NAME}" 
        echo "                 or: sudo rpm -e ${PROJECT_NAME}" 
    fi
fi
