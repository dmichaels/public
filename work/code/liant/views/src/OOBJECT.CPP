// $Header:   Q:/views/win/vcs/oobject.cpv   1.6   Mar 21 1997 08:43:04   gregm  $

//	COleObject
//
//  OLE IOleObject Implementation
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
//	12/04/95 dss	Added PVCS header.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
// XOleObject Interface
//

//**********************************************************************
//
XOleObject::XOleObject(COleObject *pOleObject)
	: OleObject( pOleObject)
{
}

//**********************************************************************
XOleObject::~XOleObject()
{
}

//**********************************************************************
//
// CROleObject Implementation (IOleObject Reference Class)
//

//**********************************************************************
//
void CROleObject::init(void)
{   
	// uncomment when CRUnknown supports aggregation
	// exposeInterface(IID_IOleObject, "COleObject");
}

//**********************************************************************
CROleObject::CROleObject(LPOLEOBJECT lpIOleObject)
	: CRUnknown(lpIOleObject)
{
	lCount = 1;
}

//**********************************************************************
CROleObject::~CROleObject()
{
}

//**********************************************************************
//
// COleObject Implementation
//

//**********************************************************************
//
void COleObject::init(void)
{
	exposeInterface(IID_IOleObject, "COleObject");
}

//**********************************************************************
//
//	refUnkWindow - the Outer VWindow CUnknown
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
COleObject::COleObject(REFCUNKNOWN refCUnknown)
	: CUnknown(refCUnknown, new XOleObject(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	CDBG(	<< "In COleObject's aggregate, sub-class constructor, this = "
			<< this << endl);

	init();
}

//**********************************************************************
//
//	refUnkWindow - the Outer VWindow CUnknown
//	refUnkBase - the Base Class IUnknown that this object is a super-class of.
//
COleObject::COleObject(REFCUNKNOWN refCUnknown, LPOLEOBJECT pOleObject)
	: CUnknown(refCUnknown, pOleObject)
{
	CDBG(	<< "In COleObject's aggregate, super-class constructor, this = "
			<< this << endl);

	init();
}

//**********************************************************************
//
COleObject::~COleObject()
{
	CDBG(	<< "In COleObject's destructor" << endl);
}

//********************************************************************
//
// Cache
//
// lpDataObj - data obj to initialize the cache, Can be NULL
//
//
HRESULT CROleObject::Cache(LPDATAOBJECT lpDataObj, CLIPFORMAT cfClipFormat, 
						  DWORD FAR *lpdwConnection)
{
	LPOLECACHE	lpCache = 0;
	FORMATETC	fmtetc;
  	HRESULT		hResult;

	CDBG(	<< "in COleObject::Cache(" << (LPOLEOBJECT) &(*this) << ", " << lpDataObj << ')' 
			<< endl);

	hResult = QueryInterface( IID_IOleCache, (LPVOID FAR*) &lpCache);
	
	if (FAILED(hResult)) {
		CDBG(	<< "    QueryInterface(IID_IOleCache) on " << (LPOLEOBJECT) &(*this)
				<< "failed" << endl 
				<< "        with result " << hex << setw(8) 
				<< long(hResult) << endl);
		lpCache->Release();
		return hResult;
	}

	fmtetc.cfFormat = cfClipFormat;
	fmtetc.ptd = NULL;
	fmtetc.lindex = -1;
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.tymed = TYMED_HGLOBAL;

//	hResult = lpCache->Cache(&fmtetc, ADVF_PRIMEFIRST, lpdwConnection);
	hResult = lpCache->Cache(&fmtetc, 
									 ADVF_PRIMEFIRST | ADVFCACHE_FORCEBUILTIN, 
									 lpdwConnection);

	CDBG(	<< "    DataObj = " << lpDataObj << ", connection = "
			<< *lpdwConnection << endl 
			<< "    Cache result = " << hex << setw(8) << long(hResult)
			<< endl);

	// if the cache succeeded, then initialize the cache, if 
	// initialization data is passed in.
	if (SUCCEEDED(hResult) && lpDataObj) {
		HRESULT hResult2 = lpCache->InitCache(lpDataObj);
		CDBG(	<< "    InitCache result = " << hex << long(hResult2) << endl);
		DebugOutputDataObj(lpDataObj);
	 }

	lpCache->Release();

	return hResult;
}



