/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		output.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		March 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

/*
 *  pr_devtbl
 *
 *  Print a table of reference designators,
 *  device (part) names, and package names,
 *  ordered alphabetically be reference designator.
 */

pr_devtbl(f,layout)
register FILE *f;
LAYOUT *layout;
{
	register DEVICE *d;

	fprintf(f,"\n%-15s%-15s%-15s\n"
		,"Designator","Part","Package");
	fprintf(f,"%-15s%-15s%-15s\n\n"
		,"----------","----","-------");

	for(d = layout->l_dev ; d != NULL ; d = d->d_next) {
		fprintf(f,"%-15s%-15s%-15s\n",
		d->d_refdes,d->d_name,
		d->d_pkg->pk_name != NULL ? d->d_pkg->pk_name : "---");
#ifdef NEVER_DEFINED
		fprintf(f,"%-15s%-15s%-15s\n",
			d->d_refdes,
			d->d_name,
			d->d_pkg->pk_name);
#endif NEVER_DEFINED
	}
	putc('\n',f);
}

/*
 * pr_pkgtbl
 *
 *  Print the layout table ordered by package.
 */

pr_pkgtbl(f,layout)
register FILE *f;
LAYOUT *layout;
{
	register PACKAGE *p;
	register DEVICE *d;
	unsigned int i, nfield = 0;
	char s[REFLEN + PRTLEN + 4];

	fprintf(f,"%-15s%-15s\n","Package","Devices");
	fprintf(f,"%-15s%-15s\n","-------","-------");

	for(p = layout->l_pkg ; p != NULL ; p = p->pk_next) {
		nfield = 0;
		fprintf(f,"\n%-15s",p->pk_name);
		for(d = p->pk_devlist ; d != NULL ; d = d->d_pkgnext) {
			if(nfield == 3) {
				fprintf(f,"\n%-15s"," ");
				nfield = 0;
			}
			sprintf(s,"(%s %s)",d->d_refdes,d->d_name);
			fprintf(f,"%-20s",s);
			nfield++;
		}
	}
	putc('\n',f);
}

/*
 *  error
 *
 *  Non-fatal error.  Print message and return.
 */

void
error(s)
register char *s;
{
	fprintf(stderr,"**** %s...continue.\n",s);
}

/*
 *  fatal
 *
 *  Fatal Error!  Print message and die.
 */

void
fatal(e,s)
register int e;
register char *s;
{
	switch (e) {
		case ERROR:
			fprintf(stderr,"fatal error: %s\n",s);
			exit(1);
		case EFILE:
			fprintf(stderr,"can't open %s\n", s);
			exit(1);
		case ESYNTAX:
			fprintf(stderr,"bad syntax: %s\n",s);
			exit(1);
		case ENOMEM:
			fprintf(stderr,"no memory! ughhhh... : %s\n", s);
			exit(1);
		default:
			fprintf(stderr,"undefined fatal error: %s\n", s);
			exit(1);
	}
}
