/* C++ CXX_TYPE_SIZE_PKG - Type size mapping package interface */

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
 *  03.10.92  DGM  009  Added EXTENDED_LONG_DOUBLE_TYPE_SIZE.
 *  11.20.91  DGM  008  Changed include file names and VOP names.
 *  11.19.91  PKT  007L Added BACK_END_TYPE_BY_REFERENCE.
 *  10.03.91  DGM  005  Changed include file names and VOP names.
 *  08.08.90  DGM  004  New type-node.
 *  04.27.90  DGM  003  Added BIT_FIELD_TYPE_SIZE.
 *  02.27.90  PKT  002  Added SUBTYPE_SIZE.
 *  02.09.90  DGM  000  Updated.
 *  02.09.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;			*/
/* %include CXX_STANDARD_DEFS_IN;	*/
/* %include CXX_EXTERNAL_NAME_MAP_IN;	*/
/* %include CXX_SYMBOL_TABLE_IN;	*/

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

TYPE_SIZE			entry    (type (NID_T),
					  type (LONG_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_TYPE_SIZE),

ANY_TYPE_SIZE			entry	 (type (POINTER_T),
					  type (SHORT_T),
					  type (LONG_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_ANY_TYPE_SIZE),

SIMPLE_TYPE_SIZE		entry    (type (SHORT_T),
					  type (LONG_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_SIMPLE_TYPE_SIZE),

BIT_FIELD_TYPE_SIZE		entry    (type (SHORT_T),
					  type (LONG_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_BIT_FIELD_TYPE_SIZE),

BACK_END_TYPE_SIZE		entry    (type (TYPE_DATA_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_BACK_END_TYPE_SIZE),

SIMPLE_BACK_END_TYPE_SIZE	entry    (type (SHORT_T),
					  type (SHORT_T),
					  type (SHORT_T))
				external (X_SIMPLE_BACK_END_TYPE_SIZE),

BACK_END_TYPE_BY_REFERENCE	entry    (type (NID_T))
				returns	 (type (BOOL_T))
				external (X_BACK_END_TYPE_BY_REFERENCE),

EXTENDED_LONG_DOUBLE_TYPE_SIZE	entry    (type (BOOL_T))
				external (X_EXTENDED_LONG_DOUBLE_TYPE_SIZE);

