/* C++ CXX_PRINT_TYPE_PKG - Print value or symbol type interface */

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
 *  09.09.92  DGM  005  Added PRINT_CLASS_CATALOGUE.
 *  03.26.92  DGM  004  New value-node scheme.
 *  05.29.91  DGM  003  Changed PRINT_CLASS_LATTICE.
 *  12.12.90  DGM  002  Moved PRINT_CLASS_LAYOUT to prclay.pl1
 *  10.22.90  DGM  001  Added PRINT_CLASS_LAYOUT & PRINT_CLASS_LATTICE.
 *  09.25.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;                */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_PRINT_TYPE_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

PRINT_VALUE_TYPE		entry    (type (CXX_VID_T),
					  type (LONG_BITS_T),
					  type (LONG_BITS_T))
				external (X_PRINT_VALUE_TYPE),

PRINT_SYMBOL_TYPE		entry    (type (NID_T),
					  type (LONG_BITS_T),
					  type (LONG_BITS_T))
				external (X_PRINT_SYMBOL_TYPE),

PRINT_CLASS_LATTICE		entry    (type (NID_T),
					  type (BOOL_T))
				external (X_PRINT_CLASS_LATTICE),

PRINT_CLASS_CATALOGUE		entry    ()
				external (X_PRINT_CLASS_CATALOGUE);

