// $Header:   Q:/views/win/vcs/MEMRY32.CPv   1.9   Mar 04 1997 14:13:02   gregm  $ 

//  memry32.cpp
//
//  VMemory implementation (32 bit version)
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
//  Revision History:
//  -----------------
//  09/09/92 jmd    made the memObj object CV_NEARDATA
//  09/09/92 jmd    made CV_WIN16 auto define CV_WINDOWS
//  09/10/92 jmd    added operator new(long) to fix Borland 3.1 problem
//  10/26/92 jmd    preened, made memInit static (for non-DLL)
//  03/03/93 pmcm   preened, consolidated #ifdef'd routines, commented
//  03/15/93 pmcm   memInit is now non-static, in non-DLL contexts we use
//                  the 'Monte Carlo' method of initialization
//  03/19/93 pmcm   reordered #include's, conditionally included dos.h
//  04/06/93 pmcm   fixed parenthesis of MEMtotal macro under M64K
//  04/07/93 pmcm   changed id to VObject *
//  06/02/93 pkt    Interface is no longer dependent on #define USED.
//  07/31/93 pkt    eliminated #ifdef macintosh.
//  09/29/93 pkt    eliminated dead code supporting memory pool.
//  10/10/93 jmd    SC6 changes.
//  11/16/93 pkt    added declaration of tracemem.
//  12/15/93 jmd    made VMemory::memory fail if request too big
//  01/13/94 dgm    merged and preened
//  01/18/94 pkt    preened and merged with Macintosh.
//  01/31/94 pmcm   declared left() and locmem() static
//  01/31/94 pmcm   added casts in calls to LocalAlloc() within locmem().
//  02/26/94 pmcm   moved Borland specific operator new() to end of source
//                  for DLL build (see note at end of file).
//  03/04/94 pmcm   removed unused left()
//  03/05/94 pmcm   removed CPPV_HUGE ifdef
//  03/07/94 pmcm   removed real mode support
//  03/10/94 pmcm   removed sub-allocation, memory tracing, macintosh
//  03/24/94 pmcm   auto defined CV_WINDOWS under CV_WIN32
//  06/06/94 jld    remove overload on new and delete under CV_WIN32
//  07/13/94 jld    added dependency on defs.h removed redundancies
//  07/19/95 evc    Got rid of code used only in win16.
//  07/29/96 dgm    Changed order of #includes to work-around Borland problem.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "memry32.h"
#include "winclass.h"
#include "intset.h"

extern "C" {
#   include <stdio.h>
#   include <stdlib.h>
#   include <string.h>
}

extern "C" {
#   if defined(_MSC_VER)
#       include <malloc.h>
#   endif
#   ifdef MSDOS
#       if defined(__SC__)
#           undef MSDOS
#       endif

#       include <dos.h>

#       if defined(__SC__)
#           define MSDOS
#       endif
#   endif
}

#ifdef CV_WINDOWS
#   include "windowsx.h"
#endif  //  CV_WINDOWS

// ---------------------------------------------------------------------------
defineClass(VMemory, VGlobal)

// ---------------------------------------------------------------------------

#ifdef DLL
//  extern int dllInit;
    static pointer locmem(pointer, long);
#else   
    //  In a non-DLL context, the global VMemory object is static.
    //  (In a DLL context, the global object is dynamic.  See operator->(), 
    //  below.)

    VMemory CV_NEARDATA memObj;

    //  for the 'Monte Carlo' initialization    

#   define VMEMORY_MAGICNUMBER  85307L
#endif

//  'mem' is the global pointer to a memory object.
//
//  Though most of the methods of interest in VMemory are static, this
//  class always refers the global memory object indirectly (that is, with
//  'mem' and operator->() in order to lookup the global memory object
//  associated with the application when in a DLL context (see 
//  operator->(), below and the VGlobal class).

VMemory *mem = 0;

// ---------------------------------------------------------------------------
//  Construct a VMemory object.
//
//  If in a DLL context under MS Windows, then initial the C++/Views heap, 
//  too.
//
VMemory::VMemory() 
#ifdef DLL
      : memInit(0)
#endif
{
    init();
}

#ifdef DLL
// ---------------------------------------------------------------------------
//  Construct a VMemory object for use with a DLL.
//
//  If in a DLL context under MS Windows, then initial the C++/Views heap, 
//  too.
//
VMemory::VMemory(boolean dll) 
    :   VGlobal(GLOBALmemory),
        memInit(0)
{
    init();
}
#endif

// ---------------------------------------------------------------------------
//  Destroy the memory object.
//
VMemory::~VMemory()
{
#ifdef DLL
//  wCls = -1;
#else
    mem = 0;
#endif
}

// ---------------------------------------------------------------------------
//  Arrange to call method 'm' on object 'o' to get more memory.
//  If no more memory is made available, then a longjmp is made
//  if it is set (non MS-Windows) or a fatal message is printed.
//
void VMemory::more(VObject *o, method m)
{
    (*mem)->moreId = o;
    (*mem)->moreMethod = m;
}

// ---------------------------------------------------------------------------
//  Establish a C standard library long jump and return 0. 'where'
//  is returned at some later time when memory is exhausted. 
//  If `fault()` is not called, then an out-of-memory message is printed
//  and the application is terminated (non MS-Windows only).
//

int VMemory::fault(int where)
{
#if defined(GNU)
    extern int setjmp(jmp_buf);
#endif

#if 0   //!!!jmd
    //  not applicable under Microsoft C/C++

    (*mem)->jump = where;
    return(setjmp((*mem)->env));
#else
    return(0);
#endif

}

// ---------------------------------------------------------------------------
//  This routine is called when a memory allocation failure occurs.
//  It may invoke an application-supplied method to get more memory,
//  perform a longjump(), or exit with an error message.
//
//  An application may use VMemory::more() to provide a callback 
//  for getting more memory when memory is exhausted.  When a memory
//  allocation fails, the callback is invoked here.  If more memory is
//  made available under the callback, the callback returns TRUE and 
//  the allocation is re-tried.  If memory is exhausted, the callback
//  returns FALSE and the application exits with an "out of memory"
//  message.
//
//  Alternately, an application may use VMemory::fault() to !!!pmcm!!!
//  When memory is exhausted, a longjump() is performed.
//
//  If the application does not use either VMemory::more() or 
//  VMemory::fault(), the application exits with an error message.
//

boolean VMemory::ismore()
{
#if !defined(_MSC_VER)
    //  not applicable under Microsoft C++

    boolean b = FALSE;
    if (moreId && moreMethod != NIL_METHOD) {
        b = moreId->perform(moreMethod);
    }
    else if (jump) {
//!!!jmd        longjmp(env, jump);
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
//  Allocate, reallocate, or deallocate from the local heap.
//
//  Allocation:     p == 0 and nBytes > 0
//  Deallocation:   p != 0 and nBytes == 0
//  Reallocation:   p != 0 and nBytes > 0
//
//  Note:   When MS Windows is not in use, then VMemory::lmemory() 
//          becomes VMemory::memory() per the #define in memry.h.
//
pointer VMemory::lmemory(pointer p, long nBytes)
{
    do {
        if (nBytes) {
            if (p) {
                p  = (pointer) realloc(p, (size_t) nBytes);
            }
            else {
                p =  (pointer) malloc((size_t) nBytes);
            }
        }
        else if (p) {
            ::free(p);
            p = 0;
        }

    //  handle allocation failures by 
    } while (nBytes && (p == 0) && (*mem)->ismore());

    return(p);
}

// ---------------------------------------------------------------------------
//  Lookup an instance of the VMemory class and return its reference. 
//  If the instance does not exist, it is created.
//
//  Note:   Though not technically proper, this operator overloads 
//          relies on the fact that the compilers with which we work 
//          let use this operator with a null pointer to a class 
//          instance ("mem" is initially 0). 
//

VMemory *VMemory::operator->()
{
    if (mem == 0) {
#ifdef DLL
        //  This application does not have an associated memory object, 
        //  create it now.  The 'dllInit' flag ensures that it lives in the
        //  DLL local heap.

//      dllInit = 1;
        mem = new VMemory(1);
//      dllInit = 0;
#else
        //  In a non-DLL context, the memory object is a static global 
        //  object.  Upon first reference, set the global "mem" pointer 
        //  to point to the memory object.

        mem = &memObj;
#endif
        return mem;
    }

#ifdef DLL
    //  lookup the memory object associated with the current client

    VMemory *m = (VMemory *) lookup(GLOBALmemory);
    if (m == 0) {
        //  not found, create it out of the the DLL heap

//      dllInit = 1;
        m = new VMemory(1);
//      dllInit = 0;
    }
    return m;
#else
    return mem;
#endif
}

// ===========================================================================
#ifdef CV_WINDOWS
//
//  CV_WINDOWS and define these static functions for
//  dealing with MS Windows and memory handles:
//
//      memhndl
//      sizehndl
//      lockhndl
//      unlockhndl
//
//

// ---------------------------------------------------------------------------
//  As with VMemory::memory except VGlobal memory handles are used. 
//  If 'share' is non-zero, then allocate the handle from shared memory.
//
handle VMemory::memhndl(handle h, long sz, int share)
{
    do {
        if (sz) {
            if (h) {
                h = GlobalReAlloc(h, (DWORD) sz, GMEM_MOVEABLE);
            }
            else {
                UINT how = GMEM_MOVEABLE;
                if (share) {
                    how |= GMEM_DDESHARE;
                }
                h = GlobalAlloc(GMEM_MOVEABLE, (DWORD) sz);
            }
        }
        else if (h) {
            GlobalFree(h);
            h = 0;
        }
    } while (sz && (h == 0) && (*mem)->ismore());

    return h;
}

// ---------------------------------------------------------------------------
//  Return the size of the memory associated with handle 'h'.
//
unsigned VMemory::sizehndl(handle h)
{
    return GlobalSize(h);
}

// ---------------------------------------------------------------------------
//  Lock 'h' and return a pointer to its memory.
//
pointer VMemory::lockhndl(handle h)
{
    return (pointer) GlobalLock(h);
}

// ---------------------------------------------------------------------------
//  Unlock handle 'h'.
//
void VMemory::unlockhndl(handle h)
{
    GlobalUnlock(h);
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

#ifdef CV_WINDOWS
    //  allocb is the blockLink at the head of a nil list of blocks
//  ::nilLink((unsigned char *) allocb);
    allocb[0] = allocb[1] = allocb;

    //  allocptr points the this block head!
    allocptr = (unsigned char *) allocb;
#endif

    //  no out-of-memory longjump
    jump        = 0;

    //  no out-of-memory callback
    moreId      = 0;            
    moreMethod  = NIL_METHOD;

#ifndef DLL
    memInit = VMEMORY_MAGICNUMBER;
#else
    memInit++;
#endif
}
