static char *sccsid = "@(#)num.c	4.1 (Berkeley) 10/1/80";
#include	"stdio.h"
#define		formfeed 0xc

/*
 * number - a cat like program which prints a file with line
 *	    numbers. Printing of numbers is suppressed on blank
 *	    lines.
 *
 * Original Version by William Joy, June 1977
 * Updated October 1979 by M. Kirk McKusick
 *
 * CHANGES
 *
 * 09.07.89  DGM  Modified to limit the left margin space to 5 spaces;
 *		  hooked on SMALL_LEFT_MARGIN. (Language Processors, Inc.)
 *
 * 10.xx.84  DGM  Modified to number blank lines too; hooked on
 *		  NUMBER_BLANK_LINES. David Michaels (david@ll-sst).
 *
 */

#define NUMBER_BLANK_LINES	1	/* DGM (10/84) */
#define SMALL_LEFT_MARGIN	1	/* DGM (8/89) */

main(ac, av)
	int ac;
	char *av[];
	{
	register int argc = ac;
	register char **argv = av;
	register int lino;
	register char *lineptr;
	char line[512];

	argv++;
	argc--;
	lino = 1;

	do {
		if (argc)
			if (freopen(*argv++, "r", stdin) == NULL)
				{
				perror(*--argv);
				exit(1);
				}
		for(;;)
			{
			lineptr = line;
			*lineptr = 0;
			fscanf(stdin,"%[^\n]",lineptr);
			if (feof(stdin))
				break;
			if (*lineptr == formfeed)
				putc(*lineptr++,stdout);
#ifndef NUMBER_BLANK_LINES
			if (*lineptr == '\0')
				putc('\n',stdout);
			else
#endif
#ifdef SMALL_LEFT_MARGIN
				fprintf(stdout,"%4d %s\n",lino,lineptr);
#else
				fprintf(stdout,"%6d  %s\n",lino,lineptr);
#endif
			lino++;
			getc(stdin);
			}
		}
		while (--argc > 0);
	exit();
	}
