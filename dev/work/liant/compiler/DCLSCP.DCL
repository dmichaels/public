/* C++ CXX_SCOPE_DECLARATION_PKG - Scope declaration interface */

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
 *  03.13.92  PKT  005	Added REMAKE_PROTOTYPE_SCOPE.
 *  05.15.91  DGM  004  Changed BEGIN_CLASS_SCOPE.
 *  01.10.91  DGM  003  Added parameter to BEGIN_PROTOTYPE_SCOPE.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.04.90  DGM  001  Added SET_CURRENT_SCOPE.
 *  02.15.90  DGM  000  Original.
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

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

BEGIN_GLOBAL_SCOPE		entry
				external (X_BEGIN_GLOBAL_SCOPE),
END_GLOBAL_SCOPE		entry
				external (X_END_GLOBAL_SCOPE),

BEGIN_CLASS_SCOPE		entry    (type (NID_T))
				external (X_BEGIN_CLASS_SCOPE),
END_CLASS_SCOPE			entry
				external (X_END_CLASS_SCOPE),

BEGIN_FUNCTION_SCOPE		entry    (type (NID_T))
				external (X_BEGIN_FUNCTION_SCOPE),
END_FUNCTION_SCOPE		entry
				external (X_END_FUNCTION_SCOPE),

BEGIN_BLOCK_SCOPE		entry
				external (X_BEGIN_BLOCK_SCOPE),
END_BLOCK_SCOPE			entry
				external (X_END_BLOCK_SCOPE),

BEGIN_PROTOTYPE_SCOPE		entry    (type (NID_T))
				external (X_BEGIN_PROTOTYPE_SCOPE),
END_PROTOTYPE_SCOPE		entry
				external (X_END_PROTOTYPE_SCOPE),
REMAKE_PROTOTYPE_SCOPE		entry	 (type (NID_T), type (NID_T),
					  type (BOOL_T))
				external (X_REMAKE_PROTOTYPE_SCOPE);

