#! /bin/sh

# C++ parse.sh - Script to run the parse tool for the C++ grammar

########################################################################
#  This product is the property of Liant Software Corporation and is   #
#  licensed pursuant to a written license agreement.  No portion of    #
#  this product may be reproduced without the written permission of    #
#  Liant Software Corporation except pursuant to the license agreement.#
########################################################################

# ----------------------------------------------------------------------
#
# LPI EDIT HISTORY
#
# 01.24.92  DGM  Updated to make PAR_ACTION in paract.pl1 an array
#		 of entry variables rather than a procedure.
#		 No longer need to recompile defext after changing grammar.
#
# 01.10.91  DGM  Updated to newest version of parse tool
#		 (allows extra long names).
#
# 04.19.90  DGM  Updated the parse tool to the currently installed one.
#
# 08.30.89  DGM  Modified for C++.
#		 Input:  gram.par
#		 Output: partab.in, paract.pl1
#
# 09.21.87  PLB  Adapted from Fortran version.
#		 Changed table names to conform to CFE usage.
#
# 05.01.87  RFO  Original;
#		 adapted from an acient manuscript by Roger Osmond.
# ----------------------------------------------------------------------

# ----------------------------------------------------------------------
# Parse tool used
# ----------------------------------------------------------------------

#PARSE_TOOL=/LPI/devel/cplus/Tools/parse
PARSE_TOOL=/LPI/targ/sun/tools/parse/bin.008/parse

# ----------------------------------------------------------------------
# Files used
# ----------------------------------------------------------------------

GRAMMAR_FILE=gram.par

PREPROCESSED_GRAMMAR_FILE=/tmp/parse.$$.gram
OUTPUT_FILE=/tmp/parse.$$.out
LISTING_FILE=/tmp/parse.$$.list
PAR_TABLE_IN=partab.in
PAR_ACTION_PLI=paract.pl1

# ----------------------------------------------------------------------
# Set the copyright notice
# ----------------------------------------------------------------------

COPYRIGHT="\
/************************************************************************
 * Copyright (c) 1992 by Liant Software Corporation.                    *
 ************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/
"

# ----------------------------------------------------------------------
# Delete old file versions
# ----------------------------------------------------------------------

echo "*** Cleaning previous parse tool output"

rm -f $OUTPUT_FILE
rm -f $LISTING_FILE
rm -f $PAR_TABLE_IN
rm -f $PAR_ACTION_PLI

# ----------------------------------------------------------------------
# Run the grammar through our MPL macro processor
# ----------------------------------------------------------------------

echo "*** Preprocessing grammar"

MPL_LANGUAGE_FILE=/LPI/src/cxxfe/base/par.lan
MPL_TOOL=/LPI/carme/sd1d/devel/cxx/mpl/build/mpl

$MPL_TOOL -language $MPL_LANGUAGE_FILE \
           $GRAMMAR_FILE -o $PREPROCESSED_GRAMMAR_FILE

# ----------------------------------------------------------------------
# Run parse tool over current input to produce new versions
# ----------------------------------------------------------------------

echo "*** Parsing grammar"

$PARSE_TOOL << END_OF_PARSE_TOOL_INPUT
$PREPROCESSED_GRAMMAR_FILE
$OUTPUT_FILE
$LISTING_FILE
END_OF_PARSE_TOOL_INPUT

# ----------------------------------------------------------------------
# Check for undeclared (misspelled) non-terminal names:  ???
# ----------------------------------------------------------------------

echo "*** Checking for undeclared non-terminals"

grep "L0" $OUTPUT_FILE

# ----------------------------------------------------------------------
# Split the parse tables/actions (produced by the
# PARSE_TOOL) into PAR_TABLE_IN and PAR_ACTION_PLI.
# ----------------------------------------------------------------------

# Determine line number of first line containing a declaration

LN=`grep -i -n -h "DECLARE PENTRY" $OUTPUT_FILE | awk -F: '{print $1}'`

# ----------------------------------------------------------------------
# First part of partbl.in becomes paract.pl1
# ----------------------------------------------------------------------

echo "*** Making $PAR_ACTION_PLI"

PARACT_SED_1=/LPI/src/cxxfe/base/parse1.sed
PARACT_SED_2=/LPI/src/cxxfe/base/parse2.sed
PARACT_SED_3=/LPI/src/cxxfe/base/parse3.sed
TMP_FILE_1=/tmp/parse.tmp.$$.1
TMP_FILE_2=/tmp/parse.tmp.$$.2
TMP_FILE_3=/tmp/parse.tmp.$$.3

head -`expr $LN - 1` $OUTPUT_FILE \
| sed 's/ EXTERNAL / INTERNAL /' >> $TMP_FILE_1

# Transform PAR_ACTION into a function table.

sed -f $PARACT_SED_1 < $TMP_FILE_1 \
| fgrep "PAR_ACTION" | fgrep -v "GOTO PAR_AC" > $TMP_FILE_2
X="`sed -f $PARACT_SED_2 < $TMP_FILE_2 | sort -n -r | head -1`"
echo "$COPYRIGHT" > $TMP_FILE_3
echo "DCL PAR_ACTION (0:" $X ") ENTRY VARIABLE STATIC INTERNAL;" >> $TMP_FILE_3
echo "" >> $TMP_FILE_3
echo "PAR_ACTION_INIT: PROC INTERNAL;" >> $TMP_FILE_3
echo "" >> $TMP_FILE_3
cat $TMP_FILE_2 >> $TMP_FILE_3
rm -f $TMP_FILE_2
echo "END;" >> $TMP_FILE_3
sed -f $PARACT_SED_3 < $TMP_FILE_1 | \
       fgrep -v "PAR_ACTION:" | fgrep -v "PARSE_RET_TRUE:" | fgrep -v "PARSE_RET:" >> $TMP_FILE_3
mv $TMP_FILE_3 $PAR_ACTION_PLI

# ----------------------------------------------------------------------
# Last part of partable.in becomes partab.in
# ----------------------------------------------------------------------

echo "*** Making $PAR_TABLE_IN"

echo "$COPYRIGHT" > $PAR_TABLE_IN
tail +$LN $OUTPUT_FILE | sed 's/ EXTERNAL / INTERNAL /' >> $PAR_TABLE_IN

# ----------------------------------------------------------------------
# Clean up
# ----------------------------------------------------------------------

echo "*** Cleaning up"

rm -f $OUTPUT_FILE
rm -f $LISTING_FILE
#rm -f $PREPROCESSED_GRAMMAR_FILE
rm -f $TMP_FILE_1
rm -f $TMP_FILE_2
rm -f $TMP_FILE_3

# ----------------------------------------------------------------------
# List output files
# ----------------------------------------------------------------------

echo "*** Done"
echo ""

ls -l $GRAMMAR_FILE $PAR_TABLE_IN $PAR_ACTION_PLI

