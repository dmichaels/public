// $Header:   Q:/views/common/vcs/GEOMGR.H_V   1.16   Mar 24 1997 16:09:10   scottt  $

//	geomgr.h
//
//	VGeoManager interface [Common]
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
//	10/20/95 dgm	Original.
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/20/95 dgm	Removed addInternalBorderSize() and
//					addNonClientAreaSize(int *, int *).
//					Added beginFrameChildren() and endFrameChildren().
//	11/28/95 dgm	Added subtractInternalBorderSize().
//	12/14/95 dgm	Minor #ifdefs for compiler bugs.
//	12/15/95 tlf	Fix minor #ifdefs for compiler bugs.
//	01/29/96 dgm	Size-group changes.
//	02/06/96 dgm	Added frameChildrenOk() for internal use only.
//	02/16/96 dgm	Support to query VWindow associated with VGeoManager.
//	02/23/96 dgm	SizeGroup changes.
//	08/26/96 dgm	Changed update() to take an int rather than a boolean.
//	04/24/97 dgm	Added set/getForceFrameChildren().
// ---------------------------------------------------------------------------

#ifndef VGEOMGR_H
#define VGEOMGR_H

// --------------------------------------------------------------------------
//
#include "geoframe.h"
#include "ordcllct.h"
#include "iterator.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VGeoFrame;
CLASS VObject;
CLASS VOrdCollect;
CLASS VWindow;

#if !defined(CV_NOARCHIVER)
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VGeoManager : public VGeoFrame {
public:
						VGeoManager(const VFrame&, VGeoManager *parent);
						VGeoManager(int x, int y, int w, int h,
									VGeoManager *parent);
						VGeoManager(int w, int h, VGeoManager *parent);
						VGeoManager(VGeoManager *parent);
						VGeoManager(const VFrame&, VWindow *parent_window);
						VGeoManager(int x, int y, int w, int h,
									VWindow *parent_window);
						VGeoManager(int w, int h, VWindow *parent_window);
						VGeoManager(VWindow *parent_window);
						VGeoManager();
					   ~VGeoManager();
	VClass			   *iam();

	void				resized(int, int);

	virtual void		update(int = FALSE);

	VOrdCollect		   *getChildren() const;
	VGeoFrame		   *getChild(int) const;
	int					getChildIndex(const VGeoFrame *) const;
	int					findChild(VGeoFrame *) const;
	int					getNumberOfChildren() const;
	boolean				hasChildren() const;

	VGeoFrame		   *addChild(VGeoFrame *);
	VGeoFrame		   *addChild(VWindow *);
	boolean				removeChild(VGeoFrame *);
	boolean				removeChild(VWindow *);
	boolean				replaceChild(VGeoFrame *, VGeoFrame *);
	boolean				moveChild(VGeoFrame *,
								  VGeoManager *, boolean = FALSE);
	boolean				moveChildren(VGeoManager *, boolean = FALSE);

	void				setInternalBorderGeometry(int left, int right,
												  int top, int bottom);
	void				getInternalBorderGeometry(int *left, int *right,
												  int *top, int *bottom)
												  const;

	boolean				associateWindow(VWindow *);

	void				setQueryWindowSize(boolean);
	boolean				getQueryWindowSize() const;
	void				setForceFrameChildren(boolean);
	boolean				getForceFrameChildren() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	enum SizeGroupType {
						Width	= 0x0001,
						Height	= 0x0002,
						Size	= Width | Height,
						Square	= 0x0004
	};
	CLASS SizeGroup : public VObject {
	public:
						SizeGroup(int = Size);
					   ~SizeGroup();
		void			setType(int);
		SizeGroupType	getType() const;
		void			add(VGeoFrame *);
		void			add(VWindow *);
		void			remove(VGeoFrame *);
		void			remove(VWindow *);
		void			removeAll();
		VOrdCollect	   *getList() const;
	private:
		VOrdCollect	   *theList;
		char			theType;
	};

	SizeGroup		   *newSizeGroup(int = Size);

#if !defined(CV_NOARCHIVER)
	virtual void		putTo(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
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
	//
	// These are the standard virtual methods which should be
	// overridden by deriving classes wishing to implement a
	// geometry management scheme.
	//
	virtual void		getPreferredSize(int *w, int *h);
	virtual boolean		getConstrainedSize(int *w, int *h, int, int);
	virtual void		getMinimumSize(int *w, int *h);
	virtual void		getMaximumSize(int *w, int *h);
	virtual void		frameChildren(int w = 0, int h = 0);
	virtual boolean		frameChildrenOk(int w = 0, int h = 0);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	//
	CLASS ManagerChildData : public VGeoFrame::ChildData {
	public:
							ManagerChildData(const VGeoManager *);
		virtual			   ~ManagerChildData();
#if !defined(CV_NOARCHIVER)
		virtual void		putTo(VArchiver&);
		virtual void		getFrom(VArchiver&);
#endif
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// The ChildIterator class is used to aid in the traversal
	// through the list of children.  It can be set up to go
	// through forward or reverse, and it automatically skips
	// children which are marked as "do-not-frame".
	// This class is *not* user-API visible.
	//
	CLASS ChildIterator : protected VIterator {
	public:
					ChildIterator(const VGeoManager *);
					ChildIterator(const VGeoManager *, boolean all);
		void		begin(const VGeoFrame * = 0);
		VGeoFrame  *next();
		void		back();
		VGeoFrame  *current() const;
		void		reverse(boolean = TRUE);
		boolean		atVeryBeginning() const;
	private:
		int			backwards;
		int			index;
		int			dontSkipAny;
	};

protected:
	void				initialize();
	virtual void		childReplaced(VGeoFrame *, VGeoFrame *);
	virtual void		childRemoved(VGeoFrame *);
	virtual void		childUpdated(VGeoFrame *);
	virtual void		windowAssociated(VWindow *);
	void				beginFrameChildren(int&, int&);
	void				endFrameChildren();
	void				subtractInternalBorderSize(int *, int *);
	void				setupSizeGroups();
	void				checkWindowPreferredSize(int *, int *);
	void				checkWindowConstrainedSize(int *, int *, int, int);
	void				checkWindowMinimumSize(int *, int *);
	void				checkWindowMaximumSize(int *, int *);

#if !defined(CV_NOARCHIVER)
    virtual void		putTo(VArchiver&, boolean);
	virtual void		putToDone(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void		getFromDone(VArchiver&, VObject *);
	virtual void		gotChildren();
#endif

private:
						VGeoManager(const VGeoManager&);
	VGeoManager&		operator=(const VGeoManager&);

private:
	VOrdCollect		   *theChildren;
	VOrdCollect		   *sizeGroups;
	char				internalLeftBorder;
	char				internalRightBorder;
	char				internalTopBorder;
	char				internalBottomBorder;

protected:
	friend				VGeoFrame;
};

extern VClass *VGeoManagerCls;

// --------------------------------------------------------------------------
//
inline void VGeoManager::initialize()
{
	isGeoManager			= 1;
	theChildren				= 0;
	internalLeftBorder		= 0;
	internalRightBorder		= 0;
	internalTopBorder		= 0;
	internalBottomBorder	= 0;
	sizeGroups				= 0;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoManager::hasChildren() const
{
	return (theChildren != 0) && (theChildren->count() > 0);
}

// --------------------------------------------------------------------------
//
inline VOrdCollect *VGeoManager::getChildren() const
{
	return theChildren;
}

// --------------------------------------------------------------------------
// Return a pointer to the VGeoFrame child of this VGeoFrame at the given
// index; child indices begin at zero.  If this VGeoFrame has no children
// or the given index is out of range, then return 0.
//
inline VGeoFrame *VGeoManager::getChild(int child_index) const
{
	if (theChildren == 0) {
		return 0;
	}
	int nchildren = int(theChildren->count());
	if ((nchildren == 0) || (child_index >= nchildren)) {
		return 0;
	}
	return (VGeoFrame *)(theChildren->idAt(child_index));
}

// --------------------------------------------------------------------------
// Return the (zero-based) index of the specified VGeoFrame child of this
// VGeoFrame.  If the given VGeoFrame is not a child of this VGeoFrame
// then return -1.
//
inline int VGeoManager::getChildIndex(const VGeoFrame *child) const
{
	if ((child == 0) || !hasChildren()) {
		return -1;
	}
	return int(theChildren->indexOf((VObject *)child));
}

// --------------------------------------------------------------------------
//
inline int VGeoManager::getNumberOfChildren() const
{
	return (theChildren != 0) ? int(theChildren->count()) : 0;
}

// --------------------------------------------------------------------------
//
inline void VGeoManager::setQueryWindowSize(boolean b)
{
	queryManagerWindowSize = b;
}

inline boolean VGeoManager::getQueryWindowSize() const
{
	return queryManagerWindowSize;
}

inline void VGeoManager::setForceFrameChildren(boolean b)
{
	forceFrameChildren = b;
}

inline boolean VGeoManager::getForceFrameChildren() const
{
	return forceFrameChildren;
}

// --------------------------------------------------------------------------
//
inline VGeoManager::ChildIterator::ChildIterator(const VGeoManager *manager)
	: VIterator(manager->getChildren())
{
	backwards = 0;
	index = 0;
	dontSkipAny = 0;
}

inline VGeoManager::ChildIterator::ChildIterator(const VGeoManager *manager,
												 boolean all)
	: VIterator(manager->getChildren())
{
	backwards = 0;
	index = 0;
	dontSkipAny = all;
}

inline void VGeoManager::ChildIterator::back()
{
	if (backwards) {
		(*this)++;
	}
	else {
		(*this)--;
	}
	if (--index < 0) {
		index = 0;
	}
}

inline void VGeoManager::ChildIterator::reverse(boolean reverse)
{
	backwards = reverse;
}

inline VGeoFrame *VGeoManager::ChildIterator::current() const
{
	VObject *object = (VObject *)(*((VIterator *)this));
	if (object == 0) {
		if (backwards) {
			((VIterator *)this)->start();
			((ChildIterator *)this)->index = 0;
		}
		else {
			((VIterator *)this)->end();
			((ChildIterator *)this)->index = 0;
		}
		return (VGeoFrame *)((VObject *)(*((VIterator *)this)));
	}
	else {
		return (VGeoFrame *)object;
	}
}

inline int VGeoManager::ChildIterator::atVeryBeginning() const
{
	return index == 0;
}

// --------------------------------------------------------------------------
//
inline VGeoManager::ManagerChildData::
					ManagerChildData(const VGeoManager *parent)
#if defined(CV_WINDOWS) && (_MSC_VER <= 800)
	: ChildData(parent)
#else
	: VGeoFrame::ChildData(parent)
#endif
{
}

inline VGeoManager::ManagerChildData::~ManagerChildData()
{
}

#if !defined(CV_NOARCHIVER)

inline void VGeoManager::ManagerChildData::putTo(VArchiver& a)
{
	ChildData::putTo(a);
}

inline void VGeoManager::ManagerChildData::getFrom(VArchiver& a)
{
	ChildData::getFrom(a);
}

#endif // CV_NOARCHIVER

#endif // VGEOMGR_H
