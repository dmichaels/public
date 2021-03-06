// $Header:   Q:/views/win/vcs/groupbox.h_v   1.14   May 30 1997 17:22:40   scottt  $
//
// groupbox.h
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
// Revision History:
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// 06/27/94 gjl   created
// 02/20/96 dgm   Update for Motif.
// 04/08/96 dgm   Fixed takeFocus() hiding compiler warning.
// 04/17/96 dgm   Removed default argument for takeFocus(boolean); oversight.
// 05/14/96 dgm   Motif support.
// 05/16/96 dgm   New constructors.
// 07/19/96 tlf   Added getText(VString &) member function.
// --------------------------------------------------------------------------

#ifndef VGROUPBOX_H
#define VGROUPBOX_H

#include "control.h"
#include "str.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VFrame;
CLASS	VString;
CLASS	VStyle;
CLASS	VWindow;
CLASS	VWinInfo;

#if defined(CV_MOTIF)
CLASS	VArgLst;
CLASS	VBrush;
CLASS	VFont;
#endif

// --------------------------------------------------------------------------
//
CLASS VGroupBox : public VControl {
public:
					VGroupBox(VWindow *, const char * = 0);
					VGroupBox(const VFrame&, VWindow *, const char * = 0);
					VGroupBox(VWindow *, const VStyle& = StyleDefault,
							  const char * = 0);
					VGroupBox(const VFrame&, VWindow *, 
							  const VStyle& = StyleDefault,
							  const char * = 0);
					VGroupBox(const VString&, const VFrame&, 
							  VWindow *, const VStyle& = StyleDefault,
							  const char *text = 0);
					VGroupBox();
				   ~VGroupBox();
	VClass		   *iam();

	void			operator()(const VFrame&, VWindow *);
	void			operator()(const VFrame&, VWindow *, const VStyle&);
	void			operator()(const VFrame&, VWindow *, const VStyle&,
							   const VString&);

	VGroupBox&		operator=(const char *);

#if defined(CV_MOTIF)
	int			getText(VString&);
#endif

	virtual void	putText(const VString&);

    virtual void    getPreferredSize(int *, int *) const;
    virtual void    getInternalBorderGeometry(int *, int *,
                                              int *, int *) const;
#if defined(CV_WINDOWS)
    virtual UINT    getCtlColor();
	virtual boolean	has3dEffects() const;
    virtual boolean has3dEffects(V_3D_EFFECTS) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#if !defined(CV_MOTIF)
   virtual WinType	winType();
   virtual boolean	erased();
#endif

	virtual boolean	key(int, char);

protected:
	void			initialize();
	void			construct();
	VWindow		   *getFirstChild() const;
	VWindow		   *getLastChild() const;
	VWindow		   *getNextChild(VWindow * = 0) const;
	VWindow		   *getPrevChild(VWindow * = 0) const;
	virtual void	getInfo(VWinInfo&);
#if defined(CV_MOTIF)
	virtual boolean	create(HWND, VArgLst&);
	virtual boolean	ownsWidget(Widget, Widget * = 0);
	virtual Widget	getTopWidget() const;
	virtual void	showBool(boolean, boolean);
	virtual void	virtSetupBackground(VBrush *);
	virtual void	virtSetupFont(VFont *);
    virtual void    getSystemBorderSize(int *, int *) const;
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section; please KEEP OUT!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
private:
#if defined(CV_MOTIF)
	HWND			frameWidget;
	HWND			labelWidget;
#endif
};

extern VClass *VGroupBoxCls;

// --------------------------------------------------------------------------
//
inline VGroupBox& VGroupBox::operator=(const char *s)
{
	putText(s);
	return *this;
}

#endif // VGROUPBOX_H
