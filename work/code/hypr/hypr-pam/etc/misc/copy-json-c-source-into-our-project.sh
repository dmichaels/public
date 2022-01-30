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

# Script to copy the salient parts of the json-c source from its GitHub repo,
# https://github.com/json-c/json-c.git, directly into our own hypr-pam source
# directory (hypr-pam/src/util/json-c) for direct compilation and linking
# into our hypr-pam shared library (pam_hypr.so); and for direct check-in into
# our own hypr-pam GitHub repo: https://g.hypr.com/david.michaels/hypr-pam
#
# We do this to eliminate dependency related friction encountered when trying
# to use the json-c shared library (libjson-c.so) across Linux variants, with
# the various ways to install json-c from various distribution repos, and/or
# with the various versions of json-c installed or already present.
#
# Yes, this process will bypass the json-c build process with all its feature
# checks and header build process and so, but it will almost certainly be fine.
# And not using a shared library also technically misses out on updates/fixes,
# et cetera, but our usage of this library is very simple/basic; we could
# almost to the JSON parsing ourselves; which would be another option too.
#
# Another option is/was to build json-c from its source as a static library and
# link to that, but then that would need to be run as part of the build on each
# platform, and that can come with its own complications; there is a script here
# though to do that as well FYI if desired (install-json-c-from-source.sh).
#
# This IS ALLOWED by the MIT licensing associated with the json-c software.
# See the json-c COPYING and README.html files, and the MIT license for details:
#
#   https://raw.githubusercontent.com/json-c/json-c/master/COPYING
#   https://raw.githubusercontent.com/json-c/json-c/master/README.html
#   https://opensource.org/licenses/MIT
#
#   "This program is free software; you can redistribute
#    it and/or modify it under the terms of the MIT License."
#
# This script should (typically) just be run once on a typical Linux instance,
# and then the resultant directory (hypr-pam/src/util/json-c) should be checked
# in into our hypr-pam GitHub repo: https://g.hypr.com/david.michaels/hypr-pam
#
# This script will clone the json-c GitHub repo, and will run cmake to create
# the additional required header files; no additional build/make is required
# for our purposes. We compile this code directly into our hypr-pam project.
#
DIR=$(dirname -- "$(readlink -f -- "$BASH_SOURCE")")

OUR_PROJECT_DIR=`readlink -f $DIR/../..`
OUR_JSON_C_SOURCE_DIR=`readlink -f $OUR_PROJECT_DIR/src/util/json-c`
TMP_DIR=/tmp/hypr-pam-util-json-c-repo

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

JSONC_GITHUB_REPO="https://github.com/json-c/json-c.git"
if [ ! -d $TMP_DIR ] ; then
    echo "Cloning via git the json-c library from $JSONC_GITHUB_REPO into: $TMP_DIR"
    git clone $JSONC_GITHUB_REPO $TMP_DIR
    if [ $? != 0 ] ; then
        echo "Error cloning $JSONC_GITHUB_REPO"
        exit 1
    fi
else
    echo "Directory $TMP_DIR exists so not cloning via git from from $JSONC_GITHUB_REPO"
fi

# On Red Hat a basic install of 'cmake' (yum install cmake) gets
# a version (2.8.12.2) which does not work to compile json-c
# so manually go get another version (3.20) ...
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
    if [ -f $TMP_DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake ] ; then
        echo "Already manually installed up-to-date-enough version of cmake (3.20)"
        CMAKE=$TMP_DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake
    else
        echo "Manually installing up-to-date-enough version of cmake (3.20)"
        if [ ! -f $MANUALLY_INSTALLED_CMAKE.tar.gz ] ; then
            wget https://cmake.org/files/v3.20/$MANUALLY_INSTALLED_CMAKE.tar.gz
        fi
        tar xf $MANUALLY_INSTALLED_CMAKE.tar.gz
        rm -f $MANUALLY_INSTALLED_CMAKE.tar.gz
        CMAKE=$TMP_DIR/$MANUALLY_INSTALLED_CMAKE/bin/cmake
    fi
fi

echo "Running cmake on the json-c project"
( cd $TMP_DIR ; $CMAKE . )

echo "Copying the json-c source code (from: $TMP_DIR) to: $OUR_JSON_C_SOURCE_DIR"
rm -rf $OUR_JSON_C_SOURCE_DIR
mkdir -p $OUR_JSON_C_SOURCE_DIR
cp $TMP_DIR/*.{c,h} $TMP_DIR/COPYING $OUR_JSON_C_SOURCE_DIR
cat > $OUR_JSON_C_SOURCE_DIR/config.h << END_OF_CONFIG_H
#include <limits.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#define HAVE_DECL_INFINITY 1
#define HAVE_DECL_ISINF 1
#define HAVE_DECL_ISNAN 1
#define HAVE_DECL_NAN 1
#define HAVE_FCNTL_H 1
#define HAVE_OPEN 1
#define HAVE_SNPRINTF 1
#define HAVE_STDARG_H 1
#define HAVE_STRDUP 1
#define HAVE_STRNCASECMP 1
END_OF_CONFIG_H
cat > $OUR_JSON_C_SOURCE_DIR/README << END_OF_README
This directory contains the salient parts of the json-c source from its GitHub repo,
https://github.com/json-c/json-c.git, copied directly into our own hypr-pam source
directory (hypr-pam/src/util/json-c) for direct compilation and linking into
our hypr-pam shared library (pam_hypr.so); and for direct check-in into our
own hypr-pam GitHub repo: https://g.hypr.com/david.michaels/hypr-pam
Copy done via: hypr-pam/etc/misc/copy-json-c-source-into-our-project.sh

We do this to eliminate dependency related friction encountered when trying
to use the json-c shared library (libjson-c.so) across Linux variants, with
the various ways to install json-c from various distribution repos, and/or
with the various versions of json-c installed or already present.

Yes, this process will bypass the json-c build process with all its feature
checks and header build process and so on, but it will almost certainly be fine.
And not using a shared library also technically misses out on updates/fixes,
et cetera, but our usage of this library is very simple/basic; we could
almost to the JSON parsing ourselves; which would be another option too.

Another option is/was to build json-c from its source as a static library and
link to that, but then that would need to be run as part of the build on each
platform, and that can come with its own complications; there is a script to
do that if desired here: hypr-pam/etc/misc/install-json-c-from-source.sh.

This copying/modification IS ALLOWED by the MIT licensing associated with json-c.
See the json-c COPYING and README.html files, and the MIT license for details:

  https://raw.githubusercontent.com/json-c/json-c/master/COPYING
  https://raw.githubusercontent.com/json-c/json-c/master/README.html

    "This program is free software; you can redistribute
     it and/or modify it under the terms of the MIT License."

  https://opensource.org/licenses/MIT

    "Permission is hereby granted, free of charge ... without restriction, including without
     limitation rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell ..."
END_OF_README
cat > $OUR_JSON_C_SOURCE_DIR/makefile-legacy << END_OF_MAKEFILE_LEGACY
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

TARGET         = libjson-c.a
SOURCES        = *.c
OBJECTS        = *.o
COMPILER       = gcc
COMPILER_FLAGS = -fPIC
COMPILE        = \$(COMPILER) \$(COMPILER_FLAGS) \$(INCLUDES)

default: \$(TARGET)

\$(TARGET): compile
	ar rcs \$(TARGET) \$(OBJECTS)
	ranlib \$(TARGET) 

compile: clean
	\$(COMPILE) -c \$(SOURCES)

clean:
	rm -rf \$(TARGET) \$(OBJECTS)
END_OF_MAKEFILE_LEGACY

echo "Done copying the json-c source code to: $OUR_JSON_C_SOURCE_DIR"
ls -l $OUR_JSON_C_SOURCE_DIR
