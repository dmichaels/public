/* @(#)malloc.c	4.1 (Berkeley) 12/21/80 */

#include <stdio.h>

/*
*	C storage allocator
*
*	Circular first-fit strategy works with noncontiguous,
*	but monotonically linked arena.  Each block is preceded
*	by a pointer to the (pointer of the) next following block.
*	Blocks are exact number of words long aligned to the data
*	type requirements of ALIGN.  Pointers to blocks must have
*	BUSY bit 0 bit in pointer is 1 for busy, 0 for idle.  Gaps
*	in arena are merely noted as busy blocks.  Last block of arena
*	(pointed to by alloct) is empty and has a pointer to first.
*	Idle blocks are coalesced during space search
*
*	A different implementation may need to redefine
*	ALIGN, NALIGN, BLOCK, BUSY, INT where INT is
*	integer type to which a pointer can be cast.
*
*	Modified to be almost readable.
*	Put ifdef DEBUG blocks in.
*		-- David Michaels, May 1984
*/

#define DEBUG

#ifdef pdp11
#define GRANULE 64
#else
#define GRANULE 0
#endif

#define INT	int
#define ALIGN	int
#define NALIGN	1
#define WORD	sizeof(union store)
#define BLOCK	1024	/* multiple of WORD */
#define BUSY	1
#define NULL	0

#define is_busy(p)	((INT)(p) & BUSY)
#define set_busy(p)	(union store *)((INT)(p) | BUSY)
#define clr_busy(p)	(union store *)((INT)(p) & ~BUSY)

union store {
	union store *ptr;	/* addr ptr, bit 0 used as BUSY flag */
	ALIGN dummy[NALIGN];	/* for proper alignment of data */
	int   calloc;		/* calloc clears an array of integers */
};

static	union store allocs[2];	/* initial arena */
static	union store *allocp;	/* search ptr */
static	union store *alloct;	/* arena top */
static	union store *allocx;	/* for benefit of realloc */

char *
malloc(nbytes)
unsigned nbytes;
{
	char	*sbrk();
	register union store *p, *q;
	register nw;
	static temp;	/* coroutines assume no auto */

#ifdef DEBUG
	fprintf(stderr,"malloc: entering\n");
#endif DEBUG
	if(allocs[0].ptr == 0) {  /* first time */
#ifdef DEBUG
		fprintf(stderr,"malloc: first time\n");
		fprintf(stderr,"malloc:\tallocs[0].ptr = %u\n",allocs[0].ptr);
		fprintf(stderr,"malloc:\tallocs[1].ptr = %u\n",allocs[1].ptr);
#endif DEBUG
		allocs[0].ptr = set_busy(&allocs[1]);
		allocs[1].ptr = set_busy(&allocs[0]);
		alloct = &allocs[1];
		allocp = &allocs[0];
#ifdef DEBUG
		fprintf(stderr,"malloc:\tallocs[0].ptr = %u\n",allocs[0].ptr);
		fprintf(stderr,"malloc:\tallocs[1].ptr = %u\n",allocs[1].ptr);
		fprintf(stderr,"malloc:\talloct = %u\n",alloct);
		fprintf(stderr,"malloc:\tallocp = %u\n",allocp);
		fprintf(stderr,"malloc:\tallocs = %u\n",allocs);
#endif DEBUG
	}

	nw = (nbytes + WORD + WORD - 1) / WORD;
#ifdef DEBUG
	fprintf(stderr,"malloc: nw = %u\n",nw);
#endif DEBUG
	for(p = allocp ; ; ) {
#ifdef DEBUG
		fprintf(stderr,"malloc: 1st for..loop, p = %u\n",p);
#endif DEBUG
		for(temp = 0; ; ) {
#ifdef DEBUG
			fprintf(stderr,"malloc: 2nd for..loop\n");
			fprintf(stderr,
			"malloc: p->ptr = %u, is_busy = %s\n",
			p->ptr,is_busy(p->ptr) ? "TRUE" : "FALSE");
#endif DEBUG
			if(!is_busy(p->ptr)) {
				while(!is_busy((q = p->ptr)->ptr))
					p->ptr = q->ptr;
				if(q >= p + nw && p + nw >= p)
					goto found;
			}
			q = p;
			p = clr_busy(p->ptr);
#ifdef DEBUG
			fprintf(stderr,"malloc: p = %u\n",p);
			fprintf(stderr,"malloc: q = %u\n",q);
			fprintf(stderr,"malloc: p->ptr = %u\n",p->ptr);
			fprintf(stderr,"malloc: alloct = %u\n",alloct);
			fprintf(stderr,"malloc: allocp = %u\n",allocp);
			fprintf(stderr,"malloc: allocs = %u\n",allocs);
#endif DEBUG
			if(p > q)
				;
			else if(q != alloct || p != allocs)
				return(NULL);
			else if(++temp > 1)
				break;
		}
		temp = ((nw + BLOCK/WORD) / (BLOCK/WORD)) * (BLOCK/WORD);
		q = (union store *)sbrk(0);
		if(q + temp + GRANULE < q)
			return(NULL);
		q = (union store *)sbrk(temp*WORD);
		if((INT)q == -1)
			return(NULL);
		alloct->ptr = q;
		if(q != alloct + 1)
			alloct->ptr = set_busy(alloct->ptr);
		alloct = q->ptr = q + temp - 1;
		alloct->ptr = set_busy(allocs);
	}
found:
	allocp = p + nw;
	if(q > allocp) {
		allocx = allocp->ptr;
		allocp->ptr = p->ptr;
	}
	p->ptr = set_busy(allocp);
	return((char *)(p+1));
}


/*
*	Freeing strategy tuned for LIFO allocation
*/

free(ap)
register char *ap;
{
	register union store *p = (union store *)ap;

	allocp = --p;
	p->ptr = clr_busy(p->ptr);
}


/*
*	Realloc(p, nbytes) reallocates a block obtained from malloc()
*	and freed since last call of malloc() to have new size nbytes,
*	and old content returns new location, or 0 on failure.
*/

char *
realloc(p, nbytes)
register union store *p;
unsigned nbytes;
{
	register union store *q;
	union store *s, *t;
	register unsigned nw;
	unsigned onw;

	if(is_busy(p[-1].ptr))
		free((char *)p);
	onw = p[-1].ptr - p;
	q = (union store *)malloc(nbytes);
	if(q == NULL || q == p)
		return((char *)q);
	s = p;
	t = q;
	nw = (nbytes + WORD - 1) / WORD;
	if(nw < onw)
		onw = nw;
	while(onw-- != 0)
		*t++ = *s++;
	if(q < p && q + nw >= p)
		(q + (q + nw - p))->ptr = allocx;
	return((char *)q);
}
