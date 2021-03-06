/* C++ CXX_PROCESS_DECLARATION_PKG - Semantic actions for declarations */

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
 *  05.29.92  DGM  035	Added SET_xxx_MEMBER_ACCESS.
 *  05.19.92  DGM  034	Added SAVE_FUNCTION_BODY.
 *  05.06.92  DGM  033	Added SET_LINKAGE_SPECIFIER_GROUP.
 *  04.30.92  DGM  032	Added BEGIN/END_PARAM_TYPE_NAME.
 *  03.13.92  PKT  031	Made BUILD_SYMBOL external.
 *  01.21.92  DGM  030  Added WARN_EXTRA_ARGUMENT_COMMA.
 *  01.13.92  DGM  029  Added SET_DECLARATOR_SCOPE.
 *  12.16.91  DGM  028  Added SET_DEFAULT_STRUCT_ALIGNMENT.
 *  11.12.91  DGM  027  Added SET_ENUM_BASE_TYPE & changed
 *			BEGIN_EXTERN_TAG_DECLARATION to SET_TAG_EXTERN.
 *  05.29.91  PKT  026  Added CHECK_CLASS_SCOPE_QUALIFIER,
 *			SET_TYPE_SCOPE_QUALIFIER, CHECK_DECLARED_DESTRUCTOR.
 *  05.21.91  DGM  025	Added BEGIN_EXTERN_ENUM_DECLARATION.
 *  05.13.91  DGM  024  Removed CLEAR_SIMPLE_TYPE_SPECIFIERS.
 *  02.27.91  DGM  023	Added BEGIN_ASM_DECLARATION.
 *  01.18.91  DGM  022	Updated.
 *  01.11.91  DGM  021	Removed SET_MEMBER_BASE_TYPE.
 *  10.16.90  PKT  020	Moved base/member initialization
 *			to CXX_PROCESS_BASE_MBR_INIT_PKG.
 *  10.10.90  DGM  019  Moved BEGIN/END_CLASS_INITIALIZER
 *			to CXX_PROCESS_INITIALIZER_PKG.
 *  09.14.90  DGM  018  Changed include file names and VOP names.
 *  09.05.90  PKT  017  Added SET_INCOMPLETE_ARRAY_TYPE.
 *  07.13.90  DGM  015  Moved SET_LABEL_DECLARATION and EMIT_FUNCTION_
 *			BEGIN/END to parstm.pl1, and PROCESS_ANONYMOUS_
 *			UNION to instal.pl1.
 *  06.14.90  PKT  014  Added SET_PROTOTYPED & BEGIN_OLD_PARAMETER_LIST.
 *  05.15.90  DGM  013  Added BEGIN_TAG_NAME.
 *  04.23.90  DGM  012  Removed BEGIN_OLD_PARAMETER_LIST and changed
 *			SET_OLD_PARAMETER to SET_OLD_PARAMETER_DECLARATION.
 *  04.19.90  DGM  011  Moved [BEGIN|END]_INITIALIZER to PARINI.
 *  04.02.90  DGM  010  Moved DECLARE_IMPLICIT, DECLARE_IMPLICIT_FUNCTION,
 *			DECLARE_LABEL, and MAKE_SIMPLE_TYPE to dclmsc.pl1
 *			(C_DECLARE_MISCELLANY_PKG);
 *			also, changed MAKE_SIMPLE_TYPE to GET_SIMPLE_TYPE.
 *  03.27.90  DGM  009  Updated.
 *  03.02.90  DGM  008  Changed MAKE_LABEL to DECLARE_LABEL; do we want
 *			to pass the scope in which to declare the label?
 *  02.27.90  DGM  007  Updated.
 *  02.22.90  DGM  006  Updated.
 *  02.20.90  PKT  005  Added SET_LABEL_DECLARATION and MAKE_LABEL.
 *			Moved SET_CONSTANT_EXPRESSION to parexp.
 *  02.15.90  DGM  004  Updated.
 *  02.09.90  DGM  003  Updated.
 *  10.02.89  DGM  000  Original.
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

CLEAR_DECLARATION		entry
				external (X_CLEAR_DECLARATION),
CLEAR_DECLARATION_SPECIFIERS	entry
				external (X_CLEAR_DECLARATION_SPECIFIERS),
CLEAR_TYPE_SPECIFIERS		entry
				external (X_CLEAR_TYPE_SPECIFIERS),
CLEAR_SCOPE_QUALIFIER		entry
				external (X_CLEAR_SCOPE_QUALIFIER),

SET_STORAGE_CLASS_SPECIFIER	entry
				external (X_SET_STORAGE_CLASS_SPECIFIER),
SET_FRIEND_SPECIFIER		entry
				external (X_SET_FRIEND_SPECIFIER),
SET_VIRTUAL_SPECIFIER		entry
				external (X_SET_VIRTUAL_SPECIFIER),
SET_INLINE_SPECIFIER		entry
				external (X_SET_INLINE_SPECIFIER),
SET_OVERLOAD_SPECIFIER		entry
				external (X_SET_OVERLOAD_SPECIFIER),
SET_SIMPLE_TYPE			entry
				external (X_SET_SIMPLE_TYPE),

SET_TYPE_NAME			entry
				external (X_SET_TYPE_NAME),

SET_TYPE_QUALIFIER		entry
				external (X_SET_TYPE_QUALIFIER),

SET_BASE_TYPE			entry
				external (X_SET_BASE_TYPE),

BEGIN_CLASS_DECLARATION		entry
				external (X_BEGIN_CLASS_DECLARATION),
SET_TAG_EXTERN			entry
				external (X_SET_TAG_EXTERN),
BEGIN_TAG_NAME			entry
				external (X_BEGIN_TAG_NAME),
SET_TAG_NAME			entry
				external (X_SET_TAG_NAME),
BEGIN_CLASS_DERIVATION		entry
				external (X_BEGIN_CLASS_DERIVATION),
SET_BASE_CLASS			entry
				external (X_SET_BASE_CLASS),
SET_PUBLIC_BASE_CLASS		entry
				external (X_SET_PUBLIC_BASE_CLASS),
SET_PRIVATE_BASE_CLASS		entry
				external (X_SET_PRIVATE_BASE_CLASS),
SET_PROTECTED_BASE_CLASS	entry
				external (X_SET_PROTECTED_BASE_CLASS),
SET_VIRTUAL_BASE_CLASS		entry
				external (X_SET_VIRTUAL_BASE_CLASS),
SET_CLASS_DERIVATION		entry
				external (X_SET_CLASS_DERIVATION),
SET_STRUCT_ALIGNMENT		entry    (type (SHORT_T))
				external (X_SET_STRUCT_ALIGNMENT),
SET_DEFAULT_STRUCT_ALIGNMENT	entry    (type (SHORT_T))
				external (X_SET_DEFAULT_STRUCT_ALIGNMENT),
BEGIN_CLASS_BODY		entry
				external (X_BEGIN_CLASS_BODY),
SET_PUBLIC_MEMBER_ACCESS	entry
				external (X_SET_PUBLIC_MEMBER_ACCESS),
SET_PRIVATE_MEMBER_ACCESS	entry
				external (X_SET_PRIVATE_MEMBER_ACCESS),
SET_PROTECTED_MEMBER_ACCESS	entry
				external (X_SET_PROTECTED_MEMBER_ACCESS),
SET_BIT_FIELD			entry
				external (X_SET_BIT_FIELD),
BEGIN_BIT_FIELD			entry
				external (X_BEGIN_BIT_FIELD),
SET_PURE_SPECIFIER		entry
				external (X_SET_PURE_SPECIFIER),
SET_MEMBER_DECLARATION		entry
				external (X_SET_MEMBER_DECLARATION),
END_CLASS_BODY			entry
				external (X_END_CLASS_BODY),
SET_CLASS_DECLARATION		entry
				external (X_SET_CLASS_DECLARATION),

BEGIN_ENUM_DECLARATION		entry
				external (X_BEGIN_ENUM_DECLARATION),
BEGIN_ENUM_BODY			entry
				external (X_BEGIN_ENUM_BODY),
SET_ENUM_BASE_TYPE		entry
				external (X_SET_ENUM_BASE_TYPE),
SET_ENUMERATOR_DECLARATION	entry
				external (X_SET_ENUMERATOR_DECLARATION),
SET_ENUMERATOR_INITIALIZED	entry
				external (X_SET_ENUMERATOR_INITIALIZED),
SET_ENUMERATOR_VALUE		entry
				external (X_SET_ENUMERATOR_VALUE),
END_ENUM_BODY			entry
				external (X_END_ENUM_BODY),
SET_ENUM_DECLARATION		entry
				external (X_SET_ENUM_DECLARATION),

SET_DERIVED_TYPE_QUALIFIER	entry
				external (X_SET_DERIVED_TYPE_QUALIFIER),
SET_DERIVED_CLASS_QUALIFIER	entry
				external (X_SET_DERIVED_CLASS_QUALIFIER),
SET_REFERENCE_TYPE		entry
				external (X_SET_REFERENCE_TYPE),
SET_POINTER_TYPE		entry
				external (X_SET_POINTER_TYPE),
SET_MEMBER_POINTER_TYPE		entry
				external (X_SET_MEMBER_POINTER_TYPE),
SET_INCOMPLETE_ARRAY_TYPE	entry
				external (X_SET_INCOMPLETE_ARRAY_TYPE),
BEGIN_ARRAY_TYPE		entry
				external (X_BEGIN_ARRAY_TYPE),
SET_ARRAY_TYPE			entry
				external (X_SET_ARRAY_TYPE),
BEGIN_FUNCTION_TYPE		entry
				external (X_BEGIN_FUNCTION_TYPE),
BEGIN_OLD_FUNCTION_TYPE		entry
				external (X_BEGIN_OLD_FUNCTION_TYPE),
SET_FUNCTION_TYPE		entry
				external (X_SET_FUNCTION_TYPE),

SET_ELLIPSIS			entry
				external (X_SET_ELLIPSIS),
SET_PARAMETER_DECLARATION	entry
				external (X_SET_PARAMETER_DECLARATION),
SET_PARAMETER_INITIALIZER	entry
				external (X_SET_PARAMETER_INITIALIZER),

SET_OLD_PARAMETER_NAME		entry
				external (X_SET_OLD_PARAMETER_NAME),
SET_OLD_PARAMETER_DECLARATION	entry
				external (X_SET_OLD_PARAMETER_DECLARATION),
BEGIN_OLD_PARAMETER_LIST	entry
				external (X_BEGIN_OLD_PARAMETER_LIST),
SET_OLD_PARAMETER_LIST		entry
				external (X_SET_OLD_PARAMETER_LIST),

SET_SIMPLE_NAME			entry
				external (X_SET_SIMPLE_NAME),
SET_VANILLA_NAME		entry
				external (X_SET_VANILLA_NAME),
SET_DESTRUCTOR_NAME		entry
				external (X_SET_DESTRUCTOR_NAME),
CHECK_DECLARED_DESTRUCTOR	entry
				external (X_CHECK_DECLARED_DESTRUCTOR),
SET_OPERATOR_NAME		entry
				external (X_SET_OPERATOR_NAME),
BEGIN_CONVERSION_NAME		entry
				external (X_BEGIN_CONVERSION_NAME),
SET_CONVERSION_NAME		entry
				external (X_SET_CONVERSION_NAME),
SET_CLASS_SCOPE_QUALIFIER	entry
				external (X_SET_CLASS_SCOPE_QUALIFIER),
CHECK_CLASS_SCOPE_QUALIFIER	entry
				external (X_CHECK_CLASS_SCOPE_QUALIFIER),
SET_TYPE_SCOPE_QUALIFIER	entry
				external (X_SET_TYPE_SCOPE_QUALIFIER),
SET_GLOBAL_SCOPE_QUALIFIER	entry
				external (X_SET_GLOBAL_SCOPE_QUALIFIER),
SET_DECLARATOR_SCOPE		entry
				external (X_SET_DECLARATOR_SCOPE),

SET_DECLARATION			entry
				external (X_SET_DECLARATION),
SET_TYPE_DECLARATION		entry
				external (X_SET_TYPE_DECLARATION),

BEGIN_FUNCTION_BODY		entry
				external (X_BEGIN_FUNCTION_BODY),
END_FUNCTION_BODY		entry
				external (X_END_FUNCTION_BODY),

BEGIN_BLOCK_BODY		entry
				external (X_BEGIN_BLOCK_BODY),
END_BLOCK_BODY			entry
				external (X_END_BLOCK_BODY),

SAVE_MEMBER_FUNCTION_BODY	entry
				external (X_SAVE_MEMBER_FUNCTION_BODY),
SAVE_FUNCTION_BODY		entry
				external (X_SAVE_FUNCTION_BODY),

BEGIN_LINKAGE_SPECIFIER		entry
				external (X_BEGIN_LINKAGE_SPECIFIER),
SET_LINKAGE_SPECIFIER_GROUP	entry
				external (X_SET_LINKAGE_SPECIFIER_GROUP),
END_LINKAGE_SPECIFIER		entry
				external (X_END_LINKAGE_SPECIFIER),

BEGIN_ASM_DECLARATION		entry
				external (X_BEGIN_ASM_DECLARATION),
SET_ASM_DECLARATION		entry
				external (X_SET_ASM_DECLARATION),

WARN_EXTRA_TEXT			entry
				external (X_WARN_EXTRA_TEXT),
WARN_EXTRA_ELLIPSIS		entry
				external (X_WARN_EXTRA_ELLIPSIS),
WARN_EXTRA_ENUM_COMMA		entry
				external (X_WARN_EXTRA_ENUM_COMMA),
WARN_EXTRA_ARGUMENT_COMMA	entry
				external (X_WARN_EXTRA_ARGUMENT_COMMA),

RUMMAGE_QUALIFIED_TYPE_NAME	entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_RUMMAGE_QUALIFIED_TYPE_NAME),

RUMMAGE_QUALIFIED_TAG_NAME	entry    (type (NID_T))
				returns  (type (NID_T))
				external (X_RUMMAGE_QUALIFIED_TAG_NAME),

BUILD_SYMBOL			entry	 (type (LONG_BITS_T))
				returns	 (type (NID_T))
				external (X_BUILD_SYMBOL),

BEGIN_PAREN_TYPE_NAME		entry    ()
				external (X_BEGIN_PAREN_TYPE_NAME),
END_PAREN_TYPE_NAME		entry    ()
				external (X_END_PAREN_TYPE_NAME);

