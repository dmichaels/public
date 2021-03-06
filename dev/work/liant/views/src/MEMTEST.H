// $Header:   Q:/views/win/vcs/memtest.h_v   1.4   Mar 04 1997 16:32:40   gregm  $ 

//	memtest.h
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
//	Revision Hisotry:
//	----------------
//	---------------------------------------------------------------------------
#ifndef VMEMTEST_H
#define VMEMTEST_H

#define CVALLOC_SILENT	0
#define CVALLOC_TERSE	1
#define CVALLOC_FULL	2

#ifdef CV_MEMTEST

extern int		cvmem_lineno;
extern char    *cvmem_filename;

#	include <stdlib.h>

	void *operator new(size_t sz, char *fname, int lineno);
//	void *operator new[](size_t sz, char *fname, int lineno);

#	define CV_NEW	new(__FILE__, __LINE__)
#	define new		CV_NEW

#	define CV_DELETE	cvmem_lineno = __LINE__, cvmem_filename = __FILE__, delete
#	define delete		CV_DELETE

    extern int  cvAllocTest(char *msg, int mode);
	extern long cvAllocTotal();
	extern int  cvAllocRowCount();
	extern void cvAllocRowData(int rowno, long &ptr, int &size, char* &fname, int &lineno);

#else

#	define cvAllocTest(x, y)

#endif // CV_MEMTEST

#endif // VMEMTEST_H

