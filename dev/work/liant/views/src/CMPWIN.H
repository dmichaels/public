// $Header:   Q:/views/win/vcs/cmpwin.h_v   1.6   Jun 03 1997 09:51:44   scottt  $

//	cmpwin.h
//
//  Compound Document Window
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
//  01/25/95 dss    Initial release.
//  12/07/95 dgm    Overrode the other getFrom().
//	12/08/95 dss	added VComponentWindowCls. Name change: VCOMPONENTWINDOW_H.
//	01/09/96 dss	VComponent name change.
//  04/29/96 evc    Added   insertItem(POINT pt, VItemWindow * pItemwin)
//  06/19/96 evc    Made insertItem virtual

//	---------------------------------------------------------------------------

#ifndef	VCOMPONENTWINDOW_H
#define VCOMPONENTWINDOW_H

// Views Compound Document Types
#include "oledefs.h"

// OLE Includes
#include "unknown.h"
#include "droptarg.h"
#include "owindow.h"
#include "winrect.h"
#include "itemwin.h"

// View Compound Document Includes
#include "cmpobj.h"

extern VClass *VComponentWindowCls;

CLASS CDataObject;

CLASS VComponentWindow : public VComponentObject
{
public: 

    VComponentWindow();

    // construct an outer Unknown VWindow
    VComponentWindow(LPVWINDOW lpWindow);
    VComponentWindow(LPVWINDOW lpWindow, COleWindow *pOleWindow);
    ~VComponentWindow();
    
    VClass *iam();

    LPTSTR      getClassName() {return _T("VComponentWindow");}

    // *** Pseudo Inherited Interfaces (no ';' follows these macros) ***
    CONTAIN_INTERFACE1(C, DropTarget)
    CONTAIN_INTERFACE1(C, OleWindow)

public: 

    COleWindow  *getOleWindowBase(void);

    // conversions
    operator LPVWINDOW()        {return getVThis();}

    // Views class interaction
    LPVWINDOW   getVThis()      {return (LPVWINDOW) getVObject();}
    LPVWINDOW   getVWindow()    {return (LPVWINDOW) getVObject();}
    LPVOBJECT   getVSuperThis() {return getVObject();}
    LPVWINDOW   getVParent()    {return (LPVWINDOW) getVWindow()->getParent();}
    VComponentWindow   *getCDParent();

    // Notifications
    virtual boolean     itemActivate(LPVCOMPONENTITEM lpItem);
    virtual boolean     itemDeactivate(LPVCOMPONENTITEM lpItem);

    boolean             allowDrops(boolean bAllow);

    // Operations
    boolean             pasteItem(POINT pt);
    virtual boolean             insertItem(POINT pt);

	boolean 			insertItem(POINT pt, VItemWindow * pItemwin);
	// use pItemwin to construct the insertable's componentwindow

    // VWindow events
    virtual boolean     paint();
    virtual boolean     resized(int w, int h);
    virtual boolean     moved(int x, int y);

    virtual boolean     mouseDbl(int x, int y);
    virtual boolean     mouseDn(int x, int y);
    virtual boolean     mouseEnter(int buttonStatus);
    virtual boolean     mouseExit(int buttonStatus);
    virtual boolean     mouseMv(int x, int y, int buttonStatus);
    virtual boolean     mouseUp(int x, int y);
    virtual boolean     rmouseDbl(int x, int y);
    virtual boolean     rmouseDn(int x, int y);
    virtual boolean     rmouseUp(int x, int y);

    virtual void        setChildFocus(VWindow *win) {;}
    virtual void        addChild(VWindow *w) {;}
    virtual boolean     givenFocus(void) {return FALSE;}
    virtual boolean     clearFocus(void) {return FALSE;}

    virtual void        destroyed(void);
    virtual boolean     close();
    virtual boolean     quit();

    // Release/destructor hack
    virtual void        destroy(void);

    // Object Events
    virtual boolean     objDropped(const CRDataObject&, 
                                   const VOlePosition&, DWORD);
                                   
    // Misc.
    virtual boolean     disconnectStorage(void);                                   
    virtual boolean     connectStorage(void);                                  
    virtual boolean     save(void);
    virtual boolean     isDirty(void);                                 
    
    // Archiving
#ifndef CV_NOOLEARCHIVER
    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
	virtual void 	getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif
    
protected:

    // Drop support
    boolean             bDropAllowed;

private:
    void                init(void);
};

//**********************************************************************
//
inline VComponentWindow *VComponentWindow::getCDParent()
{
    return getVThis()->getParent()->getComponentThis();
}

extern VClass	*VComponentWindowCls;

#endif // VCOMPONENTWINDOW_H
