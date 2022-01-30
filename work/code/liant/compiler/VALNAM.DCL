/* C++ CXX_VALUE_NAMES_PKG - Value-node names interface */

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
 *  03.26.92  DGM  004  New value-node scheme.
 *  10.12.90  DGM  003  Updated.
 *  09.25.90  DGM  002  Updated.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  09.14.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;              */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

VALUE_TYPE_NAME			entry    (type (CXX_VID_T))
				returns  (character (512) varying)
				external (X_VALUE_TYPE_NAME),

ANY_VALUE_TYPE_NAME		entry    (type (CXX_VID_T), type (LONG_BITS_T))
				returns  (character (512) varying)
				external (X_ANY_VALUE_TYPE_NAME),

VALUE_SYMBOL			entry    (type (CXX_VID_T))
				returns  (type (NID_T))
				external (X_VALUE_SYMBOL);


