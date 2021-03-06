/* C++ CXX_CONVERT_CLASS_POINTER_PKG - Pointer conversion package interface */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  03.26.92  DGM  005  New value-node scheme.
 *  12.17.90  PKT  004	Extended the interface
 *			to DERIVED_TO_BASE_CLASS_POINTER.
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  07.03.90  DGM  002  Updated.
 *  06.29.90  DGM  001  Updated.
 *  06.25.90  DGM  000  Original.
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

DERIVED_TO_BASE_CLASS_POINTER	entry     (type (CXX_VID_T),
					   type (NID_T),
					   type (NID_T),
					   type (NID_T),
					   type (NID_T),
					   type (BOOL_T),
					   type (SHORT_T))
				returns   (type (BOOL_T))
				external  (X_DERIVED_TO_BASE_CLASS_POINTER),

BASE_TO_DERIVED_CLASS_POINTER	entry     (type (CXX_VID_T),
					   type (NID_T),
					   type (NID_T),
					   type (BOOL_T))
				returns   (type (BOOL_T))
				external  (X_BASE_TO_DERIVED_CLASS_POINTER),

BASE_CLASS_OFFSET		entry     (type (NID_T),
					   type (NID_T),
					   type (LONG_T))
				returns   (type (SHORT_T))
				external  (X_BASE_CLASS_OFFSET),

NON_VIRTUAL_BASE_CLASS_OFFSET	entry     (type (NID_T),
					   type (NID_T),
					   type (LONG_T))
				returns   (type (SHORT_T))
				external  (X_NON_VIRTUAL_BASE_CLASS_OFFSET);


