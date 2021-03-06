#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

/*
 *  fseek_npat
 *
 *  Set the IO pointer of file `f' to the position of the next
 *  occurrence of any of the `npat' pattern strings in the string
 *  array `pat', and return an index to pattern in the pat.  If
 *  no patterns are found in the file, return NOTFOUND (-1).
 *  The IO pointer will be set to the last character in the pattern
 *  string.  That is, the next getc(f) will yield the character
 *  immediately following the the chosen pattern string.
 */

#define FSEEK_BEGIN	0
#define NOTFOUND	(-1)

int
fseek_npat(f,pat,npat)
FILE *f;
char *pat[];
unsigned int npat;
{
	int		fstat();
	long		ftell_pat();
	int		p = NOTFOUND;
	unsigned int	i;
	struct stat	stat;
	register long	pos = 0;
	register long	curpos;
	register long	minoff;

	if(npat <= 0)
		return(NOTFOUND);

	/*  Set minoff sufficiently large */

	if(fstat(fileno(f),&stat) == -1)
		return(p);

	minoff = stat.st_size + 1;

	for(i = 0 ; i < npat ; i++){
		if((curpos = ftell_pat(f,pat[i],0)) == -1)
			continue;
		if(curpos < minoff){
			minoff = curpos;
			p = i;
		}
	}
	if(p != NOTFOUND) {
		fseek(f,minoff,FSEEK_BEGIN);
		for(i = strlen(pat[p]) ; i > 0 ; i--)
			getc(f);
	}
	return(p);
}
