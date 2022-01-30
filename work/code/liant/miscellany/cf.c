/***********************************************************************
* PROGRAM:	Character Frequency Program
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		July 1984
***********************************************************************/

/*
* NAME
*	cf - character frequency program
*
* SYNOPSIS
*	cf [-t] [file]...
*
* DESCRIPTION
*	Prints to the standard output a listing of the 
*	frequency of occurrence of each ASCII character
*	in the given file(s).  If multiple files are
*	given then a total count will be given along
*	with the counts for each file.  If the -t flag
*	is given then only a total count will be given.
*	If no file arguments are given, then the standard
*	input will be read.
*
* SEE ALSO
*	wc(1), wf(1L), ascii(7)
*/

/***********************************************************************
*		INCLUDE FILES
***********************************************************************/

#include <stdio.h>

/***********************************************************************
*		DEFINITIONS
***********************************************************************/

#define CMASK	0177	/* mask off 8th (parity) bit */
#define NASCII	128	/* number of ASCII characters */

#define FALSE	(0)
#define TRUE	(!FALSE)

typedef int	bool_t;
typedef int	void_t;

/***********************************************************************
*		GLOBAL DATA
***********************************************************************/

static char *ascii_chars[NASCII] =
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

/***********************************************************************
*		FUNCTIONS
***********************************************************************/

main(ac,av)
int	ac;
char	*av[];
{
	void_t			totals();
	static char		*progname = "cf";
	FILE			*fopen();
	FILE			*f;
	register int		c;
	register long int	total, alltotal = 0;
	register long int	char_freq[NASCII];
	register long int	total_freq[NASCII];
	register unsigned int	i;
	unsigned int		fi, filesread = 0;
	bool_t			tflag;

	if(av[1][0] == '-' && av[1][1] == 't')
		tflag = TRUE;
	else
		tflag = FALSE;
	av += (tflag ? 2 : 1);
	ac -= (tflag ? 2 : 1);
	if(ac == 0){
		f = stdin;
		tflag = TRUE;
	}

	/*
	*  Initialize the
	*  total character
	*  frequency table.
	*/
	for(i = 0 ; i < NASCII ; i++)
		total_freq[i] = 0;

	for(fi = 0 ; fi < ac || f == stdin ; fi++){

		if(f != stdin && (f = fopen(av[fi],"r")) == NULL){
			fprintf(stderr,"%s: can't open %s\n",progname,av[fi]);
			continue;
		}
		filesread++;
		total = 0;

		/*
		*  Re-initialize the
		*  per-file character
		*  frequency table.
		*/
		for(i = 0 ; i < NASCII ; i++)
			char_freq[i] = 0;

		if(!tflag){
			printf("\nfile \"%s\"",av[fi]);
			fflush(stdout);
		}

		while((c = getc(f)) != EOF){
			c &= CMASK;
			char_freq[c]++;
			total_freq[c]++;
			total++;
		}

		if(!tflag){
			printf("  (%D character",total);
			if(total != 1)
				putchar('s');
			printf(")\n\n");
			totals(char_freq);
		}

		alltotal += total;
		fclose(f);
		f = NULL;
	}

	if(tflag || filesread > 1){
		printf("\ntotals  (%D character",alltotal);
		if(total != 1)
			putchar('s');
		printf(")\n\n");
		totals(total_freq);
	}
}

void_t
totals(freq_tbl)
register unsigned int *freq_tbl[];
{
	register unsigned int i, col;

	for(i = 0, col = 0 ; i < NASCII ; i++, col++){
		if(col == 4){
			putchar('\n');
			col = 0;
		}
		printf("%8d  %-5s", freq_tbl[i],ascii_chars[i]);
	}
	putchar('\n');
}
