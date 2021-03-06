/* CPP CPP_TOKEN_POD_DEBUG_PKG - Token pod stream manager package */

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
 *  06.05.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CPP_TOKEN_POD_DEBUG_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

DUMP_TOKEN	entry    (pointer)
		external (X_DUMP_TOKEN);

