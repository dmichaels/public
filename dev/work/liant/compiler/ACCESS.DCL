/* C++ CXX_ACCESS_CONTROL_PKG - Access control package interface */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  07.08.91  DGM  003  Added IS_ACCESSIBLE_STATIC.
 *  12.20.90  PKT  002  Removed interface to CURRENT_FUNCTION_BASE_ACCESS,
 *			CURRENT_FUNCTION_IS_FRIEND_OF, and
 *			CURRENT_FUNCTION_IS_MEMBER_OF.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  06.29.90  DGM  000  Original.
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

IS_ACCESSIBLE			entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_ACCESSIBLE),

IS_ACCESSIBLE_STATIC		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_ACCESSIBLE_STATIC),

EFFECTIVE_ACCESS		entry    (type (NID_T),
					  type (NID_T),
					  type (NID_T),
					  type (SHORT_T))
				returns  (type (SHORT_T))
				external (X_EFFECTIVE_ACCESS),

IS_FRIEND_OF			entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_FRIEND_OF);

