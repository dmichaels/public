/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		xnet_parse.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern char	Xnetfile[];
extern FILE	*Errfp;

/*
 *  xnet_parse
 *
 *  Parse the VALID expanded net list file.
 *  Make an entry in the wirelist for each defined net.
 *
 *  File format:
 *  --------------------------------------------------------------
 *  EXPANDED NET LIST - <sequence #> <date/time>
 *  <user info>
 *  NET_NAME
 *  'physical_net_name' logical_net_name (VCS):version
 *  net properties[,net properties]...		(not relevant)
 *  ;
 *  NODE_NAME   reference_designator  pin_number
 *  'logical designator'  bit  version : pin name (VCS)
 *   node properties[,node properties]...	(not relevant)
 *  ;
 *  --------------------------------------------------------------
 *  where VCS is VALID canonical syntax (VCS):
 *  	assertion'name'<subscript>
 *
 *  Example:
 *  --------------------------------------------------------------
 *  NET_NAME
 *  'Q2'
 *   'Q'<2>: ;
 *  NODE_NAME   U2 3
 *   '(CT1 LS042.3P)':
 *   'K'<0>:
 *  ;
 *  --------------------------------------------------------------
 */

#define N_XNET_KEYWORDS	2

char *net_words[N_XNET_KEYWORDS] = {

#define   NODE_NAME	0
	 "NODE_NAME",

#define   NET_NAME	1
	 "NET_NAME",
};

void
xnet_parse(f,w)
register FILE *f;
register WIRELIST *w;
{
	PIN *add_pin();
	NET *add_net();
	int fseek_npat();
	long ftell_pat();
	long off;
	char netname[STR_BUFSIZE], refdes[STR_BUFSIZE];
	unsigned int pin, key;

	netname[0] = EOS;

	/*
	 *  Set file pointer at first NET_NAME so that the
	 *  first iteration of the while loop will yield
	 *  a switch on NET_NAME.
	 */

	if((off = ftell_pat(f,net_words[NET_NAME],0)) == FALSE) {
		fprintf(Errfp,"No NET_NAME entry in \"%s\"\n",Xnetfile); 
		w->w_error |= EXNETSYN;
		return;
	}
	fseek(f,off,0);

	while((key = fseek_npat(f,net_words,N_XNET_KEYWORDS)) != -1) {
		switch(key) {
		  case NET_NAME:

			if(fscanf(f," '%*[^']' %[^:]: ;",netname) != 1) {
			   fprintf(Errfp,
			   "Bad NET_NAME entry in \"%s\", last net: %s\n",
			    Xnetfile,netname);
			   w->w_error |= EXNETSYN;
			}
			break;

		  case NODE_NAME:
			if(fscanf(f," %s %u",refdes,&pin) != 2) {
			   fprintf(Errfp,
			   "Bad NODE_NAME entry in \"%s\", last net: %s\n",
			   Xnetfile,netname);
				w->w_error |= EXNETSYN;
			}
			else {
				/*
				 *  Wrap pin  `pin' of device `refdes'
				 *  onto the signal `netname'
				 */

				if(add_pin(netname,pin,refdes,w) == NULL) {
					fprintf(Errfp,
					"Ignoring \"%s\" net: %s (%s)-%d\n",
					Xnetfile,netname,refdes,pin);
					w->w_error |= EXNETSYN;
				}
			}
			break;
		}
	}
}
