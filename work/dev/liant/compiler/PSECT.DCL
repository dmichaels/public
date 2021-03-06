/* C++ CXX_PSECT_CONTROL_PKG - Manage operators into various psects */

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
 *  03.26.92  DGM  001  New value-node scheme.
 *  11.07.90  TLF  000  Original.
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

EMIT_CPLUS_TREE			entry    (type (CXX_VID_T))
				returns  (type (OID_T))
				external (X_EMIT_CPLUS_TREE),

ALLOCATE_CPLUS_OPERATOR  	entry (type (SHORT_T), type (SHORT_T))
				external (X_ALLOCATE_CPLUS_OPERATOR),

SET_STARTUP_PSECT		entry
				external (X_SET_STARTUP_PSECT),

SET_CLEANUP_PSECT		entry
				external (X_SET_CLEANUP_PSECT),

SET_PSECT			entry (type (SHORT_T))
				external (X_SET_PSECT),

RESET_PSECT			entry 
				external (X_RESET_PSECT),

SET_PSECT_LINK			entry
				external (X_SET_PSECT_LINK),

GET_STARTUP_INIT_VID		entry
				returns (type (CXX_VID_T))
				external (X_GET_STARTUP_INIT_VID);

