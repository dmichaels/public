/* LPI-DEBUG M68 dodis.c - Print disassembled MC68xxx instructions */

/**********************************************************************/
/* This product is the property of Language Processors, Inc.          */
/* and is licensed pursuant to a written license agreement.           */
/* Now portion of this product may be reproduced without the written  */
/* permission of Language Processors, Inc. except pursuant to the     */
/* license agreement.                                                 */
/**********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *   1.13.92  VTF  (006) Removed the hexadecimal display of the bytes making
 *                       up the disassembled instruction (to save space).
 *
 *  12.04.91  VTF  (005) DODIS() now prints the disassembled address
 *                       according to the current address print mode.
 *                       Changed the call to DIS() to include the new
 *                       address parameter (see ~src/debug/div/m68/dis.003.pl1)
 *
 *  11.20.91  VTF  (004) DODIS() now changes the "from" parameter to point to
 *                       the address following the last address disassembled.
 *                       This is to support DISASSEMBLE /NEXT.
 * 
 *  11.11.91  VTF  Added code to print out a proper error message when
 *                 REAUSR/REAUST fails to read memory correctly.
 *
 *  01.05.88  DGM  Replaced calls to printf() with calls
 *		   to prnttt(), prntnc(), and prntno().
 *
 *  12.15.87  DGM  Original.
 *
 **********************************************************************/

#include <stdio.h>
#include "debug.h"

/* --------------------------------------------------------------------
** Imported external data
** ----------------------------------------------------------------- */

extern short	INTERRUPT;
extern char *sys_errlist[];
extern int errno, sys_nerr;

/* --------------------------------------------------------------------
** Local definitions
** ----------------------------------------------------------------- */

#define	MAX_INSTRUCTION_WORDS	12
#define	MAX_INSTRUCTION_BYTES	(MAX_INSTRUCTION_WORDS * 2)
#define SPCLEN 40
static char spaces[SPCLEN+1] = "                                        ";

/* --------------------------------------------------------------------
 * DODIS
 *
 * If "ninstructions" is greater than zero, then disassemble and print
 * to the standard ouput, "ninstructions" instructions starting at
 * machine address "from" of the user process identified by "pid".
 *
 * If "ninstructions" is less than or equal to zero and "to" is greater
 * than or equal to "from", then disassemble and print to the standard
 * output, all of the instructions starting at address "from" to the
 * address "to" in the user process identified by "pid".
 *
 * Return non-zero for success, zero for failure.
 */

int
DODIS (pid, from, to, ninstructions)

long	*pid;		/* User process ID */
long	*from;		/* Address in user process from which to read */
long	*to;		/* Address in user process to read to */
short	*ninstructions;	/* Number of disassembled instructions to print */

{
	cv_define(256)	string;
	unsigned short	buffer[MAX_INSTRUCTION_WORDS];
	unsigned short	*bufferptr;
	long		address, toaddress, null;
        int             fieldwidth;
	short		n, ni, nbytes, nwords, max, ret;

	null = NULL;
	bufferptr = buffer;
	max = MAX_INSTRUCTION_BYTES;
	ni = *ninstructions;

	if (ni > 0)
		toaddress = *from;
	else	toaddress = *to;

	for (address = *from ;
	     (INTERRUPT == 0) && ((ni > 0) || (address <= toaddress)) ;
	     ni--, address += nbytes) {

		REAUST (pid, &null, &address, &max, &bufferptr, &ret);

		ADDR2S (&address, NULL, &string);
                string.text[string.length++] = ':';
                string.text[string.length] = '\0';
		fieldwidth = string.length = pad(string.text,20,10);
		prntnc("  ");
                WRITNC(&string);

		if (ret != 0) goto ErrorDone;

		if ((nbytes = DIS (buffer, &address, &string)) <= 0) {
		        prnttt("???");
			nbytes = 1;
	        }
		else
		        WRITTT (&string);
	}

	Done:

        *from = address;
	return (1);

	ErrorDone:

        *from = address;
        if (errno == EIO)
            prnttt(" text address not found.");
        else if (errno < sys_nerr)
	    prnttt(sys_errlist[errno]);
        else
            prnttt(" error reading data.");

	return (0);
}


/* pad(s,min,inc) 
   Pad a string with spaces to a minimum field width.  If the string
   length exceeds the minimum, increase the field width by a specified
   value until it fits.

   pad("12345",10,5) results in "12345     " (length 10)
   pad("123456789012",10,5) results in "123456789012   " (length 15)
   pad("1234567890123456",10,5") results in "1234567890123456    " (length 20)

   In:   s -  the string to pad
       min -  minimum field width
       inc -  the amount to increase the field width if s does not fit in the
              minimum.  The field width is increased by this amount until s
              fits

  Out:   s -  the string padded with spaces.  It should be large enough
              to be padded.

  Return value: the new length of s
*/
pad(s,min,inc)
     char *s;
     int min;
     int inc;
{
  int oldlen=strlen(s), newlen;

  if (oldlen > min)
    newlen = min + (inc * ((oldlen-min) / inc + ((oldlen-min) % inc > 0)));
  else
    newlen = min;

  if (newlen - oldlen <= SPCLEN) {
    strcat(s,&spaces[SPCLEN-(newlen-oldlen)]);
    return newlen;
  }
  else {
    strcat(s,spaces);
    return oldlen+SPCLEN;
  }
}
