/* C++ C_DEFAULT_PROC_GENERATION_PKG - Default procedure generation interface */

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
 *  01.10.92  PKT  005	Updated DEFAULT_CONSTRUCTOR_OK and
 *			DEFAULT_DESTRUCTOR_OK.
 *  12.16.91  PKT  004  Updated COPY_CONSTRUCTOR_OK and
 *			ASSIGN_OPERATOR_OK.
 *  09.25.91  PKT  003  Added ASSIGN_OPERATOR_OK and
 *			CHECK_TYPE_TAG_SYMBOL.
 *  07.24.91  DGM  002  Changed GENERATE_DEFAULT_PROCEDURES.
 *  05.07.91  PKT  001  Added NEEDS_ARRAY_CONSTRUCTOR.
 *  09.18.90  TLF  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include Files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

ADD_TO_GENERATE_LIST 		entry 	 (type (NID_T))
				external (X_ADD_TO_GENERATE_LIST),

GENERATE_DEFAULT_PROCEDURES	entry	 ()
				external (X_GENERATE_DEFAULT_PROCEDURES),

DEFAULT_PROCEDURES_TO_GENERATE	entry	 ()
				returns  (type (BOOL_T))
				external (X_DEFAULT_PROCEDURES_TO_GENERATE),

DEFAULT_CONSTRUCTOR_OK  	entry 	 (type (NID_T), type (BOOL_T),
					  type (BOOL_T))
				returns	 (type (BOOL_T))
				external (X_DEFAULT_CONSTRUCTOR_OK),

DEFAULT_DESTRUCTOR_OK  		entry 	 (type (NID_T), type (BOOL_T))
				returns	 (type (BOOL_T))
				external (X_DEFAULT_DESTRUCTOR_OK),

COPY_CONSTRUCTOR_OK  		entry 	 (type (NID_T), type (BOOL_T))
				returns	 (type (BOOL_T))
				external (X_COPY_CONSTRUCTOR_OK),

ASSIGN_OPERATOR_OK		entry	 (type (NID_T), type (BOOL_T))
				returns  (type (BOOL_T))
			       	external (X_ASSIGN_OPERATOR_OK),

CHECK_TYPE_TAG_SYMBOL		entry	 (type (NID_T))
				returns	 (type (NID_T))
				external (X_CHECK_TYPE_TAG_SYMBOL),

ALL_ASSIGN_PARAM_CONST		entry 	 (type (NID_T))
				returns	 (type (BOOL_T))
				external (X_ALL_ASSIGN_PARAM_CONST),

ALL_COPY_PARAM_CONST		entry 	 (type (NID_T))
				returns	 (type (BOOL_T))
				external (X_ALL_COPY_PARAM_CONST),

NEEDS_ARRAY_CONSTRUCTOR		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_NEEDS_ARRAY_CONSTRUCTOR);

