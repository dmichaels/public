// $Header:   Q:/views/common/vcs/dockwin.h_v   1.24   25 Feb 1997 10:39:50   tomf  $

//  dockwin.h
//
//  VDockableWindow interface [Common]
//
//  Allegris Foundation 1.1.00
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
//  11/01/95 dgm    Original.
//  11/15/95 dgm    Updated.
//  11/20/95 dgm    Added getDockedSize().
//  11/21/95 dgm    Added uponUndockedClose().
//  11/29/95 dgm    Added beginDrag()/endDrag().
//  11/30/95 dgm    Added uponBeginDrag()/uponEndDrag() & setClientWindow().
//  02/06/96 dgm    Added setUndockedSquare() and setUndockedCenter().
//					Added setDockedAttributes(), setUndockedAttributes(),
//					and getBestSquareSize() -- for internal use.
//	02/07/96 dgm	Added NoClose.
//	02/29/96 dgm	Added DockOnClose.
//	03/04/96 dgm	Added get/setOverDockSitePercentage() and
//					get/setOverDockSitePercentage() and other minor additions.
//	03/21/96 dgm	Archiver support.
//	03/26/96 dgm	Publicized nested classes.
//	05/16/96 dgm	Fixed uponUndockedClose().
//	09/13/96 dgm	Befriended VDockSiteWindow.
//	03/19/97 dgm	Added doingUndocked.
// --------------------------------------------------------------------------

#ifndef DOCKWIN_H
#define DOCKWIN_H

// --------------------------------------------------------------------------
//
#include "window.h"
#include "utilwin.h"
#include "popupmen.h"

// --------------------------------------------------------------------------
//
CLASS   VClass;
CLASS   VDockSiteWindow;
CLASS   VMenuItem;
CLASS   VPopupMenu;
CLASS   VPopupWindow;
CLASS   VString;
CLASS   VStyle;
CLASS   VUtilityWindow;
CLASS   VWindow;

#ifndef CV_NOARCHIVER
CLASS   VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VDockableWindow : public VWindow {
public:
    enum CloseBehavior
    {                   NothingOnClose      =  0x0000,
                        HideOnClose         =  0x0001,
						DeleteOnClose		=  0x0002,
						NoClose				=  0x0004,
						DockOnClose			=  0x0010 };
public:
                        VDockableWindow(VWindow *,
                                        const VStyle& = StyleDefault);
                        VDockableWindow(const VString&,
                                        VWindow *,
                                        const VStyle& = StyleDefault);
                        VDockableWindow(const VFrame&, VWindow *,
                                        const VStyle& = StyleDefault);
                        VDockableWindow(const VString&,
                                        const VFrame&, VWindow *,
                                        const VStyle& = StyleDefault);
                        VDockableWindow();
                       ~VDockableWindow();

    void                operator()(VWindow *);
    void                operator()(VWindow *, const VStyle&);
    void                operator()(VWindow *, const VStyle&,
                                   const VString&);
    void                operator()(const VFrame&, VWindow *);
    void                operator()(const VFrame&, VWindow *, const VStyle&);
    void                operator()(const VFrame&, VWindow *, const VStyle&,
                                   const VString&);
    VClass             *iam();

    virtual boolean     dock(VWindow * = 0);
    virtual boolean     undock(int x = 32767, int y = 32767);
    void                toggleDock();

    boolean             isDocked() const;
    boolean             isDocked(VWindow *) const;
    boolean             isUndocked() const;

    void                uponBeginDrag(VObject *, method);
    void                uponEndDrag(VObject *, method);
    void                uponDocked(VObject *, method);
    void                uponUndocked(VObject *, method);
    void                uponUndockedClose(VObject *, method);

    VUtilityWindow     *getUndockedWindow() const;
    VWindow            *getDockSite() const;

    void                setUndockedCloseBehavior(int);
    int                 getUndockedCloseBehavior() const;

    void                setUndockedWindowMargins(int);
    void                setUndockedWindowMargins(int, int, int, int);
    void                getUndockedWindowMargins(int *, int *, int *, int *);

    void                setUndockedTitle(const VString&);
    void                getUndockedTitle(VString&) const;

    void				setUndockedPosition(int x, int y);
    void				getUndockedPosition(int *x, int *y) const;
    void				setUndockedCenter(boolean = TRUE);
    void				setUndockedSquare(boolean = TRUE);
    void				setUndockedNoClose(boolean = TRUE);
    boolean				getUndockedNoClose() const;

    void                registerDockSite(VWindow *);
    void                unregisterDockSite(VWindow *);

	void				enableDocking(boolean = TRUE);
	void				disableDocking(boolean = TRUE);
	boolean				isDockingEnabled() const;
	boolean				isDockingDisabled() const;

	void				setOverDockSitePercentage(float);
	float				getOverDockSitePercentage() const;

	void				setOverDockSitePointer(boolean);
	boolean				getOverDockSitePointer() const;

	void				useConfigurationMenu(boolean);

    void                setClientWindow(VWindow *);
    virtual VWindow    *getClientAreaWindow() const;
    virtual void        frameWindow(boolean = FALSE);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // Protected section; please KEEP OUT if you're a non-deriving user!
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:

    CLASS DockSiteData : public VObject {
    private:
                            DockSiteData(VWindow *);
    private:
        VWindow            *dockSiteWindow;
        short               dockSiteX;
        short               dockSiteY;
        short               dockSiteWidth;
        short               dockSiteHeight;
        short               dockableWindowWidth;
        short               dockableWindowHeight;
    private:
        friend              VDockableWindow;
    };

	CLASS ConfigurationMenu : public VPopupMenu {
	public:
							ConfigurationMenu();
		void				popup(int, int, VDockableWindow *);
	private:
		void				poppedUp(VPopupMenu *);
		void				poppedUpOptions(VPopupMenu *);
		void				poppedUpOpaque(VPopupMenu *);
		void				poppedUpSensitivity(VPopupMenu *);
		void				poppedUpStyle(VPopupMenu *);
		void				setMove(VMenuItem *);
		void				setSize(VMenuItem *);
		void				setDock(VMenuItem *);
		void				setClose(VMenuItem *);
		void				setOpaque(VMenuItem *);
		void				setSensitivity(VMenuItem *);
		void				setStyle(VMenuItem *);
	private:
		VDockableWindow	   *dockableWindow;
	};

public:

#ifndef CV_NOARCHIVER
    virtual void	        putTo(VArchiver&);
    virtual void	        getFrom(VArchiver&);
    virtual void	        getFrom(VArchiver&, VObject *, boolean = TRUE);
    virtual void	        realize(VWindow *);
#endif

protected:
    virtual boolean			docked(VWindow *docksite);
    virtual boolean			undocked(VWindow *docksite);
    virtual void			undockedClosed(VWindow *);
    virtual VUtilityWindow *newUndockedWindow(VWindow *);
    virtual VWindow		   *newClientWindow(VWindow *);
    virtual void			getDockedSize(int *, int *,
                                          int = -1, int = -1,
                                          VWindow * = 0) const;
    virtual void            beginDrag();
    virtual void            endDrag();
	virtual void			setDockedAttributes(VWindow *);
	virtual void			setUndockedAttributes();

protected:
    virtual boolean         mouseDn(int, int);
    virtual boolean         mouseDbl(int, int);
    virtual boolean         rmouseDn(int, int);
#ifndef CV_NOARCHIVER
    void	                putTo(VArchiver&, boolean);
#endif
public:
    virtual void            showBool(boolean, boolean = TRUE);
protected:
    void                    initialize();
    void                    construct();
    void                    constructUndockedWindow();
    VWindow                *getLastDockSite() const;
    VWindow                *getDockableWindow() const;
    boolean                 prepareForDragging();
    void                    beginDragging(int, int);
    boolean                 dragging(int&, int&, int&, int&, int, int);
    boolean                 endDragging(int, int, int, int, int, int, int&);
    boolean                 endResizing(int, int, int, int, int, int&);
    VWindow                *overDockSite(int, int, int&, int&, int&, int&);
    void	                getBestSquareSize(VWindow *, int *, int *);
	boolean					rmouseDnUndockedWindow(int, int);
	void					constructConfigurationMenu();
	void					controlMenuPoppedUp(VPopupMenu *);
	void					setUndockedDockedStyle(boolean);

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // Private section! please KEEP OUT!
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
    VWindow            *clientWindow;
    VUtilityWindow     *undockedWindow;
    VOrdCollect        *dockSites;
    VWindow            *lastDockSite;
    VString            *undockedTitle;
    VObject            *clientObject;
    VWindow			   *dockSiteUnregisteredWhileDoingUndocked;
    void     (VObject::*beginDragMethod)(VDockableWindow *);
    void     (VObject::*endDragMethod)(VDockableWindow *);
    boolean  (VObject::*dockedMethod)(VDockableWindow *, VWindow *);
    boolean  (VObject::*undockedMethod)(VDockableWindow *, VWindow *);
    void     (VObject::*undockedCloseMethod)(VWindow *);
#ifndef CV_NOARCHIVER
	integer				clientObjectIndex;
	integer				beginDragMethodIndex;
	integer				endDragMethodIndex;
	integer				dockedMethodIndex;
	integer				undockedMethodIndex;
	integer				undockedCloseMethodIndex;
	integer				clientWindowIndex;
#endif
	float				overDockSitePercentage;
    short				lastUndockedX;
    short				lastUndockedY;
    short				lastUndockedWidth;
    short				lastUndockedHeight;
    char				undockedCloseBehavior;
    char				undockedLeftMargin;
    char				undockedRightMargin;
    char				undockedTopMargin;
    char				undockedBottomMargin;
    char				lastUndockedStyle;
    unsigned short		currentlyDocked : 1;
    unsigned short		actingAsDockSite : 1;
    unsigned short		explicitShow : 1;
    unsigned short		explicitHide : 1;
    unsigned short		undockedSquare : 1;
    unsigned short		undockedCenter : 1;
    unsigned short		undockedNotYetShown : 1;
    unsigned short		overDockSitePointer : 1;
    unsigned short		configurationMenuDisabled : 1;
    unsigned short		dockingCurrentlyDisabled : 1;
    unsigned short		undockedNoCloseBox : 1;
    unsigned short		undockedDockOnClose : 1;
    unsigned short		undockedDockedStyle : 1;
    unsigned short		lastUndockedSizeIsValid : 1;
    unsigned short		doingUndocked : 1;

private:
	friend				ConfigurationMenu;
	friend				VDockSiteWindow;
};

extern VClass *VDockableWindowCls;

// --------------------------------------------------------------------------
//
inline boolean VDockableWindow::isDocked() const
{
    return currentlyDocked;
}

// --------------------------------------------------------------------------
//
inline boolean VDockableWindow::isDocked(VWindow *docksite) const
{
    return currentlyDocked && (getDockSite() == docksite);
}

// --------------------------------------------------------------------------
//
inline boolean VDockableWindow::isUndocked() const
{
    return !currentlyDocked;
}

// --------------------------------------------------------------------------
//
inline VUtilityWindow *VDockableWindow::getUndockedWindow() const
{
    return currentlyDocked ? 0 : undockedWindow;
}

// --------------------------------------------------------------------------
//
inline VWindow *VDockableWindow::getClientAreaWindow() const
{
    if (clientWindow != 0) {
        return clientWindow;
    }
    else {
        return (VWindow *)this;
    }
}

// --------------------------------------------------------------------------
//
inline VDockableWindow::DockSiteData::DockSiteData(VWindow *window)
{
    dockSiteWindow = window;
}


inline void VDockableWindow::setUndockedWindowMargins(int all)
{
    setUndockedWindowMargins(all, all, all, all);
}

inline void VDockableWindow::getUndockedWindowMargins(int *left, int *right,
                                                      int *top, int *bottom)
{
    if (left   != 0) { *left   = undockedLeftMargin;   }
    if (right  != 0) { *right  = undockedRightMargin;  }
    if (top    != 0) { *top    = undockedTopMargin;    }
    if (bottom != 0) { *bottom = undockedBottomMargin; }
}

inline VWindow *VDockableWindow::getLastDockSite() const
{
    return lastDockSite;
}

inline void VDockableWindow::enableDocking(boolean b)
{
	dockingCurrentlyDisabled = !b;
}

inline void VDockableWindow::disableDocking(boolean b)
{
	dockingCurrentlyDisabled = b;
}

inline boolean VDockableWindow::isDockingEnabled() const
{
	return !dockingCurrentlyDisabled;
}

inline boolean VDockableWindow::isDockingDisabled() const
{
	return dockingCurrentlyDisabled;
}

inline boolean VDockableWindow::getUndockedNoClose() const
{
    return undockedNoCloseBox;
}

inline void VDockableWindow::setOverDockSitePercentage(float f)
{
	overDockSitePercentage = f;
}

inline float VDockableWindow::getOverDockSitePercentage() const
{
	return overDockSitePercentage;
}

inline void VDockableWindow::setOverDockSitePointer(boolean b)
{
	overDockSitePointer = b;
}

inline boolean VDockableWindow::getOverDockSitePointer() const
{
	return overDockSitePointer;
}

inline void VDockableWindow::useConfigurationMenu(boolean b)
{
	configurationMenuDisabled = !b;
}

#endif // DOCKWIN_H
