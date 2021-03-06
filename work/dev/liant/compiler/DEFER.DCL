/* C++ CXX_DEFER_FUNCTION_PKG - Defer function processing package */

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
 *  08.31.92  DGM  004	Changed NOTE_DEFERRED_FUNCTION
 *			to NOTE_DEFERRED_CURRENT_FUNCTION.
 *  08.11.92  DGM  003	Added CLEANUP_DEFERRED_FUNCTIONS.
 *  06.15.92  DGM  002	Changed NOTE_DEFERRED_MEMBER_FUNCTIONS.
 *  06.11.92  DGM  001	Added PARSING_DEFERRED_FUNCTION.
 *  05.19.92  DGM  000	Original.
 *
 ************************************************************************/

declare

NOTE_DEFERRED_CURRENT_FUNCTION	entry    ()
				external (X_NOTE_DEFERRED_CURRENT_FUNCTION),

NOTE_DEFERRED_MEMBER_FUNCTIONS	entry    (type (NID_T),
					  type (POINTER_T))
				external (X_NOTE_DEFERRED_MEMBER_FUNCTIONS),

PARSE_DEFERRED_FUNCTIONS	entry    ()
				returns  (type (LONG_T))
				external (X_PARSE_DEFERRED_FUNCTIONS),

PARSING_DEFERRED_FUNCTION	entry    ()
				returns  (type (BOOL_T))
				external (X_PARSING_DEFERRED_FUNCTION),

CLEANUP_DEFERRED_FUNCTIONS	entry    ()
				external (X_CLEANUP_DEFERRED_FUNCTIONS);

