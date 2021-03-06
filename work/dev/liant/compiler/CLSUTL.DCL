/* C++ CXX_CLASS_UTL_PKG - class operations support interface      */

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
 *  04.30.92  PKT  017	Added RESTORE_FUNCTION_VALUE.
 *  03.26.92  DGM  016	New value-node scheme.
 *  02.06.92  PKT  015	Added PLAIN_VALUE_OF_REF.
 *  01.10.92  PKT  014	Added FETCH_VALUE.
 *  09.25.91  PKT  013  Added RESTORE_ARRAY_VALUE.
 *  09.06.91  PKT  012  Added BYPASS_COMMA.
 *  06.04.91  PKT  011  Made CALL_CONSTRUCTOR external.
 *  03.27.91  PKT  010  Added context to GET_CONSTRUCTOR_VALUE.
 *			Added DETACH_COMMA and REATTACH_COMMA.
 *  01.11.91  PKT  009  Changed GET_CONSTRUCTOR_VALUE interface.
 *  12.17.90  PKT  008  Improved interface to TRANSMUTE_WITH_TEMP.
 *  11.01.90  PKT  007  Added EFFECT_BEFORE_VALUE and CONCATENATE_STATEMENTS;
 *			took VALUE_BEFORE_EFFECT from dstroy.dcl.
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  09.05.90  PKT  005  Added DECLARE_BUILT_INS.
 *  08.08.90  DGM  004  New type-node; changed GET_CONSTRUCTOR_VALUE.
 *  08.07.90  PKT  003  Moved destruction support to dstroy.dcl.
 *			Added HAS_CONSTRUCTOR and CONSTRUCTOR_OF_TYPE.
 *  07.10.90  PKT  002  Added more destruction support.
 *  07.05.90  PKT  001  Added destructor routines.
 *  05.04.90  PKT  000  Original.
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

HAS_CONSTRUCTOR			entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_HAS_CONSTRUCTOR),

CONSTRUCTOR_OF_TYPE		entry    (type (NID_T),
					  type (SHORT_T))
				returns  (type (NID_T))
				external (X_CONSTRUCTOR_OF_TYPE),

CALL_CONSTRUCTOR		entry    (type (NID_T),
					  type (CXX_VID_T),
				          type (NID_T),
					  type (BOOL_T))
				returns  (type (CXX_VID_T))
				external (X_CALL_CONSTRUCTOR),

GET_CONSTRUCTOR_VALUE		entry    (type (NID_T),
					  type (CXX_VID_T),
					  type (NID_T),
					  type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_GET_CONSTRUCTOR_VALUE),

TRANSMUTE_WITH_TEMP		entry    (type (CXX_VID_T),
					  type (CXX_VID_T), 
					  type (BOOL_T),
					  type (SHORT_T),
					  type (SHORT_T))
				returns	 (type (CXX_VID_T))
				external (X_TRANSMUTE_WITH_TEMP),


INITIALIZE_OBJECT		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_INITIALIZE_OBJECT),

DECLARE_BUILT_INS		entry    ()
				external (X_DECLARE_BUILT_INS),

VALUE_BEFORE_EFFECT		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_VALUE_BEFORE_EFFECT),

EFFECT_BEFORE_VALUE		entry    (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_EFFECT_BEFORE_VALUE),

CONCATENATE_STATEMENTS		entry	 (type (CXX_VID_T),
					  type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_CONCATENATE_STATEMENTS),

DETACH_COMMA			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
				          type (CXX_VID_T))
				external (X_DETACH_COMMA),

REATTACH_COMMA			entry    (type (CXX_VID_T),
					  type (CXX_VID_T),
				          type (CXX_VID_T))
				external (X_REATTACH_COMMA),

BYPASS_COMMA			entry    ()
				external (X_BYPASS_COMMA),

RESTORE_ARRAY_VALUE		entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T))
				external (X_RESTORE_ARRAY_VALUE),

RESTORE_FUNCTION_VALUE		entry    (type (CXX_VID_T),
					  type (TYPE_INFO_T))
				external (X_RESTORE_FUNCTION_VALUE),

FETCH_VALUE			entry	 (type (CXX_VID_T),
					  type (NID_T))
				returns	 (type (CXX_VID_T))
				external (X_FETCH_VALUE),

PLAIN_VALUE_OF_REF		entry	 (type (CXX_VID_T))
				returns  (type (CXX_VID_T))
				external (X_PLAIN_VALUE_OF_REF);

