// $Header:   Q:/views/win/vcs/cmpview.h_v   1.4   Jun 03 1997 09:51:44   scottt  $

//	cmpview.h
//
//  Compound Document View
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
//  Revision History:
//  -----------------
//  01/30/95 dss    Initial release.
//  12/08/95 dss    added canLocallyActivate(). Name change: VCOMPONENTVIEW_H.
//					added VComponentViewCls. added default ctor.
//	01/09/96 dss	VComponent name change.
//	01/22/96 dss	Removed onClipboard data member.
//	---------------------------------------------------------------------------

#ifndef	VCOMPONENTVIEW_H
#define VCOMPONENTVIEW_H

// Views Compound Document Types
#include "oledefs.h"

// OLE Includes
#include "unknown.h"

// View Compound Document Includes
#include "cmpwin.h"

// View Ole Classes
#include "oipuiwin.h"

CLASS VComponentView : public VComponentWindow
{
public: 

    // construct a default VComponentView
    VComponentView();

    // construct an outer Unknown VCDVView
    VComponentView(LPVVIEW lpVView);

    // construct an outer Unknown VComponentView with storage
    VComponentView(LPVVIEW lpVView, VString *path, VComponentObject *storageParent);

    // construct an outer Unknown VComponentView using an unknown
    VComponentView(LPVVIEW lpVView, COleInPlaceUIWindow *pOleInPlaceUIWindow);

    // construct an outer Unknown VComponentView using an unknown with storage
    VComponentView(LPVVIEW lpVView, COleInPlaceUIWindow *pOleInPlaceUIWindow,
            VString *path, VComponentObject *storageParent);
            
    ~VComponentView();

	VClass		*iam();

    LPTSTR          getClassName() {return _T("VComponentView");}

    // *** Inherited methods (no ';' follows these macros) ***
    CONTAIN_INTERFACE1(C, OleInPlaceUIWindow)

public: 

    // conversions
    operator LPVVIEW()          {return getVThis();}

    // operations
    LPVVIEW     getVThis()      {return (LPVVIEW) getVObject();}
    LPVWINDOW   getVSuperThis() {return (LPVWINDOW) getVObject();}
    LPVVIEW     getVView()      {return (LPVVIEW) getVObject();}
    VComponentView     *getCDParent()  {return (VComponentView*) VComponentWindow::getCDParent();}

    // Notifications
    virtual boolean itemActivate(LPVCOMPONENTITEM lpItem);
    virtual boolean itemDeactivate(LPVCOMPONENTITEM lpItem);

    // VView events
    virtual boolean resized(int w, int h);
    boolean         mouseDn(int x, int y);
    boolean         close();
    boolean         quit();
    boolean         renameStorage(VPathString& newName);
    boolean         activate(VView *v);
    boolean         deactivate(VView *v);
    
    // Storage
    enum yesNoCancel querySave(VPathString &newName);
    boolean         save(void);
    boolean         saveAs(VPathString& newName);
    boolean         saveInto(void);

    // Archiving
#ifndef CV_NOOLEARCHIVER
    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
	void		 	getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif
    
    // DocView Management
    LPVCOMPONENTITEM       getFocusedComponentItem();
	boolean			canLocallyActivate(void) const {return bAllowLocalActivatation;}
	boolean			allowLocalActivation(boolean bAllow);

    virtual boolean	createStorage(VComponentObject *storageParent = 0,
								  VString *lpStorageName = 0);
protected:
    // activation
    boolean         bAllowLocalActivatation;	// allow local activatation

private:
    void            init();
};

extern VClass	*VComponentViewCls;

#endif // VCOMPONENTVIEW_H
