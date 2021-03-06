/* C++ CXX_EMIT_CONSTRUCTOR_CODE_PKG - Interface to constructor package */

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
 *  03.26.92  DGM  003  New value-node scheme.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  09.05.90  PKT  001  Added declaration of INITIALIZE_VFT_POINTERS.
 *  07.13.90  DGM  000  Original.
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

EMIT_CONSTRUCTOR_STARTUP_CODE	entry    (type (NID_T))
				external (X_EMIT_CONSTRUCTOR_STARTUP_CODE),

INITIALIZE_VFT_POINTERS		entry	 (type (NID_T),
					  type (NID_T))
				external (X_INITIALIZE_VFT_POINTERS);

