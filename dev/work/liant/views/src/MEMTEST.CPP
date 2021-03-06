// $Header:   Q:/views/win/vcs/memtest.cpv   1.4   Mar 04 1997 17:55:22   gregm  $ 

//	memtest.cpp
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
//	---------------------------------------------------------------------------

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

///!!!jmd  fix cvAlloc return data

#include "memtest.h"

#ifdef CV_MEMTEST

#undef new
#undef delete

// global data (line number of call to delete)
int		cvmem_lineno = 0;
char   *cvmem_filename = "";

#if !defined(CV_WINDOWS) && ( defined(CV_WIN16) || defined(CV_WIN32) )
#	define CV_WINDOWS
#endif

#ifdef CV_WIN16
#	include "memry.h"
//#	include "memtstbx.h"

#	define cvMalloc(sz)		VMemory::memory(0, (sz))
#	define cvFree(ptr)		VMemory::memory((pointer)(ptr), 0)
//#	define cvRefresh()		memTestBox->refresh()
#   define cvRefresh()

	extern void print(char *fmt...);		//!!!

#	define ALARM			print
#	define PRINT			print

#elif defined(CV_WIN32)

#	include "memry.h"

//#	define cvMalloc(sz)		VMemory::memory(0, (sz))
//#	define cvFree(ptr)		VMemory::memory((pointer)(ptr), 0)
#	define cvMalloc(sz)		malloc(sz)
#	define cvFree(ptr)		free(ptr)

#   define cvRefresh()
#	define default_color	FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#	define alarm_color		FOREGROUND_RED 

	extern void print(char *fmt...);		//!!!
	static HANDLE console_out;				// console window for Win32
	static HANDLE console_in;				// console window for Win32
	static int console_init = 0;

#	define ALARM			console_print
#	define PRINT			console_print

#else

#	define cvMalloc(sz)		malloc(sz)
#	define cvFree(ptr)		free(ptr)
#	define ALARM			printf
#	define PRINT			printf

#	define cvRefresh()

#endif

// -------------------------------------------------------------------------
#define ALIST_SIZE		2000
#define CVFILENAMELEN	14

struct alist_struct {
	void 			    *ptr;		// allocated block
	struct alist_struct *next;		// next entry in list
};

struct hdr_struct {
	int				lineno;
	int				size;
	char			filename[CVFILENAMELEN + 1];
	char			guard1;
	unsigned long	first_data;
};

#define HEADER	(sizeof(struct hdr_struct) - sizeof(unsigned long))
#define FOOTER	(sizeof(char))
#define GUARD       0x22
#define INIT_NEW    0x23
#define INIT_DELETE 0x24

#define ptr_SetGuard1(ptr)	\
		(((struct hdr_struct *) ptr)->guard1 = GUARD)

#define ptr_SetGuard2(ptr, size)	\
		(*((char *) ((char *)(ptr) + (size) + HEADER)) = GUARD)

#define ptr_SetLineNo(ptr, _lineno) \
		(((struct hdr_struct *) ptr)->lineno = (_lineno))

#define ptr_SetFileName(ptr, _fname) \
		(strncpy(((struct hdr_struct *) ptr)->filename, (_fname), CVFILENAMELEN + 1))

#define ptr_SetSize(ptr, _size)	\
		(((struct hdr_struct *) ptr)->size = (_size))

#define ptr_GetGuard1(ptr)	\
		(((struct hdr_struct *) ptr)->guard1)

#define ptr_GetGuard2(ptr, size)	\
		(*((char *) ((char *)(ptr) + (size) + HEADER)))

#define ptr_GetLineNo(ptr) \
		(((struct hdr_struct *) ptr)->lineno)

#define ptr_GetSize(ptr)	\
		(((struct hdr_struct *) ptr)->size)

#define ptr_GetFileName(ptr) \
		(((struct hdr_struct *) ptr)->filename)
		
#define ptr_GetFirstData(ptr) \
		(((struct hdr_struct *) ptr)->first_data)

// -------------------------------------------------------------------------

static struct alist_struct alist[ALIST_SIZE];
static struct alist_struct *alist_used = 0; 		// list of free entries
static struct alist_struct *alist_free = 0; 		// list of used entries

static int alist_init = 0;				 			// has the list been initialized?

static void alist_Add(void *);
static void alist_Remove(void *);

#ifdef CV_WIN32
static void console_print(char *fmt...);
static void console_refresh();
#endif // CV_WIN32

// -------------------------------------------------------------------------
void *operator new(size_t size, char *fname, int lineno)
{
	void   *m;
	int		x;
	char*	ss;

	if ((m = cvMalloc(size + HEADER + FOOTER)) == 0) {
		return 0;
	}

	// Strip drive and directory off fname
	if(0 != (ss = strrchr( fname, '\\' )) ) {
		fname = ss + 1;
	}

	// Initialize the allocated memory
	memset(m, INIT_NEW, size + HEADER + FOOTER);

	// Test the heap
	if ((x = cvAllocTest(0, 0)) != 0) {
		ALARM("CVALLOC: (new %s:%d) Heap error with lineno %d.\n",
							fname, lineno, x);
		exit(1);
	}

 	// Set up guard bytes
	ptr_SetLineNo(m, lineno);
	ptr_SetFileName(m, fname);
	ptr_SetSize(m, size);
	ptr_SetGuard1(m);
	ptr_SetGuard2(m, size);

	// Add it to the list
	alist_Add(m);

	cvRefresh();

	return (void *) &(((struct hdr_struct *) m)->first_data);
}

// -------------------------------------------------------------------------
//void *operator new[](size_t size, char *fname, int lineno)
//{
//	return ::operator new(size, fname, lineno);
//}

// -------------------------------------------------------------------------
void *operator new(size_t size)
{
	return ::operator new(size, "unknown", 0);
}

// -------------------------------------------------------------------------
void operator delete(void *buffer)
{
	void   *m;
	int		x;

	if (buffer == 0) {
		return;
	}

	// Test heap integrity
	if ((x = cvAllocTest(0, 0)) != 0) {
		ALARM("CVALLOC: (delete %s:%d) Heap error with lineno %d.\n",
								cvmem_filename, cvmem_lineno, x);
		exit(1);
	}

	// test if this is a valid pointer to delete
	m = (void *) ((char *) buffer - HEADER);

	int 	is_ok = 0;
	struct	alist_struct *temp;

	for (temp = alist_used; temp != 0; temp = temp->next) {
		if (temp->ptr == m) {
			is_ok = 1;
			break;
		}
	}

	if (!is_ok) {
		ALARM("CVALLOC: (delete %s:%d) Attempt to delete bad pointer.\n",
						cvmem_filename, cvmem_lineno);
		exit(1);
	}
	
	// Initialize the memory being freed
	int sz = ptr_GetSize(m);
	memset( m, INIT_DELETE, sz + HEADER + FOOTER );

	// Remove block from the list
	alist_Remove(m);

	// Test heap integrity
	if ((x = cvAllocTest(0, 0)) != 0) {
		ALARM("CVALLOC: (delete %s:%d) Delete has corrupted heap.\n",
								cvmem_filename, cvmem_lineno);
		exit(1);
	}

	cvFree(m);

	cvRefresh();
}

// alist routines

// -------------------------------------------------------------------------
// Add a new entry to the used list.  (from the unused list)
//
static void alist_Add(void *ptr)
{
	register int i;
	struct alist_struct *temp;

	// If list hasn't been set up, then set it up
	if (!alist_init) {

	    PRINT("CV Memtest initializing\n");
		for (i = 0; i < ALIST_SIZE; i++) {
			alist[i].ptr = 0;
			alist[i].next = &alist[i+1];
		}

		alist[ALIST_SIZE - 1].next = 0;
		alist_used = 0;
		alist_free = &alist[0];

		alist_init = 1;
	}

	// Get a slot from the free list
	if (alist_free != 0) {
		temp = alist_free;
		alist_free = alist_free->next;

		temp->next = alist_used;
		temp->ptr  = ptr;
		alist_used = temp;
	}
	else {
		// there are no more slots in the free list
		PRINT("CVALLOC: Allocation list is full.\n");
	}
}

// -------------------------------------------------------------------------
// Remove an entry from the used list.  (add it to the unused list)
//
static void alist_Remove(void *ptr)
{
	struct alist_struct *temp, *last;

	// If list hasn't been set up, then don't do anything
	if (alist_init) {

		// find the ptr in the used list
		for (temp = alist_used, last = 0; 
			 temp != 0; 
			 last = temp, temp = temp->next) {

			if (temp->ptr == ptr) {
				// remove this slot from the used list
				if (last == 0) {
					alist_used = temp->next;
				}
				else {
					last->next = temp->next;
				}

				// put the slot at the top of the free list
				temp->ptr = 0;
				temp->next = alist_free;
				alist_free = temp;
				break;
			}
		}
	}
}

// -------------------------------------------------------------------------
//	Tests integrity of allocated storage.
//
//	mode can have the following values:
//
//	0	silent
//	1	print summaries
//	2	print dump list
//
//	returns 0 if the heap is OK.
//	else the lineno value of the bad block.
//
int cvAllocTest(char *msg, int mode)
{
	int 	error = 0;
	int		size;
	long	used_total = 0L;
	long	item_count = 0L;
	char 	g1, g2;

	struct alist_struct *temp;

	if (msg != 0 && mode > 0) {
		PRINT("%s\n", msg);
	}

	// loop through the used list
	for (temp = alist_used; temp != 0; temp = temp->next) {

		size = ptr_GetSize(temp->ptr);
		used_total += size;
		item_count++;

		// test the block
		g1 = ptr_GetGuard1(temp->ptr);
		g2 = ptr_GetGuard2(temp->ptr, size);

		if (g1 != GUARD || g2 != GUARD) {
			error = ptr_GetLineNo(temp->ptr);
			break;
		}

		if (mode > 1) {
			PRINT("Used block %p of size %4.4X (%5d) file: %*.*s line: %4d.\n", 
				temp->ptr, size, size, 
				CVFILENAMELEN, CVFILENAMELEN,
				ptr_GetFileName(temp->ptr),
				ptr_GetLineNo(temp->ptr));
		}
	}

	if (mode > 0) {
		PRINT("       %ld bytes used by %ld allocations.\n", used_total, item_count);

		if (error == 0) {
			PRINT("CVALLOC: Heap is OK.\n\n");
		}
	}
	if (error != 0) {
		if (msg != 0 && mode <= 0) {
			PRINT("%s\n", msg);
		}
		ALARM("CVALLOC: Error in allocation block %s:%d.\n\n", ptr_GetFileName(temp->ptr), error);
	}
	return error;
} 

// -------------------------------------------------------------------------
int cvAllocRowCount()
{
	struct	alist_struct *temp;
	int 	count = 0;

	for (temp = alist_used; temp != 0; temp = temp->next) {
		count++;
	}

	return count;
}

// -------------------------------------------------------------------------
long cvAllocTotal()
{
	struct	alist_struct *temp;
	long 	total = 0;

	for (temp = alist_used; temp != 0; temp = temp->next) {
		long size = ptr_GetSize(temp->ptr);
		total += size;
	}

	return total;
}

// -------------------------------------------------------------------------
void cvAllocRowData(int rowno, long &ptr, int &size, char * &fname, int &lineno)
{
	struct	alist_struct *temp;
	int 	count = 0;

	for (temp = alist_used; temp != 0; temp = temp->next) {
		if (count == rowno) {
			ptr = (long) temp->ptr,
			size = ptr_GetSize(temp->ptr);
			fname = ptr_GetFileName(temp->ptr);
			lineno = ptr_GetLineNo(temp->ptr);
			break;
		}
		count++;
	}
}

#ifdef CV_WIN32

void console_print(char *fmt...)
{
	if( console_init == 0 ) {
		AllocConsole();
		console_out = GetStdHandle( STD_OUTPUT_HANDLE );
		console_in = GetStdHandle( STD_INPUT_HANDLE );
		if( console_out == INVALID_HANDLE_VALUE ) {
			console_out = 0;
		}
		else {
			SetConsoleTextAttribute( console_out, default_color );
		}
		console_init = 1;
	}		
			
	char	b[2000];
	int		count;
	DWORD	g;
	va_list	arg;
	va_start(arg, fmt);
	count = vsprintf(b, fmt, arg);
	va_end(arg);

	if( console_out != 0) {
		WriteConsole( console_out, b, count, &g, 0);
//		ReadConsole( console_in, b, 50, &g, 0);
	}
	else {
		ReleaseCapture();
		MessageBox((HWND)0, b, "CVALLOC", MB_OK);
	}
}


void console_refresh()
{
	if( console_out == 0 ) {
		return;
	}
	PRINT("%s (%d : %d)\n", cvmem_filename, cvAllocRowCount(), cvAllocTotal());
}


#endif

// -------------------------------------------------------------------------

#endif	// CV_MEMTEST

