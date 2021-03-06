//	dndimp.cpp -- [UNDER DEVELOPMENT]
//
//	VDragSiteImp & VDropSiteImp implementations [MS/Windows, OLE, Internal]
//
//	Allegris Foundation 1.1.00 for MS/Windows
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

#if defined(CV_OLE_DRAG_DROP) && defined(CV_WIN32)

// ==========================================================================
// MS/Windows OLE Based Implementation Specific Code
// ==========================================================================

#include "dndimp.h"
#include "dropsite.h"
#include "dragsite.h"
#include "objcour.h"
#include "objtype.h"
#include "ordcllct.h"
#include "iterator.h"
#include "str.h"
#include "bitmap.h"
#include "archiver.h"
#include "mouscrsr.h"
#include "notifier.h"
#include <ole2.h>
#include <shlobj.h>

// --------------------------------------------------------------------------
// Drag-Site Classes
//
CLASS	VDragSite;
CLASS	VDragSiteImp;
CLASS	VObjectCourier;
CLASS	VOleDataObject;
CLASS	VOleDropSource;
CLASS	VOleEnumFormatEtc;

// --------------------------------------------------------------------------
// Drop-Site Classes
//
CLASS	VDropSite;
CLASS	VDropSiteImp;
CLASS	VObjectCourier;
CLASS	VObjectCourierImp;
CLASS	VOleDropTarget;

// ==========================================================================
// VOleEnumFormatEtc
// --------------------------------------------------------------------------
// The VOleEnumFormatEtc class is tightly coupled with the VOleDataObject
// class; this is therefore a drag-site only class.
// ==========================================================================

CLASS VOleEnumFormatEtc : public IEnumFORMATETC {
private:
							VOleEnumFormatEtc(VObjectCourier *);
							VOleEnumFormatEtc(ULONG, LPFORMATETC, ULONG = 0);
						   ~VOleEnumFormatEtc();
private:
	STDMETHODIMP			QueryInterface(REFIID, LPVOID *);
    STDMETHODIMP_(ULONG)	AddRef();
    STDMETHODIMP_(ULONG)	Release();
	STDMETHODIMP			Next(ULONG, LPFORMATETC, ULONG *);
	STDMETHODIMP			Skip(ULONG);
	STDMETHODIMP			Reset();
	STDMETHODIMP			Clone(LPENUMFORMATETC *);
private:
	CLASS VFormatEtc : public VObject {
							VFormatEtc(CLIPFORMAT, DWORD, DWORD);
		FORMATETC			formatetc;
		friend				VOleEnumFormatEtc;
	};
private:
	void					setup();
private:
	ULONG					refCount;
	LPFORMATETC				formatList;
	ULONG					formatCount;
	ULONG					formatIndex;
	VObjectCourier		   *objectCourier;
private:
	friend					VOleDataObject;
};

// --------------------------------------------------------------------------
//
VOleEnumFormatEtc::VOleEnumFormatEtc(VObjectCourier *object_courier)
{
	refCount = 1;
	formatList = 0;
	formatCount = 0;
	formatIndex = 0;
	objectCourier = object_courier;
}

// --------------------------------------------------------------------------
//
VOleEnumFormatEtc::VOleEnumFormatEtc(ULONG format_count,
									 LPFORMATETC format_list,
									 ULONG format_index)
{
	refCount = 1;
	formatList = new FORMATETC [format_count];
	formatCount = format_count;
	formatIndex = format_index;
	objectCourier = 0;

	if (formatList != 0) {
		for (UINT i = 0 ; i < formatCount ; i++) {
			formatList[i] = format_list[i];
		}
	}
}

// --------------------------------------------------------------------------
//
VOleEnumFormatEtc::~VOleEnumFormatEtc()
{
	if (formatList != 0) {
		delete [] formatList;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleEnumFormatEtc::QueryInterface(REFIID riid, LPVOID *ppv)
{
	if (IsEqualIID(riid, IID_IEnumFORMATETC) ||
		IsEqualIID(riid, IID_IUnknown)) {
		*ppv = LPVOID(this);
		LPUNKNOWN(*ppv)->AddRef();
		return NOERROR;
	}
	else {
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleEnumFormatEtc::AddRef()
{
	return ++refCount;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleEnumFormatEtc::Release()
{
	if (--refCount == 0) {
		delete this;
		return 0;
	}
	else {
		return refCount;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleEnumFormatEtc::Next(ULONG format_count,
									 LPFORMATETC format_list,
									 ULONG *format_got_count)
{
	setup();

	if ((formatIndex >= formatCount) ||
		(formatCount == 0) || (formatList == 0)) {
		if (format_got_count != 0) {
			*format_got_count = 0;
		}
		if (format_count == 0) {
			return NOERROR;
		}
		else {
			return ResultFromScode(S_FALSE);
		}
	}

	int n;

	for (n = 0 ; (formatIndex < formatCount) &&
					 (format_count-- > 0) ; n++) {
		*format_list++ = formatList[formatIndex++];
	}

	if (format_got_count != 0) {
		*format_got_count = n;
	}

	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleEnumFormatEtc::Skip(ULONG skip_count)
{
	setup();

	if ((formatIndex += skip_count) >= formatCount) {
		formatIndex = formatCount;
		return ResultFromScode(S_FALSE);
	}
	else {
		return NOERROR;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleEnumFormatEtc::Reset()
{
	formatIndex = 0;
	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleEnumFormatEtc::Clone(LPENUMFORMATETC *enumformatetc)
{
	if (enumformatetc == 0) {
		return ResultFromScode(E_INVALIDARG);
	}

	if ((formatList == 0) && (objectCourier != 0)) {
		*enumformatetc = new VOleEnumFormatEtc(objectCourier);
	}
	else {
		*enumformatetc = new VOleEnumFormatEtc(formatCount,
											   formatList,
											   formatIndex);
	}

	if (*enumformatetc == 0) {
		return ResultFromScode(E_OUTOFMEMORY);
	}
	else {
		return NOERROR;
	}
}

// --------------------------------------------------------------------------
//
void VOleEnumFormatEtc::setup()
{
	if ((formatList != 0) || (objectCourier == 0)) {
		return;
	}

	VOrdCollect *views_types = objectCourier->getTypes();

	if ((views_types == 0) || (views_types->count() <= 0)) {
		return;
	}

	VOrdCollect native_types;
	boolean found_vstring = FALSE, found_vbitmap = FALSE;

	DO (*views_types, VObjectType, views_type)
		VClass *views_class = views_type->getClass();
		if (!found_vstring && views_class->isa(VStringCls)) {
			native_types.add(new VFormatEtc(CF_TEXT,
											DVASPECT_CONTENT,
											TYMED_HGLOBAL));
			found_vstring = TRUE;

		}
		else if (!found_vbitmap && views_class->isa(VBitMapCls)) {
			native_types.add(new VFormatEtc(CF_BITMAP,
											DVASPECT_CONTENT,
											TYMED_GDI));
			found_vbitmap = TRUE;
		}
	END

	formatCount = views_types->count() + native_types.count();
	formatList = new FORMATETC [formatCount];

	int i = 0;
	DO (*views_types, VObjectType, views_type)
		formatList[i].cfFormat = short(views_type->getHandle());
		formatList[i].ptd	   = NULL;
		formatList[i].dwAspect = DVASPECT_CONTENT;
		formatList[i].lindex   = -1;
		formatList[i].tymed	   = TYMED_HGLOBAL;
		i++;
	END

	DO (native_types, VFormatEtc, native_type)
		formatList[i] = native_type->formatetc;
	END
}

// --------------------------------------------------------------------------
//
VOleEnumFormatEtc::VFormatEtc::VFormatEtc(CLIPFORMAT cfformat,
										  DWORD dwaspect,
										  DWORD tymed)
{
	formatetc.cfFormat	= cfformat;
	formatetc.ptd		= NULL;
	formatetc.dwAspect	= dwaspect;
	formatetc.lindex	= -1;
	formatetc.tymed		= tymed;
}

// ==========================================================================
// VOleDataObject
// --------------------------------------------------------------------------
// The VOleDataObject class is our implementation of the OLE IDataObject
// class and is used essentially as a wrapper around our VObjectCourier
// object when it is passed out from a drag-site as an object to drag;
// this is therefore a drag-site only class.
// ==========================================================================

CLASS VOleDataObject : public IDataObject {
private:
							VOleDataObject(VObjectCourier * = 0);
						   ~VOleDataObject();
private:
	STDMETHODIMP			QueryInterface(REFIID, LPVOID *);
    STDMETHODIMP_(ULONG)	AddRef();
    STDMETHODIMP_(ULONG)	Release();
private:
	STDMETHODIMP			GetData(LPFORMATETC, LPSTGMEDIUM);
	STDMETHODIMP			EnumFormatEtc(DWORD, LPENUMFORMATETC *);
private:
	STDMETHODIMP			GetDataHere(LPFORMATETC, LPSTGMEDIUM);
	STDMETHODIMP			QueryGetData(LPFORMATETC);
	STDMETHODIMP			GetCanonicalFormatEtc(LPFORMATETC, LPFORMATETC);
	STDMETHODIMP			SetData(LPFORMATETC, LPSTGMEDIUM, BOOL);
	STDMETHODIMP			DAdvise(LPFORMATETC, DWORD, LPADVISESINK, DWORD *);
	STDMETHODIMP			DUnadvise(DWORD);
	STDMETHODIMP			EnumDAdvise(LPENUMSTATDATA *);
private:
	boolean					getViewsObject(const VClass *, LPSTGMEDIUM);
	boolean					getStringObject(CLIPFORMAT, LPSTGMEDIUM);
	boolean					getBitmapObject(CLIPFORMAT, LPSTGMEDIUM);
private:
	ULONG					refCount;
	VObjectCourier		   *objectCourier;
	LPENUMFORMATETC			enumFormatEtc;
private:
	friend					VDragSiteImp;
};

// --------------------------------------------------------------------------
//
VOleDataObject::VOleDataObject(VObjectCourier *object_courier)
{
	objectCourier = object_courier;
	refCount = 1;
	enumFormatEtc = 0;
}

// --------------------------------------------------------------------------
//
VOleDataObject::~VOleDataObject()
{
	if (enumFormatEtc != 0) {
		enumFormatEtc->Release();
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::QueryInterface(REFIID riid, LPVOID *ppv)
{
	if (IsEqualIID(riid, IID_IDataObject) ||
		IsEqualIID(riid, IID_IUnknown)) {
		*ppv = LPVOID(this);
		LPUNKNOWN(*ppv)->AddRef();
		return NOERROR;
	}
	else {
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDataObject::AddRef()
{
	return ++refCount;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDataObject::Release()
{
	if (--refCount == 0) {
		delete this;
		return 0;
	}
	else {
		return refCount;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::GetData(LPFORMATETC formatetc,
									 LPSTGMEDIUM stgmedium)
{
	if (objectCourier == 0) {
		return ResultFromScode(E_UNEXPECTED);
	}

	VObjectType object_type(formatetc->cfFormat);
	VClass *class_type = object_type.getClass();
	boolean ok;

	if (class_type != 0) {
		ok = getViewsObject(class_type, stgmedium);
	}
	else if (object_type == CF_TEXT) {
		ok = getStringObject(object_type, stgmedium);
	}
	else if ((object_type == CF_BITMAP) ||
			 (object_type == CF_DIB) ||
			 (object_type == CF_METAFILEPICT)) {
		ok = getBitmapObject(object_type, stgmedium);
	}
	else {
		return ResultFromScode(DATA_E_FORMATETC);
	}
	if (ok) {
		return NOERROR;
	}
	else {
		return ResultFromScode(E_UNEXPECTED);
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::EnumFormatEtc(DWORD direction,
										   LPENUMFORMATETC *enumformatetc)
{
	if (objectCourier == 0) {
		return ResultFromScode(E_UNEXPECTED);
	}

	if (direction != DATADIR_GET) {
		return ResultFromScode(E_NOTIMPL);
	}

	if (enumformatetc != 0) {
		if (enumFormatEtc == 0) {
			enumFormatEtc = new VOleEnumFormatEtc(objectCourier);
			enumFormatEtc->AddRef();
		}
		*enumformatetc = enumFormatEtc;
		enumFormatEtc = 0;
	}

	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::SetData(LPFORMATETC, LPSTGMEDIUM, BOOL)
{
	return ResultFromScode(DATA_E_FORMATETC);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::GetDataHere(LPFORMATETC, LPSTGMEDIUM)
{
	return ResultFromScode(E_NOTIMPL);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::QueryGetData(LPFORMATETC formatetc)
{
	if (objectCourier == 0) {
		return ResultFromScode(E_UNEXPECTED);
	}

	VObjectType object_type(formatetc->cfFormat);

	if (objectCourier->contains(object_type)) {
		return NOERROR;
	}
	else if (formatetc->cfFormat == CF_TEXT) {
		if (objectCourier->contains(VStringCls)) {
			return NOERROR;
		}
	}
	else if ((formatetc->cfFormat == CF_BITMAP) ||
			 (formatetc->cfFormat == CF_DIB) ||
			 (formatetc->cfFormat == CF_METAFILEPICT)) {
		if (objectCourier->contains(VBitMapCls)) {
			return NOERROR;
		}
	}

	return ResultFromScode(DATA_E_FORMATETC);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::GetCanonicalFormatEtc(LPFORMATETC, LPFORMATETC)
{
	return ResultFromScode(DATA_S_SAMEFORMATETC);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::DAdvise(LPFORMATETC, DWORD, LPADVISESINK, DWORD *)
{
	return ResultFromScode(E_NOTIMPL);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::DUnadvise(DWORD)
{
	return ResultFromScode(E_NOTIMPL);
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDataObject::EnumDAdvise(LPENUMSTATDATA *)
{
	return ResultFromScode(E_NOTIMPL);
}

// --------------------------------------------------------------------------
//
boolean VOleDataObject::getViewsObject(const VClass *class_type,
									   LPSTGMEDIUM stgmedium)
{
	if (objectCourier == 0) {
		return FALSE;
	}

	if (class_type == 0) {
		class_type = VObjectCls;
	}

	VObject *object = objectCourier->getObject(class_type);

	if (object == 0) {
		return FALSE;
	}

	VArchiver archiver;
	archiver.putObject(object);
	const char *buffer = 0;
	unsigned long sizeof_buffer = 0;
	archiver.getMemoryArchiveBuffer(buffer, sizeof_buffer);
	if ((buffer == 0) || (sizeof_buffer == 0)) {
		return FALSE;
	}

	HGLOBAL hglobal = (char *)GlobalAlloc(GMEM_SHARE, sizeof_buffer);
	if (hglobal == 0) {
		return FALSE;
	}

	char *global_buffer = (char *)GlobalLock(hglobal);

	memcpy(global_buffer, buffer, sizeof_buffer);

	GlobalUnlock(hglobal);

	stgmedium->tymed			= TYMED_HGLOBAL;
	stgmedium->hGlobal			= hglobal;
	stgmedium->pUnkForRelease	= 0;

	return TRUE;
}

// --------------------------------------------------------------------------
// Publish the actual data of the VString object referred to by the
// contained VObjectCourier object, as a native CF_TEXT object, and
// return TRUE if all is well.  If the VObjectCourier does not refer
// to a VString or an error occurs, then simply return FALSE.
//
boolean VOleDataObject::getStringObject(CLIPFORMAT clipformat,
										LPSTGMEDIUM stgmedium)
{
	if (objectCourier == 0) {
		return FALSE;
	}

	if (clipformat == CF_TEXT) {

		VString *string = (VString *)(objectCourier->getObject(VStringCls));

		if (string == 0) {
			return FALSE;
		}

		const char *buffer = string->gets();

		if (buffer == 0) {
			buffer = "";
		}

		int sizeof_buffer = strlen(buffer) + 1;

		HGLOBAL hglobal = (char *)GlobalAlloc(GMEM_SHARE, sizeof_buffer);
		if (hglobal == 0) {
			return FALSE;
		}

		char *global_buffer = (char *)GlobalLock(hglobal);

		memcpy(global_buffer, buffer, sizeof_buffer);

		GlobalUnlock(hglobal);

		stgmedium->tymed			= TYMED_HGLOBAL;
		stgmedium->hGlobal			= hglobal;
		stgmedium->pUnkForRelease	= 0;

		return TRUE;
	}

	return FALSE;
}

// --------------------------------------------------------------------------
//
boolean VOleDataObject::getBitmapObject(CLIPFORMAT clipformat,
										LPSTGMEDIUM stgmedium)
{
	if (objectCourier == 0) {
		return FALSE;
	}

	if (clipformat == CF_BITMAP) {

		VBitMap *bitmap = (VBitMap *)(objectCourier->getObject(VBitMapCls));

		if (bitmap == 0) {
			return FALSE;
		}

		HBITMAP hbitmap = bitmap->getBitmap();

		if (hbitmap == 0) {
			return FALSE;
		}

		int w, h;

		bitmap->sizeOfImage(&w, &h);

		if ((w <= 0) || (h <= 0)) {
			return FALSE;
		}

		HDC dc = GetDC(0);
		HDC srcdc = CreateCompatibleDC(dc);
		HDC dstdc = CreateCompatibleDC(dc);
	    HBITMAP dstbitmap = CreateCompatibleBitmap(dstdc, w, h);
		HGDIOBJ oldsrc = SelectObject(srcdc, hbitmap);
		HGDIOBJ olddst = SelectObject(dstdc, dstbitmap);
		BitBlt(dstdc, 0, 0, w, h, srcdc, 0, 0, SRCCOPY);
		SelectObject(srcdc, oldsrc);
		SelectObject(dstdc, olddst);
	    DeleteDC(srcdc);
	    DeleteDC(dstdc);
	    ReleaseDC(0, dc);

		stgmedium->hGlobal			= HGLOBAL(dstbitmap);
		stgmedium->tymed			= TYMED_GDI;
		stgmedium->pUnkForRelease	= 0;

		return TRUE;
	}

	else if (clipformat == CF_DIB) {

		// ToDo!!!
	}

	else if (clipformat == CF_METAFILEPICT) {

		// ToDo!!!
	}

	return FALSE;
}

// ==========================================================================
// VObjectCourierImp
// --------------------------------------------------------------------------
// The VObjectCourierImp class is used to as a wrapper around an OLE
// IDataObject object when a drag-enter or drop occurs on a drop-site;
// this is therefore a drop-site only class.
// ==========================================================================

CLASS VObjectCourierImp : public VObjectCourier {
public:
							VObjectCourierImp(LPDATAOBJECT);
						   ~VObjectCourierImp();

public:
	virtual void			setObject(const VObject *);
	virtual VObject		   *getObject(const VClass * = 0) const;
	virtual boolean			contains(const VClass *) const;
	virtual boolean			contains(const VObjectType&) const;

public:
	LPDATAOBJECT			getIDataObject() const;

protected:
	boolean					contains(CLIPFORMAT) const;
	VObject				   *getViewsObject() const;
	VObject				   *getStringObject() const;
	VObject				   *getBitmapObject() const;

private:
	LPDATAOBJECT			iDataObject;
};

// --------------------------------------------------------------------------
//
VObjectCourierImp::VObjectCourierImp(LPDATAOBJECT idata_object)
{
	if ((iDataObject = idata_object) != 0) {
		iDataObject->AddRef();
	}
}

// --------------------------------------------------------------------------
//
VObjectCourierImp::~VObjectCourierImp()
{
	if (iDataObject != 0) {
		iDataObject->Release();
	}
}

// --------------------------------------------------------------------------
//
void VObjectCourierImp::setObject(const VObject *)
{
}

// --------------------------------------------------------------------------
// Creates and returns a pointer to a VObject object from the contained
// IDataObject if it supports the format specified by the given VClass.
// Returns null if the format is not supported (or if error).
//
VObject *VObjectCourierImp::getObject(const VClass *class_type) const
{
	if (class_type == 0) {
		class_type = VObjectCls;
	}
	if (contains(VObjectType(class_type))) {
		return getViewsObject();
	}
	else if (((VClass *)class_type)->isa(VStringCls)) {
		return getStringObject();
	}
	else if (((VClass *)class_type)->isa(VBitMapCls)) {
		return getBitmapObject();
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
boolean VObjectCourierImp::contains(const VClass *class_type) const
{
	if (class_type == 0) {
		return FALSE;
	}
	else if (contains(VObjectType(class_type))) {
		return TRUE;
	}
	else if (((VClass *)class_type)->isa(VStringCls)) {
		return contains(CF_TEXT) || contains(CF_HDROP);
	}
	else if (((VClass *)class_type)->isa(VBitMapCls)) {
		return contains(CF_DIB) ||
			   contains(CF_BITMAP) ||
			   contains(CF_METAFILEPICT);
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
boolean VObjectCourierImp::contains(const VObjectType& object_type) const
{
	return contains(CLIPFORMAT(object_type.getHandle()));
}

// --------------------------------------------------------------------------
//
boolean VObjectCourierImp::contains(CLIPFORMAT clipformat) const
{
	if (iDataObject == 0) {
		return FALSE;
	}

	LPENUMFORMATETC enumformatetc;

	iDataObject->EnumFormatEtc(DATADIR_GET, &enumformatetc);

	if (enumformatetc == 0) {
		return FALSE;
	}

	FORMATETC formatetc;
	LPFORMATETC formatetcptr = &formatetc;

	enumformatetc->Reset();

	while (enumformatetc->Next(1, formatetcptr, NULL) == S_OK) {
		if (formatetcptr->cfFormat == clipformat) {
			enumformatetc->Release();
			return TRUE;
		}
	}

	enumformatetc->Release();
	return FALSE;
}

// --------------------------------------------------------------------------
// Creates and returns a pointer to a VObject object from the contained
// IDataObject if it supports the VObjectCls format.  Returns null if
// the format is not supported (or if error).
//
VObject *VObjectCourierImp::getViewsObject() const
{
	if (iDataObject == 0) {
		return 0;
	}

	FORMATETC formatetc;
	STGMEDIUM stgmedium;
	boolean acceptable;
	VObject *object;
	VObjectType object_type(VObjectCls);

	formatetc.cfFormat	= short(object_type.getHandle());
	formatetc.ptd		= NULL;
	formatetc.dwAspect	= DVASPECT_CONTENT;
	formatetc.lindex	= -1;
	formatetc.tymed		= TYMED_HGLOBAL;

	iDataObject->GetData(&formatetc, &stgmedium);

	if (formatetc.dwAspect != DVASPECT_CONTENT) {
		return 0;
	}

	acceptable = ((stgmedium.tymed & TYMED_HGLOBAL) == TYMED_HGLOBAL) &&
				  (stgmedium.hGlobal != 0);

	if (!acceptable) {
		return 0;
	}

	char *global_buffer = (char *)GlobalLock(stgmedium.hGlobal);
	VArchiver archiver;
	archiver.write(global_buffer, GlobalSize(stgmedium.hGlobal));
	archiver.rewind();
	object = archiver.getObject(0);

	GlobalUnlock(stgmedium.hGlobal);
	if (stgmedium.pUnkForRelease == 0) {
		GlobalFree(stgmedium.hGlobal);
	}

	return object;
}

// --------------------------------------------------------------------------
// Creates and returns a pointer to a VString object from the contained
// IDataObject if it supports the CF_TEXT format.  Returns null if the
// format is not supported (or if error).
//
VObject *VObjectCourierImp::getStringObject() const
{
	if (iDataObject == 0) {
		return 0;
	}

	FORMATETC formatetc;
	STGMEDIUM stgmedium;
	VObject *object;

	if (contains(CF_TEXT)) {

		formatetc.cfFormat	= CF_TEXT;
		formatetc.ptd		= NULL;
		formatetc.dwAspect	= DVASPECT_CONTENT;
		formatetc.lindex	= -1;
		formatetc.tymed		= TYMED_HGLOBAL;

		if ((iDataObject->GetData(&formatetc, &stgmedium) != S_OK) ||
			(formatetc.dwAspect != DVASPECT_CONTENT) ||
			((stgmedium.tymed & TYMED_HGLOBAL) != TYMED_HGLOBAL) ||
			(stgmedium.hGlobal == 0)) {
			return 0;
		}

		object = new VString((char *)GlobalLock(stgmedium.hGlobal));

		GlobalUnlock(stgmedium.hGlobal);
		if (stgmedium.pUnkForRelease == 0) {
			GlobalFree(stgmedium.hGlobal);
		}

		return object;
	}

	else if (contains(CF_HDROP)) {

		formatetc.cfFormat	= CF_HDROP;
		formatetc.ptd		= NULL;
		formatetc.dwAspect	= DVASPECT_CONTENT;
		formatetc.lindex	= -1;
		formatetc.tymed		= TYMED_HGLOBAL;

		if ((iDataObject->GetData(&formatetc, &stgmedium) != S_OK) ||
			((stgmedium.tymed & TYMED_HGLOBAL) != TYMED_HGLOBAL) ||
			 (stgmedium.hGlobal == 0)) {
			return 0;
		}

		char filename[2048];

		DragQueryFile(HDROP(stgmedium.hGlobal), 0, filename, 2047);

		object = new VString(filename);

		if (stgmedium.pUnkForRelease == 0) {
			GlobalFree(stgmedium.hGlobal);
		}
		else {
			//
			// ??? From MSDN CD ("Drop Target Handlers").
			//     Should all of them be like this?
			//
			stgmedium.pUnkForRelease->Release();
		}

		return object;
	}

	return 0;
}

// --------------------------------------------------------------------------
// Creates and returns a pointer to a VBitMap object from the contained
// IDataObject if it supports the CF_BITMAP, CF_DIB, or CF_METAFILEPICT
// formats.  Returns null if the formats are not supported (or if error).
//
VObject *VObjectCourierImp::getBitmapObject() const
{
	if (iDataObject == 0) {
		return 0;
	}

	FORMATETC formatetc;
	STGMEDIUM stgmedium;
	boolean acceptable;
	VObject *object;

	if (contains(CF_DIB)) {

		// This seems to work!!!

		formatetc.cfFormat	= CF_DIB;
		formatetc.ptd		= NULL;
		formatetc.dwAspect	= DVASPECT_CONTENT;
		formatetc.lindex	= -1;
		formatetc.tymed		= TYMED_HGLOBAL;

		if (iDataObject->GetData(&formatetc, &stgmedium) != S_OK) {
			return 0;
		}

		if (formatetc.dwAspect != DVASPECT_CONTENT) {
			return 0;
		}

		acceptable = ((stgmedium.tymed & TYMED_HGLOBAL) == TYMED_HGLOBAL) &&
					  (stgmedium.hGlobal != 0);

		if (!acceptable) {
			return 0;
		}

		object = new VBitMap(LPBITMAPINFO(GlobalLock(stgmedium.hGlobal)));

		GlobalUnlock(stgmedium.hGlobal);
		if (stgmedium.pUnkForRelease == 0) {
			GlobalFree(stgmedium.hGlobal);
		}

		return object;
	}

	else if (contains(CF_BITMAP)) {

		// ToDo!!!

		formatetc.cfFormat	= CF_BITMAP;
		formatetc.ptd		= NULL;
		formatetc.dwAspect	= DVASPECT_CONTENT;
		formatetc.lindex	= -1;
		formatetc.tymed		= TYMED_GDI;

		if (iDataObject->GetData(&formatetc, &stgmedium) != S_OK) {
			return 0;
		}

		if (formatetc.dwAspect != DVASPECT_CONTENT) {
			return 0;
		}

		acceptable = ((stgmedium.tymed & TYMED_HGLOBAL) == TYMED_HGLOBAL) &&
					  (stgmedium.hGlobal != 0);

		if (!acceptable) {
			return 0;
		}

		HBITMAP hbitmap = HBITMAP(stgmedium.hGlobal);
		HDC srcdc = CreateCompatibleDC(0);
		HDC dstdc = CreateCompatibleDC(srcdc);
		BITMAP bitmap;
		GetObject(hbitmap, sizeof(BITMAP), &bitmap);
		HBITMAP oldsrc = HBITMAP(SelectObject(srcdc, hbitmap));
		HBITMAP newbitmap = CreateCompatibleBitmap(srcdc,
												   bitmap.bmWidth,
												   bitmap.bmHeight);
		HBITMAP olddst = HBITMAP(SelectObject(dstdc, newbitmap));
		BitBlt(dstdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight,
			   srcdc, 0, 0, SRCCOPY);
		SelectObject(dstdc, olddst);
		SelectObject(srcdc, oldsrc);
		DeleteDC(dstdc);
		DeleteDC(srcdc);
		object = new VBitMap;

		// ((VBitMap *)object)->hBitmap = newbitmap;

		return 0;
	}

	else if (contains(CF_METAFILEPICT)) {

		// ToDo!!!

		return 0;

	}

	return 0;
}

// --------------------------------------------------------------------------
//
LPDATAOBJECT VObjectCourierImp::getIDataObject() const
{
	return iDataObject;
}

// ==========================================================================
// VOleDropSource
// ==========================================================================

CLASS VOleDropSource : public IDropSource {
private:
							VOleDropSource(VDragSiteImp *);
						   ~VOleDropSource();
private:
	STDMETHODIMP			QueryInterface(REFIID, LPVOID *);
    STDMETHODIMP_(ULONG)	AddRef();
    STDMETHODIMP_(ULONG)	Release();
	STDMETHODIMP			QueryContinueDrag(BOOL, DWORD);
	STDMETHODIMP			GiveFeedback(DWORD);
public:
	static long				getOperationFromKeyState(DWORD);
	static long				getOperationFromKeyState(DWORD, DWORD);
	static DWORD			getDropEffectFromOperation(long);
private:
	ULONG					refCount;
	VDragSiteImp		   *dragSite;
private:
	friend					VDragSiteImp;
};

// --------------------------------------------------------------------------
//
VOleDropSource::VOleDropSource(VDragSiteImp *dragsite)
{
	refCount = 1;
	dragSite = dragsite;
}

// --------------------------------------------------------------------------
//
VOleDropSource::~VOleDropSource()
{
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDropSource::AddRef()
{
	return ++refCount;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDropSource::Release()
{
	if (--refCount == 0) {
		delete this;
		return 0;
	}
	else {
		return refCount;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropSource::QueryInterface(REFIID riid, LPVOID *ppv)
{
	if (IsEqualIID(riid, IID_IDropSource) ||
		IsEqualIID(riid, IID_IUnknown)) {
		*ppv = LPVOID(this);
		LPUNKNOWN(*ppv)->AddRef();
		return NOERROR;
	}
	else {
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropSource::QueryContinueDrag(BOOL escape, DWORD keystate)
{
	if (escape) {
		return ResultFromScode(DRAGDROP_S_CANCEL);
	}
	else if (!(keystate & MK_LBUTTON)) {
		return ResultFromScode(DRAGDROP_S_DROP);
	}
	else {
		return NOERROR;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropSource::GiveFeedback(DWORD keystate)
{
	if (!dragSite->dragFeedback(getOperationFromKeyState(keystate))) {
		return ResultFromScode(DRAGDROP_S_USEDEFAULTCURSORS);
	}
	else {
		return NOERROR;
	}
}

// --------------------------------------------------------------------------
//
long VOleDropSource::getOperationFromKeyState(DWORD keystate)
{
	if (keystate & MK_CONTROL) {
		if (keystate & MK_SHIFT) {
			return VDropSite::LinkOp;
		}
		else {
			return VDropSite::CopyOp;
		}
	}
	else {
		return VDropSite::MoveOp;
	}
}

// --------------------------------------------------------------------------
//
long VOleDropSource::getOperationFromKeyState(DWORD keystate, DWORD effect)
{
	if (keystate & MK_CONTROL) {
		if (keystate & MK_SHIFT) {
			if (effect & DROPEFFECT_LINK) {
				return VDropSite::LinkOp;
			}
			else if (effect & DROPEFFECT_COPY) {
				return VDropSite::CopyOp;
			}
			else if (effect & DROPEFFECT_MOVE) {
				return VDropSite::MoveOp;
			}
			else {
				return VDropSite::NoOp;
			}
		}
		else {
			if (effect & DROPEFFECT_COPY) {
				return VDropSite::CopyOp;
			}
			else if (effect & DROPEFFECT_LINK) {
				return VDropSite::LinkOp;
			}
			else if (effect & DROPEFFECT_MOVE) {
				return VDropSite::MoveOp;
			}
			else {
				return VDropSite::NoOp;
			}
		}
	}
	else if (effect & DROPEFFECT_COPY) {
		return VDropSite::CopyOp;
	}
	else if (effect & DROPEFFECT_LINK) {
		return VDropSite::LinkOp;
	}
	else if (effect & DROPEFFECT_MOVE) {
		return VDropSite::MoveOp;
	}
	else {
		return VDropSite::NoOp;
	}
}

// --------------------------------------------------------------------------
//
DWORD VOleDropSource::getDropEffectFromOperation(long operation)
{
	if (operation & VDropSite::NoOp) {
		return DROPEFFECT_NONE;
	}
	DWORD effect = 0;
	if (operation & VDropSite::MoveOp) {
		effect |= DROPEFFECT_MOVE;
	}
	if (operation & VDropSite::CopyOp) {
		effect |= DROPEFFECT_COPY;
	}
	if (operation & VDropSite::LinkOp) {
		effect |= DROPEFFECT_LINK;
	}
	return effect;
}

// ==========================================================================
// VDragSiteImp
// ==========================================================================

VDragSiteImp::VDragSiteImp(VDragSite *dragsite)
{
	dragSite = dragsite;
	oleDropSource = 0;
}

// --------------------------------------------------------------------------
//
boolean VDragSiteImp::registerDragSite()
{
	if ((dragSite == 0) || (dragSite->theWindow == 0)) {
		return FALSE;
	}
	HWND hwnd = dragSite->theWindow->hwnd();
	if (hwnd == 0) {
		return FALSE;
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDragSiteImp::unregisterDragSite()
{
	if ((dragSite == 0) || (dragSite->theWindow == 0)) {
		return;
	}
	HWND hwnd = dragSite->theWindow->hwnd();
	if (hwnd == 0) {
		return;
	}
}

// --------------------------------------------------------------------------
//
boolean VDragSiteImp::dragLoop(VObjectCourier *object_courier)
{
	// boolean control_key_down = notifier->keyStatus(K_Mod_Control);
	// boolean shift_key_down = notifier->keyStatus(K_Mod_Shift);

	if ((dragSite == 0) || (object_courier == 0)) {
		return FALSE;
	}

	long allowable_op = object_courier->getOperation();
	DWORD allowable_effect = 0, effect;
	HRESULT hresult;
	SCODE result;
	boolean dropped;

	if (allowable_op & VObjectCourier::MoveOp) {
		allowable_effect |= DROPEFFECT_MOVE;
	}
	if (allowable_op & VObjectCourier::CopyOp) {
		allowable_effect |= DROPEFFECT_COPY;
	}
	if (allowable_op & VObjectCourier::LinkOp) {
		allowable_effect |= DROPEFFECT_LINK;
	}

	oleDataObject = new VOleDataObject(object_courier);
	oleDropSource = new VOleDropSource(this);

	hresult = DoDragDrop(oleDataObject,
						oleDropSource,
						allowable_effect, &effect);

	result = GetScode(hresult);

	if (result == S_OK) {
		dropped = TRUE;
	}
	else if (result == DRAGDROP_S_DROP) {
		dropped = TRUE;
	}
	else if (DRAGDROP_S_CANCEL) {
		dropped = FALSE;
	}
	else {
		dropped = FALSE;
	}

	oleDataObject->Release(); oleDataObject = 0;
	oleDropSource->Release(); oleDropSource = 0;

	return dropped;
}

// --------------------------------------------------------------------------
//
boolean VDragSiteImp::dragFeedback(long op)
{
	return FALSE;
}


// ==========================================================================
// VOleDropTarget
// ==========================================================================

CLASS VOleDropTarget : public IDropTarget {
private:
							VOleDropTarget(VDropSiteImp *);
						   ~VOleDropTarget();
private:
	STDMETHODIMP			QueryInterface(REFIID, LPVOID *);
    STDMETHODIMP_(ULONG)	AddRef();
    STDMETHODIMP_(ULONG)	Release();
	STDMETHODIMP			DragEnter(LPDATAOBJECT, DWORD, POINTL, DWORD *);
	STDMETHODIMP			DragOver(DWORD, POINTL, DWORD *);
	STDMETHODIMP			DragLeave();
	STDMETHODIMP			Drop(LPDATAOBJECT, DWORD, POINTL, DWORD *);
private:
	void					setObject(LPDATAOBJECT);
	void					setOperation(long);
	long					getOperation() const;
	static long				getOperationFromKeyState(DWORD);
	static long				getOperationFromKeyState(DWORD, DWORD);
	static DWORD			getDropEffectFromOperation(long);
	void					lock();
	void					unlock();
private:
	ULONG					refCount;
	VDropSiteImp		   *dropSite;
	VObjectCourierImp	   *objectCourier;
private:
	friend					VDropSiteImp;
};

// --------------------------------------------------------------------------
//
VOleDropTarget::VOleDropTarget(VDropSiteImp *dropsite)
{
	refCount = 1;
	dropSite = dropsite;
	objectCourier = 0;
}

// --------------------------------------------------------------------------
//
VOleDropTarget::~VOleDropTarget()
{
	if (objectCourier != 0) {
		delete objectCourier;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDropTarget::AddRef()
{
	return ++refCount;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP_(ULONG) VOleDropTarget::Release()
{
	if (--refCount == 0) {
		delete this;
		return 0;
	}
	else {
		return refCount;
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropTarget::QueryInterface(REFIID riid, LPVOID *ppv)
{
	if (IsEqualIID(riid, IID_IDropTarget) ||
		IsEqualIID(riid, IID_IUnknown)) {
		*ppv = LPVOID(this);
		LPUNKNOWN(*ppv)->AddRef();
		return NOERROR;
	}
	else {
		*ppv = NULL;
		return ResultFromScode(E_NOINTERFACE);
	}
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropTarget::DragEnter(LPDATAOBJECT dataobject,
									   DWORD keystate, POINTL point,
									   DWORD *effect)
{
	if (dataobject == 0) {
		*effect = DROPEFFECT_NONE;
		return NOERROR;
	}

	setObject(dataobject);
	setOperation(getOperationFromKeyState(keystate, *effect));
	long op = dropSite->dragObjectEnter();
	if (dropSite->isDragTracking()) {
		ScreenToClient(dropSite->getWindowHandle(), LPPOINT(&point));
		op = dropSite->dragObjectOver(point.x, point.y);
	}
	setOperation(op &= getOperation());
	*effect = getDropEffectFromOperation(op);

	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropTarget::DragLeave()
{
	if (objectCourier == 0) {
		return NOERROR;
	}
	setObject(0);
	dropSite->dragObjectExit();
	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropTarget::DragOver(DWORD keystate, POINTL point,
									  DWORD *effect)
{
	if (objectCourier == 0) {
		*effect = DROPEFFECT_NONE;
		return NOERROR;
	}

	setOperation(getOperationFromKeyState(keystate, *effect));
	ScreenToClient(dropSite->getWindowHandle(), LPPOINT(&point));
	long op = dropSite->dragObjectOver(point.x, point.y);
	setOperation(op &= getOperation());
	*effect = getDropEffectFromOperation(op);

	return NOERROR;
}

// --------------------------------------------------------------------------
//
STDMETHODIMP VOleDropTarget::Drop(LPDATAOBJECT dataobject,
								  DWORD keystate, POINTL point,
								  DWORD *effect)
{
	if (objectCourier == 0) {
		*effect = DROPEFFECT_NONE;
		return NOERROR;
	}

	if ((dataobject == 0) || (objectCourier == 0)) {
		return ResultFromScode(E_FAIL);
	}

	setObject(dataobject);
	setOperation(getOperationFromKeyState(keystate, *effect));
	ScreenToClient(dropSite->getWindowHandle(), LPPOINT(&point));
	long op = dropSite->objectDropped(point.x, point.y);
	setOperation(op &= getOperation());
	*effect = getDropEffectFromOperation(op);
	setObject(0);

	return NOERROR;
}

// --------------------------------------------------------------------------
//
void VOleDropTarget::setObject(LPDATAOBJECT idata_object)
{
	if (idata_object == 0) {
		if (objectCourier != 0) {
			dropSite->setObjectCourier(0);
			delete objectCourier;
			objectCourier = 0;
		}
		return;
	}

	if ((objectCourier != 0) &&
		(objectCourier->getIDataObject() != idata_object)) {
		delete objectCourier;
	}

	objectCourier = new VObjectCourierImp(idata_object);
	dropSite->setObjectCourier(objectCourier);
}

// --------------------------------------------------------------------------
//
long VOleDropTarget::getOperationFromKeyState(DWORD keystate)
{
	return VOleDropSource::getOperationFromKeyState(keystate);
}

// --------------------------------------------------------------------------
//
long VOleDropTarget::getOperationFromKeyState(DWORD keystate, DWORD effect)
{
	return VOleDropSource::getOperationFromKeyState(keystate, effect);
}

// --------------------------------------------------------------------------
//
DWORD VOleDropTarget::getDropEffectFromOperation(long operation)
{
	return VOleDropSource::getDropEffectFromOperation(operation);
}

// --------------------------------------------------------------------------
//
void VOleDropTarget::setOperation(long op)
{
	dropSite->getObjectCourier()->setOperation(op);
}

// --------------------------------------------------------------------------
//
long VOleDropTarget::getOperation() const
{
	return dropSite->getObjectCourier()->getOperation();
}

// --------------------------------------------------------------------------
//
void VOleDropTarget::lock()
{
	CoLockObjectExternal(LPUNKNOWN(this), TRUE, FALSE);
}

// --------------------------------------------------------------------------
//
void VOleDropTarget::unlock()
{
	CoLockObjectExternal(LPUNKNOWN(this), FALSE, TRUE);
}

// ==========================================================================
// VDropSiteImp
// ==========================================================================

VDropSiteImp::VDropSiteImp(VDropSite *dropsite)
{
	dropSite = dropsite;
	oleDropTarget = 0;
}

// --------------------------------------------------------------------------
//
VDropSiteImp::~VDropSiteImp()
{
	if (oleDropTarget != 0) {
		oleDropTarget->Release();
	}
}

// --------------------------------------------------------------------------
//
boolean VDropSiteImp::registerDropSite()
{
	if ((dropSite == 0) || (dropSite->theWindow == 0)) {
		return FALSE;
	}
	if (oleDropTarget != 0) {
		unregisterDropSite();
	}
	HWND hwnd = dropSite->theWindow->hwnd();
	if (hwnd == 0) {
		return FALSE;
	}
	if (oleDropTarget != 0) {
		delete oleDropTarget;
	}
	oleDropTarget = new VOleDropTarget(this);
	oleDropTarget->lock();
	if (RegisterDragDrop(hwnd, LPDROPTARGET(oleDropTarget)) != S_OK) {
		oleDropTarget->Release();
		oleDropTarget = 0;
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VDropSiteImp::unregisterDropSite()
{
	if ((dropSite == 0) || (dropSite->theWindow == 0)) {
		return;
	}
	HWND hwnd = dropSite->theWindow->hwnd();
	if (hwnd == 0) {
		return;
	}
	RevokeDragDrop(hwnd);
	if (oleDropTarget != 0) {
		oleDropTarget->unlock();
		oleDropTarget->Release();
		oleDropTarget = 0;
	}
}

#endif // CV_OLE_DRAG_DROP && CV_WIN32
