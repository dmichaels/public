/* C++ CXX_VIRTUAL_FUNCTION_TABLE_PKG - Virtual function table interface */

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
 *  07.31.91  DGM  005  Added DELETE_VFT_QUEUE.
 *  07.24.91  DGM  004  Changed GENERATE_VFTS.
 *  03.12.91  DGM  003  Added FUDGE_VFTS.
 *  09.14.90  DGM  002  Changed include file names and VOP names.
 *  05.25.90  DGM  001  Updated.
 *  05.17.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Required nclude Files
/* ------------------------------------------------------------------- */

/* %include CXX_STANDARD_DEFS_IN;     */
/* %include CXX_EXTERNAL_NAME_MAP_IN; */

/* ---------------------------------------------------------------------
/* Entry point declarations
/* ------------------------------------------------------------------- */

declare

QUEUE_VFT		entry    (type (NID_T))
			external (X_QUEUE_VFT),

GENERATE_VFTS		entry    ()
			external (X_GENERATE_VFTS),

DELETE_VFT_QUEUE	entry    ()
			external (X_DELETE_VFT_QUEUE),

FUDGE_VFTS		entry    ()
			external (X_FUDGE_VFTS);

