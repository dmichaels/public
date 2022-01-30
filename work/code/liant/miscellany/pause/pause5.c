/* Pause utility for CodeWatch demo */

/* ---------------------------------------------------------------------
/*
/*  USAGE
/*
/*      pause [ -i ] [ seconds ]
/*
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/*
/*  LPI EDIT HISTORY
/*
/*	
/*  01.16.90  DGM  000  Original (for CodeWatch demo).
/*
/* ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <termio.h>

/* ---------------------------------------------------------------------
/* Local definitions
/* ------------------------------------------------------------------- */

#define FALSE                           0
#define TRUE                            1

#define STDIN                           0
#define STDOUT                          1
#define STDERR                          2

#define SYS_ERROR                       (-1)

#define DEFAULT_PAUSE_TIME              10
#define INTERACTIVE_MODE_LOCK_FILE      ".__imode__"

/* ---------------------------------------------------------------------
/* Local static data
/* ------------------------------------------------------------------- */

static char             *ProgramName    = "pause";
static unsigned int      PauseTime      = DEFAULT_PAUSE_TIME;
static int               AlarmWentOff   = FALSE;
static int		 InteractiveModeOnly	= FALSE;

/* ---------------------------------------------------------------------
/* save_input_state
/* restore_input_state
/* ------------------------------------------------------------------- */

static struct termio save_tty_state;

void
save_input_state ()
{
        (void) ioctl (STDIN, TCGETA, &save_tty_state);
}

void
restore_input_state ()
{
        (void) ioctl (STDIN, TCSETAF, &save_tty_state);
}

/* ---------------------------------------------------------------------
/* set_cbreak_input_mode
/* clear_cbreak_input_mode
/* ------------------------------------------------------------------- */

void
set_cbreak_input_mode ()
{
        struct termio tty_state;

        (void) ioctl (STDIN, TCGETA, &tty_state);
        tty_state.c_lflag &= ~ICANON;
        tty_state.c_cc[VEOF] = 1;
        (void) ioctl (STDIN, TCSETAF, &tty_state);
}

void
clear_cbreak_input_mode ()
{
        struct termio tty_state;

        (void) ioctl (STDIN, TCGETA, &tty_state);
        tty_state.c_lflag |= ICANON;
        (void) ioctl (STDIN, TCSETAF, &tty_state);
}

/* ---------------------------------------------------------------------
/* set_echo_input_mode
/* clear_echo_input_mode
/* ------------------------------------------------------------------- */

void
set_echo_input_mode ()
{
        struct termio tty_state;

        (void) ioctl (STDIN, TCGETA, &tty_state);
        tty_state.c_lflag |= ECHO;
        (void) ioctl (STDIN, TCSETAF, &tty_state);
}

void
clear_echo_input_mode ()
{
        struct termio tty_state;

        (void) ioctl (STDIN, TCGETA, &tty_state);
        tty_state.c_lflag &= ~ECHO;
        (void) ioctl (STDIN, TCSETAF, &tty_state);
}

/* ---------------------------------------------------------------------
/* alarm_handler
/* set_alarm
/* clear_alarm
/* ------------------------------------------------------------------- */

void
alarm_handler ()
{
        AlarmWentOff = TRUE;
        return;
}

set_alarm (seconds)
unsigned int seconds;
{
        AlarmWentOff = FALSE;
        signal (SIGALRM, alarm_handler);
        alarm (seconds);
}

clear_alarm ()
{
        AlarmWentOff = FALSE;
        signal (SIGALRM, SIG_IGN);
}

/* ---------------------------------------------------------------------
/* unbuffered_getchar
/* ------------------------------------------------------------------- */

int
unbuffered_getchar ()
{
        char c;

        if (read (STDIN, &c, 1) <= 0)
                return (EOF);
        else    return ((int)c);
}

/* ---------------------------------------------------------------------
/* timed_getchar
/* ------------------------------------------------------------------- */

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

/* ---------------------------------------------------------------------
/* is_interactive_mode
/* ------------------------------------------------------------------- */

int
is_interactive_mode ()
{
	if (InteractiveModeOnly)
		return (TRUE);

        if (access (INTERACTIVE_MODE_LOCK_FILE, F_OK) != SYS_ERROR)
                return (TRUE);
        else    return (FALSE);
}

/* ---------------------------------------------------------------------
/* set_interactive_mode
/* ------------------------------------------------------------------- */

void
set_interactive_mode ()
{
	if (InteractiveModeOnly)
		return;

        if (is_interactive_mode ())
                return;

        (void) close (open (INTERACTIVE_MODE_LOCK_FILE, O_CREAT, 0));
}

/* ---------------------------------------------------------------------
/* clear_interactive_mode
/* ------------------------------------------------------------------- */

void
clear_interactive_mode ()
{
	if (InteractiveModeOnly)
		return;

        if (!is_interactive_mode ())
                return;

        (void) unlink (INTERACTIVE_MODE_LOCK_FILE);
}

/* ---------------------------------------------------------------------
/* interrupt_handler
/* ------------------------------------------------------------------- */

void
interrupt_handler (n)
int n;
{
        restore_input_state ();
        exit (n);
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
		if ((argv[1][0] == '-') && (argv[1][1] = 'i')) {
			InteractiveModeOnly = TRUE;
			argc--, argv++;
		}
                if ((n = atoi (argv[1])) > 0)
                        PauseTime = (unsigned)n;
                else    PauseTime = DEFAULT_PAUSE_TIME;
        }

        setbuf (stdout, NULL);

        signal (SIGINT, interrupt_handler);
        signal (SIGQUIT, interrupt_handler);

        save_input_state ();

        set_cbreak_input_mode ();
        clear_echo_input_mode ();

        if (!is_interactive_mode ()) {
                if ((c = timed_getchar ()) != EOF) {
                        printf ("STOPPED: Hit SPACE or");
                        printf (" RETURN to continue");
                        printf (" (hit `z' to return");
                        printf ("to automatic mode).\n");
                        set_interactive_mode ();
                }
                else    goto done;
        }
        if (is_interactive_mode ()) {
                c = unbuffered_getchar ();
                if ((c == 'z') || (c == 'Z')) {
                        printf ("Returning to automatic mode.\n");
                        clear_interactive_mode ();
                }
                else    goto done;
        }

        done:

        restore_input_state ();
        exit (0);
}
