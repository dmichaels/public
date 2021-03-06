/* CPP pp_write.c - Output routines */

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
 *  06.22.92  DGM  001  Call fwrite for speed.
 *  06.05.90  DGM  000  First CPP component version.
 *  --------------------------------------------------------------------
 *  09.28.88  DGM  Original.
 *
 ***********************************************************************/

/* --------------------------------------------------------------------
** Include files
** ----------------------------------------------------------------- */

#include <stdio.h>
#include "pp_sys.h"
#include "pp_str.h"
#include "pp_char.h"

/* --------------------------------------------------------------------
** Exported functions
** ----------------------------------------------------------------- */

void WRITTT ();
void WRITFW ();
void WRITNC ();
void WRITNO ();

/* --------------------------------------------------------------------
** Local definitions
** ----------------------------------------------------------------- */

#define BYTE_SIZE	1
#define SHORT_SIZE	2
#define LONG_SIZE	4

/* ---------------------------------------------------------------------
** convert
**
** Convert the given number to the given base (2 thru 16)
**
** In:	number	= Number (long) to convert to a string.
**	base	= The base (2 thru 16).
**	pad	= If zero then no padding is done.
**		  If positive, then this specifies the
**		  field width; ZERO pad if necessary.
**		  If negative, then the absolute value of this
**		  specifies the field width; BLANK pad if necessary.
**		  Right justified.
** Out:	string	= The resultant string (assumed to have enough space).
** ------------------------------------------------------------------- */

static
void
convert (number, base, string, pad)
unsigned long int number;
int		  base;
char		 *string;
int	 	  pad;
{
	static char	digits[] = "0123456789ABCDEF";
	char		buffer [1024];
	char		padchar;
	int		n;

	*string = NULL_CHAR;
	if ((base < 2) || (base > 16)) return;
	n = 0; do { buffer [n++] = digits [number % base];
	} while ((number /= base) != 0);
	if (pad != 0) {
		if (pad < 0) {
			padchar = ' ';
			pad = -pad;
		}
		else	padchar = '0';
		while (n < pad) buffer [n++] = padchar;
	}
	while (n > 0) *string++ = buffer [--n];
	*string = NULL_CHAR;
}

/* ---------------------------------------------------------------------
** WRITNO
**
** Write the C number to the standard ouput.
**
** In:  number	= Pointer to the number to write.
**	size	= Size in bytes of the number (1, 2, or 4 ONLY).
**	base	= BINARY, OCTAL, DECIMAL, or HEX.
**	pad	= If zero then no padding is done.
**		  If positive, then this specifies the
**		  field width; and ZERO pad if necessary.
**		  If negative, then the absolute value of this
**		  specifies the field width; BLANK pad if necessary.
**		  Right justified.
** Out:	Nothing
** ------------------------------------------------------------------- */

void
WRITNO (number, size, base, pad)
char		**number;
short int	*size;
short int	*base;
short int	*pad;
{
	char		  buffer [1024];
	unsigned long int n;
	register char	 *p;

	if (*size == BYTE_SIZE)
		n = (unsigned long int)(*((unsigned char *)(*number)));
	else if (*size == SHORT_SIZE)
		n = (unsigned long int)(*((unsigned short int *)(*number)));
	else if (*size == LONG_SIZE)
		n = (unsigned long int)(*((unsigned long int *)(*number)));
	convert (n, (int)*base, buffer, (int)*pad);
	for (p = buffer ; *p != NULL_CHAR ; p++)
		;
        /* SYS_WRITE (STDOUT, buffer, (int)(p - buffer)); */
	fwrite (buffer, 1, (int)(p - buffer), stdout);
}

/* ---------------------------------------------------------------------
** WRITTT
**
** Write the PL/I character varying string pointed to by the given
** argument, and newline to the standard ouput.
** ------------------------------------------------------------------- */

void
WRITTT (STR)
STRING *STR;
{
        /* SYS_WRITE (STDOUT, STR->text, STR->length); */
        /* SYS_WRITE (STDOUT, "\n", 1); */
	fwrite (STR->text, 1, STR->length, stdout);
	fwrite ("\n", 1, 1, stdout);
}

/* ---------------------------------------------------------------------
** WRITNC
**
** Write the PL/I character varying string pointed to by the given
** argument, without a newline to the standard ouput.
** ------------------------------------------------------------------- */

void
WRITNC (STR)
STRING *STR;
{
        /* SYS_WRITE (STDOUT, STR->text, STR->length); */
        fwrite (STR->text, 1, STR->length, stdout);
}

/* ---------------------------------------------------------------------
** WRITFW
**
** Write the PL/I character varying string pointed to by the given
** argument, right justified within the given field width.
** ------------------------------------------------------------------- */

void
WRITFW (STR, WIDTH)
STRING *STR;
short int *WIDTH;
{
	char		buffer [1024];
	register int	i, n;

        /* SYS_WRITE (STDOUT, STR->text, STR->length); */
        fwrite (STR->text, 1, STR->length, stdout);
	if ((n = (*WIDTH - STR->length)) > 0) {
		for (i = 0 ; i < n ; i++)
			buffer [i] = ' ';
		/* SYS_WRITE (STDOUT, buffer, n); */
		fwrite (buffer, 1, n, stdout);
	}
}
