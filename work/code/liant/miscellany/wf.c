/***********************************************************************
* PROGRAM:	Word Frequency Program
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		July 1984
***********************************************************************/

/*
 *  NAME
 *	wf - word frequency program
 *
 *  SYNOPSIS
 *	wf [-t] [-i] [-c] [text_file]...
 *
 *  DESCRIPTION
 *	Prints to the standard output a listing of the frequency of
 *	each word appearing in the given file.  If multiple files
 *	are given then a total word count will also be given.  If
 *	the "-t" flag is given then only a total word count will be
 *	given.  If no file is given, then standard input will be
 *	read (in this case, the "-t" flag would be superfluous.  A 
 *	total unique word count is also given.  If the "-i" flag is
 *	given then upper case letters will be converted to lower case.
 *	If the "-c" flag is given, then single column output is produced
 *	rather than the default three entries per column; this is useful
 *	for piping output to other useful programs such as "sort".
 *
 *	A word is defined as being consecutive sequence of characters
 *	beginning with an alphabetic character, followed by zero
 *	or more alphanumeric characters, an underscore, or a dash.
 */

/***********************************************************************
*			INCLUDE FILES
***********************************************************************/

#include <stdio.h>
#include <ctype.h>

/***********************************************************************
*			DEFINITIONS
***********************************************************************/

#define WORDLEN		50	/* max length of a word */

#define DASH		'-'
#define EOS		'\0'
#define UNDERSCORE	'_'

#define FALSE		(0)
#define TRUE		(!FALSE)

typedef int		bool_t;
typedef int		void_t;	
typedef unsigned int	u_int;	

/***********************************************************************
*			DATA STRUCTURES
***********************************************************************/

struct word_node{
	char		 *w_word;
	u_int		  w_cnt;
	struct word_node *w_left;
	struct word_node *w_right;
};

/***********************************************************************
*			MACROS
***********************************************************************/

#define talloc() ((struct word_node *)malloc(sizeof(struct word_node)))

#define word_start(c) (isalpha(c))
#define word_char(c)  (isalnum(c) || c == UNDERSCORE || c == DASH)

/***********************************************************************
*			GLOBAL VARIABLES
***********************************************************************/

char		*Progname;	/* name of this program (av[0]) */
bool_t		Newword;	/* TRUE if new node added to tree */

/***********************************************************************
*			MAIN FUNCTIONS
***********************************************************************/

main(ac,av)
int	ac;
char	*av[];
{
	FILE			  *fopen();
	char			  *nextword(), *strlower();
	struct word_node	  *install();
	void_t			  free_tree(), print_tree();
	void_t			  header();

	register struct word_node *word_tree;
	struct word_node	  *total_tree = NULL;
	register char		  *word;
	register long	  	  i, uniq, total_uniq = 0;
	register long	  	  words, total_words = 0;
	u_int			  filesread = 0, n;
	FILE			  *f = NULL;
	bool_t			  cflag = FALSE;
	bool_t			  iflag = FALSE;
	bool_t			  tflag = FALSE;

	Progname = av[0];

	/*
	 *  Parse the command line arguments.
	 *  All flags must precede all file arguments.
	 *  "av" is left pointing at the first argument
	 *  after the last flag, and "ac" is the number
	 *  of file arguments.  If no file names are present,
	 *  then the standard input will be the input file.
	 */

	for(i = 1, n = 1 ; i < ac ; i++){
	   if(av[i][0] == DASH){
		n++;
		switch(av[i][1]){
		 case 'c':
			cflag = TRUE;
			break;
		 case 'i':
			iflag = TRUE;
			break;
		 case 't':
			tflag = TRUE;
			break;
		 default:
			;
		}
	   }
	}
	ac -= n;
	av += n;
	if(ac == 0){
		f = stdin;
		tflag = TRUE;
	}

	/*
	 *  Loop through each file.
	 */

	for(i = 0 ; i < ac || f == stdin ; i++){

		if(f != stdin && (f = fopen(av[i],"r")) == NULL){
			fprintf(stderr,"%s: can't open %s\n",Progname,av[i]);
			continue;
		}
		filesread++;

		word_tree = NULL;
		words = 0;
		uniq = 0;

		if(!tflag){
			printf("\nfile \"%s\"",av[i]);
			fflush(stdout);
		}

		/*
		 *  If the [-t] option was given
		 *  then just use the total tree.
		 */

		if(tflag) {
			while((word = nextword(f)) != NULL) {
				if(iflag)
					strlower(word);
				total_tree = install(total_tree,word);
				words++;
				if(Newword)
				   uniq++;
			}
		}

		/*
		 *  If more than one file is to be parsed,
		 *  then used both the per-file word tree,
		 *  and the total tree.
		 */

		else if(ac > 1) {
			while((word = nextword(f)) != NULL) {
				if(iflag)
					strlower(word);
				word_tree = install(word_tree,word);
				words++;
				if(Newword)
				   uniq++;
				total_tree = install(total_tree,word);
			}
		}

		/*
		 *  If just one file is to be parsed,
		 *  then just use the per-file word tree.
		 */

		else {
			while((word = nextword(f)) != NULL) {
				if(iflag)
					strlower(word);
				word_tree = install(word_tree,word);
				words++;
				if(Newword)
				   uniq++;
			}
		}

		/*
		 *  If the [-t] option was not given,
		 *  then print the totals for each file.
		 */

		if(!tflag){
			header(words,uniq);
			print_tree(word_tree, cflag ? 1 : 3);
		}

		total_uniq += uniq;
		total_words += words;
		free_tree(word_tree);
		fclose(f);
		f = NULL;
	}

	/*
	 *  Print totals if the [-t] option was given,
	 *  or if more than one file was parsed.
	 */

	if(tflag || filesread > 1){
		fputs("\n\ntotals",stdout);
		header(total_words,total_uniq);
		print_tree(total_tree, cflag ? 1 : 3);
	}
	exit(0);
}

void_t
header(words,uniqwords)
register long words, uniqwords;
{
	printf("  (%D word",words);
	if(words != 1)
		putchar('s');
	printf(", %D unique word",uniqwords);
	if(uniqwords != 1)
		putchar('s');
	printf(")\n\n");
}

void_t
fatal(s)
register char *s;
{
	fprintf(stderr,"%s: %s ...exit.\n",Progname,s);
	exit(1);
}

/***********************************************************************
*			UTILITY FUNCTIONS
***********************************************************************/

/*
 *  strsave
 *
 *  Return copy of string `s`.
 *  If no space, return NULL.
 */

char *
strsave(s)
register char *s;
{
	char		*malloc();
	register char	*p = NULL, *q;

	if((p = malloc(strlen(s) + 1)) != NULL)
		for(q = p ; (*q++ = *s++) != EOS ; )
			;
	return(p);
}

/*
 *  strlower
 *
 *  Convert all upper case letters in the given
 *  string `s` to lower case.  Return the string.
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
 *  nextword
 *
 *  Return pointer to the next word in the given file.
 *  Storage is locally static to this function, and is
 *  overwritten on each call.  Return NULL on EOF.
 */

char *
nextword(f)
register FILE *f;
{
	static char	word[WORDLEN+1];
	register int	c;
	register int	i;

	if(feof(f))
		return(NULL);

	for(c = fgetc(f), i = 0 ; c != EOF ; ){

		if(word_start((char)c)){
			do {
				word[i++] = c;
				c = fgetc(f);
			} while(word_char((char)c) && i <= WORDLEN);
			break;
		}
		else
			c = fgetc(f);
	}
	word[i] = EOS;
	return(*word == EOS ? NULL : word);
}

/***********************************************************************
*			TREE FUNCTIONS
***********************************************************************/

/*
 *  install
 *
 *  Installs the given word `w` in the binary
 *  search tree pointed to by `p`.  Recursive.
 */

struct word_node *
install(p,w)
register struct word_node *p;
register char *w;
{
	void_t		fatal();
	char		*strsave(), *malloc();
	register int	cond;

	if(p == NULL){
		/*
		 *  New word - make new node.
		 */
		if((p = talloc()) == NULL)
			fatal("no space for new tree node");
		if((p->w_word = strsave(w)) == NULL)
			fatal("no space for new word");
		p->w_cnt = 1;
		p->w_left = p->w_right = NULL;
		Newword = TRUE;
	}
	else if((cond = strcmp(w,p->w_word)) == 0){
		p->w_cnt++;
		Newword = FALSE;
	}
	else if(cond < 0)
		p->w_left = install(p->w_left,w);
	else
		p->w_right = install(p->w_right,w);
	return(p);
}

/*
 *  free_tree
 *
 *  Frees up the tree pointed to by `p`.  (Recursive)
 */

void_t
free_tree(p)
register struct word_node *p;
{
	int free();

	if(p != NULL){
		free_tree(p->w_left);
		free(p->w_word);
		free(p);
		free_tree(p->w_right);
	}
}

/***********************************************************************
*			OUTPUT FUNCTIONS
***********************************************************************/

static u_int	Outcol = 0;
static u_int	Ncol = 1;

/*
 *  print_tree
 *
 *  Prints to the standard output, the word tree
 *  formatted with `ncol` entries per line.
 *  Front end to the `trav_tree` function below.
 */

void_t
print_tree(p,ncol)
register struct word_node *p;
register u_int ncol;
{
	void_t trav_tree();

	if(ncol > 0)
		Ncol = ncol;
	trav_tree(p);
	Outcol = 0;
	putchar('\n');
}

static void_t
trav_tree(p)
register struct word_node *p;
{
	if(p != NULL){
		trav_tree(p->w_left);
		if(Outcol == Ncol){
			putchar('\n');
			Outcol = 0;
		}
		if(Ncol <= 1)
			printf("%4d  %s",p->w_cnt,p->w_word);
		else
			printf("%4d  %-20s",p->w_cnt,p->w_word);
		Outcol++;
		trav_tree(p->w_right);
	}
}
