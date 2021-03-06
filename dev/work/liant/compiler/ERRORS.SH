#! /bin/sh

# C++ error.sh - Error file creation script.

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
# 10.10.89  DGM  000  Original; cloned/modified from ANSI-C.
#                     In:  errors.asc
#                     Out: errors.in errtbl.in errtbl.c
#
# ------------------------------------------------------------------------

# ----------------------------------------------------------------------
# Error file building tool used
# ----------------------------------------------------------------------

# BUILD_ERROR_TOOL=/LPI/devel/ansic/Tools/blderr/blderr
BUILD_ERROR_TOOL=/LPI/devel/ansic/paul/Errors/blderr.out

# ----------------------------------------------------------------------
# Files used
# ----------------------------------------------------------------------

INPUT_FILE=errors.asc
ERROR_DEFS_IN=errors.in
ERROR_TABLE_IN=errtbl.in
ERROR_TABLE_IN_TMP=errtbl.in.tmp
ERROR_TABLE_IN_BAK=errtbl.in.bak
ERROR_TABLE_C=errtbl.c
ERROR_TABLE_C_TMP=errtbl.c.tmp

# ------------------------------------------------------------------------
# Run the BLDERR program on "errors.asc".
# ------------------------------------------------------------------------

echo "** Running the blderr tool on errors.asc"

rm -f $ERROR_DEFS_IN
rm -f $ERROR_TABLE_IN
rm -f $ERROR_TABLE_C

$BUILD_ERROR_TOOL << END_OF_BUILD_ERROR_TOOL_INPUT
$INPUT_FILE
$ERROR_TABLE_IN
$ERROR_DEFS_IN
END_OF_BUILD_ERROR_TOOL_INPUT

# ------------------------------------------------------------------------
# Make the copyright notice
# ------------------------------------------------------------------------

echo "** Making copyright notice"

set `date`
COPYRIGHT="\
/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************
 * Copyright (c) $6 Language Processors, Inc.                          *
 ***********************************************************************/
"
set $*

# ------------------------------------------------------------------------
# Remove all but the "%replace" constant declarations from "errtbl.in".
# ------------------------------------------------------------------------

echo "** Backing up $ERROR_TABLE_IN into $ERROR_TABLE_IN_BAK"

rm -f $ERROR_TABLE_IN_BAK
cp    $ERROR_TABLE_IN $ERROR_TABLE_IN_BAK

echo "** Making a new $ERROR_TABLE_IN"

echo "/* C++ errtbl.in - Error table definitions */"    > $ERROR_TABLE_IN_TMP
echo ""           >> $ERROR_TABLE_IN_TMP
echo "$COPYRIGHT" >> $ERROR_TABLE_IN_TMP
echo ""           >> $ERROR_TABLE_IN_TMP

fgrep "%replace" $ERROR_TABLE_IN			 >> $ERROR_TABLE_IN_TMP
echo ""							 >> $ERROR_TABLE_IN_TMP
echo "declare ERRTBL (0:HB_ERRMSG_RECS,0:HB_ERRMSG_LEN)" >> $ERROR_TABLE_IN_TMP
echo "               fixed binary (15) external;"	 >> $ERROR_TABLE_IN_TMP
echo ""							 >> $ERROR_TABLE_IN_TMP

mv $ERROR_TABLE_IN_TMP $ERROR_TABLE_IN

# ------------------------------------------------------------------------
# Put just the initialization of "ERRTBL" into the C header file "errtbl.c"
# ------------------------------------------------------------------------

echo "** Making $ERROR_TABLE_C"

rm -f $ERROR_TABLE_C
cp    $ERROR_TABLE_IN_BAK $ERROR_TABLE_C

fgrep -v "%replace" $ERROR_TABLE_C     > $ERROR_TABLE_C_TMP
fgrep -v "ERRTBL"   $ERROR_TABLE_C_TMP > $ERROR_TABLE_C
fgrep -v "initial"  $ERROR_TABLE_C     > $ERROR_TABLE_C_TMP
fgrep -v "/"	    $ERROR_TABLE_C_TMP > $ERROR_TABLE_C
fgrep -v "*"	    $ERROR_TABLE_C     > $ERROR_TABLE_C_TMP

echo "/* C++ errtbl.c - Error message table */"    > $ERROR_TABLE_C
echo ""                                           >> $ERROR_TABLE_C
echo "$COPYRIGHT"                                 >> $ERROR_TABLE_C
echo ""                                           >> $ERROR_TABLE_C

echo "short ERRTBL [] = {"          >> $ERROR_TABLE_C
cat  $ERROR_TABLE_C_TMP             >> $ERROR_TABLE_C

sed -e "s/(/{/" $ERROR_TABLE_C       > $ERROR_TABLE_C_TMP
sed -e "s/)/}/" $ERROR_TABLE_C_TMP   > $ERROR_TABLE_C

rm -f $ERROR_TABLE_C_TMP

echo "** Done."
echo ""

ls -l $INPUT_FILE $ERROR_DEFS_IN $ERROR_TABLE_IN $ERROR_TABLE_C

