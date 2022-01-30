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

# usage: add-static-libs-to-static-lib.sh target.a source.a...
#
# Adds any of the specified source static libraries to the specified
# target static library. If the the --first argument is specifed then
# only the first specified source static library which exists will be added.

if [ $# -lt 2 ] ; then
    echo "usage: add-static-libs-to-static-lib.sh [--first] target.a source.a..."
    exit 1
fi
if [ $1 = "--first" ] ; then
    FIRST=1
    shift
fi
if [ $# -lt 2 ] ; then
    echo "usage: add-static-libs-to-static-lib.sh [--first] target.a source.a..."
    exit 1
fi

TARGET_LIBRARY=$1

if [ ! -f $TARGET_LIBRARY ] ; then
    echo "Target library ($TARGET_LIBRARY) does not exist"
    exit 0
fi

shift
NADDED=0
for SOURCE_LIBRARY in "$@"
do
    if [ -f "$TARGET_LIBRARY" -a -f "$SOURCE_LIBRARY" ] ; then
        TMP_TARGET_LIBRARY=/tmp/tmp_$TARGET_LIBRARY
        echo "Adding $SOURCE_LIBRARY to $TARGET_LIBRARY"
        ar -M << ________END
            CREATE $TMP_TARGET_LIBRARY
            ADDLIB $TARGET_LIBRARY
            ADDLIB $SOURCE_LIBRARY
            SAVE
            END
________END
        ranlib $TMP_TARGET_LIBRARY
        mv $TMP_TARGET_LIBRARY $TARGET_LIBRARY
        let "NADDED=NADDED+1"
        if [ $FIRST ] ; then
            FIRST=$SOURCE_LIBRARY
            break
        fi
    fi
done
if [ $NADDED -eq 0 ] ; then
    # echo "No libraries added to $TARGET_LIBRARY"
    exit 0
elif [ $NADDED -eq 1 ] ; then
    if [ "$FIRST" ] ; then
        echo "Added 1 static library ($FIRST) to static library $TARGET_LIBRARY"
    else
        echo "Added 1 static library to static library $TARGET_LIBRARY"
    fi
else
    echo "Added $NADDED static libraries to static library $TARGET_LIBRARY"
fi
exit 0
