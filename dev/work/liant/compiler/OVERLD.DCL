/* C++ C_OVERLD__PKG - function overload and user conversion interface */

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
 *  06.25.92  DGM  007  Added CAN_CONVERT_FROM_CLASS_TO_TYPE.
 *  03.26.92  DGM  006  New value-node scheme.
 *  12.17.91  PKT  005  Removed FIND_USER_CONVERSION.
 *  09.16.91  PKT  004  Added QUIET_USER_CONVERSIONS.
 *  03.04.91  PKT  003  Added IS_OVERLOADED_VALUE.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  08.21.90  TLF  001  Updated.  DGM.
 *  07.23.90  TLF  000  Original.
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

CHECK_USER_CONVERSION	entry    (type (CXX_VID_T),
				  type (SHORT_T),
				  type (NID_T))
			returns  (type (CXX_VID_T))
			external (X_CHECK_USER_CONVERSION),

CALL_USER_CONVERSION    entry    (type (NID_T),
				  type (CXX_VID_T))
			returns  (type (CXX_VID_T))
			external (X_CALL_USER_CONVERSION),

OVERLOAD		entry    (type (CXX_VID_T),
				  type (NID_T),
				  type (NID_T),
				  type (SHORT_T))
			returns  (type (NID_T))
			external (X_OVERLOAD),

USER_CONVERSIONS	entry    (type (NID_T),
				  type (CXX_VID_T),
				  type (BOOL_T),
				  type (BOOL_T))
			returns  (type (NID_T))
			external (X_USER_CONVERSIONS),

QUIET_USER_CONVERSIONS	entry    (type (NID_T),
				  type (CXX_VID_T),
				  type (BOOL_T),
				  type (BOOL_T),
				  type (NID_T))
			returns  (type (NID_T))
			external (X_QUIET_USER_CONVERSIONS),

FIND_OVERLOAD		entry    (type (CXX_VID_T),
				  type (NID_T),
				  type (NID_T),
				  type (SHORT_T),
				  type (BOOL_T))
			returns  (type (NID_T))
			external (X_FIND_OVERLOAD),

IS_OVERLOADED_VALUE	entry    (type (CXX_VID_T),
				  type (NID_T))
			returns  (type (BOOL_T))
			external (X_IS_OVERLOADED_VALUE),

CAN_CONVERT_FROM_CLASS_TO_TYPE	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_CAN_CONVERT_FROM_CLASS_TO_TYPE);

