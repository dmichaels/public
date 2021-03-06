// $Header:   Q:/views/win/vcs/DROPSRC.H_v   1.5   Mar 04 1997 14:12:32   gregm  $

//	CDropSource
//
//  OLE IDropSource Implementation
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
//	12/28/94 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#if !defined( _DROPSOURCE_H_ )
#define _DROPSOURCE_H_

#include "unknown.h"

CLASS CDropSource;
typedef CDropSource FAR *LPCDROPSOURCE;
typedef CDropSource FAR& REFCDROPSOURCE;

CLASS CDropSource : public CUnknown
{
public:
	CDropSource(REFVCOMPONENTWINDOW refCDWindow);
	virtual ~CDropSource();

	LPTSTR			getClassName() {return _T("CDropSource");}

public:

	//  *** IDropSource methods ***
	virtual STDMETHODIMP QueryContinueDrag (BOOL fEscapePressed, DWORD grfKeyState);
	virtual STDMETHODIMP GiveFeedback (DWORD dwEffect);

	// cast operators to equate Cxxx class to Ixxx class
	operator 		LPDROPSOURCE() {return (LPDROPSOURCE)(LPUNKNOWN)getUnknown();}

	VComponentWindow&		getComponentThis() {return *lpCDWindow;}
	VWindow*		getVThis();

protected:
	LPVCOMPONENTWINDOW			lpCDWindow;
};

#define STD_IDROPSOURCE_METHODS() \
	virtual STDMETHODIMP QueryContinueDrag(BOOL bEscapePressed, DWORD grfKeyState) \
			{return getDropSource().QueryContinueDrag(bEscapePressed, grfKeyState);} \
	virtual STDMETHODIMP GiveFeedback(DWORD dwEffect) \
			{return getDropSource().GiveFeedback(dwEffect);}

#define STD_DropSource_METHODS() STD_IDROPSOURCE_METHODS()

#define STD_IDROPSOURCE_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, DropSource, access)

// This macro is embedded in my aggregate parent's class.
// It gives my aggregate parent the C++ API that would occur if inhertiance 
// was used to access this interface.
//
// NOTE: This macro leaves the class inheritance in "protected" member
//		 access mode.
//		 The programmer is advised to follow this macro with a different 
//		 access mode if protected is not desired.
//
#define CONTAIN_IDROPSOURCE_INTERFACE() \
public: \
	STD_IDROPSOURCE_METHODS() \
	STD_IDROPSOURCE_ACCESS(*DropSource) \
protected: \
	CDropSource		*DropSource;

#define INHERIT_IDROPSOURCE_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*DropSource) \
	STD_IDROPSOURCE_METHODS() \
	STD_IDROPSOURCE_ACCESS(*DropSource) \
protected: \
	CDropSource		*DropSource;

//**********************************************************************
//
//  IDropSource Class
//
CLASS XDropSource : public IDropSource
{
public:
	XDropSource(CDropSource *);
	~XDropSource();

	INHERIT_INTERFACE(IDROPSOURCE, DropSource)
};

#endif // _DROPSOURCE_H_
