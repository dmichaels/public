/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		device.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern BOARD	*Bgeom;
extern FILE	*Errfp;

/*
 *  add_dev
 *
 *  Add to the wirelist, a device with reference designator `refdes',
 *  name `devname', package name `pkgname', and VALID coordinates
 *  (xcoord , ycoord).  Return the newly added device struture
 *  or NULL if an error occured.
 */

#define newdev()  ((DEVICE *)malloc(sizeof(DEVICE)))

DEVICE *
add_dev(refdes,devname,pkgname,xcoord,ycoord,w)
char *refdes, *devname, *pkgname;
int xcoord, ycoord;
WIRELIST *w;
{
	DEVICE		*find_dev();
	void		fatal();
	PACKAGE		*pkg, *pkg_desc();
	register DEVICE	*d, *prev, *new;
	register int	cmp;

	if((pkg = pkg_desc(pkgname,w)) == NULL) {
		fprintf(Errfp,
		"Ignoring device: %s %s %s %s (no package description)\n",
		refdes,devname,pkgname,(Bgeom->b_boardloc)(xcoord,ycoord));
		return(NULL);
	}
	if((d = w->w_dev) == NULL) {
		if((new = newdev()) == NULL)
			fatal(ENOMEM,"new device list");
		new->d_next = NULL;
		w->w_dev = new;
		goto Out;
	}
	while(TRUE) {
		if(d == NULL) {
			if((new = newdev()) == NULL)
				fatal(ENOMEM,"new (last) device node");
			prev->d_next = new;
			new->d_next = NULL;
			goto Out;
		}
		/*  before d */
		if((cmp = strcmp(refdes,d->d_refdes)) < 0) {
			if((new = newdev()) == NULL)
				fatal(ENOMEM,"new device node");
			new->d_next = d;
			/*  New first */
			if(d == w->w_dev)
				w->w_dev = new;
			else
				prev->d_next = new;
			goto Out;
		}
		if(cmp == 0)
			return(d);
		prev = d;
		d = d->d_next;
	}
Out:
	new->d_pkg = pkg;
	strncpy(new->d_refdes,refdes,REFLEN);
	new->d_refdes[REFLEN] = EOS;
	strncpy(new->d_name,devname,DEVLEN);
	new->d_name[DEVLEN] = EOS;
	new->d_xcoord = xcoord;
	new->d_ycoord = ycoord;
	new->d_gndlist = NULL;
	new->d_vcclist = NULL;
	w->w_ndev++;
	return(new);
}

/*
 *  find_dev
 *
 *  Return pointer to the device structure in the DEVICE table
 *  of the WIRELIST table with the given reference designator
 *  `refdes'.  Return NULL if not found.
 */

DEVICE *
find_dev(refdes,w)
register char *refdes;
register WIRELIST *w;
{
	register DEVICE *dev;

	for(dev = w->w_dev ; dev != NULL ; dev = dev->d_next)
		if(str_eq(refdes,dev->d_refdes))
			return(dev);
	return(NULL);
}

/*
 *  find_cnn
 *
 *  Return pointer to the connector structure (CNN) of
 *  the board geometry with the given reference
 *  designator `refdes'.  Return NULL if not found.
 */

CNN *
find_cnn(refdes,w)
register char *refdes;
register BOARD *w;
{
	register CNN *c;

	for(c = Bgeom->b_cnn ; c->c_refdes != NULL ; c++)
		if(str_eq(refdes,c->c_refdes))
			return(c);
	return(NULL);
}
