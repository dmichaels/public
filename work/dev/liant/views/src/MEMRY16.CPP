// $Header:   Q:/views/win/vcs/MEMRY16.CPv   1.10   Mar 04 1997 14:12:58   gregm  $ 

//	memry16.cpp
//
//	VMemory implementation (16 bit version)
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	09/09/92 jmd	made the memObj object CV_NEARDATA
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	09/10/92 jmd	added operator new(long) to fix Borland 3.1 problem
//	10/26/92 jmd	preened, made memInit static (for non-DLL)
//	03/03/93 pat	preened, consolidated #ifdef'd routines, commented
//	03/15/93 pat	memInit is now non-static, in non-DLL contexts we use
//					the 'Monte Carlo' method of initialization
//	03/19/93 pat	reordered #include's, conditionally included dos.h
//	04/06/93 pat	fixed parenthesis of MEMtotal macro under M64K
//	04/07/93 pat	changed id to VObject *
//	06/02/93 pkt	Interface is no longer dependent on #define USED.
//	07/31/93 pkt	eliminated #ifdef macintosh.
//	09/29/93 pkt	eliminated dead code supporting memory pool.
//	10/10/93 jmd	SC6 changes.
//	11/16/93 pkt	added declaration of tracemem.
//	12/15/93 jmd	made VMemory::memory fail if request too big
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	01/31/94 pat	declared left() and locmem() static
//	01/31/94 pat	added casts in calls to LocalAlloc() within locmem().
//  02/26/94 pat	moved Borland specific operator new() to end of source
//					for DLL build (see note at end of file).
//	07/14/94 jld	added defs.h to define CV_WINDOWS and CV_NEARDATA
//	08/30/94 evc 	caste from unsigned * to char * for asign to allocpter
//	11/04/94 dss	STRICT HANDLE conformance.
//  11/28/95 evc    Replaced the dual heap memory allocation scheme
//					with one that satisfies all requests with global
//					sharable memory.  The first use of new() will generate
//					A VMemory object (in the DLL's local heap) that will,in
//					turn initialize the master heap using GMEM_DDESHARE.
//					DLLInit has no bearing on the source of allocated memory.
//					Note also that only ONE VMemory object is needed for
//					the DLL and all its clients.
//  11/30/95 dgm    Un-inlined function containing loop to prevent Borland
//					warnings (they won't be expanded anyways).
//  12/14/95 evc    Cleanup that lets the static libraries use the
//					COmpiler vender's new/delete.  Only 16 bit DLLs
//					overload new/delete.
//  02/01/96 evc    Even more cleanup.
//	05/28/96 glm	removed cv_new_delete
// ----------------------------------------------------------------------------

extern "C" {
#	include <stdio.h>
}

#include "defs.h"
#include "memry16.h"
#include "winclass.h"
#include "intset.h"

extern "C" {
#	if defined(_MSC_VER)
#		include <malloc.h>
#	endif
#	ifdef MSDOS
#		if defined(__SC__)
#			undef MSDOS
#		endif

#		include <dos.h>

#		if defined(__SC__)
#			define MSDOS
#		endif
#	endif
}

#define MEMlink 4 			//	size in words of a prev/next link
							//		(where a word == sizeof(unsigned))
#ifdef M64K
			//	MEMtotal is 7 bytes shy of 64K
#	define MEMchunk 32754u // size in WORD's of chunk
#	define MEMtotal ((unsigned(MEMchunk << 1) + 20)
#else
			//	MEMtotal is 7 bytes shy of 32K
#	define MEMchunk 16370 // size in WORD's of chunk
#	define MEMtotal ((MEMchunk << 1) + 20)
#endif

#define MEMguard 0xffff 	//	guard value for memory block
#define MEMfree 0x8000		//	block-is-free flag

	//	some easy aliases for using MS Window's memory routines

#define rMalloc(size) \
((handle)GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE ,(DWORD)(size)))

#define rShareMalloc(size) 	\
((handle)GlobalAlloc(GMEM_MOVEABLE|GMEM_DDESHARE, (DWORD)(size)))

#define rNewsize(h,size) \
(handle)GlobalReAlloc(h, (DWORD)size, GMEM_MOVEABLE|GMEM_DDESHARE )

#	define rIshandle(h) 		\
						(GlobalSize(h) == 0L)
#	define rHandleOf(p) 		\
						(handle)(GlobalHandle(FP_SEG(p) & 0xffff) & 0xffff)
#	define rLockCount(h) 		\
						(int)(GlobalFlags(h) & GMEM_LOCKCOUNT)
#	define rFree(h) 			\
						GlobalFree(h)
#	define rUnlock(h) 			\
						(handle)((h)?GlobalUnlock(h):NIL)   
#	define rSize(h) 			\
						(unsigned int)GlobalSize(h)
#	define rLock(h) 			\
						(char *)GlobalLock(h) 


#ifdef CV_MEM_TRACE
#define CVP_USED
#endif

// ---------------------------------------------------------------------------
//
static void dummyUse(void *)
{
}

static void tracemem(int in, pointer p);
long used = 0;

defineClass(VMemory, VGlobal)

static long left();

#ifndef DLL
	//	In a non-DLL context, the global VMemory object is static.
	//	(In a DLL context, the global object is dynamic.  See operator->(), 
	//	below.)

	VMemory CV_NEARDATA memObj;

	//	for the 'Monte Carlo' initialization	

#	define VMEMORY_MAGICNUMBER	85307L
#endif

// 	'mem' is the global pointer to a memory object.
//
//	Though most of the methods of interest in VMemory are static, this
//	class always refers the global memory object indirectly (that is, with
//	'mem' and operator->() in order to create the global memory object the
//	first time its needed.

static VMemory *mem = 0;

// ---------------------------------------------------------------------------
//	Construct a VMemory object.
//
//	If in a DLL context under MS Windows, then initial the Allegris heap, 
//	too.
//
VMemory::VMemory() : real(-1)
#ifdef DLL
	, memInit(0)
#endif
{
	init();
}

#ifdef DLL
// ---------------------------------------------------------------------------
//	Construct a VMemory object for use with a DLL.
//
//	If in a DLL context under MS Windows, then initial the Allegris heap, 
//	too.
//
VMemory::VMemory(boolean dll) :
	VGlobal(GLOBALmemory),
	real(-1),
	memInit(0)
{
	init();
}

void* VMemory::operator new(size_t nBytes) {
	char * p = 0;
	return (void* ) GlobalAllocPtr(GMEM_MOVEABLE|GMEM_DDESHARE, nBytes);
}

void VMemory::operator delete( void* p ){
	GlobalFreePtr(p);
}
#endif

// ---------------------------------------------------------------------------
//	Destroy the memory object.
//
VMemory::~VMemory()
{
#ifdef DLL
//	wCls = -1;
#else
	mem = 0;
#endif
}

// ---------------------------------------------------------------------------
//	Arrange to call method 'm' on object 'o' to get more memory.
//	If no more memory is made available
//	a fatal message is printed.
//
void VMemory::more(VObject *o, method m)
{
	(*mem)->moreId = o;
	(*mem)->moreMethod = m;
}


// ---------------------------------------------------------------------------
//	This routine is called when a memory allocation failure occurs.
//	It may invoke an application-supplied method to get more memory,
//	or exit with an error message.
//
//	An application may use VMemory::more() to provide a callback 
//	for getting	more memory when memory is exhausted.  When a memory
//	allocation fails, the callback is invoked here.  If more memory is
//	made available under the callback, the callback returns	TRUE and 
//	the allocation is re-tried.  If memory is exhausted, the callback
//	returns FALSE and the application exits with an "out of memory"
//	message.
//
//	If the application does not use either VMemory::more().

boolean VMemory::ismore()
{
#if !defined(_MSC_VER)
	//	not applicable under Microsoft C

	boolean b = FALSE;
	if (moreId && moreMethod != NIL_METHOD) {
		b = moreId->perform(moreMethod);
	}
	if (b == 0) {
		print("out of memory\n");
		exit(1);
	}

	return(b);
#else
	return(FALSE);
#endif
}

// ---------------------------------------------------------------------------
//	Lookup an instance of the VMemory class and return its reference. 
//	If the instance does not exist, it is created.
//
//	Note:	Though not technically proper, this operator overloads 
//			relies on the fact that the compilers with which we work 
//			let use this operator with a null pointer to a class 
//			instance ("mem" is initially 0). 
//

VMemory *VMemory::operator->()
{
	if (mem == 0) {
#ifdef DLL
		//	This application does not have an associated memory object, 
		//	create it now.

		// Notice this is not placed in globals table because
		// 1 VMemory object is sufficient for the DLL and all
		// its clients
		mem = new VMemory();
#else
		//	In a non-DLL context, the memory object is a static global 
		//	object.  Upon first reference, set the global "mem" pointer 
		//  to point to the	memory object.

		mem = &memObj;
#endif
	}
		return mem;
}

// ===========================================================================
#ifdef CV_WINDOWS
//
//	CV_WINDOWS defines these static functions for allocating memory 
//	under MS Windows:
//
//		gmemory
//		memory
//
//

struct link {
	struct link *next;		//	these members must remain in this order
	struct link *prev;		//	or code will break!
};

unsigned *allocp;	//	pointer to chunk list

// ---------------------------------------------------------------------------
//	Copy 'n' words from 'f' to 't'.

void umemcpy(void *f, void *t, unsigned n)
{
	register unsigned *fm = (unsigned *)f, *to = (unsigned *)t;
	for (; n > 0; n--) {
		*to++ = *fm++;
	}
}


// ---------------------------------------------------------------------------
//	Set 'n' bytes of memory, starting at address 'p', to zero.

void umemclr(void *p, unsigned n)
{
	register unsigned *lp = (unsigned *)p;
	for (; n > 0; n--) {
		*lp++ = 0;
	}
}


// ---------------------------------------------------------------------------
//	Return non-zero if link pointed to by 'p' is only a link; zero, otherwise.

inline int only(unsigned *p)
{
	register struct link *k = (struct link *)p;
	return(k->next == k->prev);
	
}

// ---------------------------------------------------------------------------
//	Set the prev/next pointers of the link that is pointed to by 'p' 
//	to point to the link itself.

inline void nil(unsigned *p)
{
	register struct link *k = (struct link *) p;
	k->next = k->prev = k;
}

// ---------------------------------------------------------------------------
//	Add link 'lnk' after link 'lst'.

inline void add(unsigned *lst, unsigned *lnk)
{
	register struct link *l = (struct link *)lst;
	register struct link *d = (struct link *)lnk;
	l->next->prev = d;
	d->next = l->next;
	d->prev = l;
	l->next = d;
}

// ---------------------------------------------------------------------------
//	Return non-zero if the block to which p points is free; 0, if not.
//
inline int isfree(unsigned *p)
{
	return (*p == MEMguard) ? 0 : (*p & MEMfree);
}

// ---------------------------------------------------------------------------
//	Return the number of words in block 'p'.

inline unsigned size(unsigned *p)
{
	if (*p == MEMguard) {
		return rSize(HGLOBAL(*(p - 1))) / 2;
	}
	return *p & 0x7fff;
}

// ---------------------------------------------------------------------------
//	Return the next link after 'lnk'.

inline unsigned *next(unsigned *lnk)
{
	register struct link *d = (struct link *)lnk;
	return(unsigned *)d->next;
}

// ---------------------------------------------------------------------------
//	Remove 'lnk' from the list it is in.

inline void remove(unsigned *lnk)
{
	register struct link *d = (struct link *)lnk;
	d->next->prev = d->prev;
	d->prev->next = d->next;
//	d->next = d->next = d;
}

// ---------------------------------------------------------------------------
//	Allocate and return a chunk of memory that contains a single free block.
//	Chunks are taken from the the global heap.  The memory chunk is moveable, 
//	but not shareable (i.e., not suitable for use by a DLL that requires the
//	memory for persistent use by multiple clients)
//
//	The chunk is set up to look like this:
//
//		start of chunk:		prev/next link
//		start of freelist:	prev/next link
//							MEMguard
//							MEMfree | size of chunk in words
//		single free block:	.
//							.	(single free memory block of MEMchunk words)
//							.
//							MEMfree | size
//		last word:			MEMguard

static unsigned *chunk()
{
	unsigned *freeList;
	unsigned *chunk = (unsigned *) VMemory::gmemory(0, 
											long((unsigned long) MEMtotal));

	unsigned *memBlock = chunk;

#ifdef CVP_USED
	used -= MEMtotal;
#endif

	//	set up the start of the chunk as a link whose next and prev members
	//	point to the first 4 bytes (sizeof(unsigned)) of the the chunk
	nil(memBlock);

	//	make a similar link after that for the free list (next and previous
	//	points to freeList) 
	memBlock += MEMlink;	
	freeList = memBlock;
	nil(freeList); 

	//	follow with a guard value
	memBlock += MEMlink;
	*memBlock++ = MEMguard;

	//	then place the bitwise OR of the free flag and the chunk's word size
	*memBlock++ = MEMfree | MEMchunk;

	add(freeList, memBlock);

	memBlock += MEMchunk - 2;
	*memBlock++ = MEMfree | MEMchunk;
	*memBlock++ = MEMguard;

	if (((char *) memBlock) - ((char *) chunk) > MEMtotal) {
		print("memory block too large!\n");
	}
//	print("chunk (%ld) %p\n",MEMtotal, chunk);

	return(chunk);
}

// ---------------------------------------------------------------------------
//	Return the chunk that 'q' points into

unsigned *inchunk(unsigned *q)
{
	unsigned *p;
	long t;
	for (p = next(allocp); p != allocp; p = next(p)) {
		t = ((unsigned long) q) - ((unsigned long) p);
		if (t > 0 && t < MEMtotal) {
			return(p);	
		}
	}
	return(0);
}

// ---------------------------------------------------------------------------
inline void allocit(unsigned *r, unsigned s)
{
	*r = s;
	*(r + s - 1) = s;
}

// ---------------------------------------------------------------------------
//	Make the block pointed to by 'p' a free block of 'n' words.

inline void freeit(unsigned *p, unsigned n)
{
	*p = MEMfree | n;
	*(p + n -1) = MEMfree | n;
}

// ---------------------------------------------------------------------------
//	Allocate and return a block of 'nWords' words. 

static void *walloc(unsigned nWords)
{
	unsigned *p, *q, *r;
	void *ret = 0;

	nWords += 2;
	if (nWords > MEMchunk) {
		return(VMemory::gmemory(0, nWords << 1));
	}

#ifdef CVP_USED
	used += nWords << 1;
#endif

	while (1) {
		for (p = next(allocp); p != allocp; p = next(p)) {
			q = p + MEMlink;

			for (r = next(q); r-- != q; r = next(++ r)) {
				unsigned s = size(r);
				unsigned res = s - nWords;

				if (s >= nWords) { 
					// got one
					if (res < 6) { 
						// alloc whole thing
						allocit(r, s);	
						remove(r +1);
						ret = r + 1;
					}
					else { 
						// alloc tail
						freeit(r, res);	
						r += res;
						allocit(r, nWords);	
						ret = r + 1;
					}

//					print("alloc %p %d\n",ret,nWords);
					return(ret);
				} 
			}
		}
	
		//	allocate a chunk that contains a single free memory block
		//	and !!!
		if (!(p = chunk())) {
//			print("alloc (nil)\n");
			return(ret);
		}

		add(allocp, p);
	}
}

// ---------------------------------------------------------------------------
//	Return 'v' to the freelist.

static void *wunalloc(void *v)
{
	int b = 0;
	unsigned n, *q, *r, *p = (unsigned *)v;	

//	print("unalloc %p (%d) ",p,size(p-1));

	if (*-- p == MEMguard) { 
		// VGlobal VMemory handle
		return(void *)VMemory::gmemory((pointer) v, 0);
	}

#ifdef CVP_USED
	used += size(p)<< 1;
#endif

	if (isfree(p -1)) { 
		// merge with lower
//		print("lower ");
		n = size(p--); 	  // size of current
		p -= size(p) - 1;
		n += size(p);
		freeit(p, n);
		b = 1;
	}

	q = p + size(p) - 1;
	if (isfree(q +1)) { // merge with upper
		//print("upper ");
		n = size(++ q); // size of upper
		r = next(++ q);
		remove(q); 
		n += size(p);
		freeit(p, n);
		if (b == 0) {
			add(r, p +1);
		}
		b |= 2;
	}
	if (b == 0) { // add to free list;
		r = inchunk(p);
		//	print("free (%p)",r);
		r += MEMlink;
		freeit(p, size(p));
		add(r, p +1);
	}

	//print("\n");
	if (only(p +1) && size(p) == MEMchunk) {
		//	unsigned *r = inchunk(p);
		//	print("!chunk %p\n",p);
		p -= MEMlink + MEMlink +1;
		if (p == allocp) {
			allocp = next(allocp);	
		}
		if (allocp == p) {
			allocp = 0;
		}
		remove(p);
		VMemory::gmemory((pointer)p, 0);
#ifdef CVP_USED
		used += MEMtotal;
#endif
	}
	return(0);
}

// ---------------------------------------------------------------------------
//	Realloc 'p' to be 'n' words and return it. 

static void *wrealloc(void *p, unsigned n)
{
	unsigned *q = (unsigned *)p;
	unsigned i = size(q -1) - 2; // includes boundary tags

	int m = i - n; 
	if (m <= 0 || m > 2) {
		q = (unsigned *)walloc(n);
		umemcpy(p, q, n > i ? i: n);
		wunalloc(p);
		p = q;
	}	
	return(p);
}



// ---------------------------------------------------------------------------
//	A general-purpose memory allocation facility. 
//
//	Allocation:
//		If 'p' is NIL and 'nBytes' is non-zero, then 'nBytes' bytes are 
//		allocated and returned.  
//
//	Deallocation:
//		If 'nBytes' is zero, then the memory associated with 'p' 
//		(if non-NIL) is returned to the heap.  
//
//	Reallocation:
//		If 'p' is non-NIL and 'nBytes' is non-zero, then 'p' is 
//		reallocated to be 'nBytes' bytes.
//	

pointer VMemory::memory(pointer p, long nBytes)
{

	//	initial the memory object if uninitialized

#ifdef DLL
	if ((*mem)->memInit == 0) {
		(*mem)->init();
	}
#else
	if ((*mem)->memInit != VMEMORY_MAGICNUMBER) {
		(*mem)->init();
	}
#endif

	allocp = (unsigned *)(*mem)->allocptr;

#ifdef CV_MEM_TRACE
	tracemem(1, p);
#endif

	// jmd 12/15/93
	if (nBytes >= (long) 0xFFFF) {
		// too big, fail
		nBytes = 0;
	}

	unsigned nWords = (unsigned) nBytes;
	if (nWords) {
		//	round odd byte requests up to the nearest word (even byte)
		nWords = (nWords + 1) >> 1;

		//	the minimum allocation request is 4 words
		nWords = (nWords < 4) ? 4 : nWords;

		if (p) {
			//	reallocation
			p = (pointer) wrealloc((void *)p, nWords);
		}
		else {
			//	allocation
			p = (pointer) walloc(nWords);
		}
	}
	else if (p) {
		//	deallocation
		p = (pointer) wunalloc((void *)p);
	}

#ifdef CV_MEM_TRACE
	tracemem(0, p);
#endif

	return(p);
}

// ---------------------------------------------------------------------------
//	A general-purpose memory allocation facility.  This is just like 
//	VMemory::memory, only the MS Windows global heap is used directly.
//
//	allocation:
//		If 'p' is NIL and 'nBytes' is non-zero, then allocate a block of
//		'nBytes' and return a pointer to the block.
//
//	deallocation:
//		If 'nBytes' is zero, then the memory associated with 'p' 
//		(if non-NIL) is returned to the heap.  
//
//	reallocation:
//		If 'p' is non-NIL and 'nBytes' is non-zero, then the block 
//		to which 'p' points is reallocated to be 'nBytes' bytes and a
//		pointer to the reallocated block is returned.
//

pointer VMemory::gmemory(pointer p, long nBytes)
{
	HANDLE h, *hp;

	if ((*mem)->real == -1) {
		//	is MS Windows running in real mode?

		DWORD d = GetWinFlags();
		(*mem)->real = (d & (WF_STANDARD | WF_ENHANCED)) ? 0 : 1;
	}

	do {
		if (nBytes) {
			if (p) {
				//	reallocate the block from the global heap

				hp = (HANDLE *)p;
				hp--; // past guard
				h = *-- hp;
#ifdef CVP_USED
				used -= rSize(h);
#endif
				if (h = rNewsize(h, nBytes + 4)) {
					//	lock the block
					//	if MS Windows is in real mode, then	move the block
					//	to a low memory position to avoid fragmentation

					if (hp = (HANDLE *)((*mem)->real ? GlobalWire(h) : GlobalLock(h))) {
						*hp++ = h;
						*hp++ = HGLOBAL(MEMguard);
#ifdef CVP_USED
						used += rSize(h);
#endif
					}
					p = (pointer)hp;
				}
				else {
					p = 0;
				}
				
			}
			else {	
				//	allocate a moveable block out of the global heap

				if (h = rMalloc(nBytes + 4)) {
					//	lock the block
					//	if MS Windows is in real mode, then	move the block
					//	to a low memory position to avoid fragmentation

					if (hp = (HANDLE *)((*mem)->real ? GlobalWire(h) : GlobalLock(h))) {
						*hp++ = h;
						*hp++ = HGLOBAL(MEMguard);
#ifdef CVP_USED
						used += rSize(h);
#endif
					}
					p = (pointer)hp;
				} 
			}
		}
		else if (p) {
			//	return the block to the global heap

			hp = (HANDLE *)p;
			hp--;
			h = *-- hp;
#ifdef CVP_USED
			used -= rSize(h);
#endif
			if ((*mem)->real) {
				GlobalUnWire(h);
			}
			else {
				GlobalUnlock(h);
			}
			rFree(h);
			p = 0;
		}
	} while (nBytes && (p == 0) && (*mem)->ismore());

	return(p);
}


#endif	// CV_WINDOWS
// ===========================================================================

// ===========================================================================
#if defined(CV_WINDOWS) 
//
//	CV_WINDOWS and Macintosh define these static functions for
//	dealing with MS Windows and Macintosh memory handles:
//
//		memhndl
//		sizehndl
//		lockhndl
//		unlockhndl
//
//

// ---------------------------------------------------------------------------
//	As with VMemory::memory except VGlobal memory handles are used. 
//	If 'share' is non-zero, then allocate the handle from shared memory.
//
handle VMemory::memhndl(handle h, long sz, int share)
{
#ifdef CV_MEM_TRACE
	tracemem(2, (pointer)((long)h));
#endif
	do {
		if (sz) {
			if (h) {
#ifdef CVP_USED
				used -= rSize(h);
				used += sz;
#endif
				h = (handle)rNewsize(h, (size_t) sz);
			}
			else {
#ifdef CVP_USED
				used += sz;
#endif
				if (share) {
					h =  (handle)rShareMalloc((size_t)sz);
				}
				else {
					h =  (handle)rMalloc((size_t)sz);
				}
			}
		}
		else if (h) {
#ifdef CVP_USED
			used -= rSize(h);
#endif
			rFree(h), h = 0;
		}
	} while (sz && (h == 0) && (*mem)->ismore());
#ifdef CV_MEM_TRACE
	tracemem(3, (pointer)((long)h));
#endif
	return h;
}

// ---------------------------------------------------------------------------
//
long left() {
	return GlobalCompact(0L);
}

// ---------------------------------------------------------------------------
//	Return the size of the memory associated with handle 'h'.
//
unsigned VMemory::sizehndl(handle h)
{
	return rSize(h);
}

// ---------------------------------------------------------------------------
//	Lock 'h' and return a pointer to its memory.
//
pointer VMemory::lockhndl(handle h)
{
#ifdef CV_MEM_TRACE
	tracemem(4, (pointer)((long)h));
#endif
	return rLock(h);
}

// ---------------------------------------------------------------------------
//	Unlock handle 'h'.
//
void VMemory::unlockhndl(handle h)
{
#ifdef CV_MEM_TRACE
	tracemem(5, (pointer)((long)h));
#endif
	rUnlock(h);
}
#endif // CV_WINDOWS 
// ===========================================================================

// ---------------------------------------------------------------------------
//
void VMemory::init()
{
#ifndef DLL
	if (memInit == VMEMORY_MAGICNUMBER) {
		return;
	}
#else
	if (memInit != 0) {
		return;
	}
#endif

	//	make member variable allocb a link that points to itself (a nil link)
	allocptr = (unsigned char *) allocb;
	::nil((unsigned int *) allocptr);

	//	no out-of-memory callback
	moreId 		= 0;			
	moreMethod 	= NIL_METHOD;

#ifndef DLL
	memInit = VMEMORY_MAGICNUMBER;
#else
	memInit++;
#endif
}

// ===========================================================================
//
//	defining CV_MEM_TRACE enables the following static memory checking functions:
//		checkpoint
//		traceon
//		traceoff
//		chkheap
//

#ifdef CV_MEM_TRACE // avoid warning "::b defined but not used"
	static FILE *f = 0;
	static int memtrace = 0;
#	ifdef CV_FULL_TRACE
		static char b[] = "-+<>()";
#	endif
	static VIntegerSet *is = 0;
#endif

// ---------------------------------------------------------------------------
//
static void tracemem(int in, pointer p)
{
#ifdef CV_MEM_TRACE

	// Must be aware that "is"'s allocation will call tracemem.
	// (Fixes a dangerous allocation. - pkt)
	if (is == 0) {
		static short skip_trace_cnt = 0;
		if (skip_trace_cnt == 0) {
			VIntegerSet *temp;
			
			skip_trace_cnt++;
			temp = new VIntegerSet(10000);
			is = temp;
		}
		else {
			return;
		}
	}
	if (memtrace == 0) {
		return;
	}
	if (p == 0) {
		return;
	}
	if (f == 0) {
		return;
	}
	if (in) {
		if (is->remove((long)p) < 0) {
			fprintf(f, "freeing already free memory (%p)\n", p); 
		}
	}
	else {
		if (!is->add((long)p)) {
			fprintf(f, "allocating already allocated memory (%p)\n", p); 
		}
	}
#ifdef CV_FULL_TRACE
	if (p && memtrace && in < 4) {
		fprintf(f, "%c%p%s", b[in], p, LINE_TERM), fflush(f);
	}
#endif

#else // CV_MEM_TRACE

	dummyUse(&in);
	dummyUse(&p);

#endif // CV_MEM_TRACE

}

#ifndef macintosh
// ---------------------------------------------------------------------------
//	Perform a heap integrity check. If 'trace' is non-zero,
//	then a verbose printout of any problems with the heap are reported.

void VMemory::chkheap(int trace)
{
#ifdef CV_MEM_TRACE
	unsigned n, *r, *p, *h;
	for (h = next(allocp); h != allocp; h = next(h)) {
		p = h;
		p += MEMlink *2;
		if (*p++ != MEMguard) {
			fprintf(f, "starting guard blown\n");
			exit(1);
		}

		do {
			n = size(p);
			if (*p != p[n -1]) {
				unsigned m = size(p + n -1);
				if (isfree(p)) {
					fprintf(f, "invalid free block at (%d) (%d) %p\n", n, m << 1, p);
				}
				else {
					fprintf(f, "invalid block at (%d) (%d) %p\n", n, m << 1, p);
				}
				if (trace == 0) {
					VMemory::chkheap(1);
				}
				exit(0);
			}
			if (trace && !isfree(p)) {
				fprintf(f, "alloc'ed block (%d) at %p\n", n << 1, p);
			}
			p += n;
			if (p > h + MEMtotal) {
				fprintf(f, "ending guard blown\n");
				if (trace == 0) {
					VMemory::chkheap(1);
				}
				exit(1);
				return;
			}
		} while (*p != MEMguard);

		p = h + MEMlink;
		for (r = next(p); r-- != p; r = next(++ r)) {
			if (!isfree(r) || !isfree(r + size(r)-1)) {
				fprintf(f, "invalid free block (%d) %p\n", size(r)<< 1, r);
				if (trace == 0) {
					VMemory::chkheap(1);
				}
				exit(1);
			}
			if (trace) {
				fprintf(f, "free block (%d) at %p\n", size(r)<< 1, r);
			}
		}
	}
#else // CV_MEM_TRACE

	dummyUse(&trace);

#endif // CV_MEM_TRACE

}
#endif

// ---------------------------------------------------------------------------
//	Print out 's' along with the number of bytes and the number
//	of allocated blocks is printed.
//
void VMemory::checkpoint(char *s)
{
#ifdef CV_MEM_TRACE
	fprintf(f, "------ %s (%ld bytes %ld blocks)\n", s, used, is->count());
#else
	dummyUse(&s);
#endif
}

// ---------------------------------------------------------------------------
//	Turn on memory tracing. A trace of memory allocation activity
//	is written to the file 'trace' in the current working directory.
//
void VMemory::traceon()
{
#ifdef CV_MEM_TRACE
	used = 0;
	memtrace = 1;
	if (f == 0) {
		f = fopen("trace", "w");
	}
	checkpoint("trace on");
#endif
}

// ---------------------------------------------------------------------------
//	Turn off memory tracing. 
//
void VMemory::traceoff()
{
#ifdef CV_MEM_TRACE
	memtrace = 0;
	checkpoint("trace off");
#endif
}
