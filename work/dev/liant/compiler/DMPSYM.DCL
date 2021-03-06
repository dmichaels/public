/* C++ CXX_SYMBOL_TABLE_DEBUG_PKG - Symbol table debug/dump interface */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  03.08.91  DGM  007  Updated.
 *  09.25.90  DGM  006  Updated.
 *  09.14.90  DGM  005  Changed include file names and VOP names.
 *  08.29.90  DGM  004  Removed DUMP_TYPES.
 *  06.26.90  DGM  003  Added DUMP_CLASSES.
 *  06.08.90  DGM  002  Added DUMP_SOURCES.
 *  03.27.90  DGM  001  Updated.
 *  02.21.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DUMP_CURRENT_SCOPE_SYMBOLS	entry    ()
				external (X_DUMP_CURRENT_SCOPE_SYMBOLS),

DUMP_SYMBOL			entry    (type (NID_T),
					  type (LONG_BITS_T))
				external (X_DUMP_SYMBOL),

DUMP_CLASSES			entry    ()
				external (X_DUMP_CLASSES);

