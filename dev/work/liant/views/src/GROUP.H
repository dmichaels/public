// $Header:   Q:/views/win/vcs/group.h_v   1.11   Mar 04 1997 16:32:26   gregm  $ 

//	group.h
//
//	VGroup interface
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
//	05/14/93 pmcm	added destructor (virtual)
//	05/25/93 jmd	preened, removed free
//	06/21/93 pmcm	added support for VArchiver; preened;
//					added constructor with name argument;
//					int currentControlIndex for realize().
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	08/08/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	09/20/93 dgm	fix for bad background repainting of when we have focus;
//	09/20/93 dgm	fix for bad background repainting of when we have focus;
//					added updateBackground().
//	10/08/93 dgm	added clearCurrent().
//	01/19/96 dgm	Overrode getInternalBorderGeometry().
//	02/17/96 dgm	Overrode getPreferredSize().
//	04/19/96 glm	removed v1.1 code/preened
//	12/23/96 pkt	updated for Web platform.
//	---------------------------------------------------------------------------

#ifndef VGROUP_H
#define VGROUP_H

#include "control.h"
#include "ordcllct.h"

extern VClass *VGroupCls;

CLASS VControl;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif


//	Introduction
//
//	VGroup is a class representing an ordered collection of
// 	controls.  Focus is passed among the controls using TAB, HOME, 
//	BACK-TAB, END, and in some cases the arrow keys.
//

CLASS VGroup : public VControl {

public:
					VGroup();
					VGroup(	unsigned cntrlId, VWindow *pwin);
					VGroup(	const VString& wname,
						 	const VFrame& frame, 
							VWindow *pwin, char *title);
					VGroup(const VFrame& frame, 
							VWindow *pwin, char *title);
					~VGroup();

	VClass 		   *iam();
	enum WinType 	winType();

	// Interrogating the VControl objects

	VOrdCollect    *getControls();

	// Setting and Interrogating the current control

	int				getCurrent();
	virtual void 	setCurrent(int i);
	VControl 	   *getControl(int i);
	VControl 	   *getControl();

	void		 	clearCurrent();

	// Adding Controls

	virtual void 	addControl(VControl *cntrl);

	// Event Handling

	virtual boolean arrowKey(int i);
	boolean 		key(int k, char ch);
	VWindow		   *getFocusCandidate(int) const;
    void        	setChildFocus(VWindow *);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

    virtual void    getPreferredSize(int *, int *) const;
    virtual void    getInternalBorderGeometry(int *, int *,
                                              int *, int *) const;
#ifndef CV_REMOTE_GUI
    virtual UINT    getCtlColor();
	virtual boolean	has3dEffects() const;
    virtual boolean has3dEffects(V_3D_EFFECTS) const;
#endif

protected:
	VWindow		   *getFirstChild() const;
	VWindow		   *getLastChild() const;
	VWindow		   *getNextChild(VWindow * = 0) const;
	VWindow		   *getPrevChild(VWindow * = 0) const;
	int				getFirstChildIndex() const;
	int				getLastChildIndex() const;
	int				getNextChildIndex(VWindow * = 0) const;
	int				getPrevChildIndex(VWindow * = 0) const;

protected:

	VControl	   *current;  	// current control
	VOrdCollect  	controls; 	

	void			getInfo(VWinInfo& grpinfo);
	void			getMinSize(short& w, short& h);

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
	void			addControlsFromChildIndices();
#endif

private:

#ifndef CV_NOARCHIVER
	int				currentControlIndex;	//	(realization data)
	integer		   *controlChildIndices;	//	(realization data)
#endif

};

// ---------------------------------------------------------------------------
// Return the 'VOrdCollect' of VControl objects in this 'VGroup'.
//
inline VOrdCollect *VGroup::getControls()
{
	return &controls;
}
// ---------------------------------------------------------------------------
// Return the 'VControl' object in this 'VGroup' that is current 
// (that is, the one with focus).
//
inline VControl *VGroup::getControl()
{
	return current;
}
// ---------------------------------------------------------------------------
inline void VGroup::clearCurrent()
{
	current = 0;
}

#endif // VGROUP_H
