// $Header:   Q:/views/win/vcs/UNKNOWN.CPv   1.10   Mar 04 1997 14:14:10   gregm  $

//	CUnknown
//
//  Views IUnknown Implementation
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
//	12/09/94 dss	Initial release.
//	12/04/95 dss	Added PVCS header. Migrated to LPCOLESTR.
//	12/12/95 dss	Added realize.
//	02/27/96 dss	CUnknown::init(), CUnknown::~CUnknown():
//					OleInit and OleUninit.
//
//	---------------------------------------------------------------------------

#include "pre.h"

// Views Object class must come first
#include "object.h"

// Views Classes
#include "iterator.h"
#include "dictinry.h"
#include "intassoc.h"
#include "ordcllct.h"

// Views Component Object Classes
#include "unknown.h"

defineClass(CUnknown, VObject)

//**********************************************************************
//
// COM Interface Class - Encapsulates IID's
//
extern VClass *ComInterfaceCls;

CLASS ComInterface : public VObject
{
public:

	ComInterface()
	{
		this->iid = IID_NULL;
		this->lpClassName = 0;
		this->lpCUnknown = 0;
	}

	ComInterface(REFIID refIID, CUnknown *lpCUnknown, LPTSTR lpClassName)
	{
		this->iid = refIID;
		this->lpClassName = lpClassName;
		this->lpCUnknown = lpCUnknown;
	}

	boolean isEqual(VObject *obj) const
	{ 
		return obj->isA(ComInterfaceCls) &&
		   	(iid == ((ComInterface *) obj)->iid);
	}

	//----------------------------------------------------------------------------
	//
	//	Hash on the IID.
	//
	HashKey hash() const
	{ 
	integer i;
	integer n = sizeof(iid);
	HashKey key = 0;
	pointer p = (pointer)&iid;

#if defined(FROM_STR)
	for (i = 0; i < n; i += 2, p += 2) {
		key += (*p) & 0177;
	}
#else
	for (i = 0; i < n; i++, p++) {
		key += (*p) & 0177;
	}
#endif

	return key & HASH_KEY_MASK;
	}
	
	VClass		*iam();
	~ComInterface() {;}

	IID			iid;
	LPTSTR		lpClassName;
	CUnknown		*lpCUnknown;
};

defineClass(ComInterface, VObject)

//**********************************************************************
//
//			>>> XUnknown <<<
//
//**********************************************************************

//**********************************************************************
//
XUnknown::XUnknown(CUnknown *pUnknown)
	: Unknown( pUnknown)
{
	CDBG(	<< "Constructing IUnknown (" << this << ") for CUnknown ("
			<< pUnknown << ')' << endl);
}

//**********************************************************************
XUnknown::~XUnknown()
{
}

//**********************************************************************
//
//			>>> CRUnknown <<<
//
//**********************************************************************

//**********************************************************************
void CRUnknown::init(void)
{
	lCount = 0;
}

//**********************************************************************
CRUnknown::CRUnknown()
{
	Unknown = 0;
	init();
}

//**********************************************************************
CRUnknown::CRUnknown(LPUNKNOWN lpIUnknown)
{
	Unknown = lpIUnknown;
	init();
}

//**********************************************************************
CRUnknown::~CRUnknown()
{
	cv_assert(lCount == 0);
}

//**********************************************************************
void CRUnknown::unrealize()
{
	CDBG(	<< "In " << getClassName() << "::unrealize(), count = "); // NO endl!

	// This releases the native object but, since we're a "CR" class
	// the release won't delete "this" (this->Release() does that).
	long	count = getUnknown().Release();

	CDBG(	<< count << endl);
			
	// Indicate native is gone so we don't use methods from invalid
	// interface pointer.
	Unknown = 0;

}

//**********************************************************************
//
STDMETHODIMP CRUnknown::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	CDBG(	<< "In " << getClassName() << "::QueryInterface, ("
			<< IID2Name(riid) << ')' << endl);
			
	return getUnknown().QueryInterface(riid, ppvObj);
}

//**********************************************************************
//
STDMETHODIMP_(ULONG) CRUnknown::AddRef()
{
	CDBG(	<< "In " << getClassName() << "::AddRef, count = " 
			<< lCount + 1 << endl);
			
	++lCount;
	
	return hasUnknown() ? getUnknown().AddRef() : lCount;
}

//**********************************************************************
//
STDMETHODIMP_(ULONG) CRUnknown::Release() 
{
	CDBG(	<< "In " << getClassName() << "::Release, count = " 
			<< lCount - 1 << endl);
			
	// do the release before the delete so that the state of the 
	// contained object is correct for the destructor.
	long	count = hasUnknown() ? getUnknown().Release() : lCount;
				         
	if (--lCount == 0) {
		delete this;
	}
	cv_assert(lCount >= 0);

	return count;
}

//**********************************************************************
//
//			>>> CUnknown <<<
//
//**********************************************************************

// Sanjay Start
CUnknown::CUnknown()
{
	CDBG(	<< "In CUnknown's default constructor, this = " << this
			<< endl);

	init();
	realize(this, (LPUNKNOWN)this);
}
	
CUnknown::CUnknown(const CUnknown& source)
{
	init();
	copyFrom((VObject*)&source);
}
 
CUnknown& CUnknown::operator=  (const CUnknown& source)
{
	if (source == *this)
		return *this;
	
	copyFrom((VObject*)&source);

	return *this;
}

CUnknown* CUnknown::operator=  (const CUnknown* source)
{
	if (source == this)
		return this;
	
	copyFrom((VObject*)source);

	return this;
}

boolean CUnknown::copyFrom(VObject *pObject)
{
	cv_assert(pObject->isA(CUnknownCls));
	CUnknown* source = (CUnknown*) pObject;	

	lCount = source->lCount;
	lpOuterCUnknown = source->lpOuterCUnknown;
	lpIUnknown = source->lpIUnknown;
	realize(lpOuterCUnknown, lpIUnknown);
	if (lpAggregates) {
		lpAggregates->freeContents();
		delete lpAggregates;
		lpAggregates = 0;
	}
   	if (lpOuterCUnknown != this) {
		lpOuterCUnknown->unAggregate(this);
	}
	// free all interfaces
	if(lpInterfaces) {
		lpInterfaces->freeContents();
		delete lpInterfaces;
		lpInterfaces = 0;
	}
	
	lpAggregates = new VOrdCollect;
	DO (source->lpAggregates, CUnknown, lpContainedUnknown)
		VObject *added = lpAggregates->add((VObject*)lpContainedUnknown);
		cv_assert(added);
	END

	lpInterfaces = new VSet;
	DO (source->lpInterfaces, ComInterface, lpInterface)
		VObject *added = lpInterfaces->add( lpInterface);
		cv_assert(added);
	END
	
	return TRUE;
}

void CUnknown::setIUnknown(LPUNKNOWN lpIUnknown)
{
	realize((LPCUNKNOWN)lpIUnknown, lpIUnknown);
}
// Sanjay End

//**********************************************************************
//
// WARNING: this constructor is only for the Controlling unknown.
//
// lpIUnknown - pointer to the IUnknown to which this CUnknown is associated.
//
CUnknown::CUnknown(LPUNKNOWN	lpIUnknown)		// associated IUnknown
{
	CDBG(	<< "In CUnknown's aggregate parent constructor, this = " << this
			<< endl);

	init();

	realize(0, lpIUnknown);
}

//**********************************************************************
//
// WARNING: this constructor is only for the aggregate children
// of the Controlling unknown.
//
// refCUnkOuter - reference of the controlling CUnknown.
//
// lpIUnknown - pointer to the IUnknown to which this CUnknown is associated.
//
CUnknown::CUnknown(REFCUNKNOWN	refCUnkOuter,	// controlling CUnknown
				   LPUNKNOWN	lpIUnknown)		// associated IUnknown
{
	CDBG(	<< "In CUnknown's aggregate child constructor, this = " << this
			<< endl);

	init();

	realize(&refCUnkOuter, lpIUnknown);
}

//**********************************************************************
//
CUnknown::~CUnknown()
{
	cv_assert(lCount == 0);

	// *** Aggregate child processing
	if (lpAggregates) {

		// assert that I'm a controlling unknown
	 	cv_assert(lpOuterCUnknown == this);

		// If I am a controlling unknown, then delete my aggregate kids,
		// as a convenience
		//!!!dss 12/11/95 - don't think my kids should be deleted. Inner Release
		// them and let them decide for themselves if they should be deleted.
		// Reason: the kids could be aggregate kids of some other controlling
		// unknown or they could be both a controlling unknown and an aggregate
		// child of some other controlling unknown.
		lpAggregates->freeContents();
		delete lpAggregates;
		
		lpAggregates = 0;
	}
                                 
	// *** Aggregate parent processing                                 
	
	// if I have a Controlling Unknown, remove me from it.
	if (lpOuterCUnknown != this) {
		lpOuterCUnknown->unAggregate(this);
	}

	// *** My processing
	                              
	// did anybody do an exposeInterface().
	cv_assert(lpInterfaces);
	                              
	// free all my interfaces
	lpInterfaces->freeContents();
	delete lpInterfaces;
	lpInterfaces = 0;

#if 0
	if (myRiids) {
		delete myRiids;
		myRiids = 0;
	}
#endif

	cv_assert(lpIUnknown);
	delete lpIUnknown;
	lpIUnknown = 0;

	if (m_bOleInitialized) {
		OleUninitialize();
		m_bOleInitialized = FALSE;
	}
}

//**********************************************************************
//
void CUnknown::init(void)
{
	lCount = 0;
	lpAggregates = 0;
	lpInterfaces = 0;
	lpOuterCUnknown = 0;
	lpIUnknown = 0;
//	lpInnerUnknown = 0;

	deleteOnRelease = TRUE;

	m_bOleInitialized = SUCCEEDED(OleInitialize(NULL));
}

//**********************************************************************
//
void CUnknown::realize(LPCUNKNOWN lpCUnkOuter, LPUNKNOWN lpIUnknown)
{
	cv_assert(lpIUnknown);

	this->lpIUnknown = lpIUnknown;
//	this->lpInnerUnknown = lpUnkInner;

	// if an outer unknown is passed in, add myself as an aggregatee on 
	// the outer unknown.
	if (lpCUnkOuter) {
		this->lpOuterCUnknown = lpCUnkOuter;
		
// This is an external operation, now.
//		lpCUnkOuter->aggregate(this);
	}

	// else set myself as the controlling unknown
	else {
		this->lpOuterCUnknown = this;
	}
}

//**********************************************************************
//
// Add unknown to list of aggregates
//
// Parameters
//
//		lpContainedUnknown - the contained CUnknown
//
void CUnknown::aggregate(CUnknown *lpContainedUnknown)
{
	if (lpAggregates == 0) {
		lpAggregates = new VOrdCollect;
	}
		
	lpAggregates->add((VObject *)lpContainedUnknown);

	if (lpInterfaces == 0) {
		lpInterfaces = new VSet;
	}

	// Did we call exposeInterface(), at least once, on this unknown?
	cv_assert(lpContainedUnknown->lpInterfaces);

	// copy all the interfaces supplied by the contained unknown
	// don't use addTo() because it doesn't account for duplicates.
	DO (lpContainedUnknown->lpInterfaces, ComInterface, lpInterface)
		VObject *added = lpInterfaces->add( lpInterface);
		cv_assert(added);
	END

}

//**********************************************************************
//
// Remove unknown from list of aggregates
//
// Parameters
//
//		lpContainedUnknown - the contained CUnknown
//
void CUnknown::unAggregate(CUnknown *lpContainedUnknown)
{
	if (lpAggregates) {
		lpAggregates->removeSame((VObject *)lpContainedUnknown);
	}

	// delete all the interfaces supplied by the container unknown
	DO (lpInterfaces, ComInterface, lpInterface)
		if ( lpInterface->lpCUnknown == lpContainedUnknown) {
			lpInterfaces->remove(lpInterface);

			// delete the ComInterface 
			// let the child delete its own interfaces
//			delete lpInterface;
		}
	END
}

//**********************************************************************
//
STDMETHODIMP CUnknown::QueryInterface(REFIID riid, LPVOID FAR* ppvObj)
{
	CDBG(	<< "In " << getClassName() << "::QueryInterface, ("
			<< IID2Name(riid) << ')' << endl);

	*ppvObj = NULL;

	// if we are the aggregate
	if (lpOuterCUnknown == this) {
		
		// if the request is for IUnknown or for my interface, then
		// return my interface.
		if ( (riid == IID_IUnknown) /* || (riid == this->riid()) */ ) {
			*ppvObj = this->lpIUnknown;
			CDBG(	<< "    QI return=" << *ppvObj << " ("
					<< this->getClassName() << '=' << this << ')' << endl);
			this->lpIUnknown->AddRef();
			return ResultFromScode(S_OK);
		}

		// else search the interfaces for a matching interface
		if (lpInterfaces) {
			ComInterface	comItf(riid, 0, 0);
			ComInterface	*lpComInfo = (ComInterface *)
										lpInterfaces->find(&comItf);

			if (lpComInfo) {
				*ppvObj = lpComInfo->lpCUnknown->lpIUnknown;
				CDBG(	<< "    QI return=" << *ppvObj << " ("
						<< lpComInfo->lpClassName << '=' 
						<< lpComInfo->lpCUnknown << ')' << endl);
				// AddRef through the returned interface pointer.
				lpComInfo->lpCUnknown->lpIUnknown->AddRef();
				return ResultFromScode(S_OK);
			}
		}
	}

	// else pass it to controlling unknown
	else {
		return lpOuterCUnknown->QueryInterface(riid, ppvObj);
	}

	return ResultFromScode(E_NOINTERFACE);
}

//**********************************************************************
//
// CUnknown::AddRef
//
// Purpose:  Adds to the reference count at the interface level.
//
// Return Value:
//
//      ULONG   -   The new reference count of the interface
//
//********************************************************************

STDMETHODIMP_(ULONG) CUnknown::AddRef()
{
	CDBG(	<< "In " << getClassName() << "::AddRef, count = " 
			<< lCount + 1 << endl);

	// outer == inner, so its for me
	if (lpOuterCUnknown == this) {
		// increment the interface reference count
		return ++lCount;
	}

	// else pass it to controlling unknown
	else {
		// inc the inner ref count for debugging purposes
		++lCount;
		return lpOuterCUnknown->AddRef();
	}
}

//**********************************************************************
//
// CUnknown::Release
//
// Purpose: Decrements the reference count at this level
//
//      ULONG   -   The new reference count of the interface.
//
//********************************************************************

STDMETHODIMP_(ULONG) CUnknown::Release()
{
	CDBG(	<< "In " << getClassName() << "::Release, count = " 
			<< lCount - 1 << endl);

	// outer == inner, so its for me.
	if (lpOuterCUnknown == this) {

		// decrement the interface reference count
		if (--lCount == 0) {
			if (getDeleteOnRelease()) {
				// this delete assumes this CUnknown is a Controlling CUnknown
				// inheriting the CUnknown in order for virtual destruction 
				// to work. You will not get virtual destruction when
				// the CUnknown is an aggregate child of "this" object.
				delete this;
			}
			return 0;
		}
		cv_assert(lCount >= 0);
	}

	// else pass it to controlling unknown
	else {
		// decr the inner ref count for debugging purposes
		--lCount;
		cv_assert(lCount >= 0);
		return lpOuterCUnknown->Release();
	}

	return lCount;
}

//**********************************************************************
//
// CUnknown::InnerAddRef
//
// Purpose:
//
// Return Value:
//
//      ULONG   -   The new reference count of the interface
//
//********************************************************************

STDMETHODIMP_(ULONG) CUnknown::InnerAddRef()
{
	CDBG(	<< "In " << getClassName() << "::InnerAddRef, count = " 
			<< lCount + 1 << endl);

	// increment the interface reference count
	return ++lCount;
}

//**********************************************************************
//
// CUnknown::InnerRelease
//
// Purpose: 
//
//      ULONG   -   The new reference count of the interface.
//
//********************************************************************

STDMETHODIMP_(ULONG) CUnknown::InnerRelease()
{
	CDBG(	<< "In " << getClassName() << "::InnerRelease, count = " 
			<< lCount - 1 << endl);

	// decrement the interface reference count
	if (--lCount == 0) {
		if (getDeleteOnRelease()) {
			// this delete assumes this CUnknown is a Controlling CUnknown
			// inheriting the CUnknown in order for virtual destruction 
			// to work. You will not get virtual destruction when
			// the CUnknown is an embedded object. However, if the 
			// CUnknown is embedded, this would be a CUnknown aggregate 
			// child.
			delete this;
		}
		return 0;
	}

	return lCount;
}

//********************************************************************
//
boolean CUnknown::setDeleteOnRelease(boolean deleteOnRelease)
{
	boolean prevValue = this->deleteOnRelease;
	
	this->deleteOnRelease = deleteOnRelease;

	return prevValue;
}

//********************************************************************
//
void CUnknown::exposeInterface(REFIID refIID, LPTSTR lpClassName)
{

	if (lpInterfaces == 0) {
		lpInterfaces = new VSet;
	}

//	lpInterfaces->add( (VObject *) new ComInterface(refIID, this, lpClassName));
	VObject* added = lpInterfaces->add( new ComInterface(refIID, this, lpClassName));
	cv_assert(added);

	CDBG(	<< "In CUnknown::exposeInterface, " << lpClassName << " = "
			<< this << ", IP = " << LPUNKNOWN(*this) << endl);

}

//********************************************************************
//
void CUnknown::unexposeInterface(REFIID refIID)
{
	ComInterface	comItf(refIID, 0, 0);
	
	ComInterface	*removedItf = (ComInterface*) lpInterfaces->remove( &comItf);

	CDBG(	<< "In CUnknown::unexposeInterface, ");

	if (removedItf) {
		CDBG(	<< removedItf->lpClassName << " = " 
				<< removedItf->lpCUnknown << ", IP = " 
				<< LPUNKNOWN(*removedItf->lpCUnknown)
				<< endl);
	}
	else {
		CDBG(	<< IID2Name(refIID) << " not found" << endl);
	}

}
