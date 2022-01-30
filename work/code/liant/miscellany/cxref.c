/* cxref - C/Pascal Token Cross Reference Program */

/***********************************************************************
 *
 *  NAME
 *	cxref - C cross-reference program
 *
 *  SYNOPSIS
 *	cxref [-k] [-pN] [-pascal] [C_source_file]...
 *
 *  DESCRIPTION
 *	Prints to the standard output, a cross-reference
 *	listing of all of the tokens appearing in the given
 *	file(s) or the standard input if no file is given.
 *
 *	C keywords and pre-processor keywords are not
 *	included in the list.  However if the "-k" flag
 *	is given, then C keywords will be included in the
 *	listing.
 *
 *	If the "-pN" option is given, then N specifies the
 *	number of path name components to include in the
 *	output of the file names ("-p0" is the same as "-p",
 *	which will delete all path name components).
 *
 *	If the "-pascal" option is given, then Pascal style
 *	comments will be skipped instead of C style comments;
 *	and Pascal keywords will be recognized instead of C keywords.
 *
 *  EXAMPLE OUTPUT
 *
 *	Token		Lines			File
 *	-----		-----			----
 *	function1	10			function1.c
 *			80,120,130,140,150	main.c
 *			160,170,180,190,200
 *	main		110			main.c
 *	variable1	200,300,400,500		function1.c
 *			800			main.c
 *  NOTES
 *	This program does no significant syntax checking.
 *
 *  SEE ALSO
 *	ctags(1), pxref(1), lxref(1), ckey(1L), num(1)
 *
 ***********************************************************************/

/***********************************************************************
 *
 *  EDIT HISTORY
 *
 *  06.xx.84  DGM  Orignal. David G. Michaels (david@ll-sst).
 *  02.xx.88  DGM  Fixed bug (LPI).
 *  10.12.89  DGM  Added (partial) Pascal support; to be used with the
 *                 TauMetric C++ compiler sources written in Pascal (LPI).
 *
 ***********************************************************************/

/***********************************************************************
 *			INCLUDE FILES
 ***********************************************************************/

#include <stdio.h>
#include <ctype.h>

/***********************************************************************
 *			DEFINITIONS
 ***********************************************************************/

#define NO_TOTALS  /* don't print token totals */

#define MAX_TOKEN_LENGTH	 64   /* max length of a C token */
#define MAX_LINE_LENGTH		512  /* max length of input line */

#define MAX_C_KEYWORDS		 32  /* number of C key words */
#define MAX_CPP_KEYWORDS	 12  /* number of C Preprocessor key words */
#define MAX_PASCAL_RESWORDS	 38  /* number of Pascal reserved words */
#define MAX_PASCAL_PREDEFWORDS	 52  /* number of Pascal predefined words */

#define BACKSLASH	'\\'
#define DASH		'-'
#define DOT		'.'
#define DQUOTE		'"'
#define EOS		'\0'
#define NEWLINE		'\n'
#define SLASH		'/'
#define SPACE		' '
#define SPLAT		'#'
#define SQUOTE		'\''
#define STAR		'*'
#define UNDERSCORE	'_'
#define LEFT_BRACE	'{'
#define RIGHT_BRACE	'}'

#define FALSE		(0)
#define TRUE		(!FALSE)

typedef int		bool;

/***********************************************************************
 *			MACROS
 ***********************************************************************/

#define find_cpp_keyword(s)       search_table (cpp_keywords, \
						MAX_CPP_KEYWORDS, \
						(s))
#define find_c_keyword(s)         search_table (c_keywords, \
						MAX_C_KEYWORDS, \
						(s))
#define find_pascal_resword(s)    search_table (pascal_reswords, \
						MAX_PASCAL_RESWORDS, \
						(s))
#define find_pascal_predefword(s) search_table (pascal_predefwords, \
						MAX_PASCAL_PREDEFWORDS, \
						(s))

#define talloc() ((struct token_node *)malloc(sizeof(struct token_node)))
#define lalloc() ((struct line_node *)malloc(sizeof(struct line_node)))

#define token_start(c)	(isalpha(c) || (c) == UNDERSCORE)
#define token_char(c)	(isalnum(c) || (c) == UNDERSCORE)

#define str_eq(a,b)	(strcmp((a),(b)) == 0)

/***********************************************************************
 *			DATA STRUCTURES
 ***********************************************************************/

struct line_node {
	unsigned int	  l_lineno;	/* the line number */
	struct line_node *l_next;	/* ptr to next in list */
	struct line_node *l_prev;	/* ptr to previous in list */
};

struct token_node {
	char		  *t_token;	/* the C token */
	char		  *t_file;	/* name of file */
	struct line_node  *t_line;	/* list of line numbers */
	unsigned int	   t_cnt;	/* number of occurrences */
	unsigned short	   t_flag;	/* for later use */
	struct token_node *t_left;	/* ptr to left subtree */
	struct token_node *t_right;	/* ptr to right subtree */
};

/***********************************************************************
 *			GLOBAL DATA
 ***********************************************************************/

static char *c_keywords [MAX_C_KEYWORDS] = {

	/*
	 *  C Key Word Table
	 *  [must be in increasing alphabetical order]
	 */

	"auto",
	"break",
	"case",
	"char",
	"const",
	"continue",
	"default",
	"do",
	"double",
	"else",
	"enum",
	"extern",
	"float",
	"for",
	"goto",
	"if",
	"int",
	"long",
	"register",
	"return",
	"short",
	"signed",
	"sizeof",
	"static",
	"struct",
	"switch",
	"typedef",
	"union",
	"unsigned",
	"void",
	"volatile",
	"while"
};

static char *cpp_keywords [MAX_CPP_KEYWORDS] = {

	/*
	 *  C Preprocessor Keyword Table.
	 *  [must be in increasing alphabetical order]
	 */

	"define",
	"elif",
	"else",
	"endif",
	"error",
	"if",
	"ifdef",
	"ifndef",
	"include",
	"line",
	"pragma",
	"undef"
};

static char *pascal_reswords [MAX_PASCAL_RESWORDS] = {

	/*
	 *  Pascal Reserved Word Table.
	 *  [must be in increasing alphabetical order]
	 */

	"and",
	"array",
	"begin",
	"case",
	"const",
	"div",
	"do",
	"downto",
	"else",
	"end",
	"extern",
	"external",
	"file",
	"for",
	"function",
	"goto",
	"if",
	"in",
	"label",
	"mod",
	"nil",
	"not",
	"of",
	"or",
	"otherwise",
	"packed",
	"procedure",
	"program",
	"record",
	"repeat",
	"set",
	"then",
	"to",
	"type",
	"until",
	"var",
	"while",
	"with"
};

static char *pascal_predefwords [MAX_PASCAL_PREDEFWORDS] = {

	/*
	 *  Pascal Predefined Word Table.
	 *  [must be in increasing alphabetical order]
	 */

	"abs",
	"alfa",
	"append",
	"arctan",
	"atan",
	"boolean",
	"break",
	"char",
	"chr",
	"close",
	"concat",
	"copy",
	"cos",
	"dispose",
	"delete",
	"eof",
	"eoln",
	"exit",
	"exp",
	"exp10",
	"expo",
	"false",
	"forward",
	"get",
	"integer",
	"ln",
	"log",
	"maxint",
	"new",
	"odd",
	"open",
	"ord",
	"pack",
	"pred",
	"put",
	"read",
	"readln",
	"real",
	"reset",
	"rewrite",
	"round",
	"sin",
	"sizeof",
	"sqrt",
	"succ",
	"text",
	"true",
	"trunc",
	"undefined",
	"unpack",
	"write",
	"writeln"
};

/***********************************************************************
 *			GLOBAL VARIABLES
 ***********************************************************************/

unsigned int	 curchar;	 /* current char # of in the file */
char		*curfile;	 /* current file name */
unsigned int	 curline;	 /* current line # of the file */
char		*progname;	 /* name of this program (av[0]) */
bool		 pascal = FALSE; /* skip Pascal style comments */

/***********************************************************************
 *			MAIN FUNCTION
 ***********************************************************************/

main(ac,av)
int	ac;
char	*av[];
{
	char			   *rindex();
	char			   *next_token();
	struct token_node	   *install();
	void			   pr_report();
	register struct token_node *ctree = NULL;
	register char		   *token;
	FILE			   *f;
	int			   npath = -1;
	bool			   kflag = FALSE;

#ifdef NO_TOTALS
	register long int	   tokentotal;
#endif NO_TOTALS

	progname = *av;

	/*
	 *  Parse the command line arguments.
	 *  All flags must precede file arguments.
	 *  "av" is left pointing at the first file
	 *  argument (if present) after the last flag,
	 *  and "ac" is the number of file arguments
	 *  (zero if none). If no file names are present,
	 *  then the standard input will be the input file.
	 */

	for(ac--, av++ ; ac > 0 && **av == DASH ; ac--, av++) {
		if (strcmp (&((*av)[1]), "pascal") == 0) {
			pascal = TRUE;
			continue;
		}
		switch((*av)[1]) {
		case 'k':
			kflag = TRUE;
			break;
		case 'p':
			if((npath = atoi(*av+2)) < 0)
				npath = 0;
			break;
		}
	}

	for(f = (ac==0 ? stdin : NULL) ; ac > 0 || f == stdin ; av++, ac--) {

		if(f != stdin && (f = fopen(*av,"r")) == NULL){
			fprintf(stderr,
				"\n%s: can't open %s...continuing.\n",
				progname,*av);
			continue;
		}

#ifdef NO_TOTALS
		tokentotal = 0;
#endif NO_TOTALS
		curline = 1;
		curchar = 0;

		curfile = *av;
		if(npath >= 0) {
			int i;
			char *p;
			p = curfile + strlen(curfile) - 1;
			for(i = 0 ; i <= npath && p != curfile ; p--) {
				if(*p == '/')
					i++;
			}
			curfile = p + 2;
		}

		if (!kflag) {
			while((token = next_token(f)) != NULL){
				if (pascal &
				    ((find_pascal_resword (token) >= 0) ||
				     (find_pascal_predefword (token) >= 0)))
					continue;
				else if (find_c_keyword (token) >= 0)
					continue;
#ifdef NO_TOTALS
				tokentotal++;
#endif NO_TOTALS
				ctree =
				install (ctree, token, curfile, curline);
			}
		}
		else {
			while ((token = next_token (f)) != NULL) {
#ifdef NO_TOTALS
				tokentotal++;
#endif NO_TOTALS
				ctree =
				install (ctree, token, curfile, curline);
			}
		}
		fclose (f);
		f = NULL;
	}

	if (ctree != NULL) {
		pr_report (stdout, ctree);
	}

	exit(0);
}

void
usage()
{
	fprintf (stderr, 
		 "usage: %s [-k] [-pascal] [C_source_file]...\n",
		 progname);
	exit (1);
}

void
fatal(s)
char *s;
{
	fprintf(stderr,"%s: %s...exit.\n",progname,s);
	exit(2);
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

/***********************************************************************
 *			PARSING FUNCTIONS
 ***********************************************************************/

/*
 *  readc
 *
 *  Return the next character in the file, and
 *  increment the global variable `curline' if
 *  a NEWLINE is read.  Front end to getc.
 */

 int
 readc(f)
 register FILE *f;
 {
	extern unsigned int curchar;
	extern unsigned int curline;
	register int c;

	if((c = getc(f)) == NEWLINE)
		curline++;
	curchar++;
	return(c);
}

int
unreadc(c,f)
register int c;
register FILE *f;
{
	extern unsigned int curchar;
	extern unsigned int curline;

	if(c == NEWLINE)
		curline--;
	curchar--;
	return(ungetc(c,f));
}

/*
 *  findchar
 *
 *  Return the next non-white-space
 *  character ** or ** NEWLINE in the input.
 */

int
findchar(f)
register FILE *f;
{
	register int c;

	for(c = readc(f) ; isspace(c) ; c = readc(f))
		if(c == NEWLINE)
			break;
	return(c);
}
	
/*
 *  nextline
 *
 *  Read past the end of line.  Return the
 *  first character on the next line or EOF.
 */

int
nextline(f)
register FILE *f;
{
	register int c;

	while((c = readc(f)) != EOF && c != NEWLINE)
		;
	return(c);
}
	
/*
 *  next_token
 *
 *  Return (in static storage) the next C token in the
 *  input.  If none are left (i.e. at EOF), return NULL.
 */

char *
next_token(f)
register FILE *f;
{
	extern unsigned int curchar;
	register int c;
	static char buf[MAX_TOKEN_LENGTH+1];
	register int i = 0;

	buf[0] = EOS;
	
	for(c = findchar(f) ; c != EOF ; ) {
		if(token_start(c)) {
			while(TRUE) {
				buf[i++] = c;
				c = readc(f);
				if(!token_char(c))
					break;
				/*
				 * Skip the rest of the token
				 * if it's too long to fit.
				 */
				if(i > MAX_TOKEN_LENGTH) {
					c = readc(f);
					while(token_char(c))
						c = readc(f);
					break;
				}
			}
			if(c != EOF)
				unreadc(c,f);
			buf[i] = EOS;
			break;
		}
		else if(c == NEWLINE) {
			if((c = readc(f)) == SPLAT)
				c = skip_cpp(f);
			else
				continue;
		}
		else if(!pascal & (curchar == 1 && c == SPLAT)) {
			c = skip_cpp(f);
		}
		else if(!pascal & (c == SLASH)) {
			c = skip_comment(f);
		}
		else if (pascal & (c == LEFT_BRACE)) {
			c = skip_pascal_comment(f);
		}
		else if(c == DQUOTE) {
			c = skip_str_const(f);
		}
		else if(c == SQUOTE) {
			c = skip_char_const(f);
		}
		else if(isdigit(c)) {
			c = skip_num_const(f);
		}
		else {
			c = readc(f);
		}
	}
	return(buf[0] == EOS ? NULL : buf);
}

/*
 *  skip_comment
 *
 *  Skip past a C comment in a the given file `f`.  The IO pointer
 *  upon entry to this routine is assumed to be just past a SLASH
 *  character.  That is:
 *
 *  If the next readc(f) == STAR then this is a comment, and
 *  we will read until we find the closing SLASH and STAR; the
 *  next character will be returned. 
 *
 *  If the the next readc(f) != STAR then this is not a comment, and
 *  the character just read will be returned.
 *
 *  Comments may not be embedded.  If the end of file is
 *  reached anywhere along the way, then EOF will be returned.
 */

int
skip_comment(f)
register FILE *f;
{
	register int c;

	if((c = readc(f)) == STAR) {
		for(c = readc(f) ; c != EOF ; c = readc(f)) {
			if(c == STAR)
				if((c = readc(f)) == SLASH)
					return(readc(f));
		}
	}
	return(c);
}

/*
 *  skip_pascal_comment
 *
 *  Skip past a Pascal comment in a the given file `f`.  The IO
 *  pointer upon entry to this routine is assumed to be just past
 *  a LEFT_BRACE character.  That is, this is a comment, and
 *  we will read until we find the closing RIGHT_BRACE; the
 *  next character will be returned. 
 *
 *  Comments may not be embedded.  If the end of file is
 *  reached anywhere along the way, then EOF will be returned.
 */

int
skip_pascal_comment (f)
register FILE *f;
{
	register int c;

	for (c = readc (f) ; c != EOF ; c = readc (f)) {
		if (c == RIGHT_BRACE)
			return (readc (f));
	}
	return (c);
}

/*
 *  skip_str_const
 *
 *  Skip past a C string in the given file.  The IO pointer is
 *  assumed to be pointing just past a DQUOTE character.  If the
 *  end of file is reached return EOF, otherwise return the character
 *  immediately follwing the end of the string.
 */

int
skip_str_const(f)
register FILE *f;
{
	register int c;
	register bool esc = FALSE;

	while((c = readc(f)) != EOF) {
		if(c == DQUOTE && !esc)
			return(readc(f));
		else if(c == BACKSLASH)
			esc = (esc ? FALSE : TRUE);
		else
			esc = FALSE;
	}
	return(c);
}

/*
 *  skip_char_const
 *
 *  Skip past a C character constant in the given file.  The IO pointer
 *  is assumed to be pointing just past a SQUOTE character.  If the
 *  end of file is reached return EOF, otherwise return the character
 *  immediately follwing the end of the string.
 */

int
skip_char_const(f)
register FILE *f;
{
	register int c;
	register bool esc = FALSE;

	while((c = readc(f)) != EOF) {
		if(c == SQUOTE && !esc)
			return(readc(f));
		else if(c == BACKSLASH)
			esc = (esc ? FALSE : TRUE);
		else
			esc = FALSE;
	}
	return(c);
}

/*
 *  skip_num_const
 *
 *  Skip past a numerical constant in a C source file.  The IO pointer
 *  is assumed to be pointing just past a numeric character.  If the
 *  end of file is reached return EOF, otherwise return the character
 *  immediately following the end of the number.
 */

skip_num_const(f)
register FILE *f;
{
	register int c;

	while((c = readc(f)) != EOF && isalnum(c))
		;
	return(c);
}

/*
 *  skip_cpp
 *
 *  Skips past a C preprocessor directive keyword (it is assumed that
 *  the previous character read was a SPLAT as the first character on
 *  a line).  Returns the next character after the keyword, or the first
 *  character in the next line if an appropriate keyword is not found
 *  (i.e. if a NEWLINE-SPLAT is followed by an illegal keyword; in this
 *  case, a warning will be printed, and the whole rest of the line is
 *  ignored).  Totally skips a "#include" line.
 */

int
skip_cpp(f)
register FILE *f;
{

#	define MAX_CPP_KEYWORD_LENGTH	  8

	extern unsigned int curline;
	char buf [MAX_CPP_KEYWORD_LENGTH+1];
	register int c, i, n = 0;
	register char *p = buf, **q;
	unsigned int thisline = curline;

	if (((c = findchar (f)) != EOF) && (c != NEWLINE)) {
		for (*p++ = c ; (c = readc(f)) != EOF ; *p++ = c) {
			if ((n++ == MAX_CPP_KEYWORD_LENGTH) ||
			    (!token_char (c) && !isspace (c)))
				goto Bad;
			else if (!token_char(c))
				break;
		}
		*p = EOS;
		if ((i = find_cpp_keyword (buf)) < 0)
			goto Bad;
		if (str_eq (cpp_keywords [i], "include") ||
		    str_eq (cpp_keywords [i], "line"))
			return (nextline (f));
		return (c);
	}
Bad:
	fprintf (stderr, "Warning: bad pre-processor line (%u)\n", thisline);
	if (c == NEWLINE)
		return (NEWLINE);
	else	return (nextline(f));

}

/***********************************************************************
*			TREE FUNCTIONS
***********************************************************************/

/*
 *  install
 *
 *  Install a the token `token` which occurred at line `line`
 *  into the binary search tree `t`.  Return the token's new
 *  (or found) node.  Return NULL if memory shortage.
 *  Recursive.
 */

struct token_node *
install(t,token,file,line)
register struct token_node *t;
register char *token;
char *file;
unsigned int line;
{
	char *strsave();
	struct token_node *install();
	struct line_node *line_add();
	register int k, kk;

	if(t == NULL) {
		if((t = talloc()) == NULL)
			fatal("No space for word node (install)");
		if((t->t_token = strsave(token)) == NULL)
			fatal("No space for word (install)");
		t->t_cnt = 1;
		t->t_file = file;
		t->t_line = NULL;
		t->t_left = t->t_right = NULL;
		line_add(t,line);
	}
	else if((k = strcmp(token,t->t_token)) == 0 &&
		(kk = strcmp(file,t->t_file)) == 0) {
		line_add(t,line);
		t->t_cnt++;
	}
	else if(k < 0 || (k == 0 && kk < 0))
		t->t_left = install(t->t_left,token,file,line);
	else
		t->t_right = install(t->t_right,token,file,line);
	return(t);
}

/*
 *  line_add
 *
 *  Make an entry (if one does not yet exist) on the line list
 *  of the given token node `toknode`, for the given line
 *  number `line`.  Return the new or existing node.
 */

struct line_node *
line_add(toknode,line)
register struct token_node *toknode;
register unsigned int line;
{
	register struct line_node *new, *p, *last;
	register int cmp;

	/*
	 *  Null list; first time only.
	 */

	if(toknode->t_line == NULL) {
		if((new = lalloc()) == NULL)
			fatal("No space for line node (line_add)");
		new->l_next = NULL;
		toknode->t_line = new;
		last = new;
		goto Out;
	}

	last = toknode->t_line->l_prev;
	toknode->t_line->l_prev = NULL;
	for(p = last ; p != NULL ; p = p->l_prev) {
		/*
		 *  Found the node.
		 */
		if(line == p->l_lineno) {
			toknode->t_line->l_prev = last;
			return(p);
		}
		/*
		 *  Put the node after p.
		 */
		if(line > p->l_lineno) {
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
	if(p == NULL) {
		if((new = lalloc()) == NULL)
			return(NULL);
		new->l_next = toknode->t_line;
		toknode->t_line->l_prev = new;
		toknode->t_line = new;
		goto Out;
	}
Out:
	new->l_lineno = line;
	toknode->t_line->l_prev = last;
	return(new);
}

#ifdef NOT_DEFINED

/*
 *  free_tree
 *
 *  Frees up the tree pointed to by `p'.
 *  Recursive.
 */

void
free_tree(p)
register struct token_node *p;
{
	int free();

	if(p != NULL) {
		free_tree(p->t_left);
		free(p->t_token);
		free(p);
		free_tree(p->t_right);
	}
}

#endif NOT_DEFINED

/*
 *  search_table
 *
 *  Return the index into the given table of the given word.
 *  Return -1 if not in the table.
 */

int
search_table (table, table_size, string)
register char **table;
register int  table_size;
register char *string;
{
	register int lo, hi, mid, cond;

	lo = 0;
	hi = table_size - 1;
	while (lo <= hi) {
		mid = (lo + hi) / 2;
		if ((cond = strcmp (string ,table [mid])) < 0)
			hi = mid - 1;
		else if (cond > 0)
			lo = mid + 1;
		else	return (mid);
	}
	return (-1);
}

/***********************************************************************
*			OUTPUT FUNCTIONS
***********************************************************************/

#define IDENTIFIER_FIELD_WIDTH		24
#define LINE_NUMBER_FIELD_WIDTH		42
#define LINE_NUMBERS_PER_LINE		 8
#define LINE_NUMBER_FIELD_BUFFER_SIZE	(LINE_NUMBER_FIELD_WIDTH * 2)

#define put_newline(f)			(fputc (NEWLINE, (f)))
#define put_character(f,c)		(fputc ((c), (f)))
#define put_string(f,s)			(fputs ((s), (f)))

static
void
put_field (f, s, c, n)
register FILE *f;
register char *s;
register int c;
register int n;
{
	if ((s != NULL) && (*s != EOS))
		put_string (f, s);

	if ((n > 0) && (c != EOS)) {
		register int i;
		if ((i = strlen (s)) >= n)
			put_character (f, c);
		else for (i = n - i ; i-- > 0 ; )
			put_character (f, c);
	}
}

static
void
pr_lines(f, t)
register FILE *f;
register struct token_node *t;
{
	register struct line_node *p;
	register unsigned int n = 0, i, l;
	char buffer [LINE_NUMBER_FIELD_BUFFER_SIZE + 1];
	bool first_line = TRUE;

	*buffer = EOS;

	for (p = t->t_line ; p != NULL ; p = p->l_next) {
		if (n == LINE_NUMBERS_PER_LINE) {
			if (first_line) {
				put_field
				(f, buffer, DOT, LINE_NUMBER_FIELD_WIDTH);
				put_string (f, t->t_file);
				first_line = FALSE;
			}
			else	put_field (f, buffer, EOS, 0);
			put_newline (f);
			put_field (f, "", SPACE, IDENTIFIER_FIELD_WIDTH);
			*buffer = EOS;
			n = 0;
		}
		else if (n > 0)
			sprintf (buffer, "%s,", buffer);
		sprintf (buffer, "%s%u", buffer, p->l_lineno);
		n++;
	}
	if (*buffer != EOS) {
		if (first_line) {
			put_field (f, buffer, DOT, LINE_NUMBER_FIELD_WIDTH);
			put_string (f, t->t_file);
			first_line = FALSE;
		}
		else	put_field (f, buffer, EOS, 0);
	}
	put_newline (f);
}

/*
 *  pr_cxref
 *
 *  Recursively traverse the given tree, visiting each
 *  node using the function "pr_lines()" below.
 */

void
pr_cxref (f, t)
register FILE *f;
register struct token_node *t;
{
	if (t != NULL) {
		pr_cxref (f, t->t_left);
		put_field (f, t->t_token, SPACE, IDENTIFIER_FIELD_WIDTH);
		pr_lines (f, t);
		pr_cxref (f, t->t_right);
	}
}

void
pr_report (f, ctree)
register FILE *f;
register struct token_node *ctree;
{
	put_newline (f);

	put_field (f , "Identifier"         , SPACE , IDENTIFIER_FIELD_WIDTH);
	put_field (f , "Line Occurrence(s)" , SPACE , LINE_NUMBER_FIELD_WIDTH);
	put_field (f , "File"               , EOS   , 0);

	put_newline (f);

	put_field (f , "----------"         , SPACE , IDENTIFIER_FIELD_WIDTH);
	put_field (f , "------------------" , SPACE , LINE_NUMBER_FIELD_WIDTH);
	put_field (f , "----"               , EOS   , 0);

	put_newline (f);

	pr_cxref (f, ctree);
}

