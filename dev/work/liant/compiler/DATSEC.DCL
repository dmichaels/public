/* C++ CXX_PRAGMA_DATA_SECTION_PKG - #pragma data_section interface */

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
 *  07.01.92  DGM  002R Added CLEANUP_DATA_SECTIONS.
 *  06.29.92  DGM  002  Added SET_CURRENT_DATA_SECTION & SET_DATA_SECTION.
 *  06.25.92  DGM  001  Updated.
 *  06.18.92  DGM  000  Original (for Sequoia LPI-C++).
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

%include CXX_PRAGMA_DATA_SECTION_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

SET_INITIAL_DATA_SECTION	entry    ()
				external (X_SET_INITIAL_DATA_SECTION),

SET_CURRENT_DATA_SECTION	entry    ()
				external (X_SET_CURRENT_DATA_SECTION),

SET_DATA_SECTION		entry    (type (NID_T),
					  type (POINTER_T))
				external (X_SET_DATA_SECTION),

REGISTER_PRAGMA_DATA_SECTION	entry    (type (POINTER_T))
				external (X_REGISTER_PRAGMA_DATA_SECTION),

DATA_SECTION_NAME		entry    (type (NID_T))
				returns  (character (512) varying)
				external (X_DATA_SECTION_NAME),

CLEANUP_DATA_SECTIONS		entry    ()
				external (X_CLEANUP_DATA_SECTIONS);

