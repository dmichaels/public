/* CPP CPP_ERROR_MGR_PKG - Interface to diagnostic reporting package */

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
 *  02.10.92  DGM  003  Removed PP_ABORT and PP_WARNING.
 *  09.25.90  DGM  002  Include file name changes.
 *  06.05.90  DGM  001  Updated.
 *  06.05.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include CPP_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CPP_ERROR_CODES_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

SCAN_ERROR		entry    (fixed binary (15))
			external (X_SCAN_ERROR),

SCAN_ERROR_I		entry    (fixed binary (15),
				  character (*) varying)
			external (X_SCAN_ERROR_I),

SCAN_ERROR_ON_LINE	entry    (fixed binary (15),
				  fixed binary (31))
			external (X_SCAN_ERROR_ON_LINE),

SCAN_ERROR_I_ON_LINE	entry    (fixed binary (15),
				  fixed binary (31),
				  character (*) varying)
			external (X_SCAN_ERROR_I_ON_LINE),

PP_ERROR		entry    (fixed binary (15))
			external (X_PP_ERROR),

PP_ERROR_I		entry    (fixed binary (15),
				  character (*) varying)
			external (X_PP_ERROR_I);

