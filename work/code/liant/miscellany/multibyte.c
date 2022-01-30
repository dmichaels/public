From uunet!sequoia.com!burn Mon Oct 14 16:34:38 1991
Date: Mon, 14 Oct 91 14:43:08 EDT
From: uunet!sequoia.com!burn (burn)
To: david@lpi.liant.com
Subject: Multibyte stuff

/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  mbtowc.c, Version 6.500
|  Latest Change on 4/4/91 at 16:22:19
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:stdio/mbtowc.c	1.6"
/*LINTLIBRARY*/

#include "synonyms.h"
#include <ctype.h>
#include <stdlib.h>
#include "_wchar.h"

int
mbtowc(wchar, s, n)
wchar_t *wchar;
const char *s;
size_t n;
{
	register int length;
	register wchar_t intcode;
	register c;
	char *olds = (char *)s;
	wchar_t mask;
	int lflag = 0;
	int shift;
	
	if(s == (char *)0)
		return 0;
	if(n == 0)
		return(-1);
	c = (unsigned char)*s++;
	if(!multibyte || c < 0200) {
		if(wchar)
			*wchar = c;
		return(c ? 1 : 0);
	}
	lflag = _ctype[520] > 3 || eucw1 > 2;
	intcode = 0;
	if (c == SS2) {
		if(!(length = eucw2)) 
			goto lab1;
		if(lflag)
			mask = P01;
		else 
			mask = H_P01;
		goto lab2;
	} else if(c == SS3) {
		if(!(length = eucw3)) 
			goto lab1;
		if(lflag)
			mask = P10;
		else
			mask = H_P10;
		goto lab2;
	} 
lab1:
	if(iscntrl(c)) {
		if(wchar)
			*wchar = c;
		return(1);
	}
	length = eucw1 - 1;
	if(lflag)
		mask = P11;
	else
		mask = H_P11;
	intcode = c & 0177;
lab2:
	if(length + 1 > n || length < 0)
		return(-1);
	shift = 8 - lflag;
	while(length--) {
		if((c = (unsigned char)*s++) < 0200 || iscntrl(c))
			return(-1);
		intcode = (intcode << shift) | (c & 0177);
	}
	if(wchar)
		*wchar = intcode | mask;
	return((char *)s - olds);
}	

#undef mblen

int
mblen(s, n)
const char *s;
size_t n;
{
	return(mbtowc((wchar_t *)0, s, n));
}



/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  wctomb.c, Version 6.500
|  Latest Change on 4/4/91 at 16:25:15
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:stdio/wctomb.c	1.6"
/*LINTLIBRARY*/

#include "synonyms.h"
#include <ctype.h>
#include <stdlib.h>
#include "_wchar.h"

int
wctomb(s, wchar)
char *s;
wchar_t wchar;
{
	char *olds = s;
	register int size, index;
	unsigned char d;
	int shift, lflag;
	if(!s)
		return(0);
	if(!multibyte || wchar <= 0177 || wchar <= 0377 && iscntrl(wchar)) {
		*s++ = (char)wchar;
		return(1);
	}
	lflag = _ctype[520] > 3 || eucw1 > 2;
	if(lflag)
		switch(wchar & EUCMASK) {
			
			case P11:
				size = eucw1;
				break;
			
			case P01:
				*s++ = (char)SS2;
				size = eucw2;
				break;
			
			case P10:
				*s++ = (char)SS3;
				size = eucw3;
				break;
			
			default:
				return(-1);
		}
	else
		switch(wchar & H_EUCMASK) {
			
			case H_P11:
				size = eucw1;
				break;
			
			case H_P01:
				*s++ = (char)SS2;
				size = eucw2;
				break;
			
			case H_P10:
				*s++ = (char)SS3;
				size = eucw3;
				break;
			
			default:
				return(-1);
		}
	if((index = size) <= 0)
		return -1;	
	shift = 8 - lflag;
	while(index--) {
		d = wchar | 0200;
		wchar >>= shift;
		if(iscntrl(d))
			return(-1);
		s[index] = d;
	}
	return(s + size - olds);
}


/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  %M%, Version %I%
|  Latest Change on %G% at %U%
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:stdio/_wchar.h	1.4"

#define SS2     0x8e
#define SS3     0x8f
#define EUCMASK 0x30000000
#define P11     0x30000000          /* Code Set 1 */
#define P01     0x10000000          /* Code Set 2 */
#define P10     0x20000000          /* Code Set 3 */
/* masks for characters < 3 bytes */
#define H_EUCMASK 0x8080
#define H_P11     0x8080          /* Code Set 1 */
#define H_P01     0x0080          /* Code Set 2 */
#define H_P10     0x8000          /* Code Set 3 */
#define multibyte       (_ctype[520]>1)
#define eucw1   _ctype[514]
#define eucw2   _ctype[515]
#define eucw3   _ctype[516]


/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  _ctype.c, Version 6.500
|  Latest Change on 4/4/91 at 16:19:32
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:gen/_ctype.c	1.3"
#ifdef __STDC__
	#pragma weak _ctype = __ctype
#endif
#include "synonyms.h"
#include <locale.h>
#include "_locale.h"
#include <ctype.h>

unsigned char _ctype[SZ_TOTAL] =
{
	0, /*EOF*/
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_S|_C,	_S|_C,	_S|_C,	_S|_C,	_S|_C,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_C,	_C,	_C,	_C,	_C,	_C,	_C,	_C,
	_S|_B,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_P,	_P,	_P,	_P,	_P,	_P,	_P,
	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,	_N|_X,
	_N|_X,	_N|_X,	_P,	_P,	_P,	_P,	_P,	_P,
	_P,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U|_X,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_U,	_U,	_U,	_U,	_U,
	_U,	_U,	_U,	_P,	_P,	_P,	_P,	_P,
	_P,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L|_X,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_L,	_L,	_L,	_L,	_L,
	_L,	_L,	_L,	_P,	_P,	_P,	_P,	_C,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,

/* tolower()  and toupper() conversion table */	0,
	0,	1,	2,	3,	4,	5,	6,	7,
	8,	9,	10,	11,	12,	13,	14,	15,
	16,	17,	18,	19,	20,	21,	22,	23,
	24,	25,	26,	27,	28,	29,	30,	31,
	32,	33,	34,	35,	36,	37,	38,	39,
	40,	41,	42,	43,	44,	45,	46,	47,
	48,	49,	50,	51,	52,	53,	54,	55,
	56,	57,	58,	59,	60,	61,	62,	63,
	64,	97,	98,	99,	100,	101,	102,	103,
	104,	105,	106,	107,	108,	109,	110,	111,
	112,	113,	114,	115,	116,	117,	118,	119,
	120,	121,	122,	91,	92,	93,	94,	95,
	96,	65,	66,	67,	68,	69,	70,	71,
	72,	73,	74,	75,	76,	77,	78,	79,
	80,	81,	82,	83,	84,	85,	86,	87,
	88,	89,	90,	123,	124,	125,	126,	127,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
	0,	0,	0,	0,	0,	0,	0,	0,
/* CSWIDTH information */
	1,	0,	0,	1,	0,	0,	1,
};

