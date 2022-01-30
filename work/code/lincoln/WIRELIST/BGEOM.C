/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		bgeom.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/* AUGAT 8136-UWG12 Universal Panel */
extern char		*uwg12boardloc();
extern char		*uwg12gnd();
extern char		*uwg12vcc();
extern char		*uwg12cnngnd();
extern char		*uwg12cnnvcc();
extern bool		uwg12okdevloc();
extern CNN		uwg12cnn;

/* AUGAT 8136-HPWG1 Universal Panel (dense pack) */
extern char		*hpwg1boardloc();
extern char		*hpwg1gnd();
extern char		*hpwg1vcc();
extern char		*hpwg1cnngnd();
extern char		*hpwg1cnnvcc();
extern bool		hpwg1okdevloc();
extern CNN		hpwg1cnn;

static BOARD bgeomsw[] = {

	{ "UWG12A",				/* AUGAT 8136-UWG12 */
	uwg12boardloc, uwg12gnd, uwg12vcc,
	uwg12cnngnd, uwg12cnnvcc,
	uwg12okdevloc, &uwg12cnn },

	{ "HPWG1A",				/* AUGAT 8136-HPWG1 */
	hpwg1boardloc, hpwg1gnd, hpwg1vcc,
	hpwg1cnngnd, hpwg1cnnvcc,
	hpwg1okdevloc, &hpwg1cnn },
};

static unsigned int NBGEOM = sizeof(bgeomsw)/sizeof(BOARD);

/*
 *  find_bgeom
 *
 *  Return the board geometry entry into the bgeomsw table
 *  entry of the given board geometry name (ignore case).
 *  Return NULL if not found.
 */
 
BOARD *
find_bgeom(board)
register char *board;
{
	char *strupper();
	register BOARD *b;
	register unsigned int n = 0;

	for(b = bgeomsw ; n++ < NBGEOM ; b++)
		if(str_eq(strupper(board),b->b_name))
			return(b);
	return(NULL);
}
