/* C++ CXX_INTEGER_VALUE_PKG - Interface to INTEGER_VALUE utility routine  */

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
 *  01.18.91  DGM  003  Added GET_INTEGER_VALUE.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  06.29.90  DGM  001  Added NULL_SIMPLE_POINTER_VALUE.
 *  01.17.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;      */
/* %include CXX_EXTERNAL_NAME_MAP_IN;  */
/* %include CXX_SYMBOL_TABLE_NODES_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

INTEGER_VALUE			entry    (type (LONG_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_INTEGER_VALUE),

INTEGER_VALUE_ZERO		entry    ()
				returns  (type (CXX_VID_T))
				external (X_INTEGER_VALUE_ZERO),

INTEGER_VALUE_ONE		entry    ()
				returns  (type (CXX_VID_T))
				external (X_INTEGER_VALUE_ONE),

GET_INTEGER_VALUE		entry    (type (LONG_T),
					  type (TYPE_DATA_T))
				returns  (type (CXX_VID_T))
				external (X_GET_INTEGER_VALUE),

NULL_SIMPLE_POINTER_VALUE	entry    (type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_NULL_SIMPLE_POINTER_VALUE);

