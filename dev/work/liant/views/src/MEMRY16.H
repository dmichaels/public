// $Header:   Q:/views/win/vcs/MEMRY16.H_v   1.9   Mar 04 1997 14:13:00   gregm  $ 

//	memry16.h
//
//	VMemory interface (16 bit version)
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	09/10/92 jmd	made MSC ifdefs use >= instead of ==
//	10/26/92 jmd	preened, made memInit static (for non-DLL)
//	03/05/93 pat	preened
//	03/15/93 pat	changed memInit from int to long, made non-static
//	03/19/93 pat	removed redundant CV_WINDOWS #define
//	04/09/93 pat	changed id to VObject *
//	06/02/93 pkt	Removed #ifdef USED and #ifdef MEM_TRACE.  Interface will
//					be as if USED and MEM_TRACE were always TRUE.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	preened and merged with Macintosh.
//	07/27/94 dgm	changed allocptr from unsigned* to char*.
//  11/28/95 evc    Replaced the dual heap memory allocation scheme
//					with one that satisfies all requests with global
//					sharable memory.  The first use of new will generate
//					A VMemory object (Using a call to GlobalAlloc) that will,in
//					turn, init the master heap using GMEM_DDESHARE'ed memory
//					DLLInit has no bearing on the source of allocated memory.
//	12/14/95 evc	More cleanup.
//  02/01/96 evc    Even more cleanup.
// ----------------------------------------------------------------------------

#ifndef VMEMRY16_H
#define VMEMRY16_H

// ---------------------------------------------------------------------------
//
#if !defined(CV_WINDOWS) && defined(CV_WIN16)
#	define CV_WINDOWS
#endif

// ---------------------------------------------------------------------------
//
#include "global.h"

// ---------------------------------------------------------------------------
//


#define nilhandle ((handle)0)

#define nilpointer ((pointer)0)

extern long used;

CLASS VMemory;

#define GLOBALmemory 1			// 	class type for the global VMemory object

// ---------------------------------------------------------------------------
//	VMemory is a fully static class in that only one instance of it
//	is constructed per application.  
//  VMemory provides reliable subsegment memory allocation facilities
//  that are largely superceded by the compiler supplied runtime libraries.

//  It  is still used for C++/Views for 16 bit DLLs built with MSVC 1.52.  
//	Other versions of the libraries use the runtime library supplied with 
//  the compiler.
//
//	Under MS Windows, there are three static VMemory methods which perform
//	memory allocation, reallocation and deallocation as follows:
//	methods:
//
//		VMemory::gmemory	uses the global heap
//		VMemory::memory		uses the C++/View heap (at root, the global heap)
//
//	In addition, these static methods provide access to global memory handles:
//
//		VMemory::memhndl	uses the global heap, can allocated shared memory
//		VMemory::sizehndl		
//		VMemory::lockhndl
//		VMemory::unlockhndl
//
//	The memory facility allocates memory out the of global heap in large
//	chunks and provides heap allocation out of these chunks.
//

CLASS VMemory : public VGlobal {
public:
					VMemory();
#ifdef DLL
					VMemory(boolean);
					// this overload of new lets us create a VMemory
					// object which, in tern, creates the global
					// heap that new and delete use.  The new of a VMemory
					// object occurs the fisrt time anyone calls ::new().  
					// The overload pervents terminal recursion.
					void* operator new(size_t ) ;
					void operator delete( void* p );
#endif
				   ~VMemory();

	VClass		   *iam();

	// Memory Allocation
	static pointer	gmemory(pointer p, long nBytes);
	static pointer	memory(pointer p, long nBytes);

	static handle	memhndl(handle h, long nBytes, int share = 0);
	static unsigned	sizehndl(handle h);
	static pointer	lockhndl(handle h);
	static void		unlockhndl(handle h);

	//	out-of-memory handlers
	static void		more(VObject *o, method m);

	//	'smart-pointer' to lookup global memory object associated with app
	VMemory 	   *operator->(); 

	//	memory checking methods
	//	active only if memry.cpp compiled with MEM_TRACE set
	static void 	checkpoint(char *s);
	static void 	traceon();
	static void 	traceoff();
	static void 	chkheap(int);

protected:
	int				real;
	VObject 	   *moreId;
	method			moreMethod;
	long  			memInit;

	void	   	   *allocb[2];	
	unsigned char  *allocptr;	   	

	boolean 		ismore();
	void			init();
};

#endif // VMEMRY16_H

