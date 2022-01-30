/* cf - Character frequency program */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/************************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/***********************************************************************/

/***********************************************************************
/*
/*  SYNOPSIS
/*
/*	cf [-t] [file]...
/*
/*  DESCRIPTION
/*
/*	Prints to the standard output a listing of the frequency of
/*	occurrence of each ASCII character in the given file(s).  If
/*	multiple files are given then a total count will be given along
/*	with the counts for each file.  If the -t flag is given then
/*	only a total count will be given.  If no file arguments are
/*	given, then the standard input will be read.
/*
/***********************************************************************/

/* ----------------------------------------------------------------------
/* Include files
/* -------------------------------------------------------------------- */

#include <stdio.h>

/* ----------------------------------------------------------------------
/* Macro definitions
/* -------------------------------------------------------------------- */

#define PROGRAM_NAME	cf
#define __PROGRAM__	STR(PROGRAM_NAME)

#define NXSTR(s)	#s		/* Stringize (no argument expansion) */
#define STR(s)		NXSTR(s)	/* Stringize (argument expanded) */

#define CMASK	0177	/* mask off 8th (parity) bit */
#define NASCII	128	/* number of ASCII characters */

#define FALSE	(0)
#define TRUE	(!FALSE)

/* ----------------------------------------------------------------------
/* Type definitions
/* -------------------------------------------------------------------- */

typedef int	boolean;

/* ----------------------------------------------------------------------
/* Static data definitions
/* -------------------------------------------------------------------- */

static char * ascii_chars [NASCII] =
{
	 "nul" ,"soh" ,"stx" ,"etx" ,"eot" ,"enq" ,"ack" ,"bel"
	,"bs"  ,"ht"  ,"nl"  ,"vt"  ,"np"  ,"cr"  ,"so"  ,"si"
	,"dle" ,"dc1" ,"dc2" ,"dc3" ,"dc4" ,"nak" ,"syn" ,"etb"
	,"can" ,"em"  ,"sub" ,"esc" ,"fs"  ,"gs"  ,"rs"  ,"us"
	,"sp"  ,"!"   ,"\""  ,"#"   ,"$"   ,"%"   ,"&"   ,"'"
	,"("   ,")"   ,"*"   ,"+"   ,","   ,"-"   ,"."   ,"/"
	,"0"   ,"1"   ,"2"   ,"3"   ,"4"   ,"5"   ,"6"   ,"7"
	,"8"   ,"9"   ,":"   ,";"   ,"<"   ,"="   ,">"   ,"?"
	,"@"   ,"A"   ,"B"   ,"C"   ,"D"   ,"E"   ,"F"   ,"G"
	,"H"   ,"I"   ,"J"   ,"K"   ,"L"   ,"M"   ,"N"   ,"O"
	,"P"   ,"Q"   ,"R"   ,"S"   ,"T"   ,"U"   ,"V"   ,"W"
	,"X"   ,"Y"   ,"Z"   ,"["   ,"\\"  ,"]"   ,"^"   ,"_"
	,"`"   ,"a"   ,"b"   ,"c"   ,"d"   ,"e"   ,"f"   ,"g"
	,"h"   ,"i"   ,"j"   ,"k"   ,"l"   ,"m"   ,"n"   ,"o"
	,"p"   ,"q"   ,"r"   ,"s"   ,"t"   ,"u"   ,"v"   ,"w"
	,"x"   ,"y"   ,"z"   ,"{"   ,"|"   ,"}"   ,"~"   ,"del"
};

void	totals	(uint *[]);

/* ----------------------------------------------------------------------
/* main
/* -------------------------------------------------------------------- */

main (int argc, char *argv[])
{
	FILE		*fopen();
	FILE		*f;
	register int	c;
	register long	total, alltotal = 0;
	register long	character_frequency[NASCII];
	register long	total_frequency[NASCII];
	register uint	i;
	uint		fi, filesread = 0;
	boolean		tflag;

	if ((av[1][0] == '-') && (av[1][1] == 't'))
		tflag = TRUE;
	else	tflag = FALSE;

	av += (tflag ? 2 : 1);
	ac -= (tflag ? 2 : 1);

	if (ac == 0) {
		f = stdin;
		tflag = TRUE;
	}

	/* Initialize the total character frequency table */

	for (i = 0 ; i < NASCII ; i++)
		total_frequency[i] = 0;

	for (fi = 0 ; (fi < ac) || (f == stdin) ; fi++) {

		if ((f != stdin) && ((f = fopen(av[fi],"r")) == NULL)) {
			fprintf (stderr, "%s: can't open %s\n",
				 __PROGRAM__, av[fi]);
			continue;
		}
		filesread++;
		total = 0;

		/* Re-initialize the per-file character frequency table */

		for (i = 0 ; i < NASCII ; i++)
			character_frequency[i] = 0;

		if (!tflag){
			printf ("\nfile \"%s\"", av[fi]);
			fflush (stdout);
		}

		while ((c = getc(f)) != EOF) {
			c &= CMASK;
			character_frequency[c]++;
			total_frequency[c]++;
			total++;
		}

		if (!tflag) {
			printf ("  (%D character",total);
			if (total != 1) putchar ('s');
			printf (")\n\n");
			totals (character_frequency);
		}

		alltotal += total;
		fclose (f);
		f = NULL;
	}

	if (tflag || (filesread > 1)) {
		printf ("\ntotals  (%D character", alltotal);
		if (total != 1)
			putchar ('s');
		printf (")\n\n");
		totals (total_frequency);
	}

	exit (0);
}

/* ----------------------------------------------------------------------
/* totals
/* -------------------------------------------------------------------- */

void
totals (register uint *frequency_table[])
{
	register unsigned int i, col;

	for (i = 0, col = 0 ; i < NASCII ; i++, col++){
		if (col == 4) {
			putchar ('\n');
			col = 0;
		}
		printf ("%8d  %-5s", frequency_table[i], ascii_chars[i]);
	}
	putchar ('\n');
}
