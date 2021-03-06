// $Header:   Q:/views/common/vcs/OBJECT.CPV   1.18   Mar 24 1997 16:12:44   scottt  $

//	object.cpp
//
//	VObject class implementation [Common]
//
//	Allegris Foundation 1.1.00
//	Copyright (c) 1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//	Revision History:
//	-----------------
//	09/09/92 jmd	made WIN31 auto define MS_WINDOWS
//	04/07/93 pmcm	updated deepCopy() for new VArchiver
//	04/07/93 pmcm	changed id to VObject *
//	04/16/93 pmcm	added matching va_end for va_start
//	04/28/93 pmcm	added virtual void getFrom(VArchiver& a, VObject *data);
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//	07/31/93 pkt	eliminated #ifdef macintosh.
//	08/02/93 jmd	added ARCHIVER ifdefs
//	08/18/93 jmd	changed error message
//	10/07/93 dgm	added another deepCopy() to support GUI objects.
//	10/09/93 pmcm	removed vsprintf() under TURBO
//	10/10/93 jmd	SC6 changes.
//	10/25/93 pkt	merged 3.0 changes.
//	01/13/94 dgm	merged and preened
//	01/18/94 pkt	merged with Macintosh; moved virtual functions nil and
//					reset from object.h.
//	01/20/94 pkt	added VObject::isA with VWARNMULTINLINE from object.h.
//	01/28/94 dgm	made nil() and reset() non-inline (again).
//  07/13/94 jmd	changes for __SC__
//  07/14/94 jld	added defs.h dependency for MS_WINDOWS
//  07/14/94 jld	replaced NULL with 0
//	11/09/94 pkt	added Macintosh logging in 'print'.
//	03/06/95 jld	simplified ifdef around vsprintf()
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	preened at time of combining international changes.
//	09/06/95 pkt	removed OFI code.
//	10/10/95 tomf	VStyle changes.
//  02/21/96 evc	properly exported print() for 16 bit windows
//	06/12/96 pkt	added support for port of library to internet server.	
//	07/26/96 pkt	added more support for internet server port.	
//	11/06/96 pkt	revised to work with no notifier.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "winclass.h"
#include "object.h"
#include "archiver.h"
#if defined(CV_REMOTE_GUI)
#include "str.h"
#if !defined(CV_NO_NOTIFIER)
#include "notifier.h"
#endif
#endif

extern "C" {
#	include <stdio.h>
#	include <string.h>
#	include <stdarg.h>
#	include <time.h>
}

#if defined(UNIX) || defined(V_MACINTOSH)
	extern int errz(const char *);
#endif

defineTopClass(VObject)

// ---------------------------------------------------------------------------
//
VObject::VObject()
#ifndef CV_NOARCHIVER
 : temporaryObjectFlag(FALSE)
#endif
{
}

// ---------------------------------------------------------------------------
//	Destroy this VObject.
//
VObject::~VObject()
{
 	;
}

// --------------------------------------------------------------------------
//
boolean VObject::nil()
{
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VObject::reset()
{
}
// ---------------------------------------------------------------------------
// Free this VObject via the C++ destructor.
// (for backwards compatability)
//
boolean VObject::free()
{
	delete this;
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return a copy of this VObject.  This method is implemented by
// calling the VClass object construct() method and calling copyTo with
// the new object.
//
VObject *VObject::copy()
{
	VClass *c = getIsA();
	VObject *obj = (VObject *)c->construct();
	MAP(this, obj);
	return obj;
}

// ---------------------------------------------------------------------------
// Copy this VObject into object obj.  This method copies just the
// memory associated with this VObject.  Subclasses are responsible
// for reimplementing this method to copy additional data.
//
boolean VObject::copyTo(VObject *obj) 
{
#if defined(UNIX) || defined(V_MACINTOSH)
	obj = 0;
	return FALSE;
#else
	char	*s1, *s2;
	unsigned size, i;
	s1 = (char *)this;
	s2 = (char *)obj;
	size = getSize();
	for (i = 0; i < size; i++) {
		*s2++ = *s1++;
	}
	return TRUE;
#endif
}

// ---------------------------------------------------------------------------
//
boolean VObject::copyFrom(VObject *obj)
{
	obj = 0;
	return FALSE;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Return a copy of this VObject and all objects deemed reachable
// from it.  Subclasses are responsible for archiving objects they refer to.
// NOTE: all objects reachable from this VObject must be Archivable,
// i.e., they must have getFrom and putTo methods implemented
// (see the VArchiver class).  The obj argument is optional (defaulted to
// NULL) and if provided, the deep copy is performed from this object to obj.
//
VObject *VObject::deepCopy(VObject *obj)
{
	VArchiver a;
//	a.expandable(TRUE);	the new 04/07/93 VArchiver is always expandable
	a.putObject(this);
//	a.close(0);			this does nothing in the old VArchiver anyway
//	a.at(0);   			use rewind()
	a.rewind();
	return a.getObject(obj);
}

// ---------------------------------------------------------------------------
VObject *VObject::deepCopy(VObject *obj, VObject *data, boolean do_realize)
{
	VArchiver a;
	a.putObject(this);
	a.rewind();
	return a.getObject(obj, data, do_realize);
}
#endif

// ---------------------------------------------------------------------------
// Print the error message s.
//
void VObject::error(const char *s)
{
#if defined(CV_REMOTE_GUI)
# if !defined(CV_NO_NOTIFIER)
	notifier->printMessage(s);
# endif
#elif defined(CV_WINDOWS)
	ReleaseCapture();
	MessageBox((HWND)0, s, "ERROR!", MB_OK);
#elif defined(PM)
 	WinSetCapture(HWND_DESKTOP, NULL);
  	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)s, (PSZ)"ERROR!", 0, MB_OK);
#else
#	if	!defined(MOTIF) && !defined(V_MACINTOSH)
		printf("%s\n", s);
#	else
		errz(s);
#	endif
#endif
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Load the contents of this VObject from the VArchiver a.
// For any class, this method is only responsible for the member
// variables of that class.  Subclasses should call getFrom of their base
// classes.
//
void VObject::getFrom(VArchiver& /* a */ ,
					  VObject * /* data */ , boolean /* do_realize */ )
{
	char buff[80];
	sprintf(buff, "Class %s is not archivable", clsName());
	error(buff);
}
#endif

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
void VObject::getFrom(VArchiver& /* a */)
{
	char buff[80];
	sprintf(buff, "Class %s is not archivable", clsName());
	error(buff);
}
#endif

// ---------------------------------------------------------------------------
// Return TRUE if the object obj is deemed to be equivalent
// to this VObject.  This method defines equivalence by
// memory location of the objects.  Subclasses will redefine this method.
//
boolean VObject::isEqual(VObject *obj) const
{
	return (obj == this) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
// Return an integer > 0 if this VObject is deemed to be greater
// to obj; 0 if obj is deemed to be equal to this VObject; and
// an integer < 0 if this VObject is deemed to be less than obj.
// This method defines comparison by memory location of the objects.
// Subclasses will redefine this method.
//
int VObject::compare(VObject *obj) const
{
	if (this > obj) {
		return 1;
	}
	else if (this < obj) {
		return -1;
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Return the hash value of this VObject based on its memory address.
//
HashKey VObject::hash() const
{
	return (HashKey)this & HASH_KEY_MASK;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
// Load the contents of this VObject from the VArchiver a.
// For any class, this method is only responsible for the method
// variables of that class.  Subclasses should call the putTo of
// their base classes.
//
void VObject::putTo(VArchiver & /* a */)
{
	char buff[80];
	sprintf(buff, "Class %s is not archivable", clsName());
	error(buff);
}
#endif

// ===========================================================================
#ifndef __ultrix	//	!!!	gjl OFI

// ---------------------------------------------------------------------------
// Perform method m on this VObject returning result.
//
boolean VObject::perform(method m)
{
	volatile boolean retval;
	method0 q = (method0)m;
	retval = (this->*q)();
	return retval;
}

// ---------------------------------------------------------------------------
// Perform method m with argument param on this VObject returning
// result.
//
boolean VObject::perform(method m, long param)
{
	volatile boolean retval;
	method1 *p = (method1 *)(&m);
	method1  q = *p;
	retval = (this->*q)(param);
	return retval;
}

// ---------------------------------------------------------------------------
// Perform method m on this VObject with arguments param1
// and param2 returning result.
//
boolean VObject::perform(method m, long param1, long param2)
{
	volatile boolean retval;
	method2 *p = (method2 *)(&m);
	method2  q = *p;
	retval = (this->*q)(param1, param2);
	return retval;
}

#endif	//	!__ultrix gjl OFI
// ===========================================================================

// ---------------------------------------------------------------------------
// Perform method m on this object with arguments param1, param2 and param3
// returning result.
//
boolean VObject::perform(method m, long param1, long param2, long param3)
{
	volatile boolean retval;
	method3 *p = (method3 *)(&m);
	method3  q = *p;
	retval = (this->*q)(param1, param2, param3);
	return retval;
}

// ---------------------------------------------------------------------------
//
#if defined(DLL)
# if defined(PM)
#  if defined(IN_DLL)
#   pragma export(print)
#  endif
void print(char *fmt...)
# else
#if defined (CV_WIN16) 
void _export print(char *fmt...)
#else
CV_DLL_DECL void print(char *fmt...)
#endif
# endif
#else
void print(char *fmt...)
#endif
{
	char b[2000];
	va_list arg;
	va_start(arg, fmt);
	vsprintf(b, fmt, arg);
	va_end(arg);
	if (strlen(b) > 300) {
		strcpy(b + 300, "...");
	}

#if defined(CV_WINDOWS)
# if defined(CV_REMOTE_GUI)
#  if !defined(CV_NO_NOTIFIER)
	notifier->printMessage(b);
#  endif
# else
	ReleaseCapture();
	MessageBox((HWND)0, b, "ERROR!", MB_OK);
# endif
#elif defined(V_MACINTOSH)
#	if defined(CVM_LOG)
		VNotifier::sendToProcess('logr', b);
#	endif
#elif defined(MOTIF)
	errz(b);
#elif defined(PM) 
 	WinSetCapture(HWND_DESKTOP, NULL);
  	WinMessageBox(HWND_DESKTOP, HWND_DESKTOP, (PSZ)b, (PSZ)"ERROR!", 0, MB_OK);
#else
	printf("%s\n", b);
#endif
}

#ifndef ATT
// ---------------------------------------------------------------------------
//
void _assert(const char *s, const char *p, unsigned i)
{
	print("assertion `%s' failed file `%s' line:%d", s, p, i);
}
#endif
// ===========================================================================
//	MS-Windows specific

#ifdef CV_WINDOWS

#if !defined(__BCPLUSPLUS__) &&  !defined(__SC__) && !defined(_MSC_VER)
	extern "C" {
		int vsprintf(char *b, const char *p, void  *a)
		{
			return wvsprintf(b, (char *)p, (char *)a);
		}
	}
#endif

// ---------------------------------------------------------------------------
void fatal() {
	print("fatal");
}

#endif	//	MS-Windows specific
// ===========================================================================

// ---------------------------------------------------------------------------
//
long currentTime()
{
	time_t t;
	time(&t);
	return t;
}
