/***********************************************************************
* PROGRAM:	VALID wirelist program
*		BOARD GEOMETRY DEPENDANT ROUTINES
* BOARD:	AUGAT 8136-HPWG1 Universal Board (Dense Pack)
* FILE:		board.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern FILE	*Errfp;

/***********************************************************************
*	AUGAT 8136-HPWG1 Universal Board Geometry Description
************************************************************************
*	1508 (NROWS * NCOLS) possible pin locations
*	58 (NROWS) row locations labelled:
*	1 thru 58
*	26 (NCOLS) column locations labelled:
*	BC,BB,BA,AZ,AY,AX,AW,AV,AU,AT,AS,AR,AP,
*	AN,AM,AL,AK,AJ,AH,AG,AF,AE,AD,AC,AB,AA
************************************************************************
*	84 possible voltage pin locations:
*	12 locations per column labelled:
*	2,7,12,17,22,27,32,37,42,47,52
*	7 columns labelled:
*	BB,AX,AT,AN,AJ,AE,AA
************************************************************************
*	77 possible ground pin locations:
*	11 rows per column labelled:
*	4,9,14,19,24,34,39,44,49,54
*	10 columns labelled:
*	BB,AX,AT,AN,AJ,AE,AA
***********************************************************************/

#define NROWS		58
#define NCOLS		26
#define NPINS		(NROWS * NCOLS)
#define MAXROW		5700
#define MAXCOL		6300

struct col_map_rec{
	unsigned int	 c_coord;
	char    	*c_label;
	char    	*c_gnd;
	char    	*c_vcc;
};

struct row_map_rec{
	char	 *r_label;
	char	 *r_gnd;
	char	 *r_vcc;
};

static struct col_map_rec ColMap[NCOLS] = {

/*
 *	VALID	Board	Nearest	Nearest
 *	Ycoord	Column	 GND	 VCC
 */
	 0	,"BC"	,"BB"	,"BB"
	,300	,"BB"	,"BB"	,"BB"
	,600	,"BA"	,"BB"	,"BB"
	,900	,"AZ"	,"AX"	,"AX"
	,1000	,"AY"	,"AX"	,"AX"
	,1300	,"AX"	,"AX"	,"AX"
	,1600	,"AW"	,"AX"	,"AX"
	,1900	,"AV"	,"AT"	,"AT"
	,2000	,"AU"	,"AT"	,"AT"
	,2300	,"AT"	,"AT"	,"AT"
	,2600	,"AS"	,"AT"	,"AT"
	,2900	,"AR"	,"AN"	,"AN"
	,3000	,"AP"	,"AN"	,"AN"
	,3300	,"AN"	,"AN"	,"AN"
	,3600	,"AM"	,"AN"	,"AN"
	,3900	,"AL"	,"AJ"	,"AJ"
	,4000	,"AK"	,"AJ"	,"AJ"
	,4300	,"AJ"	,"AJ"	,"AJ"
	,4600	,"AH"	,"AJ"	,"AJ"
	,4900	,"AG"	,"AE"	,"AE"
	,5000	,"AF"	,"AE"	,"AE"
	,5300	,"AE"	,"AE"	,"AE"
	,5600	,"AD"	,"AE"	,"AE"
	,5900	,"AC"	,"AA"	,"AA"
	,6000	,"AB"	,"AA"	,"AA"
	,6300	,"AA"	,"AA"	,"AA"
};

static struct row_map_rec RowMap[NROWS] = {

/*
 *	VALID		Board	Nearest	Nearest
 *	Xcoord		Row	 GND	 VCC
 */
	/* 0	*/	 "1"	,"4"	,"2"
	/* 100	*/	,"2"	,"4"	,"2"
	/* 200	*/	,"3"	,"4"	,"2"
	/* 300	*/	,"4"	,"4"	,"2"
	/* 400	*/	,"5"	,"4"	,"7"
	/* 500	*/	,"6"	,"4"	,"7"
	/* 600	*/	,"7"	,"9"	,"7"
	/* 700	*/	,"8"	,"9"	,"7"
	/* 800	*/	,"9"	,"9"	,"7"
	/* 900	*/	,"10"	,"9"	,"12"
	/* 1000	*/	,"11"	,"9"	,"12"
	/* 1100	*/	,"12"	,"14"	,"12"
	/* 1200	*/	,"13"	,"14"	,"12"
	/* 1300	*/	,"14"	,"14"	,"12"
	/* 1400	*/	,"15"	,"14"	,"17"
	/* 1500	*/	,"16"	,"14"	,"17"
	/* 1600	*/	,"17"	,"19"	,"17"
	/* 1700	*/	,"18"	,"19"	,"17"
	/* 1800	*/	,"19"	,"19"	,"17"
	/* 1900	*/	,"20"	,"19"	,"22"
	/* 2000	*/	,"21"	,"19"	,"22"
	/* 2100	*/	,"22"	,"24"	,"22"
	/* 2200	*/	,"23"	,"24"	,"22"
	/* 2300	*/	,"24"	,"24"	,"22"
	/* 2400	*/	,"25"	,"24"	,"27"
	/* 2500	*/	,"26"	,"24"	,"27"
	/* 2600	*/	,"27"	,"29"	,"27"
	/* 2700	*/	,"28"	,"29"	,"27"
	/* 2800	*/	,"29"	,"29"	,"27"
	/* 2900	*/	,"30"	,"29"	,"32"
	/* 3000	*/	,"31"	,"29"	,"32"
	/* 3100	*/	,"32"	,"34"	,"32"
	/* 3200	*/	,"33"	,"34"	,"32"
	/* 3300	*/	,"34"	,"34"	,"32"
	/* 3400	*/	,"35"	,"34"	,"37"
	/* 3500	*/	,"36"	,"34"	,"37"
	/* 3600	*/	,"37"	,"39"	,"37"
	/* 3700	*/	,"38"	,"39"	,"37"
	/* 3800	*/	,"39"	,"39"	,"37"
	/* 3900	*/	,"40"	,"39"	,"42"
	/* 4000	*/	,"41"	,"39"	,"42"
	/* 4100	*/	,"42"	,"44"	,"42"
	/* 4200	*/	,"43"	,"44"	,"42"
	/* 4300	*/	,"44"	,"44"	,"42"
	/* 4400	*/	,"45"	,"44"	,"47"
	/* 4500	*/	,"46"	,"44"	,"47"
	/* 4600	*/	,"47"	,"49"	,"47"
	/* 4700	*/	,"48"	,"49"	,"47"
	/* 4800	*/	,"49"	,"49"	,"47"
	/* 4900	*/	,"50"	,"49"	,"52"
	/* 5000	*/	,"51"	,"49"	,"52"
	/* 5100	*/	,"52"	,"54"	,"52"
	/* 5200	*/	,"53"	,"54"	,"52"
	/* 5300	*/	,"54"	,"54"	,"52"
	/* 5400	*/	,"55"	,"54"	,"52"
	/* 5500	*/	,"56"	,"54"	,"52"
	/* 5600	*/	,"57"	,"54"	,"52"
	/* 5700	*/	,"58"	,"54"	,"52"
};

/*
 *  row_index	(local)
 *
 *  Return an index into the ColMap array
 *  corresponding to the given VALID `y' coordinate.
 *  If `y' is and illegal value, then return -1.
 */

static int
row_index(x)
register int x;
{
	if(x < 0 || x > MAXROW || x % 100 != 0)
		return(-1);
	return(x / 100);
}

/*
 *  col_index	(local)
 *
 *  Return an index into the ColMap array
 *  corresponding to the given VALID `y' coordinate.
 *  If `y' is and illegal value, then return -1.
 */

static int
col_index(y)
register int y;
{
	register int lo, hi, mid;

	if(y < 0 || y > MAXCOL || y % 100 != 0)
		return(-1);
	lo = 0;
	hi = NCOLS - 1;
	while(lo <= hi){
		mid = (lo + hi) / 2;
		if(y < ColMap[mid].c_coord)
			hi = mid - 1;
		else if(y > ColMap[mid].c_coord)
			lo = mid + 1;
		else{
			return(mid);
		}
	}
	return(-1);
}

/***********************************************************************
*	DATA: 	hpwg1cnn
************************************************************************
*	The board connector table
***********************************************************************/

/*
 *  BOARD CONNECTOR INFORMATION
 *
 *  There are two connectors for this board.
 *  J connector ("J1"):		50 pins
 *  P connector ("P1") 		120 pins
 *
 *  The P connector is too big for the VALID system so it
 *  is represented it in three parts: P1A, P1B, and P1C.
 */

CNN hpwg1cnn[] = {

/*
 *	 VALID	wirelist  			GND	VCC
 *	 refdes	refdes	  name		size	list	list
 */

	{ "J1"	,"J1"	,"ECON50"	,50	,NULL	,NULL },
	{ "P1A"	,"P1"	,"ECON120"	,120	,NULL	,NULL },
	{ "P1B"	,"P1"	,"ECON120"	,120	,NULL	,NULL },
	{ "P1C"	,"P1"	,"ECON120"	,120	,NULL	,NULL },
	{ NULL	,NULL	,NULL		,0	,NULL	,NULL }
};

#define is_p1cnn(cnn)  (strn_eq((cnn->c_refdes),"P1",2))  /* kludge */
#define is_j1cnn(cnn)  (str_eq((cnn->c_refdes),"J1"))

struct near_cnn_pwr {
	char	*n_gnd;
	char	*n_vcc;
};

static struct near_cnn_pwr p1nearpwr[] = {

/*
 *	P1 Connector			     Nearest
 *	Pin numbers			    GND    VCC
 */
	/* 1-8,41-48,81-88 */		{ "AX54", "AX57" },
	/* 9-16,49-56,89-96 */		{ "AT54", "AT57" },
	/* 17-24,57-64,97-104 */	{ "AN54", "AN57" },
	/* 25-32,65-72,105-112 */	{ "AJ54", "AJ57" },
	/* 33-40,73-80,113-120 */	{ "AE54", "AE57" },
};

static struct near_cnn_pwr j1nearpwr[] = {

/*
 *	J1 Connector			     Nearest
 *	Pin numbers			    GND    VCC
 */
	/* 1-10 */			{ "AX4", "AX2" },
	/* 11-20 */			{ "AT4", "AT2" },
	/* 21-30 */			{ "AT4", "AT2" },
	/* 31-40 */			{ "AN4", "AN2" },
	/* 41-50 */			{ "AJ4", "AJ2" }
};

/***********************************************************************
*	FUNCTION: 	hpwg1cnngnd
************************************************************************
*	Return in board geometry form, the location of the
*	nearest ground pin to the given board connector pin.
*	If the given connector or pin is illegal, return NULL.
***********************************************************************/

char *
hpwg1cnngnd(cnn,pin)
CNN *cnn;
register int pin;
{
	if(cnn == NULL)
		return(NULL);
	if(pin > cnn->c_npins || pin <= 0)
		return(NULL);
	if(is_p1cnn(cnn)){
		pin = pin - ((pin / 41) * 40);
		pin = (--pin) / 8;
		return(p1nearpwr[pin].n_gnd);
	}
	if(is_j1cnn(cnn))
		return(j1nearpwr[pin / 11].n_gnd);
}

/***********************************************************************
*	FUNCTION: 	hpwg1cnnvcc
************************************************************************
*	Return in board geometry form, the location of the
*	nearest voltage pin to the given board connector pin.
*	If the given connector name or pin is illegal, return NULL.
***********************************************************************/

char *
hpwg1cnnvcc(cnn,pin)
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
*	FUNCTION: 	hpwg1boardloc
************************************************************************
*	Return in board geometry form, the
*	location ofthe given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
***********************************************************************/

char *
hpwg1boardloc(x,y)
int x, y;
{
	int		row_index();
	int		col_index();
	static char	s[BGLLEN + 1];
	int		row, col;

	if((row = row_index(x)) < 0 || (col = col_index(y)) < 0)
		return(NULL);
	sprintf(s,"%s%s",ColMap[col].c_label,RowMap[row].r_label);
	return(s);
}


/***********************************************************************
*	FUNCTION: 	hpwg1gnd
************************************************************************
*	Return in board geometry form, the location of the
*	nearest ground pin to the given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
***********************************************************************/

char *
hpwg1gnd(x,y)
unsigned int x,y;
{
	int		row_index();
	int		col_index();
	static char	s[BGLLEN + 1];
	int		row, col;

	if((row = row_index(x)) < 0 || (col = col_index(y)) < 0)
		return(NULL);
	sprintf(s,"%s%s",ColMap[col].c_gnd,RowMap[row].r_gnd);
	return(s);
}

/***********************************************************************
*	FUNCTION: 	hpwg1vcc
************************************************************************
*	Return in board geometry form, the location of the
*	nearest voltage pin to the given VALID coordinates.
*	If (x,y) is an illegal coordinate, return NULL.
***********************************************************************/

char *
hpwg1vcc(x,y)
unsigned int x,y;
{
	int		row_index();
	int		col_index();
	static char	s[BGLLEN + 1];
	int		row, col;

	if((row = row_index(x)) < 0 || (col = col_index(y)) < 0)
		return(NULL);
	sprintf(s,"%s%s",ColMap[col].c_vcc,RowMap[row].r_vcc);
	return(s);
}

/***********************************************************************
*	FUNCTION: 	hpwg1okdevloc
************************************************************************
*	Check each pin of each device on the given device list to
*	make sure that the locations are legal; specifically, make
*	sure that no device pins are:
*		1. over the edge of the board
*		2. in between rows or columns
*		3. occupying the same board location
*	If all is well, return TRUE, otherwise print a
*	message in the error log file `Errfp' and return FALSE.
***********************************************************************/

#define BYTE_SZ		8
#define bit_index(x,y)	((y) * NROWS + (x))
#define BMAPSIZE	((NPINS + BYTE_SZ - 1) / BYTE_SZ)

hpwg1okdevloc(dev)
register DEVICE *dev;
{
	char		*calloc(), *hpwg1boardloc();
	int		row_index(), col_index();
	bool		setbit();
	void		fatal();
	register int	i, row, col;
	char		*bmap;
	bool		error = FALSE;

	if((bmap = (char *)calloc(BMAPSIZE,sizeof(char))) == NULL)
		fatal(ENOMEM,"checking device locations");
	for( ; dev != NULL ; dev = dev->d_next){
		for(i = 0; i <  dev->d_pkg->pk_npins ; i++){
			if((row = row_index(dev_xpin(dev,i+1))) < 0 ||
			   (col = col_index(dev_ypin(dev,i+1))) < 0){
				fprintf(Errfp,
				"illegal pin location: (%s)-%d %s ; (%d %d)\n",
				dev->d_refdes,i+1,dev->d_pkg->pk_name,
				dev_xpin(dev,i+1),dev_ypin(dev,i+1));
				error = TRUE;
				continue;

			}
			if(setbit(bmap,bit_index(row,col)) == TRUE){
				fprintf(Errfp,
				"pin conflict: (%s)-%d %s ; (%d %d) %s\n",
				dev->d_refdes,i+1,dev->d_pkg->pk_name,
				dev_xpin(dev,i+1),dev_ypin(dev,i+1),
				hpwg1boardloc(dev->d_xcoord,dev->d_ycoord));
				error = TRUE;
				continue;
			}
		}
	}
	free(bmap);
	return(!error);
}
