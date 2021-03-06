// $Header:   Q:/views/win/vcs/xferagnt.cpv   1.9   Mar 21 1997 08:43:06   gregm  $

//	xferagnt.cpp
//
//  Compound Document Transfer Agent
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
//	04/03/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	02/07/96 dss	disambuguate some operator cast to ptrs (the new compilers
//					are getting tougher),
//	06/12/96 evc	without the conversion operators we took out
//					for borland 5.0 be carefull not to
//					assume LPOLEOBJECT is the same as &getOleObject().
//					One can say &getOleObject().getUnknown() and cast it to 
//					LPOLEOBJECT
//	---------------------------------------------------------------------------

#include "pre.h"
#include "xferagnt.h"

#include "cvassert.h"

CLIPFORMAT g_cfEmbedSource = RegisterClipboardFormat(CF_EMBEDSOURCE);
CLIPFORMAT g_cfEmbeddedObject = RegisterClipboardFormat(CF_EMBEDDEDOBJECT);
CLIPFORMAT g_cfObjectDescriptor = RegisterClipboardFormat(CF_OBJECTDESCRIPTOR);
CLIPFORMAT g_cfLinkSource = RegisterClipboardFormat(CF_LINKSOURCE);
CLIPFORMAT g_cfLinkSourceDescriptor = RegisterClipboardFormat(CF_LINKSRCDESCRIPTOR);

// List of formats offered by our data transfer object via EnumFormatEtc
static FORMATETC s_arrGetFmtEtcs[] =
{
	{ g_cfEmbeddedObject, NULL, DVASPECT_CONTENT, -1, TYMED_ISTORAGE},
	{ CF_TEXT, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL},
	{ CF_METAFILEPICT, NULL, DVASPECT_CONTENT, -1, TYMED_MFPICT},
	{ g_cfObjectDescriptor, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL},
	{ g_cfLinkSourceDescriptor, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL}
};

//**********************************************************************
//
// XDataObject Interface
//

//**********************************************************************
//
XDataObject::XDataObject(CDataObject *pDataObject)
	: DataObject( pDataObject)
{
}

//**********************************************************************
XDataObject::~XDataObject()
{
}

//**********************************************************************
//
// CRDataObject Implementation (IDataObject Reference Class)
//

//**********************************************************************
//
void CRDataObject::init(void)
{   
	// uncomment when CRUnknown supports aggregation
	// exposeInterface(IID_IDataObject, "CRDataObject");
}

//**********************************************************************
CRDataObject::CRDataObject(LPDATAOBJECT lpIDataObject)
	: CRUnknown(lpIDataObject)
{
	lCount = 1;
}

//**********************************************************************
CRDataObject::~CRDataObject()
{
}

//**********************************************************************
//
// CDataObject Implementation
//

//**********************************************************************
//
void CDataObject::init(void)
{
	lpStorage = NULL;
	lpOleObject = NULL;
	sizel.cx = sizel.cy = 0;
	pointl.x = pointl.y = 0;
	
	exposeInterface(IID_IDataObject, "CDataObject");
}

//**********************************************************************
//
//	lpDataObject - the IDataObject object is a super-class of.
//
CDataObject::CDataObject(LPDATAOBJECT lpDataObject)
	: CUnknown(lpDataObject)
{
	init();
}

//**********************************************************************
//
#pragma warning(disable : 4355)  // turn off this warning.
	// This warning tells us that we are passing this in an initializer,
	// before "this" is through initializing.  This is ok, because
	// we just store the ptr in the other constructors
CDataObject::CDataObject(void)
	: CUnknown(new XDataObject(this))
#pragma warning (default : 4355)  // Turn the warning back on
{
	init();
}

//********************************************************************
//
CDataObject::~CDataObject ()
{
	CDBG(	<< "In CTransferObj's Destructor" << endl);

	if (lpOleObject) {
		// TO DO: when the ole object becomes aggregated, take this line out.
		// aggregate kids are released in the aggregate parent's dtor.
		// NOTE: aggrgate kids remove themselves from the aggregate parent
		// so this is not really mecessary. It might be desirable to leave
		// this in this dtor to demonstrate symmetry with the storage.
		lpOleObject->Release();
		lpOleObject = NULL;

		// Release the storage for this object
		lpStorage->Release();
		lpStorage = NULL;
		}
}

//**********************************************************************
//
// CDataObject::Create
//
// Purpose:	Create a CDataObject
//
// Notes:
// 1) Reference count of will be 0 on return.
//
//********************************************************************

CDataObject FAR * CDataObject::Create(
		VComponentItem FAR *lpCDItem,
		POINTL FAR* pPointl
)
{
	LPSTORAGE		lpStorage;

	CDBG(	<< "In CDataObj::Create" << endl);

	CDataObject FAR * lpDataObj = new CDataObject();

	if (!lpDataObj)
		return NULL;
                     
	// create a sub-storage for the object
	HRESULT hErr = StgCreateDocfile(
				NULL,
				STGM_READWRITE | STGM_DIRECT | STGM_SHARE_EXCLUSIVE |
					STGM_DELETEONRELEASE,
				0,
				&lpStorage);

	cv_assert(hErr == NOERROR);

	if (hErr != NOERROR) {
		delete lpDataObj;
		return NULL;
	}
	
	lpDataObj->lpStorage = new CRStorage(lpStorage);

	// Clone the source object
	if (lpCDItem->hasOleObject()) {
		// Object is loaded; ask the object to save into the new storage
		LPPERSISTSTORAGE pPersistStorage;

		lpCDItem->getOleObject().QueryInterface(IID_IPersistStorage,
				(LPVOID FAR*)&pPersistStorage);
		cv_assert(pPersistStorage);
		OleSave(pPersistStorage, lpStorage, FALSE);

		// pass NULL so that object application won't forget the real stg
		pPersistStorage->SaveCompleted(NULL);
		pPersistStorage->Release();
		
	} else {
		//!!!dss debug
		TestDataObj((LPOLEOBJECT) &lpCDItem->getOleObject().getUnknown(), NULL);

		// Object not loaded so use cheaper IStorage CopyTo operation
		// To DO: how about a VCD copy method.
		lpCDItem->getStorage().getStorage().CopyTo(0, NULL, NULL, lpStorage);
	
		//!!!dss debug
		TestDataObj((LPOLEOBJECT) &lpCDItem->getOleObject().getUnknown(), NULL);
	}
     
    LPOLEOBJECT		lpOleObject;
	OleLoad((LPSTORAGE) &(lpDataObj->getStorage().getUnknown()), IID_IOleObject, NULL,
			(LPVOID FAR*)&lpOleObject);
	cv_assert(lpOleObject);
	
	lpDataObj->lpOleObject = new CROleObject(lpOleObject);
	
	// TO DO: uncomment when CRUnknown class supports aggregation.
	// aggregate( getOleObject());

	lpCDItem->getSize().getPoint(lpDataObj->sizel, VOlePoint::MM100ths);
										 
	if (pPointl)
		lpDataObj->pointl = *pPointl;
	else
		lpDataObj->pointl.x = lpDataObj->pointl.y = 0;

	return lpDataObj;
}

//********************************************************************
//
STDMETHODIMP CDataObject::DAdvise(FORMATETC FAR* pFormatetc, DWORD advf,
		LPADVISESINK pAdvSink, DWORD FAR* pdwConnection)
{
	CDBG( << "In CDataXferObj::DAdvise: unsupported" << endl);
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

//********************************************************************
//
STDMETHODIMP CDataObject::DUnadvise(DWORD dwConnection)
{
	CDBG( << "In CDataXferObj::DUnadvise: unsupported" << endl);
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

//********************************************************************
//
STDMETHODIMP CDataObject::EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise)
{
	CDBG( << "In CDataXferObj::EnumDAdvise: unsupported" << endl);
	return ResultFromScode(OLE_E_ADVISENOTSUPPORTED);
}

//**********************************************************************
//
// CDataObject::QueryGetData
//
// Purpose:
//      Called to determine if our object supports a particular
//      FORMATETC.
//
// Parameters:
//      LPFORMATETC pformatetc  - Pointer to the FORMATETC being queried for.
//
// Return Value:
//      DV_E_FORMATETC    - The FORMATETC is not supported
//      S_OK              - The FORMATETC is supported.
//
//********************************************************************
STDMETHODIMP CDataObject::QueryGetData (LPFORMATETC pformatetc)
{
	CDBG(	<< "In CDataObject::QueryGetData" << endl);

	DebugOutputFormatEtc(pformatetc);

#if 1
	HRESULT	hResult = ResultFromScode( E_FAIL);
	LPDATAOBJECT	pDataObj = 0;

	hResult = getOleObject().QueryInterface( IID_IDataObject,
											  (LPVOID FAR*)&pDataObj);

	CDBG(	<< "    OleObj(" << (LPOLEOBJECT) &(getOleObject().getUnknown())
			<< ")->QI(IDataObj) = " << pDataObj << endl);

	// detect infinite recursion.
	cv_assert(pDataObj != this->getUnknown());

	if (SUCCEEDED(hResult) && pDataObj) {
		// defer to the ole object's data
		hResult = pDataObj->QueryGetData (pformatetc);
		pDataObj->Release();
	}

	return hResult;

#else
	SCODE sc = DV_E_FORMATETC;

	// check the validity of the formatetc.

	if ( (pformatetc->cfFormat == CF_TEXT) &&
		 (pformatetc->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetc->tymed == TYMED_HGLOBAL) )
		sc = S_OK;

	else if ( (pformatetc->cfFormat == g_cfEmbeddedObject) &&
		 (pformatetc->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetc->tymed == TYMED_ISTORAGE) )
		sc = S_OK;

	else if ( (pformatetc->cfFormat == g_cfObjectDescriptor) &&
		 (pformatetc->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetc->tymed == TYMED_HGLOBAL) )
		sc = S_OK;

	else if ( (pformatetc->cfFormat == CF_METAFILEPICT) &&
		 (pformatetc->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetc->tymed == TYMED_MFPICT) )
		sc = S_OK;

	return ResultFromScode(sc);
#endif

}


//**********************************************************************
//
STDMETHODIMP CDataObject::EnumFormatEtc(
		DWORD dwDirection,
		LPENUMFORMATETC FAR* ppenumFormatEtc)
{
	SCODE sc = E_NOTIMPL;

	CDBG(	<< "In CDataObject::EnumFormatEtc" << endl);

	*ppenumFormatEtc = NULL;

	if (dwDirection == DATADIR_GET) {
		*ppenumFormatEtc = OleStdEnumFmtEtc_Create(
				sizeof(s_arrGetFmtEtcs)/sizeof(s_arrGetFmtEtcs[0]),
				s_arrGetFmtEtcs);
		if (*ppenumFormatEtc == NULL)
			sc = E_OUTOFMEMORY;
		else
			sc = S_OK;
	}
	return ResultFromScode(sc);
}

#if defined(CV_DEBUG)

//**********************************************************************
//
void TestDataObj(LPOLEOBJECT lpOleObject, LPDATAOBJECT lpDataObj)
{

#if 1
	FORMATETC	fmtetc;
	DWORD count;
	boolean bQueriedIface = FALSE;

	fmtetc.cfFormat = CF_TEXT;
	fmtetc.ptd = NULL;
	fmtetc.lindex = -1;
	fmtetc.dwAspect = DVASPECT_CONTENT;
	fmtetc.tymed = TYMED_HGLOBAL;

	if (lpOleObject) {
		count = lpOleObject->AddRef();
		count = lpOleObject->Release();
	}

	if (!lpDataObj) {
		cv_assert(lpOleObject);
		lpOleObject->QueryInterface( IID_IDataObject, (LPVOID FAR*) &lpDataObj);
		bQueriedIface = TRUE;
	}
	cv_assert(lpDataObj);

	count = lpDataObj->AddRef();
	count = lpDataObj->Release();

	DebugOutputDataObj(lpDataObj);

	if (bQueriedIface)
		count = lpDataObj->Release();

#endif
}
#endif

//********************************************************************
//
STDMETHODIMP CDataObject::GetCanonicalFormatEtc(LPFORMATETC pformatetc,
			LPFORMATETC pformatetcOut)
{
	CDBG( << "In CDataXferObj::GetCanonicalFormatEtc: unsupported" << endl);
	pformatetcOut->ptd = NULL; 
	return ResultFromScode(E_NOTIMPL);
}

//**********************************************************************
//
// CDataObject::GetData
//
// Purpose:
//      Returns the data in the format specified in pformatetcIn.
//
// Parameters:
//      LPFORMATETC pformatetcIn    -   The format requested by the caller
//      LPSTGMEDIUM pmedium         -   The medium requested by the caller
//
// Returns:
//      DV_E_FORMATETC    - Format not supported
//      S_OK                - Success
//
//********************************************************************

STDMETHODIMP CDataObject::GetData (
		LPFORMATETC pformatetcIn,
		LPSTGMEDIUM pmedium)
{
	CDBG(	<< "In CDataObject::GetData" << endl);

	DebugOutputFormatEtc(pformatetcIn);

#if 1
	LPDATAOBJECT	pDataObj = 0;
	HRESULT	hResult = ResultFromScode( E_FAIL);

	hResult = getOleObject().QueryInterface( IID_IDataObject,
											(LPVOID FAR*)&pDataObj);

	// detect infinite recursion.
	cv_assert(pDataObj != this->getUnknown());

	if (SUCCEEDED(hResult) && pDataObj) {
		// defer to the ole object's data
		hResult = pDataObj->GetData (pformatetcIn, pmedium);
		pDataObj->Release();
	}

	return hResult;

#else
	SCODE sc = DV_E_FORMATETC;

	// we must set all out pointer parameters to NULL. */
	pmedium->tymed = TYMED_NULL;
	pmedium->pUnkForRelease = NULL;    // we transfer ownership to caller
	pmedium->hGlobal = NULL;

	if ( (pformatetcIn->cfFormat == CF_TEXT) &&
		 (pformatetcIn->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetcIn->tymed & TYMED_HGLOBAL) ) {

		LPDATAOBJECT	pDataObj = 0;
		getOleObject().QueryInterface(IID_IDataObject,
									(LPVOID FAR *) &pDataObj);
		cv_assert(pDataObj);

		pmedium->tymed = TYMED_HGLOBAL;
		pmedium->pUnkForRelease = NULL;
		pmedium->hGlobal = NULL;

		HRESULT hrErr = pDataObj->GetData( pformatetcIn, pmedium);

		if (FAILED(hrErr))
			sc = E_FAIL;
		else if (! pmedium->hGlobal)
			sc = E_OUTOFMEMORY;
		else {
			pmedium->tymed = TYMED_HGLOBAL;
			sc = S_OK;
		}

		pDataObj->Release();

	// Check the FORMATETC and fill pmedium if valid.
	} else if ( (pformatetcIn->cfFormat == g_cfEmbeddedObject) &&
		(pformatetcIn->dwAspect == DVASPECT_CONTENT) &&
		(pformatetcIn->tymed & TYMED_ISTORAGE) ) {
		LPPERSISTSTORAGE pPersistStorage;

		/* render CF_EMBEDDEDOBJECT by asking the object to save
		**    into a temporary, DELETEONRELEASE IStorage allocated by us.
		*/
		getOleObject().QueryInterface(IID_IPersistStorage,
									  (LPVOID FAR*)&pPersistStorage);
		cv_assert(pPersistStorage);

		HRESULT hrErr = OleStdGetOleObjectData(
				pPersistStorage,
				pformatetcIn,
				pmedium,
				FALSE   /* fUseMemory -- (use file-base stg) */
		);

		cv_assert(SUCCEEDED(hrErr));

		pPersistStorage->Release();
		sc = GetScode( hrErr );

	} else if ( (pformatetcIn->cfFormat == g_cfObjectDescriptor) &&
		 (pformatetcIn->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetcIn->tymed & TYMED_HGLOBAL) ) {

		 // render CF_OBJECTDESCRIPTOR data
		 pmedium->hGlobal = OleStdGetObjectDescriptorDataFromOleObject(
				 m_lpOleObject,
				 "C++/Views OLE 2.0 Container",    // string to identify source
				 DVASPECT_CONTENT,
				 m_pointl,
				 (LPSIZEL)&m_sizel
			);
		 if (! pmedium->hGlobal)
			 sc = E_OUTOFMEMORY;
		 else {
			 pmedium->tymed = TYMED_HGLOBAL;
			 sc = S_OK;
		 }

	} else if ( (pformatetcIn->cfFormat == CF_METAFILEPICT) &&
		 (pformatetcIn->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetcIn->tymed & TYMED_MFPICT) ) {

		 // render CF_METAFILEPICT by drawing the object into a metafile DC
		 pmedium->hGlobal = OleStdGetMetafilePictFromOleObject(
				 m_lpOleObject, DVASPECT_CONTENT, NULL, pformatetcIn->ptd);
		 if (! pmedium->hGlobal)
			 sc = E_OUTOFMEMORY;
		 else {
			 pmedium->tymed = TYMED_MFPICT;
			 sc = S_OK;
		 }
	}

	return ResultFromScode( sc );

#endif

}

//**********************************************************************
//
// CDataObject::GetDataHere
//
// Purpose:
//      Called to get a data format in a caller supplied location
//
// Parameters:
//      LPFORMATETC pformatetc  - FORMATETC requested
//      LPSTGMEDIUM pmedium     - Medium to return the data
//
// Returns:
//      DATA_E_FORMATETC    - We don't support the requested format
//
//********************************************************************

STDMETHODIMP CDataObject::GetDataHere (
		LPFORMATETC pformatetc,
		LPSTGMEDIUM pmedium)
{
	SCODE sc = DV_E_FORMATETC;

	CDBG(	<< "In CDataObject::GetDataHere" << endl);

	// NOTE: pmedium is an IN parameter. we should NOT set
	//           pmedium->pUnkForRelease to NULL

            
	// Check the FORMATETC and fill pmedium if valid.
	if ( (pformatetc->cfFormat == g_cfEmbeddedObject) &&
		 (pformatetc->dwAspect == DVASPECT_CONTENT) &&
		 (pformatetc->tymed == TYMED_ISTORAGE) ) {
		 LPPERSISTSTORAGE pPersistStorage;

		 /* render CF_EMBEDDEDOBJECT by asking the object to save
		 **    into the IStorage allocated by the caller.
		 */
		 getOleObject().QueryInterface(IID_IPersistStorage,
		 							(LPVOID FAR*)&pPersistStorage);
		 cv_assert(pPersistStorage);
		 HRESULT hrErr = OleStdGetOleObjectData(
				 pPersistStorage, pformatetc, pmedium,0 /*fUseMemory--N/A*/ );
		 pPersistStorage->Release();
		 sc = GetScode( hrErr );
	}
	return ResultFromScode( sc );
}

//********************************************************************
//
STDMETHODIMP CDataObject::SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
			BOOL fRelease)
{
	CDBG( << "In CDataXferObj::SetData: unsupported" << endl);
	return ResultFromScode(E_NOTIMPL);
}

