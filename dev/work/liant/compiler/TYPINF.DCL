/* C++ CXX_TYPE_INFO_PKG - type information for symbols and values interface */

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
 *  04.18.92  PKT  011	Merged 008->009 with 008->010.
 *  03.26.92  DGM  010  New value-node scheme.
 *  03.30.92  DGM  009  Added TYPE_INFO_NAME.
 *  09.14.90  DGM  008  Changed include file names and VOP names.
 *  08.21.90  DGM  007  Added SET_VALUE_SIMPLE_TYPE_DATA.
 *  08.08.90  DGM  006  Added SET_VALUE_CLASS_POINTER_TYPE.
 *  06.29.90  DGM  005  Added SET_VALUE_CLASS_POINTER_TYPE.
 *  06.25.90  DGM  004  Added SET_VALUE_SIMPLE_POINTER_TYPE.
 *  06.14.90  PKT  003  Added parameter to STRIP_TOP_TYPE.
 *  05.15.90  PKT  002  Added GET_TYPE_NODE_TYPE.
 *  04.17.90  PKT  001  Added SET_VALUE_SIMPLE_TYPE and DIRECT_TYPE_ID.
 *			Added DIMS_AND_SIGS_FROM_INFO.
 *  01.16.90  PKT  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_TYPE_INFO_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

GET_SYMBOL_TYPE			entry    (type (TYPE_INFO_T))
				external (X_GET_SYMBOL_TYPE),

GET_TYPE_NODE_TYPE		entry	 (type (TYPE_INFO_T))
				external (X_GET_TYPE_NODE_TYPE),

GET_VALUE_TYPE			entry    (type (TYPE_INFO_T))
				external (X_GET_VALUE_TYPE),

SET_VALUE_TYPE			entry    (type (TYPE_INFO_T))
				external (X_SET_VALUE_TYPE),

SET_VALUE_SIMPLE_TYPE		entry	 (type (SHORT_T))
				external (X_SET_VALUE_SIMPLE_TYPE),

SET_VALUE_SIMPLE_TYPE_DATA	entry	 (type (TYPE_DATA_T))
				external (X_SET_VALUE_SIMPLE_TYPE_DATA),

SET_VALUE_SIMPLE_POINTER_TYPE	entry	 (type (SHORT_T))
				external (X_SET_VALUE_SIMPLE_POINTER_TYPE),

SET_VALUE_CLASS_POINTER_TYPE	entry	 (type (NID_T))
				external (X_SET_VALUE_CLASS_POINTER_TYPE),

STRIP_TOP_TYPE			entry    (type (TYPE_INFO_T), type (BOOL_T))
				external (X_STRIP_TOP_TYPE),

ADD_POINTER_LEVEL		entry    (type (TYPE_INFO_T))
				external (X_ADD_POINTER_LEVEL),

ADD_REFERENCE_LEVEL		entry    (type (TYPE_INFO_T))
				external (X_ADD_REFERENCE_LEVEL),

DIRECT_TYPE_ID			entry    (type (TYPE_INFO_T))
				returns	 (type (NID_T))
				external (X_DIRECT_TYPE_ID),

DETERMINE_SIZE			entry    (type (TYPE_INFO_T))
				external (X_DETERMINE_SIZE),

GET_TOP_LEVEL_TYPE_INFO		entry    (type (TYPE_INFO_T),
					  type (SHORT_T),
					  type (TYPE_DATA_T))
				external (X_GET_TOP_LEVEL_TYPE_INFO),

TYPE_INFO_NAME			entry    (type (TYPE_INFO_T))
				returns  (character (512) varying)
				external (X_TYPE_INFO_NAME);
