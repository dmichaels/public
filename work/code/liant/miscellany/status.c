/* status - Print the status for the given users */

/* ------------------------------------------------------------------- */
/* This product is the property of Language Processors, Inc. and is    */
/* licensed pursuant to a written license agreement.  No portion of    */
/* this product may be reproduced without the written permission of    */
/* Language Processors, Inc. except pursuant to the license agreement. */
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* 
/*  LPI EDIT HISTORY               [ Update the __VERSION__ string ]
/*
/*  07.11.90  DGM  000	Orignal.
/*
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Version and copyright stamp
/* ------------------------------------------------------------------- */

static char *__VERSION__ =

"@(#)status.c  000  7/11/90  (c) 1990 Language Processors, Inc.";

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

/* ---------------------------------------------------------------------
/* Local definitions
/* ------------------------------------------------------------------- */

#define DEFAULT_STATUS_FILE_NAME	".status"

/* ---------------------------------------------------------------------
/* Local static internal data
/* ------------------------------------------------------------------- */

static char	*ProgramName		= NULL;
static char	*StatusFileName		= DEFAULT_STATUS_FILE_NAME;

/* ---------------------------------------------------------------------
/* Local static internal functions
/* ------------------------------------------------------------------- */

static void		print_user_status_file (char *);
static struct passwd *	get_user_data (char *);
static FILE *		open_user_status_file (struct passwd *);
static void		close_user_status_file (FILE *);
static void		print_user_status_file_header (struct passwd *, FILE *);
static void		print_file (FILE *);

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (int argc, char *argv [])
{
	int	  ac;
	char	**av;

	ProgramName = argv [0];

	for (ac = argc - 1, av = argv + 1 ; ac > 0 ; ac--, av++)
		print_user_status_file (*av);

	printf ("\n");
}

/* ---------------------------------------------------------------------
/* print_user_status_file
/* ------------------------------------------------------------------- */

static
void
print_user_status_file (char *user_name)
{
	struct passwd	*udp;
	FILE		*usf;

	if ((udp = get_user_data (user_name)) == NULL)
		return;
	usf = open_user_status_file (udp);
	print_user_status_file_header (udp, usf);
	print_file (usf);
	close_user_status_file (usf);
}

/* ---------------------------------------------------------------------
/* get_user_data
/* ------------------------------------------------------------------- */

static
struct passwd *
get_user_data (char *user_name)
{
	extern struct passwd	*getpwnam ();
	struct passwd		 *p;

	if ((p = getpwnam (user_name)) == NULL) {
		fprintf (stderr,
			 "\n%s: ERROR -- unknown user name \"%s\"\n",
			 ProgramName, user_name);
		return (NULL);
	}
	return (p);
}

/* ---------------------------------------------------------------------
/* open_user_status_file
/* ------------------------------------------------------------------- */

static
FILE *
open_user_status_file (struct passwd *udp)
{
	FILE	*f;
	char	 fn [2048];

	if (access (udp->pw_dir, R_OK)) {
		fprintf (stderr,
			 "\n%s: ERROR -- can't access home directory"
			 "\"%s\" of user \"%s\"\n",
			 ProgramName, udp->pw_dir, udp->pw_name);
		return (NULL);
	}

	strcpy (fn, udp->pw_dir);
	strcat (fn, "/");
	strcat (fn, StatusFileName);

	if ((f = fopen (fn, "r")) == NULL)
		return (NULL);

	return (f);
}

/* ---------------------------------------------------------------------
/* close_user_status_file
/* ------------------------------------------------------------------- */

static
void
close_user_status_file (FILE *f)
{
	if (f == NULL)
		return;
	(void) fclose (f);
}

/* ---------------------------------------------------------------------
/* print_user_status_file_header
/* ------------------------------------------------------------------- */

static
void
print_user_status_file_header (struct passwd *upd, FILE *usf)
{
	struct stat	 fs;
	char		 mts [64];
	char 		*p;

	if (usf == NULL) {
		printf ("\n"
			"------------------------------------"
			"------------------------------------"
			"\n"
			"-- No status report for:  %s  (%s)"
			"\n"
			"------------------------------------"
			"------------------------------------"
			"\n",
			upd->pw_gecos,
			upd->pw_name);
		return;
	}

	(void) fstat (fileno (usf), &fs);

	strcpy (mts, ctime (&fs.st_mtime));

	if ((p = strchr (mts, '\n')) != NULL)
		*p = '\0';

	printf ("\n"
		"------------------------------------"
		"------------------------------------"
		"\n"
		"-- Status report for:  %s  (%s)"
		"\n"
		"-- Last update:        %s"
		"\n"
		"------------------------------------"
		"------------------------------------"
		"\n",
		upd->pw_gecos,
		upd->pw_name,
		mts);
}

/* ---------------------------------------------------------------------
/* print_file
/* ------------------------------------------------------------------- */

static
void
print_file (FILE *f)
{
	int c;

	if (f == NULL)
		return;
	while ((c = getc (f)) != EOF)
		putchar (c);
}

