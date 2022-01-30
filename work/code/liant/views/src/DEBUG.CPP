// $Header:   Q:/views/win/vcs/debug.cpv   1.11   Mar 26 1997 10:04:22   gregm  $

//	debug.cpp
//
//	OLE debug
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
//	10/??/95 dss	created.
//	11/15/95 dss	OLESTR'ized. Always use ANSI OutputDebugString for
//					WIN95 compatibility under WIN32.
//	12/06/95 dss	moved definition of cdbg to globals2.cpp.
//	12/06/95 dss	IID2Name(): added GUID_NULL.
//  02/23/96 dss    Borland conversion.
//  04/30/96 evc    prevent parser stack overflow in ms 4.1's compiler
// ---------------------------------------------------------------------------

#include "pre.h"

#if defined(CV_WIN16)
	#include <compobj.h>
	#include <dispatch.h>
#elif defined(CV_WIN32)
	#include <oleauto.h>
#endif

#if defined(__BCPLUSPLUS__)
	// Borland protects these files, which is wrong.
	#undef	_INITGUID
	#undef __OLECTLID_H
#endif

#include <initguid.h>		// for DEFINE_GUID()
#if _MSC_VER <=1000 || __BCPLUSPLUS__
#include <olectlid.h>		// to define the ole control GUID's
#else // Microsft 5.0 gives warning to include olectl.h instead
#include <olectl.h>
#endif

// TO DO: don't understand why this doesn't work in dispatch.h
DEFINE_OLEGUID(IID_StdOle,		0x00020430L, 0, 0);
         
#if defined(CV_DEBUG)

//********************************************************************
// Ctor
VDebugStream::VDebugStream()
{ 
	reset(*this);
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const long l)
{
	char	buf[61],
			*num = buf + 26;
	                   
	ltoa(l, num, _base);

	int len = strlen(num);

	for ( int i = 0; i < _width - len; i++) {
		*--num = _fill;
	}

	switch (_base)
	{

		// prefix "0x"
		case 16: 
			*--num = 'x'; 
			// fall through

		// prefix "0"
		case 8: 
			*--num = '0'; 
			break;

		default:
			break;
	}
	
	OutputDebugString(num);

	return *this;
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const void FAR *p)
{
	char	buf[25];
	wsprintf(buf, "0x%.4x:0x%.4x", HIWORD(p), LOWORD(p));
	OutputDebugString(buf);
	return *this;
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const RECT& r)
{
	return *this << '(' << r.left	<< ", " << r.top	<< ", " 
				 		<< r.right	<< ", " << r.bottom << ')';
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const RECTL& r)
{
	return *this << '(' << r.left	<< ", " << r.top	<< ", " 
				 		<< r.right	<< ", " << r.bottom << ')';
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const POINT& p)
{
	return *this << '(' << p.x << ", " << p.y << ')';
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const POINTL& p)
{
	return *this << '(' << p.x << ", " << p.y << ')';
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const SIZEL& s)
{
	return *this << '(' << s.cx << ", " << s.cy << ')';
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const ULARGE_INTEGER& i)
{ 
	if (ULIGet32Hi(i) != 0) *this << ULIGet32Hi(i); 

  int w = width(0);
  *this << ULIGet32Lo(i); 
  width(w);
  
  return *this;
}

//********************************************************************
VDebugStream& VDebugStream::operator<<(const LARGE_INTEGER& i)
{
	if (ULIGet32Hi(i) != 0) *this << ULIGet32Hi(i); 
	int w = width(0);
	*this << ULIGet32Lo(i);
	width(w);

	return *this;
}


#if 0
//********************************************************************
VDebugStream& VDebugStream::operator<<(const HRESULT hr)
{
	char buf[20];
	wsprintf( buf, "0x%.8lx", (long) hr);
	OutputDebugString(buf);
	return *this;
}
#endif

#if defined(CV_WIN32)
//**********************************************************************
//
VDebugStream&	VDebugStream::operator<<(LPCWSTR s) 
{
	if (s) {
		char	buf[200];
		size_t count;

		count = wcstombs( buf, s, NUM_ELEMENTS_IN(buf));
		cv_assert(count > 0);
		if (count > 0)
			OutputDebugString(buf);
	}
	else
		OutputDebugString("<null>"); 

	return *this;
}

//**********************************************************************
//
VDebugStream&	VDebugStream::operator<<(const WCHAR c)
{
	char		buf[2];
	size_t	count;

	count = wctomb( buf, c);
	cv_assert(count > 0);
	if (count > 0)
		OutputDebugString(buf);

	return *this;
}
#endif

//**********************************************************************
//

#if defined(CV_WIN16)
void AppDbgDumpObject(LPUNKNOWN lpUnknown, DWORD dwReserved)
{
	DbgDumpObject(lpUnknown, dwReserved);
}
#endif

//**********************************************************************
//
// DebugOutputEnumX
//
// enumerates until lpDebugOutputStruct() returns true

int DebugOutputEnumX(
	LPENUMUNKNOWN		lpEnumX,
	LPDEBUGOUTPUTSTRUCT	lpDebugOutputStruct,
	LPVOID 				lpStruct,
	long				lUserData,
	int					iLevel)
{
	int found = FALSE;

	if (lpEnumX) {
		lpEnumX->Reset();
		while (lpEnumX->Next(1, (LPUNKNOWN FAR*) lpStruct, NULL) == S_OK) {
			if (lpDebugOutputStruct(lpStruct, lUserData, iLevel)) {
				found = TRUE;
				break;
			}
		}
	}

	return found;
}

//********************************************************************
//
HRESULT DebugOutputDataObj(LPDATAOBJECT pDataObj)
{
	FORMATETC	fmtetc;
	STGMEDIUM	StgMedium;
	HRESULT		hrErr;

	StgMedium.tymed = TYMED_NULL;
	StgMedium.pUnkForRelease = NULL;
	StgMedium.hGlobal = NULL;

	fmtetc.cfFormat = CF_TEXT;
	fmtetc.ptd = NULL;
	fmtetc.lindex = -1;
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.tymed = TYMED_HGLOBAL;

	hrErr = pDataObj->GetData( &fmtetc, &StgMedium);
	if (hrErr == S_OK) {
		if (StgMedium.tymed == TYMED_HGLOBAL) {
			LPTSTR lpStr = (LPTSTR) GlobalLock(StgMedium.hGlobal);

			cdbg	<< "    DataObj " << pDataObj << " = " << lpStr << endl;

			GlobalUnlock(StgMedium.hGlobal);
			ReleaseStgMedium(&StgMedium);
		}
	}
	else {
		cdbg	<< "    DataObj " << pDataObj << " - GetData failed, result = "
				<< hex << long(hrErr) << endl;
	}

	return hrErr;
}

//**********************************************************************
//
void DebugOutputFormatEtc(LPFORMATETC lpFormatEtc)
{
	cdbg	<< "    fmt =  " << ClipboardFormatToText(lpFormatEtc->cfFormat)
			<< ", aspect = " << AspectToText(lpFormatEtc->dwAspect) 
			<< ", tymed = " << TymedToText(lpFormatEtc->tymed) 
			<< endl;
}

//**********************************************************************
//
// DebugOutputFormatEtc2
//
// it a clipboard format is specified, return whether it matches the
// one in the FORMATETC, else return false.
//

int DebugOutputFormatEtc2(LPFORMATETC lpFormatEtc, long lClipFmt, int iLevel) {
	DebugOutputFormatEtc(lpFormatEtc);
	return (lClipFmt ? (long) lpFormatEtc->cfFormat == lClipFmt : 0);
}

//**********************************************************************
//
void DebugOutputStatStg(LPSTATSTG lpStatStg, int iLevel)
{
	static char near *TypeText[4] = {"undefined", "STORAGE", "STREAM", "LOCKBYTES"};

	cv_assert( lpStatStg);
	cv_assert( lpStatStg->type < 4);
   	cv_assert( lpStatStg->pwcsName);

	if (lpStatStg->type == STGTY_STORAGE) {
		OLECHAR	buf[200];
		int		cnt;

		cnt = StringFromGUID2(lpStatStg->clsid, buf, NUM_ELEMENTS_IN(buf) - 1);
		cv_assert(cnt < NUM_ELEMENTS_IN(buf));

		cdbg	<< ws(2*iLevel)
				<< "    name = " << lpStatStg->pwcsName 
				<< ", CLSID = " << buf << endl;
	}
	else {
		cdbg	<< ws(2*iLevel)
				<< "    name = " << lpStatStg->pwcsName 
				<< ", type = " << TypeText[lpStatStg->type]
				<< ", size = " << lpStatStg->cbSize << endl;
	}

}

//**********************************************************************
//
// DebugOutputStatStg2
//
// it a lStgName is specified, return whether it matches the
// one in the STATSTG, else return false.
//

int DebugOutputStatStg2(LPSTATSTG lpStatStg, long lStgName, int iLevel) {
	DebugOutputStatStg(lpStatStg, iLevel);
#if defined(CV_WIN32)
	return (lStgName ? VWideCharAux::wcscmp(lpStatStg->pwcsName, (LPOLESTR) lStgName) == 0 : 0);
#else
	return (lStgName ? _tcscmp(lpStatStg->pwcsName, (LPTSTR) lStgName) == 0 : 0);
#endif
}

#if 0

//**********************************************************************
//
// DebugOutputEnumFmtEtc
//

int DebugOutputEnumFmtEtc(
	LPDATAOBJECT	lpDataObj,
	LPFORMATETC		pFmtEtc,
	CLIPFORMAT		cfClipFmt)
{
	LPENUMFORMATETC penumFormatEtc;
	int		found = FALSE;

	cdbg	<< "Enumerating GET fmts for Data Object " << lpDataObj << endl;

	lpDataObj->EnumFormatEtc(DATADIR_GET, &penumFormatEtc);

	if (penumFormatEtc) {
		FORMATETC FormatEtc;

		if (!lpFormatEtc) {
			lpFormatEtc = &FormatEtc;
		}

		penumFormatEtc->Reset();

		while (penumFormatEtc->Next(1, lpFormatEtc, NULL) == S_OK) {
			DebugOutputFormatEtc(*lpFormatEtc);

			if (lpFormatEtc->cfFormat == cfClipFmt)) {
				found = TRUE;
				break;
			}
		}
		penumFormatEtc->Release();
	}

	return found;
}

#else

//**********************************************************************
//
// DebugOutputEnumFmtEtc
//

int DebugOutputEnumFmtEtc(
	LPDATAOBJECT	lpDataObj,
	LPFORMATETC		lpFormatEtc,
	CLIPFORMAT		cfClipFmt)
{
	LPENUMFORMATETC penumFormatEtc;
	int		found = FALSE;
	int		iLevel = 0;

	cdbg	<< "    Enumerating GET fmts for Data Object " << lpDataObj
			<< ':' << endl;

	lpDataObj->EnumFormatEtc(DATADIR_GET, &penumFormatEtc);

	if (penumFormatEtc) {
		FORMATETC		FormatEtc;

		if (!lpFormatEtc) {
			lpFormatEtc = &FormatEtc;
		}

		found = DebugOutputEnumX((LPENUMUNKNOWN)penumFormatEtc,
								 (LPDEBUGOUTPUTSTRUCT)DebugOutputFormatEtc2, 
								 lpFormatEtc, (long) cfClipFmt, iLevel);
		penumFormatEtc->Release();
	}

	return found;
}
#endif

//**********************************************************************
//
// DebugOutputEnumStatStg
//
// Returns whether the lpStgName, passed in, was found. Returns FALSE
// if lpStgName is NULL.
//
// lpStgName - Stop enumerating if a storage or stream is found with
//				this name. Can be NULL.
//

int DebugOutputEnumStatStg(
	LPSTORAGE		lpStorage,
	LPSTATSTG		lpStatStg,
	LPTSTR			lpStgName,
	int				iLevel)
{
	LPENUMSTATSTG	penumStatStg;
	int				found = FALSE;
 	STATSTG			StatStg;
	OLEResult		result;

	cdbg	<< ws(2*iLevel)
			<< "    Enumerating StatStg for IStorage " << lpStorage
			<< ':' << endl;

	CLIPFORMAT	cfClipFmt = 0;
	LPOLESTR		lpszUserType = 0;

	result = ReadFmtUserTypeStg(lpStorage, 
								 &cfClipFmt, 
								 &lpszUserType);

	if (SUCCEEDED(result.asResult())) {

		cdbg	<< ws(2*iLevel)
				<< "    clip fmt = " << ClipboardFormatToText(cfClipFmt);

		if (lpszUserType) {
			cdbg << ", user type = " <<  lpszUserType << endl;
			OleStdFree(lpszUserType);
		}
		else {
			CDBG( << ", user type = <none>" << endl);
		}
	}
	else {
		CDBG(	<< ws(2*iLevel)
				<< "    ReadFmtUserTypeStg() failed: "
				<< result.asSystemMessage() << endl);
	}

	result = lpStorage->Stat(&StatStg, STATFLAG_DEFAULT);
	cv_assert(SUCCEEDED(result.asResult()));

	if (iLevel == 0) {
		DebugOutputStatStg(&StatStg, iLevel);
	}

	cdbg	<< ws(2*iLevel) << "    Storage Elements:" << endl;

	lpStorage->EnumElements(0, NULL, 0, &penumStatStg);

	if (penumStatStg) {

		if (!lpStatStg) {
			lpStatStg = &StatStg;
		}

		LPOLESTR	pwcsName = 0;

		penumStatStg->Reset();
		while (penumStatStg->Next(1, (LPSTATSTG) lpStatStg, NULL) == S_OK) {
			pwcsName = lpStatStg->pwcsName;

			if (DebugOutputStatStg2(lpStatStg, (long) lpStgName, iLevel)) {
				found = TRUE;
				break;
			}

			// recursive descent
			if (lpStatStg->type == STGTY_STORAGE && lpStatStg->pwcsName) {
				LPSTORAGE		lpSubStorage;

				if (SUCCEEDED(lpStorage->OpenStorage(pwcsName, NULL, 
											  STGM_READ | STGM_SHARE_EXCLUSIVE, 
											  NULL, 0,
											  &lpSubStorage)
							  ) ) {
					found = DebugOutputEnumStatStg(lpSubStorage, lpStatStg, 0, 
											   	iLevel + 1);

					lpSubStorage->Release();

					if (found) {
						break;
					}
				}
			
			} // recursive descent

			if (pwcsName) {
 				OleStdFree(pwcsName);
				pwcsName = 0;
			}

		} // while

		if (pwcsName) {
			OleStdFree(pwcsName);
			pwcsName = 0;
		}

		penumStatStg->Release();

	} // if (penumStatStg)

	return found;
}

//**********************************************************************
//
// DebugOutputEnumStatStg
//
extern CLIPFORMAT g_cfEmbedSource;
extern CLIPFORMAT g_cfEmbeddedObject;

int DebugOutputEnumStatStg(LPDATAOBJECT pDataObj)
{
	FORMATETC		FormatEtc;
	STGMEDIUM		StgMedium;
	int				found = FALSE;
	LPSTORAGE		lpStorage;

	FormatEtc.cfFormat = g_cfEmbedSource;
	FormatEtc.ptd = NULL;
	FormatEtc.lindex = -1;
	FormatEtc.dwAspect = DVASPECT_CONTENT;
	FormatEtc.tymed = TYMED_ISTORAGE;

	HRESULT hResult = StgCreateDocfile(
				NULL,
				STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE |
					STGM_DELETEONRELEASE,
				0,
				&lpStorage);

	cv_assert(SUCCEEDED(hResult));

	cdbg	<< "    Enumerating IStorage(" << lpStorage 
			<< ") from IDataObject(" << pDataObj << ')' << endl;

	StgMedium.tymed = TYMED_ISTORAGE;
	StgMedium.pUnkForRelease = lpStorage;
	StgMedium.pstg = lpStorage;

	hResult = pDataObj->GetDataHere( &FormatEtc, &StgMedium);

	if (SUCCEEDED( hResult) && StgMedium.tymed == TYMED_ISTORAGE) {
		found = DebugOutputEnumStatStg(StgMedium.pstg);
	}
	else {
		FormatEtc.cfFormat = g_cfEmbeddedObject;
		FormatEtc.ptd = NULL;
		FormatEtc.lindex = -1;
		FormatEtc.dwAspect = DVASPECT_CONTENT;
		FormatEtc.tymed = TYMED_ISTORAGE;

		StgMedium.tymed = TYMED_ISTORAGE;
		StgMedium.pUnkForRelease = lpStorage;
		StgMedium.pstg = lpStorage;
	
		hResult = pDataObj->GetDataHere( &FormatEtc, &StgMedium);

		if (SUCCEEDED( hResult) && StgMedium.tymed == TYMED_ISTORAGE) {
			found = DebugOutputEnumStatStg(StgMedium.pstg);
		}
	}

	ReleaseStgMedium(&StgMedium);

	return found;

}
#endif // defined(CV_DEBUG)         

//********************************************************************
//
int IID2Name(REFIID riid, LPTSTR pName, int nNameSize)
{

	// Basic Ole
	 _tcscpy( pName, _T("No definition"));


	if (riid == IID_IEnumUnknown) {
		_tcscpy( pName, _T("IID_IEnumUnknown"));
	} else if (riid == IID_IEnumString) {
		_tcscpy( pName, _T("IID_IEnumString"));
	} else if (riid == IID_IEnumMoniker) {
		_tcscpy( pName, _T("IID_IEnumMoniker"));
	} else if (riid == IID_IEnumFORMATETC) {
		_tcscpy( pName, _T("IID_IEnumFORMATETC"));
	} else if (riid == IID_IEnumOLEVERB) {
		_tcscpy( pName, _T("IID_IEnumOLEVERB"));
	} else if (riid == IID_IEnumSTATDATA) {
		_tcscpy( pName, _T("IID_IEnumSTATDATA"));
	} else if (riid == IID_IEnumGeneric) {
		_tcscpy( pName, _T("IID_IEnumGeneric"));
	} else if (riid == IID_IEnumHolder) {
		_tcscpy( pName, _T("IID_IEnumHolder"));
	} else if (riid == IID_IEnumCallback) {
		_tcscpy( pName, _T("IID_IEnumCallback"));
	} else if (riid == IID_IPersistStream) {
		_tcscpy( pName, _T("IID_IPersistStream"));
	} else if (riid == IID_IPersistStorage) {
		_tcscpy( pName, _T("IID_IPersistStorage"));
	} else if (riid == IID_IPersistFile) {
		_tcscpy( pName, _T("IID_IPersistFile"));
	} else if (riid == IID_IPersist) {
		_tcscpy( pName, _T("IID_IPersist"));
	} else if (riid == IID_IViewObject) {
		_tcscpy( pName, _T("IID_IViewObject"));
	} else if (riid == IID_IDataObject) {
		_tcscpy( pName, _T("IID_IDataObject"));
	} else if (riid == IID_IAdviseSink) {
		_tcscpy( pName, _T("IID_IAdviseSink"));
	} else if (riid == IID_IDataAdviseHolder) {
		_tcscpy( pName, _T("IID_IDataAdviseHolder"));
	} else if (riid == IID_IOleAdviseHolder) {
		_tcscpy( pName, _T("IID_IOleAdviseHolder"));
	} else if (riid == IID_IOleObject) {
		_tcscpy( pName, _T("IID_IOleObject"));
	} else if (riid == IID_IOleInPlaceObject) {
		_tcscpy( pName, _T("IID_IOleInPlaceObject"));
	} else if (riid == IID_IOleWindow) {
		_tcscpy( pName, _T("IID_IOleWindow"));
	} else if (riid == IID_IOleInPlaceUIWindow) {
		_tcscpy( pName, _T("IID_IOleInPlaceUIWindow"));
	} else if (riid == IID_IOleInPlaceFrame) {
		_tcscpy( pName, _T("IID_IOleInPlaceFrame"));
	} else if (riid == IID_IOleInPlaceActiveObject) {
		_tcscpy( pName, _T("IID_IOleInPlaceActiveObject"));
	} else if (riid == IID_IOleClientSite) {
		_tcscpy( pName, _T("IID_IOleClientSite"));
	} else if (riid == IID_IOleInPlaceSite) {
		_tcscpy( pName, _T("IID_IOleInPlaceSite"));
	} else if (riid == IID_IParseDisplayName) {
		_tcscpy( pName, _T("IID_IParseDisplayName"));
	} else if (riid == IID_IOleContainer) {
		_tcscpy( pName, _T("IID_IOleContainer"));
	} else if (riid == IID_IOleItemContainer) {
		_tcscpy( pName, _T("IID_IOleItemContainer"));
	} else if (riid == IID_IOleLink) {
		_tcscpy( pName, _T("IID_IOleLink"));
	} else if (riid == IID_IOleCache) {
		_tcscpy( pName, _T("IID_IOleCache"));
	} else if (riid == IID_IOleManager) {
		_tcscpy( pName, _T("IID_IOleManager"));
	} else if (riid == IID_IOlePresObj) {
		_tcscpy( pName, _T("IID_IOlePresObj"));
	} else if (riid == IID_IDropSource) {
		_tcscpy( pName, _T("IID_IDropSource"));
	} else if (riid == IID_IDropTarget) {
		_tcscpy( pName, _T("IID_IDropTarget"));
	} else if (riid == IID_IDebug) {
		_tcscpy( pName, _T("IID_IDebug"));
	} else if (riid == IID_IDebugStream) {
		_tcscpy( pName, _T("IID_IDebugStream"));
	} else if (riid == IID_IAdviseSink2) {
		_tcscpy( pName, _T("IID_IAdviseSink2"));
	} else if (riid == IID_IRunnableObject) {
		_tcscpy( pName, _T("IID_IRunnableObject"));
	} else if (riid == IID_IViewObject2) {
		_tcscpy( pName, _T("IID_IViewObject2"));
	} else if (riid == IID_IOleCache2) {
		_tcscpy( pName, _T("IID_IOleCache2"));
	} else if (riid == IID_IOleCacheControl) {
		_tcscpy( pName, _T("IID_IOleCacheControl"));
	} else if (riid == IID_IUnknown) {
		_tcscpy( pName, _T("IID_IUnknown"));
	} else if (riid == IID_IClassFactory) {
		_tcscpy( pName, _T("IID_IClassFactory"));
	} else if (riid == IID_IMalloc) {
		_tcscpy( pName, _T("IID_IMalloc"));
	} else if (riid == IID_IMarshal) {
		_tcscpy( pName, _T("IID_IMarshal"));
	} else if (riid == IID_IRpcChannel) {
		_tcscpy( pName, _T("IID_IRpcChannel"));
	} else if (riid == IID_IRpcStub) {
		_tcscpy( pName, _T("IID_IRpcStub"));
	} else if (riid == IID_IStubManager) {
		_tcscpy( pName, _T("IID_IStubManager"));
	} else if (riid == IID_IRpcProxy) {
		_tcscpy( pName, _T("IID_IRpcProxy"));
	} else if (riid == IID_IProxyManager) {
		_tcscpy( pName, _T("IID_IProxyManager"));
	} else if (riid == IID_IPSFactory) {
		_tcscpy( pName, _T("IID_IPSFactory"));
	} else if (riid == IID_ILockBytes) {
		_tcscpy( pName, _T("IID_ILockBytes"));
	} else if (riid == IID_IStorage) {
		_tcscpy( pName, _T("IID_IStorage"));
	} else if (riid == IID_IStream) {
		_tcscpy( pName, _T("IID_IStream"));
	} else if (riid == IID_IEnumSTATSTG) {
		_tcscpy( pName, _T("IID_IEnumSTATSTG"));
		
	// COM
		
	} else if (riid == IID_IBindCtx) {
		_tcscpy( pName, _T("IID_IBindCtx"));
	} else if (riid == IID_IMoniker) {
		_tcscpy( pName, _T("IID_IMoniker"));
	} else if (riid == IID_IRunningObjectTable) {
		_tcscpy( pName, _T("IID_IRunningObjectTable"));
	} else if (riid == IID_IInternalMoniker) {
		_tcscpy( pName, _T("IID_IInternalMoniker"));
	} else if (riid == IID_IRootStorage) {
		_tcscpy( pName, _T("IID_IRootStorage"));
	} else if (riid == IID_IDfReserved1) {
		_tcscpy( pName, _T("IID_IDfReserved1"));
	} else if (riid == IID_IDfReserved2) {
		_tcscpy( pName, _T("IID_IDfReserved2"));
	} else if (riid == IID_IDfReserved3) {
		_tcscpy( pName, _T("IID_IDfReserved3"));
	} else if (riid == IID_IMessageFilter) {
		_tcscpy( pName, _T("IID_IMessageFilter"));
	} else if (riid == CLSID_StdMarshal) {
		_tcscpy( pName, _T("CLSID_StdMarshal"));
	} else if (riid == IID_IStdMarshalInfo) {
		_tcscpy( pName, _T("IID_IStdMarshalInfo"));
	} else if (riid == IID_IExternalConnection) {
		_tcscpy( pName, _T("IID_IExternalConnection"));

	// Dispatch
	} else if (riid == IID_IDispatch) {
		_tcscpy( pName, _T("IID_IDispatch"));
	} else if (riid == IID_IEnumVARIANT) {
		_tcscpy( pName, _T("IID_IEnumVARIANT"));
	} else if (riid == IID_ITypeInfo) {
		_tcscpy( pName, _T("IID_ITypeInfo"));
	} else if (riid == IID_ITypeLib) {
		_tcscpy( pName, _T("IID_ITypeLib"));
	} else if (riid == IID_ITypeComp) {
		_tcscpy( pName, _T("IID_ITypeComp"));
	} else if (riid == IID_ICreateTypeInfo) {
		_tcscpy( pName, _T("IID_ICreateTypeInfo"));
	} else if (riid == IID_ICreateTypeLib) {
		_tcscpy( pName, _T("IID_ICreateTypeLib"));
	} else if (riid == IID_StdOle) {
		_tcscpy( pName, _T("IID_StdOle"));

	// Ole Controls

	} else if (riid == IID_IPropertyNotifySink) {
		_tcscpy( pName, _T("IID_IPropertyNotifySink"));
	} else if (riid == IID_IClassFactory2) {
		_tcscpy( pName, _T("IID_IClassFactory2"));
	} else if (riid == IID_IProvideClassInfo) {
		_tcscpy( pName, _T("IID_IProvideClassInfo"));
// Start of Sanjay's Code
	//} else if (riid == IID_IProvideClassInfo2) {
	//	_tcscpy( pName, _T("IID_IProvideClassInfo2"));
// End of Sanjay's Code
	} else if (riid == IID_IConnectionPointContainer) {
		_tcscpy( pName, _T("IID_IConnectionPointContainer"));
	} else if (riid == IID_IEnumConnectionPoints) {
		_tcscpy( pName, _T("IID_IEnumConnectionPoints"));
	} else if (riid == IID_IConnectionPoint) {
		_tcscpy( pName, _T("IID_IConnectionPoint"));
	} else if (riid == IID_IEnumConnections) {
		_tcscpy( pName, _T("IID_IEnumConnections"));
	} else if (riid == IID_IOleControl) {
		_tcscpy( pName, _T("IID_IOleControl"));
	} else if (riid == IID_IOleControlSite) {
		_tcscpy( pName, _T("IID_IOleControlSite"));
	} else if (riid == IID_ISimpleFrameSite) {
		_tcscpy( pName, _T("IID_ISimpleFrameSite"));
	} else if (riid == IID_IPersistStreamInit) {
		_tcscpy( pName, _T("IID_IPersistStreamInit"));
	} else if (riid == IID_IPropertyFrame) {
		_tcscpy( pName, _T("IID_IPropertyFrame"));
	} else if (riid == IID_ISpecifyPropertyPages) {
		_tcscpy( pName, _T("IID_ISpecifyPropertyPages"));
	} else if (riid == IID_IPerPropertyBrowsing) {
		_tcscpy( pName, _T("IID_IPerPropertyBrowsing"));
	} else if (riid == IID_IPropertyPageSite) {
		_tcscpy( pName, _T("IID_IPropertyPageSite"));
	} else if (riid == IID_IPropertyPage) {
		_tcscpy( pName, _T("IID_IPropertyPage"));
	} else if (riid == IID_IPropertyPage2) {
		_tcscpy( pName, _T("IID_IPropertyPage2"));
	} else if (riid == IID_IFont) {
		_tcscpy( pName, _T("IID_IFont"));
	} else if (riid == IID_IFontDisp) {
		_tcscpy( pName, _T("IID_IFontDisp"));
	} else if (riid == IID_IPicture) {
		_tcscpy( pName, _T("IID_IPicture"));
	} else if (riid == IID_IPictureDisp) {
		_tcscpy( pName, _T("IID_IPictureDisp"));

	// Views Extensions
	} else if (riid == IID_ViewsObject) {
		_tcscpy( pName, _T("IID_ViewsObject"));
	} 
	else if (riid == GUID_NULL) {
		_tcscpy( pName, _T("GUID_NULL"));
	}

	if (  _tcscmp ( pName, _T("No definition")) == 0){
	
		int count;

#if defined(CV_WIN32)
		OLECHAR buf[61];
		count = StringFromGUID2( riid, buf, 60);
		VWideCharAux::wcstombs(pName, buf, nNameSize);
#else
		count = StringFromGUID2( riid, pName, nNameSize);
#endif

		pName[count] = '\0';
	}

	return lstrlen( pName);
}

//********************************************************************
//
LPTSTR IID2Name(REFIID riid)
{
	static TCHAR near buf[61];

	IID2Name(riid, buf, NUM_ELEMENTS_IN(buf) - 1);

	return buf;
}

//********************************************************************
//
int TymedToText(DWORD tymed, LPTSTR lpBuf, int nBufSize)
{
	if (tymed == TYMED_HGLOBAL) {
		_tcscpy( lpBuf, _T("TYMED_HGLOBAL"));
	} else if (tymed == TYMED_FILE) {
		_tcscpy( lpBuf, _T("TYMED_FILE"));
	} else if (tymed == TYMED_ISTREAM) {
		_tcscpy( lpBuf, _T("TYMED_ISTREAM"));
	} else if (tymed == TYMED_ISTORAGE) {
		_tcscpy( lpBuf, _T("TYMED_ISTORAGE"));
	} else if (tymed == TYMED_GDI) {
		_tcscpy( lpBuf, _T("TYMED_GDI"));
	} else if (tymed == TYMED_MFPICT) {
		_tcscpy( lpBuf, _T("TYMED_MFPICT"));
	} else if (tymed == TYMED_NULL) {
		_tcscpy( lpBuf, _T("TYMED_NULL"));
	} else {
		wsprintf( lpBuf, _T("%ld"), (long) tymed);
	}

	return lstrlen(lpBuf);
}

//********************************************************************
//
LPTSTR TymedToText(DWORD tymed)
{
	static TCHAR near buf[120];

	TymedToText(tymed, (LPTSTR) buf, NUM_ELEMENTS_IN(buf));

	return buf;
}

//********************************************************************
//
int AspectToText(DWORD dwAspect, LPTSTR lpBuf, int nBufSize)
{
	if (dwAspect == DVASPECT_CONTENT) {
		_tcscpy( lpBuf, _T("DVASPECT_CONTENT"));
	} else if (dwAspect == DVASPECT_THUMBNAIL) {
		_tcscpy( lpBuf, _T("DVASPECT_THUMBNAIL"));
	} else if (dwAspect == DVASPECT_ICON) {
		_tcscpy( lpBuf, _T("DVASPECT_ICON"));
	} else if (dwAspect == DVASPECT_DOCPRINT) {
		_tcscpy( lpBuf, _T("DVASPECT_DOCPRINT"));
	} else {
		wsprintf( lpBuf, _T("%ld"), (long) dwAspect);
	}

	return lstrlen(lpBuf);
}

//********************************************************************
//
LPTSTR AspectToText(DWORD dwAspect)
{
	static TCHAR near buf[120];

	AspectToText(dwAspect, (LPTSTR) buf, NUM_ELEMENTS_IN(buf));

	return buf;
}

//********************************************************************
//
int ClipboardFormatToText(UINT cfFormat, LPTSTR lpBuf, int nBufSize)
{
	int count;

	if ((count = GetClipboardFormatName(cfFormat, lpBuf, nBufSize)) > 0) {
		return count;
	}

	switch (cfFormat) {
		case CF_TEXT:
			_tcscpy( lpBuf, _T("CF_TEXT"));
			break;
		case CF_BITMAP:
			_tcscpy( lpBuf, _T("CF_BITMAP"));
			break;
		case CF_METAFILEPICT:
			_tcscpy( lpBuf, _T("CF_METAFILEPICT"));
			break;
		case CF_SYLK:
			_tcscpy( lpBuf, _T("CF_SYLK"));
			break;
		case CF_DIF:
			_tcscpy( lpBuf, _T("CF_DIF"));
			break;
		case CF_TIFF:
			_tcscpy( lpBuf, _T("CF_TIFF"));
			break;
		case CF_OEMTEXT:
			_tcscpy( lpBuf, _T("CF_OEMTEXT"));
			break;
		case CF_DIB:
			_tcscpy( lpBuf, _T("CF_DIB"));
			break;
		case CF_PALETTE:
			_tcscpy( lpBuf, _T("CF_PALETTE"));
			break;
		case CF_PENDATA:
			_tcscpy( lpBuf, _T("CF_PENDATA"));
			break;
		case CF_RIFF:
			_tcscpy( lpBuf, _T("CF_RIFF"));
			break;
		case CF_WAVE:
			_tcscpy( lpBuf, _T("CF_WAVE"));
			break;

		case CF_OWNERDISPLAY:
			_tcscpy( lpBuf, _T("CF_OWNERDISPLAY"));
			break;
		case CF_DSPTEXT:
			_tcscpy( lpBuf, _T("CF_DSPTEXT"));
			break;
		case CF_DSPBITMAP:
			_tcscpy( lpBuf, _T("CF_DSPBITMAP"));
			break;
		case CF_DSPMETAFILEPICT:
			_tcscpy( lpBuf, _T("CF_DSPMETAFILEPICT"));
			break;

		default:
			//!!!dss: wsprintf is not TCHAR compatible!
			wsprintf( lpBuf, _T("%ld"), (long) cfFormat);
			break;
	}

	return _tcslen(lpBuf);

}

//********************************************************************
//
LPTSTR ClipboardFormatToText(UINT cfFormat)
{
	static TCHAR near buf[120];

	ClipboardFormatToText(cfFormat, (LPTSTR) buf, NUM_ELEMENTS_IN(buf));

	return buf;
}

//********************************************************************
//
int SCodeITFToText(SCODE sc, LPTSTR lpBuf, int nBufSize)
{
	if (SCODE_CODE(sc) >= 0 && SCODE_CODE(sc) < 0x0100) {
		_tcscpy( lpBuf, _T("general"));
	} 
	else if (SCODE_CODE(sc) >= 0x0100 && SCODE_CODE(sc) < 0x0110) {
		_tcscpy( lpBuf, _T("drag & drop"));
	} 
	else if (SCODE_CODE(sc) >= 0x0110 && SCODE_CODE(sc) < 0x0120) {
		_tcscpy( lpBuf, _T("class factory"));
	} 
	else if (SCODE_CODE(sc) >= 0x0120 && SCODE_CODE(sc) < 0x0130) {
		_tcscpy( lpBuf, _T("marshal"));
	} 
	else if (SCODE_CODE(sc) >= 0x0130 && SCODE_CODE(sc) < 0x0140) {
		_tcscpy( lpBuf, _T("data object"));
	}
	else if (SCODE_CODE(sc) >= 0x0140 && SCODE_CODE(sc) < 0x0150) {
		_tcscpy( lpBuf, _T("view object"));
	}
	else if (SCODE_CODE(sc) >= 0x0150 && SCODE_CODE(sc) < 0x0160) {
		_tcscpy( lpBuf, _T("registiration"));
	}
	
//  the 160 range is reserved by OLE. It has no implementation
//	else if (SCODE_CODE(sc) >= 0x0160 && SCODE_CODE(sc) < 0x0170) {
//		_tcscpy( lpBuf, _T("unimplemented"));
//	}

	else if (SCODE_CODE(sc) >= 0x0170 && SCODE_CODE(sc) < 0x0180) {
		_tcscpy( lpBuf, _T("cache"));
	}
	else if (SCODE_CODE(sc) >= 0x0180 && SCODE_CODE(sc) < 0x0190) {
		_tcscpy( lpBuf, _T("ole object"));
	}
	else if (SCODE_CODE(sc) >= 0x0190 && SCODE_CODE(sc) < 0x01a0) {
		_tcscpy( lpBuf, _T("ole client site"));
	}
	else if (SCODE_CODE(sc) >= 0x01a0 && SCODE_CODE(sc) < 0x01b0) {
		_tcscpy( lpBuf, _T("in place"));
	}
	else if (SCODE_CODE(sc) >= 0x01b0 && SCODE_CODE(sc) < 0x01c0) {
		_tcscpy( lpBuf, _T("enum"));
	}
	else if (SCODE_CODE(sc) >= 0x01c0 && SCODE_CODE(sc) < 0x01d0) {
		_tcscpy( lpBuf, _T("convert OLE 1.0"));
	}
	else if (SCODE_CODE(sc) >= 0x01d0 && SCODE_CODE(sc) < 0x01e0) {
		_tcscpy( lpBuf, _T("clipboard"));
	}
	else if (SCODE_CODE(sc) >= 0x01e0 && SCODE_CODE(sc) < 0x01f0) {
		_tcscpy( lpBuf, _T("moniker"));
	}
	else if (SCODE_CODE(sc) >= 0x01f0 && SCODE_CODE(sc) < 0x0200) {
		_tcscpy( lpBuf, _T("component object"));
	}
	else {
		_tcscpy( lpBuf, _T("ITF"));
	}
	
	return lstrlen(lpBuf);
}

//********************************************************************
//
LPTSTR SCodeITFToText(SCODE sc)
{
	static TCHAR near buf[25];

	SCodeITFToText(sc, (LPTSTR) buf, NUM_ELEMENTS_IN(buf));

	return buf;
}

