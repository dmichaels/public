/***********************************************************************
* PROGRAM:	Word Cross Reference Program
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		June 1984
***********************************************************************/

/*
 *  NAME
 *	wxref - word cross-reference program
 *
 *  SYNOPSIS
 *	wxref [-i] [-l] [file]...
 *
 *  DESCRIPTION
 *	Prints to the standard output, a line numbered listing of
 *	the given file(s) followed by a cross-reference listing
 *	of all of the words appearing in the file.
 *
 *	If the "-i" flag (ignore case) is given, then all
 *	upper case character will be mapped into lower case.
 *
 *	If the "-l" flag is given, then the line numbered 
 *	listing of the file will be suppressed.
 *
 *	If no file arguments are given, then the standard
 *	input will be read.  In this case the "-l" flag will
 *	be the default (i.e. the file will not be listed).
 *
 *  NOTES
 *	The flags apply to all file arguments.
 *
 *  SEE ALSO
 *	cxref(1L), wf(1L)
 */


/***********************************************************************
*			INCLUDE FILES
***********************************************************************/

#include <stdio.h>
#include <ctype.h>

/***********************************************************************
*			DEFINITIONS
***********************************************************************/

#define WORDLEN		24	/* maximum length of a word */
#define MAXLINE		512	/* maximum length of input line */

#define BACKSLASH	'\\'
#define DASH		'-'
#define DQUOTE		'"'
#define EOS		'\0'
#define NEWLINE		'\n'
#define SLASH		'/'
#define SQUOTE		'\''
#define STAR		'*'
#define UNDERSCORE	'_'

#define FALSE		(0)
#define TRUE		(!FALSE)

typedef int		bool_t;
typedef int		void_t;

/***********************************************************************
*			MACROS
***********************************************************************/

#define walloc() ((struct word_node *)malloc(sizeof(struct word_node)))
#define lalloc() ((struct line_node *)malloc(sizeof(struct line_node)))

#define word_start(c)	(isalpha(c) || (c) == UNDERSCORE)
#define word_char(c)	(isalnum(c) || (c) == UNDERSCORE || (c) == DASH)

/***********************************************************************
*			DATA STRUCTURES
***********************************************************************/

struct line_node {
	unsigned int	  l_lineno;	/* the line number */
	struct line_node *l_next;	/* ptr to next in list */
	struct line_node *l_prev;	/* ptr to previous in list */
};

struct word_node {
	char		  *w_word;	/* the word */
	struct line_node  *w_line;	/* list of line numbers */
	unsigned int	   w_cnt;	/* number of occurrences */
	struct word_node  *w_left;	/* ptr to left subtree */
	struct word_node  *w_right;	/* ptr to right subtree */
};

/***********************************************************************
*			GLOBAL VARIABLES
***********************************************************************/

char		*Progname;	/* name of this program (av[0]) */
long int	Uniqwords;	/* total no. of unique word */
unsigned int	Curline;	/* current line of the file */

/***********************************************************************
*			MAIN FUNCTIONS
***********************************************************************/

main(ac,av)
int	ac;
char	*av[];
{
	FILE			  *fopen();
	void_t			  fncopy(), pr_wxref(), free_tree();
	char			  *next_word(), *strlower();
	struct word_node	  *install();

	register struct word_node *wtree;
	char			  *word;
	register long int	  total;
	FILE			  *f;
	bool_t			  iflag = FALSE;
	bool_t			  lflag = FALSE;

	Progname = av[0];

	/*
	 *  Parse the command line arguments.
	 *  All flag options must come before
	 *  any file arguments.  A bit sloppy.
	 */

	for(av++, ac-- ; ac > 0 ; av++, ac--){
		if((*av)[0] == DASH){
			if((*av)[1] == 'i'){
				iflag = TRUE;
				continue;
			}
			else if((*av)[1] == 'l'){
				lflag = TRUE;
				continue;
			}
			break;
		}
		break;
	}
	f = (ac == 0 ? stdin : NULL);

	/* Loop through each file */

	for( ; ac > 0 || f == stdin ; av++, ac--){

		if(f != stdin && (f = fopen(*av,"r")) == NULL){
			fprintf(stderr,
			"\n%s: can't open %s\n\n",Progname,*av);
			continue;
		}

		wtree = NULL;
		total = 0;
		Uniqwords = 0;
		Curline = 1;

		if(iflag)
			while((word = next_word(f)) != NULL){
				total++;
				strlower(word);
				wtree = install(wtree,word,Curline);
			}
		else
			while((word = next_word(f)) != NULL){
				total++;
				wtree = install(wtree,word,Curline);
			}

		if(f != stdin){
			if(!lflag){
				rewind(f);
				putchar('\n');
				fncopy(f,stdout);
			}
			fclose(f);
		}

		/* Print the results */

		if(f != stdin)
			printf("\n\"%s\" ",*av);
		else
			putchar('\n');
		printf("Cross Reference (%D word",total);
		if(total > 1)
			putchar('s');
		printf(", %D unique word",Uniqwords);
		if(Uniqwords > 1)
			putchar('s');
		printf(")\n\n");
		printf("%5s  %-20s%s\n",
		       "Total","Word","Line Occurrence");
		printf("%5s  %-20s%s\n\n",
		       "-----","----","---------------");
		pr_wxref(stdout,wtree);
		putchar('\n');
		free_tree(wtree);
		f = NULL;
	}
	exit(0);
}

void_t
usage()
{
	fprintf(stderr,"usage: %s [-i] [-l] [file]...\n",Progname);
	exit(1);
}

void_t
fatal(s)
char *s;
{
	fprintf(stderr,"%s: %s...exit.\n",Progname,s);
	exit(1);
}

/***********************************************************************
*			UTILITY FUNCTIONS
***********************************************************************/

/*
 *  strsave
 *
 *  Return a copy of the given string,
 *  or NULL if can't allocate enough space.
 */

char *
strsave(s)
register char *s;
{
	char *malloc();
	register char *p = NULL, *q;

	if((p = malloc(strlen(s) + 1)) != NULL)
		for(q = p ; (*q++ = *s++) != EOS ; )
			;
	return(p);
}

/*
 *  strlower
 *
 *  Convert all upper case letters in the given
 *  string `s' to lower case.  Return the string.
 */

char *
strlower(s)
register char *s;
{
	register char *p;

	for(p = s ; *p != EOS ; p++)
		if(isupper(*p))
			*p = tolower(*p);
	return(s);
}

/***********************************************************************
*			PARSING FUNCTIONS
***********************************************************************/

/*
 *  next_word
 *
 *  Return (in static storage) the next word in the input.
 *  If none are left (i.e. at EOF), return NULL.
 */

char *
next_word(f)
register FILE *f;
{
	register int c;
	register int i = 0;
	static char buf[WORDLEN+1];

	*buf = EOS;
	
	if(feof(f))
		return(NULL);

	while((c = getc(f)) != EOF){
		if(word_start(c)){
			while(TRUE){
				buf[i++] = c;
				c = getc(f);
				if(word_char(c) && i <= WORDLEN)
					continue;
				if(c == NEWLINE)
					ungetc(c,f);
				break;
			}
			break;
		}
		else if(c == NEWLINE)
			Curline++;
	}
	buf[i] = EOS;
	return(*buf == EOS ? NULL : buf);
}
	
/***********************************************************************
*			TREE FUNCTIONS
***********************************************************************/

/*
 *  install
 *
 *  Install a the word `word' which occurred at line `line'
 *  into the binary search tree `t'.  Return the word's new
 *  (or found) node.  Return NULL if memory shortage.
 *  Increments global variable `Uniqwords' if this
 *  is a new word.  Recursive.
 */

struct word_node *
install(t,word,line)
register struct word_node *t;
register char *word;
register unsigned int line;
{
	char *strsave();
	struct word_node *install();
	struct line_node *line_add();
	register int cond;

	if(t == NULL){
		if((t = walloc()) == NULL)
			fatal("no space for word node (install)");
		if((t->w_word = strsave(word)) == NULL)
			fatal("no space for word (install)");
		t->w_line = NULL;
		line_add(t,line);
		t->w_cnt = 1;
		t->w_left = t->w_right = NULL;
		Uniqwords++;
	}
	else if((cond = strcmp(word,t->w_word)) == 0){
		line_add(t,line);
		t->w_cnt++;
	}
	else if(cond < 0)
		t->w_left = install(t->w_left,word,line);
	else
		t->w_right = install(t->w_right,word,line);
	return(t);
}

/*
 *  line_add
 *
 *  Make an entry (if one does not yet exist) on the line list
 *  of the given word node `wnode', for the given line
 *  number `line'.  Return the new (or existing) node.
 */

struct line_node *
line_add(wnode,line)
register struct word_node *wnode;
register unsigned int line;
{
	register struct line_node *new, *p, *last;
	register int cmp;

	/*
	 *  Null list; first time only.
	 */

	if(wnode->w_line == NULL){
		if((new = lalloc()) == NULL)
			fatal("no space for line node (line_add)");
		new->l_next = NULL;
		wnode->w_line = new;
		last = new;
		goto Out;
	}

	last = wnode->w_line->l_prev;
	wnode->w_line->l_prev = NULL;
	for(p = last ; p != NULL ; p = p->l_prev){
		/*
		 *  Found the node.
		 */
		if(line == p->l_lineno){
			wnode->w_line->l_prev = last;
			return(p);
		}
		/*
		 *  Put the node after p.
		 */
		if(line > p->l_lineno){
			if((new = lalloc()) == NULL)
				return(NULL);
			new->l_next = p->l_next;
			new->l_prev = p;
			/*
			 *  New mid-list node.
			 */
			if(p != last)
				p->l_next->l_prev = new;
			/*
			 *  New last node.
			 */
			else
				last = new;
			p->l_next = new;
			goto Out;
		}
	}
	/*
	 *  New first node.
	 */
	if(p == NULL){
		if((new = lalloc()) == NULL)
			return(NULL);
		new->l_next = wnode->w_line;
		wnode->w_line->l_prev = new;
		wnode->w_line = new;
		goto Out;
	}
Out:
	new->l_lineno = line;
	wnode->w_line->l_prev = last;
	return(new);
}

/*
 *  free_tree
 *
 *  Frees up the tree pointed to by `p'.  Recursive.
 */

void_t
free_tree(p)
register struct word_node *p;
{
	int free();
	register struct line_node *a, *b;

	if(p != NULL){
		free_tree(p->w_left);
		free(p->w_word);
		for (a = p->w_line ; a != NULL ; ) {
			b = a;
			a = a->l_next;
			free (b);
		}
		free(p);
		free_tree(p->w_right);
	}
}

/***********************************************************************
*			OUTPUT FUNCTIONS
***********************************************************************/

/*
 *  pr_wxref
 *
 *  Recursively traverse the given tree, visiting each
 *  node using the function "pr_lines()" below.
 */

void_t
pr_wxref(f,t)
register FILE *f;
register struct word_node *t;
{
	if(t != NULL){
		pr_wxref(f,t->w_left);
		fprintf(f,"%5d  %-20s",t->w_cnt,t->w_word);
		pr_lines(f,t);
		pr_wxref(f,t->w_right);
	}
}

void_t
pr_lines(f,t)
register FILE *f;
register struct word_node *t;
{
	register struct line_node *p;
	register unsigned int n = 0;

	for(p = t->w_line ; p != NULL ; p = p->l_next){
		if(n == 10){
			fprintf(f,"\n%5s  %20s"," "," ");
			n = 1;
		}
		else if(n++ > 0){
			fputc(',',f);
		}
		fprintf(f,"%u",p->l_lineno);
	}
	fputc('\n',f);
}

/*
 *  fncopy
 *
 *  Print a line numbered listing of the
 *  file `f1' to the given file `f2'.
 */

void_t
fncopy(f1,f2)
register FILE *f1, *f2;
{
	char *fgets();
	register unsigned int n;
	char line[MAXLINE+1];

	for(n = 1 ; fgets(line,MAXLINE,f1) != NULL ; n++)
		fprintf(f2,"%4u  %s",n,line);
}
