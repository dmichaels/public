/*
 *  NAME
 *	sizedip - make a parts library file
 *
 *  SYNOPSIS
 *	sizedip complete_path/SCALDdirectory_filename
 *		the output file will be called sizedip.dat, which
 *		you then want to rename and merge into part_map.
 *
 *	[sizedip part_library_file_name...]
 *
 *  DESCRIPTION
 *	Creates a file for the VALID program "layout" (David Michaels)
 *	that contains the package-size of each dip in every library.
 *	For example, one obvious entry would be:  "74LS00  DIP14"
 *	The path specified must go all the way to the SCALD directory
 *	containing the library.prt text file, for example:
 *
 *			sizedip /u0/lib/memory/memory.prt
 *
 *	and the output file is written (for now) to the file "sizedip.dat"
 *	in the current directory.  It should then be manually renamed to
 *	something distinctive (e.g. sizedip.memory) so that all such files
 *	can be merged into a file called "part_map" (used by "layout" and
 *	by "wirelist") in ~lincoln.
 *
 *  NOTES
 *	Compile with David Michaels' string manipulation routines.
 *	That is:   "cc sizedip.c lib.a"
 *
 *  HISTORY
 *	- Don Malpass (group 24) May 1984.
 *	  Original version written.
 *	- David Michaels (group 24) December 1984.
 *	  Rewritten and revised.
 */

#include <stdio.h>

#define MAXLINE	256	/* max length of input line */
#define ICLEN	20	/* max length of IC name */

int	 maxpin;
char	*p;
int	 pin;

main(ac,av)
int ac;
char *av[];
{
	FILE *infp;
#if 1
	FILE *outfp;
#endif
	char *sdelim();
	char *sindex();
	char *index();
	char *cindex();

	char linebuf[MAXLINE+1];
	char ictype[20];
	char *prefix;

	if (ac != 2) {
		fprintf(stderr,
		"usage: sizedip complete_path/SCALDdirectory_filename\n") ;
		fprintf(stderr,
		"   e.g.,     sizedip ~lincoln/misc/misc.prt\n") ;
		fprintf(stderr,
		" Output file will be sizedip.dat, which you must\n") ;
		fprintf(stderr,
		"   merge into ~lincoln/part_map.\n") ;
		exit(1);
	}

	if ((infp = fopen(av[1], "r")) == NULL) {
		fprintf(stderr,"can't: %s\n",av[1]);
		exit(1);
	}

#if 1
	if ((outfp = fopen("sizedip.dat","w")) == NULL) {
		fprintf(stderr,"can't open sizedip.dat\n");
		exit(1);
	}

	/* put in a test of file-header line. */

#endif
	/*
	 *  GET ON WITH IT!
	 */

	while (fgets(linebuf,MAXLINE,infp)) {
		if ((strncmp(linebuf,"primitive",strlen("primitive"))) == 0) {
			/*
			 *  Extract IC type.
			 */
			if ((p = sdelim(linebuf,"\'","\'",ictype)) != NULL) {
				if (sindex(ictype,"ECON",0) == NULL)
					/*
					 *  If connector, don't print.
					 */
					fprintf( outfp, "%s    \t", ictype) ;
					/*
					 *  Should skip everything else.
					 */
				if ((sindex(ictype,"SIP",1)) != NULL)
					prefix = "SIP" ;
				else if ((str_eq((ictype + (strlen(ictype))-3),"_PC")))
					prefix = "BABY_" ;
				else
					prefix = "DIP" ;

				pin = 0 ;
				maxpin = 1 ;
			}
			else {
			    fprintf(stderr,"Format error in PRIMITIVE line.\n");
			    exit (1);
			}
		}
		else if ((p = sindex(linebuf,"PIN_NUMBER",1)) != NULL) {
			/*
			 *  Left delimiters are comma & left paren.
			 */
			while ((p = cindex (p,"\(\,")) != NULL)
				newmax();  /* increments p */
		}
		else if ((p = sindex(linebuf,"POWER_PINS",1)) != NULL) {
			/*
			 *  Left delimiters are comma & colon.
			 */
			while ((p = cindex (p,":\,")) != NULL)
				newmax();  /* increments p */
		}
		else if ((strncmp(linebuf,"end_primitive",13)) == 0) {
			if (sindex(ictype,"ECON",0) == NULL) {
				/*
				 *  Don't print anything for connectors.
				 */
				if (!str_eq(prefix, "BABY_"))
					fprintf(outfp,"%s%d\n",prefix,maxpin);
				else
					fprintf(outfp,"BABY_%s\n",ictype);
			}
		}
	}

#if 1
	fclose(outfp);
#endif
	printf("\nDone.\nsizedip.dat created for output.\n");
}

/*
 *  newmax
 *
 *  This is NOT a general function.  It is specific to
 *  this program, and it forces p, pin, and maxpin
 *  to be global.
 *  Enter with p pointing to delimiter to left of a pin number.
 *  Leaves p pointing to last digit of the pin number.
 *  If the pin is larger than maxpin, update maxpin.
 */

int
newmax()
{
	sscanf(++p, "%d", &pin);
	if (pin > maxpin)
		maxpin = pin ;
}
