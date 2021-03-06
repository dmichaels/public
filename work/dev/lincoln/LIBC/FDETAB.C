#include "cdef.h"
#include <stdio.h>

/*
 *  fdetab
 *
 *  Copy file `f1' to file `f2' while translating
 *  tabs in `f1' to an appropriate number of blanks.
 *
 *  Tab stop positions are currently every 8 columns
 *  stating with column number one (one indexed).
 *
 *  The file `f1' must be opened for reading, and the
 *  file `f2' must be opened for writing (via fopen(3S)).
 *  The two files may not be the same.
 */

void
fdetab(f1,f2)
register FILE *f1, *f2;
{
	void settab();
	bool is_tabstop();
	register unsigned int col = 1;
	static bool tab_set = FALSE;
	register char c;

	if(f1 == f2 || f1 == NULL || f2 == NULL)
		return;

	if(!tab_set){
		settab(0,NULL);
		tab_set = TRUE;
	}

	while((c = getc(f1)) != EOF){
		if(c == TAB)
			do{
				fputc(SPACE,f2);
				col++;
			} while(!is_tabstop(col));
		else{
			fputc(c,f2);
			if(c == NEWLINE)	
				col = 1;
			else if(c != BACKSPACE)	
				col++;
		}
	}
}
