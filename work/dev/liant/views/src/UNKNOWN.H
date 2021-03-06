// $Header:   Q:/views/win/vcs/UNKNOWN.H_v   1.12   Mar 04 1997 14:14:10   gregm  $

//	CUnknown
//
//	Views IUnknown Implementation
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
//	??/??/?? dss	Initial release.
//	12/04/95 dss	Added PVCS header. Use Views style #ifndef CUNKNOWN_H.
//  12/12/95 dss    added realize.
//  01/30/96 dss    declare VSet. Remove class methods from CONTAIN_INTERFACE1.
//	02/27/96 dss    added CUnknown::m_bOleInitialized.
//	---------------------------------------------------------------------------

//**********************************************************************
//
// Description: This class defined the abstract class IUnknown, so that
// it does not have to be redundantly defined. It also, handles 
// aggregation.
//
// This class functions by inheritance or by including the CUnknown
// association macro in the associated class.
// 
// For aggregate children (inner unknowns), this class must have:
//		- a pointer to the associated IUnknown.
//		- a pointer to the controlling CUnknown.
//
// For Controlling unknowns (outer unknowns), this class must have,
//		- a pointer to the associated IUnknown.
//		- a pointer to the object that is (ISA) the controlling unknown.
//		  This must the the pointer returned from a "new" operator,
//		  that was used to dynamically create the controlling unknown.
//
//**********************************************************************

#ifndef CUNKNOWN_H
#define CUNKNOWN_H

#include "oledefs.h"

extern VClass *CUnknownCls;

CLASS VOrdCollect;
CLASS VSet;

CLASS CUnknown;
typedef CUnknown FAR *LPCUNKNOWN;
typedef CUnknown FAR& REFCUNKNOWN;

CLASS CRUnknown;
typedef CRUnknown FAR *LPCRUNKNOWN;
typedef CRUnknown FAR& REFCRUNKNOWN;

CLASS CUnknown : public VObject
{
public:

	// *** IUnknown methods ***
	virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppvObj);
	virtual STDMETHODIMP_(ULONG) AddRef();
	virtual STDMETHODIMP_(ULONG) Release();

	virtual STDMETHODIMP_(ULONG) InnerAddRef();
	virtual STDMETHODIMP_(ULONG) InnerRelease();

	// ctor/dtor

	// Inner (aggregattee) unknown's constructor
	// This constructor must be used on objects that embed a CUnknown
	CUnknown(REFCUNKNOWN	refCUnkOuter,	// the controlling CUnknown
			 LPUNKNOWN		lpIUnknown);	// the associated IUnknown

	// Outer (Controlling) unknown's constructor
	// This constructor must be used on objects that inherit from CUnknown
	CUnknown(LPUNKNOWN		lpIUnknown);	// the associated IUnknown
	// Start Sanjay
	CUnknown();	
	CUnknown(const CUnknown& source); 
    CUnknown& operator=  (const CUnknown& source);
    CUnknown* operator=  (const CUnknown* source);
	LPUNKNOWN getIUnknown() { return lpIUnknown; }
	void setIUnknown(LPUNKNOWN lpIUnknown);
	// Required meta-method
	virtual VClass*	iam();
	virtual boolean	copyFrom(VObject *pObject);
	// End Sanjay

	virtual ~CUnknown();

	virtual LPTSTR	getClassName()		{return _T("CUnknown");}
//	virtual REFIID	getRiid()			{return *myRiid;}

	virtual	void	exposeInterface(REFIID refIID, LPTSTR lpClassName);
	virtual	void	unexposeInterface(REFIID refIID);

	boolean			setDeleteOnRelease(boolean deleteOnRelease);
	boolean			getDeleteOnRelease() {return deleteOnRelease;}
	virtual void	realize(LPCUNKNOWN lpUnkOuter, LPUNKNOWN lpIUnknown);
	
	virtual void	aggregate(CUnknown *lpContainedUnknown);
	virtual void	unAggregate(CUnknown *lpContainedUnknown);

	// cast operators to equate Cxxx class to Ixxx class
	operator 			LPUNKNOWN() {return lpIUnknown;}

	// This method duplicates the behavior of STD_IUNKNOWN_INTERFACE
	// when this class is inherited using C++ inheritance.
	virtual CUnknown&	getUnknown() const {return (CUnknown&)(*this);}
//	operator 			IUnknown() {return *lpIUnknown;}

protected:
	friend CLASS VVariable;
	int				lCount;
	VOrdCollect		*lpAggregates;		// collection of lpCUnknowns for QI().
	VSet			*lpInterfaces;		// collection of interfaces for QI().
	LPCUNKNOWN		lpOuterCUnknown;	// the controlling unknown for
										// aggregation. Equals this if
										// unknown is aggregatee.
	LPUNKNOWN		lpIUnknown;			// associated IUnknown
//	LPUNKNOWN		lpInnerUnknown;		// Object's IUnknown
	
	boolean			deleteOnRelease;
	boolean			m_bOleInitialized;

private:
	void			init(void);
};

//**********************************************************************
//
// Turns IUnknown Interface into CUnknown interface
// Use this to declare the IUnknown when a class inherits from 
// any Ixxx interface
//

#define STD_IUNKNOWN_METHODS() \
	virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppvObj) \
		{return getUnknown().QueryInterface(riid, ppvObj);} \
	virtual STDMETHODIMP_(ULONG) AddRef() {return getUnknown().AddRef();} \
	virtual STDMETHODIMP_(ULONG) Release() {return getUnknown().Release();}

// equivilance
#define STD_Unknown_METHODS()  STD_IUNKNOWN_METHODS()
      
// This macro contributes to pseudo-inheritance. It gives the parent object
// the ability to behave as if it was a super-class of the child object.
// The behaviors are:
//
//		A reference to the parent object behaves like it was a reference
//		to the child object.
//
// This macro creates the methods to access a contained object.
//
// The following are the methods created by this macro using "Unknown"
// as an example class.
//
//		getUnknown()
//		hasUnknown()
//		operator Unknown&()
//		operator Unknown*()
//		operator ==(Unknown*)
//		operator !=(Unknown*)
//
#define CONTAIN_ACCESS(prefix, theClass, access) \
    prefix##theClass&	get##theClass() const {return (prefix##theClass&) (access);} \
	boolean		has##theClass() const {return LPVOID(&get##theClass()) != 0;} 

// This macro expands upon the CONTAIN_ACCESS() to provide parent access 
// to the base object (basePrefix) of the contained object.
#define CONTAIN_ACCESS2(containedPrefix, basePrefix, containedClass, access) \
	CONTAIN_ACCESS(containedPrefix, containedClass, access) 
    
#if 0    
#define CONTAIN_ACCESS(CR, Storage, *Storage)
    CRStorage&	getStorage() {return (CRStorage&) (*Storage);}
	boolean		hasStorage() const {return LPVOID(&getStorage()) != 0;}
    operator CRStorage&()	{return getStorage();}
    operator CRStorage*()	{return (CRStorage*)&getStorage();}
    int operator ==(CRStorage* p) {return p == (CRStorage*)&getStorage();}
    int operator !=(CRStorage* p) {return p != (CRStorage*)&getStorage();}
    			
#define CONTAIN_ACCESS2(CR, I, Storage, *Storage)
	CONTAIN_ACCESS(CR, Storage, *Storage)
    operator IStorage&() {return (IStorage&)getStorage();}
    operator IStorage*() {return (IStorage*)getStorage();}
    int operator ==(IStorage* p) {return p == (IStorage*)getStorage();}
    int operator !=(IStorage* p) {return p != (IStorage*)getStorage();}
#endif

// This macro defines a containment for class prefix##theClass and creates
// access the containment parent's access to the contained class. Also, access
// to the base OLE class I##theClass is created for the containment parent.
#if 0
#define CONTAIN_INTERFACE1(prefix, theClass) \
public: \
	STD_##theClass##_METHODS() \
	CONTAIN_ACCESS2(prefix, I, theClass, *theClass) \
protected: \
	prefix##theClass		*theClass;
#else
#define CONTAIN_INTERFACE1(prefix, theClass) \
public: \
	CONTAIN_ACCESS2(prefix, I, theClass, *theClass) \
protected: \
	prefix##theClass		*theClass;
#endif

// This macro defines a containment for class prefix##theClass and creates
// the containment parent's access to the contained class. Also, access
// to the base OLE class I##theClass is created for the containment parent.
#define CONTAIN_INTERFACE2(prefix, theClass) \
public: \
	CONTAIN_ACCESS2(prefix, I, theClass, *theClass) \
protected: \
	prefix##theClass		*theClass;
                        
// Defines a reference to derived CRUnknown class.
// The access is always *Unknown as defined in class CRUnknown because this
// macro should only be embedded in derived classes of CRUnknown.
#define CRUNKNOWN_INTERFACE(theClass) \
public: \
	STD_##theClass##_METHODS() \
	CONTAIN_ACCESS(I, theClass, *Unknown)

#define STD_IUNKNOWN_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, Unknown, access)

// Same as inheritance except methods not defined (inherited).   
#define CONTAIN_IUNKNOWN_INTERFACE() \
public: \
	STD_IUNKNOWN_ACCESS(*Unknown) \
protected: \
	CUnknown		*Unknown;

#define INHERIT_IUNKNOWN_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*Unknown) \
protected: \
	CUnknown		*Unknown;
           
//**********************************************************************
//
// Unknown Reference Class
//
// This class is
//		- a sub-object of an IUnknown object
//
// This is a convenience class that refers to an IUnknown Interface Pointer 
// (IP) that is given to you by another process. It adds minimal helpful
// functionality not found in OLE (inheritance, among others) to use an
// Interface Pointer.
//
// This class is NOT an IUnknown implementation, and hence cannot be used
// to override IUnknown methods. You must reference the IP contain within 
// to access IUnknown methods.
//

// CLASS CRUnknown : public VObject
CLASS CRUnknown : public VObject
{
public:
	CRUnknown();
	CRUnknown(LPUNKNOWN lpIUnknown);
	virtual ~CRUnknown();
	
	virtual boolean	isRealized() {return Unknown != 0;}
    void			realize(LPUNKNOWN lpUnk) {Unknown = lpUnk; lCount = 1;}
    void			unrealize(void);
    
	void	init(void);

//	CRUNKNOWN_INTERFACE(Unknown)
	virtual STDMETHODIMP QueryInterface(REFIID riid, LPVOID FAR* ppvObj);
	virtual STDMETHODIMP_(ULONG) AddRef();
	virtual STDMETHODIMP_(ULONG) Release();

	CONTAIN_ACCESS(I, Unknown, *Unknown)

public:	
	virtual LPTSTR	getClassName()		{return _T("CRUnknown");}
	
protected:
	IUnknown	*Unknown;
	long		lCount;
};

//**********************************************************************
//
//  IUnknown Class
//
CLASS XUnknown : public IUnknown
{
public:

	XUnknown(CUnknown *);
	~XUnknown();

	INHERIT_INTERFACE(IUNKNOWN, Unknown)
};

#endif // _CUNKNOWN_H_
