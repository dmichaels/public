#include <stdio.h>
#include <sys/termio.h>

#define STDIN				0

#define SYS_ERROR			(-1)

#define DEFAULT_PAUSE_TIME		10
#define INTERACTIVE_MODE_LOCK_FILE	".__lpi_imode__"

#define GET_TTY_STATE(tty)	((void) ioctl (0, TCGETS, &(tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (0, TCSETS, &(tty)))

typedef struct termio	TTY;

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

static
void
set_echo_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
	tty_state.c_lflag |= ECHO;
	SET_TTY_STATE (tty_state);
}

static
void
clear_echo_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
	tty_state.c_lflag &= ~ECHO;
	SET_TTY_STATE (tty_state);
}

main (argc, argv)
int argc;
char *argv[];
{
	int c;

	clear_echo_input_mode ();

	printf ("hello\n");
	while ((c = getc (stdin)) != EOF)
		printf ("Character %d\n", c);
}
