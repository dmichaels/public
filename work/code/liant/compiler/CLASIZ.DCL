/* C++ CXX_CLASS_SIZE_PKG - Class sizing package interface */

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
 *  09.14.90  DGM  006  Changed include file names and VOP names.
 *  07.23.90  DGM  005  Moved GET/DELETE_VIRTUAL_BASE_CLASS_LIST
 *			to lookup.dcl.
 *  07.19.90  DGM  004  Added INITIALIZE/FINISH_CLASS_SIZE and
 *			removed ALLOCATE_[VIRTUAL_]BASE_CLASSES.
 *  05.25.90  DGM  003  Changed interface to UPDATE_CLASS_SIZE and
 *			GET_VIRTUAL_BASE_CLASSES (take pointers).
 *  05.15.90  DGM  002  Added:	ALLOCATE_BASE_CLASSES
 *				ALLOCATE_VIRTUAL_BASE_CLASSES
 *				GET_VIRTUAL_BASE_CLASS_LIST
 *				DELETE_VIRTUAL_BASE_CLASS_LIST
 *  04.27.90  DGM  000  Original.
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

UPDATE_CLASS_SIZE		entry    (type (POINTER_T),
					  type (POINTER_T))
				external (X_UPDATE_CLASS_SIZE),

INITIALIZE_CLASS_SIZE		entry    (type (NID_T))
				external (X_INITIALIZE_CLASS_SIZE),

FINISH_CLASS_SIZE		entry    (type (NID_T))
				external (X_FINISH_CLASS_SIZE);

