// $Header:   Q:/views/common/vcs/GEOROC.H_V   1.13   Mar 24 1997 16:09:14   scottt  $

//	georoc.h
//
//	VGeoRowOrColumn interface [Common]
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
//	11/20/95 dgm	Added set/getPreferredMinorSpacing().
//	12/14/95 dgm	Minor #ifdefs for compiler bugs.
//	12/15/95 tlf	Fix minor #ifdefs for compiler bugs.
//	01/15/96 dgm	Added NearJustify.
//	01/15/96 dgm	Miscellaneous name changes.
//	01/29/96 dgm	Size-group changes.
//	06/05/96 dgm	Added VGeoFrame * and VWindow * versions of the child
//					attribute setting methods, i.e. in addition to the ones
//					which take and index.
//	08/27/96 dgm	Implemented previously stubbed
//					setPreferredNumberOfRowsOrColumns()
//					and added getNumberOfRowsOrColumns().
//	09/17/96 dgm	Added setSpaceElasticity(), setDefaultChildElasticity(),
//					setDefaultChildMinorElasticity(), setChildrenElasticity(),
//					setChildElasticity(), setChildrenMinorElasticity(),
//					and setChildMinorElasticity() -- elasticity is simply
//					shorthand for both stretchability and shrinkability.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VGEOROC_H
#define VGEOROC_H

// --------------------------------------------------------------------------
//
#include "geomgr.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VFrame;
CLASS VObject;
CLASS VWindow;

#if !defined(CV_NOARCHIVER)
CLASS VArchiver;
#endif

// --------------------------------------------------------------------------
// The VGeoRowOrColumn manager arranges its VGeoFrame children in a row.
// The following attributes may be set:
//
//   Spacing:
//   --------
//   This controls the horizontal (major dimension) spacing of the
//   children.  The following values are supported for this attribute:
//
//   o NoSpacing         --> No space is inserted between children, and
//                           the children are left justified in the row.
//   o NoSpacingRight    --> No space is inserted between children, and
//                           the children are right justified in the row.
//   o EqualSpacing      --> The children are separated by equal spacing;
//                           the left-most and right-most children are
//                           also separated from the left and right side
//                           of the parent by that same amount of space.
//   o EqualFlushSpacing --> The children are separated by equal spacing;
//                           the left-most and right-most children are
//                           flush up against the left and right edges
//                           of the parent.
//
//   Preferred Spacing:
//   ------------------
//   This applies only when the spacing style is EqualSpacing
//   EqualFlushSpacing, or NoSpacing; it controls the spacing
//   between children.
//
//   o For EqualSpacing, this is the preferred number of pixels to place
//     between each child; if there is only one child, then this is
//     the preferred number of pixels between the left/right side of
//     the child and the left/right edge.
//   o For EqualFlushSpacing, this is the preferred number of pixels to
//     place between each child; if there is only one child, then this
//     is not meaningful.
//   o For NoSpacing, this is the number of pixels to place between each
//     child.
//
//   Note that there is a distinct disccrepency between the way this
//   value is treated for the EqualSpacing/EqualSpacingFlush and the
//   NoSpacing cases.  For the EqualSpacing/EqualSpacingFlush cases,
//   the specified spacing will be shrunk or stretched if the manager
//   is smaller or larger (in the major dimension) than its preferred
//   size.  For the NoSpacing case, the specified spacing will *not*
//   be stretched or shrunk according to the size of the manager; the
//   value in this case is treated represents the minimum distance the
//   children will be separated by (they could be separated by more
//   if child margins dictate).
//
//   Justification:
//   --------------
//   This controls the vertical (minor dimension) alignment of the
//   children.  The following values are supported for this attribute:
//
//   o CenterJustify     --> The children are vertically aligned so
//                           as to be centered relative to each other.
//   o NearJustify       --> The children are top or left justified.
//   o FarJustify        --> The children are bottom or right justified.
//   o FilledJustify     --> The children heights are set to fill
//                           the entire height of the row.
//
//   Margins:
//   --------
//   This controls the left, right, top, and bottom inner margins of the
//   manager.  No child will be placed closer than the specified margin
//   value (in pixels), if at all possible.
//
//   Child Margins:
//   --------------
//   This controls the the left, right, top, and bottom outer margins
//   surrounding each child.  No sibling, nor the parent, will be
//   placed closer than the specified margin value (in pixels).
//   Note that margins themselves may overlap.
//
//   Space Stretchability:
//   ---------------------
//   This applies only when the spacing style is EqualSpacing or
//   EqualFlushSpacing; it is a floating point value between 0.0
//   and 1.0 (inclusive) which indicates the percentage of the "extra"
//   space which is given to the spaces around the children (in the
//   major dimension) when the row/column manager is sized larger
//   than its preferred size.
//
//   For example, if the preferred size of a row manager is 100 pixels
//   wide, and the space-stretchability ratio is 0.7, and the row is
//   sized to be 150 pixels wide, then 35 (70%) of the extra pixels
//   will be given to the spaces surrounding the children and 15 pixels
//   will be given to the children.  However, if none of the children
//   are stretchable (see below), then these remaining 15 pixels will
//   also go to the spaces surrounding the children.
//
//   Space Shrinkability:
//   --------------------
//   This applies only when the spacing style is EqualSpacing or
//   EqualFlushSpacing; it is a floating point value between 0.0
//   and 1.0 (inclusive) which indicates the percentage of the
//   deficit space whcih is taken away from the spaces around the
//   children (in the major dimension) when the row/column manager
//   is sized smaller than its preferred size.
//
//   Child Stretchability:
//   ---------------------
//   This controls if and by how much a given child's major size is
//   increased when the row/column manager's major size is larger than
//   its preferred size (i.e. when "extra" space is available).  The
//   value is an arbitrary (floating point) number which is meant to
//   specify a relative percentage of the extra space which the child
//   would like to obtain for its own growth.
//
//   Child Shrinkability:
//   --------------------
//   This controls if and by how much a given child's major size is
//   decreased when the row/column manager's minor size is smaller than
//   its preferred size (i.e. when there is insufficient space available).
//
//   Child Minor Stretchability:
//   ---------------------------
//   This controls if and by how much a given child's minor size is
//   increased when the row/column manager's minor size is larger than
//   its preferred size; it is a floating point value between 0.0 and
//   1.0 (inclusive) which indicates the percentage of the "extra"
//   space which is given to the child's major size.
//
//   Child Minor Shrinkability:
//   --------------------------
//   This controls if and by how much a given child's minor size is
//   decreased when the row/column manager's minor size is smaller than
//    its preferred size; it is a floating point value between 0.0 and
//   1.0 (inclusive) which indicates the percentage of the deficit
//   space which is taken away from to the child's minor size.
//
CLASS VGeoRowOrColumn : public VGeoManager {

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Public Interface.
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

public:

	// Constants.

	enum SpacingStyle { NoSpacing			= 0,
						NoSpacingOpposite	= 1,
						EqualSpacing		= 2,
						EqualFlushSpacing	= 3,
						NoSpacingRight		= NoSpacingOpposite,
						NoSpacingBottom		= NoSpacingOpposite
	};
	enum JustifyStyle { CenterJustify		= 0,
						NearJustify			= 1,
						FarJustify			= 2,
						FilledJustify		= 3,
						TopJustify			= NearJustify,
						BottomJustify		= FarJustify,
						LeftJustify			= NearJustify,
						RightJustify		= FarJustify
	};
	enum Dimension {	Row					= 0,
						Column				= 1
	};

	// Construction.
						VGeoRowOrColumn(Dimension,
										const VFrame&,
										VGeoManager *parent);
						VGeoRowOrColumn(Dimension,
										int x, int y, int w, int h,
										VGeoManager *parent);
						VGeoRowOrColumn(Dimension,
										int w, int h,
										VGeoManager *parent);
						VGeoRowOrColumn(Dimension,
										VGeoManager *parent);
						VGeoRowOrColumn(Dimension,
										const VFrame&,
										VWindow *parent_window);
						VGeoRowOrColumn(Dimension,
										int x, int y, int w, int h,
										VWindow *parent_window);
						VGeoRowOrColumn(Dimension,
										int w, int h,
										VWindow *parent_window);
						VGeoRowOrColumn(Dimension,
										VWindow *parent_window);
						VGeoRowOrColumn(Dimension);
						VGeoRowOrColumn();
					   ~VGeoRowOrColumn();
	VClass			   *iam();

	// Row/column manager attributes.

	boolean				setDimension(Dimension);
	Dimension			getDimension() const;

	void				setSpacingStyle(SpacingStyle);
	SpacingStyle		getSpacingStyle() const;

	int					getPreferredSpacing() const;
	void				setPreferredSpacing(int);

	int					getPreferredMinorSpacing() const;
	void				setPreferredMinorSpacing(int);

	void				setMargins(int all);
	void				setMargins(int major, int minor);
	void				setMargins(int major_near,
								   int major_far,
								   int minor_near,
								   int minor_far);
	void				getMargins(int *major_near,
								   int *major_far,
								   int *minor_near,
								   int *minor_far) const;

	void				setEqualSize(boolean);
	boolean				getEqualSize() const;

	void				setEqualMinorSize(boolean);
	boolean				getEqualMinorSize() const;

	void				setSpaceStretchability(float);
	float				getSpaceStretchability() const;

	void				setSpaceShrinkability(float);
	float				getSpaceShrinkability() const;

	void				setSpaceElasticity(float);

	void				setWrapping(boolean);
	boolean				isWrapping() const;

	// These are rather obscure.

	int					getNumberOfRowsOrColumns() const;
	int					getPreferredNumberOfRowsOrColumns() const;
	void				setPreferredNumberOfRowsOrColumns(int);

	void				setMinimizeConstrainedMajorDimension(boolean);
	boolean				getMinimizeConstrainedMajorDimension() const;

	void				setAssumeWrappingOnGetConstrainedSize(boolean);
	boolean				getAssumeWrappingOnGetConstrainedSize() const;

	// Default child specific constraints.

	void				setDefaultChildJustifyStyle(JustifyStyle);
	JustifyStyle		getDefaultChildJustifyStyle() const;

	void				setDefaultChildMargins(int all);
	void				setDefaultChildMargins(int major, int minor);
	void				setDefaultChildMargins(int major_near,
											   int major_far,
											   int minor_near,
											   int minor_far);
	void				getDefaultChildMargins(int *major_near,
											   int *major_far,
											   int *minor_near,
											   int *minor_far) const;

	void				setDefaultChildStretchability(float);
	float				getDefaultChildStretchability() const;

	void				setDefaultChildShrinkability(float);
	float				getDefaultChildShrinkability() const;

	void				setDefaultChildMinorStretchability(float);
	float				getDefaultChildMinorStretchability() const;

	void				setDefaultChildMinorShrinkability(float);
	float				getDefaultChildMinorShrinkability() const;

	void				setDefaultChildElasticity(float);
	void				setDefaultChildMinorElasticity(float);

	// Child specific constraints.

	void				setChildrenJustifyStyle(JustifyStyle);
	void				setChildJustifyStyle(int child_index, JustifyStyle);
	void				setChildJustifyStyle(VGeoFrame *, JustifyStyle);
	void				setChildJustifyStyle(VWindow *, JustifyStyle);
	JustifyStyle		getChildJustifyStyle(int child_index) const;
	JustifyStyle		getChildJustifyStyle(VGeoFrame *) const;
	JustifyStyle		getChildJustifyStyle(VWindow *) const;

	void				setChildrenMargins(int all);
	void				setChildrenMargins(int major, int minor);
	void				setChildrenMargins(int major_near, int major_far,
										   int minor_near, int minor_far);
	void				setChildMargins(int child_index, int all);
	void				setChildMargins(int child_index,
									    int major, int minor);
	void				setChildMargins(int child_index,
									    int major_near, int major_far,
									    int minor_near, int minor_far);
	void				setChildMargins(VGeoFrame *, int all);
	void				setChildMargins(VGeoFrame *,
									    int major, int minor);
	void				setChildMargins(VGeoFrame *,
									    int major_near, int major_far,
									    int minor_near, int minor_far);
	void				setChildMargins(VWindow *, int all);
	void				setChildMargins(VWindow *,
									    int major, int minor);
	void				setChildMargins(VWindow *,
									    int major_near, int major_far,
									    int minor_near, int minor_far);
	void				getChildMargins(int child_index,
									    int *major_near, int *major_far,
									    int *minor_near, int *minor_far)
										const;
	void				getChildMargins(VGeoFrame *,
									    int *major_near, int *major_far,
									    int *minor_near, int *minor_far)
										const;
	void				getChildMargins(VWindow *,
									    int *major_near, int *major_far,
									    int *minor_near, int *minor_far)
										const;

	void				setChildrenStretchability(float);
	void				setChildStretchability(int child_index, float);
	void				setChildStretchability(VGeoFrame *, float);
	void				setChildStretchability(VWindow *, float);
	float				getChildStretchability(int child_index) const;
	float				getChildStretchability(VGeoFrame *) const;
	float				getChildStretchability(VWindow *) const;

	void				setChildrenShrinkability(float);
	void				setChildShrinkability(int child_index, float);
	void				setChildShrinkability(VGeoFrame *, float);
	void				setChildShrinkability(VWindow *, float);
	float				getChildShrinkability(int child_index) const;
	float				getChildShrinkability(VGeoFrame *) const;
	float				getChildShrinkability(VWindow *) const;

	void				setChildrenMinorStretchability(float);
	void				setChildMinorStretchability(int child_index, float);
	void				setChildMinorStretchability(VGeoFrame *, float);
	void				setChildMinorStretchability(VWindow *, float);
	float				getChildMinorStretchability(int child_index) const;
	float				getChildMinorStretchability(VGeoFrame *) const;
	float				getChildMinorStretchability(VWindow *) const;

	void				setChildrenMinorShrinkability(float);
	void				setChildMinorShrinkability(int child_index, float);
	void				setChildMinorShrinkability(VGeoFrame *, float);
	void				setChildMinorShrinkability(VWindow *, float);
	float				getChildMinorShrinkability(int child_index) const;
	float				getChildMinorShrinkability(VGeoFrame *) const;
	float				getChildMinorShrinkability(VWindow *) const;

	void				setChildrenElasticity(float);
	void				setChildElasticity(int child_index, float);
	void				setChildElasticity(VGeoFrame *, float);
	void				setChildElasticity(VWindow *, float);

	void				setChildrenMinorElasticity(float);
	void				setChildMinorElasticity(int child_index, float);
	void				setChildMinorElasticity(VGeoFrame *, float);
	void				setChildMinorElasticity(VWindow *, float);

	// Old names.

	void				setSpacing(SpacingStyle);
	SpacingStyle		getSpacing() const;
	boolean				getWrapping() const;
	void				setEqualMajorDimension(boolean);
	boolean				getEqualMajorDimension() const;
	void				setEqualMinorDimension(boolean);
	boolean				getEqualMinorDimension() const;
	void				setDefaultChildJustify(JustifyStyle);
	JustifyStyle		getDefaultChildJustify() const;
	void				setChildrenJustify(JustifyStyle);
	void				setChildJustify(int child_index, JustifyStyle);
	JustifyStyle		getChildJustify(int child_index) const;

	// Archiving.

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
	virtual void		frameChildren(int w = 0, int h = 0);
	virtual void		getPreferredSize(int *w, int *h);
	virtual boolean		getConstrainedSize(int *w, int *h, int, int);
	virtual void		getMinimumSize(int *w, int *h);

protected:
	void				initialize(VGeoRowOrColumn::Dimension);
	void				getPreferredSizeWithChildren(int *, int *,
													 boolean = FALSE,
													 int = 0, int = 0);
	void				setupLayoutData(void *, int = 0, int = 0);
	int					setupRowOrColumnData(ChildIterator&,
											 void *, int = 0);
	void				setupChildSize(VGeoFrame *, int *, int *,
									   boolean = FALSE);
	void				layoutFromData(void *, int);
private:
						VGeoRowOrColumn(const VGeoRowOrColumn&);
	VGeoRowOrColumn&	operator=(const VGeoRowOrColumn&);

public:

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Each VGeoFrame which is a child of a VGeoRowOrColumn will have
	// a ChildData object which will be pointed to by its VGeoFrame::
	// childData member.  This contains the per-child information
	// needed by the parent VGeoRowOrColumn manager.
	//
	CLASS RowOrColumnChildData : public VGeoManager::ManagerChildData {
	public:
						RowOrColumnChildData(const VGeoRowOrColumn *);
#if !defined(CV_NOARCHIVER)
		virtual void	putTo(VArchiver&);
		virtual void	getFrom(VArchiver&);
#endif
	public:
		float			stretchAbility;
		float			shrinkAbility;
		float			minorStretchAbility;
		float			minorShrinkAbility;
		short			leftMargin;
		short			rightMargin;
		short			topMargin;
		short			bottomMargin;
		short			majorSize;
		short			minorSize;
		char			justifyStyle;
		char			ignoreShrinkAbility : 1;
		char			ignoreEqualMajorSize : 1;
		char			ignoreEqualMinorSize : 1;
	};

public:
	virtual VGeoFrame::ChildData *newChildData() const;

protected:

#if !defined(CV_NOARCHIVER)
    virtual void		putTo(VArchiver&, boolean);
	virtual void		getFrom(VArchiver&);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	float				spaceStretchAbility;
	float				spaceShrinkAbility;
	float				defaultChildStretchAbility;
	float				defaultChildShrinkAbility;
	float				defaultChildMinorStretchAbility;
	float				defaultChildMinorShrinkAbility;
	short				leftMargin;
	short				rightMargin;
	short				topMargin;
	short				bottomMargin;
	short				preferredSpacing;
	short				preferredNumberOfRowsOrColumns;
	short				defaultChildLeftMargin;
	short				defaultChildRightMargin;
	short				defaultChildTopMargin;
	short				defaultChildBottomMargin;
	char				defaultChildJustifyStyle;
	char				spacingStyle;
	char				preferredMinorSpacing;
	char				equalChildMajor : 1;
	char				equalChildMinor : 1;
	char				wrappingEnabled : 1;
	char				minimizeConstrainedMajor : 1;
	char				assumeWrappingOnGetConstrainedSize : 1;

private:
	friend				RowOrColumnChildData;
};

extern VClass *VGeoRowOrColumnCls;

// --------------------------------------------------------------------------
//
inline void VGeoRowOrColumn::setSpacingStyle(VGeoRowOrColumn::SpacingStyle s)
{
	spacingStyle = char(s);
}
inline VGeoRowOrColumn::SpacingStyle VGeoRowOrColumn::getSpacingStyle() const
{
	return SpacingStyle(spacingStyle);
}

inline void VGeoRowOrColumn::setSpacing(VGeoRowOrColumn::SpacingStyle s)
{
	spacingStyle = char(s);
}
inline VGeoRowOrColumn::SpacingStyle VGeoRowOrColumn::getSpacing() const
{
	return SpacingStyle(spacingStyle);
}

inline void VGeoRowOrColumn::setPreferredSpacing(int i)
{
	preferredSpacing = short(i) < short(0) ? short(0) : short(i);
}
inline int VGeoRowOrColumn::getPreferredSpacing() const
{
	return preferredSpacing;
}

inline void VGeoRowOrColumn::setPreferredMinorSpacing(int i)
{
	if (i < 0) {
		preferredMinorSpacing = 0;
	}
	else if (i > 127) {
		preferredMinorSpacing = 127;
	}
	else {
		preferredMinorSpacing = i;
	}
}
inline int VGeoRowOrColumn::getPreferredMinorSpacing() const
{
	return preferredMinorSpacing;
}

inline void VGeoRowOrColumn::setEqualSize(boolean b)
{
	equalChildMajor = char(b);
}
inline boolean VGeoRowOrColumn::getEqualSize() const
{
	return equalChildMajor;
}

inline void VGeoRowOrColumn::setEqualMinorSize(boolean b)
{
	equalChildMinor = char(b);
}

inline boolean VGeoRowOrColumn::getEqualMinorSize() const
{
	return equalChildMinor;
}

inline void VGeoRowOrColumn::setEqualMajorDimension(boolean b)
{
	equalChildMajor = char(b);
}
inline boolean VGeoRowOrColumn::getEqualMajorDimension() const
{
	return equalChildMajor;
}

inline void VGeoRowOrColumn::setEqualMinorDimension(boolean b)
{
	equalChildMinor = char(b);
}

inline boolean VGeoRowOrColumn::getEqualMinorDimension() const
{
	return equalChildMinor;
}

inline void VGeoRowOrColumn::setSpaceStretchability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	spaceStretchAbility = f;
}
inline float VGeoRowOrColumn::getSpaceStretchability() const
{
	return spaceStretchAbility;
}

inline void VGeoRowOrColumn::setSpaceShrinkability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	spaceShrinkAbility = f;
}
inline float VGeoRowOrColumn::getSpaceShrinkability() const
{
	return spaceShrinkAbility;
}

inline void VGeoRowOrColumn::setWrapping(boolean b)
{
	if (b) {
		if (!wrappingEnabled) {
			wrappingEnabled = 1;
		}
	}
	else if (wrappingEnabled) {
		wrappingEnabled = 0;
	}
}
inline boolean VGeoRowOrColumn::isWrapping() const
{
	return wrappingEnabled;
}
inline boolean VGeoRowOrColumn::getWrapping() const
{
	return wrappingEnabled;
}

inline void VGeoRowOrColumn::setPreferredNumberOfRowsOrColumns(int i)
{
	preferredNumberOfRowsOrColumns =
		short(i) < short(1) ? short(1) : short(i);
}
inline int VGeoRowOrColumn::getPreferredNumberOfRowsOrColumns() const
{
	return preferredNumberOfRowsOrColumns;
}

inline void VGeoRowOrColumn::setMinimizeConstrainedMajorDimension(boolean b)
{
	minimizeConstrainedMajor = char(b);
}
inline boolean VGeoRowOrColumn::getMinimizeConstrainedMajorDimension() const
{
	return minimizeConstrainedMajor;
}

inline void VGeoRowOrColumn::setAssumeWrappingOnGetConstrainedSize(boolean b)
{
	assumeWrappingOnGetConstrainedSize = b;
}

inline boolean VGeoRowOrColumn::getAssumeWrappingOnGetConstrainedSize() const
{
	return assumeWrappingOnGetConstrainedSize;
}


inline VGeoRowOrColumn::Dimension VGeoRowOrColumn::getDimension() const
{
	if (isGeoRow) {
		return Row;
	}
	else {
		return Column;
	}
}

// --------------------------------------------------------------------------
//
inline void VGeoRowOrColumn::setDefaultChildJustifyStyle
							(VGeoRowOrColumn::JustifyStyle j)
{
	defaultChildJustifyStyle = char(j);
}

inline VGeoRowOrColumn::JustifyStyle VGeoRowOrColumn::
									 getDefaultChildJustifyStyle() const
{
	return JustifyStyle(defaultChildJustifyStyle);
}

inline void VGeoRowOrColumn::setDefaultChildJustify(VGeoRowOrColumn::
													JustifyStyle j)
{
	defaultChildJustifyStyle = char(j);
}

inline VGeoRowOrColumn::JustifyStyle VGeoRowOrColumn::
									 getDefaultChildJustify() const
{
	return JustifyStyle(defaultChildJustifyStyle);
}

inline void VGeoRowOrColumn::setDefaultChildStretchability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	defaultChildStretchAbility = f;
}
inline float VGeoRowOrColumn::getDefaultChildStretchability() const
{
	return defaultChildStretchAbility;
}

inline void VGeoRowOrColumn::setDefaultChildShrinkability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	defaultChildShrinkAbility = f;
}
inline float VGeoRowOrColumn::getDefaultChildShrinkability() const
{
	return defaultChildShrinkAbility;
}

inline void VGeoRowOrColumn::setDefaultChildMinorStretchability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	defaultChildMinorStretchAbility = f;
}
inline float VGeoRowOrColumn::getDefaultChildMinorStretchability() const
{
	return defaultChildMinorStretchAbility;
}

inline void VGeoRowOrColumn::setDefaultChildMinorShrinkability(float f)
{
	if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
	defaultChildMinorShrinkAbility = f;
}
inline float VGeoRowOrColumn::getDefaultChildMinorShrinkability() const
{
	return defaultChildMinorShrinkAbility;
}

// --------------------------------------------------------------------------
//
inline void VGeoRowOrColumn::
			setChildJustifyStyle(VGeoFrame *geoframe,
								 VGeoRowOrColumn::JustifyStyle justify)
{
	setChildJustifyStyle(geoframe->getIndex(), justify);
}
inline void VGeoRowOrColumn::
			setChildJustifyStyle(VWindow *window,
								 VGeoRowOrColumn::JustifyStyle justify)
{
	setChildJustifyStyle(window->getGeoIndex(), justify);
}
inline VGeoRowOrColumn::JustifyStyle
	   VGeoRowOrColumn::getChildJustifyStyle(VGeoFrame *geoframe) const
{
	return getChildJustifyStyle(geoframe->getIndex());
}
inline VGeoRowOrColumn::JustifyStyle
	   VGeoRowOrColumn::getChildJustifyStyle(VWindow *window) const
{
	return getChildJustifyStyle(window->getGeoIndex());
}
inline void VGeoRowOrColumn::
			setChildMargins(VGeoFrame *geoframe, int all)
{
	setChildMargins(geoframe->getIndex(), all);
}
inline void VGeoRowOrColumn::
			setChildMargins(VGeoFrame *geoframe, int major, int minor)
{
	setChildMargins(geoframe->getIndex(), major, minor);
}
inline void VGeoRowOrColumn::
			setChildMargins(VGeoFrame *geoframe,
							int major_near, int major_far,
							int minor_near, int minor_far)
{
	setChildMargins(geoframe->getIndex(),
					major_near, major_far, minor_near, minor_far);
}
inline void VGeoRowOrColumn::
			setChildMargins(VWindow *geoframe, int all)
{
	setChildMargins(geoframe->getGeoIndex(), all);
}
inline void VGeoRowOrColumn::
			setChildMargins(VWindow *window, int major, int minor)
{
	setChildMargins(window->getGeoIndex(), major, minor);
}
inline void VGeoRowOrColumn::
			setChildMargins(VWindow *window,
							int major_near, int major_far,
							int minor_near, int minor_far)
{
	setChildMargins(window->getGeoIndex(),
					major_near, major_far, minor_near, minor_far);
}
inline void VGeoRowOrColumn::
			getChildMargins(VGeoFrame *geoframe,
							int *major_near, int *major_far,
							int *minor_near, int *minor_far) const
{
	getChildMargins(geoframe->getIndex(),
					major_near, major_far, minor_near, minor_far);
}
inline void VGeoRowOrColumn::
			getChildMargins(VWindow *window,
							int *major_near, int *major_far,
							int *minor_near, int *minor_far) const
{
	getChildMargins(window->getGeoIndex(),
					major_near, major_far, minor_near, minor_far);
}
inline void VGeoRowOrColumn::
			setChildStretchability(VGeoFrame *geoframe, float f)
{
	setChildStretchability(geoframe->getIndex(), f);
}
inline void VGeoRowOrColumn::
			setChildStretchability(VWindow *window, float f)
{
	setChildStretchability(window->getGeoIndex(), f);
}
inline float VGeoRowOrColumn::
			 getChildStretchability(VGeoFrame *geoframe) const
{
	return getChildStretchability(geoframe->getIndex());
}
inline float VGeoRowOrColumn::
			 getChildStretchability(VWindow *window) const
{
	return getChildStretchability(window->getGeoIndex());
}
inline void VGeoRowOrColumn::
			setChildShrinkability(VGeoFrame *geoframe, float f)
{
	setChildShrinkability(geoframe->getIndex(), f);
}
inline void VGeoRowOrColumn::
			setChildShrinkability(VWindow *window, float f)
{
	setChildShrinkability(window->getGeoIndex(), f);
}
inline float VGeoRowOrColumn::
			 getChildShrinkability(VGeoFrame *geoframe) const
{
	return getChildShrinkability(geoframe->getIndex());
}
inline float VGeoRowOrColumn::
			 getChildShrinkability(VWindow *window) const
{
	return getChildShrinkability(window->getGeoIndex());
}
inline void VGeoRowOrColumn::
			setChildMinorStretchability(VGeoFrame *geoframe, float f)
{
	setChildMinorStretchability(geoframe->getIndex(), f);
}
inline void VGeoRowOrColumn::
			setChildMinorStretchability(VWindow *window, float f)
{
	setChildMinorStretchability(window->getGeoIndex(), f);
}
inline float VGeoRowOrColumn::
			getChildMinorStretchability(VGeoFrame *geoframe) const
{
	return getChildMinorStretchability(geoframe->getIndex());
}
inline float VGeoRowOrColumn::
			 getChildMinorStretchability(VWindow *window) const
{
	return getChildMinorStretchability(window->getGeoIndex());
}
inline void VGeoRowOrColumn::
			setChildMinorShrinkability(VGeoFrame *geoframe, float f)
{
	setChildMinorShrinkability(geoframe->getIndex(), f);
}
inline void VGeoRowOrColumn::
			setChildMinorShrinkability(VWindow *window, float f)
{
	setChildMinorShrinkability(window->getGeoIndex(), f);
}
inline float VGeoRowOrColumn::
			 getChildMinorShrinkability(VGeoFrame *geoframe) const
{
	return getChildMinorShrinkability(geoframe->getIndex());
}
inline float VGeoRowOrColumn::
			 getChildMinorShrinkability(VWindow *window) const
{
	return getChildMinorShrinkability(window->getGeoIndex());
}

inline void VGeoRowOrColumn::setSpaceElasticity(float f)
{
	setSpaceStretchability(f);
	setSpaceShrinkability(f);
}

inline void VGeoRowOrColumn::setDefaultChildElasticity(float f)
{
	setDefaultChildStretchability(f);
	setDefaultChildShrinkability(f);
}

inline void VGeoRowOrColumn::setDefaultChildMinorElasticity(float f)
{
	setDefaultChildMinorStretchability(f);
	setDefaultChildMinorShrinkability(f);
}

inline void VGeoRowOrColumn::setChildrenElasticity(float f)
{
	setChildrenStretchability(f);
	setChildrenShrinkability(f);
}

inline void VGeoRowOrColumn::setChildElasticity(int child, float f)
{
	setChildStretchability(child, f);
	setChildShrinkability(child, f);
}

inline void VGeoRowOrColumn::setChildElasticity(VGeoFrame *child, float f)
{
	setChildStretchability(child, f);
	setChildShrinkability(child, f);
}

inline void VGeoRowOrColumn::setChildElasticity(VWindow *child, float f)
{
	setChildStretchability(child, f);
	setChildShrinkability(child, f);
}

inline void VGeoRowOrColumn::setChildrenMinorElasticity(float f)
{
	setChildrenMinorStretchability(f);
	setChildrenMinorShrinkability(f);
}

inline void VGeoRowOrColumn::setChildMinorElasticity(int child, float f)
{
	setChildMinorStretchability(child, f);
	setChildMinorShrinkability(child, f);
}

inline void VGeoRowOrColumn::setChildMinorElasticity(VGeoFrame *child, float f)
{
	setChildMinorStretchability(child, f);
	setChildMinorShrinkability(child, f);
}

inline void VGeoRowOrColumn::setChildMinorElasticity(VWindow *child, float f)
{
	setChildMinorStretchability(child, f);
	setChildMinorShrinkability(child, f);
}

// --------------------------------------------------------------------------
//
inline VGeoRowOrColumn::RowOrColumnChildData::
						RowOrColumnChildData(const VGeoRowOrColumn *parent)
#if defined(CV_WINDOWS) && (_MSC_VER <= 800)
	: ManagerChildData(parent)
#else
	: VGeoManager::ManagerChildData(parent)
#endif
{
	justifyStyle		= parent->defaultChildJustifyStyle;
	leftMargin			= parent->defaultChildLeftMargin;
	rightMargin			= parent->defaultChildRightMargin;
	topMargin			= parent->defaultChildTopMargin;
	bottomMargin		= parent->defaultChildBottomMargin;
	stretchAbility		= parent->defaultChildStretchAbility;
	shrinkAbility		= parent->defaultChildShrinkAbility;
	minorStretchAbility	= parent->defaultChildMinorStretchAbility;
	minorShrinkAbility	= parent->defaultChildMinorShrinkAbility;
	//
	// The following members are used only for temporary
	// caching data and don't need to be initialized.
	//
	//	majorSize
	//	minorSize
	//	ignoreShrinkAbility
	//	ignoreEqualMajorSize
	//	ignoreEqualMinorSize
	//
}

#endif // VGEOROC_H

