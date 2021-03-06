// $Header:   Q:/views/win/vcs/iconbttn.h_v   1.7   Mar 04 1997 16:32:30   gregm  $ 

//	iconbttn.h
//
//	VIconButton class interface
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
//	05/13/93 pat	added constructor that takes name argument;
//	              	added getFrom(), putTo(), realize();
//					added reference to VArchiver.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/09/93 jmd	made VFrame const
//	08/09/93 jmd	removed free
//	09/23/93 pat 	added constructor that takes name argument; preened.
//	10/13/93 jmd	added inline to inline functions (thanks uwe)
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#ifndef VICONBTTN_H
#define VICONBTTN_H

#include "pushbttn.h"
#include "icon.h"
#include "shade.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

//	Introduction
//
//	The VIconButton class is designed to support graphical push buttons in
//	an application. VIconButton objects have the same 3-D background as
//	VPushButton objects, but they differ in that they have an icon instead
//	of a text label on their surface. VIconButtons are most useful for
//	building button ribbons and tool bars in applications. An VIconButton
//	object requires an VIcon object to draw on its surface. The VIcon
//	object can be provided directly with a pointer to an instance of the
//	VIcon class, or indirectly with a name of an icon resource. If an icon
//	resource name is provided, then the VIconButton will manage the
//	construction and destruction of the VIcon object that it uses for
//	drawing the button. Otherwise, the user of the VIconButton is
//	responsible for the disposition of the VIcon object once the
//	VIconButton is destroyed.

CLASS VIconButton : public VPushButton {

public:
					VIconButton(const VFrame& frame, 
								VIcon *ic, 
								VWindow *pwin, 
								const VStyle& style);

					VIconButton(const VFrame& frame, 
								char *icName, 
								VWindow *pwin, 
								const VStyle& style);

					VIconButton(const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleDefault);


					VIconButton(const VString& name,
								const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleDefault);

					VIconButton(unsigned cntrlId, VWindow *pwin);
					VIconButton();

					~VIconButton();

	VClass 		   *iam();

	VIcon 		   *getIcon(VIcon *ic) const;
	void 			setIcon(VIcon *);
	void 			setIcon(char *);
	void 			setDefault(boolean);
	int 			getBevelThickness();
	void 			setBevelThickness(int bt);

	boolean 		paint();

	//	Archival and Retrieval

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin, 
									boolean doRealize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

    virtual void    systemColorChanged();

protected:

	VIcon 		   *bIcon;
	VShade 			bkShade;
	boolean 		internal;

	void 			getInfo(VWinInfo &);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

};

// ---------------------------------------------------------------------------
//
inline VIcon *VIconButton::getIcon(VIcon *ic) const
{ 
	return(bIcon); 
}

// ---------------------------------------------------------------------------
//
inline int VIconButton::getBevelThickness()
{ 
	return(bkShade.getBevelThickness()); 
}

// ---------------------------------------------------------------------------
//
inline void VIconButton::setBevelThickness(int bt) 
{ 
	bkShade.setBevelThickness(bt); 
}

// ---------------------------------------------------------------------------

extern VClass *VIconButtonCls;

#endif 	//	VICONBTTN_H
