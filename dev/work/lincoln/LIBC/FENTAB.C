#include "cdef.h"
#include <stdio.h>

/*
 *  fentab
 *
 *  Convert multiple in file `f1' to an appropriate
 *  number of tabs and blanks in file `f2'.
 *
 *  Tab stop positions are currently every 8 columns
 *  stating with column number one (one indexed).
 *
 *  The file `f1' must be opened for reading, and the
 *  file `f2' must be opened for writing (via fopen(3S)).
 *  The two files may not be the same.
 */

void
fentab(f1,f2)
register FILE *f1, *f2;
{
	void		  	settab();
	bool		  	is_tabstop();
	static	bool	  	tab_set = FALSE;
	register unsigned int 	col = 1, newcol;
	register char	  	c;

	if(f1 == NULL || f2 == NULL || f1 == f2)
		return;

	if(!tab_set){
		settab(0,NULL);
		tab_set = TRUE;
	}

	while(TRUE){
		newcol = col;
		while((c = getc(f1)) == SPACE){
			newcol++;
			if(is_tabstop(newcol)){
				putc(TAB,f2);
				col = newcol;
			}
		}

		/* Print left over blanks */

		for( ; col < newcol ; col++)
			putc(SPACE,f2);
		if(c == EOF)
			break;
		putc(c,f2);
		if(c == NEWLINE)	
			col = 1;
		else if(c != BACKSPACE)
			col++;
	}
}
