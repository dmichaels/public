/* C++ CXX_DECLARE_SYMBOL_PKG - Miscellaneous symbol declaration routines */

/***********************************************************************
 * This product is the property of Liant Software Corporation and is   *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Liant Software Corporation except pursuant to the license agreement.*
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  10.26.91  PKT  015L Updated.
 *  07.18.91  DGM  013  Added DECLARE_MAIN_EXIT_FUNCTION,
 *			DECLARE_MAIN_ENTRY_FUNCTION, & DECLARE_C_FUNCTION.
 *  05.01.91  PKT  012  Added DECLARE_ARRAY_CONSTRUCTOR.
 *  05.03.91  DGM  011  Added DECLARE_FUNCTION.
 *  03.12.91  DGM  010  Changed declaration of DECLARE_ASSIGN_OPERATOR.
 *  11.28.90  TLF  009  Fixed declaration of DECLARE_ASSIGN_OPERATOR
 *			so that it is correctly declared as a function.
 *  10.18.90  TLF  008  Added DECLARE_DEFAULT_CONSTRUCTOR,
 *                      DECLARE_DEFAULT_DESTRUCTOR, and
 *                      DECLARE_ASSIGN_OPERATOR.
 *  10.10.90  DGM  007  Added DECLARE_PURE_VIRTUAL_FUNCTION.
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  09.12.90  DGM  005  Added DECLARE_COPY_CONSTRUCTOR.
 *  09.05.90  PKT  004  Added parameter to DECLARE_IMPLICIT and
 *			DECLARE_IMPLICIT_FUNCTION.
 *  07.10.90  DGM  003  Added GET_VFT_POINTER_NAME.
 *  05.25.90  DGM  002  Changed below to DECLARE_VFT
 *			and added DECLARE_VFT_POINTER.
 *  05.17.90  DGM  001  Added DECLARE_VIRTUAL_FUNCTION_TABLE.
 *  04.19.90  DGM  000  Original.
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

DECLARE_IMPLICIT		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_DECLARE_IMPLICIT),

DECLARE_IMPLICIT_FUNCTION	entry    (type (NID_T), type (SHORT_T))
				returns  (type (NID_T))
				external (X_DECLARE_IMPLICIT_FUNCTION),

DECLARE_C_FUNCTION		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_DECLARE_C_FUNCTION),

DECLARE_FUNCTION		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_DECLARE_FUNCTION),

DECLARE_LABEL			entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_DECLARE_LABEL),

DECLARE_VFT			entry    (type (POINTER_T))
				external (X_DECLARE_VFT),

DECLARE_VFT_POINTER		entry    (type (POINTER_T))
				external (X_DECLARE_VFT_POINTER),

DECLARE_PURE_VIRTUAL_FUNCTION	entry    ()
				returns  (type (NID_T))
				external (X_DECLARE_PURE_VIRTUAL_FUNCTION),

DECLARE_COPY_CONSTRUCTOR	entry    (type (POINTER_T))
				external (X_DECLARE_COPY_CONSTRUCTOR),

DECLARE_DEFAULT_CONSTRUCTOR	entry    (type (POINTER_T))
				external (X_DECLARE_DEFAULT_CONSTRUCTOR),

DECLARE_ARRAY_CONSTRUCTOR	entry    (type (POINTER_T))
				external (X_DECLARE_ARRAY_CONSTRUCTOR),

DECLARE_DEFAULT_DESTRUCTOR	entry    (type (POINTER_T))
				external (X_DECLARE_DEFAULT_DESTRUCTOR),

DECLARE_ASSIGNMENT_OPERATOR	entry    (type (NID_T))
				external (X_DECLARE_ASSIGNMENT_OPERATOR),

DECLARE_MAIN_EXIT_FUNCTION	entry    ()
				returns  (type (NID_T))
				external (X_DECLARE_MAIN_EXIT_FUNCTION),

DECLARE_MAIN_ENTRY_FUNCTION	entry    ()
				returns  (type (NID_T))
				external (X_DECLARE_MAIN_ENTRY_FUNCTION);

