/* ---------------------------------------------------------------------- */
/* Do "lpiadmin" size and checksum.
/* ---------------------------------------------------------------------- */

#include <stdio.h>

#define TRUE		(-1)
#define FALSE		(0)

#define MAXSUM		((long)0xFFFF)
#define BLOCKSIZE	((long)4096)

#define nblocks(nbytes)	(((nbytes) + BLOCKSIZE - 1) / BLOCKSIZE)

typedef struct {
	long         cs_sum;         /* checksum */
	long         cs_index;       /* checksum index */
	long         cs_nbytes;      /* file size in bytes */
} CHECKSUM;

typedef unsigned long	ulong;
typedef int		bool;

main (argc, argv)
int	argc;
char	*argv[];
{
        bool		dochecksum ();
        CHECKSUM	cs;
        register int	i;
	int		status = 0;

        if (argc < 2) {
                fprintf (stderr, "usage: %s file...\n", argv[0]);
                exit (1);
        }
        for (i = 1 ; i < argc ; i++) {
                if (dochecksum(argv[i],&cs) == FALSE) {
                        fprintf(stdout,
                        "\nCan't do checksum on \"%s\"...", argv[i]);
                        if (i == argc - 1) {
                                fprintf (stdout, "exit.\n");
                                exit (1);
                        }
                        fprintf (stdout, "continue.\n");
                        status = 2;
                        continue;
                }
                fprintf (stdout, "\n%s:\n\n", argv[i]);
                fprintf (stdout,
                "%-16s%d\n", "-SIZE", cs.cs_nbytes);
                fprintf(stdout,
                "%-16s%d.%d\n", "-CHECKSUM", cs.cs_sum, cs.cs_index);
        }
        exit (status);
}

/*
 * ------------------------------------------------------------------------
 * dochecksum
 *
 * Perform a checksum and fill in the given
 * checksum structure with the result.
 */

bool
dochecksum (file, check)
char		*file;
CHECKSUM	*check;
{
        register FILE	*f;
        register int	c;
        register ulong	sum, nbytes;
        ulong		index;

        if ((f = fopen(file,"r")) == NULL)
                return(FALSE);
        for (sum = index = nbytes = 0L; ; (c = getc(f)) != EOF ; nbytes++) {
                if ((sum + c) <= MAXSUM)
                        sum += (long)c;
                else {
                        index++;
                        sum = (long)c - (MAXSUM - sum);
                }
        }
        check->cs_sum = sum;
        check->cs_index = index;
        check->cs_nbytes = nbytes;
        fclose (f);
        return (TRUE);
}
