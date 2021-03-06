/* C++ BLDMSG -- Translates C++ front-end message file into common format */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 * 
 *  LPI EDIT HISTORY
 *
 *  11.12.90  DGM  001  Updated for C++ front-end.
 *  08.13.90  PLB  000  Original (for ANSI-C front-end).
 *
 **********************************************************************/

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>

/* ---------------------------------------------------------------------
/* Local definitions
/* ------------------------------------------------------------------- */

#define EOS			'\0'
#define NEWLINE			'\n'
#define BLANK			' '

#define NULL_LINE		0
#define COMMENT_OR_BLANK_LINE	1
#define ORG_LINE		2
#define REPLACE_LINE		3
#define TEXT_LINE		4

/* ---------------------------------------------------------------------
/* print_copyright
/* ------------------------------------------------------------------- */

static void
print_copyright (FILE *f)
{

fprintf (f,
"/***********************************************************************/\n" \
"/* This product is the property of Language Processors, Inc. and is    */\n" \
"/* licensed pursuant to a written license agreement.  No portion of    */\n" \
"/* this product may be reproduced without the written permission of    */\n" \
"/* Language Processors, Inc. except pursuant to the license agreement. */\n" \
"/***********************************************************************/\n\n"
);

}

/* ---------------------------------------------------------------------
/* is_comment_or_blank_line
/* ------------------------------------------------------------------- */

static int
is_comment_or_blank_line (const char *buffer)
{
	register const char *p;

	for (p = buffer ; *p != EOS ; p++) {
		if (!isspace (*p)) {
			if (*p == '#')
				return (1);
			else	return (0);
		}
	}
	return (1);
}

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, char *argv[])
{   
	char buffer [2048];
	char *ifile, *mfile, *rfile, severity, *p, *r;
	FILE *ifd, *mfd, *rfd;
	int i, msgno = 0, last_line = NULL_LINE;

	/* Check for command line arguments */

	if (argc != 4) {
		fprintf (stderr, "usage:  %s"
				 " in_file message_file replace_file\n",
				 argv[0]);
		exit (1);
	}
    
	/* Open the input file */

	argc--; argv++;
	ifile = *argv;
	if ((ifd = fopen (ifile, "r")) == NULL) {
		fprintf (stderr, "%s: cannot open \"%s\"\n", argv[0], ifile);
		exit (2);
	}

	/* Open the message file */

	argc--; argv++;
	mfile = *argv;
	if ((mfd = fopen (mfile, "w")) == NULL) {
		fprintf (stderr, "%s: cannot open \"%s\"\n", argv[0], ifile);
		exit (2);
	}

	/* Open the replace file */

	argc--; argv++;
	rfile = *argv;
	if ((rfd = fopen (rfile, "w")) == NULL) {
		fprintf (stderr, "%s: cannot open \"%s\"\n", argv[0], ifile);
		exit (2);
	}

	/* Print the copyright header in the %replace file */

	print_copyright (rfd);

	/* Loop, reading line by line (up to newline) */

	while (fgets (buffer, sizeof (buffer), ifd) != NULL) {

		/* Comment or blank line ? */

		if (is_comment_or_blank_line (buffer)) {
			if (last_line == TEXT_LINE)
				fprintf (mfd, "!\n");
			fprintf (mfd, "/* %s", buffer);
			last_line = COMMENT_OR_BLANK_LINE;
		}

		/* "%%ORG" line ? */

		else if ((buffer[0] == '%') &&
			 (buffer[1] == '%') &&
			 (buffer[2] == 'O') &&
			 (buffer[3] == 'R') &&
			 (buffer[4] == 'G')) {
			if (last_line == TEXT_LINE)
				fprintf (mfd, "!\n");
			fprintf (mfd, "*%s", buffer + 2);
			last_line = ORG_LINE;
		}

		/* "%ERROR_REPLACE_CONSTANT_NAME" line ? */

		else if ((buffer[0] == '%') && isalpha (buffer[1])) {
			if (last_line == TEXT_LINE)
				fprintf (mfd, "!\n");
			msgno++;
			fprintf(mfd, "/* %s", buffer + 1);
			if (isspace (buffer[1])) {
				for (p = buffer + 2 ; *p != EOS ; p++) {
					if (!isspace (*p)) {
						r = p;
						break;
					}
				}
			}
			else    r = buffer + 1;
			for (p = r ; *p != EOS ; p++) {
				if (isspace (*p)) {
					*p = EOS;
					for (p++ ; *p != EOS ; p++) {
						if (!isspace (*p)) {
							if (isdigit (*p))
								severity = *p;
							break;
						}
					}
					break;
				}
			}
			fprintf (rfd, "%%replace ERR_");
			fprintf (rfd, "%s", r);
			i = strlen(r);
			for ( ; i < 40; i++)
				fputc (BLANK, rfd);
			fprintf(rfd, "by %3d;\n", msgno);
			last_line = REPLACE_LINE;
		}

		/* Start of message text line ? */

		else if (last_line == REPLACE_LINE) {
			fprintf (mfd, "%-4d", msgno);
			if (isdigit (buffer[0])) {
				fputc (buffer[0], mfd);
				i = 1;
			}
			else {
				fputc (severity, mfd);
				i = 0;
			}
			severity = EOS;
			for ( ; !isgraph (buffer[i]); i++)
				;
			for (p = buffer + i ; *p != EOS ; p++) {
				if (*p == NEWLINE) {
					*p = EOS;
					break;
				}
			}
			fprintf (mfd, "   %s", buffer + i);
			last_line = TEXT_LINE;
		}

		/* Continuation of message line */

		else {
			if (last_line == TEXT_LINE)
				fprintf (mfd, "\n");
			for (i = 0 ; !isgraph (buffer[i]) ; i++)
				;
			for (p = buffer + i ; *p != EOS ; p++) {
				if (*p == NEWLINE) {
					*p = EOS;
					break;
				}
			}
			fprintf(mfd, "        %s", buffer + i);
			last_line = TEXT_LINE;
		}
	}

	/* Close the files, and exit */

	fclose (ifd);
	fclose (mfd);
	fclose (rfd);

	exit (0);
}

