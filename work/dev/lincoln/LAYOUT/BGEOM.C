/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		bgeom.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/*
 *  The following is the board geometry table `bgeomtbl'
 *
 *  `name'	- name of the board geometry (case is ignored).
 *  `length'	- overall physical length of board (1000ths inch).
 *  `width'	- overall physical width of board (1000ths inch).
 *  `x'		- x offset to pin 1 of the board (1000ths inch).
 *  `y'		- y offset to pin 1 of the board (1000ths inch).
 */

static BOARD bgeomtbl[] = {

/*
 *	   Name		length		width		x     y
 */

	{ "UWG12A", norm_coord(7000), norm_coord(7000), 400, 400 },

	{ "HPWG1A", norm_coord(7375), norm_coord(7000), 600, 500 },
};

static unsigned int NBGEOM = sizeof(bgeomtbl)/sizeof(BOARD);

/*
 *  find_bgeom
 *
 *  Return a pointer to the layout BOARD entry
 *  of the given board geometry name (ignore case).
 *  Return NULL if not found.
 */

BOARD *
find_bgeom(name)
register char *name;
{
	char *strupper();
	register BOARD *b;
	register unsigned int n = 0;

	for(b = bgeomtbl ; n++ < NBGEOM ; b++)
		if(str_eq(strupper(name),b->b_name))
			return(b);
	return(NULL);
}
