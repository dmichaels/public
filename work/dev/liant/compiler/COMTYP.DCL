/* C++ CXX_COMPOSITE_TYPE_PKG - Composite type interface */

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
 *  10.03.91  DGM  004  Added GET_ENUM_INTEGRAL_TYPE.
 *  07.08.91  DGM  003  Updated COMPOSITE_EXPRESSION_TYPE.
 *  10.22.90  DGM  002  Updated.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  08.31.90  DGM  000  Fix in declaration of GET_TOP_TYPE_P.
 *  08.01.90  DGM  000  Original.
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

COMPLETE_CURRENT_DECLARATION	entry    ()
				external (X_COMPLETE_CURRENT_DECLARATION),

COMPLETE_ARRAY_TYPE		entry    (type (NID_T),
					  type (LONG_T))
				external (X_COMPLETE_ARRAY_TYPE),

COMPLETE_ARRAY_SYMBOL		entry    (type (NID_T),
					  type (LONG_T))
				external (X_COMPLETE_ARRAY_SYMBOL),

MODIFY_TYPE			entry    (type (NID_T),
					  type (SHORT_T),
					  type (BOOL_T))
				returns  (type (NID_T))
				external (X_MODIFY_TYPE),

MODIFY_TYPE_LEVEL		entry    (type (NID_T),
					  type (SHORT_T),
					  type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_MODIFY_TYPE_LEVEL),

ADD_TOP_TYPE			entry    (type (NID_T),
					  type (TYPE_DATA_T))
				returns  (type (NID_T))
				external (X_ADD_TOP_TYPE),

COMPOSITE_EXPRESSION_TYPE	entry    (type (NID_T),
					  type (SHORT_T),
					  type (NID_T),
					  type (SHORT_T),
					  type (NID_T),
					  type (SHORT_T))
				external (X_COMPOSITE_EXPRESSION_TYPE),

GET_TOP_TYPE			entry    (type (NID_T),
					  type (TYPE_DATA_T))
				external (X_GET_TOP_TYPE),

GET_TOP_TYPE_P			entry    (type (POINTER_T),
					  type (TYPE_DATA_T))
				external (X_GET_TOP_TYPE_P),

GET_TOP_LEVEL_TYPE		entry    (type (NID_T),
					  type (SHORT_T),
					  type (TYPE_DATA_T))
				external (X_GET_TOP_LEVEL_TYPE),

GET_TOP_LEVEL_TYPE_P		entry    (type (POINTER_T),
					  type (SHORT_T),
					  type (TYPE_DATA_T))
				external (X_GET_TOP_LEVEL_TYPE_P),

GET_ENUM_INTEGRAL_TYPE		entry    (type (TYPE_DATA_T))
				returns  (type (SHORT_T))
				external (X_GET_ENUM_INTEGRAL_TYPE);

