/******************************************************************************
*	String/Pattern Matching Routines
******************************************************************************/

/*
*	FUNCTION:	sindex
*
*	Return pointer to first occurrence of pattern string `pat`
*	in string `str`.  Return NULL if not found.
*
*	If flag = 0:	Returned pointer points to the
*			the first character of the string.
*	If flag != 0:	Returned pointer points to the
*			the last character of the string.
*/


/******************************************************************************
*	Brute-Force Algorithm
******************************************************************************/

sindex(str,pat,flag)
register char *str, *pat;
int flag;
{
	register char *p = pat;
	register char *s = str;

	do {
		if(*s == *p){
			s++;
			p++;
		}
		else{
			s = ++str;
			p = pat;
		}
	} while(*p != EOS && *s != EOS);
	return(*p == EOS ? (flag == 0 ? str : (str+(p-pat)-1)) : NULL);
}


/******************************************************************************
*	Knuth/Morris/Pratt Algorithm
*
*	This algorithm is particularly useful (efficient) when patterns are
*	highly self-repetitive (e.g. 10101110), and when backing up the the
*	string pointer is very undesirable (e.g. when the input is being
*	sequentially read from a file) since we never need to backup the
*	string pointer.  Otherwise, this algorithm will do (on the average)
*	about as well as the brute-force alogorithm.
******************************************************************************/

char *
kmp_sindex(str,pat,flag)
register char	*str;
char		*pat;
int		flag;
{
	void_t		kmp_init();
	int		next[STR_BUFSIZE];
	int		pat_len = strlen(pat);
	register int	i = 0, j = 0;

	kmp_init(pat,pat_len,next);

	do {
		if(j == 0 || str[i] == pat[j]){
			i++;
			j++;
		}
		else{
		    j = next[j];
		}
	}  while(pat[j] != EOS && str[i] != EOS);

	return(pat[j] == EOS ? (flag == 0 ? &str[i-pat_len] : &str[i-1]):NULL);
}

/*
*	FUNCTION:	kmp_init
*
*	Creates the "next table" needed for the Knuth/Morris/Pratt pattern
*	matching algorithm.  (see "Algorithms" - R. Sedgwick, page 244)
*	This table tells us how far to back up in the pattern string when a
*	mismatch is found.
*/

static void_t
kmp_init(pat,patlen,next)
char *pat;
register patlen;
int next[];
{
	register int i = 0, j = -1;
	next[0] = -1;

	do {
		if(j == -1 || pat[i] == pat[j]){
			i++;
			j++;
			next[i] = j;
		}
		else{
			j = next[j];
		}
	} while (pat[i] != EOS);
	next[0] = 0;
}
