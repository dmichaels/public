/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		device.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/*
 *  add_dev
 *
 *  Add the device with reference designator `refdes', and part
 *  name `part' to the layout.  Return pointer to the new entry.
 */

#define dev_alloc()	((DEVICE *)malloc(sizeof(DEVICE)))

DEVICE *
add_dev(refdes,prtname,layout)
char *refdes, *prtname;
LAYOUT *layout;
{
	DEVICE *find_dev();
	void fatal();
	register DEVICE *d, *prev, *new;
	register int cmp;

	if((d = layout->l_dev) == NULL) {
		if((new = dev_alloc()) == NULL)
			fatal(ENOMEM,"new device list");
		new->d_next = NULL;
		layout->l_dev = new;
		goto Out;
	}
	while(TRUE) {
		if(d == NULL) {
			if((new = dev_alloc()) == NULL)
				fatal(ENOMEM,"new (last) device node");
			prev->d_next = new;
			new->d_next = NULL;
			goto Out;
		}
		/*
		 *  Before d
		 */
		if((cmp = strcmp(refdes,d->d_refdes)) < 0) {
			if((new = dev_alloc()) == NULL)
				fatal(ENOMEM,"new device node");
			new->d_next = d;
			/*
			 *  New first
			 */
			if(d == layout->l_dev)
				layout->l_dev = new;
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
	strncpy(new->d_refdes,refdes,REFLEN);
	new->d_refdes[REFLEN] = EOS;
	strncpy(new->d_name,prtname,REFLEN);
	new->d_name[REFLEN] = EOS;
	new->d_pkgnext = NULL;
	layout->l_ndev++;
	return(new);
}

/*
 *  link_dev
 *
 *  Link together into a list (using the d_pkgnext pointers), all
 *  of the devices which use the same package, and hang each of
 *  those lists off of the appropriate `l_pkg' entry.
 */

void
link_dev(layout)
register LAYOUT *layout;
{
	register DEVICE  *d;
	register PACKAGE *p;

	for(p = layout->l_pkg ; p != NULL ; p = p->pk_next) {
		for(d = layout->l_dev ; d != NULL ; d = d->d_next)
			if(d->d_pkg == p) {
				d->d_pkgnext = p->pk_devlist;
				p->pk_devlist = d;
			}
	}
}

/*
 *  find_dev	(local)
 *
 *  Return pointer to the device structure in the device table
 *  of the layout with the given reference designator `refdes'.
 *  Return NULL if not found.
 */

static DEVICE *
find_dev(refdes,layout)
register char *refdes;
register LAYOUT *layout;
{
	register DEVICE *dev;

	for(dev = layout->l_dev ; dev != NULL ; dev = dev->d_next)
		if(str_eq(refdes,dev->d_refdes))
			return(dev);
	return(NULL);
}
