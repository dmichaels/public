/* C++ hasht.c - Type-node hashing routine (in C for speeeeeeeeeeeed) */

/************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY               [ Update the VERSION__ string below ]
 *
 *  01.30.92  PKT  002	Bug fix.
 *  12.04.91  DGM  001  Updated with bug fix.
 *  08.29.90  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char VERSION__ [] = "@(#)LPI 12.04.91 001 HASHT";

/* ---------------------------------------------------------------------
/* Definitions from C_STANDARD_DEFS_IN
/* ------------------------------------------------------------------- */

typedef char			BYTE_T;
typedef short			NID_T;

/* ---------------------------------------------------------------------
/* Definitions from C_SYMBOL_TABLE_DEFS_IN
/* ------------------------------------------------------------------- */

#define ARRAY_DT		((BYTE_T)26)
#define FUNCTION_DT		((BYTE_T)27)

#define NULL_TQ			((BYTE_T)0x00)

/* ---------------------------------------------------------------------
/* Definitions from C_SYMBOL_TABLE_NODES_IN
/* ------------------------------------------------------------------- */

typedef struct {

	BYTE_T	     code;
	BYTE_T	     qualifiers;
	NID_T	     nid;

} TYPE_DATA_T;

/* ---------------------------------------------------------------------
/* Type definitions from C_TYPE_HASHING_PKG
/* ------------------------------------------------------------------- */

#define SIZE_TYPE_HASH_TABLE	1031

/* ---------------------------------------------------------------------
/* HASH_TYPE
/*
/* Returns <0 to indicate unhashable.
/* Otherwise returns a hash index between 1 and SIZE_TYPE_HASH_TABLE.
/* ------------------------------------------------------------------- */

long
HASHT (tdp, level)
register TYPE_DATA_T *tdp;
register int level;
{
	register long h, hash;

	for (h = 1 ; level-- >= 0 ; tdp++) {

		if ((tdp->code == FUNCTION_DT) ||
		    (tdp->code == ARRAY_DT))
			return (-1);

		h = h * ((tdp->code * 4) + (tdp->qualifiers));

		if (tdp->nid != 0)
			h = h * tdp->nid;

		if (h == 0)
			h = 1;
	}

	hash = h * 10464 % SIZE_TYPE_HASH_TABLE;

	if (hash <= 0)
		hash += SIZE_TYPE_HASH_TABLE;

	return (hash);
}

