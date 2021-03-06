/* CPP pp_op.c - Low level preprocessor expression evaluation operations */

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
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  11.08.88  DGM  000  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
** Version and copyright stamp
** ------------------------------------------------------------------- */

static char *VERSION__ =

"@(#)pp_op.c  000  6/5/90  (c) 1990 Language Processors, Inc.";

/* ---------------------------------------------------------------------
** Local type definitions
** ------------------------------------------------------------------- */

typedef unsigned long	ulong;

/* ---------------------------------------------------------------------
** Exported entry points
** ------------------------------------------------------------------- */

extern long	PPMUL	();	/* Multiply (signed) */
extern ulong	PPUMUL	();	/* Multiply (unsigned) */
extern long	PPDIV	();	/* Divide (signed) */
extern ulong	PPUDIV	();	/* Divide (unsigned) */
extern long	PPMOD	();	/* Modulus (signed) */
extern ulong	PPUMOD	();	/* Modulus (unsigned) */
extern long	PPADD	();	/* Add (signed) */
extern ulong	PPUADD	();	/* Add (unsigned) */
extern long	PPSUB	();	/* Subtract (signed) */
extern ulong	PPUSUB	();	/* Subtract (unsigned) */
extern long	PPLSH	();	/* Bitwise left shift (signed) */
extern ulong	PPULSH	();	/* Bitwise left shift (unsigned) */
extern long	PPRSH	();	/* Bitwise right shift (signed) */
extern ulong	PPURSH	();	/* Bitwise right shift (unsigned) */
extern long	PPOR	();	/* Bitwise inclusive OR (signed) */
extern ulong	PPUOR	();	/* Bitwise inclusive OR (unsigned) */
extern long	PPXOR	();	/* Bitwise exclusive OR (signed) */
extern ulong	PPUXOR	();	/* Bitwise exclusive OR (unsigned) */
extern long	PPAND	();	/* Bitwise AND (signed) */
extern ulong	PPUAND	();	/* Bitwise AND (unsigned) */
extern long	PPLT	();	/* Less-than (signed) */
extern ulong	PPULT	();	/* Less-than (unsigned) */
extern long	PPGT	();	/* Greater-than (signed) */
extern ulong	PPUGT	();	/* Greater-than (unsigned) */
extern long	PPLE	();	/* Less-than-or-equal (signed) */
extern ulong	PPULE	();	/* Less-than-or-equal (unsigned) */
extern long	PPGE	();	/* Greater-than-or-equal (signed) */
extern ulong	PPUGE	();	/* Greater-than-or-equal (unsigned) */
extern ulong	PPCOMP	();	/* Bitwise complement */

/* ---------------------------------------------------------------------
** PPMUL - Multiply (signed)
** PPUMUL - Multiply (unsigned)
** ------------------------------------------------------------------- */

long  PPMUL  (x, y) long  *x, *y; { return ((*x) * (*y)); }
ulong PPUMUL (x, y) ulong *x, *y; { return ((*x) * (*y)); }

/* ---------------------------------------------------------------------
** PPDIV - Divide (signed)
** PPUDIV - Divide (unsigned)
** ------------------------------------------------------------------- */

long  PPDIV  (x, y) long  *x, *y; { return ((*x) / (*y)); }
ulong PPUDIV (x, y) ulong *x, *y; { return ((*x) / (*y)); }

/* ---------------------------------------------------------------------
** PPMOD - Modulus (signed)
** PPUMOD - Modulus (unsigned)
** ------------------------------------------------------------------- */

long  PPMOD  (x, y) long  *x, *y; { return ((*x) % (*y)); }
ulong PPUMOD (x, y) ulong *x, *y; { return ((*x) % (*y)); }

/* ---------------------------------------------------------------------
** PPADD - Add (signed)
** PPUADD - Add (unsigned)
** ------------------------------------------------------------------- */

long  PPADD  (x, y) long  *x, *y; { return ((*x) + (*y)); }
ulong PPUADD (x, y) ulong *x, *y; { return ((*x) + (*y)); }

/* ---------------------------------------------------------------------
** PPSUB - Subtract (signed)
** PPUSUB - Subtract (unsigned)
** ------------------------------------------------------------------- */

long  PPSUB  (x, y) long  *x, *y; { return ((*x) - (*y)); }
ulong PPUSUB (x, y) ulong *x, *y; { return ((*x) - (*y)); }

/* ---------------------------------------------------------------------
** PPLSH - Bitwise left shift (signed)
** PPULSH - Bitwise left shift (unsigned)
** ------------------------------------------------------------------- */

long  PPLSH  (x, y) long  *x, *y; { return ((*x) << (*y)); }
ulong PPULSH (x, y) ulong *x, *y; { return ((*x) << (*y)); }

/* ---------------------------------------------------------------------
** PPRSH - Bitwise right shift (signed)
** PPURSH - Bitwise right shift (unsigned)
** ------------------------------------------------------------------- */

long  PPRSH  (x, y) long  *x, *y; { return ((*x) >> (*y)); }
ulong PPURSH (x, y) ulong *x, *y; { return ((*x) >> (*y)); }

/* ---------------------------------------------------------------------
** PPOR - Bitwise inclusive OR (signed)
** PPUOR - Bitwise inclusive OR (unsigned)
** ------------------------------------------------------------------- */

long  PPOR  (x, y) long  *x, *y; { return ((*x) | (*y)); }
ulong PPUOR (x, y) ulong *x, *y; { return ((*x) | (*y)); }

/* ---------------------------------------------------------------------
** PPXOR - Bitwise exclusive OR (signed)
** PPUXOR - Bitwise exclusive OR (unsigned)
** ------------------------------------------------------------------- */

long  PPXOR  (x, y) long  *x, *y; { return ((*x) ^ (*y)); }
ulong PPUXOR (x, y) ulong *x, *y; { return ((*x) ^ (*y)); }

/* ---------------------------------------------------------------------
** PPAND - Bitwise AND (signed)
** PPUAND - Bitwise AND (unsigned)
** ------------------------------------------------------------------- */

long  PPAND  (x, y) long  *x, *y; { return ((*x) & (*y)); }
ulong PPUAND (x, y) ulong *x, *y; { return ((*x) & (*y)); }

/* ---------------------------------------------------------------------
** PPLT - Less-than (signed)
** PPULT - Less-than (unsigned)
** ------------------------------------------------------------------- */

long  PPLT  (x, y) long  *x, *y; { return ((*x) < (*y)); }
ulong PPULT (x, y) ulong *x, *y; { return ((*x) < (*y)); }

/* ---------------------------------------------------------------------
** PPGT - Greater-than (signed)
** PPUGT - Greater-than (unsigned)
** ------------------------------------------------------------------- */

long  PPGT  (x, y) long  *x, *y; { return ((*x) > (*y)); }
ulong PPUGT (x, y) ulong *x, *y; { return ((*x) > (*y)); }

/* ---------------------------------------------------------------------
** PPLE - Less-than-or-equal (signed)
** PPULE - Less-than-or-equal (unsigned)
** ------------------------------------------------------------------- */

long  PPLE  (x, y) long  *x, *y; { return ((*x) <= (*y)); }
ulong PPULE (x, y) ulong *x, *y; { return ((*x) <= (*y)); }

/* ---------------------------------------------------------------------
** PPGE - Greater-than-or-equal (signed)
** PPUGE - Greater-than-or-equal (unsigned)
** ------------------------------------------------------------------- */

long  PPGE  (x, y) long  *x, *y; { return ((*x) >= (*y)); }
ulong PPUGE (x, y) ulong *x, *y; { return ((*x) >= (*y)); }

/* ---------------------------------------------------------------------
** PPCOMP - Bitwise complement
** ------------------------------------------------------------------- */

ulong PPCOMP (x) ulong *x; { return (~(*x)); }

