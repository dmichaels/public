/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		ged_parse.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern BOARD	*Bgeom;
extern FILE	*Errfp;
extern char	Gedfile[];

/*
 *  ged_parse
 *
 *  Go through the graphics file, and make an entry for each
 *  device (FORCEADD package) with a REFDES, and CHIP property.
 *  The pertinent lines are:
 *  ----------------------------------------------------------------
 *  FORCEADD DIP<size>..<num>
 *  (<x coord of dev> <y coord of dev>);
 *  FORCEPROP <num> LAST REFDES <reference designator>
 *  (<x coord> <y coord>);
 *  FORCEPROP <num> LAST CHIP <name of chip>
 *  (<x coord> <y coord>);
 *
 *  FORCEADD <board_type_name>..<num>
 *  (<x coord of board> <y coord of board>);
 *  FORCEPROP num LAST REF_PIN <x coord of pin1>,<y coord of pin1>
 *  (<x coord to pin 1 of board> <y coord to pin 1 of board>);
 *  ----------------------------------------------------------------
 *  board location (below UNIV BOARD line)	xorig, yorig
 *  offset to pin 1 of board (after REF_PIN)	xoff, yoff
 *
 *  (bxorigin, byorigin) = (xorig, yorig) + (xoff, yoff)
 */

#define WORDLEN		80
#define NKEYWORD	2

char *keyword[NKEYWORD] = {

#define   FORCEPROP	0
	 "FORCEPROP",

#define   FORCEADD	1
	 "FORCEADD",
};

void
ged_parse(f,w)
register FILE *f;
register WIRELIST *w;
{

	void		fatal(), pr_devtbl();
	BOARD		*find_bgeom();
	int		key, fseek_npat();
	bool		got_refdes = FALSE;
	bool		got_dev = FALSE;
	bool		got_pkg = FALSE;
	bool		got_loc = FALSE;
	char		propname[WORDLEN], prop[WORDLEN];
	char		refdes[WORDLEN], pkgname[WORDLEN], devname[WORDLEN];
	int		xloc, yloc;
	register int	bxorigin, byorigin;
	register DEVICE	*dev;

	*refdes = EOS;

	while((key = fseek_npat(f,keyword,NKEYWORD)) != -1) {
		switch(key) {
		/*
		 *  CASE: FORCEPROP 1 LAST property_name property
		 *
		 *  Ignore any FORCEPROPs before FORCEADDs
		 *  Ignore bad formatted FORCEPROPs
		 */
		case FORCEPROP:
			if(!got_pkg)
				break;
			if(fscanf(f," %*d LAST %s",propname) != 1)
				break;
			if(str_eq(propname,"CHIP")) {
				if(fscanf(f," %s",prop) != 1) {
				  fprintf(Errfp,
				  "Bad CHIP in \"%s\" (last refdes: %s)\n",
				  Gedfile,refdes);
				  w->w_error |= EGEDSYN;
				  break;
				}
				strcpy(devname,prop);
				got_dev = TRUE;
			}
			else if(str_eq(propname,"REFDES")) {
				if(fscanf(f," %s",prop) != 1) {
				  fprintf(Errfp,
				  "Bad REFDES in \"%s\" (last refdes: %s)\n",
				  Gedfile,refdes);
				  w->w_error |= EGEDSYN;
				  break;
				}
				strcpy(refdes,prop);
				got_refdes = TRUE;
			}
			else if(str_eq(propname,"REF_PIN")) {
				bxorigin = xloc;
				byorigin = yloc;
				if(fscanf(f,
				  "%d%*[^0123456789]%d",&xloc,&yloc) != 2) {
				  fprintf(Errfp,
				  "Bad REF_PIN in \"%s\" (board %s)\n",
				  Gedfile,pkgname);
				  w->w_error |= EGEDSYN;
				  break;
				}
				/*
				 *  Set the global variable `Bgeom'
				 */
				if((Bgeom = find_bgeom(pkgname)) == NULL)
				  fatal(ENOBOARD,pkgname);
				bxorigin += xloc;
				byorigin += yloc;
			}
			break;
		/*
		 *  CASE: FORCEADD package_name..1
		 *	  location
		 */
		case FORCEADD:
			if(got_refdes && got_loc && got_pkg && got_dev) {
				got_refdes = FALSE;
				got_loc = FALSE;
				got_pkg = FALSE;
				got_dev = FALSE;
				if(add_dev(refdes,devname,pkgname,xloc,yloc,w)
								      == NULL) {
				  fprintf(Errfp,
				  "Ignoring device: %s %s %s (%d %d)\n",
				  refdes,devname,pkgname,xloc,yloc);
				  w->w_error |= EPKGDESC;
				}
			}
			if(fscanf(f," %[^.]%*[^\n]\n",pkgname) != 1) {
				fprintf(Errfp,
				"Bad FORCEADD in \"%s\" (last refdes: %s)\n",
				Gedfile,refdes);
				w->w_error |= EGEDSYN;
				break;
			}
			got_pkg = TRUE;
			if(fscanf(f," (%d%d ) ;",&xloc,&yloc) != 2) {
				fprintf(Errfp,
				"Bad FORCEADD in \"%s\" (last refdes: %s)\n",
				Gedfile,refdes,pkgname);
				w->w_error |= EGEDSYN;
				break;
			}
			got_loc = TRUE;
			break;
		}
	}
	if(got_refdes && got_loc && got_pkg && got_dev)
		/*
		 *  Last FORCEADD
		 */
		if(add_dev(refdes,devname,pkgname,xloc,yloc,w) == NULL) {
		  fprintf(Errfp,
		  "Ignoring device: %s %s %s (%d %d)\n",
		  refdes,devname,pkgname,xloc,yloc);
		  w->w_error |= EPKGDESC;
		}

	/*
	 *  Subtract out board origin
	 *  from each device location.
	 */

	for(dev = w->w_dev ; dev != NULL ; dev = dev->d_next) {
		dev->d_xcoord -= bxorigin;
		dev->d_ycoord -= byorigin;
	}
	w->w_bxorigin = bxorigin;
	w->w_byorigin = byorigin;
}
