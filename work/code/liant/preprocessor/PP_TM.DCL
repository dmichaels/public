/* CPP CPP_TOKEN_POD_MGR_PKG - Token pod stream manager package */

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
 *  09.25.90  DGM  002  Include file name changes.
 *  06.05.90  DGM  001  First CPP component version.
 *  --------------------------------------------------------------------
 *  06.05.90  DGM  001  Removed include of C_TOKEN_POD_MGR_IN and TINIT.
 *  09.28.89  DGM  000  Original for C++.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CPP_TOKEN_POD_IN;
%include CPP_TOKEN_POD_MGR_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

OPEN_TOKEN_POD_STREAM	entry
			external (X_OPEN_TOKEN_POD_STREAM),

CLOSE_TOKEN_POD_STREAM	entry
			external (X_CLOSE_TOKEN_POD_STREAM),

TPUT			entry   (pointer)
			external,

TGET			entry
			returns (pointer)
			external,

TPEEK			entry
			returns (pointer)
			external,

TSPEEK			entry
			external,

TUNGET			entry   (pointer, pointer)
			external,

TALLOC			entry
			returns (pointer)
			external,

TFREE			entry   (pointer)
			external,

TFREEB			entry
			external,

TFREET			entry   (pointer)
			external,

TFREEL			entry   (pointer)
			external;

declare

SALLOC			entry   (fixed binary (15)) 
			returns (pointer)
			external,

SFREE			entry   (pointer)
			external,

SFREEB			entry
			external;

