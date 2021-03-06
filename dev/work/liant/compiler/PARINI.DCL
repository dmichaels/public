/* C++ CXX_PROCESS_INITIALIZER_PKG - Initializer parser interface */

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
 *  06.18.91  PKT  005  Eliminated SET_INITIALIZER_STRING.
 *  10.10.90  DGM  004  Added BEGIN/END_CLASS_INITIALIZER, SET_
 *			INITIALIZER_ARGUMENTS, SET_NULL_INITIALIZER;
 *			removed CHECK_SYMBOL_DEFINITION, MAYBE_
 *			SYMBOL_DEFINITION, INITIALIZE_FROM_ARG_LIST.
 *  09.14.90  DGM  003  Changed include file names and VOP names.
 *  08.07.90  PKT  002  Added CHECK_SYMBOL_DEFINITION and 
 *			MAYBE_SYMBOL_DEFINITION to track defined status
 *			of declared symbols.
 *  05.21.90  PKT  001  Added INITIALIZE_FROM_ARG_LIST.
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

PARSE_INITIALIZER		entry
				external (X_PARSE_INITIALIZER),

SET_INITIALIZER_DELIMITER	entry
				external (X_SET_INITIALIZER_DELIMITER),

SET_INITIALIZER_BEGIN		entry
				external (X_SET_INITIALIZER_BEGIN),

SET_INITIALIZER_END		entry
				external (X_SET_INITIALIZER_END),

SET_INITIALIZER_EXPRESSION	entry
				external (X_SET_INITIALIZER_EXPRESSION),

SET_INITIALIZER_UNKNOWN		entry
				external (X_SET_INITIALIZER_UNKNOWN),

BEGIN_CLASS_INITIALIZER		entry
				external (X_BEGIN_CLASS_INITIALIZER),
SET_INITIALIZER_ARGUMENTS	entry
				external (X_SET_INITIALIZER_ARGUMENTS),
END_CLASS_INITIALIZER		entry
				external (X_END_CLASS_INITIALIZER),
SET_NULL_INITIALIZER		entry
				external (X_SET_NULL_INITIALIZER);
