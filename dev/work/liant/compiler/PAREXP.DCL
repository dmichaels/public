/* C++ - CXX_PROCESS_EXPRESSION_PKG - Semantic expression package */

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
 *  03.26.92  DGM  010  New value-node scheme.
 *  06.28.91  PKT  009  Removed STRING_VALUE.
 *  03.26.91  PKT  008  Added AFFIX_DESTRUCTORS.
 *  09.14.90  DGM  007  Changed include file names and VOP names.
 *  08.27.90  TLF  006  Added SET_SSADD_VO and DEREFERENCE_ARROW.
 *  05.17.90  PKT  005  Added SET_FUNCTIONAL_CAST_OP.
 *  03.14.90  PKT  004  Interfaces for SET_SIZEOF_OP and TYPE_INTO_VALUE.
 *  02.20.90  PKT  003  Got SET_CONSTANT_EXPRESSION from pardcl.
 *			Added PUSH_VID and POP_VID.
 *  02.08.90  PKT  002  Moved statement-oriented actions to
 *			CXX_PROCESS_STATEMENT_PKG.
 *  02.06.90  PKT  001  Added interface declarations for semantic actions.
 *  10.18.89  DGM  000  Original.
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

%include CXX_PROCESS_EXPRESSION_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

SET_BINARY_OP			entry
				external (X_SET_BINARY_OP),

SET_UNARY_OP			entry
				external (X_SET_UNARY_OP),

POP_FOR_PRECEDENCE		entry
				external (X_POP_FOR_PRECEDENCE),

PUSH_VALUE_WITH_OP		entry
				external (X_PUSH_VALUE_WITH_OP),

BINARY_OPERATION		entry
				external (X_BINARY_OPERATION),

POP_ALL_VALUES			entry
				external (X_POP_ALL_VALUES),

PUSH_PREFIX			entry
				external (X_PUSH_PREFIX),

POP_PREFIX			entry
				external (X_POP_PREFIX),

SET_THIS_NAME			entry
				external (X_SET_THIS_NAME),
 
REF_VALUE			entry
				external (X_REF_VALUE),

DOT_VALUE			entry
				external (X_DOT_VALUE),

DEREFERENCE_VALUE		entry
				external (X_DEREFERENCE_VALUE),

DEREFERENCE_ARROW		entry
				external (X_DEREFERENCE_ARROW),

POSTINCREMENT_VALUE		entry
				external (X_POSTINCREMENT_VALUE),

POSTDECREMENT_VALUE		entry
				external (X_POSTDECREMENT_VALUE),

CONSTANT_VALUE  		entry
				external (X_CONSTANT_VALUE),

SET_CAST_OP			entry
				external (X_SET_CAST_OP),

SET_FUNCTIONAL_CAST_OP		entry
				external (X_SET_FUNCTIONAL_CAST_OP),

SET_ADD_OP			entry
				external (X_SET_ADD_OP),

SET_SSADD_OP			entry
				external (X_SET_SSADD_OP),

SET_MEMBER_INIT_OP		entry
				external (X_SET_MEMBER_INIT_OP),

SET_CALL_OP			entry
				external (X_SET_CALL_OP),

SET_COMMA_OP			entry
				external (X_SET_COMMA_OP),

SET_ARGUMENT_OP			entry
				external (X_SET_ARGUMENT_OP),

SET_SIZEOF_OP			entry
				external (X_SET_SIZEOF_OP),

NULL_VALUE			entry
				external (X_NULL_VALUE),

POP_ARGUMENT_VALUES		entry
				external (X_POP_ARGUMENT_VALUES),

TYPE_VALUE			entry
				external (X_TYPE_VALUE),

TYPE_INTO_VALUE			entry    (type (NID_T))
				external (X_TYPE_INTO_VALUE),

PUSH_NEW			entry
				external (X_PUSH_NEW),

ENTER_PLACEMENT			entry
				external (X_ENTER_PLACEMENT),

ENTER_TYPE_FOR_NEW		entry
				external (X_ENTER_TYPE_FOR_NEW),

POP_NEW				entry
				external (X_POP_NEW),

PUSH_DELETE			entry
				external (X_PUSH_DELETE),

ENTER_DELETE_DIMENSION		entry
				external (X_ENTER_DELETE_DIMENSION),

POP_DELETE			entry
				external (X_POP_DELETE),
	
SET_CONSTANT_EXPRESSION		entry
				external (X_SET_CONSTANT_EXPRESSION),

AFFIX_DESTRUCTORS		entry
				external (X_AFFIX_DESTRUCTORS),

PUSH_MARKER			entry    ()
				external (X_PUSH_MARKER),

POP_MARKER			entry    ()
				external (X_POP_MARKER);

