/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		output.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern BOARD	*Bgeom;

/*
 *  pr_devtbl
 *
 *  Print a formatted device table including:
 *	Reference Designator
 *	Part Name
 *	Package Name
 *	Package Size (# of pins)
 *	Board Location
 */

pr_devtbl(f,w)
register FILE *f;
register WIRELIST *w;
{
	register DEVICE *dev;

	fprintf(f,"\n%-12s%-12s%-12s%-12s%-12s\n"
		,"Designator","Part","Package","Size","Location");
	fprintf(f,"%-12s%-12s%-12s%-12s%-12s\n"
		,"----------","----","-------","----","--------");
	for(dev = w->w_dev ; dev != NULL ; dev = dev->d_next) {
		fprintf(f,"\n%-12s%-12s%-12s%-12d%-10s"
			,dev->d_refdes
			,dev->d_name
			,dev->d_pkg->pk_name
			,dev->d_pkg->pk_npins
			,(Bgeom->b_boardloc)(dev->d_xcoord,dev->d_ycoord));
	}
	putc('\n',f);
}
	
/*
 *  pr_destbl
 *
 *  Print a formatted reference designator table including:
 *	Reference Designator
 *	Package Name
 *	Board Location
 */

pr_destbl(f,w)
register FILE *f;
register WIRELIST *w;
{
	register DEVICE *dev;

	fprintf(f,"%-12s%-12s%-12s\n"
		,"Designator","Part","Location");
	fprintf(f,"%-12s%-12s%-12s"
		,"----------","----","--------");
	for(dev = w->w_dev ; dev != NULL ; dev = dev->d_next) {
		fprintf(f,"\n%-12s%-12s%-10s"
			,dev->d_refdes
			,dev->d_name
			,(Bgeom->b_boardloc)(dev->d_xcoord,dev->d_ycoord));
	}
	putc('\n',f);
}

/*
 *  pr_nettbl
 *
 *  Print a formatted READABLE wirelist.
 */

pr_nettbl(f,w)
register FILE *f;
register WIRELIST *w;
{
	register NET	*net;
	char		s[STR_BUFSIZE];
	register PIN	*p;
	unsigned int	col;

	fprintf(f,"\nReadable Wirelist (%u nets)\n",w->w_nnet);
	fprintf(f,"-----------------\n\n");
	for(net = w->w_net ; net != NULL ; net = net->n_next) {
		fprintf(f,"%-20s",net->n_sig);
		for(col = 0, p = net->n_pinlist ; p != NULL ; p = p->p_next) {
			if(col == 4) {
				fprintf(f,"\n%-20s"," ");
				col = 0;
			}
			col++;
			sprintf(s,"(%s)-%u",pin_ref(p),p->p_pin);
			fprintf(f,"%-13s",s);
		}
		fputs("\n\n",f);
	}
}

/*
 *  pr_netmap
 *
 *  Print the mappings from physical net name to
 *  signal names (as they appear on the wirelist).
 */

void
pr_netmap(f,w)
register FILE *f;
register WIRELIST *w;
{
	register NET *n;

	fprintf(f,"\n%-30s%-30s\n"
		,"Wirelist signal name","VALID physical net name");
	fprintf(f,"%-30s%-30s\n\n"
		,"--------------------","-----------------------");
	for(n = w->w_net ; n != NULL ; n = n->n_next)
		fprintf(f,"%-30s%-30s\n",n->n_sig,n->n_name);

}

/*
 *  pr_pwrpins
 *
 *  Print a formatted table of GND/VCC pins.
 */

pr_pwrpins(f,w)
FILE *f;
WIRELIST *w;
{
	register DEVICE	*d;
	register CNN	*c;
	register PIN	*p;
	unsigned int	col;

	fprintf(f,"\nGround & Voltage Pins\n");
	fprintf(f,"---------------------\n");
	for(d = w->w_dev ; d != NULL ; d = d->d_next) {
		if(d->d_gndlist == NULL && d->d_vcclist == NULL)
			continue;
		fprintf(f,"\n%-10s",d->d_refdes);
		for(col = 0,p = d->d_gndlist; p != NULL; p = p->p_next,col++) {
			if(col == 10) {
				printf(f,"\n%-10s","");
				col = 0;
			}
			fprintf(f,"GND-%-4u",p->p_pin);
		}
		for(col = 0,p = d->d_vcclist; p != NULL; p = p->p_next,col++) {
			if(col == 10) {
				printf(f,"\n%-10s","");
				col = 0;
			}
			fprintf(f,"VCC-%-4u",p->p_pin);
		}
	}
	for(c = Bgeom->b_cnn ; c->c_refdes != NULL ; c++) {
		if(c->c_gndlist == NULL && c->c_vcclist == NULL)
			continue;
		fprintf(f,"\n%-10s",c->c_refdes);
		for(col = 0,p = c->c_gndlist; p != NULL; p = p->p_next,col++) {
			if(col == 10) {
				printf(f,"\n%-10s","");
				col = 0;
			}
			fprintf(f,"GND-%-4u",p->p_pin);
		}
		for(col = 0,p = c->c_vcclist; p != NULL; p = p->p_next,col++) {
			if(col == 10) {
				printf(f,"\n%-10s","");
				col = 0;
			}
			fprintf(f,"VCC-%-4u",p->p_pin);
		}
	}
	putc('\n',f);
}

/*
 *  ck_error
 *
 *  Print  to the standard error file, a message
 *  for each type of error encountered.
 */

unsigned int
ck_error(e)
register int e;
{
	register unsigned int n = 0;

	if(e == 0) {
		fprintf(stderr,"** No errors\n");
		return(0);
	}

	fprintf(stderr, "**\n** Possible PROBLEMS in the following areas:\n");

	if(e & EGEDSYN) {
		fprintf(stderr,"** - graphics file\n");
		n++;
	}
	if(e & EXNETSYN) {
		fprintf(stderr,"** - net list file\n");
		n++;
	}
	if(e & EPGNDSYN) {
		fprintf(stderr,"** - power/ground list file\n");
		n++;
	}
	if(e & EPKGSYN) {
		fprintf(stderr,"** - package description file(s)\n");
		n++;
	}
	if(e & EDEVLOC) {
		fprintf(stderr,"** - device/pin location(s)\n");
		n++;
	}
	if(e & EWIRELST) {
		fprintf(stderr,"** - wirelist file\n");
		n++;
	}
	return(n);
}

/*
 *  fatal
 *
 *  FATAL ERROR! Print given error message `a` and die.
 */

fatal(e,s)
register int e;
register char *s;
{
	switch(e) {
		case ERROR:
			fprintf(stderr,"Fatal error: %s ..exit\n",s);
			exit(1);
		case ENOBOARD:
			fprintf(stderr,"Unknown board type: \"%s\" ..exit\n",s);
			exit(1);
		case EFILE:
			fprintf(stderr,"Can't access %s ..exit\n",s);
			exit(1);
		case ENOMEM:
			fprintf(stderr,"Memory shortage: %s ..exit\n",s);
			exit(1);
		default:
			fprintf(stderr,"Unknown error: %s ..exit\n",s);
			exit(1);
	}
}
