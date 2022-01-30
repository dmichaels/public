/* C++ CXX_INITIALIZE_STRING_PKG - String initialization interface */

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
 *  03.26.92  DGM  006  New value-node scheme.
 *  11.12.91  DGM  005  Added argument to INITIALIZE_STRING.
 *  06.25.91  PKT  004  Changed for revised string literal handling.
 *  03.12.91  DGM  003  Changed MAKE_INITIALIZED_STRING.
 *  03.08.91  DGM  002  Added MAKE_INITIALIZED_STRING.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
 *  04.19.90  DGM  000  Original.
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

INITIALIZE_STRING	entry	 (type (CXX_VID_T),
				  type (BOOL_T))
			returns  (type (NID_T))
			external (X_INITIALIZE_STRING),

MAKE_INITIALIZED_STRING	entry    (character (*) varying)
			returns  (type (NID_T))
			external (X_MAKE_INITIALIZED_STRING),

START_SAVING_STRINGS	entry    ()
			external (X_START_SAVING_STRINGS),

STOP_SAVING_STRINGS	entry    ()
			external (X_STOP_SAVING_STRINGS),

RECYCLE_STRING_SYMBOL	entry	 (type (CXX_VID_T),
				  type (POINTER_T),
				  type (LONG_T))
			returns	 (type (BOOL_T))
			external (X_RECYCLE_STRING_SYMBOL);

