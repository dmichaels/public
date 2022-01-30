/* ctoe - Map simple ANSI C declarators from C to English */

/***********************************************************************
/* This product is the property of Language Processors, Inc. and is    *
/* licensed pursuant to a written license agreement.  No portion of    *
/* this product may be reproduced without the written permission of    *
/* Language Processors, Inc. except pursuant to the license agreement. *
/***********************************************************************/

/***********************************************************************
/*
/*  LPI EDIT HISTORY
/*
/*  01.09.89  DGM  Original.
/*
/**********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	ctoe
/*
/*  DESCRIPTION
/*
/*	This is an interactive program to print in English,
/*	the meaning of a C declarator.
/*
/*	For example:  *volatile*(*(*const)[])()
/*	Would give:   const-pointer to array of pointer to function
/*		      returning pointer to volatile-pointer to TYPE
/*
/*	Control-D to terminate.
/*
/**********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

typedef unsigned int	uint;

/* ---------------------------------------------------------------------
/* Definitions
/* ------------------------------------------------------------------- */

/* Miscellaneous */

#define FALSE	0
#define TRUE	1

#define NXSTR(s)	#s	  /* Non-macro-expanding stringize */
#define STR(s)		NXSTR(s)  /* Macro-expanding stringize */

/* Program name */

#define PROGRAM_NAME	ctoe
#define __PROGRAM__	STR(PROGRAM_NAME)

/* Token type codes */

#define ERROR_TOKEN		(-2)
#define NULL_TOKEN		0
#define IDENTIFIER_TOKEN	1
#define STAR_TOKEN		2
#define LPAREN_TOKEN		3
#define RPAREN_TOKEN		4
#define LSQUARE_TOKEN		5
#define RSQUARE_TOKEN		6
#define CONST_TOKEN		7
#define VOLATILE_TOKEN		8

/* Derived declarator type codes */

#define NULL_TYPE		((uint)0x0000)
#define POINTER_TYPE		((uint)0x0001)
#define ARRAY_TYPE		((uint)0x0002)
#define FUNCTION_TYPE		((uint)0x0003)

#define TYPE_MASK		((uint)0x00FF)

/* Type qualifier codes */

#define NULL_TYPE_QUALIFIER	((uint)0x0000)
#define CONST_TYPE_QUALIFIER	((uint)0x0100)
#define VOLATILE_TYPE_QUALIFIER	((uint)0x0200)

#define TYPE_QUALIFIER_MASK	((uint)0xFF00)

/* Maxima */

#define DERIVED_DECLARATOR_STACK_SIZE	1024
#define DECLARATOR_IDENTIFIER_SIZE	 256
#define TOKEN_SPELLING_SIZE		 256

/* ---------------------------------------------------------------------
/* Global data definitions
/* ------------------------------------------------------------------- */

static uint	BaseTypeQualifier = 0;
static char	DeclaratorIdentifier [DECLARATOR_IDENTIFIER_SIZE] = "";

static uint	DerivedDeclaratorStack [DERIVED_DECLARATOR_STACK_SIZE];
static int	TopDerivedDeclaratorStack = -1;

static int	LookAheadToken = NULL_TOKEN;
static char	LookAheadTokenSpelling [TOKEN_SPELLING_SIZE];

static int	HitEof = 0;

/* ---------------------------------------------------------------------
/* Internal function declarations
/* ------------------------------------------------------------------- */

static void	greeting			(void);
static void	c_declarator_to_english		(void);
static int	declaration			(void);
static int	type_qualifiers			(uint *);
static int	declarator			(void);
static int	push_derived_declarator_stack	(uint);
static void	print_english_declarator	(void);
static int	match_token			(int expected_token_type);
static int	get_next_token			(char *spelling);
static void	skipline			(void);
static void	newline				(void);
static void	prompt				(void);
static void	warning				(const char *);
static void	error				(const char *);

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, const char *const argv[])
{
	greeting ();
	c_declarator_to_english ();
}

/* ---------------------------------------------------------------------
/* greeting
/* ------------------------------------------------------------------- */

void
greeting (void)
{
	printf (

	"**************************************"
	"**************************************\n"
        "*  Program:  " __PROGRAM__ "\n"
        "*  Compiled: " __DATE__ ", " __TIME__ "\n"
        "*  Copyright (c) Language Processors, Inc. 1989\n"
	"**************************************"
	"**************************************\n"
	"*  This program will print in English,\n"
	"*  the meaning of a simple ANSI C declarator.\n"
	"*\n"
	"*  For example:  *volatile*(*(*const)[])()\n"
	"*  Would give:   const-pointer to array of pointer to function\n"
	"*                returning pointer to volatile-pointer to TYPE\n"
	"*\n"
	"*  Enter <control-D> to terminate.\n"
	"**************************************"
	"**************************************\n"
	
	);
}

/* ---------------------------------------------------------------------
/* c_declarator_to_english
/* ------------------------------------------------------------------- */

static
void
c_declarator_to_english (void)
{
	while (1) {
		BaseTypeQualifier = NULL_TYPE_QUALIFIER;
		DeclaratorIdentifier [0] = '\0';
		TopDerivedDeclaratorStack = -1;
		prompt ();
		LookAheadToken = NULL_TOKEN;
		if (!match_token (LookAheadToken))
			continue;
		if (!declaration ())
			continue;
		if (HitEof)
			break;
		if (LookAheadToken == ERROR_TOKEN) {
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
int
declaration (void)
{
	if (!type_qualifiers (&BaseTypeQualifier))
		return (FALSE);
	if (!declarator ())
		return (FALSE);
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* type_qualifiers
/* ---------------------------------------------------------------------
/* type_qualifiers	::= [ 'const' | 'volatile' ] ...
/* ------------------------------------------------------------------- */

static
int
type_qualifiers (uint *btq)
{
	uint tq = NULL_TYPE_QUALIFIER;

	while (1) {
		if (LookAheadToken == CONST_TOKEN) {
			if (!match_token (CONST_TOKEN))
				return (FALSE);
			if (tq & CONST_TYPE_QUALIFIER)
				warning ("Duplicate type qualifier");
			else	tq |= CONST_TYPE_QUALIFIER;
		}
		else if (LookAheadToken == VOLATILE_TOKEN) {
			if (!match_token (VOLATILE_TOKEN))
				return (FALSE);
			if (tq & VOLATILE_TYPE_QUALIFIER)
				warning ("Duplicate type qualifier");
			else	tq |= VOLATILE_TYPE_QUALIFIER;
		}
		else	break;
	}
	*btq = tq;
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* declarator
/* ---------------------------------------------------------------------
/* declarator		::= { identifier
/*			    | '(' declarator  ')'
/*			    | [ '*' [ type_qualifiers ] ] declarator
/*			    | [ '&' [ type_qualifiers ] ] declarator
/*			    | [ class_name
/*			      '::*' [ type_qualifiers ] ] declarator
/*			    }
/*			    [ '[' constant_expression ']'
/*			    | '(' parameter_type_list ')'
/*			    ] ...
/* constant_expression	::= { }
/* parameter_type_list	::= { }
/* ------------------------------------------------------------------- */

static
int
declarator (void)
{
	uint tq;

	if (LookAheadToken == IDENTIFIER_TOKEN) {
		strcpy (DeclaratorIdentifier, LookAheadTokenSpelling);
		if (!match_token (IDENTIFIER_TOKEN))
			return (FALSE);
	}
	else if (LookAheadToken == LPAREN_TOKEN) {
		if (!match_token (LPAREN_TOKEN))
			return (FALSE);
		if (LookAheadToken != RPAREN_TOKEN) {	/* Hmmm */
			if (!declarator ())
				return (FALSE);
			if (!match_token (RPAREN_TOKEN))
				return (FALSE);
		}
		else {
			if (!push_derived_declarator_stack (FUNCTION_TYPE))
				return (FALSE);
			if (!match_token (RPAREN_TOKEN))
				return (FALSE);
		}
	}
	else if (LookAheadToken == STAR_TOKEN) {
		if (!match_token (STAR_TOKEN))
			return (FALSE);
		if (!type_qualifiers (&tq))
			return (FALSE);
		if (!declarator ())
			return (FALSE);
		if (!push_derived_declarator_stack (POINTER_TYPE | tq))
			return (FALSE);
	}
	while (1) {
		if (LookAheadToken == LSQUARE_TOKEN) {
			if (!match_token (LSQUARE_TOKEN))
				return (FALSE);
			if (!push_derived_declarator_stack (ARRAY_TYPE))
				return (FALSE);
			if (!match_token (RSQUARE_TOKEN))
				return (FALSE);
		}
		else if (LookAheadToken == LPAREN_TOKEN) {
			if (!match_token (LPAREN_TOKEN))
				return (FALSE);
			if (!push_derived_declarator_stack (FUNCTION_TYPE))
				return (FALSE);
			if (!match_token (RPAREN_TOKEN))
				return (FALSE);
		}
		else	break;
	}
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* push_derived_declarator_stack
/* ------------------------------------------------------------------- */

static
int
push_derived_declarator_stack (uint t)
{
	if (TopDerivedDeclaratorStack++ >= DERIVED_DECLARATOR_STACK_SIZE) {
		error ("Declaration stack overflow!");
		return (FALSE);
	}
	DerivedDeclaratorStack [TopDerivedDeclaratorStack] = t;
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* print_english_declarator
/* ------------------------------------------------------------------- */

static
void
print_english_declarator (void)
{
	register int n, t, tq;

	printf ("----> ");

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
	printf ("TYPE\n");
}

/* ---------------------------------------------------------------------
/* match_token
/* ------------------------------------------------------------------- */

static
int
match_token (int expected_token_type)
{
	if (LookAheadToken != expected_token_type) {
		error ("Unexpected token!");
		return (FALSE);
	}
	LookAheadToken = get_next_token (LookAheadTokenSpelling);
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* get_next_token
/* ------------------------------------------------------------------- */

static
int
get_next_token (char *spelling)
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
	else	return  (ERROR_TOKEN);
}

/* ---------------------------------------------------------------------
/* skipline
/* ------------------------------------------------------------------- */

static
void
skipline (void)
{
	while (getc(stdin) != '\n')
		;
}

/* ---------------------------------------------------------------------
/* newline
/* ------------------------------------------------------------------- */

static
void
newline (void)
{
	printf ("\n");
}

/* ---------------------------------------------------------------------
/* prompt
/* ------------------------------------------------------------------- */

static
void
prompt (void)
{
	printf (__PROGRAM__ "> ");
}

/* ---------------------------------------------------------------------
/* warning
/* ------------------------------------------------------------------- */

static
void
warning (const char *s)
{
	printf ("***** Warning: %s\n", s);
}

/* ---------------------------------------------------------------------
/* error
/* ------------------------------------------------------------------- */

static
void
error (const char *s)
{
	printf ("***** Error: %s\n", s);
	skipline ();
}

