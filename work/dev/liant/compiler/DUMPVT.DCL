/* C++ CXX_DUMP_VALUE_TREE_PKG - Interface to DUMPVT routine */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  03.26.92  DGM  003  New value-node scheme.
 *  02.21.91  DGM  002  Updated.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  01.23.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

DUMPV                		entry    (type (CXX_VID_T))
				external (X_DUMPV),
DUMPVT                		entry    (type (CXX_VID_T))
				external (X_DUMPVT),
DVT                		entry    (type (CXX_VID_T))
				external (X_DVT);

