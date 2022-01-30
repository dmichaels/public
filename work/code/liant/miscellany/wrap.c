/*
 *  NAME
 *	wrap - print a file with long line wrap-around
 *
 *  SYNOPSIS
 *	wrap [-l length] [-i indent] [file...]
 *
 *  DESCRIPTION
 *	The lines in the specified file(s), (or the standard input if
 *	no files are given) are copied to the standard output with
 *	long lines wrapped around. The "length" argument following the
 *	"-l" flag gives the maximum total line length.  The "indent"
 *	argument following the "-i" flag specifies the left margin
 *	for wrapped lines.
 *
 *	The wrap part of a wrapped around line is right justified
 *	if it is less than the length given by ("length" - "indent").
 *
 *	The default values for "length" and "indent" are eighty (80)
 *	and zero (0) respectively.
 *
 *  NOTES
 *	Tabs are not expanded to spaces, therefore lines with
 *	tabs appear shorter to the program than they actually
 *	appear on the screen. In order to handle this properly
 *	the "expand" program may be used for example as follows:
 *
 *		   expand file | wrap -l 50 -i 6
 *  SEE ALSO
 *	expand(1)
 *
 *  AUTHOR
 *	David Michaels (david@ll-sst) October 1984
 */

#include <stdio.h>

#define DASH		'-'
#define	FALSE		((bool_t)0)
#define	EOS		'\0'
#define FORMFEED	'\f'
#define MAXLINE		512
#define NEWLINE		'\n'
#define SPACE		' '
#define	TRUE		((bool_t)(!FALSE))

typedef	int		bool_t;
typedef	int		void_t;

char	*Progname;

main(ac,av)
int	ac;
char	*av[];
{
	FILE		*freopen();
	int		atoi(), strcmp(), putnchar();
	char		*gets();
	void_t		putspace(), usage();

	register int 	charsleft, maxlen = 80, indent = 0, nput;
	register char 	*lineptr, line[MAXLINE+1];
	bool_t		wrap;

	Progname = *av;

	for(ac--, av++ ; ac > 0 && (*av)[0] == DASH ; ac--, av++) {
		switch((*av)[1]) {
		 case 'l':
			ac--;
			av++;
			if((maxlen = atoi(*av)) <= 0) {
				fprintf(stderr,
				"%s: length (%d) must be positive\n",
				Progname,*av);
				exit(2);
			}
			break;
		 case 'i':
			ac--;
			av++;
			if((indent = atoi(*av)) < 0) {
				fprintf(stderr,
				"%s: indent (%d) must greater than 0\n",
				Progname,*av);
				exit(3);
			}
			break;
		}
	}
	if(indent >= maxlen) {
		fprintf(stderr,
		"%s: indent (%d) must be less than length (%d)\n",
		Progname,indent,maxlen);
		exit(4);
	}

	do {
		if(ac > 0 && freopen(*av++, "r", stdin) == NULL) {
			fprintf(stderr,"%s: can't open %s\n",Progname,*--av);
			exit(5);
		}
		while(TRUE) {
			lineptr = line;
			*lineptr = EOS;
			if(gets(lineptr) == NULL)
				break;
			if(*lineptr == FORMFEED)
				putchar(*lineptr++);
			wrap = FALSE;
			if((charsleft = strlen(lineptr)) > maxlen) {
				while(TRUE) {
				   if(wrap)
				   	nput = putnchar(lineptr,maxlen-indent);
				   else
				   	nput = putnchar(lineptr,maxlen);
				   lineptr += nput;
				   charsleft -= nput;
  				   if(charsleft == 0)
  					break;
  				   putchar(NEWLINE);
				   putspace(indent);
				   wrap = TRUE;

				   /*  Last wrap-around line */

				   if(charsleft <= maxlen-indent) {
					putspace(maxlen-charsleft-indent);
					putnchar(lineptr,charsleft);
					putchar(NEWLINE);
					break;
				   }
				}
				continue;
			}
  			if(*lineptr == EOS)
  				putchar(NEWLINE);
			else
				printf("%s\n",lineptr);
  		}
  	} while(--ac > 0);
}

/*
 *  putspace
 *
 *  Print `n' spaces to the standard output.
 */

void_t
putspace(n)
register unsigned int n;
{
	register unsigned int i;

	for(i = 0 ; i < n ; i++)
		putchar(SPACE);
}

/*
 *  putnchar
 *
 *  Print `n' characters to the standard output
 *  from the string `s' or up til the end of
 *  string (whichever comes first).  Return
 *  the number of characters actually printed.
 */

int
putnchar(s,n)
register char *s;
register unsigned int n;
{
	register unsigned int i;

	for(i = 0 ; i < n && *s != EOS ; i++)
		putchar(*s++);
	return(i);
}

void_t
usage()
{
	fprintf(stderr,
	"usage: %s [-l length] [-i indent] [file]...\n",Progname);
	exit(1);
}
