/* LPI-C++ mbtowc.c -- Multi-byte to wide character conversions */

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
 *  05.29.92  DGM  003  Updated.
 *  05.19.92  DGM  002  Updated.
 *  12.05.91  DGM  001  Minor update.
 *  11.21.91  DGM  000  Original; to handle L'x' and L"x" converions
 *			in the LPI-C++ comiler; called from CONVERT_
 *			CHARACTER_CONSTANT and MAP_STRING_LITERAL in
 *			the cnvcon module.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
/* Configuration
/*
/* Use the target system mbtowc and mbstowcs routines if possible.
/* If we can, then compiled mbtowc.c with USE_SYSTEM_MBTOWC_ROUTINES
/* #defined, in addition, if the the particular routine mbstowcs does
/* not exist, then compile mbtowc.c with MBSTOWCS_DOES_NOT_EXIT #defined.
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdlib.h>

/* ---------------------------------------------------------------------
/* Local definitions
/* ------------------------------------------------------------------- */

#ifdef USE_SYSTEM_MBTOWC_ROUTINES
#define system_mbtowc(pwc,s,n)		mbtowc(pwc,s,n)
#define system_mbstowcs(pwcs,s,n)	mbstowcs(pwcs,s,n)
#else
#define system_mbtowc(pwc,s,n)		trivial_mbtowc(pwc,s,n)
#define system_mbstowcs(pwcs,s,n)	trivial_mbstowcs(pwcs,s,n)
#endif

/* ---------------------------------------------------------------------
/* Local type definitions
/* ------------------------------------------------------------------- */

typedef struct {	/* character varying string (PL/I style) */
	short	size;
	char	text [32767];
} cvstring;

/* ---------------------------------------------------------------------
/* trivial_mblen  (assuming sizeof (wchar_t) == sizeof (char))
/* ------------------------------------------------------------------- */

static
int
trivial_mblen (s, n)
const char	*s;
size_t		 n;
{
	int ret;

	if      (s == 0)	ret = 0;
	else if (*s == '\0')	ret = 0;
	else if (n < 1)		ret = -1;
	else			ret = 1;
	return (ret);
}

/* ---------------------------------------------------------------------
/* trival_mbtowc  (assuming sizeof (wchar_t) == sizeof (char))
/* ------------------------------------------------------------------- */

static
int
trivial_mbtowc (pwc, s, n)
wchar_t		*pwc;
const char	*s;
size_t		 n;
{
	int	 ret = trivial_mblen (s, n);

	if ((pwc != 0) && (s != 0))
		if ((ret > 0) || (*s == '\0'))
			*pwc = *s;
	return (ret);
}

/* ---------------------------------------------------------------------
/* trival_mbstowcs  (assuming sizeof (wchar_t) == sizeof (char))
/* ------------------------------------------------------------------- */

static
size_t
trivial_mbstowcs (pwcs, s, n)
wchar_t		*pwcs;
const char	*s;
size_t		 n;
{
	char	*pcs = (char *)pwcs;
	size_t	 len = strlen (s);
	long	 m   = n;

	while ((--m >= 0) && (*pcs++ = *s++))
		;
	return ((len > n) ? n : len);
}

/* ---------------------------------------------------------------------
/* mbstowcs
/*
/* To be used only if we are using the target system's mbtowc routine,
/* but the target system does not have a mbstowcs routine.
/* ------------------------------------------------------------------- */

#ifdef USE_SYSTEM_MBTOWC_ROUTINES
#ifdef SYSTEM_MBSTOWCS_DOES_NOT_EXIST

extern
size_t
mbstowcs (pwcs, s, n)
wchar_t		*pwcs;
const char	*s;
size_t		 n;
{
	int		 i;
	wchar_t		*pwc;

	for (pwc = pwcs ; n > 0 ; pwc++, n--) {
		if ((i = mbtowc (pwc, s, n)) == -1)
			return (-1);
		else if ((i  == 0) || (*pwc == 0))
			break;
		s += i;
	}
	return (pwc - pwcs);
}

#endif /* SYSTEM_MBSTOWCS_DOES_NOT_EXIST */
#endif /* USE_SYSTEM_MBTOWC_ROUTINES */

/* ---------------------------------------------------------------------
/* MBTOWC entry   (fixed binary (31),
/*		   character (*) varying,
/*		   fixed binary (15) value)
/*        returns (fixed binary (31));
/* ------------------------------------------------------------------- */

extern
int
MBTOWC (pwc, s, trivial)
long		*pwc;		/* out */
cvstring	*s;		/*  in */
short		 trivial;	/*  in */
{
	wchar_t	wc;
	size_t	n;

	if (trivial)
		n = trivial_mbtowc (&wc, s->text, s->size);
	else	n = system_mbtowc (&wc, s->text, s->size);
	*pwc = (long)wc;
	return (n);
}

/* ---------------------------------------------------------------------
/* MBSTOWCS entry (character (*) varying,
/*		   character (*) varying,
/*		   fixed binary (15) value)
/*        returns (fixed binary (31));
/* ------------------------------------------------------------------- */

extern
int
MBSTOWCS (pwcs, s, trivial)
cvstring	*pwcs;		/* out */
cvstring	*s;		/*  in */
short		 trivial;	/*  in */
{
	size_t	n;

	if (trivial)
		n = trivial_mbstowcs
		    ((wchar_t *)pwcs->text, s->text, s->size);
	else	n = system_mbstowcs
		    ((wchar_t *)pwcs->text, s->text, s->size);
	pwcs->size = n * sizeof (wchar_t) + sizeof (wchar_t);
	return (n);
}

