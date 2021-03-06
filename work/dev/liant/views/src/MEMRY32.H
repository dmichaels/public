// $Header:   Q:/views/win/vcs/MEMRY32.H_v   1.6   Mar 04 1997 14:13:04   gregm  $

//	memry32.h
//
//	VMemory interface (32 bit version)
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
//	03/05/94 pat	removed CPPV_HUGE ifdef
//	03/07/94 pat	removed real mode flag
//	03/10/94 pat	removed memory tracing methods, macintosh
//	03/23/94 pat	removed jmp_buf and env under _MSC_VER 
//	03/24/94 pat	auto defined CV_WINDOWS under CV_WIN32
//	07/13/94 jld	removed jmp_buf
//	07/14/94 jld	added defs.h dependency for CV_WINDOWS
// ----------------------------------------------------------------------------

#ifndef VMEMRY32_H
#define VMEMRY32_H

#include "defs.h"
#include "global.h"

extern "C" {
#	include <stdlib.h>
}

// ---------------------------------------------------------------------------
//
#ifndef CV_WINDOWS
	//	when not under MS Windows
	//	rename the lmemory() facility and all references to it

#	define lmemory memory

#	define nilhandle ((handle)0)
#endif

#define nilpointer ((pointer)0)

extern long used;

extern VClass *VMemoryCls;		//	pointer to our meta-class instance

CLASS VMemory;
extern VMemory *mem;			//	pointer to global memory object

#define GLOBALmemory 1			// 	class type for the global VMemory object

// ---------------------------------------------------------------------------
//	VMemory is a fully static class in that only one instance of it
//	is constructed per application.  It handles all memory allocation
//	for C++/Views.  
//
//	Under MS Windows, there are three static VMemory methods which perform
//	memory allocation, reallocation and deallocation as follows:
//	methods:
//
//		VMemory::gmemory	uses the global heap
//		VMemory::lmemory 	uses the local heap
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
//	When MS Windows is not in use, then the VMemory::lmemory() facility
//	is renamed to VMemory::memory().  See the #define above.

CLASS VMemory : public VGlobal {
public:
					VMemory();
#ifdef DLL
					VMemory(boolean);
#endif
				   ~VMemory();

	VClass		   *iam();

	// Memory Allocation
	static pointer	lmemory(pointer p, long nBytes);
#ifdef CV_WINDOWS
	static pointer	gmemory(pointer p, long nBytes);
	static pointer	memory(pointer p, long nBytes);

	static handle	memhndl(handle h, long nBytes, int share = 0);
	static unsigned	sizehndl(handle h);
	static pointer	lockhndl(handle h);
	static void		unlockhndl(handle h);
#endif

	//	out-of-memory handlers
	static void		more(VObject *o, method m);
	static int		fault(int where);

	//	'smart-pointer' to lookup global memory object associated with app
	VMemory 	   *operator->(); 

protected:
	int				jump;
	VObject 	   *moreId;
	method			moreMethod;
	long  			memInit;

#ifdef CV_WINDOWS
	void	   	   *allocb[2];	
	unsigned char  *allocptr;	   	
#endif

	boolean 		ismore();
	void			init();
};

#endif // VMEMRY32_H

