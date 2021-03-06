/***********************************************************************
* PROGRAM:	VALID post-proccessor expanded net list file lister
* FILE:		xnetlist.c
* DATE:		June 1984
* AUTHOR:	David Michaels (david@ll-sst)
***********************************************************************/

#include "cdef.h"
#include <stdio.h>

/*
 *  xnet_parse
 *
 *  Parse the VALID expanded net list file.
 *  Print to the standard output each net encountered.
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

main(ac,av)
int	ac;
char	*av[];
{
	void_t xnet_parse();
	char *file;
	FILE *f, *fopen();

	if(ac == 1)
		file = "pstxnet.dat";
	else if(ac == 2)
		file = av[1];
	else {
		fprintf(stderr,"usage: %s VALID_expanded_net_list\n");
		exit(1);
	}
	if((f = fopen(file,"r")) == NULL) {
		fprintf(stderr,"%s: can't open \"%s\"\n",*av,file);
		exit(1);
	}
	printf("\nVALID Expanded Net List \"%s\"\n\n",file);
	xnet_parse(f);
}


#define BUFSIZE		128
#define NKEYWORD	2

char *net_words[NKEYWORD] = {

#define   NODE_NAME	0
	 "NODE_NAME",

#define   NET_NAME	1
	 "NET_NAME",
};

void_t
xnet_parse(f)
register FILE *f;
{
	int		fseek_npat();
	long		ftell_pat();
	long		off;
	char		netname[BUFSIZE], refdes[BUFSIZE];
	unsigned int	pin, key;

	*netname = EOS;

	/*
	 *  Set file pointer at first NET_NAME so that the
	 *  first iteration of the while loop will yield
	 *  a switch on NET_NAME.
	 */

	if((off = ftell_pat(f,net_words[NET_NAME],0)) == FALSE) {
		fprintf(stderr,"** No NET_NAME entry\n"); 
		return;
	}
	fseek(f,off,0);

	printf("%-30s%-12s%s\n","Net Name","Designator","Pin Number");
	printf("%-30s%-12s%s\n","--------","---------","----------");

	while((key = fseek_npat(f,net_words,NKEYWORD)) != -1) {
	   switch(key) {
	   case NET_NAME:

		if(fscanf(f," '%*[^']' %[^:]: ;",netname) != 1)
			fprintf(stderr,
			"** Bad NET_NAME entry (last net: %s)\n",netname);
		break;
	   case NODE_NAME:
		if(fscanf(f," %s %u",refdes,&pin) != 2)
			fprintf(stderr,
			"** Bad NODE_NAME entry (last net: %s)\n",netname);
		else
			printf("%-30s%-12s%u\n",netname,refdes,pin);
		break;
	   }
	}
}
