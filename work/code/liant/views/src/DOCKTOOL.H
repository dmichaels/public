// $Header:   Q:/views/common/vcs/docktool.h_v   1.14   25 Feb 1997 10:39:48   tomf  $

//	docktool.h
//
//	VDockableToolBar interface [Common]
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
//	Revision History:
//	-----------------
//	11/01/95 dgm	Original.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Changed setSpacing(), setGroupSpacing(), and beginGroup()
//					to take an optional minor spacing parameter.
//					Added set/getMinorSpacing()
//					and set/getMinorGroupSpacing().
//					Added getDockedSize().
//	11/29/95 dgm	Added beginDrag()/endDrag() & docked()/undocked().
//	11/29/95 dgm	Added enableBool().
//	11/30/95 dgm	Added isEnabled() and isDisabled().
//	02/06/96 dgm	Made set(Un)DockedAttributes() virtual.
//	03/05/96 dgm	Publicized enableBool().
//	03/21/96 dgm	Archiver support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef DOCKTOOL_H
#define DOCKTOOL_H

// --------------------------------------------------------------------------
//
#include "dockwin.h"
#include "toolbar.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VDockableWindow;
CLASS	VString;
CLASS	VStyle;
CLASS	VToolBar;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS   VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VDockableToolBar : public VDockableWindow {
public:
	enum ShadeStyle {	NoShade,
						RaisedShade,
						SunkenShade };

public:
						VDockableToolBar(VWindow *,
									     const VStyle& = StyleDefault);
						VDockableToolBar(const VString&,
										 VWindow *,
									     const VStyle& = StyleDefault);
						VDockableToolBar(const VFrame&, VWindow *,
									     const VStyle& = StyleDefault);
						VDockableToolBar(const VString&,
										 const VFrame&, VWindow *,
									     const VStyle& = StyleDefault);
						VDockableToolBar();
					   ~VDockableToolBar();

    void	            operator()(VWindow *);
    void	            operator()(VWindow *, const VStyle&);
    void	            operator()(VWindow *, const VStyle&,
								   const VString&);
    void	            operator()(const VFrame&, VWindow *);
    void	            operator()(const VFrame&, VWindow *, const VStyle&);
    void	            operator()(const VFrame&, VWindow *, const VStyle&,
								   const VString&);
	VClass			   *iam();

	boolean				dock(VWindow *docksite = 0);
	boolean				undock(int x = 32767, int y = 32767);

	// VToolBar dispatch.

	void				setRow();
	void				setColumn();

	boolean				isRow() const;
	boolean				isColumn() const;

	void				setWrapping(boolean);
	boolean				getWrapping() const;

	void				setSpacing(int, int = -1);
	int					getSpacing() const;

	void				setMinorSpacing(int);
	int					getMinorSpacing() const;

	void				setMargins(int);
	void				setMargins(int, int);
	void				setMargins(int, int, int, int);
	void				getMargins(int *, int *, int *, int *);

	VGeoRowOrColumn	   *beginGroup(int = -1, int = -1);
	void				endGroup();

	void				setGroupSpacing(int, int = -1);
	int					getGroupSpacing() const;

	void				setMinorGroupSpacing(int);
	int					getMinorGroupSpacing() const;

	void				addSeparator(int = 0);
	void				addSpace(int = 0);

	void				setShadeStyle(int);
	int					getShadeStyle() const;

	VToolBar		   *getToolBar() const;

    boolean			    isEnabled() const;
    boolean			    isDisabled() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	//
	// N.B. There are sections below which are specified as public,
	// however, they are conceptually protected in that only derived
	// classes are meant to use them in their implementations, and they
	// should only be used as such.  The reason why these conceptually
	// protected sections need to actually be public are compiler bugs
	// which prevent the use of non-public nested types.
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
public:

#ifndef CV_NOARCHIVER
    virtual void	        putTo(VArchiver&);
    virtual void	        getFrom(VArchiver&);
    virtual void	        getFrom(VArchiver&, VObject *, boolean = TRUE);
    virtual void	        realize(VWindow *);
#endif

protected:
	virtual VToolBar	   *newToolBar(VWindow *);
	virtual VUtilityWindow *newUndockedWindow(VWindow *);
	virtual VWindow		   *newClientWindow(VWindow *);
	virtual void			getDockedSize(int *, int *,
										  int, int, VWindow *) const;
	virtual void			beginDrag();
	virtual void			endDrag();
	virtual boolean			docked(VWindow *);
	virtual boolean			undocked(VWindow *);
	virtual void			setDockedAttributes(VWindow *);
	virtual void			setUndockedAttributes();
protected:
	virtual boolean			mouseDn(int, int);
	virtual boolean			mouseDbl(int, int);
	virtual boolean			rmouseDn(int, int);
#ifndef CV_NOARCHIVER
    void	                putTo(VArchiver&, boolean);
#endif
public:
    virtual void		    enableBool(boolean);
protected:
	void					initialize();
	void					construct();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	VToolBar			   *toolBar;
	short					explicitShading : 1;
	short					explicitWrapping : 1;
};

extern VClass *VDockableToolBarCls;

// --------------------------------------------------------------------------
//
inline void VDockableToolBar::setRow()
{
	if (toolBar != 0) {
		toolBar->setRow();
	}
}

inline void VDockableToolBar::setColumn()
{
	if (toolBar != 0) {
		toolBar->setColumn();
	}
}

inline boolean VDockableToolBar::isRow() const
{
	return toolBar != 0 ? toolBar->isRow() : 0;
}

inline boolean VDockableToolBar::isColumn() const
{
	return toolBar != 0 ? toolBar->isColumn() : 0;
}

inline boolean VDockableToolBar::getWrapping() const
{
	return toolBar != 0 ? toolBar->getWrapping() : 0;
}

inline void VDockableToolBar::setSpacing(int major, int minor)
{
	if (toolBar != 0) {
		toolBar->setSpacing(major, minor);
	}
}

inline int VDockableToolBar::getSpacing() const
{
	return toolBar != 0 ? toolBar->getSpacing() : 0;
}

inline void VDockableToolBar::setMinorSpacing(int i)
{
	if (toolBar != 0) {
		toolBar->setMinorSpacing(i);
	}
}

inline int VDockableToolBar::getMinorSpacing() const
{
	return toolBar != 0 ? toolBar->getMinorSpacing() : 0;
}

inline void VDockableToolBar::setMargins(int all)
{
	if (toolBar != 0) {
		toolBar->setMargins(all);
	}
}

inline void VDockableToolBar::setMargins(int major, int minor)
{
	if (toolBar != 0) {
		toolBar->setMargins(major, minor);
	}
}

inline void VDockableToolBar::setMargins(int major_near, int major_far,
										 int minor_near, int minor_far)
{
	if (toolBar != 0) {
		toolBar->setMargins(major_near, major_far, minor_near, minor_far);
	}
}

inline void VDockableToolBar::getMargins(int *major_near, int *major_far,
										 int *minor_near, int *minor_far)
{
	if (toolBar != 0) {
		toolBar->getMargins(major_near, major_far, minor_near, minor_far);
	}
}

inline VGeoRowOrColumn *VDockableToolBar::beginGroup(int major, int minor)
{
	return toolBar != 0 ? toolBar->beginGroup(major, minor) : 0;
}

inline void VDockableToolBar::endGroup()
{
	if (toolBar != 0) {
		toolBar->endGroup();
	}
}

inline void VDockableToolBar::setGroupSpacing(int major, int minor)
{
	if (toolBar != 0) {
		toolBar->setGroupSpacing(major, minor);
	}
}

inline int VDockableToolBar::getGroupSpacing() const
{
	return toolBar != 0 ? toolBar->getGroupSpacing() : 0;
}

inline void VDockableToolBar::setMinorGroupSpacing(int minor)
{
	if (toolBar != 0) {
		toolBar->setMinorGroupSpacing(minor);
	}
}

inline int VDockableToolBar::getMinorGroupSpacing() const
{
	return toolBar != 0 ? toolBar->getMinorGroupSpacing() : 0;
}

inline void VDockableToolBar::addSeparator(int space)
{
	if (toolBar != 0) {
		toolBar->addSeparator(space);
	}
}

inline void VDockableToolBar::addSpace(int space)
{
	if (toolBar != 0) {
		toolBar->addSpace(space);
	}
}

inline void VDockableToolBar::setShadeStyle(int i)
{
	if (toolBar != 0) {
		explicitShading = 1;
		toolBar->setShadeStyle(i);
	}
}

inline int VDockableToolBar::getShadeStyle() const
{
	return toolBar != 0 ? toolBar->getShadeStyle() : 0;
}

inline VToolBar *VDockableToolBar::getToolBar() const
{
	return toolBar;
}

inline boolean VDockableToolBar::isEnabled() const
{
	return toolBar != 0 ? toolBar->isEnabled() : TRUE;
}

inline boolean VDockableToolBar::isDisabled() const
{
	return toolBar != 0 ? toolBar->isDisabled() : FALSE;
}

#endif // DOCKTOOL_H
