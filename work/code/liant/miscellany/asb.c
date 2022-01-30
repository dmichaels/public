/* P$ASB -- Assign PL/I bit-string */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/***********************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  03.28.91  DGM  000  Original.
/*
/***********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
/* Local type definitions
/* ------------------------------------------------------------------- */

typedef struct {
	union {
		unsigned char  *byte_pointer;
		unsigned short *short_pointer;
		unsigned long  *long_pointer;
	} pointer;
	unsigned char bit_offset;
} BitString;

/* ---------------------------------------------------------------------
/* Local macro definitions
/* ------------------------------------------------------------------- */

#define BYTE_SIZE		 8
#define SHORT_WORD_SIZE		16
#define LONG_WORD_SIZE		32

#define NULL_BYTE		'\0'

/* ---------------------------------------------------------------------
/* Local macro function definitions
/* ------------------------------------------------------------------- */

#define BitOffset(bp)		(((unsigned long)((bp)->bit_offset)) >> 5)
#define BytePointer(bp)		((bp)->pointer.byte_pointer)
#define ShortWordPointer(bp)	((bp)->pointer.short_pointer)
#define LongWordPointer(bp)	((bp)->pointer.long_pointer)
#define Byte(bp)		(*BytePointer(bp))
#define ShortWord(bp)		(*ShortWordPointer(bp))
#define LongWord(bp)		(*LongWordPointer(bp))
#define ByteArray(bp,i)		(BytePointer(bp)[i])

#define SizeInBytes(nbits)	((nbits) / BYTE_SIZE)
#define LeftOverBits(nbits)	((nbits) % BYTE_SIZE)

#define GetLoBits(bp,bo)	(Byte (bp) & MaskLoBits [bo])
#define GetHiBits(bp,bo)	(Byte (bp) & MaskHiBits [bo])
#define ClearLoBits(bp,bo)	(Byte (bp) &= MaskHiBits [bo])
#define ClearHiBits(bp,bo)	(Byte (bp) &= MaskLoBits [bo])
#define CopyLoBits(dbp,sbp,bo)	(ClearLoBits (dbp, bo), \
				 Byte (dbp) |= GetLoBits (sbp, bo))
#define CopyHiBits(dbp,sbp,bo)	(ClearHiBits (dbp, bo), \
				 Byte (dbp) |= GetHiBits (sbp, bo))

#define GetLoBitsInByteArray(bp,bn,bo) \
				(ByteArray (bp, bn) & MaskLoBits [bo])
#define GetHiBitsInByteArray(bp,bn,bo) \
				(ByteArray (bp, bn) & MaskHiBits [bo])
#define ClearLoBitsInByteArray(bp,bn,bo) \
				(ByteArray (bp, bn) &= MaskHiBits [bo])
#define ClearHiBitsInByteArray(bp,bn,bo) \
				(ByteArray (bp, bn) &= MaskLoBits [bo])
#define CopyLoBitsInByteArray(dbp,dbn,sbp,sbn,bo) \
				(ClearLoBitsInByteArray (dbp, dbn, bo), \
				 ByteArray (dbp, bo) |= \
				 GetLoBitsInByteArray (sbp, sbn, bo))
#define CopyHiBitsInByteArray(dbp,dbn,sbp,sbn,bo) \
				(ClearHiBitsInByteArray (dbp, dbn, bo), \
				 ByteArray (dbp, bo) |= \
				 GetHiBitsInByteArray (sbp, sbn, bo))

#define BitIsClearInByteArray(bp,bn,bo) \
				((ByteArray(bp,bn) & MaskBit [bo]) == 0)
#define BitIsSetInByteArray(bp,bn,bo) \
				((ByteArray(bp,bn) & MaskBit [bo]) != 0)
#define ClearBitInByteArray(bp,bn,bo) \
				(ByteArray(bp,bn) &= UnMaskBit [bo])
#define SetBitInByteArray(bp,bn,bo) \
				(ByteArray(bp,bn) |= MaskBit [bo])

/* ---------------------------------------------------------------------
/* Local static data
/* ------------------------------------------------------------------- */

#ifndef STORAGE_RIGHT_TO_LEFT		/* E.g. MC680x0 */

static unsigned char	MaskHiBits [BYTE_SIZE] =
{
	  0x00		/* 0: 00000000 */
	, 0x80		/* 1: 10000000 */
	, 0xC0		/* 2: 11000000 */
	, 0xE0		/* 3: 11100000 */
	, 0xF0		/* 4: 11110000 */
	, 0xF8		/* 5: 11111000 */
	, 0xFC		/* 6: 11111100 */
	, 0xFE		/* 7: 11111110 */
};
static unsigned char	MaskLoBits [BYTE_SIZE] =
{
	  0xFF		/* 0: 11111111 */
	, 0x7F		/* 1: 01111111 */
	, 0x3F		/* 2: 00111111 */
	, 0x1F		/* 3: 00011111 */
	, 0x0F		/* 4: 00001111 */
	, 0x07		/* 5: 00000111 */
	, 0x03		/* 6: 00000011 */
	, 0x01		/* 7: 00000001 */
};
static unsigned char	MaskBit [BYTE_SIZE] =
{
	  0x80		/* 0: 10000000 */
	, 0x40		/* 1: 01000000 */
	, 0x20		/* 2: 00100000 */
	, 0x10		/* 3: 00010000 */
	, 0x08		/* 4: 00001000 */
	, 0x04		/* 5: 00000100 */
	, 0x02		/* 6: 00000010 */
	, 0x01		/* 7: 00000001 */
};
static unsigned char	UnMaskBit [BYTE_SIZE] =
{
	  0x7F		/* 0: 01111111 */
	, 0xBF		/* 1: 10111111 */
	, 0xDF		/* 2: 11011111 */
	, 0xEF		/* 3: 11101111 */
	, 0xF7		/* 4: 11110111 */
	, 0xFB		/* 5: 11111011 */
	, 0xFD		/* 6: 11111101 */
	, 0xFE		/* 7: 11111110 */
};

#else /* defined (STORAGE_RIGHT_TO_LEFT) */	/* E.g. i80386 */

static unsigned char	MaskHiBits [BYTE_SIZE] =
{
	  0x00		/* 0: 00000000 */
	, 0x80		/* 1: 00000001 */
	, 0xC0		/* 2: 00000011 */
	, 0xE0		/* 3: 00000111 */
	, 0xF0		/* 4: 00001111 */
	, 0xF8		/* 5: 00011111 */
	, 0xFC		/* 6: 00111111 */
	, 0xFE		/* 7: 01111111 */
};
static unsigned char	MaskLoBits [BYTE_SIZE] =
{
	  0xFF		/* 0: 11111111 */
	, 0x7F		/* 1: 11111110 */
	, 0x3F		/* 2: 11111100 */
	, 0x1F		/* 3: 11111000 */
	, 0x0F		/* 4: 11110000 */
	, 0x07		/* 5: 11100000 */
	, 0x03		/* 6: 11000000 */
	, 0x01		/* 7: 10000000 */
};
static unsigned char	MaskBit [BYTE_SIZE] =
{
	  0x80		/* 0: 00000001 */
	, 0x40		/* 1: 00000010 */
	, 0x20		/* 2: 00000100 */
	, 0x10		/* 3: 00001000 */
	, 0x08		/* 4: 00010000 */
	, 0x04		/* 5: 00100000 */
	, 0x02		/* 6: 01000000 */
	, 0x01		/* 7: 10000000 */
};
static unsigned char	UnMaskBit [BYTE_SIZE] =
{
	  0x7F		/* 0: 11111110 */
	, 0xBF		/* 1: 11111101 */
	, 0xDF		/* 2: 11111011 */
	, 0xEF		/* 3: 11110111 */
	, 0xF7		/* 4: 11101111 */
	, 0xFB		/* 5: 11011111 */
	, 0xFD		/* 6: 10111111 */
	, 0xFE		/* 7: 01111111 */
};

#endif /* defined STORAGE_RIGHT_TO_LEFT */

/* ---------------------------------------------------------------------
/* P$ASB
/* ------------------------------------------------------------------- */

void
P$ASB (dst, dst_size, src, src_size)

register BitString	*dst;
register short		*dst_size;
register BitString	*src;
register short		*src_size;

{
	register int dst_offset, src_offset, i, n, m, dm, sm, nzerobits;

	/* Do the bit-string assigment; simplest/most-common cases first */

	dst_offset = BitOffset (dst);
	src_offset = BitOffset (src);

	if (*dst_size == *src_size) {
		/*
		/* Same-size.
		/**/
		if (dst_offset == src_offset) {
			/*
			/* Same-size and same-alignment.
			/**/
			if (dst_offset == 0) {
				/*
				/* Same-size and aligned.
				/* Look for easy cases first.
				/**/
				if (*dst_size == SHORT_WORD_SIZE) {
					ShortWord (dst) = ShortWord (src);
					return;
				}
				else if (*dst_size == LONG_WORD_SIZE) {
					LongWord (dst) = LongWord (src);
					return;
				}
				else if (*dst_size == BYTE_SIZE) {
					Byte (dst) = Byte (src);
					return;
				}
				else {
					n = *dst_size;
					m = 0;
				}
			}
			else {
				CopyLoBits (dst, src, dst_offset);
				n = *dst_size - (BYTE_SIZE - dst_offset);
				m = 1;
			}
			for (i = m, m += SizeInBytes (n) ; i < m ; i++)
				ByteArray (dst, i) = ByteArray (src, i);
			if ((i = LeftOverBits (n)) > 0)
				CopyHiBitsInByteArray (dst, m, src, m, i);
			return;
		}
		/*
		/* Same-size and non-aligned (and different-alignment).
		/* Fall thru to last case.
		/**/
	}
	/*
	/* Different-size.
	/**/
	else if (dst_offset == src_offset) {
		/*
		/* Different-size and aligned or
		/* non-aligned (but same-alignment).
		/**/
		if (*dst_size < *src_size)
			n = *dst_size, nzerobits = 0;
		else	n = *src_size, nzerobits = *dst_size - *src_size;
		if (dst_offset != 0) {
			/*
			/* Different-size and non-aligned (but same-alignment).
			/**/
			CopyLoBits (dst, src, dst_offset);
			n -= (BYTE_SIZE - dst_offset);
			m = 1;
		}
		else	m = 0;
		for (i = m, m += SizeInBytes (n) ; i < m ; i++)
			ByteArray (dst, i) = ByteArray (src, i);
		if ((i = LeftOverBits (n)) > 0)
			CopyHiBitsInByteArray (dst, m, src, m, i);
		if (nzerobits > 0) {
			if (i > 0) {
				ClearLoBitsInByteArray (dst, m, i);
				nzerobits -= (BYTE_SIZE - i);
				m++;
			}
			for (i = m, m += SizeInBytes (nzerobits) ; i < m ; i++)
				ByteArray (dst, i) = NULL_BYTE;
			if ((i = LeftOverBits (nzerobits)) > 0)
				ClearHiBitsInByteArray (dst, m, i);
		}
		return;
	}
	/*
	/* Different-or-same-size and non-aligned (and different-alignment).
	/**/
	if (*dst_size <= *src_size)
		n = *dst_size, nzerobits = 0;
	else	n = *src_size, nzerobits = *dst_size - *src_size;
	for (dm = sm = 0, i = n ; i > 0 ; i--) {
		if (BitIsClearInByteArray (src, sm, src_offset))
			ClearBitInByteArray (dst, dm, dst_offset);
		else	SetBitInByteArray (dst, dm, dst_offset);
		if (++dst_offset == BYTE_SIZE)
			dm++, dst_offset = 0;
		if (++src_offset == BYTE_SIZE)
			sm++, src_offset = 0;
	}
	if (nzerobits > 0) {
		if (dst_offset > 0) {
			ClearLoBitsInByteArray (dst, dm, dst_offset);
			nzerobits -= (BYTE_SIZE - dst_offset);
			m++;
		}
		for (i = m, m += SizeInBytes (nzerobits) ; i < m ; i++)
			ByteArray (dst, i) = NULL_BYTE;
		if ((i = LeftOverBits (nzerobits)) > 0)
			ClearHiBitsInByteArray (dst, m, i);
	}
	return;
}

