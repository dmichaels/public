/* malloc.c - Memory allocation/freeing services for vanilla UNIX */

/***********************************************************************
 * This product is the property of Language Processors, Inc. and is    *
 * licensed pursuant to a written license agreement.  No portion of    *
 * this product may be reproduced without the written permission of    *
 * Language Processors, Inc. except pursuant to the license agreement. *
 ***********************************************************************/

/***********************************************************************
 *
 *  LPI EDIT HISTORY
 *
 *  06.29.88  DGM  Original.
 *
 ***********************************************************************/

/* ---------------------------------------------------------------------
** Conditional compilation definitions
** ------------------------------------------------------------------- */

#define	CC_DEBUG_ALLOC		/* Compile debug function? */
#define	CC_OS_UNIX		/* Vanilla UNIX system? */
#undef	CC_UNION_INITIAL	/* Union initialization possible? */

/* ---------------------------------------------------------------------
** Local type definitions
** ------------------------------------------------------------------- */

typedef long int		ALIGN;
typedef char			BYTE;
typedef unsigned long int	SIZE;

typedef union data_header	DATA_HEADER;
typedef struct block_header	BLOCK_HEADER;
typedef BLOCK_HEADER		BLOCK;

union data_header {		/* Header of data portion of block */
	SIZE		size;	/* Total block size in BLOCK_UNIT_SIZE units */
	ALIGN		dummy;	/* Dummy alignment field */
};

typedef struct block_header {	/* Header of the total free block */
	DATA_HEADER	data;	/* Header of data portion of block */
	BLOCK_HEADER	*next;	/* Pointer to next free block */
};

/* ---------------------------------------------------------------------
** Exported external functions
** ------------------------------------------------------------------- */

BYTE	* calloc ();	/* Allocate N bytes and zero initialize */
void	  free ();	/* Put back memory on free list */
BYTE	* malloc ();	/* Allocate N bytes */
BYTE	* realloc ();	/* Resize an area of memory */

#ifdef CC_DEBUG_ALLOC

void  debug_alloc ();	/* Print debugging data */

#endif

/* ---------------------------------------------------------------------
** Imported external functions
** ------------------------------------------------------------------- */

/* None */

/* ---------------------------------------------------------------------
** Local definitions
** ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
 * MIN_UNITS_TO_ALLOC represents the minimum number of units (each of
 * BLOCK_UNIT_SIZE bytes in size), which will be requested at a time
 * from the system (via sys_getmem()).  This is done for performance
 * reasons since this sort of system request is relatively expensive.
 * ---------------------------------------------------------------------
 * BLOCK_UNIT_SIZE is the (minimum) size in bytes of the units of
 * memory which will be dealt with.  Due to alignment constraints,
 * as well as to simplify allocation of small block from larger
 * blocks, and to minimize storage waste, this must correspond
 * exactly to sizeof(BLOCK).
 * ---------------------------------------------------------------------
 * OVERHEAD_SIZE is the minimum amount of memory wasted by each
 * allocation requested by the caller.  The maximum that could
 * be wasted is ((BLOCK_UNIT_SIZE - 1) + OVERHEAD_SIZE).
 * ---------------------------------------------------------------------
 * SIZE_MAX is the maximum value that a type SIZE can hold.
 * ---------------------------------------------------------------------
 * SIZE_FIELD_MAX represents the largest value which may legally exist
 * in the size field of a block.  Since this field represents size in
 * terms of multiples of BLOCK_UNIT_SIZE and is of type SIZE, its
 * maximum is (obviously) BLOCK_UNIT_SIZE times smaller than the
 * maximum value that size field may possibly contain.  This will
 * used to aid in error checking in free().
 * ------------------------------------------------------------------- */

#define MIN_UNITS_TO_ALLOC	(1024)
#define BLOCK_UNIT_SIZE		(sizeof(BLOCK))
#define OVERHEAD_SIZE		(sizeof(DATA_HEADER))

#define SIZE_MAX		((SIZE)4292967295)
#define SIZE_FIELD_MAX		((SIZE)(SIZE_MAX / BLOCK_UNIT_SIZE))

#define NULL			(0)

/* ---------------------------------------------------------------------
** Local static data
** ------------------------------------------------------------------- */

#ifdef CC_UNION_INITIAL

static BLOCK	DummyFreeBlock = { { (SIZE)0 }, (BLOCK *)(&FreeList) };
static BLOCK	*FreeList = (BLOCK *)(&DummyFreeBlock);

#else

static BLOCK	 DummyFreeBlock;
static BLOCK	*FreeList = (BLOCK *)NULL;

static SIZE	 LargestBlockEverAllocated = (SIZE)0;

#endif /* CC_UNION_INITIAL */

/* ---------------------------------------------------------------------
** Macro functions
** ------------------------------------------------------------------- */

/* ---------------------------------------------------------------------
 * block_size(bp)	- Return the "size" field of the block pointed
 *			  to by "bp".  This represents the size in
 *			  BLOCK_UNIT_SIZE byte units of the total block
 *			  including the data area as well as the size
 *			  of the block header (i.e. sizeof(BLOCK)).
 * next_block(bp)	- Return the "next" link field of the block
 *			  pointed to by "bp".  This should point to the
 *			  next free block on the free list; this is a
 *			  circularly linked list of free blocks which
 *			  is in order of increasing storage addresses.
 * data_block(bp)	- Return a pointer to the user data block
 *			  portion of the block pointed to by "bp".
 * data_size(bp)	- Return the size in bytes of the data block
 *			  portion of the block pointed to by "bp".
 * ---------------------------------------------------------------------
 * block_header(dp)	- Given a pointer to a data block which was
 *			  previously returned by a malloc(), return a
 *			  pointer to the start of its block header.
 * ---------------------------------------------------------------------
 * round_up(x,y)	 - Round "x" up to units of "y".
 * ------------------------------------------------------------------- */

#define block_size(bp)		(((BLOCK *)(bp))->data.size)
#define next_block(bp)		(((BLOCK *)(bp))->next)
#define data_block(bp)		((BYTE *)((DATA_HEADER *)(bp) + 1))
#define data_size(bp)		((block_size(bp) * BLOCK_UNIT_SIZE) \
				 - OVERHEAD_SIZE)
#define block_header(dp)	((BLOCK *)((DATA_HEADER *)(dp) - 1))
#define round_up(x,y)		(((x) + (y) - 1) / (y))

/* ---------------------------------------------------------------------
** sys_getmem   **SYSTEM-DEPENDENT**
**
** Obtains exactly "nbytes" bytes of contifuous memory from the
** system.  A pointer to (i.e. the address of) the first (i.e.
** lowest addressed) byte in the memory block will be returned.
** The byte in the memory block is guaranteed to be aligned on
** a sizeof(ALIGN) byte boundary.
** ------------------------------------------------------------------- */

#ifdef CC_OS_UNIX

/* ---------------------
 * Vanilla UNIX version.
 * ------------------- */

static
BYTE *
sys_getmem (nbytes)
SIZE nbytes;
{
	extern long int sbrk ();
	long int p;

	/* TODO: check for proper alignment */

	if ((p = sbrk((int)nbytes)) == -1L)
		return ((BYTE *)NULL);
	return ((BYTE *)p);
}

#else

static
BYTE *
sys_getmem (nbytes)
SIZE nbytes;
{
	return ((BYTE *)NULL);
}

#endif /* CC_OS_UNIX */

/* ---------------------------------------------------------------------
** getmemory
** ------------------------------------------------------------------- */

static
BLOCK *
getmemory (nunits)
register SIZE nunits;
{
	register BLOCK	*p;
	register SIZE	size;

	if (nunits < MIN_UNITS_TO_ALLOC)
		nunits = MIN_UNITS_TO_ALLOC;
	p = (BLOCK *)sys_getmem(nunits * BLOCK_UNIT_SIZE);
	if (p == (BLOCK *)NULL)
		return ((BLOCK *)NULL);
	block_size(p) = nunits;
	return (p);
}

/* ---------------------------------------------------------------------
** malloc
**
** Allocates and returns a pointer to a block of at least "nbytes"
** bytes of memory beginning on a sizeof(ALIGN) byte boundary.
** If enough memory can't be allocated, return a NULL pointer.
** ------------------------------------------------------------------- */

BYTE *
malloc (nbytes)
SIZE nbytes;
{
	register BLOCK	*p, *q;
	register SIZE	size;

	/* --------------
	 * Special cases.
	 * ------------ */

	if (nbytes <= 0)
		return ((BYTE *)NULL);

	/* -----------
	 * Initialize.
	 * --------- */

#ifndef CC_UNION_INITIAL

	if (FreeList == (BLOCK *)NULL) {
		FreeList = &DummyFreeBlock;
		block_size(FreeList) = (SIZE)0;
		next_block(FreeList) = FreeList;
	}

#endif  /* CC_UNION_INITIAL */

	/* ---------------------------------------------------
	 * Get the total number of units (each of which is
	 * BLOCK_UNIT_SIZE bytes in size) we need to allocate.
	 * ------------------------------------------------- */

	size = round_up (nbytes, BLOCK_UNIT_SIZE);

	/* ----------------------------------------------
	 * Search through the free list.
	 * At any point, "p" is previous and "q" current.
	 * -------------------------------------------- */

	for (p = FreeList, q = next_block(p) ; 1 ; p = q, q = next_block(q)) {

		/* ----------------------------------------------
		 * This block will fit; shave the existing block.
		 * -------------------------------------------- */

		if (block_size(q) > size) {
			block_size(q) -= size;
			q += block_size(q);
			block_size(q) = size;
			break;
		}

		/* ------------------------------------
		 * This block is an exact fit. Grab it!
		 * ---------------------------------- */

		else if (block_size(q) == size) {
			next_block(p) = next_block(q);
			break;
		}

		/* --------------------------------------------------
		 * We went through the entire free list and couldn't
		 * allocate a block; get more memory from the system.
		 * ------------------------------------------------ */

		else if (q == FreeList) {
			if ((q = getmemory(size)) == (BLOCK *)NULL)
				return ((BYTE *)NULL);
			free (data_block(q));
		}
	}

	/* ------------------------------------------------------
	 * Point the free list head the block which preceded
	 * the block which we just allocated (or allocated from).
	 * This may aid in keeping the list homogeneous.
	 * ---------------------------------------------------- */

	FreeList = p;

	if (size > LargestBlockEverAllocated)
		LargestBlockEverAllocated = size;

	/* --------------------------------------------------
	 * Return a pointer to the data portion of the block.
	 * ------------------------------------------------ */

	return (data_block(q));
}

/* ---------------------------------------------------------------------
** free
**
** Frees the memory (which is assumed to have been obtained
** by a previous call to malloc()) pointed to by "ptr".
** ------------------------------------------------------------------- */

void
free (ptr)
BYTE *ptr;
{
	register BLOCK	*q, *block;
	register SIZE	size;

	/* -------------
	 * Special case.
	 * ----------- */

	if (ptr == (BYTE *)NULL)
		return;

	/* ------------------------------------------------------
	 * Adjust "block" to to point to the actual block header.
	 * ---------------------------------------------------- */

	block = block_header(ptr);

	/* ------------------------------------------------------------
	 * Check for a bad value in the size field.  If there is, then
	 * either this block was not allocated via malloc() or the
	 * caller fucked up an overwrote it somehow.  In either case,
	 * we're not gonna corrupt our free list, so we just ignore it.
	 * ---------------------------------------------------------- */

	if (((size = block_size(block)) == (SIZE)0) ||
	    (size > SIZE_FIELD_MAX))
		return;
	
	if ((FreeList != next_block(FreeList)) &&
	    (size > LargestBlockEverAllocated))
		return;

	/* -----------------------------------------------
	 * Search through the free list (whose blocks are
	 * kept in order of increasing address) to find
	 * the appropriate place to insert the free block.  
	 * --------------------------------------------- */

	for (q = FreeList ; 1 ; q = next_block(q)) {
		if (((q >= next_block(q)) &&
		     ((block > q) || (block < next_block(q)))) ||
		    ((block > q) && (block < next_block(q))))
			break;
	}

	/* --------------------------------------------------
	 * Here, "q" points at the block in the free list
	 * after which the new free block should be inserted.
	 * See if we can do any upper/lower block merging.
	 * ------------------------------------------------ */

	/* Can we merge in with the upper block? */

	if ((block + size) == next_block(q)) {
		block_size(block) += block_size(next_block(q));
		next_block(block) = next_block(next_block(q));
	}
	else	next_block(block) = next_block(q);

	/* Can we merge in with the lower block? */

	if ((q + block_size(q)) == block) {
		block_size(q) += size;
		next_block(q) = next_block(block);
	}
	else	next_block(q) = block;

	FreeList = q;
}

/* ---------------------------------------------------------------------
** calloc
**
** Allocates memory for an array of "nelem" elements of size "size"
** bytes, initializes the memory to zeros, and returns a pointer to the
** initialized block.  If enough memory can't be allocated, return NULL.
** ------------------------------------------------------------------- */

BYTE *
calloc (nelem, size)
SIZE nelem, size;
{
	register BYTE	*q;
	register SIZE	n;

	if ((q = malloc (nelem * size)) == (BYTE *)NULL)
		return ((BYTE *)NULL);
	for (n = 0 ; n < (nelem * size) ; n++)
		*(q + n) = (BYTE)0;
	return (q);
}

/* ---------------------------------------------------------------------
** realloc
**
** Changes the size of the block referenced by "ptr" to size "nbytes"
** bytes and returns a pointer to the (probably moved) block.
** If enough memory can't be allocated, return NULL.
** ------------------------------------------------------------------- */

BYTE *
realloc (ptr, nbytes)
BYTE *ptr;
SIZE nbytes;
{
	register BYTE	*q;
	register SIZE	n, oldsize;

	if ((q = malloc (nbytes)) == (BYTE *)NULL)
		return ((BYTE *)NULL);
	oldsize = data_size(block_header(ptr));
	for (n = 0; n < oldsize ; n++)
		*(q + n) = *(ptr + n);
	free (ptr);
	return (q);
}

#ifdef CC_DEBUG_ALLOC

/* ---------------------------------------------------------------------
** debug_alloc
**
** Print debugging data.
** ------------------------------------------------------------------- */

void
debug_alloc ()
{
	register BLOCK *list, *p;

	printf ("\n*** LPI malloc data ***\n");
	printf ("\n*** alignment: %d byte boundary", sizeof(ALIGN));
	printf ("\n*** granularity: %d byte blocks\n\n", BLOCK_UNIT_SIZE);
	if ((list = FreeList) == (BLOCK *)NULL) {
		printf ("*** null free list\n\n");
		return;
	}
	if (list == next_block(list)) {
		printf ("*** empty free list\n\n");
		return;
	}
	for (p = next_block(list) ; 1 ; p = next_block(p)) {
		if (next_block(p) <= p) {
			list = next_block(p);
			break;
		}
	}
	for (p = list ; 1 ; ) {
		printf (
		"*** %s: 0x%lx %s: %4ld (%4ld bytes) %s: %lx\n",
		"free" , p,
		"size" , block_size(p), block_size(p) * BLOCK_UNIT_SIZE,
		"next" , next_block(p));
		if ((p = next_block(p)) == list)
			break;
	}
	putchar ('\n');
}

#endif /* CC_DEBUG_ALLOC */
