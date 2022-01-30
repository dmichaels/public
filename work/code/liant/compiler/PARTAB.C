/* C++ partab.c - Get parse table entry */

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
 *  08.30.89  DGM  000  Cloned & modified for C++ from ANSI-C.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* PARTAB/PARTABN
/*
/* These routines are used to directly extract data from the parse
/* table produced by the LPI parse tool; here is a brief description.
/*
/* A parse table simply consists of an ordered array of entries.  Each
/* parse table entry is one (or possiblly two) 16-bit word divided into
/* three fields (of 4, 2, and 10 bits each) followed by possibly one
/* additional 16-bit word (for "long" entries); bits[3:0] are the OPCODE
/* field and bits[5:4] are the ACTION field.  If the OPCODE field is not
/* OPC_LONG, then bits[15:6] are the OPERAND field, otherwise if the
/* OPCODE is OPC_LONG, then bits[15:6] are the real OPCODE field, and
/* the next 16-bit word is the OPERAND field.
/*
/* Here is the layout for a "normal" one word entry:
/*
/*      15                            6 5    4 3          0
/*      +------------------------------+------+------------+
/*      |           operand            |action|   opcode   |
/*      +------------------------------+------+------------+
/*
/* Here is the layout for a "long" two word entry:
/*
/*      15                            6 5    4 3          0
/*      +------------------------------+------+------------+
/*      |           opcode             |action|  OPC_LONG  |
/*      +------------------------------+------+------------+
/*      |                    operand                       |
/*      +--------------------------------------------------+
/*
/* ------------------------------------------------------------------- */

extern unsigned	short	PARTBL [];	/* The parse table itself */

#define OPC_LONG	10		/* See tools/parse/parse.in */

#define ACTION_FIELD(e)		(((e) >> 4) & 0x0003)
#define OPERAND_FIELD(e)	((e) >> 6)
#define OPCODE_FIELD(e)		((e) & 0x000F)

/* ---------------------------------------------------------------------
/* PARTAB
/*
/* Given an index into the LPI parse table (*index), return the parse
/* table entry's OPERAND field in "*operand", its ACTION field in
/* "*action", and its OPCODE field in "*opcode" (handles long entries).
/* The index of the next parse table entry is returned in "*index". 
/* ------------------------------------------------------------------- */

void

PARTAB (index, opcode, operand, action)

short int *index;	/* in/out */
short int *opcode;	/* out */
short int *operand;	/* out */
short int *action;	/* out */

{
	register unsigned short	entry;

	entry = PARTBL [ (*index)++ ];

	*action = ACTION_FIELD (entry);

	if ((*opcode = OPCODE_FIELD (entry)) == OPC_LONG) {
		*opcode  = OPERAND_FIELD (entry);
		*operand = PARTBL [ (*index)++ ];
	}
	else	*operand = OPERAND_FIELD (entry);
}

/* ---------------------------------------------------------------------
/* PARTABN
/*
/* Given an index into the LPI parse table (*index), return the parse
/* table entry's OPERAND field in "*operand", its ACTION field in
/* "*action", and its OPCODE field in "*opcode" (handles long entries).
/* The index of the "*count"th entry after the entry pointed to by the
/* given index is returned in "*index" (if "*count" is less than or
/* equal to one, it is taken to be one).
/*
/* On other words, this is equivalent to PARTAB, except instead of
/* advancing the index by *one* entry, we advance it *count* entries.
/* ------------------------------------------------------------------- */

void

PARTABN (index, opcode, operand, action, count)

short int *index;	/* in/out */
short int *opcode;	/* out */
short int *operand;	/* out */
short int *action;	/* out */
short int *count;	/* in */

{
	register unsigned short	entry;
	register int		n;

	/* This part is exactly like PARTAB */

	entry = PARTBL [ (*index)++ ];

	*action = ACTION_FIELD (entry);

	if ((*opcode = OPCODE_FIELD (entry)) == OPC_LONG) {
		*opcode  = OPERAND_FIELD (entry);
		*operand = PARTBL [ (*index)++ ];
	}
	else	*operand = OPERAND_FIELD (entry);

	/* Now set (*index) to the (*count-1)'th entry after (*index) */

	for (n = *count ; n > 1 ;  n--) {
		if (OPCODE_FIELD (PARTBL [ (*index)++ ] ) == OPC_LONG)
			(*index)++;
	}
}

