/* LPI-DEBUG M68 dismain.c - Standalone MC680x0/68881 disassembler driver */

/**********************************************************************/
/* This product is the property of Language Processors, Inc.          */
/* and is licensed pursuant to a written license agreement.           */
/* No portion of this product may be reproduced without the written   */
/* permission of Language Processors, Inc. except pursuant to the     */
/* license agreement.                                                 */
/**********************************************************************/

/**********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  03.15.88  DGM  Removed FINADR stub; real one in sym.c.  Now prints
 *		   symbols where necessary, and the a.out header.
 *  03.09.88  DGM  Added FINADR stub.
 *  12.10.87  DGM  Original.
 *
 **********************************************************************/

/* ---------------------------------------------------------------------
**
** Usage:
**
**	dis [-a] [-h] [-s] [object_file_name]
**
** Description:
**
**	Prints to the standard output, the given MC680x0/68881 object
**	file in disassembled format.
**
**	If the "-h" flag is given then the object file header will
**	not be printed out.
**
**	If the "-a" flag is given then the disassembler will NOT try
**	to determine whether or not a zero word in the code is for
**	(long word) alignment purposes.
**
**	If the "-s" flag is given then no symbol mapping will be done.
**
**	If no object file name is given then the default "a.out" will
**	be used.
**
** ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
** Include files
** ------------------------------------------------------------------- */

#include <stdio.h>
#include <a.out.h>

/* ---------------------------------------------------------------------
** Definitions
** ------------------------------------------------------------------- */

#define	MAX_INSTRUCTION_WORDS	12
#define	MAX_INSTRUCTION_BYTES	(MAX_INSTRUCTION_WORDS * 2)

/* ---------------------------------------------------------------------
** Type definitions
** ------------------------------------------------------------------- */

typedef struct {
	short	cv_length;
	char	cv_string[256];
} CHAR_VAR_256;

/* ---------------------------------------------------------------------
** Local static data
** ------------------------------------------------------------------- */

static char *ThisProgram = "dis";
static char *DefaultFile = "a.out";

/* ---------------------------------------------------------------------
** readcode
** ------------------------------------------------------------------- */

static
int
readcode (file, from, buffer, nbytes)
FILE		*file;
unsigned long	from;
char    	*buffer;
int	   	nbytes;
{
	register int n;

	for (n = 0 ; n < nbytes ; n++)
		buffer[n] = '\0';
	if (fseek (file, from, 0) == -1)
		return (0);
	if ((n = fread(buffer, 1, nbytes, file)) != nbytes)
		return (0);
	return (nbytes);
}

/* ---------------------------------------------------------------------
** print_cv_string
** ------------------------------------------------------------------- */

static
void
print_cv_string (f,s)
FILE *f;
CHAR_VAR_256 *s;
{
	register int n;

	for (n = 0 ; n < s->cv_length ; n++)
	    fprintf (f,"%c", s->cv_string[n]);
}

/* ---------------------------------------------------------------------
** link_instruction
** ------------------------------------------------------------------- */

static
int
link_instruction (w)
unsigned short *w;
{
	return (((*w & 0xFFF8) == 0x4E50) || ((*w & 0xFFF8) == 0x4408));
}

/* ---------------------------------------------------------------------
** rts_instruction
** ------------------------------------------------------------------- */

static
int
rts_instruction (w)
unsigned short *w;
{
	return (*w == 0x4E75);
}

/* ---------------------------------------------------------------------
** usage
** ------------------------------------------------------------------- */

static
void
usage ()
{
	fprintf (stderr, "usage: %s [-a] [-h] [-s] object_file\n", ThisProgram);
	exit (1);
}

/* ---------------------------------------------------------------------
** main
** ------------------------------------------------------------------- */

main (argc, argv)
int	argc;
char	*argv[];
{
	int			initsym ();
	char			*tsymname ();

	int			ac, do_alignment, do_header, do_symbols;
	int			last_was_rts;
	char			*name, *last_name, **av;
	unsigned long		end, offset;
	register unsigned long	address, core_address, last_name_address;
	register short		n, nbytes, nwords;
	unsigned short		buffer[MAX_INSTRUCTION_WORDS];
	CHAR_VAR_256		string;
	FILE			*file;
	struct exec		header;

	/* Set defaults etc. */

	do_alignment = 1;
	do_header = 1;
	do_symbols = 1;
	last_was_rts = 0;
	last_name = NULL;

	/* Check the usage */

	ac = argc;
	av = argv;

	for (ac--, av++ ; ac > 0 ; ac--, av++) {
		if ((*av)[0] == '-') {
			switch ((*av)[1]) {
			case 'a':
				do_alignment = 0;
                                break;
			case 'h':
				do_header = 0;
                                break;
                        case 's':
				do_symbols = 0;
                                break;
                        default:
				printf ("%s: bad flag (%s)\n",ThisProgram,*av);
                                usage ();
                        }
                }
                else    break;
        }

        if (ac == 0) {
		ac = 1;
                av[0] = DefaultFile;
	}

	if (ac < 1)
		usage ();

	if ((file = fopen (av[0], "r")) == NULL) {
		fprintf (stderr, "%s: can't open \"%s\"\n", ThisProgram, av[0]);
		exit (2);
	}

	/* Initialize the symbol table */

	if (do_symbols && !initsym(file)) {
		fprintf (stderr, "%s: can't read symbol table from \"%s\"\n",
			 ThisProgram, av[0]);
		exit (3);
	}

	rewind (file);

	/* Read the header */

	if ((nbytes = fread ((char *)&header, 1, sizeof(header), file))
	    != sizeof(header)) {
		fprintf (stderr, "%s: can't read header of \"%s\"\n",
			ThisProgram, av[0]);
		exit (5);
	}

	/* Determine the start of text in the object file */

	if ((address = N_TXTOFF(header)) == 0)
		address = sizeof(struct exec);

	/* Print the header */

	printf ("\n-----------------------------");
	printf ("\nLPI MC68020/68881 Disassembly");
	printf ("\n-----------------------------\n\n");

	printf ("File:                  \"%s\"\n", av[0]);

	if (do_header) {
		printf ("Machine:      %8d (0x%x)\n",
			header.a_machtype, header.a_machtype);
		printf ("Magic:        %8d (0x%x)\n",
			header.a_magic, header.a_magic);
		printf ("Text size:    %8d (0x%x)\n",
			header.a_text, header.a_text);
		printf ("Data size:    %8d (0x%x)\n",
			header.a_data, header.a_data);
		printf ("Bss size:     %8d (0x%x)\n",
			header.a_bss, header.a_bss);
		printf ("Text address: %8d (0x%x)\n",
			N_TXTADDR(header), N_TXTADDR(header));
		printf ("Data address: %8d (0x%x)\n",
			N_DATADDR(header), N_DATADDR(header));
		printf ("Bss address:  %8d (0x%x)\n",
			N_BSSADDR(header), N_BSSADDR(header));
		printf ("Symbols:      %8d (0x%x)\n",
			header.a_syms/sizeof(struct nlist),
			header.a_syms/sizeof(struct nlist));
		printf ("Entry:        %8d (0x%lx)\n",
			header.a_entry, header.a_entry);
	}

	printf ("\n");

	/* Determine the load offset */

	offset = header.a_entry - sizeof(struct exec);

	/* Disassemble! */

	for (end = address + header.a_text; address < end; address += nbytes) {

		/* Read MAX_INSTRUCTION_BYTES bytes into the buffer */

		readcode (file, address, (char *)buffer, MAX_INSTRUCTION_BYTES);

		/*
		 * Wicked hackola to see if the zero word at this address
		 * is just to long-word align the start of the next routine.
		 */

		core_address = address + offset;

		if (do_alignment &&
		    ((core_address % 2) == 0) &&
		    ((core_address % 4) != 0) &&
		    (buffer[0] == 0x0000) &&
		    (last_was_rts || link_instruction(&buffer[1]))) {
			nbytes = 2;
			strcpy (string.cv_string, "--------- ALIGNMENT");
			string.cv_length = strlen("--------- ALIGNMENT");
		}

		/* Disassemble */

		else if ((nbytes = DIS (buffer, &string)) <= 0)
			nbytes = 1;

		nwords = (nbytes / 2) + (nbytes % 2);

		/* Print the symbol name corresponding to this address */

		if (do_symbols && (name = tsymname(core_address)) != NULL) {
			last_name = name;
			last_name_address = core_address;
			printf ("\n%s:\n\n", name);
		}
		else if (link_instruction(buffer)) {
			if (last_name != NULL) {
				printf ("\n%s + 0x%lx:\n", last_name,
					core_address - last_name_address);
			}
			printf ("\n");
		}

		/* Print this address */

		printf ("%08lX:", (unsigned long)(core_address));

		/* Print the instruction (words and disassembled string) */

		for (n = 0 ; n < nwords ; n++) {
			printf (" %04X", (unsigned short)buffer[n]);
			if ((n + 1) == 3) {
				if ((string.cv_length <= 0) || (nbytes <= 1))
					printf ("  --------- UNKNOWN");
				else {	printf ("  ");
					print_cv_string (stdout, &string);
				}
				printf ("\n");
				if (nwords > 3) printf ("         ");
			}
			else if (((n + 1) % 3) == 0) {
				printf ("\n");
				if ((n + 1) >= (nwords - 1))
					printf ("          ");
			}
		}

		if (nwords < 3) {
			for (n = 0 ; n < (3 - nwords) ; n++)
				printf ("     ");
			if ((string.cv_length <= 0) || (nbytes == 1))
				printf ("  --------- UNKNOWN");
			else {	printf ("  ");
				print_cv_string (stdout, &string);
			}
		}

		if (nwords != 3)
			printf ("\n");

		last_was_rts = rts_instruction(buffer);
	}

	fclose (file);
	exit (0);
}
