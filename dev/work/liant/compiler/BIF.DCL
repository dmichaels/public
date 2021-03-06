/* C++ CXX_LOOKUP_BUILTIN_PKG - identify built-in functions interface */

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
 *  05.02.91  DGM  002  Changed parameter types.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  03.27.90  PKT  000  Original.
 *
 ***********************************************************************/

declare

ANY_BIF				entry	 (character (512) varying)
				returns	 (type (SHORT_T))
				external (X_ANY_BIF),

STRING_BIF			entry	 (character (512) varying)
				returns	 (type (SHORT_T))
				external (X_STRING_BIF),

MATH_BIF			entry	 (character (512) varying)
				returns	 (type (SHORT_T))
				external (X_MATH_BIF),

VAR_ARG_BIF			entry	 (character (512) varying)
				returns  (type (SHORT_T))
				external (X_VAR_ARG_BIF);

