/* C++ C_REFCHK__PKG - Reference type initialization routines */

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
 *  03.26.92  DGM  004  New value-node scheme.
 *  09.28.90  PKT  003  Changed declaration of INIT_REF_CHECK.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  08.07.90  PKT  001  Updated for destruction.
 *  06.15.90  TLF  000  Original.
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

INIT_REF_CHECK		entry    (type (TYPE_INFO_T),
				  type (CXX_VID_T), 
				  type (SHORT_T),
				  type (SHORT_T),
				  type (NID_T),
				  type (CXX_VID_T))
			returns  (type (CXX_VID_T))
			external (X_INIT_REF_CHECK);

