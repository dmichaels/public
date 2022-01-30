/* malloc.c - Memory allocation/freeing services for vanilla UNIX */

/**********************************************************************
** This product is the property of Language Processors, Inc.         **
** and is licensed pursuant to a written license agreement.          **
** No portion of this product may be reproduced without the written  **
** permission of Language Processors, Inc. except pursuant to the    **
** license agreement.                                                **
**********************************************************************/

/* ====================================================================
** Implementation Notes
** ====================================================================
** This implementation will work only on or can be easily ported to
** systems which can maintain a contiguous memory area which may be
** dynamically extended contiguously and toward higher addresses!
** See the function sys_getmem().
** --------------------------------------------------------------------
** The implementation of this storage management system is very similar
** to the one desrcibed in "The Art of Computer Programming" Volume 1 -
** Fundamental Algorithms, Second Edition by Donald E. Knuth (Addison -
** Wesley 1973).  It can be found in section 2.5, algorithms A and C on
** pages 437 and 442.  The following is a general description.
** --------------------------------------------------------------------
** In this scheme, each block of memory consists of a header (described
** in the HEADER typedef below) which simply indicates the size of the
** block of memory in bytes and a flag indicating whether this block is
** free or allocated and a trailer at the end of the memory block which
** contains exactly the same data as the header.  In our case this is
** simply an long integer, the absolute value of which represents the
** size in bytes of the memory block (including the header and trailer
** sizes); a negative size indicates that the block is free and a
** positive size indicates that it is allocated.
** --------------------------------------------------------------------
** A doubly linked circular free list of available (free) memory
** blocks is maintained in no particular order.  The forward and
** backward links themselves are kept in the first two long words of
** the actual data space of the free block, which immmediately follows
** the the block header (the layout of the block header and the links
** is described in the BLOCK typedef below).  This can be done only
** because the block is free and thus that space is not being used
** by the caller.
** --------------------------------------------------------------------
** Blocks are allocated via a first-fit allocation strategy.  When
** a block is allocated, the header and trailer are simply set to
** indicate that it is no longer free (i.e. the size is set from
** negative to positive), and it is removed from the free list.
** --------------------------------------------------------------------
** When memory blocks are freed, they will be merged if possible with
** the previous and/or next blocks in memory if possible (i.e. if they
** are free); this will be easily facilitated by the block trailer
** (and the double links).
** --------------------------------------------------------------------
** In order to avoid the possibility of maintaining very small free
** list nodes, the constant ALLOC_BLOCK_SIZE is used to represent the
** smallest number of bytes we are willing to maintain in each free
** list node (i.e. we will allocate memory to the caller in chunks of
** ALLOC_BLOCK_SIZE bytes).  For this implementation, ALLOC_BLOCK_SIZE
** *must* be at least eight (8) bytes ((BLOCK_SIZE-HEADER_SIZE) bytes)
** since the forward and backward free list links are kept in the data
** space of the block.
* --------------------------------------------------------------------
** For each allocated block, at least (HEADER_SIZE + TRAILER_SIZE) and
** and at most (HEADER_SIZE + TRAILER_SIZE + (ALLOC_BLOCK_SIZE - 1)) 
** bytes could be wasted.
** --------------------------------------------------------------------
** The constant HEAP_BLOCK_SIZE represents the size in bytes in which
** chunks of new heap memory will be allocated from the system; the
** greater this size, the fewer the OS requests (system calls).
** --------------------------------------------------------------------
** Everything is (32-bit) long word aligned; this is reflected in
** BLOCK_SIZE, HEADER_SIZE, TRAILER_SIZE, ALLOC_BLOCK_SIZE, ALIGN,
** and ALIGN_SIZE.
** --------------------------------------------------------------------
** The global variable "HeapStart" is set to represent the address of
** the first byte of the heap space.  If we need to request more heap
** space from the OS, then a call to getmemory() will do the job; if
** there is not enough heap space "left-over" then an system request
** (i.e. sys_getmem()) will be made to extend the heap (in blocks of
** HEAP_BLOCK_SIZE), and any "left-over" heap space is kept track of
** accordingly.  The global variable "HeapEnd" represents the address
** of the first byte beyond the current end of our heap area.
** "HeapStart" and "HeapEnd" are both set by getmemory() the first
** time it's called. See sys_initmem() and sys_getmem() for more info.
** ================================================================== */

/* --------------------------------------------------------------------
** Conditional compilation definitions
** ------------------------------------------------------------------ */

#define DEBUG_ALLOC	/* Compile debug function */
#define OS_UNIX		/* Vanilla UNIX system */
#undef  OS_MSDOS	/* MS-DOS system (with Phar-Lap Extender) */

/* --------------------------------------------------------------------
** Imported external functions
** ------------------------------------------------------------------ */

/* --------------------------------------------------------------------
** Local type definitions
** ------------------------------------------------------------------ */

typedef char	BYTE;
typedef long	ALIGN;
typedef long	HEADER;		/* Size in bytes of memory block */
typedef long	TRAILER;	/* Size in bytes of memory block */

typedef struct block {
	HEADER		 size;	/* Size in bytes of memory block */
	struct block	*next;	/* Pointer to next memory block */
	struct block	*prev;	/* Pointer to previous memory block */
} BLOCK;

/* --------------------------------------------------------------------
** Exported functions
** ------------------------------------------------------------------ */

BYTE *calloc ();	/* Allocate N bytes and zero initialize */
void  free ();		/* Put back memory on free list */
BYTE *malloc ();	/* Allocate N bytes */
BYTE *realloc ();	/* Resize an area of memory */

#ifdef DEBUG_ALLOC

void  dump_alloc ();	/* Print debugging data */

#endif

/* --------------------------------------------------------------------
** Local (static) function declarations
** ------------------------------------------------------------------ */

int   sys_initmem ();
BYTE *sys_getmem ();
BYTE *getmemory ();

/* --------------------------------------------------------------------
** Local definitions
** ------------------------------------------------------------------ */

/* Tunable parameters */

#define HEAP_BLOCK_SIZE		(8192)	/* Bytes */
#define ALLOC_BLOCK_SIZE	(8)	/* Bytes */

/* Alignment and other size data */

#define ALIGN_SIZE	sizeof(ALIGN)
#define BLOCK_SIZE	sizeof(BLOCK)
#define HEADER_SIZE	sizeof(HEADER)
#define TRAILER_SIZE	sizeof(TRAILER)
#define OVERHEAD_SIZE	(HEADER_SIZE + TRAILER_SIZE)
#define MIN_BLOCK_SIZE	(OVERHEAD_SIZE + ALLOC_BLOCK_SIZE)

/* Miscellaneous */

#define NULL		((void *)0)

/* --------------------------------------------------------------------
** Macro functions
** ------------------------------------------------------------------ */

/* --------------------------------------------------------------------
** block_size(bp)	 - Return the "size" field of the block
**			   pointed to by "bp".  This represents the
**			   size in bytes of the data area as plus
**			   the size of the header and trailer data
**			   (i.e. OVERHEAD_SIZE).  If the block is
**			   on the free list then it is negative; if
**			   it is on the allocated list it is positive.
** next_block(bp)	 - Return the "next" link field of the block
**			   pointed to by "bp".
** prev_block(bp)	 - Return the "prev" link field of the block
**			   pointed to by "bp".
** data_block(bp)	 - Return a pointer to the data block of the
**			   memory block pointed to by "bp".
** --------------------------------------------------------------------
** alloc_block_size(bp)	 - Return the absolute size in bytes of the
**			   allocated block pointed to by "bp" (i.e.
**			   it is assumed that "bp" points to an
**			   allocated block).
** free_block_size(bp)	 - Return the absolute size in bytes of the
**			   free block pointed to by "bp" (i.e. it is
**			   assumed that "bp" points to a free block).
** --------------------------------------------------------------------
** is_alloc(bp)		 - True if the block pointed to by "bp" is
**			   allocated, otherwise false.
** is_free(bp)		 - True if the block pointed to by "bp" is
**			   free, otherwise false.
** --------------------------------------------------------------------
** set_alloc_block(bp,n) - Set both the header and trailer data of
**			   the block pointed to by "bp" to represent
**			   an allocated block of size "n" bytes.
** set_free_block(bp,n)	 - Set both the header and trailer data of
**			   the block pointed to by "bp" to represent
**			   a free block of size "n" bytes (positive).
** --------------------------------------------------------------------
** prev_is_free(bp)	 - True if previous memory block is free.
** prev_free_block(bp)	 - Return a pointer to the previous free
**			   memory block (i.e. assumes that the
**			   previous memory block is free).
** prev_free_size(bp)	 - Return the size of the previous free
**			   memory block (i.e. assumes that the
**			   previous memory block is free).
** --------------------------------------------------------------------
** next_alloc_block(bp)	 - Return a pointer to the next memory block;
**			   it is assumed that the block pointed to by
**			   "bp" is an allocted block.
** next_free_block(bp)	 - Return a pointer to the next memory block;
**			   it is assumed that the block pointed to by
**			   "bp" is a free block (unlike prev_free_block).
** --------------------------------------------------------------------
** block_start(dp)	 - Return a pointer to the start of the
**			   memory block header given a pointer "dp"
**			   to the data block of the memory block.
** data_block_size(dp)	 - Return the size of the data block pointed
**			   to by "dp".
** --------------------------------------------------------------------
** in_heap(p)		 - Return true if the given pointer address is
**			   within our heap space, otherwise false.
** --------------------------------------------------------------------
** round_up(x,y)	 - Round "x" up to the nearest multiple of "y".
** ------------------------------------------------------------------ */

#define block_size(bp)		(((BLOCK *)(bp))->size)
#define next_block(bp)		(((BLOCK *)(bp))->next)
#define prev_block(bp)		(((BLOCK *)(bp))->prev)
#define data_block(bp)		((BYTE *)((BYTE *)(bp) + HEADER_SIZE))

#define alloc_block_size(bp)	(block_size(bp))
#define free_block_size(bp)	(-block_size(bp))

#define is_alloc(bp)		(block_size(bp) > 0)
#define is_free(bp)		(block_size(bp) < 0)

#define set_alloc_block(bp,n)	(*(TRAILER *)((BYTE *)(bp) + \
				 (n) - TRAILER_SIZE) = (TRAILER)(n), \
				 block_size(bp) = (HEADER)(n))
#define set_free_block(bp,n)	(*(TRAILER *)((BYTE *)(bp) + \
				 (n) - TRAILER_SIZE) = -(TRAILER)(n), \
				 block_size(bp) = -(HEADER)(n))

#define prev_is_free(bp)	(*(TRAILER *)((BYTE *)(bp) - TRAILER_SIZE) < 0)
#define prev_free_block(bp)	((BLOCK *)((BYTE *)(bp) + \
				 *(TRAILER *)((BYTE *)(bp) - TRAILER_SIZE)))
#define prev_free_size(bp)	(-(*(TRAILER *)((BYTE *)(bp) - TRAILER_SIZE)))

#define next_free_block(bp)	((BLOCK *)((BYTE *)(bp) + free_block_size(bp)))
#define next_alloc_block(bp)	((BLOCK *)((BYTE *)(bp) + alloc_block_size(bp)))

#define block_start(dp)		((BLOCK *)((BYTE *)(dp) - HEADER_SIZE))
#define data_block_size(dp)	(block_size(block_start(dp)) - OVERHEAD_SIZE)

#define in_heap(p)		(((BYTE *)(p) >= HeapStart) && \
				 ((BYTE *)(p) < HeapEnd))

#define round_up(x,y)		((((x) + (y) - 1) / (y)) * (y))

/* --------------------------------------------------------------------
** Local static data
** ------------------------------------------------------------------ */

static BYTE  Dummy 	    = (BYTE)0;

static BLOCK DummyFreeBlock = {	0L,
				(BLOCK *)(&DummyFreeBlock),
				(BLOCK *)(&DummyFreeBlock)
			      };

static BLOCK *FreeList	    = (BLOCK *)(&DummyFreeBlock);

static BYTE  *HeapStart	    = (BYTE *)NULL;
static BYTE  *HeapEnd	    = (BYTE *)NULL;

/* --------------------------------------------------------------------
** sys_getmem   **SYSTEM-DEPENDENT**
**
** This function assumes that the system can maintain a contiguous
** memory area which may be dynamically extended for use by the
** calling program at program execution time.  This memory area is
** known as the heap.  It further assumes that the memory (when
** extended) will be extended toward higher addresses.  At any given
** time, the first address beyond the end of this heap memory area
** is known as the break value.
**
** This function will cause the system to add the value of the nbytes
** argument to the break value, and to change the allocated heap space
** accordingly.  That is, nbytes will be added to the heap space.
** If nbytes is zero, then no memory will be obtained.
**
** The break value at the time of the call to this function be
** returned in the break argument.  That is, a pointer to the
** newly allocated (extended) area will be returned in break.
**
** If no more heap memory can be obtained, then return NULL.
** ------------------------------------------------------------------ */

#ifndef OS_MSDOS

/* ---------------------
 * Vanilla UNIX version.
 * ------------------- */

static
BYTE *
sys_getmem (nbytes)
int nbytes;
{
	long int p;

	if ((p = sbrk(nbytes)) == -1L)
		return ((BYTE *)NULL);
	return ((BYTE *)p);
}

#else

/* ---------------
 * MS-DOS version.
 * ------------- */

static
BYTE *
sys_getmem (nbytes)
int nbytes;
{
	return ((BYTE *)NULL);
}

#endif  /* OS_MSDOS */

/* --------------------------------------------------------------------
** sys_initmem   **SYSTEM-DEPENDENT**
**
** Do any necessary heap management initialization and returns in
** "start", the address of the bottom (i.e. the start) of heap memory,
** and returns in "end", the address of the end of heap memory.
** Return 1 if all is well, otherwise return 0.
**
** NOTE: This routine MUST be called ONCE AND ONLY ONCE at
**       start-up time (before any allocations are done).
** ------------------------------------------------------------------ */

#ifndef OS_MSDOS

/* ---------------------
 * Vanilla UNIX version.
 * ------------------- */

static
int
sys_initmem (start, end)
BYTE **start;
BYTE **end;
{
	unsigned long int n;

	/* -------------------------------------------
	 * Determine the start address of heap memory.
	 * ----------------------------------------- */

	if ((*start == (BYTE *)NULL) &&
	    ((*start = sys_getmem(0)) == (BYTE *)NULL))
		return (0);

	/* ---------------------------------------------
	 * Make sure the heap start is properly aligned.
	 * ------------------------------------------- */

	if (((n = ((long)(*start) % ALIGN_SIZE)) != 0L) &&
	    ((*start = sys_getmem(n)) == (BYTE *)NULL))
		return (0);

	/* -------------------------------------------------
	 * Determine the current end address of heap memory.
	 * ----------------------------------------------- */

	*end = *start;

	return (1);
}

#else

/* ---------------
 * MS-DOS version.
 * ------------- */

static
int
sys_initmem (start, end)
BYTE **start;
BYTE **end;
{
	/* -------------------------------------------
	 * Determine the start address of heap memory.
	 * ----------------------------------------- */

	if (*start == (BYTE *)NULL)
		*start = (BYTE *)_mem_bot();

	/* -------------------------------------------------
	 * Determine the current end address of heap memory.
	 * ----------------------------------------------- */

	*end = (BYTE *)_mem_top();

	/* ---------------------------------------------
	 * Make sure the heap start is properly aligned.
	 * ------------------------------------------- */

	if ((n = ((long)(*start) % ALIGN_SIZE)) != 0L) {
		if ((*start + n) <= *end)
			*start += n;
		else	return (0);
	}

	return (1);
}

#endif  /* OS_MSDOS */

/* --------------------------------------------------------------------
** getmemory
**
** Extend the system heap by "nbytes" bytes and return a pointer to
** the start of the new area.  If enough memory could not be allocated
** then return NULL.  Also, update the global variable "HeapEnd" to
** point to the end of our heap space (i.e. the address of the first
** byte past the end of the heap).
** ------------------------------------------------------------------ */

static
BYTE *
getmemory (nbytes)
register long nbytes;
{
	static unsigned long int	heap_left_over = 0L;
	static int			first_time = 1;
	register BYTE			*p;
	register long int		nbytes_alloc;

	/* Initialize this sucker */

	if (first_time) {
		if (!sys_initmem(&HeapStart,&HeapEnd))
			return ((BYTE *)NULL);
		if (HeapStart < HeapStart) {
			heap_left_over = (HeapEnd - HeapStart);
			HeapEnd = HeapStart;
		}
		first_time = 0;
	}

	/* We have more than enough or exactly enough left overs */

	if (nbytes <= heap_left_over) {
		heap_left_over -= nbytes;
		p = HeapEnd;
		HeapEnd = p + nbytes;
		return (p);
	}

	/* We don't have enough (or any) left overs */

	nbytes_alloc = round_up(nbytes, HEAP_BLOCK_SIZE);

	if ((long)(p = (BYTE *)sys_getmem(nbytes_alloc)) == 0L)
		return ((BYTE *)NULL);

	/* We had some (but not enough) leftovers */

	if (heap_left_over > 0L)
		p = HeapEnd;

	heap_left_over = (heap_left_over + nbytes_alloc) - nbytes;
	HeapEnd = p + nbytes;
	return (p);
}

/* --------------------------------------------------------------------
** malloc
**
** Allocates and returns a pointer to a block of at least "nbytes"
** bytes of memory beginning on an ALIGN_SIZE byte boundary.
** If enough memory can't be allocated, return a NULL pointer.
** ------------------------------------------------------------------ */

BYTE *
malloc (nbytes)
long nbytes;
{
	register BLOCK	*p, *q;
	register long	allocsize, newsize;

	/* --------------
	 * Special cases.
	 * ------------ */

	if (nbytes < 0)
		return ((BYTE *)NULL);

	if (nbytes == 0)
		return ((BYTE *)&Dummy);

	/* ---------------------------------------------------------
	 * Figure out the total number of bytes we need to allocate.
	 * ------------------------------------------------------- */

	allocsize = round_up(nbytes, ALLOC_BLOCK_SIZE) + OVERHEAD_SIZE;

	/* ---------------------------
	 * Loop through the free list.
	 * ------------------------- */

	for (p = next_block(FreeList) ; p != FreeList ; p = next_block(p)) {

		if (free_block_size(p) < allocsize)
			continue;

		if ((free_block_size(p) > allocsize) &&
		    ((newsize = free_block_size(p) - allocsize) >=
		     MIN_BLOCK_SIZE)) {

			/* ----------------------------------------------
			 * This block will fit; shave the existing block.
			 * -------------------------------------------- */

			/* Point "q" to the block we're allocating */

			q = p;

			/* Point "p" to the new free block */

			p = (BLOCK *)((BYTE *)p + allocsize);

			/* Set up the new smaller free block */

			set_free_block (p, newsize);

			/* Link the new smaller free block into the free list */

			next_block(p) = next_block(q);
			prev_block(p) = prev_block(q);
			prev_block(next_block(p)) = p;
			next_block(prev_block(p)) = p;

			/* Designate this as an allocated block */

			set_alloc_block (q, allocsize);

			return (data_block(q));
		}

		if (free_block_size(p) == allocsize) {

			/* -------------------
			 * Exact fit. Grab it!
			 * ----------------- */

			/* Unlink this block from the free list */

			prev_block(next_block(p)) = prev_block(p);
			next_block(prev_block(p)) = next_block(p);

			/* Designate this as an allocated block */

			set_alloc_block (p, free_block_size(p));

			return (data_block(p));
		}
	}

	/* -----------------------------------
	 * No free blocks big enough.
	 * Try to get more memory from the OS.
	 * --------------------------------- */

	if ((p = (BLOCK *)getmemory(allocsize)) == (BLOCK *)NULL)
		return ((BYTE *)NULL);

	/* Designate this as an allocated block */

	set_alloc_block (p, allocsize);

	return (data_block(p));
}

/* --------------------------------------------------------------------
** free
**
** Frees the memory (which was obtained by a previous call to "malloc")
** pointed to by "ptr".
** ------------------------------------------------------------------ */

void
free (ptr)
BYTE *ptr;
{
	register BLOCK	*p, *q;
	register long	newsize;

	if (ptr == (BYTE *)NULL)
		return;

	/* Adjust "p" to actual block start */

	p = block_start(ptr);

	/* Make sure this block was previously allocated */

	if (!is_alloc(p) || !in_heap(p))
		return;

	/* -----------------------------------------------------
	 * Try to merge this block with neighboring free blocks.
	 * --------------------------------------------------- */

	if (next_block(FreeList) == FreeList) {

		/* ----------------------------------
		 * There are no free blocks to merge.
		 * -------------------------------- */

		/* Designate this as a free block */

		set_free_block (p, alloc_block_size(p));

		/* Initialize the free list pointer to this block */

		next_block(p) = FreeList;
		prev_block(p) = FreeList;
		next_block(FreeList) = p;
		prev_block(FreeList) = p;
		return;
	}

	/* --------------------------------------------------------
	 * Check if the block in front of this block is free,
	 * and merge it in if so; be careful of lower memory bound.
	 * ------------------------------------------------------- */

	if ((p > (BLOCK *)HeapStart) && prev_is_free(p)) {

		/* -------------------------------------------
		 * Lower block is free merge this one into it.
		 * ----------------------------------------- */

		/* Get the size of this new larger free block */

		newsize = prev_free_size(p) + alloc_block_size(p);

		/* Point to the lower free block */

		p = prev_free_block(p);

		/* Set up this larger free block */

		set_free_block (p, newsize);
	}
	else {
		/* ---------------------------
		 * No lower block to merge in.
		 * ------------------------- */

		/* Thread this baby into the free list */

		set_free_block (p, alloc_block_size(p));
		next_block(p) = next_block(FreeList);
		prev_block(p) = FreeList;
		prev_block(next_block(FreeList)) = p;
		next_block(FreeList) = p;
	}

	/* -------------------------------------------------
	 * Now "p" points to a free block in the free list.
	 * See if there is an upper block we can merge with.
	 * ----------------------------------------------- */

	if (((q = next_free_block(p)) < (BLOCK *)HeapEnd) && is_free(q)) {

		/* -----------------------------------
		 * Merge this block with higher block.
		 * --------------------------------- */

		/* Set up this this larger free block */

		newsize = free_block_size(p) + free_block_size(q);
		set_free_block (p, newsize);

		/* Unlink the upper block */

		/*
		 * Note that this maneuver below (i.e. removing an
		 * essentially random node from the free list) is the
		 * ONLY reason the free list needs to be doubly linked!
		 */

		prev_block(next_block(q)) = prev_block(q);
		next_block(prev_block(q)) = next_block(q);
	}
	return;
}

/* --------------------------------------------------------------------
** calloc
**
** Allocates memory for an array of "nelem" elements of size "nbytes",
** initializes the memory to zeros, and returns a pointer to the
** initialized block.  If enough memory can't be allocated, return NULL.
** ------------------------------------------------------------------ */

BYTE *
calloc (nelem, nbytes)
long nelem, nbytes;
{
	register BYTE	*p;
	register long	n;

	p = malloc (nelem * nbytes);
	for (n = 0 ; n < (nelem * nbytes) ; n++)
		*(p + n) = (BYTE)0;
	return (p);
}

/* --------------------------------------------------------------------
** realloc
**
** Changes the size of the block referenced by "ptr" to size "nbytes"
** bytes and returns a pointer to the (probably moved) block.
** If enough memory can't be allocated, return NULL.
** ------------------------------------------------------------------ */

BYTE *
realloc (ptr, nbytes)
BYTE *ptr;
long nbytes;
{
	register BYTE	*newptr;
	register long	n, oldsize;

	newptr = malloc (nbytes);
	oldsize = data_block_size(ptr);
	for (n = 0; n < oldsize ; n++)
		*(newptr + n) = *(ptr + n);
	free (ptr);
	return (newptr);
}

#ifdef DEBUG_ALLOC

/* --------------------------------------------------------------------
** debug_alloc
**
** Print debugging data.
** ------------------------------------------------------------------ */

void
dump_alloc ()
{
	register BLOCK *p;

	printf ("\nFree List:\n\n");
	if (FreeList == (BLOCK *)NULL) {
		printf (" [null]\n");
		goto FreeListDone;
	}
	if (FreeList == next_block(FreeList)) {
		printf (" [empty]\n");
		goto FreeListDone;
	}
	printf (" %s: 0x%lx %s: %ld %s: 0x%lx %s: 0x%lx\n",
		"dummy" , FreeList,
		"size" , block_size(FreeList),
		"next" , next_block(FreeList),
		"prev" , prev_block(FreeList));
	for (p = next_block(FreeList) ; p != FreeList ; p = next_block(p))
		printf (" %s: 0x%lx %s: %ld %s: 0x%lx %s: 0x%lx\n",
			"free" , p,
			"size" , block_size(p),
			"next" , next_block(p),
			"prev" , prev_block(p));

	FreeListDone:

	printf ("\nMemory Map (Heap: 0x%lx ~ 0x%lx):\n\n", HeapStart, HeapEnd);
	for (p = (BLOCK *)HeapStart ; p < (BLOCK *)HeapEnd ; ) {
		printf (" 0x%lx size: ", p);
		if (is_free(p)) {
			printf ("%ld -free\n", free_block_size(p));
			p = next_free_block(p);
		}
		else {
			printf ("%ld -used\n", alloc_block_size(p));
			p = next_alloc_block(p);
		}
	}
	printf ("\n");
}

#endif /* DEBUG_ALLOC */
