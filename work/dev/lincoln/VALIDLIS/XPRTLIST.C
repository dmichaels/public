/***********************************************************************
* PROGRAM:	VALID post-proccessor expanded parts list file lister
* FILE:		xprtlist.c
* DATE:		June 1984
* AUTHOR:	David Michaels (david@ll-sst)
***********************************************************************/

#include "cdef.h"
#include <stdio.h>

/*
 *  xprt_parse
 *
 *  Read the the VALID expanded parts list and print to
 *  standard output, a formatted list of pertinent data.
 *
 *  VALID Expanded Parts List (partial) format:
 *  ----------------------------------------------------------
 *  PART_NAME
 *  <reference_designator>
 *  <partname>:
 *  <logical_part_properties>
 *  ----------------------------------------------------------
 *  (Newline is optional between refdes and partname)
 *  ----------------------------------------------------------
 *  ex:	PART_NAME
 *  	U32   'LS04':
 *  	VALUE = '??';
 *  ----------------------------------------------------------
 *  Here we would want the partname (i.e. LS04) and reference
 *  designator (i.e. U32).  Return the number of designators.
 */

main(ac,av)
int	ac;
char	*av[];
{
	void_t xprt_parse();
	char *file;
	FILE *f, *fopen();

	if(ac == 1)
		file = "pstxprt.dat";
	else if(ac == 2)
		file = av[1];
	else {
		fprintf(stderr,"usage: %s VALID_expanded_parts_list\n",*av);
		exit(1);
	}
	if((f = fopen(file,"r")) == NULL){
		fprintf(stderr,"%s: can't open \"%s\"\n",*av,file);
		exit(1);
	}
	printf("\nVALID Expanded Parts List \"%s\"\n\n",file);
	xprt_parse(f);
}


#define BUFSIZE	128
#define NKEYWORD	2

char *keyword[NKEYWORD] = {

#define   PART_NAME	0
	 "PART_NAME",

#define   VALUE	1
	 "VALUE",
};

xprt_parse(f)
register FILE *f;
{
	long ftell_pat();
	int fseek_npat();
	int rmchar();
	register bool_t once_thru = FALSE;
	long off;
	register int key;
	char refdes[BUFSIZE], partname[BUFSIZE], value[BUFSIZE];

	*value = EOS;

	/*
	 *  Set file pointer at first PART_NAME so that
	 *  the first iteration of the while loop will
	 *  yield a switch on PART_NAME.
	 */

	if((off = ftell_pat(f,keyword[PART_NAME],0)) == FALSE) {
		fprintf(stderr,"** No PART_NAME entry\n"); 
		return;
	}
	fseek(f,off,0);

	printf("%-15s%-15s%s\n","Designator","Part Name","Value");
	printf("%-15s%-15s%s\n","----------","---------","-----");

	while((key = fseek_npat(f,keyword,NKEYWORD,1)) != -1) {
	   switch(key) {
	   case PART_NAME:
		if(once_thru)
			printf("%-15s%-15s%s\n",
			refdes,partname,*value == EOS ? "---" : value);
		*value = EOS;
		once_thru = TRUE;
		if(fscanf(f,"%s%[^:]",refdes,partname) != 2)
			break;
		rmchar(partname,"'",0);
		break;
	   case VALUE:
		if(fscanf(f,"%*[^']'%[^']",value) != 1)
			*value = EOS;
		break;
	   }
	}
	printf("%-15s%-15s%s\n",
	refdes,partname,*value == EOS ? "---" : value);
}
