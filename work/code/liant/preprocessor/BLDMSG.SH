#! /bin/sh

# CPP errmsg.sh - ANSI-C/C++ preprocessor diagnostic messages make script.

# ----------------------------------------------------------------------
#  This product is the property of Language Processors, Inc. and is    -
#  licensed pursuant to a written license agreement.  No portion of    -
#  this product may be reproduced without the written permission of    -
#  Language Processors, Inc. except pursuant to the license agreement. -
# ----------------------------------------------------------------------

# ------------------------------------------------------------------------
#
# LPI EDIT HISTORY
#
# 11.08.90  DGM  000  Original.
#
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Error file building tool used
# ------------------------------------------------------------------------

BUILD_ERROR_TOOL=/LPI/devel/cplus/Tools/bldmsg

# ------------------------------------------------------------------------
# Files used
# ------------------------------------------------------------------------

ERROR_INPUT_FILE=pp_err.txt  # Input; error message text file
ERROR_CODE_FILE=pp_err.in    # Output; error code include file
ERROR_DATA_FILE=cpp.txt      # Output; messages component error data file

# ------------------------------------------------------------------------
# Run the BLDMSG program
# ------------------------------------------------------------------------

echo "** Making" $ERROR_DATA_FILE "&" $ERROR_CODE_FILE from $ERROR_INPUT_FILE

rm -f $ERROR_DATA_FILE
rm -f $ERROR_CODE_FILE

$BUILD_ERROR_TOOL $ERROR_INPUT_FILE $ERROR_DATA_FILE $ERROR_CODE_FILE

echo "** Done"

echo -n "** "
ls -l $ERROR_DATA_FILE
echo -n "** "
ls -l $ERROR_CODE_FILE
echo -n "** "
ls -l $ERROR_INPUT_FILE

