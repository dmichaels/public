/* C++ CXX_BUILD_TYPE_PKG - Type builder interface */

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
 *  04.18.91  DGM  008  Added BUILD_CURRENT_MEMBER_TYPE.
 *  10.12.90  DGM  007  Added BUILD_CURRENT_FRIEND_TYPE.
 *  10.10.90  DGM  006  Added BUILD_CURRENT_TYPEDEF_TYPE.
 *  09.25.90  DGM  005  New interface; added BUILD_CURRENT_NEW_TYPE
 *			and BUILD_CURRENT_PARAMETER_TYPE.
 *  09.14.90  DGM  004  Changed include file names and VOP names.
 *  09.05.90  PKT  003  Added parameter to BUILD_CURRENT_TYPE.
 *  08.08.90  DGM  002  New type-node.
 *  05.25.90  DGM  001  Added COMPLETE_ARRAY_SYMBOL & COMPLETE_SYMBOL.
 *  05.17.90  DGM  000  Original.
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

BUILD_CURRENT_TYPE		entry    ()
				external (X_BUILD_CURRENT_TYPE),

BUILD_CURRENT_PARAMETER_TYPE	entry    ()
				external (X_BUILD_CURRENT_PARAMETER_TYPE),

BUILD_CURRENT_FRIEND_TYPE	entry    ()
				external (X_BUILD_CURRENT_FRIEND_TYPE),

BUILD_CURRENT_TYPEDEF_TYPE	entry    ()
				external (X_BUILD_CURRENT_TYPEDEF_TYPE),

BUILD_CURRENT_TAG_TYPE		entry    ()
				external (X_BUILD_CURRENT_TAG_TYPE),

BUILD_CURRENT_CONVERSION_TYPE	entry    ()
				external (X_BUILD_CURRENT_CONVERSION_TYPE),

BUILD_CURRENT_NEW_TYPE		entry    ()
				external (X_BUILD_CURRENT_NEW_TYPE),

BUILD_CURRENT_MEMBER_TYPE	entry    ()
				external (X_BUILD_CURRENT_MEMBER_TYPE);

