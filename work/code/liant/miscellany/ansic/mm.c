/**************************************************************************
 * PROGRAM:	Master Mind Program
 * AUTHOR:	David G. Michaels (david at ll-sst)
 * DATE:	September 1983
 **************************************************************************/

/*
*  NAME
*	mm - Master Mind program
*
*  SYNOPSIS
*	mm
*
*  DESCIPTION
*	Acts as a passive partner in the game of master mind.
*	The "pegs" are letters from the set [abcdef], you
*	have seven tries to guess the code.  A "black" reply
*	means that you have a correct color in a correct
*	position, a "white" reply means that you have a
*	correct color in an incorrect position.
*/

/*
 * If desired, the board size (MMSZ), number of different
 * pegs (NPEGS), code characters (CodeChar, continuous ascii),
 * or number of tries allowed (MAXTRY) can easily be changed.
 */

/**************************************************************************
*		INCLUDE FILES
**************************************************************************/

#include <stdio.h>
#include <ctype.h>

/**************************************************************************
*		MODIFIABLE DEFINITIONS
**************************************************************************/

#define MMSZ	    4		/* size of mastermind board */
#define	NPEGS	    6		/* number of diff pegs used */
#define MAXTRY	    7		/* max user guesses allowed */

char CodeChars[] = "abcdef";	/* code chars (MMSZ) */

/**************************************************************************
*		OTHER DEFINITIONS
**************************************************************************/

/*
 *  The random
 *  peg generator.
 */

#define rand_peg()	((rand() % NPEGS) + (int)CodeChars[0])

/*
 *  The last
 *  code character.
 */

#define LAST_CC		((int)CodeChars[0] + (NPEGS-1))

#define EOS		'\0'		/* end of string character */
#define MAXLINE		(MMSZ+6)	/* max length of input line */
#define NEWLINE		'\n'		/* newline character */
#define WIN		MMSZ		/* i.e. MMSZ pegs get black peg	*/

/**************************************************************************
*		GLOBAL VARIABLES
**************************************************************************/

unsigned int	Black;		/* number of black resposes	*/
unsigned int	White;		/* number of white resposes	*/
char		Code[MMSZ + 1];	/* randomly generated code	*/

/**************************************************************************
*		FUNCTIONS
**************************************************************************/

main (int argc, char *argv[])
{
	char	*get_guess(), *gen_code(), *getline();
	int	result();
	int	ntry;
	char	userline [MAXLINE+1];	/* for user input (guesses&commands) */

	printf("\t\t*** Master Mind ***\n\nDo you want instructions? (y/n) ");

	if(getline(userline,MAXLINE,stdin) != NULL &&
	   (userline[0] == 'y' || userline[0] == 'Y')){
		instruct();
	}
	else{
		userline[0] = 'y';
	}

	while(userline[0] == 'y' || userline[0] == 'Y'){

		putchar('\n');
		ntry = 0;
		gen_code(Code);

		while(1){

			while(get_guess(userline) == NULL){
				printf("\t\tUse %d chars from [%s]\n",
				       MMSZ,CodeChars);
			}

			if(result(Code,userline) == WIN){
				printf("\nYou won in %d tries\n",++ntry);
				switch(ntry){
				  case 1:
					printf("(pure luck),");
					break;
				  case 2:
					printf("(lucky stiff),");
					break;
				  case 3:
					printf("very good,");
					break;
				  case 4:
					printf("pretty good,");
					break;
				  case MAXTRY:
					printf("just barely,");
					break;
				  default:
					printf("not too bad,");
				}
				printf(" play again?(y/n) ");
				break;
			}

			if(++ntry == MAXTRY){
				printf("\n%d tries and still wrong!!\n",ntry);
				printf("correct code was:\t   %s\n",Code);
				printf("Want to play again?(y/n) ");
				break;
			}

			printf("\t\t%d black, %d white\n",Black,White);
		}
		/*
		*  Get the y/n answer
		*/
		getline(userline,MAXLINE,stdin);
	}
}

/*
*	gen_code
*
*	Return in `code` a randomly genarated string
*	of `MMSZ` charecters from the set `CodeChars`
*/

char *
gen_code (register char *code)
{
	long time();
	register int i;

	/*
	*  Re-initialize the
	*  random number generator.
	*/
	srand((int)(time(0)*17));

	for(i = 0 ; i < MMSZ ; i++){
		code[i] = rand_peg();
	}
	code[i] = EOS;
	return(code);
}

/*
*	get_guess
*
*	Get the users next guess from the terminal
*	If it is not a valid respnse, return NULL,
*	otherwise return a pointer to the string.
*/

char *
get_guess(register char *s)
{
	char *getline();
	register int i, c;

	/*
	*  Get a non-trivial line
	*  from the standard input.
	*/
	if(getline(s,MAXLINE,stdin) == NULL)
		return(NULL);
	/*
	*  Make sure the first MMSZ characters
	*  consist of valid code characters.
	*/
	for(i = 0 ; i < MMSZ ; i++)
		if(s[i] < CodeChars[0] || s[i] > LAST_CC)
			return(NULL);
	return(s);
}

/*
*	result
*
*	Figure out the result of the user's guess.
*	Set the global variables `Black` and `White`,
*	and return the number of exact matches (Black).
*/

result (char *codep, char *guessp)
{
	register char guess[MMSZ+1], code[MMSZ+1];
	register int i, j;

	/*
	*  Make local copies of
	*  the code and the guess.
	*/
	strcpy(guess,guessp);
	strcpy(code,codep);

	/*
	*  Initialize the
	*  response pegs.
	*/
	Black = White = 0;

	/*
	*  Find exact (black)
	*  matches; correct
	*  color and position.
	*/
	for(i = 0 ; i < MMSZ ; i++){
		/*
		*  A match!
		*/
		if(code[i] == guess[i]){
			/*
			*  Increment the black count,
			*  and mark it with a NULL.
			*/
			Black++;
			code[i] = guess[i] = EOS;
		}
	}

	/*
	*  Find inexact (white)
	*  matches; correct
	*  color, wrong position.
	*/
	for(i = 0 ; i < MMSZ ; i++)
		if(code[i] != EOS)
			for(j = 0 ; j < MMSZ ; j++)
			    if(j != i && guess[j] != EOS
			       && code[i]==guess[j]){
				    /*
				    *  Increment the white count,
				    *  and mark it with a NULL.
				    */
				    White++;
				    code[i] = guess[j] = EOS;
				    break;
				}
	return(Black);
}

/*
*	getline
*
*	Reads n - 1 characters, or up to a newline, whichever comes
*	first, from the given file `f` into the string `line` after 
*	first skipping any leading blanks in the input stream (i.e.
*	spaces, tabs, backspaces, and newlines).  The last character
*	read into `line` is followed by NULL character.	 The newline
*	is NOT included in the string. 	The IO pointer is set to the
*	beginning of the next line (or to EOF if there is no next line. 
*	Returns the `line` pointer or NULL upon EOF, or error.
*
*	This is just like fgets(3S) except that leading blanks are
*	skipped, the newline is not kept, and we skip to the beginning
*	of the next line.
*/

char *
getline (char *line, register int maxline, register FILE *f)
{
	register int c;
	register char *p = line;

	if(maxline <= 0 || line == NULL)
		return(NULL);
	if(maxline == 1){
		*p = EOS;
		return(line);
	}
 	while((c = getc(f)) != EOF){
		if(!isspace(c)){
			*p++ = c;
			maxline--;
			break;
		}
	}
	while(maxline-- > 0 && (c = getc(f)) != EOF) {
		if (c == NEWLINE)
			break;
		*p++ = c;
	}
	if(c == EOF && p == line)
		return(NULL);
	*p = EOS;

	if(c != NEWLINE)
		while((c = getc(f)) != EOF && c != NEWLINE)
				;
	return(line);
}

/*
*	instruct
*
*	Print instructions to the user.
*/

instruct (void)
{
	register int perms = 1;
	register int i

 	for (i = 0 ; i < MMSZ ; i++)
		perms *= NPEGS;

 printf (

"\nI will act as a passive partner in the game of master mind.\n\n"
"     * I will create a code consisting of %d \"colored pegs\".\n"
                ,MMSZ
"       In this case, the \"colored peg\" will be\n"
"       %d characters from the set [%s].\n",NPEGS,CodeChars
"     * You must make successive guesses at the code\n"
"       and modify your guesses using the below clues\n"
"       until you guess the code, or until you run out\n"
"       of turns (%d). There are %d possible codes.\n"
                ,MMSZ,perms
"     * A \"black\" reply means that you\n"
"       have a correct color in a correct position.\n"
"     * A \"white\" reply means that you\n"
"       have a correct color in an incorrect position.\n"
"     * You have %d tries to guess the code.\n"
                ,MAXTRY
"     * Enter your first try. ---\n"

);

}
