/* LPI C++ Name Demangler */

/***********************************************************************/
/* This product is the property of Language Processors, Inc. and is    */
/* licensed pursuant to a written license agreement.  No portion of    */
/* this product may be reproduced without the written permission of    */
/* Language Processors, Inc. except pursuant to the license agreement. */
/***********************************************************************/

/* ---------------------------------------------------------------------- */
/*
/*  LPI EDIT HISTORY		    [ Update the VERSION__ string below ]
/*
/*  11.14.90  DGM  Original.
/*
/* ---------------------------------------------------------------------- */

/* ---------------------------------------------------------------------- */
/* Version and copyright stamp
/* ---------------------------------------------------------------------- */

static char VERSION__ [] =

"@(#)demangle.c  000  11/14/90  (c) 1990 Language Processors, Inc.";

/* ---------------------------------------------------------------------- */
/* Configuration
/* ---------------------------------------------------------------------- */

#undef	PLI_CALLABLE_DEMANGLER
#define	STAND_ALONE_DEMANGLER

/* ---------------------------------------------------------------------- */
/* Include files
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>

/* ---------------------------------------------------------------------- */
/* Local Type Definitions
/* ---------------------------------------------------------------------- */

typedef int	bool;

#if defined (PLI_CALLABLE_DEMANGLER)

typedef struct {
	short	size;
	char	text[1];
} PLI_STRING;

#endif

/* ---------------------------------------------------------------------- */
/* Local Macro Definitions
/* ---------------------------------------------------------------------- */

#define FALSE				((bool)0)
#define TRUE				(!(FALSE))

#define EOS				'\0'
#define NEWLINE				'\n'
#define SPACE				' '
#define UNDERSCORE			'_'

#define NULL_TQ				0x0000
#define CONST_TQ			0x0001
#define VOLATILE_TQ			0x0002
#define SIGNED_TQ			0x0004
#define UNSIGNED_TQ			0x0008

#define MAX_NAME_LENGTH			2048
#define MAX_DEMANGLED_NAME_LENGTH	3072

#define MAX_PARAMETERS			512

/* ---------------------------------------------------------------------- */
/* Local Macro Function Definitions
/* ---------------------------------------------------------------------- */

#define seq(a,b)			(strcmp ((a), (b)) == 0)
#define seqn(a,b,n)			(strncmp ((a), (b), (n)) == 0)

#define todigit(c)			((c) - '0')

#define is_name_start_char(c)		(isalpha((c)) || ((c) == UNDERSCORE))
#define is_name_char(c)			(isalnum((c)) || \
					((c) == UNDERSCORE) || \
					(AllowDollarInNames & ((c) == '$')))

#define is_function_code(c)		((c) == FunctionCode)
#define is_array_code(c)		((c) == ArrayCode)
#define is_member_pointer_code(c)	((c) == MemberPointerCode)
#define is_pointer_code(c)		((c) == PointerCode)
#define is_reference_code(c)		((c) == ReferenceCode)
#define is_name_qualifier_code(c)	((c) == NameQualifierCode)
#define is_class_name_code(c)		(isdigit (c) || \
					 is_name_qualifier_code (c))

#define is_void_code(c)			((c) == VoidCode)
#define is_char_code(c)			((c) == CharCode)
#define is_short_code(c)		((c) == ShortCode)
#define is_int_code(c)			((c) == IntCode)
#define is_long_code(c)			((c) == LongCode)
#define is_float_code(c)		((c) == FloatCode)
#define is_double_code(c)		((c) == DoubleCode)
#define is_long_double_code(c)		((c) == LongDoubleCode)
#define is_ellipsis_code(c)		((c) == EllipsisCode)

#define is_signed_code(c)		((c) == SignedCode)
#define is_unsigned_code(c)		((c) == UnsignedCode)
#define is_const_code(c)		((c) == ConstCode)
#define is_volatile_code(c)		((c) == VolatileCode)

#define is_type_repetition_code(c)	((c) == TypeRepetitionCode)
#define is_next_type_repetition_code(c)	((c) == NextTypeRepetitionCode)

/* ---------------------------------------------------------------------- */
/* Local Static Data
/* ---------------------------------------------------------------------- */

static bool	 AllowDollarInNames		= FALSE;
static bool	 OmitLeadingUnderScore		= TRUE;
static char	*MangledNameSeparator		= "__";
static short	 MangledNameSeparatorLength	= 2;

static char	 FunctionCode			= 'F';
static char	 ArrayCode			= 'A';
static char	 PointerCode			= 'P';
static char	 MemberPointerCode		= 'M';
static char	 ReferenceCode			= 'R';
static char	 NameQualifierCode		= 'Q';
static char	 VoidCode			= 'v';
static char	 CharCode			= 'c';
static char	 ShortCode			= 's';
static char	 IntCode			= 'i';
static char	 LongCode			= 'l';
static char	 FloatCode			= 'f';
static char	 DoubleCode			= 'd';
static char	 LongDoubleCode			= 'r';
static char	 EllipsisCode			= 'e';
static char	 SignedCode			= 'S';
static char	 UnsignedCode			= 'U';
static char	 ConstCode			= 'C';
static char	 VolatileCode			= 'V';
static char	 TypeRepetitionCode		= 'T';
static char	 NextTypeRepetitionCode		= 'N';

#if defined (STAND_ALONE_DEMANGLER)

static char	*ProgramName			= "demangle";
static char	*InputFileName			= NULL;
static FILE	*InputFile			= stdin;
static char	*OutputFileName			= NULL;
static FILE	*OutputFile			= stdout;
static char	*MapFileName			= NULL;
static FILE	*MapFile			= NULL;

#endif

/* ---------------------------------------------------------------------- */
/* Local Function Declarations
/* ---------------------------------------------------------------------- */

#if defined (PLI_CALLABLE_DEMANGLER)

extern void	    CXX_DEMANGLE_NAME	  (PLI_STRING *, PLI_STRING *);

#endif

#if defined (STAND_ALONE_DEMANGLER)

extern int	    main		   (int, char **);
static void	    usage		   ();
static void	    output_demangled_name  ();

#endif

static bool	    demangle_name	   (const char *, char *);
static void	    declarator_name	   (const char **, char **,
					    const char *, int);
static void	    function_signature	   (const char **, char **);
static void	    array_dimension	   (const char **, char **);
static void	    type_name		   (const char **, char **);
static int	    class_name		   (const char **, char **, char **);
static int	    unqualified_class_name (const char **, char **, char **);
static int	    number		   (const char **);
static const char * operator_name	   (const char *, int);
static const char * prefix_declarator_name (int);
static const char * base_type_name	   (int);

static void	    append_qualifier_name  (char **, const char *);
static void	    append_type_name	   (char **, const char *);
static void	    append_type_qualifiers (char **, int);
static void	    insert_type_qualifiers (char **, char *, int, bool);
static void	    insert_class_name	   (char **, char *, const char **);

static void	    append_character	   (char **, int);
static void	    append_string	   (char **, const char *);
static void	    append_string_max	   (char **, const char *, int);
static void	    insert_character	   (char **, char *, int);
static void	    insert_string	   (char **, char *, const char *);
static void	    insert_space	   (char **, char *, int);

/* ====================================================================== */
/* PL/I Call-able C++ name demangler routines
/* ====================================================================== */

#if defined (PLI_CALLABLE_DEMANGLER)

/* ---------------------------------------------------------------------- */
/* CXX_DEMANGLED_NAME
/* ---------------------------------------------------------------------- */

void
CXX_DEMANGLE_NAME (PLI_STRING *name, PLI_STRING *mangled_name)
{
	mangled_name->text [mangled_name->size] = EOS;
	demangle_name (mangled_name->text, mangled_name->text);
}

#endif

/* ====================================================================== */
/* Stand-alone C++ name demangler program routines
/* ====================================================================== */

#if defined (STAND_ALONE_DEMANGLER)

/* ---------------------------------------------------------------------- */
/* main
/* ---------------------------------------------------------------------- */

int
main (int argc, char *argv[])
{
	int	ac;
	char	**av, c;

	/* Process the command line arguments */

	for (ac = argc - 1, av = argv + 1 ; ac > 0 ; ac--, av++) {
		if ((*av)[0] != '-')
			continue;
		else if (seq (*av, "-in")) {
			ac--, av++;
			if (ac == 0) usage ();
			InputFileName = *av;
			if ((InputFile =
			     fopen (InputFileName, "r")) == NULL) {
				fprintf (stderr,
					 "%s: can't open input file "
					 "\"%s\"\n",
					 ProgramName, InputFileName);
				usage ();
			}
		}
		else if (seq (*av, "-out")) {
			ac--, av++;
			if (ac == 0) usage ();
			OutputFileName = *av;
			if ((OutputFile =
			     fopen (OutputFileName, "w")) == NULL) {
				fprintf (stderr,
					 "%s: can't open output file "
					 "\"%s\"\n",
					 ProgramName, OutputFileName);
				usage ();
			}
		}
		else if (seq (*av, "-map")) {
			ac--, av++;
			if (ac == 0) usage ();
			MapFileName = *av;
			if ((MapFile = fopen (MapFileName, "w")) == NULL) {
				fprintf (stderr,
					 "%s: can't open map file "
					 "\"%s\"\n", MapFileName);
				usage ();
			}
		}
		else if (seq (*av, "-dollar"))
			AllowDollarInNames = FALSE;
		else if (seq (*av, "-nlu"))
			OmitLeadingUnderScore = FALSE;
		else if (seq (*av, "-mns")) {
			ac--, av++;
			if (ac == 0) usage ();
			MangledNameSeparator = *av;
			MangledNameSeparatorLength = strlen (*av);
		}
		else if (seq (*av, "F")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			FunctionCode = **av;
		}
		else if (seq (*av, "A")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			ArrayCode = **av;
		}
		else if (seq (*av, "P")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			PointerCode = **av;
		}
		else if (seq (*av, "M")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			MemberPointerCode = **av;
		}
		else if (seq (*av, "R")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			ReferenceCode = **av;
		}
		else if (seq (*av, "Q")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			NameQualifierCode = **av;
		}
		else if (seq (*av, "v")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			VoidCode = **av;
		}
		else if (seq (*av, "c")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			CharCode = **av;
		}
		else if (seq (*av, "s")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			ShortCode = **av;
		}
		else if (seq (*av, "i")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			IntCode = **av;
		}
		else if (seq (*av, "l")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			LongCode = **av;
		}
		else if (seq (*av, "f")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			FloatCode = **av;
		}
		else if (seq (*av, "d")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			DoubleCode = **av;
		}
		else if (seq (*av, "r")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			LongDoubleCode = **av;
		}
		else if (seq (*av, "e")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			EllipsisCode = **av;
		}
		else if (seq (*av, "S")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			SignedCode = **av;
		}
		else if (seq (*av, "U")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			UnsignedCode = **av;
		}
		else if (seq (*av, "C")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			ConstCode = **av;
		}
		else if (seq (*av, "V")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			VolatileCode = **av;
		}
		else if (seq (*av, "T")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			TypeRepetitionCode = **av;
		}
		else if (seq (*av, "N")) {
			ac--, av++;
			if ((ac == 0) || ((*av)[1] != EOS)) usage ();
			NextTypeRepetitionCode = **av;
		}
		else	usage ();
	}

	if (ac > 0)
		usage ();

	/* Copy input to output while demangling names */

	while ((c = getc (InputFile)) != EOF) {
		if is_name_start_char (c) {
			ungetc (c, InputFile);
			output_demangled_name ();
		}
		else	putc (c, OutputFile);
	}

	if (InputFileName != NULL)	fclose (InputFile);
	if (OutputFileName != NULL)	fclose (OutputFile);
	if (MapFileName != NULL)	fclose (MapFile);

	exit (0);
}

/* ---------------------------------------------------------------------- */
/* usage
/* ---------------------------------------------------------------------- */

static
void
usage ()
{
	fprintf (stderr,
		 "usage: %s\n"
		 "       [ -version ]\n"
		 "       [ -in input_file ]\n"
		 "       [ -out output_file ]\n"
		 "       [ -map map_file ]\n"
		 "       [ -dollar ]\n"
		 "       [ -nlu ]\n"
		 "       [ -mns mangled_name_separator]\n"
		 "       [ -F function_character_code ]\n"
		 "       [ -A array_character_code ]\n"
		 "       [ -M member_pointer_character_code ]\n"
		 "       [ -R reference_character_code ]\n"
		 "       [ -Q name_qualifier_character_code ]\n"
		 "       [ -v void_character_code ]\n"
		 "       [ -c char_character_code ]\n"
		 "       [ -s short_character_code ]\n"
		 "       [ -i int_character_code ]\n"
		 "       [ -l long_character_code ]\n"
		 "       [ -f float_character_code ]\n"
		 "       [ -d double_character_code ]\n"
		 "       [ -r long_double_character_code ]\n"
		 "       [ -e ellipsis_character_code ]\n"
		 "       [ -S signed_character_code ]\n"
		 "       [ -U unsigned_character_code ]\n"
		 "       [ -C const_character_code ]\n"
		 "       [ -V volatile_character_code ]\n"
		 "       [ -T type_repetition_character_code ]\n"
		 "       [ -N next_type_repetition_character_code ]\n",
		 ProgramName);
	exit (1);
}

/* ---------------------------------------------------------------------- */
/* output_demangled_name
/* ---------------------------------------------------------------------- */

static
void
output_demangled_name ()
{
	char *p, c;
	char name [MAX_NAME_LENGTH];
	char demangled_name [MAX_DEMANGLED_NAME_LENGTH];

	name[0] = EOS; p = name;

	/* Read in the first character of the name */

	c = getc (InputFile);

	if (!is_name_start_char (c)) {
		ungetc (c, InputFile);
		return;
	}

	/* Read in the rest of the name */

	for (*p++ = c ; (c = getc (InputFile)) != EOF ; ) {
		if (!is_name_char (c)) {
			ungetc (c, InputFile);
			break;
		}
		*p++ = c;
	}

	*p = EOS;

	/* Demangle the name if necessary and output */

	if (demangle_name (name, demangled_name)) {
		if (MapFile != NULL)
			fprintf (MapFile, "%s  %s\n", name, demangled_name);
		fputs (demangled_name, OutputFile);
	}
	else	fputs (name, OutputFile);
}

/* ====================================================================== */
/* Common (stand-alone & PL/I callable) C++ name demangler routines
/* ====================================================================== */

#endif

/* ---------------------------------------------------------------------- */
/* demangle_name
/* ---------------------------------------------------------------------- */

static
bool
demangle_name (const char *mangled_name, char *name)
{
	int		 dname_length;
	const char	*dname;

	/* Omit any leading underscore */

	if (OmitLeadingUnderScore && (*mangled_name == UNDERSCORE))
		mangled_name++;

	/* Note where the declarator name begins */

	dname = mangled_name;

	/* Skip any leading underscores */

	while ((*mangled_name != EOS) && (*mangled_name == UNDERSCORE))
		mangled_name++;

	/*
	/* Now, look for the double-underscore mangled name seperator.
	/* Also, note the length of the declarator name.
	/**/

	for ( ; *mangled_name != EOS ; mangled_name++) {
		if (seqn (mangled_name,
			  MangledNameSeparator,
			  MangledNameSeparatorLength)) {
			dname_length = mangled_name - dname;
			mangled_name += MangledNameSeparatorLength;
			break;
		}
	}

	/* Is this a non-mangled name ? */

	if (*mangled_name == EOS)
		return (FALSE);

	/* Here, this is indeed a mangled name (well, probably) */

	declarator_name (&mangled_name, &name, dname, dname_length);

	/* Demangle the function signature if any */

	function_signature (&mangled_name, &name);

	return (TRUE);
}

/* ---------------------------------------------------------------------- */
/* declarator_name
/* ---------------------------------------------------------------------- */

static
void
declarator_name (const char **mangled_name, char **name,
		 const char *dname, int dname_length)
{
	char		*last_qualifier;
	const char	*o;

	/* Get any name qualifier */

	if (class_name (mangled_name, name, &last_qualifier) > 0)
		append_string (name, "::");

	/* See if this is a simple declarator name */

	if ((dname[0] != UNDERSCORE) ||
	    (dname[1] != UNDERSCORE)) {
		append_string_max (name, dname, dname_length);
		return;
	}

	/*
	/* Here, we have a special constructor, destructor,
	/* conversion, or operator function declarator name.
	/**/

	dname += 2; dname_length -= 2;

	if (seqn (dname, "ct", 2))
		append_qualifier_name (name, last_qualifier);
	else if (seqn (dname, "dt", 2)) {
		append_string (name, "~");
		append_qualifier_name (name, last_qualifier);
	}
	else if (seqn (dname, "op", 2)) {
		dname += 2;
		append_string (name, "operator ");
		type_name (&dname, name);
	}
	else if ((o = operator_name (dname, dname_length)) != NULL) {
		append_string (name, "operator");
		append_string (name, o);
	}
}

/* ---------------------------------------------------------------------- */
/* class_name
/* ---------------------------------------------------------------------- */

static
int
class_name (const char **mangled_name, char **name, char **last_qualifier)
{
	register int i, n, m;

	if (last_qualifier != NULL)
		*last_qualifier = NULL;

	if (!is_name_qualifier_code (**mangled_name))
		return (unqualified_class_name
			(mangled_name, name, last_qualifier));

	if (!isdigit ((*mangled_name)[1]))
		return (FALSE);

	(*mangled_name)++;

	for (n = 0, i = todigit (*(*mangled_name)++) ; i > 0 ; i--) {
		if (last_qualifier != NULL)
			*last_qualifier = *name;
		m = unqualified_class_name (mangled_name, name, NULL);
		if (m > 0) {
			n += m;
			if (i > 1) {
				n += 2;
				append_string (name, "::");
			}
		}
	}

	return (n);
}

/* ---------------------------------------------------------------------- */
/* unqualified_class_name
/* ---------------------------------------------------------------------- */

static
int
unqualified_class_name (const char **mangled_name, char **name, char **qualifier)
{
	int n;

	n = number (mangled_name);
	if (qualifier != NULL)
		*qualifier = *name;
	append_string_max (name, *mangled_name, n);
	(*mangled_name) += n;
	return (n);
}
/* ---------------------------------------------------------------------- */
/* number
/* ---------------------------------------------------------------------- */

static
int
number (const char **mangled_name)
{
	register int n;

	for (n = 0 ; isdigit (**mangled_name) ; (*mangled_name)++)
		n = (n * 10) + todigit (**mangled_name);
	return (n);
}

/* ---------------------------------------------------------------------- */
/* function_signature
/* ---------------------------------------------------------------------- */

static
void
function_signature (const char **mangled_name, char **name)
{
	short		 pi [MAX_PARAMETERS];
	register char	 c;
	const char	*start;
	register int	 i, n, tq;
	static short	 level = 0;

	level++;

	if (!is_function_code (**mangled_name)) {
		if (!is_const_code (**mangled_name)) {
			level--;
			return;
		}
		else if (!is_function_code ((*mangled_name)[1])) {
			level--;
			return;
		}
		else if (level == 1)
			tq = CONST_TQ;
		(*mangled_name)++;
	}
	else	tq = NULL_TQ;

	(*mangled_name)++;

	append_string (name, " (");

	for (start = *name, n = 0 ; (c = **mangled_name) != EOS ; n++) {
		if (c == UNDERSCORE)
			break;
		if (n > 0)
			append_string (name, ", ");
		if (n <= MAX_PARAMETERS)
			pi[n] = *name - start;
		if (is_type_repetition_code (c)) {
			(*mangled_name)++;
			i = number (mangled_name);
			if ((i > 0) && (i <= n) && (i <= MAX_PARAMETERS))
				append_type_name (name, start + pi[i-1]);
		}
		else if (is_next_type_repetition_code (c)) {
			(*mangled_name)++;
			if (isdigit (**mangled_name)) {
				(*mangled_name)++;
				i = number (mangled_name);
				if ((i > 0) && (i <= n) &&
				    (i <= MAX_PARAMETERS)) {
					for (i-- ; i >= 0 ; i--) {
						append_type_name
						(name, start + pi[i]);
						if (i > 0)
							append_string
							(name, ", ");
					}
				}
			}
		}
		else	type_name (mangled_name, name);
	}

	append_character (name, ')');
	append_type_qualifiers (name, tq);

	level--;
}

/* -----------------------------------------------------------------------
/* type_name
/* ---------------------------------------------------------------------- */

static
void
type_name (const char **mangled_name, char **name)
{
	char		c, *start = *name;
	const char	*p;
	bool		last_was_prefix_declarator = FALSE;
	bool		done = FALSE;
	int		tq = NULL_TQ;

	for ( ; ((c = **mangled_name) != EOS) ; (*mangled_name)++) {
		if (done)
			break;
		if (c == UNDERSCORE)
			break;
		else if (is_const_code (c))
			tq |= CONST_TQ;
		else if (is_volatile_code (c))
			tq |= VOLATILE_TQ;
		else if (is_signed_code (c))
			tq |= SIGNED_TQ;
		else if (is_unsigned_code (c))
			tq |= UNSIGNED_TQ;
		else if ((p = prefix_declarator_name (c)) != NULL) {
			insert_type_qualifiers (name, start, tq, FALSE);
			insert_string (name, start, p);
			if (is_member_pointer_code (c)) {
				(*mangled_name)++;
				insert_class_name (name, start, mangled_name);
				(*mangled_name)--;
			}
			tq = NULL_TQ;
			last_was_prefix_declarator = TRUE;
		}
		else if (is_function_code (c) || is_array_code (c)) {
			if (last_was_prefix_declarator) {
				insert_character (name, start, '(');
				append_character (name, ')');
				last_was_prefix_declarator = FALSE;
			}
			if (is_function_code (c))
				function_signature (mangled_name, name);
			else	array_dimension (mangled_name, name);
			append_type_qualifiers (name, tq);
			tq = NULL_TQ;
		}
		else if ((p = base_type_name (c)) != NULL) {
			if ((*start != EOS) && (*start != SPACE))
				insert_character (name, start, SPACE);
			insert_string (name, start, p);
			insert_type_qualifiers (name, start, tq, TRUE);
			done = TRUE;
		}
		else if (is_class_name_code (c)) {
			if (*start != EOS)
				insert_character (name, start, SPACE);
			insert_class_name (name, start, mangled_name);
			insert_type_qualifiers (name, start, tq, TRUE);
			done = TRUE;
		}
		else	done = TRUE;
	}
}

/* -----------------------------------------------------------------------
/* array_dimension
/* ---------------------------------------------------------------------- */

static
void
array_dimension (const char **mangled_name, char **name)
{
	if (!is_array_code (**mangled_name))
		return;

	append_character (name, '[');

	for ((*mangled_name)++ ; isdigit (**mangled_name) ; (*mangled_name)++)
		append_character (name, **mangled_name);

	append_character (name, ']');
}

/* ---------------------------------------------------------------------- */
/* base_type_name
/* ---------------------------------------------------------------------- */

static
const char *
base_type_name (int c)
{
	if	(is_void_code (c))		return ("void");
	else if	(is_char_code (c))		return ("char");
	else if	(is_short_code (c))		return ("short");
	else if	(is_int_code (c))		return ("int");
	else if	(is_long_code (c))		return ("long");
	else if	(is_float_code (c))		return ("float");
	else if	(is_double_code (c))		return ("double");
	else if	(is_long_double_code (c))	return ("long double");
	else if	(is_ellipsis_code (c))		return ("...");
	else					return (NULL);
}

/* ---------------------------------------------------------------------- */
/* prefix_declartor_name
/* ---------------------------------------------------------------------- */

static
const char *
prefix_declarator_name (int c)
{
	if	(is_pointer_code (c))		return ("*");
	else if	(is_member_pointer_code (c))	return ("::*");
	else if	(is_reference_code (c))		return ("&");
	else					return (NULL);
}

/* ---------------------------------------------------------------------- */
/* operator_name
/* ---------------------------------------------------------------------- */

static
const char *
operator_name (const char *mangled_name, int length)
{
	struct mangled_name_map {
		const char *mangled_name;
		const char *unmangled_name;
	};

	static const struct mangled_name_map operator_name [] = {

	  {	"vc"	,	"[]"		}
	, {	"cl"	,	"()"		}
	, {	"rm"	,	"->*"		}
	, {	"pl"	,	"+"		}
	, {	"pp"	,	"++"		}
	, {	"mi"	,	"-"		}
	, {	"mm"	,	"--"		}
	, {	"ml"	,	"*"		}
	, {	"dv"	,	"/"		}
	, {	"md"	,	"%"		}
	, {	"ad"	,	"&"		}
	, {	"or"	,	"|"		}
	, {	"er"	,	"^"		}
	, {	"co"	,	"~"		}
	, {	"nt"	,	"!"		}
	, {	"ls"	,	"<<"		}
	, {	"rs"	,	">>"		}
	, {	"aa"	,	"&&"		}
	, {	"oo"	,	"||"		}
	, {	"lt"	,	"<"		}
	, {	"le"	,	"<="		}
	, {	"gt"	,	">"		}
	, {	"ge"	,	">="		}
	, {	"eq"	,	"=="		}
	, {	"ne"	,	"!="		}
	, {	"as"	,	"="		}
	, {	"apl"	,	"+="		}
	, {	"ami"	,	"-="		}
	, {	"amu"	,	"*="		}
	, {	"adv"	,	"/="		}
	, {	"amd"	,	"%="		}
	, {	"aad"	,	"&="		}
	, {	"aor"	,	"|="		}
	, {	"aer"	,	"^="		}
	, {	"als"	,	"<<="		}
	, {	"ars"	,	">>="		}
	, {	"nw"	,	"new"		}
	, {	"dl"	,	"delete"	}
	, {	NULL	,	""		} };

	register int i;

	for (i = 0 ; operator_name[i].mangled_name != NULL ; i++) {
		if (seqn (operator_name[i].mangled_name, mangled_name, length))
			return (operator_name[i].unmangled_name);
	}
	return (NULL);
}

/* ---------------------------------------------------------------------- */
/* append_type_name
/* ---------------------------------------------------------------------- */

static
void
append_type_name (char **string, const char *s)
{
	int level = 0;

	if ((string == NULL) || (*string == NULL))
		return;
	if ((s == NULL) || (*s == EOS))
		return;
	while ((*s != EOS) && ((*s != ',') || (level > 0))) {
		if (*s == '(')
			level++;
		else if (*s == ')')
			level--;
		*(*string)++ = *s++;
	}
	**string = EOS;
}

/* ---------------------------------------------------------------------- */
/* append_qualifier_name
/* ---------------------------------------------------------------------- */

static
void
append_qualifier_name (char **string, const char *s)
{
	if ((string == NULL) || (*string == NULL))
		return;
	if ((s == NULL) || (*s == EOS))
		return;
	while ((*s != EOS) && (*s != ':'))
		*(*string)++ = *s++;
	**string = EOS;
}

/* ---------------------------------------------------------------------- */
/* append_type_qualifiers
/* ---------------------------------------------------------------------- */

static
void
append_type_qualifiers (char **name, int tq)
{
	if (tq & VOLATILE_TQ)	append_string (name, " volatile");
	if (tq & CONST_TQ)	append_string (name, " const");
}

/* ---------------------------------------------------------------------- */
/* insert_type_qualifiers
/* ---------------------------------------------------------------------- */

static
void
insert_type_qualifiers (char **name, char *start, int tq, bool space)
{
	if ((tq != NULL_TQ) && space) {
		insert_character (name, start, SPACE);
		space = FALSE;
	}
	if (tq & SIGNED_TQ) {
		insert_string (name, start, "signed");
		space = TRUE;
	}
	else if (tq & UNSIGNED_TQ) {
		insert_string (name, start, "unsigned");
		space = TRUE;
	}
	if (tq & VOLATILE_TQ) {
		if (space)
			insert_character (name, start, SPACE);
		insert_string (name, start, "volatile");
		space = TRUE;
	}
	if (tq & CONST_TQ) {
		if (space)
			insert_character (name, start, SPACE);
		insert_string (name, start, "const");
	}
}

/* ---------------------------------------------------------------------- */
/* insert_class_name
/* ---------------------------------------------------------------------- */

static
void
insert_class_name (char **name, char *start, const char **mangled_name)
{
	const char	*mn = *mangled_name;
	char		*s = start;
	int		length;
	char		save_character;

	save_character = *start;
	length = class_name (&mn, NULL, NULL);
	insert_space (name, start, length);
	(void) class_name (mangled_name, &s, NULL);
	*s = save_character;
}

/* -----------------------------------------------------------------------
/* append_character
/* ---------------------------------------------------------------------- */

static
void
append_character (char **string, int c)
{
	if ((string == NULL) || (*string == NULL))
		return;
	*(*string)++ = (char)c;
	**string = EOS;
}

/* -----------------------------------------------------------------------
/* append_string
/*
/* Copy the (null terminated) string represented by "b" to the string
/* represented by "*a" and update "*a" to point to the end of the string.
/* ---------------------------------------------------------------------- */

static
void
append_string (char **string, const char *s)
{
	if ((string == NULL) || (*string == NULL))
		return;
	if ((s == NULL) || (*s == EOS))
		return;
	while (*s != EOS)
		*(*string)++ = *s++;
	**string = EOS;
}

/* ---------------------------------------------------------------------- */
/* append_string_max
/* ---------------------------------------------------------------------- */

static
void
append_string_max (char **string, const char *s, int n)
{
	if ((string == NULL) || (*string == NULL))
		return;
	if ((s == NULL) || (*s == EOS) || (n <= 0))
		return;
	while ((*s != EOS) && (n-- > 0))
		*(*string)++ = *s++;
	**string = EOS;
}

/* ---------------------------------------------------------------------- */
/* insert_string
/* ---------------------------------------------------------------------- */

static
void
insert_string (char **string, char *start, const char *insert)
{
	const char	*p;
	int	 	length;

	if ((string == NULL) || (*string == NULL))
		return;
	if ((start == NULL) || (insert == NULL) || (*insert == EOS))
		return;
	length = strlen (insert);
	insert_space (string, start, length);
	for (p = insert ; *p != EOS ; p++)
		*start++ = *p;
}

/* ---------------------------------------------------------------------- */
/* insert_character
/* ---------------------------------------------------------------------- */

static
void
insert_character (char **string, char *start, int insert)
{
	char *end, *p;

	if ((string == NULL) || (*string == NULL))
		return;
	if (start == NULL)
		return;
	insert_space (string, start, 1);
	*start = (char)insert;
}

/* ---------------------------------------------------------------------- */
/* insert_space
/* ---------------------------------------------------------------------- */

static
void
insert_space (char **string, char *start, int length)
{
	char		*end, *p;
	const char	*pi;
	int	 	n;

	if ((string == NULL) || (*string == NULL))
		return;
	if ((start == NULL) || (length <= 0))
		return;
	for (end = start + strlen (start), p = end ; p >= start ; p--)
		*(p + length) = *p;
	(*string) += length;
}

