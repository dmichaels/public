/* get_one_char */

/* ---------------------------------------------------------------------
/* Configuration
/* ------------------------------------------------------------------- */

/* Define *exactly* one of the following */

#define UNIX_BSD
#undef  UNIX_SYSTEM_V

/* ---------------------------------------------------------------------
/* Include files
/* ------------------------------------------------------------------- */

#include <stdio.h>
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

#ifdef UNIX_BSD
#define GET_TTY_STATE(tty)	((void) ioctl (STDIN, TIOCGETP, (tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (STDIN, TIOCSETP, (tty)))
#endif

#ifdef UNIX_SYSTEM_V
#define GET_TTY_STATE(tty)	((void) ioctl (STDIN, TCGETA, (tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (STDIN, TCSETAF, (tty)))
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
/* save_input_state
/* restore_input_state
/* ------------------------------------------------------------------- */

static
void
save_input_state (tty)
TTY *tty;
{
	GET_TTY_STATE (tty);
}

static
void
restore_input_state (tty)
TTY *tty;
{
	SET_TTY_STATE (tty);
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

/* ---------------------------------------------------------------------
/* get_one_char
/*
/* Gets and returns the very next character from the standard input
/* without waiting for a new-line/carriage-return or any such nonesense.
/* ------------------------------------------------------------------- */

int
get_one_char ()
{
	TTY save_tty_state;
	int c;

	GET_TTY_STATE (&save_tty_state);
	set_cbreak_input_mode ();
	c = getchar ();
	SET_TTY_STATE (&save_tty_state);
	return (c);
}


