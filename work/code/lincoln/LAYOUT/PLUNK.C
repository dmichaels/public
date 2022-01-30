/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		plunk.c
* AUTHORS:	David Michaels (david@ll-sst)
*		Nick Bonifanti (nick@ll-sst)
* DATE:		March 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/*
 *  `WS_FACTOR' is the fraction of board to
 *  be added to board's length and width to
 *  determine WorkSpace size.
 *
 *  `Bmap' is a bit map representing a 2-dimensional
 *  coordinate plane. (x1,y1) and (x2,y2) represent
 *  points on the plane of opposite corners of a rectangle.
 */

#define BYTE_SIZE	8
#define	WS_FACTOR	3

static unsigned int	WorkRows;	/* length of WorkSpace */
static unsigned int	WorkCols;	/* width of WorkSpace */
static unsigned int	WorkSpace;	/* area of WorkSpace */
static unsigned int	BmapSize;
static char	*Bmap;

/*
 *  init_board
 *
 *  Initializes WorkSpace; calls place_dev to place board in the
 *  lower right hand corner of WorkSpace.  Parameters len and wid
 *  are dimensions of board.  Returns in (x,y) WorkSpace coordinates
 *  of board's lower left hand corner.
 */

void
init_board(len,wid,x,y)
register unsigned int len, wid;
register int *x, *y;
{
	void	place_dev();

	/*
	 *  WorkSpace is 1/WS_FACTOR longer
	 *  and 1/WS_FACTOR wider than board.
	 */

	WorkRows = len + len/WS_FACTOR;
	WorkCols = wid + wid/WS_FACTOR;
	WorkSpace = WorkRows * WorkCols;
	BmapSize = WorkSpace/BYTE_SIZE;
	Bmap = (char *)calloc(BmapSize,sizeof(char));
	*x = WorkRows - len;
	*y = 0;
	place_dev(len,wid,*x,*y);
}

/*
 *  place_dev
 *
 *  Records on the WorkSpace map `Bmap', the place (x,y) where
 *  the rectangle, whose length and width are len and wid, is to
 *  be placed.  If the place where the rectangle is to put down
 *  is already occupied then this indicates a fatal condition
 *  since it is the caller's responsibility to insure that this
 *  does not happen (through use of find_place()).
 */

void
place_dev(len,wid,x,y)
register unsigned int len, wid;
register int x,y;
{
	void	fatal();
	int	rectangle();

	if(rectangle(x, y, x + len, y + wid) == -1)
		fatal(ERROR,"bug - dup device location passed (place_dev)");

}

/*
 *  find_place
 *
 *  Return in (x,y), the coordinates of the place on the WorkSpace
 *  where rectangle will be placed.  Start at the end of the x-axis
 *  (WorkRows-1,0), and working backwards along the x-axis toward the
 *  origin (0,0), look for the first location where rectangle will fit.
 *  Increment the y coordinate and repeat the above process until the
 *  rectangle fits, or the limits of WorkSpace are reached.  (x,y) will
 *  be (-1,-1) if rectangle will not fit.  Return TRUE if rectangle fits;
 *  FALSE otherwise.
 */

#define bit_index(x,y)	((y)*WorkRows+(x))  /* row major */

bool
find_place(len,wid,x,y)
unsigned int len, wid;
register int *x, *y;
{
	bool will_it_fit(), bit_is_set();
	register int fx, fy;

	for(fy = 0; fy + wid < WorkCols ; fy++) {
		for(fx = WorkRows - len ; fx >= 0; fx--) {
			if(bit_is_set(Bmap,bit_index(fx,fy))) {
				fx -= (len - 1);
				continue;
			}
			if(will_it_fit(fx,fy,fx + len, fy + wid)) {
				*x = fx;
				*y = fy;
				return (TRUE);
			}
		}
	}
	*x = *y = -1;
	return (FALSE);
}

/*
 *  rectangle	(local)
 *
 *  `Bmap' is a bit map representing a 2-dimensional coordinate plane.
 *  (x1,y1) and (x2,y2) represent points on the plane of opposite
 *  corners of a rectangle.  With this data, `rectangle' sets to 1 all
 *  of the points (bits) on the plane (Bmap) which correspond to each
 *  point of the rectangle.  If any of the points to be set (to 1) are
 *  already set (to 1), then the return value is -1, otherwise 0.
 *
 *  NOTE: Serious errors will occur if coordinates are
 *	  not within the bounds of the plane (Bmap).
 *  NOTE: x1 must be <= to x2  and  y1 must be <= to y2.
 */

static int
rectangle(x1,y1,x2,y2)
register int x1, y1, x2, y2;
{
	void	fatal();
	bool	setbit();
	int	t = x1, status = 0;

#ifdef DEBUG
	fprintf(stderr,
	"rectangle: x1 = %d, y1 = %d, x2 = %d, y2 = %d\n",x1,y1,x2,y2);
#endif DEBUG

	if(x1 > x2 || y1 > y2)
		fatal(ERROR,"bug - bad coordinates passed (rectangle)");
	for( ; y1 <= y2 ; y1++)
		for(x1 = t ; x1 <= x2 ; x1++) {
			if(setbit(Bmap,bit_index(x1,y1)) == 1)
				 status = -1;
		}
	return(status);
}

/*
 *  will_it_fit
 *
 *  `Bmap' is a bit map representing a 2-dimensional coordinate plane.
 *  (x1,y1) and (x2,y2) represent points on the plane of opposite corners
 *  of a rectangle.  With this data, `will_it_fit' determines if any of
 *  the points (bits) on the plane (Bmap) which correspond to each point
 *  of the rectangle are set.  If any of the points are set (to 1) already
 *  then `will_it_fit' immediately returns FALSE, otherwise TRUE.
 *
 *  NOTE: Serious errors will occur if coordinates are
 *	  not within the bounds of the plane (Bmap).
 *  NOTE: x1 must be <= to x2  and  y1 must be <= to y2.
 */

bool
will_it_fit(x1,y1,x2,y2)
register int x1, y1, x2, y2;
{
	void	fatal();
	bool	bit_is_set();
	int	t = x1;

	if(x1 > x2 || y1 > y2 || x2 > WorkRows || y2 > WorkCols)
		fatal(ERROR,"bug - bad coordinates passed (will_it_fit)");
	for( ; y1 <= y2 ; y1++)
		for(x1 = t ; x1 <= x2 ; x1++)
			if(bit_is_set(Bmap,bit_index(x1,y1)))
				 return (FALSE);
	return(TRUE);
}
