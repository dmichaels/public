/* C++ CXX_CHECK_ARITHMETIC_PKG - Interface to ADD/SUB/MUL check routines  */

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
 *  03.26.92  DGM  002  New value-node scheme.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  01.22.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

ADD_CHECK		entry    (type (LONG_T),
				  type (LONG_T),
				  type (SHORT_T))
			returns  (type (BOOL_T))
			external (X_ADD_CHECK),

SUBTRACT_CHECK		entry    (type (LONG_T),
				  type (LONG_T),
				  type (SHORT_T))
			returns  (type (BOOL_T))
			external (X_SUBTRACT_CHECK),

MULTIPLY_CHECK		entry    (type (LONG_T),
				  type (LONG_T),
				  type (SHORT_T))
			returns  (type (BOOL_T))
			external (X_MULTIPLY_CHECK);

