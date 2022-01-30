/***********************************************************************
* PROGRAM:	Concatenate Range of Lines in File
* AUTHOR:	David Michaels (david at ll-sst)
* DATE:		November 1984
***********************************************************************/

/*
*  NAME
*	catn - concatenate a range of lines
*	       from files to standard output.
*
*  SYNOPSIS
*	catn from_line to_line file...
*
*  DESCRIPTION
*	Concatenate to the standard output, lines
*	"from_line" thru "to_line" of the named files.
*
*  AUTHOR
*	David Michaels (david@ll-sst) November 1983
*/

/***********************************************************************
*		INCLUDE FILES
***********************************************************************/

#include <stdio.h>

/***********************************************************************
*		DEFINITIONS
***********************************************************************/

#define MAXLINE 512		/* max length of input line */

/***********************************************************************
*		GLOBAL VARIABLES
***********************************************************************/

char	*ProgName = "catn";	/* name of this program */

/***********************************************************************
*		FUNCTIONS
***********************************************************************/

main(ac,av)
int	ac;
char	*av[];
{
	FILE		*fopen();
	FILE		*f;
	char		line[MAXLINE];
	unsigned int	from, to, nlines;
	register int	m, l, i;

	if(ac < 4 ||
	   (nlines = (to = (atoi(av[2])) - (from = atoi(av[1])) + 1)) <= 0){
		usage();
	}
	from--;

	for(i = 3 ; i < ac ; i++){
		if((f = fopen(av[i],"r")) == NULL){
			fprintf(stderr,"%s: can't open %s \n",ProgName,av[i]);
			continue;
		}

		/*
		*  Skip past the
		*  first m lines.
		*/
		for(m = from ; m > 0 ; m-- ){
			if(fgets(line,MAXLINE,f) == NULL)
				goto Next;
		}

		/*
		*  Print nlines to
		*  the standard output.
		*/
		for(l = nlines ; l > 0 ; l--){
			if(fgets(line,MAXLINE,f) == NULL)
				goto Next;
			printf("%s",line);
		}
	  Next:
		fclose(f);
	}
	exit(0);
}

usage()
{
	fprintf(stderr,"usage: %s from_line to_line file\n",ProgName);
	fprintf(stderr,"       (from_line and to_line > 0)\n");
	fprintf(stderr,"       (from_line <= to_line)\n");
	exit(1);
}
