/* CPP CPP_TOKEN_POD_SPELLING_PKG - Interface to token spelling routines */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  09.25.90  DGM  001  Include file name changes.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  02.22.90  DGM  002  Updated.
 *  11.20.89  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CPP_TOKEN_LIMITS_IN;
%include CPP_TOKEN_POD_SPELLING_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

TOKEN_SPELLING		entry    (pointer)
			returns  (character
				  (MAX_TOKEN_SPELLING_LENGTH) varying)
			external (X_TOKEN_SPELLING),

SIMPLE_TOKEN_SPELLING	entry    (fixed binary (15))
			returns  (character
				  (MAX_TOKEN_SPELLING_LENGTH) varying)
			external (X_SIMPLE_TOKEN_SPELLING);

