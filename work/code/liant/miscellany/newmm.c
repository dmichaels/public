#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/termio.h>

extern "C" long random ();
extern "C" long srandom (int);

enum Boolean { False = 0, True = !False };

typedef struct termio	TTY;

#define GET_TTY_STATE(tty)	((void) ioctl (0, TCGETS, &(tty)))
#define SET_TTY_STATE(tty)	((void) ioctl (0, TCSETS, &(tty)))

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
	tty_state.c_lflag |= ECHONL;
	SET_TTY_STATE (tty_state);
}

static
void
clear_echo_input_mode ()
{
	TTY tty_state;

	GET_TTY_STATE (tty_state);
	tty_state.c_lflag &= ~ECHONL;
	SET_TTY_STATE (tty_state);
}

class MasterMind {

  private:

	typedef int		  Peg;
	typedef int		  Hole;
	typedef Hole		 *Row;
	typedef Hole		(*Board)[1];

	enum Configuration {
		DefaultNumberOfPegs	=    6 ,
		DefaultRowSize		=    4 ,
		DefaultNumberOfRows	=    6 ,
		MaximumNumberOfPegs	=   26 ,
		MaximumRowSize		=  256 ,
		MaximumNumberOfRows	= 1024 };
			    
	enum MatchCode { NoMatch, InexactMatch, ExactMatch };

  private:

	static unsigned char CharacterCodes[];

  private:

	Board			user_board;
	Row			secret_code;
	Board			response_board;
	int			number_of_pegs;
	int			row_size;
	int			number_of_rows;
	int			current_row;

	Boolean			user_quit;
	Boolean			user_won;
	Boolean			user_lost;

	int			user_ngames;
	int			user_nwins;
	int			user_nloses;
	double			user_average;

  public:
		MasterMind (int npegs   = DefaultNumberOfPegs,
			    int rowsize = DefaultRowSize,
			    int nrows   = DefaultNumberOfRows);

	void	Resize (int npegs   = DefaultNumberOfPegs,
			int rowsize = DefaultRowSize,
			int nrows   = DefaultNumberOfRows);

	void	Play ();
	void	MakeSecretCode ();
	void	StartNewGame ();
	void	AcceptInput ();
	void	ProcessInput ();
	void	DisplayBoard ();
	void	DisplayResponse (int = -1);
	void	DisplaySecretCode ();
	Boolean	Quit () { return (user_quit); }
	void	ResponsePrefix ();
	void	InputPrefix ();

	static void	Greeting ();
	static void	Farewell ();
};

unsigned char MasterMind::CharacterCodes[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void
MasterMind::Greeting ()
{
	printf ("**********************************************************\n"
		"** MasterMind ** MasterMind ** MasterMind ** MasterMind **\n"
		"**********************************************************\n");
}

void
MasterMind::Farewell ()
{
	printf ("Bye.\n");
}

MasterMind::MasterMind (int npegs,
			int rowsize,
			int nrows)
{
	int i, j, k;

	if (npegs > MaximumNumberOfPegs)
		npegs = MaximumNumberOfPegs;
	if (rowsize > MaximumRowSize)
		rowsize = MaximumRowSize;
	if (nrows > MaximumNumberOfRows)
		nrows = MaximumNumberOfRows;

	user_board	= (Board) new Hole [rowsize * nrows];
	response_board	= (Board) new Hole [rowsize * nrows];
	secret_code	= new Hole [rowsize];
	number_of_pegs	= npegs;
	row_size	= rowsize;
	number_of_rows	= nrows;
	user_nwins	= 0;
	user_nloses	= 0;
	user_ngames	= 0;
	user_average	= 0.0;

	StartNewGame ();
}

void
MasterMind::Play ()
{
	while (True) {
		AcceptInput ();
		if (Quit ())
			break;
		ProcessInput ();
	}
}

void
MasterMind::StartNewGame ()
{
	user_quit	= False;
	user_won	= False;
	user_lost	= False;
	current_row	= 0;

	MakeSecretCode ();

	printf ("\n-- MasterMind --------------------------------------------\n");
	InputPrefix ();
	printf ("\n");
}

void
MasterMind::Resize (int npegs,
		    int rowsize,
		    int nrows)
{
}

void
MasterMind::MakeSecretCode ()
{
//	srandom (time (0));
	for (int i = 0 ; i < row_size ; i++)
		secret_code[i] = /*random () %*/ number_of_pegs;
}

void
MasterMind::AcceptInput ()
{
	int i, c;

	Begin:

	InputPrefix ();

	for (i = 0 ; True ; ) {
		if ((c = toupper (getc (stdin))) == EOF)
			goto Quit;
		else if (c == '\n') {
			if (i == 0) {
				InputPrefix ();
				continue;
			}
			else if (i != row_size)
				goto Error;
			else	return;
		}
		else if (isspace (c))
			continue;
		else if (i >= row_size)
			goto Error;
		else if ((c < CharacterCodes[0]) ||
			 (c > CharacterCodes[number_of_pegs - 1]))
			 goto Error;
		else {
			user_board[current_row][i++] =
				int (c) - int (CharacterCodes[0]);
		}
	}

	Error:
	if (c != '\n') { while (getc (stdin) != '\n') ; }
	ResponsePrefix ();
	printf ("Error: use %d characters from \"", row_size);
	for (i = 0 ; i < number_of_pegs ; i++)
		printf ("%c", CharacterCodes[i]);
	printf ("\"\n");
	goto Begin;

	Quit:
	user_quit = True;
	return;
}

void
MasterMind::ProcessInput ()
{
	int i, j, n;

	// Initialize this response.

	for (i = 0 ; i < row_size ; i++)
		response_board[current_row][i] = NoMatch;

	// First, check for exact matches.

	for (i = n = 0 ; i < row_size ; i++)
		if (secret_code[i] == user_board[current_row][i])
			response_board[current_row][i] = ExactMatch, n++;

	// Check if we're done (i.e. all exact matches, i.e. a win).

	if (n == row_size)
		goto Done;

	// Check for for inexact matches.

	for (i = 0 ; i < row_size ; i++) {
		if (response_board[current_row][i] == ExactMatch)
			continue;
		for (j = 0 ; j < row_size ; j++) {
			if (i == j)
				continue;
			if (secret_code[i] == user_board[current_row][j]) {
				response_board[current_row][i] = InexactMatch, n++;
				if (n == row_size)
					goto Done;
			}
		}
	}

	for (i = 0 ; i < row_size ; i++)
		if (response_board[current_row][i] != ExactMatch)
			break;

	Done:

	if (i == row_size) {
		user_won = True;
		user_ngames++;
		user_nwins++;
	}
	else if ((current_row + 1) == number_of_rows) {
		user_lost = True;
		user_ngames++;
		user_nloses++;
	}

	DisplayResponse ();

	if (user_won || user_lost)
		StartNewGame ();
	else	current_row++;
}

void
MasterMind::DisplayResponse (int whichrow)
{
	int i, j, n;

	ResponsePrefix ();
	if (whichrow <= 0) {
		if (user_won) {
			printf ("You win\n");
			return;
		}
	}
	for (i = n = 0 ; i < row_size ; i++) {
		if (response_board[current_row][i] == ExactMatch) {
			n++;
			printf ("X");
		}
	}
	for (i = 0 ; i < row_size ; i++) {
		if (response_board[current_row][i] == InexactMatch) {
			n++;
			printf ("O");
		}
	}
	for (i = 0 ; i < row_size - n ; i++)
		printf ("-");
	if ((current_row + 1) >= number_of_rows) {
		printf ("  You lose -- ");
		DisplaySecretCode ();
	}
	printf ("\n");
}

void
MasterMind::DisplaySecretCode ()
{
	int i;

	for (i = 0 ; i < row_size ; i++)
		printf ("%c", CharacterCodes[secret_code[i]]);
}

void
MasterMind::DisplayBoard ()
{
	int i, j;

	for (i = 0 ; i < current_row ; i++) {
		for (j = 0 ; j < row_size ; j++)
			printf ("%c", CharacterCodes[user_board[i][j]]);
		DisplayResponse (i);
	}
}

void
MasterMind::InputPrefix ()
{
	printf ("-- "); 
}

void
MasterMind::ResponsePrefix ()
{
	int i;

	InputPrefix ();
}

main (int argc, const char *argv[])
{
	save_input_state ();
	clear_echo_input_mode ();

	MasterMind::Greeting ();
	MasterMind *mm = new MasterMind;
	mm->Play ();
	delete mm;
	MasterMind::Farewell ();

	restore_input_state ();

	return (0);
}

