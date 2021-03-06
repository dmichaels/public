/* C++ CXX_DATA_INITIAL_FILE_PKG - Data-initial-file interface */

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
 *  10.06.92  DGM  007  Changed WRITE_RELOCATION_DATA.
 *  05.12.92  DGM  006  Added MAKE_NULL_DATA_INITIAL_NODE.
 *  04.30.92  DGM  005  Added OPEN_ALIGNED_DATA_INITIAL_FILE.
 *  06.25.91  PKT  004  Removed SET_STRING_RELOCATION_DATA.
 *			Added PENDING_DATA_INITIAL_NODE and
 *			LOCATE_DATA_INITIAL_NODE.
 *  03.12.91  DGM  003  Updated WRITE_RELOCATION_DATA declaration.
 *  10.30.90  TLF  002  Added GET_DATA_INITIAL_BYTE_OFFSET.
 *  09.14.90  DGM  001  Changed include file names and VOP names.
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

%include CXX_DATA_INITIAL_FILE_IN;

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

OPEN_DATA_INITIAL_FILE		entry    (type (DIFILE_T))
				external (X_OPEN_DATA_INITIAL_FILE),

OPEN_ALIGNED_DATA_INITIAL_FILE	entry    (type (DIFILE_T),
					  type (SHORT_T))
				external (X_OPEN_ALIGNED_DATA_INITIAL_FILE),

CLOSE_DATA_INITIAL_FILE		entry    (type (DIFILE_T))
				external (X_CLOSE_DATA_INITIAL_FILE),

WRITE_DATA_INITIAL_FILE		entry     (type (DIFILE_T),
					   pointer,
					   type (LONG_T))
				external  (X_WRITE_DATA_INITIAL_FILE),

ALLOC_DATA_INITIAL_FILE		entry    (type (DIFILE_T),
					  type (LONG_T),
					  type (SHORT_T))
				external (X_ALLOC_DATA_INITIAL_FILE),

APPEND_DATA_INITIAL_FILE	entry     (type (DIFILE_T),
					   pointer,
					   type (LONG_T))
				external  (X_APPEND_DATA_INITIAL_FILE),

ADVANCE_DATA_INITIAL_FILE	entry    (type (DIFILE_T),
					  type (LONG_T),
					  type (SHORT_T))
				external (X_ADVANCE_DATA_INITIAL_FILE),


ALIGN_DATA_INITIAL_FILE		entry    (type (DIFILE_T),
					  type (SHORT_T))
				external (X_ALIGN_DATA_INITIAL_FILE),

SET_DATA_INITIAL_FILE		entry    (type (DIFILE_T))
				external (X_SET_DATA_INITIAL_FILE),

SET_RELOCATION_DATA		entry    (type (NID_T),
					  type (LONG_T),
					  type (DIFILE_T))
				external (X_SET_RELOCATION_DATA),

WRITE_RELOCATION_DATA		entry    (type (NID_T),
					  type (NID_T))
				external (X_WRITE_RELOCATION_DATA),

MAKE_DATA_INITIAL_NODE		entry    (type (DIFILE_T))
				returns  (type (NID_T))
				external (X_MAKE_DATA_INITIAL_NODE),

MAKE_NULL_DATA_INITIAL_NODE	entry    ()
				returns  (type (NID_T))
				external (X_MAKE_NULL_DATA_INITIAL_NODE),

PENDING_DATA_INITIAL_NODE	entry
				returns  (type (NID_T))
				external (X_PENDING_DATA_INITIAL_NODE),

LOCATE_DATA_INITIAL_NODE	entry    (type (NID_T), type (DIFILE_T))
				external (X_LOCATE_DATA_INITIAL_NODE),

GET_DATA_INITIAL_BYTE_OFFSET    entry	 (type (DIFILE_T))
				returns  (type (LONG_T))
				external (X_GET_DATA_INITIAL_BYTE_OFFSET);

