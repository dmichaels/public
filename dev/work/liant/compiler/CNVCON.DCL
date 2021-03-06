/* C++ CXX_CONVERT_CONSTANT_PKG - Convert constants (FE and CPP) */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  10.08.91  DGM  000  Original.
 *
 ***********************************************************************/

declare

CONVERT_INTEGER_CONSTANT	entry    (character (*) varying,
					  type (SHORT_T),
					  type (SHORT_T),
					  type (SHORT_T),
					  type (BOOL_T),
					  type (LONG_T),
					  type (SHORT_T))
				external (X_CONVERT_INTEGER_CONSTANT),

CONVERT_CHARACTER_CONSTANT	entry    (character (*) varying,
					  type (SHORT_T),
					  type (BOOL_T),
					  type (LONG_T),
					  type (SHORT_T))
				external (X_CONVERT_CHARACTER_CONSTANT),

MAP_STRING_LITERAL		entry    (type (POINTER_T),
					  type (BOOL_T))
				external (X_MAP_STRING_LITERAL),

CONCATENATE_STRING_LITERALS	entry    (type (POINTER_T),
					  type (SHORT_T),
					  type (SHORT_T))
				returns  (type (POINTER_T))
				external (X_CONCATENATE_STRING_LITERALS),


MAP_SOURCE_CHARACTERS		entry    (character (*) varying,
					  type (BOOL_T))
				external (X_MAP_SOURCE_CHARACTERS);

