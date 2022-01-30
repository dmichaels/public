/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		lister.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

extern FILE	*Errfp;
extern BOARD	*Bgeom;

/*
 *  wirelist
 *
 *  Make the final wirelist.
 */

void
wirelist(f,w)
register FILE *f;
WIRELIST *w;
{
	void		net_call();
	char		*vf_pkgdir();
	register NET	*net;
	char		*board_dir, *header;
	FILE		*h;
	register char	c;

	/*
	 *  Prepend the (board dependant) wirelist header to
	 *  the wirelist.  Warning if header file not found.
	 */
	if((board_dir = vf_pkgdir(Bgeom->b_name)) == NULL) {
		fprintf(Errfp,
		"Ignoring \"%s\" header (can't access directory)\n",
		Bgeom->b_name);
		w->w_error |= EWIRELST;
		goto Makeit;
	}
	header = vf_brdlsthdr(board_dir);
	if((h = fopen(header,"r")) == NULL) {
		fprintf(Errfp,
		"Ignoring \"%s\" header (can't access \"%s\")\n",
		Bgeom->b_name,header);
		w->w_error |= EWIRELST;
		goto Makeit;
	}

	/*
	 *  Prepend the header
	 */
	while((int)(c = getc(h)) != EOF)
		putc(c,f);
	fclose(h);

Makeit:
	for(net = w->w_net ; net != NULL ; net = net->n_next)
		net_call(f,net);
}

/*
 *  net_call	(local)
 *
 *  --------------------------------------------------------------------
 *  Output to the named file, a net callout for the given net
 *  according to the conventions of the Lincoln Laboratory
 *  wirewrap service (group 72).
 *  --------------------------------------------------------------------
 *  WIRELIST FORMAT:
 *	column	1	format control number (1, 2 or 3)
 *	columns	2-3	empty
 *	column	4	signal name continuation code (1 or 9)
 *	columns 5-14	signal name (blanks significant)
 *	columns 15-24	pin callout
 *	columns 25-34	pin callout
 *	columns 35-44	pin callout
 *	columns 45-54	pin callout
 *	columns 55-64	pin callout
 *  --------------------------------------------------------------------
 *  FORMAT CONTROL NUMBERS:
 *	1	`from-to' or `point-to-point' listing
 *		 - exactly 2 pin_names per line
 *		 - signal name optional (recommended)
 *		 - pin callouts wired in same sequence as listed
 *		 - use continuation code 9
 *	2	`to-to' or `signal-pin' listing
 *		 - up to 5 pin_names per line
 *		 - use continuation code 1
 *	3	 `twisted pairs' listing
 *		 - exactly 4 pin_names per line
 *		 - signal name optional (recommended)
 *
 *	* We will only use number 2.
 *  --------------------------------------------------------------------
 *  PIN CALLOUT NOMENCLATURE:	(Handled by "pin_call()")
 *
 *  Normal Signals:
 *  --------------
 *  Old format:		<size><column><row>-<pin #>
 *			 - blanks not significant
 *			 - for example. "16C18-13" for a 16 pin device
 *			   at location C18 to be wrapped on pin 13.
 *
 *  New Format:		<column><row>
 *
 *  Ground Signals:	<column><row>-G	
 *  --------------
 *  Voltage Signals:	<column><row>-V	
 *  --------------
 *  Connector Pins:	J1-<pin #>
 *  --------------
 *			P1-<pin #>
 *  --------------------------------------------------------------------
 */

#define pr_field(file,str)	fprintf((file),"%-10s",(str))

static void
net_call(f,net)
FILE *f;
register NET *net;
{
	char *pin_call(), *gnd_loc(), *vcc_loc();
	register PIN *p;
	unsigned int i, callouts;
	char fcn = '2';		/* format control no. */
	char cc = '1';		/* continuation code */
	char stmp[LSTLEN];	/* temporary string */

	/*
	 *  Skip nets with:
	 *  1. no associated pins
	 *  2. with only one associated pin (except gnd/vcc)
	 *  3. with net name 'NC' (not connected).
	 */

	if(net->n_pinlist == NULL
	   || (net->n_pinlist->p_next == NULL
	       && !(is_gnd(net->n_sig) || is_vcc(net->n_sig)))
	   || no_pins(net->n_sig))
		return;

	callouts = 0;

	/*
	 *  Skip continuation
	 *  code column.
	 */
	fprintf(f,"%-4c",fcn);

	/*
	 *  Print signal name.
	 */
	pr_field(f,net->n_sig);

	/*
	 *  Print gnd/vcc callouts if gnd/vcc net.
	 */
	if(is_gnd(net->n_sig)) {
		sprintf(stmp,"%s-G",gnd_loc(net));
		pr_field(f,stmp);
		callouts++;
	}
	else if(is_vcc(net->n_sig)) {
		sprintf(stmp,"%s-V",vcc_loc(net));
		pr_field(f,stmp);
		callouts++;
	}

	/*
	 *  Print pin callouts.
	 */
	for(p = net->n_pinlist ; p != NULL ; p = p->p_next) {
		if(callouts == 5) {
			callouts = 0;
			fprintf(f,"\n%-3c%c%-10s",fcn,cc,net->n_sig);
		}
		pr_field(f,pin_call(p));
		callouts++;
	}
	putc('\n',f);
}

/*
 *  pin_call	(local)
 *
 *  Return the appropriate pin callout for the given pin structure
 *  Handles connector callouts.  Does not handle gnd/vcc callouts.
 */

static char *
pin_call(p)
register PIN *p;
{
	static char s[LSTLEN+1];

	if(is_cnn(p)) 
		sprintf(s,"%s-%u",p->p_owner.cnn->c_lstdes,p->p_pin);
	/*
	 *  NEW FORMAT: <pin location>
	 *  OLD FORMAT: <size><location>-<pin>
	 */
	else if(is_dev(p))
		sprintf(s,"%s",pin_loc(p));
	else
		return(NULL);
	return(s);
}
