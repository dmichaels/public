/* LPI pause utility (for CodeWatch demo) */

/************************************************************************
 * Copyright (c) 1991 by Liant Software Corporation.                    *
 ************************************************************************
 * This product is the property of Liant Software Corporation and is    *
 * licensed pursuant to a written license agreement.  No portion of     *
 * this product may be reproduced without the written permission of     *
 * Liant Software Corporation except pursuant to the license agreement. *
 ************************************************************************/

/* ---------------------------------------------------------------------
/*
/*  USAGE
/*
/*	pause [ seconds ]
/*
/*  DESCRIPTION
/*
/*	This program will attempt to read from the standard input
/* 	for a specified number of seconds (10 seconds by default).
/*	If no input is received within that time, then this program
/* 	will exit with status zero.  If any input is received, then
/*	the program will enter "interactive" mode after the following
/*	message is display (on the standard output):
/*
/*		STOPPED: Hit SPACE or RETURN to continue;
/*		         hit Z to return to automatic mode.
/*
/*	At this point, the program will attempt to read from the
/* 	standard input until a SPACE or RETURN (or actually *any*
/*	character except a 'Z') is read, at which time the program
/*	will exit with status zero.  If instead a 'Z' is read, the
/*	program  will return to "automatic" mode and will exit with
/*	status zero.
/*
/*	Note that the toggling between "automatic" mode and "interactive"
/*	is accomplished through the use of a lock file named ".__imode__"
/*	residing in the current directory.  If the program is to start
/*	off in "interactive" mode, then the invoking program should make
/*	sure that the lock file does not exist.
/*
/*	If the number of seconds given is less than or equal to zero, then
/*	the program will only run in "interactive" mode; the lock file is not
/*	referenced in any way, and a 'Z' input will have no special effect.
/*	In this case, the program simply reads from the standard input until
/*	something is read, and then exits with status zero.
/*
/*	If the program is interrupted, then it will exit with status one.
/*
/*	Note that all input is read in "cbreak" mode, i.e. an input
/*	character is seen immediatly by the program and no carraige
/*	RETURN is required.
/*
/*  EXAMPLE
/*
/*	This program was written for the CodeWatch demo so that it
/*	can scroll by automatically while pausing occasionally for
/*	a few seconds at "pause points", but can also stop (instead
/*	of pause) at these "pause points" if some character is hit,
/*	at which time is in "interactive" mode and will continue
/*	from the "pause point" to the next "pause point" (where it
/*	will stop (not pause) again) when a SPACE or RETURN is hit.
/*
/*	Use the following shell script to test this program.
/*
/*		while (1)
/*			echo "CODEWATCH-DEMO-OUTPUT ..."
/*			pause
/*		end
/*
/*  BUGS
/*
/*	This seems like an awful lot of trouble for such a seemingly
/*	trivial task.  Maybe there's a much simpler way but I couldn't
/*	find it; essentially, all we need is some simple portable way
/*	to do a timed read.
/*
/*	Assuming this is way to do it, I couldn't make it work the
/*	same way on BSD UNIX, and UNIX System V.
/*
/*	On UNIX System V, it seems to take multiple interrupts to
/*	kill the program.
/*
/*  SEE ALSO
/*
/*	ioctl (2), tty (4)
/*
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/*
/*  LPI EDIT HISTORY
/*
/*  01.16.90  DGM  000  Original (for CodeWatch demo).
/*
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Configuration
/* ------------------------------------------------------------------- */

/* Define *exactly* one of the following */

/* #define UNIX_SYSTEM_V */
/* #define UNIX_BSD      */

/* Check the configuration */

#if !defined (UNIX_SYSTEM_V) & !defined (UNIX_BSD)
#error "Either UNIX_SYSTEM_V or UNIX_BSD must be defined!"
#endif

#if defined (UNIX_SYSTEM_V) & defined (UNIX_BSD)
#error "Both UNIX_SYSTEM_V and UNIX_BSD may not be defined!"
#endif

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>

#ifdef UNIX_BSD
#include <unistd.h>
#include <sgtty.h>
#endif

#ifdef UNIX_SYSTEM_V
#include <unistd.h>
#include <termio.h>
#endif

/* ---------------------------------------------------------------------
/* Local definitions
/* ------------------------------------------------------------------- */

#define FALSE				0
#define TRUE				1

#define STDIN				0
#define STDOUT				1
#define STDERR				2

#define SYS_ERROR			(-1)

#define DEFAULT_PAUSE_TIME		10
#define INTERACTIVE_MODE_LOCK_FILE	".__lpi_imode__"

#ifdef UNIX_BSD
#define GET_TTY_STATE(tty)	((void) ioctl (STDIN, TIOCGETP, &(tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (STDIN, TIOCSETP, &(tty)))
#endif

#ifdef UNIX_SYSTEM_V
#define GET_TTY_STATE(tty)	((void) ioctl (STDIN, TCGETA, &(tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (STDIN, TCSETAF, &(tty)))
#endif

/* ---------------------------------------------------------------------
/* Type definitions
/* ------------------------------------------------------------------- */

#ifdef UNIX_BSD
typedef struct sgttyb	TTY;
#endif

#ifdef UNIX_SYSTEM_V
typedef struct termio	TTY;
#endif

/* ---------------------------------------------------------------------
/* Local static data
/* ------------------------------------------------------------------- */

static char		*ProgramName		= "pause";
static unsigned int	 PauseTime		= DEFAULT_PAUSE_TIME;
static int		 AlarmWentOff		= FALSE;
static int		 OnlyInteractiveMode	= FALSE;

/* ---------------------------------------------------------------------
/* save_input_state
/* restore_input_state
/* ------------------------------------------------------------------- */

static TTY save_tty_state;

static
void
save_input_state ()
{
	GET_TTY_STATE (save_tty_state);
}

static
void
restore_input_state ()
{
	SET_TTY_STATE (save_tty_state);
}

/* ---------------------------------------------------------------------
/* set_cbreak_input_mode
/* clear_cbreak_input_mode
/* ------------------------------------------------------------------- */

static
void
set_cbreak_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
#ifdef UNIX_BSD
	tty_state.sg_flags |= CBREAK;
#endif
#ifdef UNIX_SYSTEM_V
        tty_state.c_lflag &= ~ICANON;
        tty_state.c_cc[VEOF] = 1;
#endif
	SET_TTY_STATE (tty_state);
}

static
void
clear_cbreak_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
#ifdef UNIX_BSD
	tty_state.sg_flags &= ~CBREAK;
#endif
#ifdef UNIX_SYSTEM_V
        tty_state.c_lflag |= ICANON;
#endif
	SET_TTY_STATE (tty_state);
}

/* ---------------------------------------------------------------------
/* set_echo_input_mode
/* clear_echo_input_mode
/* ------------------------------------------------------------------- */

static
void
set_echo_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
#ifdef UNIX_BSD
	tty_state.sg_flags |= ECHO;
#endif
#ifdef UNIX_SYSTEM_V
        tty_state.c_lflag |= ECHO;
#endif
	SET_TTY_STATE (tty_state);
}

static
void
clear_echo_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
#ifdef UNIX_BSD
	tty_state.sg_flags &= ~ECHO;
#endif
#ifdef UNIX_SYSTEM_V
        tty_state.c_lflag &= ~ECHO;
#endif
	SET_TTY_STATE (tty_state);
}

/* ---------------------------------------------------------------------
/* alarm_handler
/* set_alarm
/* clear_alarm
/* ------------------------------------------------------------------- */

static
void
alarm_handler ()
{
	AlarmWentOff = TRUE;
	return;
}

static
void
set_alarm (seconds)
unsigned int seconds;
{
	AlarmWentOff = FALSE;
	signal (SIGALRM, alarm_handler);
	alarm (seconds);
}

static
void
clear_alarm ()
{
	AlarmWentOff = FALSE;
	signal (SIGALRM, SIG_IGN);
}

/* ---------------------------------------------------------------------
/* unbuffered_getchar
/* ------------------------------------------------------------------- */

static
int
unbuffered_getchar ()
{
	char c;

	if (read (STDIN, &c, 1) <= 0)
		return (EOF);
	else	return ((int)c);
}

/* ---------------------------------------------------------------------
/* non_blocking_getchar
/* ------------------------------------------------------------------- */

#ifdef UNIX_BSD

static
int
non_blocking_getchar ()
{
	int c;
	int n;

	(void) ioctl (STDIN, FIONREAD, &n);

	if (n <= 0)
		return (EOF);

	n = read (STDIN, &c, 1);

	if (n <= 0)
		return (EOF);

	return ((int)c);
}

#endif /* UNIX_BSD */

/* ---------------------------------------------------------------------
/* timed_getchar
/* ------------------------------------------------------------------- */

#ifdef UNIX_BSD

static
int
timed_getchar ()
{
	int c;

	set_alarm (PauseTime);

	for (c = EOF ; !AlarmWentOff ; ) {
		if ((c = non_blocking_getchar ()) != EOF)
			break;
	}

	clear_alarm ();

	return (c);
}

#endif /* UNIX_BSD */

#ifdef UNIX_SYSTEM_V

static
int
timed_getchar ()
{
        int c;

        set_alarm (PauseTime);

        /*
        /* If the alarm goes off, this call will
        /* be interrupted and will return EOF.
        /**/

        c = unbuffered_getchar ();

        clear_alarm ();

        return (c);
}

#endif /* UNIX_SYSTEM_V */

/* ---------------------------------------------------------------------
/* is_interactive_mode
/* ------------------------------------------------------------------- */

static
int
is_interactive_mode ()
{
	if (OnlyInteractiveMode)
		return (TRUE);

	if (access (INTERACTIVE_MODE_LOCK_FILE, F_OK) != SYS_ERROR)
		return (TRUE);
	else	return (FALSE);
}

/* ---------------------------------------------------------------------
/* set_interactive_mode
/* ------------------------------------------------------------------- */

static
void
set_interactive_mode ()
{
	if (OnlyInteractiveMode)
		return;

	if (is_interactive_mode ())
		return;

	(void) close (open (INTERACTIVE_MODE_LOCK_FILE, O_CREAT, 0));
}

/* ---------------------------------------------------------------------
/* clear_interactive_mode
/* ------------------------------------------------------------------- */

static
void
clear_interactive_mode ()
{
	if (OnlyInteractiveMode)
		return;

	if (!is_interactive_mode ())
		return;

	(void) unlink (INTERACTIVE_MODE_LOCK_FILE);
}

/* ---------------------------------------------------------------------
/* interrupt_handler
/* ------------------------------------------------------------------- */

static
void
interrupt_handler ()
{
	restore_input_state ();

	if (is_interactive_mode ())
		clear_interactive_mode ();

	printf ("pause: killed.\n");

	exit (1);
}

/* ---------------------------------------------------------------------
/* main
/* ------------------------------------------------------------------- */

main (argc, argv)
int argc;
char *argv[];
{
	int n;
	char c;

        if (argc > 1) {
                if ((n = atoi (argv[1])) <= 0)
			OnlyInteractiveMode = TRUE;
		else	PauseTime = n;
        }

        setbuf (stdout, NULL);

	signal (SIGINT, interrupt_handler);
	signal (SIGQUIT, interrupt_handler);

	save_input_state ();

	set_cbreak_input_mode ();
	clear_echo_input_mode ();

	if (OnlyInteractiveMode) {
		c = unbuffered_getchar ();
		goto done;
	}

	if (!is_interactive_mode ()) {
		if ((c = timed_getchar ()) != EOF) {
                        printf ("STOPPED: Hit SPACE or RETURN to continue");
                        printf (" (hit Z to return to automatic mode).\n");
			set_interactive_mode ();
		}
		else	goto done;
	}

	if (is_interactive_mode ()) {
		if ((c = unbuffered_getchar ()) == 'Z') {
			printf ("Returning to automatic mode.\n");
			clear_interactive_mode ();
		}
		else	goto done;
	}

	done:

	restore_input_state ();
	exit (0);
}

