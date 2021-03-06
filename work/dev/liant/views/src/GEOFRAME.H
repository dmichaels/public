// $Header:   Q:/views/common/vcs/GEOFRAME.H_V   1.13   Mar 24 1997 16:09:06   scottt  $

//	geoframe.h
//
//	VGeoFrame interface [Common]
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
//	11/01/95 dgm	Updated.
//	11/10/95 dgm	Updated.
//	11/15/95 dgm	Updated.
//	11/29/95 dgm	Added static getFromDone().
//	11/30/95 djs	OS/2 changes.
//	11/30/95 dgm	Zero ratio/offset in GeoComponent/Dimension::setInvalid()
//	12/05/95 dgm	Changes for UNIX DEC/Alpha-C++ compiler.
//	12/06/95 dgm	Added dontArchive support; archiving fix in getFrame().
//	01/29/96 dgm	Size-group changes.
//	08/26/96 dgm	Changed update() to take an int rather than a boolean.
//	09/06/96 dgm	Added the forceFrameChildren bit; see geoframe.cpp.
//	09/18/96 dgm	Fixed memory leak (theGeometry).
// ---------------------------------------------------------------------------

#ifndef VGEOFRAME_H
#define VGEOFRAME_H

// --------------------------------------------------------------------------
//
#include "object.h"
#include "window.h"
#include "frame.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VGeoManager;
CLASS VObject;
CLASS VWindow;

#if !defined(CV_NOARCHIVER)
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
// The VGeoFrame class represents a rectangular area which may or may not be
// associated with a VWindow, and which may contain any number of VGeoFrame
// (or derivative) children.  If there are children then the VGeoFrame acts
// as a geometry managers for its children.  It is intended that classes be
// derived from VGeoFrame whiich will impose a specific geometry management
// on its children.  The main mechanism used to implement such a scheme are
// the virtual member-functions frame(), getPreferredSize(),
// and getMinimumSize().
//
CLASS VGeoFrame : public VObject {
public:
						VGeoFrame(const VFrame&, VGeoManager *parent);
						VGeoFrame(int x, int y, int w, int h,
								  VGeoManager *parent);
						VGeoFrame(int w, int h, VGeoManager *parent);
						VGeoFrame(VGeoManager *parent);
						VGeoFrame(const VFrame&, VWindow *parent_window);
						VGeoFrame(int x, int y, int w, int h,
								  VWindow *parent_window);
						VGeoFrame(int w, int h, VWindow *parent_window);
						VGeoFrame(VWindow *parent_window);
						VGeoFrame();
					   ~VGeoFrame();
	VClass			   *iam();

	void				frame(boolean minimum = FALSE);

	void				determinePreferredSize(int *, int *);
	void				determineConstrainedSize(int *, int *, int, int);
	void				determineMinimumSize(int *, int *);
	void				determineMaximumSize(int *, int *);
	virtual void		update(int = 0);

	void				setDontFrame(boolean);
	boolean				isDontFrame() const;

	boolean				hasChildren() const;
	boolean				isManager() const;
	boolean				isFormManager() const;
	boolean				isRowOrColumnManager() const;
	boolean				isRowManager() const;
	boolean				isColumnManager() const;

	VGeoManager		   *getParent() const;
	VWindow			   *getWindow() const;
	VWindow			   *getParentWindow() const;
	int					getIndex() const;

	void				setFrame(const VFrame&);
	VFrame			   *getFrame() const;

	void				translateToWindowLocation(int&, int&);

	boolean				associateWindow(VWindow *);

#if !defined(CV_NOARCHIVER)
	void				setDontArchive(boolean);
	boolean				isDontArchive() const;
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

public:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// The Geometry class is used to contain the VFrame for the VGeoFrame
	// as well as the actual geometry of a VGeoFrame which does not have
	// an associated VWindow.  This class is *not* user-API visible.
	//
	CLASS Geometry {
	public:
						Geometry();
						Geometry(const VFrame&);
						Geometry(int, int);
						Geometry(int, int, int, int);
					   ~Geometry();
		void			setFrame(const VFrame&);
#if !defined(CV_NOARCHIVER)
		virtual void	putTo(VArchiver&);
		virtual void	getFrom(VArchiver&);
#endif
	public:
		VFrame		   *frame;
		short			x;
		short			y;
		short			width;
		short			height;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// The GetGeometryFlags are flags used to control the state
	// of GeoComponent objects, via the getGeometry() method.
	// This enumeration is *not* a user-API visible.
	//
	enum GetGeometryFlags {
						Ratios						= 0x0001,
						GetPreferredSize			= 0x0002,
						GetSimplePreferredSize		= 0x0004 | 0x0002,
						GetConstrainedPreferredSize	= 0x0008 | 0x0002,
						GetMinimumPreferredSize		= 0x0010 | 0x0002,
						GetFontSize					= 0x0020,
						NoRatios					= 0x0100,
						NoGetPreferredSize			= 0x0200,
						NoGetFontSize				= 0x1000,
						ForcePreferredSize			= 0x2000
	};

	CLASS GeoDimension;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// This class is used to represent the value of one component of
	// the geometry of a VGeoFrame.  A component is either the left
	// side, the right side, the width, the top side, the bottom side,
	// or the height.  This handles both integer and ratio values and
	// keeps track of preferred-size and font-size dependencies.
	// This class is *not* user-API visible.
	//
	CLASS GeoComponent {
	public:
						GeoComponent();
						GeoComponent(float, int);
						GeoComponent(float);
						GeoComponent(int);

		void			set(float, int);
		void			set(float);
		void			set(int);
		void			set();

		void			setRatio(float);
		void			setOffset(int);

		void			setValid();
		void			setInvalid();
		void			setPreferredSizeNeeded();
		void			setSimplePreferredSizeDone();
		void			setConstrainedPreferredSizeDone();
		void			setMinimumPreferredSizeDone();

		void			setFontSizeNeeded();
		void			setFontSizeDone();

		void			setPreferredSizeDependencies(int);

		float			getRatio() const;
		int				getOffset() const;
		operator		int() const;

		int				isComplete(boolean minimum) const;
		int				isRatio() const;

		int				isPreferredSizeNeeded() const;
		int				isPreferredSizeDone() const;
		int				isSimplePreferredSizeDone() const;
		int				isConstrainedPreferredSizeDone() const;
		int				isMinimumPreferredSizeDone() const;

		int				isFontSizeNeeded() const;
		int				isFontSizeDone() const;

		int				isDependentOnPreferredSize() const;
		int				isDependentOnSimplePreferredSize() const;
		int				isDependentOnConstrainedPreferredSize() const;
		int				isDependentOnMinimumPreferredSize() const;
		int				getPreferredSizeDependencies() const;

		int				getChildDimension(int);
		int				getChildPosition(int);
		int				getParentDimension(int);

	public:
		CLASS Dependency {
		public:
						Dependency();
			void		setValid();
			void		setInvalid();
			void		setPreferredSizeDependencies(int);
			int			isDependentOnPreferredSize() const;
			int			isDependentOnSimplePreferredSize() const;
			int			isDependentOnConstrainedPreferredSize() const;
			int			isDependentOnMinimumPreferredSize() const;
			boolean		isComplete(boolean minimum) const;
			int			getPreferredSizeDependencies() const;
		public:
			short		bits;
		};

	public:
		enum Bits {
						ValidFrameData						= 0x0001,
						PreferredSizeNeeded					= 0x0002,
						FontSizeNeeded						= 0x0020,
						SimplePreferredSizeDone				= 0x0040,
						ConstrainedPreferredSizeDone		= 0x0080,
						MinimumPreferredSizeDone			= 0x0100,
						FontSizeDone						= 0x0200,
						DependentOnSimplePreferredSize		= 0x0400,
						DependentOnConstrainedPreferredSize	= 0x0800,
						DependentOnMinimumPreferredSize		= 0x1000
		};

	public:
		float			ratio;
		short			offset;
		short			bits;

	private:
		friend			Dependency;
		friend			GeoDimension;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// This class is used to represent the width or height
	// of a VGeoFrame.  It is basically a simplified version
	// of GeoComponent which does not handle ratios, but does
	// keep track of preferred-size and font-size dependencies.
	// This class is *not* user-API visible.
	//
	CLASS GeoDimension {
	public:
						GeoDimension();
						GeoDimension(int);

		void			operator=(const GeoComponent&);
		void			operator=(int);
		void			set(int);
		void			set();

		void			setValid();
		void			setInvalid();
		void			setPreferredSizeNeeded();
		void			setSimplePreferredSizeDone();
		void			setConstrainedPreferredSizeDone();
		void			setMinimumPreferredSizeDone();
		void			setFontSizeNeeded();
		void			setFontSizeDone();

		int				get() const;
		operator		int() const;

		int				isComplete(boolean minimum) const;

		int				isPreferredSizeNeeded() const;
		int				isPreferredSizeDone() const;
		int				isSimplePreferredSizeDone() const;
		int				isConstrainedPreferredSizeDone() const;
		int				isMinimumPreferredSizeDone() const;
		int				isFontSizeNeeded() const;
		int				isFontSizeDone() const;

	public:
		short			offset;
		short			bits;
	};

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// The ChildData class is used to represent child specific data
	// which is needed by the parent geometry manager of a VGeoFrame
	// or sub-class; such child specific data is commonly referred to
	// as "constraint" data.  It is intended to be sub-classed and used
	// by sub-classes of VGeoFrame.  Every VGeoFrame object will have a
	// ChildData pointer (since every VGeoFrame object may be a child)
	// which refers to a ChildData sub-class containing information
	// needed by its parent VGeoFrame in order to perform proper and
	// efficient geometry management on it.  Since a VGeoFrame object
	// may be freely moved from one VGeoFrame parent manager to another,
	// the union of all ChildData objects ever created in the within
	// the lifetime of the VGeoFrame object is kept in a list (formed
	// by the next member); the ChildData object at the front of the
	// list (i.e. the one pointed to directly by the childData member
	// is the current one, i.e. the one which corresponds to the parent
	// VGeoFrame object of which the VGeoFrame is currently a child.
	// This class is *not* user-API visible.
	//
	CLASS ChildData {
	public:
							ChildData(const VGeoManager *);
		virtual			   ~ChildData();
#if !defined(CV_NOARCHIVER)
		virtual void		putTo(VArchiver&);
		virtual void		getFrom(VArchiver&);
#endif
	};

public:
	void				getSize(int *w, int *h,
								boolean minimum = FALSE,
								int constrained_w = 0,
								int constrained_h = 0);
	void				getGeometry(GeoComponent *x, GeoComponent *y,
									GeoComponent *w, GeoComponent *h,
									int flags = 0,
									int constrained_w = 0,
									int constrained_h = 0);
	void				completeGeometry(GeoComponent *x, GeoComponent *y,
										 GeoComponent *w, GeoComponent *h,
										 int flags = 0,
										 int constrained_w = 0,
										 int constrained_h = 0);

	void				addNonClientAreaSize(int *w, int *h);

	void				getPhysicalGeometry(int *, int *, int *, int *);
	void				getPhysicalClientSize(int *, int *);
	void				setPhysicalGeometry(int, int, int, int);
	void				setPhysicalSize(int, int);
	void				moveWindowChildren(int, int);

protected:
	void				initialize();
	virtual ChildData  *newChildData() const;
	virtual void		windowAssociated(VWindow *);
	static boolean		verifyManager(VWindow *);

#if !defined(CV_NOARCHIVER)
    virtual void		putTo(VArchiver&, boolean);
	virtual void		putToDone(VArchiver&);
	virtual void		getFrom(VArchiver&);
	virtual void		getFromDone(VArchiver&, VObject *);
	static int			getWindowIndex(VWindow *);
public:
	static void			getFromDone(VWindow *);
#endif

private:
						VGeoFrame(const VGeoFrame&);
	VGeoFrame&			operator=(const VGeoFrame&);

public:
	VGeoManager		   *theParent;
	ChildData		   *childData;
	union {
		VWindow		   *theWindow;
		Geometry	   *theGeometry;
		int				theWindowIndex;
	};
	GeoDimension		preferredWidthCache;
	GeoDimension		preferredHeightCache;
	char				reservedA;
	char				reservedB;
	short				usingTheWindow : 1;
	short				dontFrame : 1;
	short				reservedC : 1;
	short				isGeoManager : 1;
	short				isGeoForm : 1;
	short				isGeoRowOrColumn : 1;
	short				isGeoRow : 1;
	short				isGeoColumn : 1;
	short				isGeoRowColumn : 1;
	short				isGeoPacker : 1;
	short				usingTheWindowIndex : 1;
	short				dontArchive : 1;
	short				isDummy : 1;
	short				reservedD : 1;
	short				forceFrameChildren : 1;
	short				queryManagerWindowSize : 1;

protected:
	friend				VGeoManager;
	friend				VWindow;
};

extern VClass *VGeoFrameCls;

// --------------------------------------------------------------------------
//
inline VGeoManager *VGeoFrame::getParent() const
{
	return theParent;
}

// --------------------------------------------------------------------------
// Return a poniter to this VGeoFrame's associated VWindow, if it has one,
// otherwise null.
//
inline VWindow *VGeoFrame::getWindow() const
{
	if (usingTheWindow) {
		return theWindow;
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
inline void VGeoFrame::setDontFrame(boolean b)
{
	dontFrame = short(b);
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isDontFrame() const
{
	return dontFrame;
}

#if !defined(CV_NOARCHIVER)
// --------------------------------------------------------------------------
//
inline void VGeoFrame::setDontArchive(boolean b)
{
	dontArchive = b;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isDontArchive() const
{
	return dontArchive;
}
#endif	//CV_NOARCHIVER

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isManager() const
{
	return isGeoManager;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isFormManager() const
{
	return isGeoForm;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isRowOrColumnManager() const
{
	return isGeoRowOrColumn;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isRowManager() const
{
	return isGeoRow;
}

// --------------------------------------------------------------------------
//
inline boolean VGeoFrame::isColumnManager() const
{
	return isGeoColumn;
}

// --------------------------------------------------------------------------
//
inline VGeoFrame::Geometry::Geometry()
{
	frame = 0;
	x = y = width = height = 0;
}

inline VGeoFrame::Geometry::Geometry(const VFrame& f)
{
	frame = new VFrame(f);
	x = y = width = height = 0;
}

inline VGeoFrame::Geometry::Geometry(int fx, int fy, int fw, int fh)
{
	frame = new VFrame(fx, fy, fw, fh, CornerDim);
	x = y = width = height = 0;
}

inline VGeoFrame::Geometry::Geometry(int fw, int fh)
{
	frame = new VFrame(0, 0, fw, fh, CornerDim);
	x = y = width = height = 0;
}

inline VGeoFrame::Geometry::~Geometry()
{
	delete frame;
}

inline void VGeoFrame::Geometry::setFrame(const VFrame& f)
{
	if (frame == 0) {
		frame = new VFrame(f);
	}
	else {
		*frame = f;
	}
}

// --------------------------------------------------------------------------
//
inline VGeoFrame::
	   GeoComponent::GeoComponent()
{
	ratio = 0.0F; offset = short(0); bits = short(0);
}

inline VGeoFrame::
	   GeoComponent::GeoComponent(float f, int i)
{
	ratio = f; offset = short(i); bits = short(0);
}

inline VGeoFrame::
	   GeoComponent::GeoComponent(float f)
{
	ratio = f; offset = short(0); bits = short(0);
}

inline VGeoFrame::
	   GeoComponent::GeoComponent(int i)
{
	ratio = 0.0F; offset = short(i); bits = short(0);
}

inline void VGeoFrame::
			GeoComponent::set(float f, int i)
{
	ratio = f; offset = short(i);
}

inline void VGeoFrame::
			GeoComponent::set(float f)
{
	ratio = f; offset = short(0);
}

inline void VGeoFrame::
			GeoComponent::set(int i)
{
	ratio = 0.0F; offset = short(i);
}

inline void VGeoFrame::
			GeoComponent::set()
{
	ratio = 0.0F; offset = short(0); bits = short(0);
}

inline void VGeoFrame::
			GeoComponent::setRatio(float f)
{
	ratio = f;
}

inline void VGeoFrame::
			GeoComponent::setOffset(int i)
{
	offset = short(i);
}

inline void VGeoFrame::
			GeoComponent::setValid()
{
	bits |= ValidFrameData;
}

inline void VGeoFrame::
			GeoComponent::setInvalid()
{
	bits = short(0);
	ratio = 0.0F;
	offset = short(0);
}

inline void VGeoFrame::
			GeoComponent::setPreferredSizeNeeded()
{
	bits |= PreferredSizeNeeded;
}

inline void VGeoFrame::
			GeoComponent::setSimplePreferredSizeDone()
{
	bits |= SimplePreferredSizeDone;
	bits &= ~(ConstrainedPreferredSizeDone | MinimumPreferredSizeDone);
}

inline void VGeoFrame::
			GeoComponent::setConstrainedPreferredSizeDone()
{
	bits |= ConstrainedPreferredSizeDone;
	bits &= ~(SimplePreferredSizeDone | MinimumPreferredSizeDone);
}

inline void VGeoFrame::
			GeoComponent::setMinimumPreferredSizeDone()
{
	bits |= MinimumPreferredSizeDone;
	bits &= ~(ConstrainedPreferredSizeDone | SimplePreferredSizeDone);
}

inline void VGeoFrame::
			GeoComponent::setFontSizeNeeded()
{
	bits |=  FontSizeNeeded;
	bits &= ~FontSizeDone;
}

inline void VGeoFrame::
			GeoComponent::setFontSizeDone()
{
	bits |= FontSizeDone;
}

inline void VGeoFrame::
			GeoComponent::setPreferredSizeDependencies(int d)
{
	bits &= ~(DependentOnSimplePreferredSize |
			  DependentOnConstrainedPreferredSize |
			  DependentOnMinimumPreferredSize);
	bits |= (d & (DependentOnSimplePreferredSize |
				  DependentOnConstrainedPreferredSize |
				  DependentOnMinimumPreferredSize));
}

inline float VGeoFrame::GeoComponent::getRatio() const
{
	return ratio;
}

inline int VGeoFrame::GeoComponent::getOffset() const
{
	return offset;
}

inline VGeoFrame::GeoComponent::operator int() const
{
	return offset;
}

inline int VGeoFrame::GeoComponent::isComplete(boolean minimum) const
{
	if (minimum) {
		return (bits & ValidFrameData) &&
			   (bits & MinimumPreferredSizeDone) &&
			   !(bits & (DependentOnSimplePreferredSize |
						 DependentOnConstrainedPreferredSize)) &&
			   (!(bits & FontSizeNeeded) ||
			     (bits & FontSizeDone));
	}
	else {
		return (bits & ValidFrameData) &&
			   (!(bits & PreferredSizeNeeded) ||
			     (bits & SimplePreferredSizeDone)) &&
			    !(bits & (DependentOnConstrainedPreferredSize |
						  DependentOnMinimumPreferredSize)) &&
			   (!(bits & FontSizeNeeded) ||
			     (bits & FontSizeDone));
	}
}

inline int VGeoFrame::
		   GeoComponent::isRatio() const
{
	return ratio != 0.0F;
}

inline int VGeoFrame::
		   GeoComponent::isPreferredSizeNeeded() const
{
	return bits & PreferredSizeNeeded;
}

inline int VGeoFrame::
		   GeoComponent::isPreferredSizeDone() const
{
	return bits & (SimplePreferredSizeDone |
				   ConstrainedPreferredSizeDone |
				   MinimumPreferredSizeDone);
}

inline int VGeoFrame::
		   GeoComponent::isSimplePreferredSizeDone() const
{
	return bits & SimplePreferredSizeDone;
}

inline int VGeoFrame::
		   GeoComponent::isConstrainedPreferredSizeDone() const
{
	return bits & ConstrainedPreferredSizeDone;
}

inline int VGeoFrame::
		   GeoComponent::isMinimumPreferredSizeDone() const
{
	return bits & MinimumPreferredSizeDone;
}

inline int VGeoFrame::
		   GeoComponent::isFontSizeNeeded() const
{
	return bits & FontSizeNeeded;
}

inline int VGeoFrame::
		   GeoComponent::isFontSizeDone() const
{
	return bits & FontSizeDone;
}

inline int VGeoFrame::
		   GeoComponent::isDependentOnPreferredSize() const
{
	return bits & (SimplePreferredSizeDone |
				   ConstrainedPreferredSizeDone |
				   MinimumPreferredSizeDone |
				   DependentOnSimplePreferredSize |
				   DependentOnConstrainedPreferredSize |
				   DependentOnMinimumPreferredSize);
}

inline int VGeoFrame::
		   GeoComponent::isDependentOnSimplePreferredSize() const
{
	return bits & (SimplePreferredSizeDone |
				   DependentOnSimplePreferredSize);
}

inline int VGeoFrame::
		   GeoComponent::isDependentOnConstrainedPreferredSize() const
{
	return bits & (ConstrainedPreferredSizeDone |
				   DependentOnConstrainedPreferredSize);
}

inline int VGeoFrame::
		   GeoComponent::isDependentOnMinimumPreferredSize() const
{
	return bits & (MinimumPreferredSizeDone |
				   DependentOnMinimumPreferredSize);
}

inline int VGeoFrame::GeoComponent::getPreferredSizeDependencies() const
{
	int d = 0;
	if (bits & (SimplePreferredSizeDone |
				DependentOnSimplePreferredSize)) {
		d |= DependentOnSimplePreferredSize;
	}
	if (bits & (ConstrainedPreferredSizeDone |
				DependentOnConstrainedPreferredSize)) {
		d |= DependentOnConstrainedPreferredSize;
	}
	if (bits & (MinimumPreferredSizeDone |
				DependentOnMinimumPreferredSize)) {
		d |= DependentOnMinimumPreferredSize;
	}
	return d;
}

inline int VGeoFrame::
		   GeoComponent::getChildDimension(int parent_dim)
{
	if (ratio == 0.0F) {
		return offset < 0 ? 0 : int(offset);
	}
	else {
		int i = int(offset + (ratio * float(parent_dim)) + 0.5F);
		return i < 0 ? 0 : i;
	}
}

inline int VGeoFrame::
		   GeoComponent::getChildPosition(int parent_dim)
{
	if (ratio == 0.0F) {
		return int(offset);
	}
	else {
		return int(offset + (ratio * float(parent_dim)) + 0.5F);
	}
}

inline int VGeoFrame::
		   GeoComponent::getParentDimension(int child_dim)
{
	if (ratio == 0.0F) {
		return 0;
	}
	else {
		int i = int((float(child_dim - offset) / ratio) + 0.5F);
		return i < 0 ? 0 : i;
	}
}

inline VGeoFrame::GeoComponent::
				  Dependency::Dependency()
{
	bits = short(0);
}

inline void VGeoFrame::GeoComponent::
					   Dependency::setValid()
{
	bits |= ValidFrameData;
}

inline void VGeoFrame::GeoComponent::
					   Dependency::setInvalid()
{
	bits = short(0);
}

inline void VGeoFrame::GeoComponent::
					   Dependency::setPreferredSizeDependencies(int d)
{
	bits &= ~(DependentOnSimplePreferredSize |
			  DependentOnConstrainedPreferredSize |
			  DependentOnMinimumPreferredSize);
	bits |= (d & (DependentOnSimplePreferredSize |
				  DependentOnConstrainedPreferredSize |
				  DependentOnMinimumPreferredSize)) /*| ValidFrameData*/;
}

inline int VGeoFrame::GeoComponent::
					  Dependency::isDependentOnPreferredSize() const
{
	return bits & (SimplePreferredSizeDone |
				   ConstrainedPreferredSizeDone |
				   MinimumPreferredSizeDone |
				   DependentOnSimplePreferredSize |
				   DependentOnConstrainedPreferredSize |
				   DependentOnMinimumPreferredSize);
}

inline int VGeoFrame::GeoComponent::
					  Dependency::isDependentOnSimplePreferredSize() const
{
	return bits & (SimplePreferredSizeDone |
				   DependentOnSimplePreferredSize);
}

inline int VGeoFrame::GeoComponent::
					  Dependency::
					  isDependentOnConstrainedPreferredSize() const
{
	return bits & (ConstrainedPreferredSizeDone |
				   DependentOnConstrainedPreferredSize);
}

inline int VGeoFrame::GeoComponent::
					  Dependency::isDependentOnMinimumPreferredSize() const
{
	return bits & (MinimumPreferredSizeDone |
				   DependentOnMinimumPreferredSize);
}

inline int VGeoFrame::GeoComponent::
					  Dependency::isComplete(boolean minimum) const
{
	if (minimum) {
		return (bits & ValidFrameData) &&
			   (bits & MinimumPreferredSizeDone) &&
			   !(bits & (DependentOnSimplePreferredSize |
						 DependentOnConstrainedPreferredSize)) &&
			   (!(bits & FontSizeNeeded) ||
			     (bits & FontSizeDone));
	}
	else {
		return (bits & ValidFrameData) &&
			   (!(bits & PreferredSizeNeeded) ||
			     (bits & SimplePreferredSizeDone)) &&
			    !(bits & (DependentOnConstrainedPreferredSize |
						  DependentOnMinimumPreferredSize)) &&
			   (!(bits & FontSizeNeeded) ||
			     (bits & FontSizeDone));
	}
}

inline int VGeoFrame::GeoComponent::
					  Dependency::getPreferredSizeDependencies() const
{
	int d = 0;
	if (bits & (SimplePreferredSizeDone |
				DependentOnSimplePreferredSize)) {
		d |= DependentOnSimplePreferredSize;
	}
	if (bits & (ConstrainedPreferredSizeDone |
				DependentOnConstrainedPreferredSize)) {
		d |= DependentOnConstrainedPreferredSize;
	}
	if (bits & (MinimumPreferredSizeDone |
				DependentOnMinimumPreferredSize)) {
		d |= DependentOnMinimumPreferredSize;
	}
	return d;
}

// --------------------------------------------------------------------------
//
inline VGeoFrame::GeoDimension::GeoDimension()
{
	offset = short(0); bits = short(0);
}

inline VGeoFrame::GeoDimension::GeoDimension(int i)
{
	offset = short(i); bits = short(0);
}

inline void VGeoFrame::
			GeoDimension::operator=(const VGeoFrame::GeoComponent& gd)
{
	offset = gd.offset; bits = gd.bits;
}

inline void VGeoFrame::GeoDimension::operator=(int i)
{
	offset = short(i);
}

inline VGeoFrame::GeoDimension::operator int() const
{
	return offset;
}

inline void VGeoFrame::GeoDimension::set(int i)
{
	offset = short(i);
}

inline void VGeoFrame::GeoDimension::set()
{
	offset = short(0); bits = short(0);
}

inline void VGeoFrame::GeoDimension::setValid()
{
	bits |= GeoComponent::ValidFrameData;
}

inline void VGeoFrame::GeoDimension::setInvalid()
{
	bits = short(0);
	offset = short(0);
}

inline void VGeoFrame::GeoDimension::setPreferredSizeNeeded()
{
	bits |= GeoComponent::PreferredSizeNeeded;
}

inline void VGeoFrame::GeoDimension::setSimplePreferredSizeDone()
{
	bits |= GeoComponent::SimplePreferredSizeDone;
	bits &= ~(GeoComponent::ConstrainedPreferredSizeDone |
			  GeoComponent::MinimumPreferredSizeDone);
}

inline void VGeoFrame::GeoDimension::setConstrainedPreferredSizeDone()
{
	bits |= GeoComponent::ConstrainedPreferredSizeDone;
	bits &= ~(GeoComponent::SimplePreferredSizeDone |
			  GeoComponent::MinimumPreferredSizeDone);
}

inline void VGeoFrame::GeoDimension::setMinimumPreferredSizeDone()
{
	bits |= GeoComponent::MinimumPreferredSizeDone;
	bits &= ~(GeoComponent::ConstrainedPreferredSizeDone |
			  GeoComponent::SimplePreferredSizeDone);
}

inline void VGeoFrame::GeoDimension::setFontSizeNeeded()
{
	bits |=  GeoComponent::FontSizeNeeded;
	bits &= ~GeoComponent::FontSizeDone;
}

inline void VGeoFrame::GeoDimension::setFontSizeDone()
{
	bits |= GeoComponent::FontSizeDone;
}

inline int VGeoFrame::GeoDimension::isComplete(boolean minimum) const
{
	if (minimum) {
		return (bits & GeoComponent::ValidFrameData) &&
			   (bits & GeoComponent::MinimumPreferredSizeDone) &&
			   (!(bits & GeoComponent::FontSizeNeeded) ||
			     (bits & GeoComponent::FontSizeDone));
	}
	else {
		return (bits & GeoComponent::ValidFrameData) &&
			   (!(bits & GeoComponent::PreferredSizeNeeded) ||
				 (bits & GeoComponent::SimplePreferredSizeDone)) &&
			   (!(bits & GeoComponent::FontSizeNeeded) ||
			     (bits & GeoComponent::FontSizeDone));
	}
}

inline int VGeoFrame::GeoDimension::get() const
{
	return offset;
}

inline int VGeoFrame::GeoDimension::isPreferredSizeNeeded() const
{
	return bits & GeoComponent::PreferredSizeNeeded;
}

inline int VGeoFrame::GeoDimension::isPreferredSizeDone() const
{
	return bits & (GeoComponent::SimplePreferredSizeDone |
				   GeoComponent::ConstrainedPreferredSizeDone |
				   GeoComponent::MinimumPreferredSizeDone);
}

inline int VGeoFrame::GeoDimension::isSimplePreferredSizeDone() const
{
	return bits & GeoComponent::SimplePreferredSizeDone;
}

inline int VGeoFrame::GeoDimension::isConstrainedPreferredSizeDone() const
{
	return bits & GeoComponent::ConstrainedPreferredSizeDone;
}

inline int VGeoFrame::GeoDimension::isMinimumPreferredSizeDone() const
{
	return bits & GeoComponent::MinimumPreferredSizeDone;
}

inline int VGeoFrame::GeoDimension::isFontSizeNeeded() const
{
	return bits & GeoComponent::FontSizeNeeded;
}

inline int VGeoFrame::GeoDimension::isFontSizeDone() const
{
	return bits & GeoComponent::FontSizeDone;
}

// --------------------------------------------------------------------------
//
inline VGeoFrame::ChildData::ChildData(const VGeoManager *)
{
}

inline VGeoFrame::ChildData::~ChildData()
{
}

#if !defined(CV_NOARCHIVER)
inline void VGeoFrame::ChildData::putTo(VArchiver& a)
{
}

inline void VGeoFrame::ChildData::getFrom(VArchiver& a)
{
}
#endif //CV_NOARCHIVER

#endif // VGEOFRAME_H
