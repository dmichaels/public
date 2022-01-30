/* etoc - Map simple ANSI C declarators from English to C */

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
/*  02.21.90  DGM  Added C++ support, i.e. references and member-pointers.
/*  01.09.89  DGM  Original.
/*
/**********************************************************************/

/***********************************************************************
/*
/*  USAGE
/*
/*	etoc
/*
/*  DESCRIPTION
/*
/*	This is an interactive program to print a C declarator given an
/*	English-like description consisting exclusively of the letters
/*	(case insensitive):
/*
/*		"P" or "p"    meaning    "pointer-to ..."
/*		"R" or "r"    meaning    "reference-to ..."
/*		"M" or "m"    meaning    "member-pointer-to ..."
/*		"A" or "a"    meaning    "array-of ..."
/*		"F" or "f"    meaning    "function-returning ..."
/*		"C" or "c"    meaning    "const-qualified ..."
/*		"V" or "v"    meaning    "volatile-qualified ..."
/*
/*	For example:  cpapfpvp
/*	Would give:   TYPE *volatile*(*(*const NAME)[])()
/*
/*	Control-Z to terminate.
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

#define FALSE		0
#define TRUE		1

#define NXSTR(s)	#s	  /* Non-macro-expanding stringize */
#define STR(s)		NXSTR(s)  /* Macro-expanding stringize */

/* Program name */

#define PROGRAM_NAME	etoc
#define __PROGRAM__	STR(PROGRAM_NAME)

/* Derived declarator type codes */

#define NULL_TYPE		((uint)0x0000)
#define POINTER_TYPE		((uint)0x0001)
#define ARRAY_TYPE		((uint)0x0002)
#define FUNCTION_TYPE		((uint)0x0003)
#define REFERENCE_TYPE		((uint)0x0004)
#define MEMBER_POINTER_TYPE	((uint)0x0005)

#define TYPE_MASK		((uint)0x00FF)

/* Type qualifier codes */

#define NULL_TYPE_QUALIFIER	((uint)0x0000)
#define CONST_TYPE_QUALIFIER	((uint)0x0100)
#define VOLATILE_TYPE_QUALIFIER	((uint)0x0200)

#define TYPE_QUALIFIER_MASK	((uint)0xFF00)

/* Maxima */

#define DERIVED_DECLARATOR_STACK_SIZE	1024
#define DECLARATOR_IDENTIFIER_SIZE	 256

/* ---------------------------------------------------------------------
/* Global data definitions
/* ------------------------------------------------------------------- */

static uint	BaseTypeQualifier = 0;

static uint	DerivedDeclaratorStack [DERIVED_DECLARATOR_STACK_SIZE];
static int	TopDerivedDeclaratorStack = -1;

static int	HitEof = 0;

/* ---------------------------------------------------------------------
/* Internal function declarations
/* ------------------------------------------------------------------- */

static void	greeting			(void);
static void	help				(void);
static void	english_to_c_declarator		(void);
static int	get_english_declarator		(void);
static void	print_c_declarator		(void);
static int	push_derived_declarator_stack	(uint);
static void	skipline			(void);
static void	newline				(void);
static void	prompt				(void);
static void	warning				(const char *);
static void	error				(const char *);

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, const char *const argv [])
{
	greeting ();
	english_to_c_declarator ();
}

/* ---------------------------------------------------------------------
/* greeting
/* ------------------------------------------------------------------- */

static
void
greeting (void)
{
	printf (

	"**************************************"
	"**************************************\n"
        "*  Program:  " __PROGRAM__ "\n"
        "*  Compiled: " __DATE__ ", " __TIME__ "\n"
        "*  Copyright (c) Language Processors, Inc. 1990\n"

	);

	help ();
}

/* ---------------------------------------------------------------------
/* help
/* ------------------------------------------------------------------- */

static
void
help (void)
{
	printf (

	"**************************************"
	"**************************************\n"
	"*  This program will print an ANSI C declarator, given a simple\n"
	"*  English-like description consisting exclusively of the letters:\n"
	"*\n"
	"*  \t\"P\" or \"p\"    meaning    \"pointer-to ...\"\n"
	"*  \t\"M\" or \"m\"    meaning    \"member-pointer-to ...\"\n"
	"*  \t\"R\" or \"r\"    meaning    \"reference-to ...\"\n"
	"*  \t\"A\" or \"a\"    meaning    \"array-of ...\"\n"
	"*  \t\"F\" or \"f\"    meaning    \"function-returning ...\"\n"
	"*  \t\"C\" or \"c\"    meaning    \"const-qualified ...\"\n"
	"*  \t\"V\" or \"v\"    meaning    \"volatile-qualified ...\"\n"
	"*\n"
	"*  For example:  cpapfpvp\n"
	"*  Would give:   TYPE *volatile *(*(*const NAME)[])()\n"
	"*\n"
	"*  Enter <control-D> to terminate.  Enter \"H\" or \"h\" for help.\n"
	"**************************************"
	"**************************************\n"

	);
}

/* ---------------------------------------------------------------------
/* english_to_declarator
/* ------------------------------------------------------------------- */

static
void
english_to_c_declarator (void)
{
	while (!HitEof) {
		BaseTypeQualifier = NULL_TYPE_QUALIFIER;
		TopDerivedDeclaratorStack = -1;
		prompt ();
		if (!get_english_declarator ())
			continue;
		print_c_declarator ();
	}
	newline ();
	exit (0);
}

/* ---------------------------------------------------------------------
/* get_english_declarator
/* ------------------------------------------------------------------- */

static
int
get_english_declarator (void)
{
	register int	c;
	register uint	tq = NULL_TYPE_QUALIFIER;
	register uint	dt;

	while ((c = getc(stdin)) != '\n') {
		if ((c == ' ') || (c == '\t'))
			continue;
		if ((c == 'p') || (c == 'P')) {
			dt = POINTER_TYPE | tq;
			if (!push_derived_declarator_stack (dt)) {
				skipline ();
				return (FALSE);
			}
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'm') || (c == 'M')) {
			dt = MEMBER_POINTER_TYPE | tq;
			if (!push_derived_declarator_stack (dt)) {
				skipline ();
				return (FALSE);
			}
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'r') || (c == 'R')) {
			dt = REFERENCE_TYPE | tq;
			if (!push_derived_declarator_stack (dt)) {
				skipline ();
				return (FALSE);
			}
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'a') || (c == 'A')) {
			dt = ARRAY_TYPE;
			if (!push_derived_declarator_stack (dt)) {
				skipline ();
				return (FALSE);
			}
			BaseTypeQualifier |= tq;
			tq = NULL_TYPE_QUALIFIER;
		}
		else if ((c == 'f') || (c == 'F')) {
			dt = FUNCTION_TYPE;
			if (!push_derived_declarator_stack (dt)) {
				skipline();
				return (FALSE);
			}
		}
		else if ((c == 'c') || (c == 'C'))
			tq |= CONST_TYPE_QUALIFIER;
		else if ((c == 'v') || (c == 'V'))
			tq |= VOLATILE_TYPE_QUALIFIER;
		else if ((c == 'h') || (c == 'H')) {
			help ();
			skipline ();
			return (FALSE);
		}
		else if (c == EOF) {
			HitEof = 1;
			return (FALSE);
		}
		else {
			error ("Use only letters: "
			       "'p', 'r', 'm', 'a', 'f', 'v', or 'c'"
			       " ('h' for help)");
			skipline ();
			return (FALSE);
		}
	}
	BaseTypeQualifier |= tq;
	return (TRUE);
}

/* ---------------------------------------------------------------------
/* print_c_declarator
/* ------------------------------------------------------------------- */

static
void
print_c_declarator (void)
{
	register int n, t, tq, lastt;
	char tmp [1024] = "", s [1024] = "NAME";

	for (t = NULL_TYPE, n = 0 ; n <= TopDerivedDeclaratorStack ; n++) {
		lastt = t;
		t  = DerivedDeclaratorStack [n] & TYPE_MASK;
		tq = DerivedDeclaratorStack [n] & TYPE_QUALIFIER_MASK;
		if (t == POINTER_TYPE) {
			strcpy (tmp, s);
			strcpy (s, "*");
			if (tq & CONST_TYPE_QUALIFIER)
				strcat (s, "const ");
			if (tq & VOLATILE_TYPE_QUALIFIER)
				strcat (s, "volatile ");
			strcat (s, tmp);
			continue;
		}
		else if (t == MEMBER_POINTER_TYPE) {
			strcpy (tmp, s);
			strcpy (s, "CLASS::*");
			if (tq & CONST_TYPE_QUALIFIER)
				strcat (s, "const ");
			if (tq & VOLATILE_TYPE_QUALIFIER)
				strcat (s, "volatile ");
			strcat (s, tmp);
			continue;
		}
		else if (t == REFERENCE_TYPE) {
			strcpy (tmp, s);
			strcpy (s, "&");
			if (tq & CONST_TYPE_QUALIFIER)
				strcat (s, "const ");
			if (tq & VOLATILE_TYPE_QUALIFIER)
				strcat (s, "volatile ");
			strcat (s, tmp);
			continue;
		}
		if ((lastt == POINTER_TYPE) ||
		    (lastt == MEMBER_POINTER_TYPE) ||
		    (lastt == REFERENCE_TYPE)) {
			strcpy (tmp, s);
			strcpy (s, "(");
			strcat (s, tmp);
			strcat (s, ")");
		}
		if (t == FUNCTION_TYPE) {
			if (lastt == FUNCTION_TYPE)
				warning ("Function-returning-function"
					 " is an illegal construct.");
			else if (lastt == ARRAY_TYPE)
				warning ("Array-of-function"
					 " is an illegal construct.");
			strcat (s, "()");
		}
		else /* t == ARRAY_TYPE */ {
			if (lastt == FUNCTION_TYPE)
				warning ("Function-returning-array"
					 " is an illegal construct.");
			strcat (s, "[]");
		}
	}

	printf ("----> ");

	if (BaseTypeQualifier & CONST_TYPE_QUALIFIER)
		printf ("const ");
	if (BaseTypeQualifier & VOLATILE_TYPE_QUALIFIER)
		printf ("volatile ");

	printf ("TYPE %s\n", s);
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
/* skipline
/* ------------------------------------------------------------------- */

static
void
skipline (void)
{
	register int c;

	while (((c = getc (stdin)) != '\n') && (c != EOF))
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
}

