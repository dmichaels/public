static char *sccsid = "@(#)fgrep.c	4.1 (Berkeley) 10/1/80";
/*
 * fgrep -- print all lines containing any of a set of keywords
 *
 *	status returns:
 *		0 - ok, and some matches
 *		1 - ok, but no matches
 *		2 - some error
 *
 * CHANGES
 * ----------------------------------------------------------------
 * Somebody added the "-h" option (hflag) to supress the printing
 * of the file name even when multiple files are given.  The "-l"
 * option overrides this.
 * ----------------------------------------------------------------
 * Added "-p" option (pflag) to get the name of the file printed
 * even if there is only one argument file given.  This is useful
 * for example when doing the following:
 *
 *	find <dirname> -exec fgrep <pattern> {} \;
 *
 * since otherwise we may get output without knowing what file
 * it came from.  This option overrides the "-h" option, and is
 * overrided by the "-i" option.
 * David Michaels (david@ll-sst) October 1984.
 * ----------------------------------------------------------------
 */

#include "stdio.h"
# include "ctype.h"

#define	MAXSIZ 6000
#define QSIZE 400
struct words {
	char 	inp;
	char	out;
	struct	words *nst;
	struct	words *link;
	struct	words *fail;
} w[MAXSIZ], *smax, *q;

long	lnum;
int	bflag, cflag, fflag, lflag, nflag, vflag, xflag, yflag;
int	hflag	= 1;
int	sflag;
int	pflag;		/* david@ll-sst */
int	retcode = 0;
int	nfile;
long	blkno;
int	nsucc;
long	tln;
FILE	*wordf;
char	*argptr;

main(argc, argv)
char **argv;
{
	while (--argc > 0 && (++argv)[0][0]=='-')
		switch (argv[0][1]) {

		case 's':
			sflag++;
			continue;

		case 'h':
			hflag = 0;
			continue;

		case 'b':
			bflag++;
			continue;

		case 'c':
			cflag++;
			continue;

		case 'e':
			argc--;
			argv++;
			goto out;

		case 'f':
			fflag++;
			continue;

		case 'l':
			lflag++;
			continue;

		case 'n':
			nflag++;
			continue;

		case 'v':
			vflag++;
			continue;

		case 'x':
			xflag++;
			continue;

		case 'i':		/* Berkeley */
		case 'y':		/* Btl */
			yflag++;
			continue;

		case 'p':		/* david@ll-sst */
			pflag++;
			continue;

		default:
			fprintf(stderr, "fgrep: unknown flag\n");
			continue;
		}
out:
	if (argc<=0)
		exit(2);
	if (fflag) {
		wordf = fopen(*argv, "r");
		if (wordf==NULL) {
			fprintf(stderr, "fgrep: can't open %s\n", *argv);
			exit(2);
		}
	}
	else argptr = *argv;
	argc--;
	argv++;

	cgotofn();
	cfail();
	nfile = argc;
	if (argc<=0) {
		if (lflag) exit(1);
		execute((char *)NULL);
	}
	else while (--argc >= 0) {
		execute(*argv);
		argv++;
	}
	exit(retcode != 0 ? retcode : nsucc == 0);
}

# define ccomp(a,b) (yflag ? lca(a)==lca(b) : a==b)
# define lca(x) (isupper(x) ? tolower(x) : x)
execute(file)
char *file;
{
	register struct words *c;
	register ccount;
	register char ch;
	register char *p;
	char buf[2*BUFSIZ];
	int f;
	int failed;
	char *nlp;
	if (file) {
		if ((f = open(file, 0)) < 0) {
			fprintf(stderr, "fgrep: can't open %s\n", file);
			retcode = 2;
			return;
		}
	}
	else f = 0;
	ccount = 0;
	failed = 0;
	lnum = 1;
	tln = 0;
	blkno = 0;
	p = buf;
	nlp = p;
	c = w;
	for (;;) {
		if (--ccount <= 0) {
			if (p == &buf[2*BUFSIZ]) p = buf;
			if (p > &buf[BUFSIZ]) {
				if ((ccount = read(f, p, &buf[2*BUFSIZ] - p)) <= 0) break;
			}
			else if ((ccount = read(f, p, BUFSIZ)) <= 0) break;
			blkno += ccount;
		}
		nstate:
			if (ccomp(c->inp, *p)) {
				c = c->nst;
			}
			else if (c->link != 0) {
				c = c->link;
				goto nstate;
			}
			else {
				c = c->fail;
				failed = 1;
				if (c==0) {
					c = w;
					istate:
					if (ccomp(c->inp ,  *p)) {
						c = c->nst;
					}
					else if (c->link != 0) {
						c = c->link;
						goto istate;
					}
				}
				else goto nstate;
			}
		if (c->out) {
			while (*p++ != '\n') {
				if (--ccount <= 0) {
					if (p == &buf[2*BUFSIZ]) p = buf;
					if (p > &buf[BUFSIZ]) {
						if ((ccount = read(f, p, &buf[2&BUFSIZ] - p)) <= 0) break;
					}
					else if ((ccount = read(f, p, BUFSIZ)) <= 0) break;
					blkno += ccount;
				}
			}
			if ( (vflag && (failed == 0 || xflag == 0)) || (vflag == 0 && xflag && failed) )
				goto nomatch;
	succeed:	nsucc = 1;
			if (cflag) tln++;
			else if (sflag)
				;	/* ugh */
			else if (lflag) {
				printf("%s\n", file);
				close(f);
				return;
			}
			else {
				if ((nfile > 1 && hflag) || pflag)
					printf("%s:", file);
				if (bflag) printf("%ld:", (blkno-ccount-1)/BUFSIZ);
				if (nflag) printf("%ld:", lnum);
				if (p <= nlp) {
					while (nlp < &buf[2*BUFSIZ]) putchar(*nlp++);
					nlp = buf;
				}
				while (nlp < p) putchar(*nlp++);
			}
	nomatch:	lnum++;
			nlp = p;
			c = w;
			failed = 0;
			continue;
		}
		if (*p++ == '\n')
			if (vflag) goto succeed;
			else {
				lnum++;
				nlp = p;
				c = w;
				failed = 0;
			}
	}
	close(f);
	if (cflag) {
		if (nfile > 1)
			printf("%s:", file);
		printf("%ld\n", tln);
	}
}

getargc()
{
	register c;
	if (wordf)
		return(getc(wordf));
	if ((c = *argptr++) == '\0')
		return(EOF);
	return(c);
}

cgotofn() {
	register c;
	register struct words *s;

	s = smax = w;
nword:	for(;;) {
		c = getargc();
		if (c==EOF)
			return;
		if (c == '\n') {
			if (xflag) {
				for(;;) {
					if (s->inp == c) {
						s = s->nst;
						break;
					}
					if (s->inp == 0) goto nenter;
					if (s->link == 0) {
						if (smax >= &w[MAXSIZ -1]) overflo();
						s->link = ++smax;
						s = smax;
						goto nenter;
					}
					s = s->link;
				}
			}
			s->out = 1;
			s = w;
		} else {
		loop:	if (s->inp == c) {
				s = s->nst;
				continue;
			}
			if (s->inp == 0) goto enter;
			if (s->link == 0) {
				if (smax >= &w[MAXSIZ - 1]) overflo();
				s->link = ++smax;
				s = smax;
				goto enter;
			}
			s = s->link;
			goto loop;
		}
	}

	enter:
	do {
		s->inp = c;
		if (smax >= &w[MAXSIZ - 1]) overflo();
		s->nst = ++smax;
		s = smax;
	} while ((c = getargc()) != '\n' && c!=EOF);
	if (xflag) {
	nenter:	s->inp = '\n';
		if (smax >= &w[MAXSIZ -1]) overflo();
		s->nst = ++smax;
	}
	smax->out = 1;
	s = w;
	if (c != EOF)
		goto nword;
}

overflo() {
	fprintf(stderr, "wordlist too large\n");
	exit(2);
}
cfail() {
	struct words *queue[QSIZE];
	struct words **front, **rear;
	struct words *state;
	int bstart;
	register char c;
	register struct words *s;
	s = w;
	front = rear = queue;
init:	if ((s->inp) != 0) {
		*rear++ = s->nst;
		if (rear >= &queue[QSIZE - 1]) overflo();
	}
	if ((s = s->link) != 0) {
		goto init;
	}

	while (rear!=front) {
		s = *front;
		if (front == &queue[QSIZE-1])
			front = queue;
		else front++;
	cloop:	if ((c = s->inp) != 0) {
			bstart = 0;
			*rear = (q = s->nst);
			if (front < rear)
				if (rear >= &queue[QSIZE-1])
					if (front == queue) overflo();
					else rear = queue;
				else rear++;
			else
				if (++rear == front) overflo();
			state = s->fail;
		floop:	if (state == 0) {
				state = w;
				bstart = 1;
			}
			if (state->inp == c) {
			qloop:	q->fail = state->nst;
				if ((state->nst)->out == 1) q->out = 1;
				if ((q = q->link) != 0) goto qloop;
			}
			else if ((state = state->link) != 0)
				goto floop;
			else if(bstart == 0){
				state = 0;
				goto floop;
			}
		}
		if ((s = s->link) != 0)
			goto cloop;
	}
}
