/***********************************************************************
* PROGRAM:	VALID wirelist program
*		BOARD GEOMETRY DEPENDANT ROUTINES
* BOARD:	AUGAT 8136-UWG12 Universal Board
* FILE:		board.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern FILE	*Errfp;

/***********************************************************************
*	AUGAT 8136-UWG12 Universal Board Geometry Description
************************************************************************
*	1160 (NROWS * NCOLS) possible pin locations
*	58 (NROWS) row locations labelled:
*	1 thru 58
*	20 (NCOLS) column locations labelled:
*	A,B,C,D,E,F,G,H,J,K,L,M,N,P,R,S,T,U,V
************************************************************************
*	70 possible voltage pin locations:
*	7 rows per column labelled:
*	4,12,20,28,36,44,52
*	10 columns labelled:
*	B,D,F,H,K,M,P,S,U,W
************************************************************************
*	70 possible ground pin locaitons:
*	7 rows per column labelled:
*	8,16,24,32,40,48,56
*	10 columns labelled:
*	B,D,F,H,K,M,P,S,U,W
************************************************************************/

#define NROWS		58
#define NCOLS		20
#define NPINS		(NROWS * NCOLS)
#define MAXROW		((NROWS-1) * ROW_UNIT)
#define MAXCOL		((NCOLS-1) * COL_UNIT)
#define ROW_UNIT	100
#define COL_UNIT	300

#define ok_xcoord(x)	((x) % ROW_UNIT == 0 && (x) <= MAXROW)
#define ok_ycoord(y)	((y) % COL_UNIT == 0 && (y) <= MAXCOL)

#define norm_xcoord(x)	((x) / ROW_UNIT)
#define norm_ycoord(y)	((y) / COL_UNIT)

static char		ColMap[] = "ABCDEFGHJKLMNPRSTUVW";

static char		VccCol[] = "BDFHKMPSUW";
static unsigned int	VccRow[] = { 4,12,20,28,36,44,52 };

static char		GndCol[] = "BDFHKMPSUW";
static unsigned int	GndRow[] = { 8,16,24,32,40,48,56 };

#define brd_row(x)	((unsigned int)(x) + 1)
#define brd_col(y)	((char)ColMap[(int)(y)])

/*
 *  norm_coord	(local)
 *
 *  This function returns in (xnorm,ynorm), the normalized VALID
 *  coordinates of the given VALID coordinates (x,y).  That is:
 *  0 <= xnorm < NROWS and 0 <= ynormv < NCOLS.  Return TRUE if
 *  the conversion was successful. and FALSE if not (i.e. illegal
 *  coordinate(s)), For example if a coordinate is too large, or
 *  is between rows or columns).  This is only used locally by these
 *  board geometry dependant rountines as an aid in translating VALID
 *  coordinates to board geometry coordinates.  It is completly transparent
 *  to the common code which only knows about VALID coordinate and board
 *  geometry locations.
 */

static bool
norm_coord(x,y,xnorm,ynorm)
register int x, y;
register int *xnorm, *ynorm;
{
	if(!ok_xcoord(x) || !ok_ycoord(y))
		return(FALSE);
	*xnorm = norm_xcoord(x);
	*ynorm = norm_ycoord(y);
	return(TRUE);
}

/***********************************************************************
*	DATA: 	uwg12cnn
************************************************************************
*	The board connector table
***********************************************************************/

/*
 *  There are two connectors for this board.
 *  J connector ("J1"):		50 pins
 *  P connector ("P1") 		120 pins
 *
 *  The P connector is too big for the VALID system so it
 *  is represented it in three parts: P1A, P1B, and P1C.
 */

CNN uwg12cnn[] = {

/*
 *		VALID	wirelist			GND	VCC
 *		refdes	refdes	  name		size	list	list
 */
	{	"J1"	,"J1"	,"ECON50"	,50	,NULL	,NULL	},
	{	"P1A"	,"P1"	,"ECON120"	,120	,NULL	,NULL	},
	{	"P1B"	,"P1"	,"ECON120"	,120	,NULL	,NULL	},
	{	"P1C"	,"P1"	,"ECON120"	,120	,NULL	,NULL	},
	{	NULL	,NULL	,NULL		,0	,NULL	,NULL	}
};

#define is_p1cnn(cnn)  (strn_eq((cnn->c_refdes),"P1",2))  /* kludgy */
#define is_j1cnn(cnn)  (str_eq((cnn->c_refdes),"J1"))

struct near_cnn_pwr {
	char	*n_gnd;
	char	*n_vcc;
};

static struct near_cnn_pwr p1nearpwr[] = {

/*
 *	P1 Connector			    Nearest
 *	Pin numbers			   GND  VCC
 */
	/* 1-6,41-46,81-86 */		{ "S8",	"S4" },
	/* 7-12,47-52,87-92 */		{ "P8",	"P4" },
	/* 13-18,53-58,93-98 */		{ "M8",	"M4" },
	/* 19-24,59-64,99-104 */	{ "K8",	"K4" },
	/* 25-31,65-70,105-110 */	{ "H8",	"H4" },
	/* 31-37,71-76,111-116 */	{ "F8",	"F4" },
	/* 37-40,77-80,117-120 */	{ "D8",	"D4" }
};

static struct near_cnn_pwr j1nearpwr[] = {

/*
 *	J1 Connector			    Nearest
 *	Pin numbers			   GND   VCC
 */
	/* 1-10 */			{ "P56", "P52" },
	/* 11-20 */			{ "M56", "M52" },
	/* 21-30 */			{ "K56", "K52" },
	/* 31-40 */			{ "H56", "H52" },
	/* 41-50 */			{ "F56", "F52" }
};

/***********************************************************************
*	FUNCTION: 	uwg12cnngnd
************************************************************************
*	Return in board geometry form, the location of the
*	nearest ground pin to the given board connector pin.
*	If the given connector or pin is illegal, return NULL.
***********************************************************************/

char *
uwg12cnngnd(cnn,pin)
CNN *cnn;
register int pin;
{
	if(cnn == NULL)
		return(NULL);
	if(pin > cnn->c_npins || pin <= 0)
		return(NULL);
	if(is_p1cnn(cnn)){
		pin = pin - ((pin / 41) * 40);
		pin = (--pin) / 6;
		return(p1nearpwr[pin].n_gnd);
	}
	if(is_j1cnn(cnn))
		return(j1nearpwr[pin / 11].n_gnd);
}

/***********************************************************************
*	FUNCTION: 	uwg12cnnvcc
************************************************************************
*	Return in board geometry form, the location of the
*	nearest voltage pin to the given board connector pin.
*	If the given connector name or pin is illegal, return NULL.
***********************************************************************/

char *
uwg12cnnvcc(cnn,pin)
CNN *cnn;
register int pin;
{
	if(cnn == NULL)
		return(NULL);
	if(pin > cnn->c_npins || pin <= 0)
		return(NULL);
	if(is_p1cnn(cnn)){
		pin = pin - ((pin / 41) * 40);
		pin = (--pin) / 6;
		return(p1nearpwr[pin].n_vcc);
	}
	if(is_j1cnn(cnn))
		return(j1nearpwr[pin / 11].n_vcc);
}

/***********************************************************************
*	FUNCTION: 	uwg12boardloc
************************************************************************
*	Return in board geometry form, the
*	location ofthe given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
***********************************************************************/

char *
uwg12boardloc(x,y)
int x, y;
{
	bool		norm_coord();
	unsigned int	xnorm, ynorm;
	static	char	s[BGLLEN + 1];

	if(!norm_coord(x,y,&xnorm,&ynorm))
		return(NULL);

	sprintf(s,"%c%u",brd_col(ynorm),brd_row(xnorm));
	return(s);
}

/***********************************************************************
*	FUNCTION: 	uwg12gnd
************************************************************************
*	Return in board geometry form, the location of the
*	nearest ground pin to the given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
*	You figure it out.
***********************************************************************/

char *
uwg12gnd(x,y)
unsigned int x,y;
{
	bool		norm_coord();
	unsigned int	xnorm, ynorm;
	register int	m, q, r, c;
	static	char	s[BGLLEN + 1];

	if(!norm_coord(x,y,&xnorm,&ynorm))
		return(NULL);

	if(ynorm % 2 != 1)
		c = (ynorm == 0 ? 0 : (ynorm - 1) / 2);
	else
		c = ynorm / 2;
	if((m = (r = brd_row(xnorm)) % 8) != 0)
		if((q = r / 8) == 0)
		 	r = 1;
		else
			r = (m > 4 ? q + 1 : q);
	else
		r /= 8;
	sprintf(s,"%c%u",GndCol[c],GndRow[r-1]);
	return(s);
}

/***********************************************************************
*	FUNCTION: 	uwg12vcc
************************************************************************
*	Return in board geometry form, the location of the
*	nearest voltage pin to the given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
*	You figure it out.
***********************************************************************/

char *
uwg12vcc(x,y)
unsigned int x,y;
{
	bool		norm_coord();
	unsigned int	xnorm, ynorm;
	register int	m, q, r, c;
	static	char	s[BGLLEN + 1];

	if(!norm_coord(x,y,&xnorm,&ynorm))
		return(NULL);

	if(ynorm % 2 != 1)
		c = (ynorm == 0 ? 0 : (ynorm - 1) / 2);
	else
		c = ynorm / 2;
	if((m = (r = brd_row(xnorm)) % 4) != 0 || r % 8 == 0 )
		r = r / 8 + 1;
	else
		r /= 8;
	sprintf(s,"%c%u",VccCol[c],VccRow[r-1]);
	return(s);
}


/***********************************************************************
*	FUNCTION: 	uwg12okdevloc
************************************************************************
*	Check each pin of each device on the given device list to
*	make sure that the locations are legal; specifically, make
*	sure that no device pins are:
*		1. over the edge of the board
*		2. in between rows or columns
*		3. occupying the same board location
*	If all is well, return TRUE, otherwise print a
*	message in the error log file `Errfp' and return FALSE.
************************************************************************/

#define BYTE_SZ		8
#define bit_index(x,y)	((y) * NROWS + (x))
#define BMAPSIZE	((NPINS + BYTE_SZ - 1) / BYTE_SZ)

uwg12okdevloc(dev)
register DEVICE *dev;
{
	char			*calloc(), *uwg12boardloc();
	int			setbit();
	void			fatal();
	register unsigned int	i;
	unsigned int		xnorm, ynorm;
	char			*bmap;
	bool			error = FALSE;

	if((bmap = (char *)calloc(BMAPSIZE,sizeof(char))) == NULL)
		fatal(ENOMEM,"checking device locations");
	for( ; dev != NULL ; dev = dev->d_next){
		if(!norm_coord(dev->d_xcoord,dev->d_ycoord,&xnorm,&ynorm)){
			fprintf(Errfp,
			"illegal device location: (%s)-%d %s ; (%d %d)\n",
			dev->d_refdes,i+1,dev->d_pkg->pk_name,
			dev->d_xcoord,dev->d_ycoord);
			error = TRUE;
			continue;
		}
		for(i = 0; i <  dev->d_pkg->pk_npins ; i++){
			if(!norm_coord(dev_xpin(dev,i+1),dev_ypin(dev,i+1),
				       &xnorm,&ynorm)){
				fprintf(Errfp,
				"illegal location: (%s)-%d %s ; (%d %d)\n",
				dev->d_refdes,i+1,dev->d_pkg->pk_name,
				dev_xpin(dev,i+1),dev_ypin(dev,i+1));
				error = TRUE;
				continue;

			}
			if(setbit(bmap,bit_index(xnorm,ynorm))){
				fprintf(Errfp,
				"pin conflict: (%s)-%d %s ; (%d %d) %s\n",
				dev->d_refdes,i+1,dev->d_pkg->pk_name,
				xnorm,ynorm,
				uwg12boardloc(dev->d_xcoord,dev->d_ycoord));
				error = TRUE;
				continue;
			}
		}
	}
	free(bmap);
	return(!error);
}
