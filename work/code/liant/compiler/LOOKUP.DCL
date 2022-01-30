/* C++ CXX_SYMBOL_TABLE_LOOKUP_PKG - Symbol table look-up interface */

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
 *  08.19.92  DGM  019  Added MATCH_OVERLOAD_FOR_OVERRIDE.
 *  06.11.92  DGM  018  Added SET/CLEAR_LOOKUP_SEQUENCE_RANGE.
 *  07.31.91  DGM  017  Updated CHECK_OVERLOAD.
 *  05.29.91  DGM  016  Removed SEARCH_CLASS_LATTICE (now internal).
 *  05.17.91  PKT  015  Changed CHECK_OVERLOAD.
 *  03.27.91  PKT  014  Added LOOKUP_NAME_QUIETLY.
 *  01.10.91  DGM  013  Added SEARCH_CLASS_FOR_TYPE_QUIETLY.
 *  12.17.90  PKT  012  Added CHECK_BASE_CLASSES.
 *  12.12.90  DGM  011  Added SEARCH_VIRTUAL_BASE_CLASS_LIST.
 *  11.06.90  DGM  010  Removed FIND_VIRTUAL_FUNCTION (see procvf.pl1).
 *  10.12.90  DGM  009  Added LOOKUP_DOV_BASE_CLASS.
 *  09.14.90  DGM  008  Changed include file names and VOP names.
 *  09.05.90  PKT  007  Added LOOKUP_RETURN_PARAMETER.
 *  07.26.90  DGM  006  Added LOOKUP_[CD]TOR_FLAG.
 *  07.23.90  DGM  005  Added GET/DELETE_VIRTUAL_BASE_CLASS_LIST.
 *  07.10.90  DGM  004  Added LOOKUP_VFT_POINTER, LOOKUP_THIS_PARAMETER,
 *			and CHECK_DUPLICATE_BASE_CLASSES.
 *  07.03.90  DGM  003  Updated.
 *  06.29.90  DGM  002  Updated.
 *  06.27.90  DGM  001  Added IS_xxx_BASE_CLASS_OF.
 *			Removed LOOKUP_BASE_CLASS.
 *  05.15.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required include files
/* ------------------------------------------------------------------- */

/* %include GLOBAL_IN;                */
/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

%include CXX_SYMBOL_TABLE_IN;
%include CXX_SYMBOL_TABLE_LOOKUP_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

SEARCH_SCOPE			entry    (type (NID_T),
					  type (NID_T),
					  type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_SEARCH_SCOPE),

SEARCH_SCOPE_FOR_TYPE		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_SEARCH_SCOPE_FOR_TYPE),

SEARCH_LOCAL_SCOPE		entry    (type (NID_T),
					  type (NID_T),
					  type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_SEARCH_LOCAL_SCOPE),

SEARCH_CLASS			entry    (type (NID_T),
					  type (NID_T),
					  type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_SEARCH_CLASS),

SEARCH_CLASS_QUIETLY		entry    (type (NID_T),
					  type (NID_T),
					  type (LONG_BITS_T))
				returns  (type (NID_T))
				external (X_SEARCH_CLASS_QUIETLY),

SEARCH_CLASS_FOR_TYPE		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_SEARCH_CLASS_FOR_TYPE),

SEARCH_CLASS_FOR_TYPE_QUIETLY	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_SEARCH_CLASS_FOR_TYPE_QUIETLY),

LOOKUP_NAME			entry    (type (NID_T),
					  type (LONG_BITS_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_NAME),

LOOKUP_NAME_QUIETLY		entry    (type (NID_T),
					  type (LONG_BITS_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_NAME_QUIETLY),

LOOKUP_TYPE_NAME_QUIETLY	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_TYPE_NAME_QUIETLY),

LOOKUP_LABEL			entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_LABEL),

LOOKUP_RETURN_PARAMETER		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_RETURN_PARAMETER),

LOOKUP_THIS_PARAMETER		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_THIS_PARAMETER),

LOOKUP_CTOR_FLAG_PARAMETER	entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_CTOR_FLAG_PARAMETER),

LOOKUP_DTOR_FLAG_PARAMETER	entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_DTOR_FLAG_PARAMETER),

LOOKUP_VFT_POINTER		entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_VFT_POINTER),

IS_BASE_CLASS_OF		entry    (type (NID_T),
					  type (NID_T),
					  type (BOOL_T),
					  type (BOOL_T),
					  type (BOOL_T))
				returns  (type (BOOL_T))
				external (X_IS_BASE_CLASS_OF),

IS_ANY_BASE_CLASS_OF		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_ANY_BASE_CLASS_OF),

IS_ACCESSIBLE_BASE_CLASS_OF	entry    (type (NID_T),
					  type (NID_T),
					  type (BOOL_T))
				returns  (type (BOOL_T))
				external (X_IS_ACCESSIBLE_BASE_CLASS_OF),

IS_VIRTUAL_BASE_CLASS_OF	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (BOOL_T))
				external (X_IS_VIRTUAL_BASE_CLASS_OF),

CHECK_BASE_CLASSES		entry	 (type (NID_T),
					  type (NID_T),
					  type (NID_T),
					  type (BOOL_T),
					  type (SHORT_T))
				external (X_CHECK_BASE_CLASSES),

LOOKUP_DOV_BASE_CLASS		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_LOOKUP_DOV_BASE_CLASS),

CHECK_OVERLOAD			entry    (type (NID_T),
					  type (NID_T),
					  type (NID_T),
					  type (OCM_T))
				returns  (type (SHORT_T))
				external (X_CHECK_OVERLOAD),

MATCH_OVERLOAD			entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_MATCH_OVERLOAD),

MATCH_OVERLOAD_FOR_OVERRIDE	entry    (type (NID_T),
					  type (POINTER_T))
				returns  (type (NID_T))
				external (X_MATCH_OVERLOAD_FOR_OVERRIDE),

CHECK_DUPLICATE_BASE_CLASSES	entry    (type (NID_T),
					  type (NID_T))
				external (X_CHECK_DUPLICATE_BASE_CLASSES),

SEARCH_BASE_CLASS_LIST		entry    (type (NID_T),
					  type (NID_T))
				returns  (type (NID_T))
				external (X_SEARCH_BASE_CLASS_LIST),

SEARCH_VIRTUAL_BASE_CLASS_LIST	entry    (type (NID_T),
					  type (NID_T))
				returns  (type (LONG_T))
				external (X_SEARCH_VIRTUAL_BASE_CLASS_LIST),

SET_LOOKUP_SEQUENCE_RANGE	entry    (type (LONG_T),
					  type (LONG_T))
				external (X_SET_LOOKUP_SEQUENCE_RANGE),
CLEAR_LOOKUP_SEQUENCE_RANGE	entry    ()
				external (X_CLEAR_LOOKUP_SEQUENCE_RANGE),

EFFECTIVELY_NON_VISIBLE		entry    (type (NID_T))
				returns  (type (BOOL_T))
				external (X_EFFECTIVELY_NON_VISIBLE),
EFFECTIVELY_NON_VISIBLE_C	entry    ()
				returns  (type (BOOL_T))
				external (X_EFFECTIVELY_NON_VISIBLE_C),
EFFECTIVELY_NON_VISIBLE_P	entry    (type (POINTER_T))
				returns  (type (BOOL_T))
				external (X_EFFECTIVELY_NON_VISIBLE_P);
