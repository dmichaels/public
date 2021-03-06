// $Header:   Q:/views/win/vcs/XFERAGNT.H_v   1.6   Mar 04 1997 14:14:36   gregm  $

//	xferagnt.h
//
//	Tansfer Agent
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

#ifndef XFERAGNT_H
#define XFERAGNT_H

// BASIC Views OLE types
#include "oledefs.h"

// unknown includes
#include "unknown.h"

// Forward Declarations
CLASS CDataObject;
typedef	CDataObject FAR *LPCDATAOBJECT;

CLASS CDataObject : public CUnknown
{   
public:

	// Outer (Controlling) Data Object
	CDataObject(void);
	
	// Outer (Controlling) Data Object inheriting an IDataObject
	CDataObject(LPDATAOBJECT lpDataObject);
	
	// dtor
	virtual ~CDataObject();

	LPTSTR		getClassName() {return _T("CDataObject");}
	
	operator LPDATAOBJECT() {return (LPDATAOBJECT) (LPUNKNOWN) getUnknown();}
	
	// *** IDataObject methods ***
	virtual STDMETHODIMP DAdvise  ( FORMATETC FAR* pFormatetc, DWORD advf,
								LPADVISESINK pAdvSink, DWORD FAR* pdwConnection);
	virtual STDMETHODIMP DUnadvise  ( DWORD dwConnection);
	virtual STDMETHODIMP EnumDAdvise  ( LPENUMSTATDATA FAR* ppenumAdvise);
	virtual STDMETHODIMP EnumFormatEtc  ( DWORD dwDirection,
											LPENUMFORMATETC FAR* ppenumFormatEtc);
	virtual STDMETHODIMP GetCanonicalFormatEtc  ( LPFORMATETC pformatetc,
												LPFORMATETC pformatetcOut);
	virtual STDMETHODIMP GetData  (LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium );
	virtual STDMETHODIMP GetDataHere  (LPFORMATETC pformatetc, LPSTGMEDIUM pmedium);
	virtual STDMETHODIMP QueryGetData  (LPFORMATETC pformatetc );
	virtual STDMETHODIMP SetData  (LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium,
								BOOL fRelease);

	static CDataObject FAR* Create(VComponentItem FAR *lpCDItem,
			POINTL FAR* pPointl);
			
	boolean			hasOleObject()	{return lpOleObject != 0;}
	CROleObject&	getOleObject()	{return *lpOleObject;}
	boolean			hasStorage()	{return lpStorage != 0;}
	CRStorage&		getStorage()	{return *lpStorage;}

protected:
	LPCRSTORAGE		lpStorage;
	LPCROLEOBJECT	lpOleObject;
	
private:
	SIZEL			sizel;
	POINTL			pointl;
	
	void			init(void);
};

#define STD_IDATAOBJECT_METHODS() \
	virtual STDMETHODIMP DAdvise(FORMATETC FAR* pFormatetc, DWORD advf, \
							LPADVISESINK pAdvSink, DWORD FAR* pdwConnection) \
				{ return getDataObject().DAdvise(pFormatetc, advf, \
												pAdvSink, pdwConnection);} \
	virtual STDMETHODIMP DUnadvise(DWORD dwConnection) \
				{ return getDataObject().DUnadvise(dwConnection);} \
	virtual STDMETHODIMP EnumDAdvise(LPENUMSTATDATA FAR* ppenumAdvise) \
				{ return getDataObject().EnumDAdvise(ppenumAdvise);} \
	virtual STDMETHODIMP EnumFormatEtc(DWORD dwDirection, \
									LPENUMFORMATETC FAR* ppenumFormatEtc) \
				{ return getDataObject().EnumFormatEtc(dwDirection, \
												ppenumFormatEtc);} \
	virtual STDMETHODIMP GetCanonicalFormatEtc(LPFORMATETC pformatetc, \
											LPFORMATETC pformatetcOut) \
				{ return getDataObject().GetCanonicalFormatEtc(pformatetc, \
																pformatetcOut);} \
	virtual STDMETHODIMP GetData(LPFORMATETC pformatetcIn, LPSTGMEDIUM pmedium) \
				{ return getDataObject().GetData(pformatetcIn, pmedium);} \
	virtual STDMETHODIMP GetDataHere(LPFORMATETC pformatetc, LPSTGMEDIUM pmedium) \
				{ return getDataObject().GetDataHere(pformatetc, pmedium);} \
	virtual STDMETHODIMP QueryGetData(LPFORMATETC pformatetc ) \
				{ return getDataObject().QueryGetData(pformatetc);} \
	virtual STDMETHODIMP SetData(LPFORMATETC pformatetc, STGMEDIUM FAR * pmedium, \
								BOOL fRelease) \
				{ return getDataObject().SetData(pformatetc, pmedium, fRelease);}

#define	STD_DataObject_METHODS() STD_IDATAOBJECT_METHODS()

#define STD_IDATAOBJECT_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, DataObject, *DataObject)
		
#define INHERIT_IDATAOBJECT_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*DataObject) \
	STD_IDATAOBJECT_METHODS() \
	STD_IDATAOBJECT_ACCESS(*DataObject) \
protected: \
	CDataObject		*DataObject;

#define CONTAIN_IDATAOBJECT_INTERFACE() \
public: \
	STD_IDATAOBJECT_METHODS() \
	STD_IDATAOBJECT_ACCESS(*DataObject) \
protected: \
	CDataObject		*DataObject;

//**********************************************************************
//
// IDataObject Reference Class
//
// This class is
//		- a sub-object of an IDataObject object
//
// This is a convenience class that refers to an IDataObject Interface Pointer 
// (IP) that is given to you by another process. It adds minimal helpful
// functionality not found in OLE (inheritance, among others) to use an
// Interface Pointer.
//
// This class is NOT an IDataObject implementation, and hence cannot be used
// to override IDataObject methods. You must reference the IP contain within 
// to access IDataObject methods.
//
CLASS CRDataObject : public CRUnknown
{
public:

	CRDataObject(LPDATAOBJECT lpIDataObject);
	virtual ~CRDataObject();
	
	CRUNKNOWN_INTERFACE(DataObject)
	
public:	
	LPTSTR		getClassName()		{return _T("CRDataObject");}
						  
private:
	void			init(void);
};

//**********************************************************************
//
//  IDataObject Class
//
CLASS XDataObject : public IDataObject
{
public:

	XDataObject(CDataObject *);
	~XDataObject();

	INHERIT_INTERFACE(IDATAOBJECT, DataObject)
};

#endif  // XFERAGNT_H
