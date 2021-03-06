/* C++ CXX_PROCESS_STATEMENT_PKG - Interface to semantic statement package */

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
 *  05.08.92  TLF  012  Added EMIT_LABEL_VALUE.
 *  03.26.92  DGM  011  New value-node scheme.
 *  12.23.91  PKT  010  Removed CHECK_NOT_LOOP_BODY.
 *  07.18.91  DGM  009  Added SET_FOR_LOOP_INITIALIZER
 *  11.29.90  DGM  008  Added BRANCH_AROUND_NESTED_FUNCTION & EMIT_LABEL.
 *  10.16.90  PKT  007  Updated how EMIT_FUNCTION_BEGIN is called.
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  08.07.90  PKT  005  Several changes for better destruction support.
 *  07.20.90  PKT  004  Added CHECK_NOT_LOOP_BODY.
 *  07.13.90  DGM  003  Added EMIT_FUNCTION_BEGIN/END.
 *  02.20.90  PKT  002  Added the basic routines.
 *  02.08.90  PKT  001  Got EMIT_EXPRESSION, EMIT, STATEMENT_START,
 *			and EMIT_STMNT_ID from CXX_PROCESS_EXPRESSION_PKG.
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

%include CXX_PROCESS_STATEMENT_IN;

/* ---------------------------------------------------------------------
/* External entry point declarations
/* ------------------------------------------------------------------- */

declare

STATEMENT_START			entry    ()
				external (X_STATEMENT_START),

EMIT_STATEMENT_ID		entry    ()
				external (X_EMIT_STATEMENT_ID),

SET_LABEL_DEFINITION		entry
				external (X_SET_LABEL_DEFINITION),

EMIT_GOTO			entry    ()
				external (X_EMIT_GOTO),

EMIT_RETURN			entry    ()
				external (X_EMIT_RETURN),

EMIT_IMPLICIT_RETURN		entry    ()
				external (X_EMIT_IMPLICIT_RETURN),

DECLARE_LABEL_VALUE		entry    (type (SHORT_T))
				returns  (type (CXX_VID_T))
				external (X_DECLARE_LABEL_VALUE),

START_IF			entry    ()
				external (X_START_IF),

IF_HEADER			entry    ()
				external (X_IF_HEADER),

ELSE_HEADER			entry    ()
				external (X_ELSE_HEADER),

IF_TRAILER			entry    ()
				external (X_IF_TRAILER),

START_FOR_LOOP			entry    ()
				external (X_START_FOR_LOOP),

SET_FOR_LOOP_INITIALIZER	entry    ()
				external (X_SET_FOR_LOOP_INITIALIZER),

TEST_FOR_LOOP			entry    ()
				external (X_TEST_FOR_LOOP),

FINISH_FOR_LOOP			entry    ()
				external (X_FINISH_FOR_LOOP),

START_WHILE_LOOP		entry    ()
				external (X_START_WHILE_LOOP),

TEST_WHILE_LOOP			entry    ()
				external (X_TEST_WHILE_LOOP),

FINISH_WHILE_LOOP		entry    ()
				external (X_FINISH_WHILE_LOOP),

START_DO_LOOP			entry    ()
				external (X_START_DO_LOOP),

TEST_DO_LOOP			entry    ()
				external (X_TEST_DO_LOOP),

FINISH_DO_LOOP			entry    ()
				external (X_FINISH_DO_LOOP),

START_SWITCH_LOOP		entry    ()
				external (X_START_SWITCH_LOOP),

TEST_SWITCH_LOOP		entry    ()
				external (X_TEST_SWITCH_LOOP),

FINISH_SWITCH_LOOP		entry    ()
				external (X_FINISH_SWITCH_LOOP),

DEFINE_SWITCH_CASE		entry    ()
				external (X_DEFINE_SWITCH_CASE),

DEFINE_SWITCH_DEFAULT		entry    ()
				external (X_DEFINE_SWITCH_DEFAULT),

EMIT_LOOP_CONTINUE_LABEL	entry    ()
				external (X_EMIT_LOOP_CONTINUE_LABEL),

SET_LOOP_EXPRESSION		entry    ()
				external (X_SET_LOOP_EXPRESSION),

EMIT_BRANCH_CONTINUE		entry    ()
				external (X_EMIT_BRANCH_CONTINUE),

EMIT_BRANCH_EXIT		entry    ()
				external (X_EMIT_BRANCH_EXIT),

EMIT_EXPRESSION			entry    ()
				external (X_EMIT_EXPRESSION),

EMIT_STATEMENT			entry    (type (CXX_VID_T))
				external (X_EMIT_STATEMENT),

EMIT_FUNCTION_BEGIN		entry    ()
				external (X_EMIT_FUNCTION_BEGIN),

EMIT_FUNCTION_END		entry    ()
				external (X_EMIT_FUNCTION_END),

BRANCH_AROUND_NESTED_FUNCTION	entry    ()
				returns  (type (CXX_VID_T))
				external (X_BRANCH_AROUND_NESTED_FUNCTION),

EMIT_LABEL			entry    (type (CXX_VID_T))
				external (X_EMIT_LABEL),

EMIT_LABEL_VALUE		entry    (type (CXX_VID_T))
				external (X_EMIT_LABEL_VALUE);


