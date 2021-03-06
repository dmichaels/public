// $Header:   Q:/views/win/vcs/debug.h_v   1.9   16 May 1997 10:58:50   gregm  $

//	debug.h
//
// OLE debug
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
//	11/08/95 dss	define DebugOutputEnumFmtEtc for non debug.
//	11/15/95 dss	OLESTR'ized. WIN95 OutDebugString is ANSI for wchar.
//	12/05/95 dss	Added includes.
//	02/23/96 evc	Fixed declarations for cdbg
// ---------------------------------------------------------------------------

#ifndef DEBUG_H
#define DEBUG_H

#include "defs.h"		// for windows.h
#include "oledefs.h"
#include "str.h"

#if !defined( NULL)
	#define NULL 0
#endif

#if defined(CV_DEBUG)

CLASS VDebugStream
{
public:
	VDebugStream();
	~VDebugStream() {;}

	VDebugStream&	operator<<(const CHAR c)
						{	CHAR buf[2] = {c,0}; 
#if defined(CV_WIN32)
							OutputDebugStringA(buf);
#else
							OutputDebugString(buf);
#endif							
							return *this;
						}
	VDebugStream&	operator<<(LPCSTR s) 
						{if (s)
#if defined(CV_WIN32)
							OutputDebugStringA(s); 
#else
							OutputDebugString(s); 
#endif							
						 else
							OutputDebugString(_T("<null>"));
						 return *this;}

#if defined(CV_WIN32)
	VDebugStream&	operator<<(LPCWSTR s);
	VDebugStream&	operator<<(const WCHAR c);
#endif

	VDebugStream&	operator<<(const void FAR *p);
	VDebugStream&	operator<<(const int i) {return *this << long(i);}
	VDebugStream&	operator<<(const unsigned int i) {return *this << long(i);}
	VDebugStream&	operator<<(const long l);
	VDebugStream&	operator<<(const unsigned long l) {return *this << long(l);}

	VDebugStream&	operator<<(const RECT& r);
	VDebugStream&	operator<<(const RECTL& r);
	VDebugStream&	operator<<(const POINT& p);
	VDebugStream&	operator<<(const POINTL& p);
	VDebugStream&	operator<<(const SIZEL& s);
	// HRESULT is really a pointer for which we have an operator <<.
//	VDebugStream&	operator<<(const HRESULT hr);
	VDebugStream&	operator<<(const VString& s)
						{operator<<(s.gets()); return *this;}
	
	VDebugStream&	operator<<(const ULARGE_INTEGER& i);
	VDebugStream&	operator<<(const LARGE_INTEGER& i);

	VDebugStream&	operator<<(VDebugStream& (*manip)(VDebugStream&))
						{return (*manip)(*this);}
						
	int				base(int b) {int prev = _base; _base = b; return prev;}
	int				base() const {return _base;}

	TCHAR			fill(TCHAR c) {TCHAR prev = _fill; _fill = c; return prev;}
	TCHAR			fill() const {return _fill;}

	int				precision(int p) {int prev = _prec; _prec = p; return prev;}
	int				precision() const {return _prec;}

	int				width(int w) {int prev = _width; _width = w; return prev;}
	int				width() const {return _width;}
			
protected:
	int			_base;
	TCHAR			_fill;
	int			_width;
	int			_prec;			// precision
};

//***************************************************************************
//
// The VDebugStream manipulators
//
//***************************************************************************

#define DBGMANIP(T) __DBGMANIP_##T

#define DCL_DBGMANIP(T)  \
class DBGMANIP(T) { \
public: \
    DBGMANIP(T)(VDebugStream& (*f)(VDebugStream&,T), T t) { _fp = f; _tp = t; } \
    friend VDebugStream& operator<<(VDebugStream& s, const DBGMANIP(T) & sm) { return sm._fp(s,sm._tp);} \
private:    \
    VDebugStream& (* _fp)(VDebugStream&,T); \
    T _tp; \
}

inline VDebugStream& hex(VDebugStream& dbg) {dbg.base(16); return dbg;}
inline VDebugStream& dec(VDebugStream& dbg) {dbg.base(10); return dbg;}
inline VDebugStream& reset(VDebugStream& dbg)
						{dbg.base(10); dbg.width(0); 
						 dbg.precision(10); dbg.fill('0'); return dbg;}
inline VDebugStream& endl(VDebugStream& dbg)
						{return dbg << '\n' << reset;}

void writeWhiteSpace( int numSpaces);

inline VDebugStream& __setbase(VDebugStream& s, int _base) {s.base(_base); return s;}
inline VDebugStream& __setfill(VDebugStream& s, int _fc) {s.fill((TCHAR)_fc); return s;}
inline VDebugStream& __setprecision(VDebugStream& s, int _pre) {s.precision(_pre); return s;}
inline VDebugStream& __setw(VDebugStream& s, int _wid) {s.width(_wid); return s;}
inline VDebugStream& __writeWhiteSpace( VDebugStream& s, int numSpaces)
                      {writeWhiteSpace(numSpaces); return s;}
DCL_DBGMANIP(int);
DCL_DBGMANIP(long);

inline DBGMANIP(int)	setbase(int _m) {return DBGMANIP(int)(__setbase, _m); }
inline DBGMANIP(int)	setfill(int _m) {return DBGMANIP(int)(__setfill, _m); }
inline DBGMANIP(int)	setprecision(int _p) {return DBGMANIP(int)(__setprecision, _p); }
inline DBGMANIP(int)	setw(int _w) { return DBGMANIP(int)(__setw, _w); }
inline DBGMANIP(int)	ws(int numSpaces)
						{return DBGMANIP(int)(__writeWhiteSpace, numSpaces);}

// THE debug stream
//#if defined (CV_WIN16) && !defined (DLL)
//extern CV_DLL_IMPORT VDebugStream	cdbg;
//#else
//#endif

#if defined (CV_WIN16) && !defined (DLL)
        extern VDebugStream    CV_NEARDATA	   cdbg; 
#elif   defined (CV_WIN16) && defined (DLL)
        extern VDebugStream    CV_DLL_EXPORT  cdbg; 


#elif defined (CV_WIN32) && defined (__BCPLUSPLUS__)
		extern VDebugStream	CV_DLL_IMPORT cdbg;
#elif defined (CV_WIN32) && defined (_MSC_VER)
		extern CV_DLL_IMPORT VDebugStream	cdbg;
#endif

void TestDataObj(LPOLEOBJECT lpOleObject, LPDATAOBJECT lpDataObj);
HRESULT DebugOutputDataObj(LPDATAOBJECT pDataObj);

void DebugOutputFormatEtc(LPFORMATETC FormatEtc);
int DebugOutputFormatEtc2(LPFORMATETC FormatEtc, long lClipFmt, int iLevel);

void DebugOutputStatStg(LPSTATSTG StatStg, int iLevel);

int DebugOutputStatStg2(LPSTATSTG StatStg, long lStgName, int iLevel);

#if defined(CV_WIN16)
void AppDbgDumpObject(LPUNKNOWN lpUnknown, DWORD dwReserved);
#endif

typedef int (FAR* LPDEBUGOUTPUTSTRUCT)(LPVOID, long, int);

int DebugOutputEnumX(
	LPUNKNOWN			pEnumX,
	LPVOID				lpDebugOutputStruct,
	LPVOID 				lpStruct,
	long				lUserData,
	int					iLevel);

int DebugOutputEnumFmtEtc(
	LPDATAOBJECT	lpDataObj,
	LPFORMATETC		pFmtEtc,
	CLIPFORMAT		cfClipFmt);

int DebugOutputEnumStatStg(
	LPSTORAGE		lpStorage,
	LPSTATSTG		lpStatStg,
	LPTSTR			lpStgName,
	int				iLevel);

int DebugOutputEnumStatStg(LPDATAOBJECT pDataObj);

//**************************************************************************
//
// Inline Functions
//

inline int DebugOutputEnumFmtEtc(LPDATAOBJECT	lpDataObj)
	{return DebugOutputEnumFmtEtc(lpDataObj, NULL, 0);}

inline HRESULT DebugOutputDataObj(LPOLEOBJECT pOleObject)
{
	LPDATAOBJECT		lpDataObject;

	HRESULT hResult = pOleObject->QueryInterface( IID_IDataObject,
		(LPVOID FAR *)&lpDataObject);

	if (SUCCEEDED( hResult)) {
		hResult = DebugOutputDataObj(lpDataObject);
		lpDataObject->Release();
		}

	return hResult;
}

inline HRESULT DebugOutputEnumFmtEtc(LPOLEOBJECT pOleObject)
{
	LPDATAOBJECT		lpDataObject;
	
	HRESULT hResult = pOleObject->QueryInterface( IID_IDataObject,
		(LPVOID FAR *)&lpDataObject);

	if (SUCCEEDED( hResult)) {
		DebugOutputEnumFmtEtc(lpDataObject, NULL, 0);
		lpDataObject->Release();
		}

	return hResult;
}

inline void DebugOutputStatStg(LPSTATSTG lpStatStg)
	{DebugOutputStatStg(lpStatStg, 0);}

inline int DebugOutputEnumStatStg(LPSTORAGE		lpStorage)
	{return DebugOutputEnumStatStg(lpStorage, NULL, 0, 0);}

inline int DebugOutputEnumStatStg(
			LPSTORAGE		lpStorage,
			LPSTATSTG		lpStatStg,
			LPTSTR			lpStgName)
{
	return DebugOutputEnumStatStg(lpStorage, lpStatStg, lpStgName, 0);
}

inline void writeWhiteSpace( int numSpaces)
{
	int		rem = numSpaces % 4;

	switch (rem) {
	
	case 1:
		cdbg	<< ' ';
		break;
	case 2:
		cdbg	<< "  ";
		break;
	case 3:
		cdbg	<< "   ";
		break;
	}
	
	numSpaces -= rem;
	
	// at this point numSpaces is evenly divisble by 4

	while (numSpaces > 0) {
		cdbg << "    ";
		numSpaces -= 4;
	}
}

#define CDBG(d)	cdbg	d

#else		// defined(CV_DEBUG)

#define CDBG(d)

#define TestDataObj( lpOleObject,  lpDataObj)
#define DebugOutputDataObj( pDataObj)

#define DebugOutputFormatEtc( FormatEtc)
#define DebugOutputFormatEtc2( FormatEtc,  lClipFmt,  iLevel)

#define DebugOutputStatStg( StatStg,  iLevel)

#define DebugOutputStatStg2( StatStg,  lStgName,  iLevel)

#define AppDbgDumpObject( lpUnknown,  dwReserved)

#define DebugOutputEnumX(pEnumX, lpDebugOutputStruct, lpStruct, lUserData, iLevel)

inline int DebugOutputEnumFmtEtc(
	LPDATAOBJECT	lpDataObj,
	LPFORMATETC		pFmtEtc,
	CLIPFORMAT		cfClipFmt) {return 0;}

inline int DebugOutputEnumFmtEtc(LPDATAOBJECT	lpDataObj) {return 0;}

inline HRESULT DebugOutputEnumFmtEtc(LPOLEOBJECT pOleObject)
	{return ResultFromScode(E_FAIL);}

// #define DebugOutputEnumStatStg(lpStorage, lpStatStg, lpStgName, iLevel)

#define DebugOutputEnumStatStg(pDataObj)

#endif	// defined(CV_DEBUG)

int IID2Name(REFIID riid, LPOLESTR pName, int nNameSize);
LPTSTR IID2Name(REFIID riid);
int TymedToText(DWORD tymed, LPTSTR lpBuf, int nBufSize);
LPTSTR TymedToText(DWORD tymed);
int AspectToText(DWORD dwAspect, LPTSTR lpBuf, int nBufSize);
LPTSTR AspectToText(DWORD dwAspect);
int ClipboardFormatToText(UINT cfFormat, LPTSTR lpBuf, int nBufSize);
LPTSTR ClipboardFormatToText(UINT cfFormat);
int SCodeITFToText(SCODE sc, LPTSTR lpBuf, int nBufSize);
LPTSTR SCodeITFToText(SCODE sc);

#endif // DEBUG_H
