// $Header:   Q:/views/common/vcs/docksite.h_v   1.10   May 30 1997 17:15:04   scottt  $

//	docksite.h
//
//	VDockSiteWindow interface [Common]
//
//	Allegris Foundation 1.1.00
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
//	11/01/95 dgm	Original.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/30/95 dgm	Added undocked()/docked().
//					Changed uponDocked()/uponUndocked().
//	12/05/95 dgm	Changed to use VColorScheme::systemXXX().
//	04/02/96 dgm	Archiver support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef DOCKSITE_H
#define DOCKSITE_H

// --------------------------------------------------------------------------
//
#include "window.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VDockableWindow;
CLASS	VFrame;
CLASS	VString;
CLASS	VStyle;
CLASS	VUtilityWindow;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS   VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VDockSiteWindow : public VWindow {
public:
	enum ShadeStyle {
						NoShade,
						RaisedShade,
						SunkenShade
	};

public:
						VDockSiteWindow(const VFrame&, VWindow *,
									    const VStyle& = StyleDefault);
						VDockSiteWindow(VWindow *,
									    const VStyle& = StyleDefault);
						VDockSiteWindow(const VString&,
										const VFrame&, VWindow *,
									    const VStyle& = StyleDefault);
						VDockSiteWindow(const VString&,
										VWindow *,
									    const VStyle& = StyleDefault);
						VDockSiteWindow();
					   ~VDockSiteWindow();
	VClass			   *iam();

    void	            operator()(const VFrame&, VWindow *);
    void	            operator()(const VFrame&, VWindow *, const VStyle&);
    void	            operator()(const VFrame&, VWindow *, const VStyle&,
								   const VString&);

	void				uponDocked(VObject *, method);
	void				uponUndocked(VObject *, method);

	int					getDockedCount() const;

	void				setShadeStyle(int);
	int					getShadeStyle() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#ifndef CV_NOARCHIVER
public:
    virtual void        putTo(VArchiver&);
    virtual void        getFrom(VArchiver&);
    virtual void        getFrom(VArchiver&, VObject *, boolean = TRUE);
    virtual void        realize(VWindow *);
protected:
    void                putTo(VArchiver&, boolean);
#endif

protected:
	virtual void		docked(VDockableWindow *);
	virtual void		undocked(VDockableWindow *);

protected:
	virtual boolean		mouseDn(int, int);
	virtual boolean		mouseDbl(int, int);
	virtual boolean		rmouseDn(int, int);
	virtual boolean		paint();
    virtual void		getInternalBorderGeometry(int *, int *,
												  int *, int *) const;
protected:
	void				initialize();
	void				construct();
	void				setup();
protected:
	static void			constructStaticData();
	static void			deleteStaticData();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VObject			   *clientObject;
	void	 (VObject::*dockedMethod)(VDockSiteWindow *, VDockableWindow *);
	void	 (VObject::*undockedMethod)(VDockSiteWindow *, VDockableWindow *);
	char				shadeStyle;

#ifndef CV_NOARCHIVER
	integer				clientObjectIndex;
	integer				dockedMethodIndex;
	integer				undockedMethodIndex;
#endif

private:
	friend				VDockableWindow;
};

extern VClass *VDockSiteWindowCls;

// --------------------------------------------------------------------------
//
inline int VDockSiteWindow::getShadeStyle() const
{
	return shadeStyle;
}

#endif // DOCKSITE_H
