#! /bin/sh

# C++ bldmsg.sh - Error/messages file creation script.

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
# 03.25.92  PKT  005  Updated.
# 01.31.92  PKT  004  Updated.
# 12.18.91  PKT  003  Updated.
# 09.11.91  PKT  002  Updated.
# 11.15.90  DGM  001  Revised.
# 11.08.90  DGM  000  Original.
#
# ------------------------------------------------------------------------

# ------------------------------------------------------------------------
# Input/output files
# ------------------------------------------------------------------------

ERROR_INPUT_FILE=errmsg.txt   # Input; CXXFE error message text file
ERROR_CODE_FILE=errcod.in     # Output; CXXFE error code include file
ERROR_OUTPUT_FILE=cxxfe.txt     # Output; MSGS input error data file
ERROR_MESSAGE_FILE=lpicxx.msg # Output; MSGS final output error data file

# ------------------------------------------------------------------------
# Common component message files
# ------------------------------------------------------------------------

COMMON_MESSAGE_FILES="	/LPI/src/msgs/base/util.005.txt		\
			/LPI/src/msgs/base/optim.000.txt	\
			/LPI/src/msgs/base/alloc.000.txt	\
			/LPI/src/msgs/base/cgg.000.txt		\
			/LPI/src/msgs/base/oed.000.txt		\
			/LPI/src/msgs/base/cpp.010.txt		"

# ------------------------------------------------------------------------
# CXXFE error file conversion tool (errmsg.txt --> cxxfe.txt)
# ------------------------------------------------------------------------

CONVERT_ERROR_FILE=/LPI/devel/cxx/Tools/bldmsg

# ------------------------------------------------------------------------
# MSGS message file build tool (cxxfe.txt --> lpicxx.msg)
# ------------------------------------------------------------------------

BUILD_MESSAGE_FILE=/usr/LPI/bin/buildm.016

# ------------------------------------------------------------------------
# Convert the C++ front-end message (errmsg.txt) into a format suitable
# for the MSGS component (cxxfe.txt); also creates a file of error code
# replace constants (errcod.in).
# ------------------------------------------------------------------------

echo "** Making" $ERROR_OUTPUT_FILE "&" $ERROR_CODE_FILE from $ERROR_INPUT_FILE

rm -f $ERROR_OUTPUT_FILE
rm -f $ERROR_CODE_FILE

$CONVERT_ERROR_FILE $ERROR_INPUT_FILE $ERROR_OUTPUT_FILE $ERROR_CODE_FILE

# ------------------------------------------------------------------------
# Build the final message file (lpicxx.msg) from the converted C++
# front-end error file (cxxfe.txt).
# ------------------------------------------------------------------------

echo "** Building" $ERROR_MESSAGE_FILE "from" $ERROR_OUTPUT_FILE

$BUILD_MESSAGE_FILE -o   $ERROR_MESSAGE_FILE	\
		    -msb $COMMON_MESSAGE_FILES	\
			 $ERROR_OUTPUT_FILE

# ------------------------------------------------------------------------
# Done
# ------------------------------------------------------------------------

echo "** Done"

echo -n "** "
ls -l $ERROR_MESSAGE_FILE
echo -n "** "
ls -l $ERROR_OUTPUT_FILE
echo -n "** "
ls -l $ERROR_CODE_FILE
echo -n "** "
ls -l $ERROR_INPUT_FILE

