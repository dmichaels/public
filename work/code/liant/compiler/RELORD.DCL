/* C++ CXX_RELOCATION_DEPENDENCY_PKG - Relocation ordering dependency */

/***********************************************************************
 * This product is the property of Liant Software Corporation and is   *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Liant Software Corporation except pursuant to the license agreement.*
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  05.13.93  DGM  002  No change.
 *  05.13.93  DGM  001  Changed interface to REORDER_RELOCATION_SYMBOLS
 *			to allow for the reordering of local symbols.
 *  10.06.92  DGM  000  Original.
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

NOTE_RELOCATION_DEPENDENCY	entry    (type (NID_T),
					  type (NID_T))
				external (X_NOTE_RELOCATION_DEPENDENCY),

REORDER_RELOCATION_SYMBOLS	entry    (type (BOOL_T))
				external (X_REORDER_RELOCATION_SYMBOLS);

