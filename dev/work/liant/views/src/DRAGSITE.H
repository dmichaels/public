//	dragsite.h -- [UNDER DEVELOPMENT]
//
//	VDragSite interface
//
//	Allegris Foundation 1.0.03
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

#ifndef VDRAGSITE_H
#define VDRAGSITE_H

// --------------------------------------------------------------------------
//
#include "object.h"

// --------------------------------------------------------------------------
//
CLASS	VClass;
CLASS	VDragSiteImp;
CLASS	VMouseCursor;
CLASS	VObject;
CLASS	VObjectCourier;
CLASS	VNotifier;
CLASS	VWindow;

// --------------------------------------------------------------------------
//
CLASS VDragSite : public VObject {
public:
								VDragSite();
								VDragSite(const VDragSite&);
							   ~VDragSite();
	VClass					   *iam();

	void						uponDragStart(VObject *, method);
	void						uponDropSiteFeedback(VObject *, method);

	void						setCopyMouseCursor(VMouseCursor *);
	void						setMoveMouseCursor(VMouseCursor *);
	void						setLinkMouseCursor(VMouseCursor *);
	void						setNoMouseCursor(VMouseCursor *);
	void						setInvalidMouseCursor(VMouseCursor *);

	VWindow					   *getWindow() const;

	boolean						registerWindow(VWindow *);
	void						unregisterWindow(VWindow *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Protected section; please keep out if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

protected:
	virtual VObjectCourier	   *dragStart(VWindow *, int x, int y);
	virtual boolean				dropSiteFeedback(long);
	virtual boolean				transferStart(VObjectCourier *);
	virtual void				transferEnd(VObjectCourier *);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Private section; for internal use only!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

private:
	boolean						dragLoop(VObjectCourier *);
	VMouseCursor			   *getCopyMouseCursor() const;
	VMouseCursor			   *getMoveMouseCursor() const;
	VMouseCursor			   *getLinkMouseCursor() const;
	VMouseCursor			   *getNoMouseCursor() const;
	VMouseCursor			   *getInvalidMouseCursor() const;

private:
	VWindow					   *theWindow;
	VObject					   *clientObject;
	VObjectCourier  *(VObject::*dragStartMethod)(VWindow *, int, int);
	boolean			 (VObject::*dropSiteFeedbackMethod)(long);
	VMouseCursor			   *copyMouseCursor;
	VMouseCursor			   *moveMouseCursor;
	VMouseCursor			   *linkMouseCursor;
	VMouseCursor			   *noMouseCursor;
	VMouseCursor			   *invalidMouseCursor;
	VDragSiteImp			   *dragSiteImp;

private:
	friend						VDragSiteImp;
	friend						VNotifier;
	friend						VWindow;
};

extern VClass *VDragSiteCls;

#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
inline void VDragSite::setCopyMouseCursor(VMouseCursor *mc)
{
	copyMouseCursor = mc;
}

// --------------------------------------------------------------------------
//
inline void VDragSite::setMoveMouseCursor(VMouseCursor *mc)
{
	moveMouseCursor = mc;
}

// --------------------------------------------------------------------------
//
inline void VDragSite::setLinkMouseCursor(VMouseCursor *mc)
{
	linkMouseCursor = mc;
}

// --------------------------------------------------------------------------
//
inline void VDragSite::setNoMouseCursor(VMouseCursor *mc)
{
	noMouseCursor = mc;
}

// --------------------------------------------------------------------------
//
inline void VDragSite::setInvalidMouseCursor(VMouseCursor *mc)
{
	invalidMouseCursor = mc;
}

// --------------------------------------------------------------------------
//
inline VWindow *VDragSite::getWindow() const
{
	return theWindow;
}

// --------------------------------------------------------------------------
//
inline VMouseCursor *VDragSite::getCopyMouseCursor() const
{
	return copyMouseCursor;
}

// --------------------------------------------------------------------------
//
inline VMouseCursor *VDragSite::getMoveMouseCursor() const
{
	return moveMouseCursor;
}

// --------------------------------------------------------------------------
//
inline VMouseCursor *VDragSite::getLinkMouseCursor() const
{
	return linkMouseCursor;
}

// --------------------------------------------------------------------------
//
inline VMouseCursor *VDragSite::getNoMouseCursor() const
{
	return noMouseCursor;
}

// --------------------------------------------------------------------------
//
inline VMouseCursor *VDragSite::getInvalidMouseCursor() const
{
	return invalidMouseCursor;
}

#else // !CV_OLE_DRAG_DROP || CV_WIN16

inline VDragSite::VDragSite() { }
inline VDragSite::VDragSite(const VDragSite&) { }
inline VDragSite::~VDragSite() { }
inline void VDragSite::uponDragStart(VObject *, method) { }
inline void VDragSite::uponDropSiteFeedback(VObject *, method) { }
inline void VDragSite::setCopyMouseCursor(VMouseCursor *) { }
inline void VDragSite::setMoveMouseCursor(VMouseCursor *) { }
inline void VDragSite::setLinkMouseCursor(VMouseCursor *) { }
inline void VDragSite::setNoMouseCursor(VMouseCursor *) { }
inline void VDragSite::setInvalidMouseCursor(VMouseCursor *) { }
inline VWindow *VDragSite::getWindow() const { return 0; }
inline boolean VDragSite::registerWindow(VWindow *) { return 0; }
inline void VDragSite::unregisterWindow(VWindow *) { }
inline VObjectCourier *VDragSite::dragStart(VWindow *, int, int) { return 0; }
inline boolean VDragSite::dropSiteFeedback(long) { return 0; }
inline boolean VDragSite::transferStart(VObjectCourier *) { return 0; }
inline void VDragSite::transferEnd(VObjectCourier *) { }
inline boolean VDragSite::dragLoop(VObjectCourier *) { return 0; }
inline VMouseCursor *VDragSite::getCopyMouseCursor() const { return 0; }
inline VMouseCursor *VDragSite::getMoveMouseCursor() const { return 0; }
inline VMouseCursor *VDragSite::getLinkMouseCursor() const { return 0; }
inline VMouseCursor *VDragSite::getNoMouseCursor() const { return 0; }
inline VMouseCursor *VDragSite::getInvalidMouseCursor() const { return 0; }

#endif // !CV_OLE_DRAG_DROP || CV_WIN16

#endif // VDRAGSITE_H
