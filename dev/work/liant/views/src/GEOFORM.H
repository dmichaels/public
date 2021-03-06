// $Header:   Q:/views/common/vcs/GEOFORM.H_V   1.9   Mar 24 1997 16:09:04   scottt  $

//	geoform.h
//
//	VGeoForm interface [Common]
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
//	09/29/95 dgm	Original.
//	10/02/95 dgm	Updated.
//	10/04/95 dgm	Updated.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	12/05/95 dgm	Changes for UNIX DEC/Alpha-C++ compiler.
//	12/14/95 dgm	Minor #ifdefs for compiler bugs.
//	12/15/95 tlf	Fix minor #ifdefs for compiler bugs.
//	01/29/96 dgm	Size-group changes.
//	02/06/96 dgm	Overrode frameChildrenOk().
//	02/27/96 dgm	Added extra check in getSibling().
//	01/21/97 dgm	Added oldAttachData.
// --------------------------------------------------------------------------

#ifndef VGEOFORM_H
#define VGEOFORM_H

// --------------------------------------------------------------------------
//
#include "geomgr.h"

#ifndef CV_NO_OLD_GEO
#include "attach.h"	
#endif

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VGeoFrame;
CLASS VGeoManager;
CLASS VObject;
CLASS VWindow;

#if !defined(CV_NOARCHIVER)
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VGeoForm : public VGeoManager {
public:
	enum Side {			Top			= 0,
						Bottom		= 1,
						Left		= 2,
						Right		= 3 };

	enum Type {			UnAttached	= 0,
						Parent		= 1,
						Sibling		= 2,
						SiblingSame	= 3,
						Frame		= 4 };

	CLASS Attachment {
	public:
						Attachment();
					   ~Attachment();
		Type			getType(Side) const;
		int				getOffset(Side) const;
		VGeoFrame	   *getSibling(Side) const;
#if !defined(CV_NOARCHIVER)
		virtual void	putTo(VArchiver&);
		virtual void	getFrom(VArchiver&);
#endif
	private:
		void			set(Side, Type, short = 0, VGeoFrame * = 0);
		void			setType(Side, Type);
		void			setOffset(Side, int);
		void			setSibling(Side, VGeoFrame *);
	private:
		union {
			VGeoFrame  *theSibling[4];
			int			theSiblingIndex[4];
		};
		short			theOffset[4];
		char			theType[4];
#ifndef CV_NO_OLD_GEO
		VAttach		   *oldAttachData;
#endif
	private:
		friend			VGeoForm;
	};

public:
						VGeoForm(const VFrame&, VGeoManager *);
						VGeoForm(int x, int y, int w, int h, VGeoManager *);
						VGeoForm(int w, int h, VGeoManager *);
						VGeoForm(VGeoManager *);
						VGeoForm(const VFrame&, VWindow *);
						VGeoForm(int x, int y, int w, int h, VWindow *);
						VGeoForm(int w, int h, VWindow *);
						VGeoForm(VWindow *);
						VGeoForm();
					   ~VGeoForm();
	VClass			   *iam();

	boolean				attachChild(VWindow *child_window,
								    Side, Type,
									VWindow *sibling_window,
									int offset = 0);
	boolean				attachChild(VWindow *child_window,
								    Side, Type,
									int offset = 0);
	boolean				attachChild(VGeoFrame *child,
								    Side, Type,
									int offset = 0);
	boolean				attachChild(VWindow *child_window,
								    Side, Type,
									VGeoFrame *sibling,
									int offset = 0);
	boolean				attachChild(VGeoFrame *child,
								    Side, Type,
									VWindow *sibling_window,
									int offset = 0);
	boolean				attachChild(VGeoFrame *child,
								    Side, Type,
									VGeoFrame *sibling,
									int offset = 0);
	void				unattachChild(VGeoFrame *child, Side);
	void				unattachChild(VGeoFrame *child);
	void				unattachChild(VWindow *child, Side);
	void				unattachChild(VWindow *child);
	void				detachChild(VGeoFrame *child);
	Attachment		   *getAttachments(VGeoFrame *child) const;

#ifndef CV_NO_OLD_GEO
	static boolean		oldAttachWin(VWindow *, VWindow *,
									 VAttach::Side, VAttach::Type,
									 int, VWindow *);
	static boolean		oldSetAttach(VWindow *,
									 VAttach::Side, VAttach::Type,
									 int, VWindow *);
	static VAttach	   *oldGetAttach(const VWindow *);
	static void			oldDetachWin(VWindow *, VWindow *);
	static boolean		oldToNewArchiveConversion(VWindow *);
#endif

#if !defined(CV_NOARCHIVER)
	void				putTo(VArchiver&);
	void				getFrom(VArchiver&, VObject *, boolean = TRUE);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	// N.B. There are sections below which are specified as public,
	// however, they are conceptually protected in that only derived
	// classes are meant to use them in their implementations, and
	// they should only be used as such.  There are two reasons why
	// these conceptually protected sections need to actually be public:
	//
	// o Derived classes typically need to access members through
	//   their child VGeoFrame objects rather than through themselves.
	// o Compiler bugs which prevent the use of non-public nested types.
	//
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:
	virtual void		getPreferredSize(int *w, int *h);
	virtual boolean		getConstrainedSize(int *w, int *h, int, int);
	virtual void		getMinimumSize(int *w, int *h);
	virtual void		frameChildren(int w = 0, int h = 0);
	virtual boolean		frameChildrenOk(int w = 0, int h = 0);
	virtual void		childUpdated(VGeoFrame *);
	virtual void		childRemoved(VGeoFrame *);

private:
						VGeoForm(const VGeoForm&);
	VGeoForm&			operator=(const VGeoForm&);

protected:
	void				initialize();
	void				getPreferredSizeWithChildren(int *, int *,
													 boolean = FALSE,
													 int = 0, int = 0);
	int					setupChild(VGeoFrame *, boolean = FALSE, int = -1);
	int					setupChildGeometry(VGeoFrame *, int,
										   boolean, boolean, boolean,
										   boolean&);
	virtual void		childReplaced(VGeoFrame *, VGeoFrame *);

#if !defined(CV_NOARCHIVER)
    virtual void		putTo(VArchiver&, boolean);
	virtual void		getFrom(VArchiver&);
	virtual void		gotChildren();
#endif

public:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Each VGeoFrame which is a child of a VGeoForm will have a
	// FormChildData object which will be pointed to by its VGeoFrame::
	// childData member.  This contains the per-child information needed
	// by the parent VGeoForm manager.
	//
	CLASS FormChildData : public VGeoManager::ManagerChildData {
	public:
									FormChildData(const VGeoManager *);
		virtual					   ~FormChildData();
#if !defined(CV_NOARCHIVER)
		virtual void				putTo(VArchiver&);
		virtual void				getFrom(VArchiver&);
#endif
	public:
		Attachment				   *theAttachments;
		GeoComponent				leftTop[2];
		GeoComponent				widthHeight[2];
		GeoComponent::Dependency	rightBottomDep[2];
		char						calculatingHorizontal : 1;
		char						calculatingVertical : 1;
		char						calculatingOtherDimension : 1;
		char						_reservedA : 1;
	};

protected:
	virtual VGeoFrame::ChildData   *newChildData() const;

private:
	short							resizingDimension[2];
};

extern VClass *VGeoFormCls;

// --------------------------------------------------------------------------
//
inline void VGeoForm::initialize()
{
	isGeoForm = 1;
	resizingDimension[0] = 0;
	resizingDimension[1] = 0;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoForm::attachChild(VWindow *child_window,
									 VGeoForm::Side side,
									 VGeoForm::Type type,
									 VWindow *sibling_window,
									 int offset)
{
	return attachChild(child_window != 0 ?
					   child_window->getGeoFrame() : 0,
					   side, type,
					   sibling_window != 0 ?
					   sibling_window->getGeoFrame() : 0,
					   offset);
}

// --------------------------------------------------------------------------
//
inline boolean VGeoForm::attachChild(VWindow *child_window,
									 VGeoForm::Side side,
									 VGeoForm::Type type,
									 int offset)
{
	return attachChild(child_window != 0 ?
					   child_window->getGeoFrame() : 0,
					   side, type,
					   (VGeoFrame *)0,
					   offset);
}

// --------------------------------------------------------------------------
//
inline boolean VGeoForm::attachChild(VGeoFrame *child,
									 VGeoForm::Side side,
									 VGeoForm::Type type,
									 int offset)
{
	return attachChild(child,
					   side, type,
					   (VGeoFrame *)0,
					   offset);
}

// --------------------------------------------------------------------------
//
inline boolean VGeoForm::attachChild(VWindow *child_window,
									 VGeoForm::Side side,
									 VGeoForm::Type type,
									 VGeoFrame *sibling,
									 int offset)
{
	return attachChild(child_window != 0 ?
					   child_window->getGeoFrame() : 0,
					   side, type,
					   sibling,
					   offset);
}

// --------------------------------------------------------------------------
//
inline boolean VGeoForm::attachChild(VGeoFrame *child,
									 VGeoForm::Side side,
									 VGeoForm::Type type,
									 VWindow *sibling_window,
									 int offset)
{
	return attachChild(child,
					   side, type,
					   sibling_window != 0 ?
					   sibling_window->getGeoFrame() : 0,
					   offset);
}

// --------------------------------------------------------------------------
//
inline VGeoForm::Attachment::Attachment()
{
	theType[0]		= UnAttached;
	theType[1]		= UnAttached;
	theType[2]		= UnAttached;
	theType[3]		= UnAttached;
	theOffset[0]	= 0;
	theOffset[1]	= 0;
	theOffset[2]	= 0;
	theOffset[3]	= 0;
	theSibling[0]	= 0;
	theSibling[1]	= 0;
	theSibling[2]	= 0;
	theSibling[3]	= 0;
#ifndef CV_NO_OLD_GEO
	oldAttachData	= 0;
#endif
}

// --------------------------------------------------------------------------
//
inline VGeoForm::Attachment::~Attachment()
{
#ifndef CV_NO_OLD_GEO
	delete oldAttachData;
#endif
}

// --------------------------------------------------------------------------
//
inline void VGeoForm::Attachment::set(VGeoForm::Side side,
									  VGeoForm::Type type,
									  short offset,  VGeoFrame *sibling)
{
	if ((side >= 0) && (side <= 3)) {
		theType[side] = type;
		theOffset[side] = offset;
		theSibling[side] = sibling;
	}
}

// --------------------------------------------------------------------------
//
inline void VGeoForm::Attachment::setType(VGeoForm::Side side,
										  VGeoForm::Type type)
{
	if ((side >= 0) && (side <= 3)) {
		theType[side] = char(type);
	}
}

// --------------------------------------------------------------------------
//
inline void VGeoForm::Attachment::setOffset(VGeoForm::Side side, int offset)
{
	if ((side >= 0) && (side <= 3)) {
		theOffset[side] = short(offset);
	}
}

// --------------------------------------------------------------------------
//
inline void VGeoForm::Attachment::
			setSibling(VGeoForm::Side side, VGeoFrame *sibling)
{
	if ((side >= 0) && (side <= 3)) {
		theSibling[side] = sibling;
	}
}

// --------------------------------------------------------------------------
//
inline VGeoForm::Type VGeoForm::Attachment::getType(VGeoForm::Side side) const
{
	if ((side >= 0) && (side <= 3)) {
		return Type(theType[side]);
	}
	else {
		return UnAttached;
	}
}

// --------------------------------------------------------------------------
//
inline int VGeoForm::Attachment::getOffset(VGeoForm::Side side) const
{
	if ((side >= 0) && (side <= 3)) {
		return theOffset[side];
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
inline VGeoFrame *VGeoForm::Attachment::getSibling(VGeoForm::Side side) const
{
	if (((side >= 0) && (side <= 3)) &&
		((theType[side] == Sibling) || (theType[side] == SiblingSame))) {
		return theSibling[side];
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
inline VGeoForm::FormChildData::FormChildData(const VGeoManager *parent)
#if defined(CV_WINDOWS) && (_MSC_VER <= 800)
	: ManagerChildData(parent)
#else
	: VGeoManager::ManagerChildData(parent)
#endif
{
	theAttachments = 0;
	calculatingHorizontal = 0;
	calculatingVertical = 0;
	calculatingOtherDimension = 0;
}

#endif // VGEOFORM_H
