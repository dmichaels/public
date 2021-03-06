/***********************************************************************
* PROGRAM:	VALID ged logic file lister
* FILE:		gedlist.c
* DATE:		June 1984
* AUTHOR:	David Michaels (david@ll-sst)
***********************************************************************/

#include "cdef.h"
#include <stdio.h>

/*
 *  ged_parse
 *
 *  Go through the graphics file, and print to the standard output,
 *  a table of reference designators, device names, package names,
 *  and locations.  The pertinent lines of the file are:
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
 *  The board location is above REF_PIN property line, and
 *  the offset to pin 1 of board is after the REF_PIN property.
 *  So the location of pin 1 of the board is given by:
 *  <board_location> + <offset_to_pin_one>
 */

main(ac,av)
int	ac;
char	*av[];
{
	void_t	ged_parse();
	char *file;
	FILE	*f, *fopen();

	if(ac == 1)
		file = "logic.1.1";
	else if(ac == 2)
		file = av[1];
	else {
		fprintf(stderr,"usage: %s VALID_graphics_editor_file\n",*av);
		exit(1);
	}
	if((f = fopen(file,"r")) == NULL) {
		fprintf(stderr,"%s: can't open \"%s\"\n",*av,file);
		exit(1);
	}
	printf("\nVALID ged file \"%s\"\n\n",file);
	ged_parse(f);
}


#define BUFSIZE		80
#define NKEYWORD	2

char *keyword[NKEYWORD] = {

#define   FORCEPROP	0
	 "FORCEPROP",

#define   FORCEADD	1
	 "FORCEADD",
};

void_t
ged_parse(f)
register FILE *f;
{

	int		key, fseek_npat();
	bool_t		got_refdes = FALSE;
	bool_t		got_dev = FALSE;
	bool_t		got_pkg = FALSE;
	bool_t		got_loc = FALSE;
	char		boardname[BUFSIZE+1];
	char		propname[BUFSIZE], prop[BUFSIZE];
	char		refdes[BUFSIZE], pkgname[BUFSIZE], devname[BUFSIZE];
	int		xloc, yloc;
	int		bxorigin, byorigin;
	int		bxoffset, byoffset;

	*refdes = EOS;
	*boardname = EOS;

	printf("%-12s%-12s%-12s%-12s\n"
	,"Designator","Device","Package","Location");
	printf("%-12s%-12s%-12s%-12s\n"
	,"----------","------","-------","--------");

	while((key = fseek_npat(f,keyword,NKEYWORD)) != -1) {
	   switch(key){
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
			  fprintf(stderr,
			  "** Bad CHIP (last refdes: %s)\n",refdes);
			  break;
			}
			strcpy(devname,prop);
			got_dev = TRUE;
		}
		else if(str_eq(propname,"REFDES")) {
			if(fscanf(f," %s",prop) != 1) {
			  fprintf(stderr,
			  "** Bad REFDES (last refdes: %s)\n",refdes);
			  break;
			}
			strcpy(refdes,prop);
			got_refdes = TRUE;
		}
		else if(str_eq(propname,"REF_PIN")) {
			bxorigin = xloc;
			byorigin = yloc;
			if(fscanf(f,
			  "%d%*[^0123456789]%d",&bxoffset,&byoffset) != 2) {
			  fprintf(stderr,
			  "** Bad REF_PIN (board %s)\n", pkgname);
			  break;
			}
			strcpy(boardname,pkgname);
		}
		break;
	   /*
	    *  CASE: FORCEADD package_name..1
	    *	  location
	    */
	   case FORCEADD:
		if(got_refdes && got_loc && got_pkg && got_dev) {
			printf("%-12s",refdes);
			printf("%-12s",devname);
			printf("%-12s",pkgname);
			printf("(%d %d)\n",xloc,yloc);
			got_refdes = FALSE;
			got_loc = FALSE;
			got_pkg = FALSE;
			got_dev = FALSE;
		}
		if(fscanf(f," %[^.]%*[^\n]\n",pkgname) != 1) {
			fprintf(stderr,
			"** Bad FORCEADD (last refdes: %s)\n", refdes);
			break;
		}
		got_pkg = TRUE;
		if(fscanf(f," (%d%d ) ;",&xloc,&yloc) != 2) {
			fprintf(stderr,
			"** Bad FORCEADD (last refdes: %s)\n",
			refdes,pkgname);
			break;
		}
		got_loc = TRUE;
		break;
	   }
	}

	if(got_refdes && got_loc && got_pkg && got_dev) {
		/*
		 *  Last FORCEADD
		 */
		printf("%-12s",refdes);
		printf("%-12s",devname);
		printf("%-12s",pkgname);
		printf("(%d %d)\n",xloc,yloc);
	}
	printf("-----------------------------------------------------\n");
	printf("%-20s   %s\n","Board name:",boardname);
	printf("%-20s   (%d %d)\n","Board location:",bxorigin,byorigin);
	printf("%-20s   (%d %d)\n","Offset to pin 1:",bxoffset,byoffset);
	printf("%-20s   (%d %d)\n","Pin 1 location:",
			bxorigin+bxoffset,byorigin+byoffset);
	printf("-----------------------------------------------------\n");
}
