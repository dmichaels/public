#!/bin/bash
#
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

# On CenotOS, at least, as opposed to Ubuntu, it seems one needs to install
# the libjson-c library from source; using yum/rpm methods seem problematic.
#
# This script will do libjson-c build/install; a (yum or manual) install of
# cmake is also done as a part of this as the libjson-c build requires this.
# The relevant artificats are put here:
#
#     /usr/local/include/json-c/json.h
#     /usr/local/lib64/libjson-c.a
#
# Note that sudo access is required to run this.
#
DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

OS=`awk -F= '/^NAME/{print $2}' /etc/os-release | tr '[:upper:]' '[:lower:]' | sed 's/"//g'`
if [[ $OS == centos* ]] ; then
    LINUX_CENTOS=1
elif [[ $OS == ubuntu* ]] ; then
    LINUX_UBUNTU=1
elif [[ $OS == "red hat"* ]] ; then
    LINUX_REDHAT=1
elif [[ $OS == "debian"* ]] ; then
    LINUX_DEBIAN=1
elif [[ $OS == "kali"* ]] ; then
    LINUX_KALI=1
elif [[ $OS == *"mint"* ]] ; then
    LINUX_MINT=1
elif [[ $OS == "amazon"* ]] ; then
    LINUX_AMAZON=1
fi

JSONC_GITHUB_REPO="git@github.com:json-c/json-c.git"
if [ ! -d json-c ] ; then
    echo "Cloning via git the json-c library from $JSONC_GITHUB_REPO"
    # git clone git@github.com:json-c/json-c.git
    git clone https://github.com/json-c/json-c.git
    if [ $? != 0 ] ; then
        echo "Error cloning $JSONC_GITHUB_REPO"
        exit 1
    fi
else
    echo "Directory json-c exists so not cloning via git from from $JSONC_GITHUB_REPO"
fi

# On Red Hat a basic install of 'cmake' (yum install cmake) gets
# a version (2.8.12.2) which does not work to compile json-c
# so manually go get another version (3.20) ... (sorry).
#
CMAKE=cmake
$CMAKE --version > /dev/null 2>&1
if [ $? != 0 ] ; then
    #
    # No 'cmake' installed.
    #
    if [[ $LINUX_CENTOS || $LINUX_REDHAT || $LINUX_AMAZON ]] ; then
        echo "Installing via yum the cmake utility which is needed to build libjson-c"
        sudo yum --assumeyes install cmake
    elif [[ $LINUX_UBUNTU || $LINUX_DEBIAN || $LINUX_KALI || $LINUX_MINT ]] ; then
        echo "Installing via apt the cmake utility which is needed to build libjson-c"
        sudo apt install --yes cmake
    fi
fi

which $CMAKE > /dev/null 2>&1
if [ $? != 0 ] ; then
    NOCMAKE=1
else
    CMAKE_VERSION_OUTPUT=`$CMAKE --version | head -1`
    CMAKE_VERSION=`echo $CMAKE_VERSION_OUTPUT | awk '{print $3}'`
fi
if [[ $NOCMAKE || $CMAKE_VERSION < "3.0.0" ]] ; then
    #
    # Installed version of 'cmake' is too old to build libjson-c.
    #
    if [ $NOCMAKE ] ; then
        echo "No cmake installed which is needed to build libjson-c"
    else
        echo "Installed version of cmake ($CMAKE_VERSION) is too old to build libjson-c"
    fi
    MANUALLY_INSTALLED_CMAKE=cmake-3.20.0-linux-x86_64
    if [ -f $DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake ] ; then
        echo "Already manually installed up-to-date-enough version of cmake (3.20)"
        CMAKE=$DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake
    else
        echo "Manually installing up-to-date-enough version of cmake (3.20)"
        if [ ! -f $MANUALLY_INSTALLED_CMAKE.tar.gz ] ; then
            wget https://cmake.org/files/v3.20/$MANUALLY_INSTALLED_CMAKE.tar.gz
        fi
        tar xf $MANUALLY_INSTALLED_CMAKE.tar.gz
        rm -f $MANUALLY_INSTALLED_CMAKE.tar.gz
        CMAKE=$DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake
    fi
fi

echo "Building json-c library"
( cd json-c ; $CMAKE . ; make ; make test )

echo "Installing json-c library"
( cd json-c ; sudo make install )

echo "Done (hopefully) installing json-c library"
ls -l json-c/libjson-c.a
