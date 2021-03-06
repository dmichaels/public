// $Header:   Q:/views/win/vcs/oipsite.h_v   1.7   Jun 03 1997 09:51:56   scottt  $

//	COleInPlaceSite
//
//  OLE IOleInPlaceSite Implementation
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	12/28/94 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#if !defined( _COLEINPLACESITE_H_ )
#define _COLEINPLACESITE_H_

#include "owindow.h"

// Forward Declarations
CLASS VComponentItem;
typedef VComponentItem FAR& REFVCOMPONENTITEM;
typedef VComponentItem FAR *LPVCOMPONENTITEM;

CLASS COleInPlaceSite;
typedef	COleInPlaceSite FAR *LPCOLEINPLACESITE;

CLASS COleInPlaceSite : public COleWindow
{
public:

	// Base class constructor
	// Use this constructor when this object is a base-class object
	COleInPlaceSite(REFVCOMPONENTITEM refCDItem);

	// super-class constructor
	// use this constructor when this object is a super-class object
	COleInPlaceSite(REFVCOMPONENTITEM refCDItem, LPUNKNOWN lpIUnkBase);
	
	virtual ~COleInPlaceSite();

	LPTSTR			getClassName() {return _T("COleInPlaceSite");}

public:

	// Start of Sanjay's Code
    virtual STDMETHODIMP GetWindow(HWND FAR* lphwnd);
	// End of Sanjay's Code
	// *** IOleInPlaceSite methods ***
   	virtual STDMETHODIMP CanInPlaceActivate();
    virtual STDMETHODIMP OnInPlaceActivate();
    virtual STDMETHODIMP OnUIActivate();
    virtual STDMETHODIMP GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame,
                        LPOLEINPLACEUIWINDOW FAR* lplpDoc,
                        LPRECT lprcPosRect,
                        LPRECT lprcClipRect,
                        LPOLEINPLACEFRAMEINFO lpFrameInfo);
    virtual STDMETHODIMP Scroll(SIZE scrollExtent);
    virtual STDMETHODIMP OnUIDeactivate(BOOL fUndoable);
    virtual STDMETHODIMP OnInPlaceDeactivate();
    virtual STDMETHODIMP DiscardUndoState();
    virtual STDMETHODIMP DeactivateAndUndo();
    virtual STDMETHODIMP OnPosRectChange(LPCRECT lprcPosRect);

	// *** COleInPlaceSite methods ***
	operator LPOLEINPLACESITE() {return (LPOLEINPLACESITE)(LPUNKNOWN)getUnknown();}

	IOleInPlaceObject&		getOleInPlaceObject(void)
							{cv_assert(lpOleInPlaceObject);
							 return *lpOleInPlaceObject;}

protected:
	LPVCOMPONENTITEM	lpCDItem;

	LPOLEINPLACEOBJECT	lpOleInPlaceObject; 
	
private:
	void				init(REFVCOMPONENTITEM refCDItem);
};

#define STD_IOLEINPLACESITE_METHODS() \
   	virtual STDMETHODIMP CanInPlaceActivate() \
				 {return getOleInPlaceSite().CanInPlaceActivate();} \
    virtual STDMETHODIMP OnInPlaceActivate() \
				 {return getOleInPlaceSite().OnInPlaceActivate();} \
    virtual STDMETHODIMP OnUIActivate() \
				 {return getOleInPlaceSite().OnUIActivate();} \
    virtual STDMETHODIMP GetWindowContext(LPOLEINPLACEFRAME FAR* lplpFrame, \
                        LPOLEINPLACEUIWINDOW FAR* lplpDoc, \
                        LPRECT lprcPosRect, \
                        LPRECT lprcClipRect, \
                        LPOLEINPLACEFRAMEINFO lpFrameInfo) \
				 {return getOleInPlaceSite().GetWindowContext( \
							lplpFrame, \
                        	lplpDoc, \
                        	lprcPosRect, \
                        	lprcClipRect, \
                        	lpFrameInfo);} \
    virtual STDMETHODIMP Scroll(SIZE scrollExtent) \
				 {return getOleInPlaceSite().Scroll(scrollExtent);} \
    virtual STDMETHODIMP OnUIDeactivate(BOOL fUndoable) \
				 {return getOleInPlaceSite().OnUIDeactivate(fUndoable);} \
    virtual STDMETHODIMP OnInPlaceDeactivate() \
				 {return getOleInPlaceSite().OnInPlaceDeactivate();} \
    virtual STDMETHODIMP DiscardUndoState() \
				 {return getOleInPlaceSite().DiscardUndoState();} \
    virtual STDMETHODIMP DeactivateAndUndo() \
				 {return getOleInPlaceSite().DeactivateAndUndo();} \
    virtual STDMETHODIMP OnPosRectChange(LPCRECT lprcPosRect) \
				 {return getOleInPlaceSite().OnPosRectChange(lprcPosRect);}

#define STD_OleInPlaceSite_METHODS() STD_IOLEINPLACESITE_METHODS()

#define STD_IOLEINPLACESITE_ACCESS(access) \
		CONTAIN_ACCESS2(C, I, OleInPlaceSite, access)

#define INHERIT_IOLEINPLACESITE_INTERFACE() \
public: \
	STD_IUNKNOWN_METHODS() \
	STD_IUNKNOWN_ACCESS(*OleInPlaceSite) \
	STD_IOLEWINDOW_METHODS() \
	STD_IOLEWINDOW_ACCESS(*OleInPlaceSite) \
	STD_IOLEINPLACESITE_METHODS() \
	STD_IOLEINPLACESITE_ACCESS(*OleInPlaceSite) \
protected: \
	COleInPlaceSite		*OleInPlaceSite;

#define CONTAIN_IOLEINPLACESITE_INTERFACE() \
public: \
	STD_IOLEINPLACESITE_METHODS() \
	STD_IOLEINPLACESITE_ACCESS(*OleInPlaceSite) \
protected: \
	COleInPlaceSite		*OleInPlaceSite;

//**********************************************************************
//
//  IOleInPlaceSite Class
//
CLASS XOleInPlaceSite : public IOleInPlaceSite
{
public:

	XOleInPlaceSite(COleInPlaceSite *);
	~XOleInPlaceSite();

	INHERIT_INTERFACE(IOLEINPLACESITE, OleInPlaceSite)
};

#endif

