/* C++ CXX_MAKE_TEMPORARY_PKG - Interface to make automatic temporary. */

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
 *  11.09.90  PKT  002  Added GET_TEMP_VALUE, GET_TEMP_REF, and DUPLICATE_TEMP.
 *			Also changed MAKE_AUTO_TEMP.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  02.28.90  PKT  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */
/* %include CXX_TYPE_INFO_PKG;        */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

MAKE_AUTO_TEMP			entry	 (type (CXX_VID_T))
				returns  (type (NID_T))
				external (X_MAKE_AUTO_TEMP),

GET_TEMP_VALUE			entry	 (type (TYPE_INFO_T),
					  type (SHORT_T),
					  type (BOOL_T))
				returns	 (type (CXX_VID_T))
				external (X_GET_TEMP_VALUE),

GET_TEMP_REF			entry	 (type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_GET_TEMP_REF),

DUPLICATE_TEMP			entry	 (type (CXX_VID_T))
				returns	 (type (CXX_VID_T))
				external (X_DUPLICATE_TEMP);

