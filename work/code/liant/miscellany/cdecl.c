/* cdecl - Map declarators from C-to-English & English-to-C */

/***********************************************************************
/*
/* USAGE
/*
/*	cdecl [-r]
/*
/* DESCRIPTION
/*
/*	By default, this is an interactive program to print in English,
/*	the meaning of a C declarator.  For example, entering:
/*	'int *volatile*(*(*const)[])()' would give: 'const-pointer to
/*	array of pointer to function returning pointer to volatile-pointer'.
/*
/*	If the "-r" flag is given, then this is an interactive program
/*	to print a C declarator given an English-like description
/*	consisting exclusively of the letters (case insensitive):
/*
/*		'P'    meaning    "pointer-to ..."
/*		'A'    meaning    "array-of ..."
/*		'F'    meaning    "function-returning ..."
/*		'C'    meaning    "const ..."
/*		'V'    meaning    "volatile ..."
/*
/*	Control-D to terminate.
/*
/* USELESS TRIVIA
/*
/*	You may wonder just how many possible unqualified (i.e. no const
/*	or volatile) derived declarator types (i.e. types which specify
/*	pointers, arrays, and/or functions) there are.  No?  Well I'll
/*	tell you anyway.  If the maximum number of these derived types
/*	per declarator is say 12 (minimum required by ANSI-C), then a
/*	total of 769,509 derived declarator types are possible.  If the
/*	maximum is 15 (the maximum allowed in LPI-C), then the total is
/*	11,352,234!  The formula is:
/*
/*	for (total = 0, n = 0 ; n < MAX_DERIVED_DECLARATOR_TYPES ; n++)
/*		total = total + (pow(3,n) - (3*(n-1)*pow(2,(n-2))))
/*
/**********************************************************************/

/***********************************************************************
/*
/*  EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/**********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <ctype.h>
#include <setjmp.h>

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

typedef unsigned short	ushort;

/* ---------------------------------------------------------------------
/* Definitions
/* ------------------------------------------------------------------- */

/* Token type codes */

#define UNKNOWN_TOKEN		 (-2)
#define NULL_TOKEN		 0
#define IDENTIFIER_TOKEN	 1
#define STAR_TOKEN		 2
#define LPAREN_TOKEN		 3
#define RPAREN_TOKEN		 4
#define LSQUARE_TOKEN		 5
#define RSQUARE_TOKEN		 6
#define CONST_TOKEN		 7
#define VOLATILE_TOKEN		 8

/* Derived declarator type codes */

#define NULL_TYPE		((ushort)0x0000)
#define POINTER_TYPE		((ushort)0x0001)
#define ARRAY_TYPE		((ushort)0x0002)
#define FUNCTION_TYPE		((ushort)0x0003)

#define TYPE_MASK		((ushort)0x00FF)

/* Type qualifier codes */

#define NULL_TYPE_QUALIFIER	((ushort)0x0000)
#define CONST_TYPE_QUALIFIER	((ushort)0x0100)
#define VOLATILE_TYPE_QUALIFIER	((ushort)0x0200)

#define TYPE_QUALIFIER_MASK	((ushort)0xFF00)

/* Maxima */

#define DERIVED_DECLARATOR_STACK_SIZE	1024
#define DECLARATOR_IDENTIFIER_SIZE	 256
#define TOKEN_SPELLING_SIZE		 256

/* ---------------------------------------------------------------------
/* Global data definitions
/* ------------------------------------------------------------------- */

static ushort	BaseTypeQualifier = 0;
static char	DeclaratorIdentifier [DECLARATOR_IDENTIFIER_SIZE] = "";

static ushort	DerivedDeclaratorStack [DERIVED_DECLARATOR_STACK_SIZE];
static int	TopDerivedDeclaratorStack = -1;

static int	LookAheadToken = NULL_TOKEN;
static char	LookAheadTokenSpelling [TOKEN_SPELLING_SIZE];

static int	HitEof = 0;

static jmp_buf	StartDeclaration;

/* ---------------------------------------------------------------------
/* External function declarations
/* ------------------------------------------------------------------- */

extern void	strcpy ();
extern void	strcat ();
extern int	strcmp ();
extern int	longjmp ();
extern int	setjmp ();

/* ---------------------------------------------------------------------
/* Internal function declarations
/* ------------------------------------------------------------------- */

static void	english_to_c_declarator ();
static void	get_english_declarator ();
static void	print_c_declarator ();
static void	c_declarator_to_english ();
static void	declaration ();
static void	type_qualifiers ();
static void	declarator ();
static void	push_derived_declarator_stack ();
static void	print_english_declarator ();
static void	match_token ();
static int	get_next_token ();
static void	skipline ();
static void	newline ();
static void	prompt ();
static void	warning ();
static void	error ();

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (argc, argv)
int	 argc;
char	*argv[];
{
	if ((argc > 1) && (argv[1][0] == '-') && (argv[1][1] == 'r'))
		english_to_c_declarator ();
	else	c_declarator_to_english ();
}

/* ---------------------------------------------------------------------
/* english_to_declarator
/* ------------------------------------------------------------------- */

static
void
english_to_c_declarator ()
{
	(void) setjmp (StartDeclaration);

	while (1) {
		BaseTypeQualifier = NULL_TYPE_QUALIFIER;
		TopDerivedDeclaratorStack = -1;
		prompt ();
		get_english_declarator ();
		if (HitEof) break;
		print_c_declarator ();
	}
	newline ();
	exit (0);
}

/* ---------------------------------------------------------------------
/* get_english_declarator
/* ------------------------------------------------------------------- */

static
void
get_english_declarator ()
{
	register int c;
	register ushort tq = NULL_TYPE_QUALIFIER;

	while ((c = getc(stdin)) != '\n') {
		if ((c == ' ') || (c == '\t'))
			continue;
		if ((c == 'p') || (c == 'P')) {
			push_derived_declarator_stack (POINTER_TYPE | tq);
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'a') || (c == 'A')) {
			push_derived_declarator_stack (ARRAY_TYPE);
			BaseTypeQualifier |= tq;
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'f') || (c == 'F'))
			push_derived_declarator_stack (FUNCTION_TYPE);
		else if ((c == 'c') || (c == 'C'))
			tq |= CONST_TYPE_QUALIFIER;
		else if ((c == 'v') || (c == 'V'))
			tq |= VOLATILE_TYPE_QUALIFIER;
		else if (c == EOF) {
			HitEof = 1;
			return;
		}
		else	error ("Use only letters: 'P', 'A', 'F', 'V', or 'C'");
	}
	BaseTypeQualifier |= tq;
}

/* ---------------------------------------------------------------------
/* print_c_declarator
/* ------------------------------------------------------------------- */

static
void
print_c_declarator ()
{
	register int n, t, tq, lastt;
	char s[1024], tmp[1024];

	s[0] = '\0';
	tmp[0] = '\0';

	for (t = NULL_TYPE, n = 0 ; n <= TopDerivedDeclaratorStack ; n++) {
		lastt = t;
		t = DerivedDeclaratorStack[n] & TYPE_MASK;
		tq = DerivedDeclaratorStack[n] & TYPE_QUALIFIER_MASK;
		if (t == POINTER_TYPE) {
			strcpy (tmp, s);
			strcpy (s, "*");
			if (tq & CONST_TYPE_QUALIFIER)
				strcat (s, "const ");
			if (tq & VOLATILE_TYPE_QUALIFIER)
				strcat (s, "volatile ");
			strcat (s, tmp);
		}
		else {	if (lastt == POINTER_TYPE) {
				strcpy (tmp, s);
				strcpy (s, "(");
				strcat (s, tmp);
				strcat (s, ")");
			}
			if (t == FUNCTION_TYPE)
				strcat (s, "()");
			else	strcat (s, "[]");
		}
	}
	printf ("-----> ");
	if (BaseTypeQualifier & CONST_TYPE_QUALIFIER)	 printf ("const ");
	if (BaseTypeQualifier & VOLATILE_TYPE_QUALIFIER) printf ("volatile ");
	printf ("<type> %s\n", s);
}

/* ---------------------------------------------------------------------
/* c_declarator_to_english
/* ------------------------------------------------------------------- */

static
void
c_declarator_to_english ()
{
	(void) setjmp (StartDeclaration);

	while (1) {
		BaseTypeQualifier = NULL_TYPE_QUALIFIER;
		DeclaratorIdentifier [0] = '\0';
		TopDerivedDeclaratorStack = -1;
		prompt ();
		LookAheadToken = NULL_TOKEN;
		match_token (LookAheadToken);
		declaration ();
		if (HitEof) break;
		if (LookAheadToken == UNKNOWN_TOKEN) {
			warning ("Unknown token(s) at end of line!");
			skipline ();
		}
		else if (LookAheadToken != NULL_TOKEN) {
			warning ("Excess token(s) at end of line!");
			skipline ();
		}
		print_english_declarator ();
	}
	newline ();
	exit (0);
}

/* ---------------------------------------------------------------------
/* declaration
/* ------------------------------------------------------------------- */

static
void
declaration ()
{
	type_qualifiers (&BaseTypeQualifier);
	declarator ();
}

/* ---------------------------------------------------------------------
/* type_qualifiers
/* ---------------------------------------------------------------------
/* type_qualifiers	::= [ 'const' | 'volatile' ] ...
/* ------------------------------------------------------------------- */

static
void
type_qualifiers (btq)
ushort *btq;
{
	ushort tq = NULL_TYPE_QUALIFIER;

	while (1) {
		if (LookAheadToken == CONST_TOKEN) {
			match_token (CONST_TOKEN);
			if (tq & CONST_TYPE_QUALIFIER)
				warning ("Duplicate type qualifier");
			else	tq |= CONST_TYPE_QUALIFIER;
		}
		else if (LookAheadToken == VOLATILE_TOKEN) {
			match_token (VOLATILE_TOKEN);
			if (tq & VOLATILE_TYPE_QUALIFIER)
				warning ("Duplicate type qualifier");
			else	tq |= VOLATILE_TYPE_QUALIFIER;
		}
		else	break;
	}
	*btq = tq;
}

/* ---------------------------------------------------------------------
/* declarator
/* ---------------------------------------------------------------------
/* declarator		::= { identifier
/*			    | '(' declarator  ')'
/*			    | [ '*' [ type_qualifiers ] ] declarator
/*			    }
/*			    [ '[' constant_expression ']'
/*			    | '(' parameter_type_list ')'
/*			    ] ...
/* constant_expression	::= { }
/* parameter_type_list	::= { }
/* ------------------------------------------------------------------- */

static
void
declarator ()
{
	ushort tq;

	if (LookAheadToken == IDENTIFIER_TOKEN) {
		strcpy (DeclaratorIdentifier, LookAheadTokenSpelling);
		match_token (IDENTIFIER_TOKEN);
	}
	else if (LookAheadToken == LPAREN_TOKEN) {
		match_token (LPAREN_TOKEN);
		if (LookAheadToken != RPAREN_TOKEN) {	/* Hmmm */
			declarator ();
			match_token (RPAREN_TOKEN);
		}
		else {	push_derived_declarator_stack (FUNCTION_TYPE);
			match_token (RPAREN_TOKEN);
		}
	}
	else if (LookAheadToken == STAR_TOKEN) {
		match_token (STAR_TOKEN);
		type_qualifiers (&tq);
		declarator ();
		push_derived_declarator_stack (POINTER_TYPE | tq);
	}
	while (1) {
		if (LookAheadToken == LSQUARE_TOKEN) {
			match_token (LSQUARE_TOKEN);
			push_derived_declarator_stack (ARRAY_TYPE);
			match_token (RSQUARE_TOKEN);
		}
		else if (LookAheadToken == LPAREN_TOKEN) {
			match_token (LPAREN_TOKEN);
			push_derived_declarator_stack (FUNCTION_TYPE);
			match_token (RPAREN_TOKEN);
		}
		else	break;
	}
}

/* ---------------------------------------------------------------------
/* push_derived_declarator_stack
/* ------------------------------------------------------------------- */

static
void
push_derived_declarator_stack (t)
ushort t;
{
	if (TopDerivedDeclaratorStack++ >= DERIVED_DECLARATOR_STACK_SIZE)
		error ("Declaration stack overflow!");
	DerivedDeclaratorStack [TopDerivedDeclaratorStack] = t;
}

/* ---------------------------------------------------------------------
/* print_english_declarator
/* ------------------------------------------------------------------- */

static
void
print_english_declarator ()
{
	register int n, t, tq;

	printf ("-----> ");

	/* Print the identifier name (if any) */

	if (DeclaratorIdentifier[0] != '\0')
		printf ("%s: ", DeclaratorIdentifier);

	/* Print the derived declarator type */

	for (n = 0 ; n <= TopDerivedDeclaratorStack ; n++) {
		t = DerivedDeclaratorStack[n] & TYPE_MASK;
		if (t == POINTER_TYPE) {
			tq = DerivedDeclaratorStack[n] & TYPE_QUALIFIER_MASK;
			if (tq & CONST_TYPE_QUALIFIER)	  printf ("const-");
			if (tq & VOLATILE_TYPE_QUALIFIER) printf ("volatile-");
			printf ("pointer to ");
		}
		else if (t == ARRAY_TYPE)	printf ("array of ");
		else if (t == FUNCTION_TYPE)	printf ("function returning ");
	}

	/* Print the base type qualifier and type */

	if (BaseTypeQualifier & CONST_TYPE_QUALIFIER) 	 printf ("const ");
	if (BaseTypeQualifier & VOLATILE_TYPE_QUALIFIER) printf ("volatile ");
	printf ("<type>\n");
}

/* ---------------------------------------------------------------------
/* match_token
/* ------------------------------------------------------------------- */

static
void
match_token (expected_token_type)
int expected_token_type;
{
	if (LookAheadToken != expected_token_type)
		error ("Unexpected token!");
	LookAheadToken = get_next_token (LookAheadTokenSpelling);
}

/* ---------------------------------------------------------------------
/* get_next_token
/* ------------------------------------------------------------------- */

static
int
get_next_token (spelling)
char *spelling;
{
	register int c;
	register char *p;

	for (c = getc(stdin) ; (c == ' ') || (c == '\t') ; c = getc(stdin))
		;

	if      (c == '(')	return (LPAREN_TOKEN);
	else if (c == ')')	return (RPAREN_TOKEN);
	else if (c == '[')	return (LSQUARE_TOKEN);
	else if (c == ']')	return (RSQUARE_TOKEN);
	else if (c == '*')	return (STAR_TOKEN);
	else if (c == '\n')	return (NULL_TOKEN);

	else if (isalpha(c)) {
		p = spelling;
		for ( ; isalnum(c) ; c = getc(stdin))
			*p++ = c;
		*p = '\0';
		ungetc (c, stdin);
		if (strcmp(spelling,"const") == 0)
			return (CONST_TOKEN);
		else if (strcmp(spelling,"volatile") == 0)
			return (VOLATILE_TOKEN);
		else	return (IDENTIFIER_TOKEN);
	}
	else if (c == EOF) {
		HitEof = 1;
		return (NULL_TOKEN);
	}
	else	return  (UNKNOWN_TOKEN);
}

/* ---------------------------------------------------------------------
/* skipline
/* ------------------------------------------------------------------- */

static
void
skipline ()
{
	while (getc(stdin) != '\n')
		;
}

/* ---------------------------------------------------------------------
/* newline
/* ------------------------------------------------------------------- */

static
void
newline ()
{
	printf ("\n");
}

/* ---------------------------------------------------------------------
/* prompt
/* ------------------------------------------------------------------- */

static
void
prompt ()
{
	printf ("cdecl> ");
}

/* ---------------------------------------------------------------------
/* warning
/* ------------------------------------------------------------------- */

static
void
warning (s)
char *s;
{
	printf ("****** Warning: %s\n", s);
}

/* ---------------------------------------------------------------------
/* error
/* ------------------------------------------------------------------- */

static
void
error (s)
char *s;
{
	printf ("****** Error: %s\n", s);
	skipline ();
	(void) longjmp (StartDeclaration);
}

