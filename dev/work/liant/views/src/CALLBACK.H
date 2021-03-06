// $Header:   Q:/views/common/vcs/CALLBACK.H_V   1.10   Mar 24 1997 16:08:20   scottt  $ 

//	callback.h
//
//	VCallback class interface [Common]
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
//	08/25/93 pmcm	moved struct from class.h and made it a class
//  09/01/93 dgm	M8 changes; changed defineCallback() to use methodOf().
//  09/02/93 dgm	Changed defineCallback() & importCallback() to use static.
//	12/23/93 pkt	added workaround for Symantec preprocessor.
//	01/13/94 dgm	merged and preened
//	01/17/94 dgm	cfront C++ changes
//	07/13/94 jld	added dependency on defs.h
//	05/19/95 pkt	selected Windows as common source.
//	06/12/96 pkt	updated HANDLE -> VHANDLE.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VCALLBACK_H
#define VCALLBACK_H

// ---------------------------------------------------------------------------
//
#include "winclass.h"	//	for VHANDLE
#include "object.h"		//	for method
#include "defs.h"		//  for CV_NEARDATA

// ---------------------------------------------------------------------------
//
CLASS VCallbackList;

// ---------------------------------------------------------------------------
//
CLASS VCallback {

public:
	enum AutoRegister { CallbackNone, Real, Ghost, Import };

public:
					VCallback();

					VCallback(	const char *className,
								const char *memberName,
								method m = 0, 
								integer index = -1L,
								AutoRegister reg = CallbackNone);


					~VCallback();	

	constPointer	getClassName() const;
	constPointer	getMemberName() const;
	method 			getMethod() const;
	integer			getIndex() const;
	VCallback 	   *getNext() const;

	boolean			set(const char *className,
						const char *memberName, 
						method 		m = 0,
						integer 	index = -1L);

protected:
	pointer			callbackClassName;
	pointer			callbackMemberName;
	VHANDLE 		hClassName;
	VHANDLE 		hMemberName;
	method			callbackMethod;
	integer			callbackIndex;
	VCallback 	   *callbackNext;
	AutoRegister	callbackReg;

	void 			setMethod(method m);
	void 			setIndex(integer index);
	VCallback 	   *setNext(VCallback *next);

private:
	void			setName(pointer &p, VHANDLE& h, const char *name);

private:
	friend			VCallbackList;
};

// ---------------------------------------------------------------------------
//
inline const char *VCallback::getClassName() const
{
	return callbackClassName;
}

// ---------------------------------------------------------------------------
//
inline const char *VCallback::getMemberName() const
{
	return callbackMemberName;
}

// ---------------------------------------------------------------------------
//
inline method VCallback::getMethod() const
{
	return callbackMethod;
}

// ---------------------------------------------------------------------------
//
inline void VCallback::setMethod(method m)
{
	callbackMethod = m;
}

// ---------------------------------------------------------------------------
//
inline integer VCallback::getIndex() const
{
	return callbackIndex;
}

// ---------------------------------------------------------------------------
//
inline void VCallback::setIndex(integer index)
{
	callbackIndex = index;
}

// ---------------------------------------------------------------------------
//
inline VCallback *VCallback::getNext() const
{
	return callbackNext;
}

// ---------------------------------------------------------------------------
//
inline VCallback *VCallback::setNext(VCallback *next)
{
	VCallback *wasNext = callbackNext;
	callbackNext = next;
	return wasNext;
}

// ---------------------------------------------------------------------------

#ifndef CVP_OLD_CPP 
#	undef  _VTOKENPASTE
#	define _VTOKENPASTE(a,b)   	a ## b
#	undef  _VSTRINGIZE
#	define _VSTRINGIZE(a)		#a
#else
#	undef  _VTOKENPASTE
#	define _VTOKENPASTE(a,b)	a/**/b
#	undef  _VSTRINGIZE
#	define _VSTRINGIZE(a)		"a"
#endif

//	these are necessary for nesting pastes and stringizings

#	undef   VTOKENPASTE
#	define  VTOKENPASTE(a,b)   	_VTOKENPASTE(a,b)
#	undef   VSTRINGIZE
#	define  VSTRINGIZE(a)		_VSTRINGIZE(a)

#ifndef CV_NOARCHIVER

#ifndef VSYMANTECPASTE
#define defineCallback(c, m)	\
		static VCallback CV_NEARDATA VTOKENPASTE(VTOKENPASTE(c,m),_Callback) \
		(VSTRINGIZE(c), VSTRINGIZE(m),methodOf(c,m),-1L,VCallback::Real);

#define importCallback(c, m)	\
		static VCallback CV_NEARDATA VTOKENPASTE(VTOKENPASTE(c,m),_Callback) \
		(VSTRINGIZE(c),VSTRINGIZE(m),0,-1L,VCallback::Import);
#else
#define defineCallback(c, m)	\
		static VCallback CV_NEARDATA VTOKENPASTE(c,m##_Callback)\
		(VSTRINGIZE(c), VSTRINGIZE(m),methodOf(c,m),-1L,VCallback::Real);

#define importCallback(c, m)	\
		static VCallback CV_NEARDATA VTOKENPASTE(c,m ## _Callback)\
		(VSTRINGIZE(c), VSTRINGIZE(m),0, -1L,VCallback::Import);
#endif

#else

defineCallback(c, m)
importCallback(c, m)

#endif

#endif // VCALLBACK_H
