/* Tower of Hanoi */

/* ------------------------------------------------------------------------
/* Include files
/* ---------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>

/* ------------------------------------------------------------------------
/* Macro definitions
/* ---------------------------------------------------------------------- */

#define DEFAULT_NUMBER_OF_DISKS	64

#define NEEDLE_A		0
#define NEEDLE_B		1
#define NEEDLE_C		2

/* ------------------------------------------------------------------------
/* Type definitions
/* ---------------------------------------------------------------------- */

typedef unsigned char	needle;
typedef long		bigint;

/* ------------------------------------------------------------------------
/* Static data
/* ---------------------------------------------------------------------- */

static char	* NeedleName []		= { "A", "B", "C" };
static bigint	  TotalDiskMoves	= 0;
static bigint	  NumberOfDisks		= 0;
static int	  VerboseFlag		= 0;

/* ------------------------------------------------------------------------
/* move_disk
/* ---------------------------------------------------------------------- */

static
void
move_disk (needle sn, needle dn)
{
	TotalDiskMoves++;

	if (VerboseFlag) {
		static int column = 0;
		printf (" %s%s", NeedleName [sn], NeedleName [dn]);
		if (column++ >= 25) {
			printf ("\n"); 
			column = 0;
		}
	}
}

/* ------------------------------------------------------------------------
/* move_tower
/* ---------------------------------------------------------------------- */

static
void
move_tower (bigint n, needle sn, needle dn, needle hn)
{
	if (n <= 1) {
		move_disk (sn, dn);
		return;
	}
	move_tower (n-1, sn, hn, dn);
	move_disk  (sn, dn);
	move_tower (n-1, hn, dn, sn);
}

/* ------------------------------------------------------------------------
/* usage
/* ---------------------------------------------------------------------- */

static
void
usage (void)
{
	fprintf (stderr, "usage: hanoi [ -v ] [ number_of_disks ]\n");
	exit (1);
}

/* ------------------------------------------------------------------------
/* main
/* ---------------------------------------------------------------------- */

main (int argc, char *argv[])
{
	int	ac;
	char	**av;

	for (ac = argc-1, av = argv+1 ; ac > 0 ; ac--, av++) {
		if ((*av)[0] == '-') {
			switch ((*av)[1]) {
			case 'v': VerboseFlag = 1;
				  break;
			default:  usage ();
			}
		}
		else	break;
	}

	if (ac > 1)
		usage ();

	else if (ac == 1) {
		if ((NumberOfDisks = atol (av[0])) <= 0)
		usage ();
	}

	if (VerboseFlag) printf ("\n");

	move_tower (NumberOfDisks, NEEDLE_A, NEEDLE_B, NEEDLE_C);

	if (VerboseFlag) printf ("\n\n");

	printf ("Moved Tower of Hanoi (%d disks) in %d moves.\n",
		NumberOfDisks, TotalDiskMoves);

	exit (0);
}

