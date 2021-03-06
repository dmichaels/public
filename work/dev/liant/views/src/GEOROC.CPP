// $Header:   Q:/views/common/vcs/GEOROC.CPV   1.16   Mar 24 1997 16:09:12   scottt  $

//	georoc.cpp
//
//	VGeoRowOrColumn implementation [Common]
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
//	11/14/95 dgm	Implemented preferred-spacing for no-spacing case.
//	11/15/95 dgm	Updated.
//	11/20/95 dgm	Changed to use RowOrColumnDataArray class as I originally
//					intended rather than using the large (1024) array which
//					was only being used for debugging purposes.
//	11/20/95 dgm	Added set/getPreferredMinorSpacing().
//	11/20/95 dgm	Call begin/endFrameChildren() from frameChildren().
//	11/20/95 dgm	Removed constrainPreferredChildMinor functionality.
//	11/28/95 dgm	Fix in getConstrainedSize() to take internal borders
//					into proper account.
//	01/15/96 dgm	Miscellaneous name changes.
//	01/17/96 dgm	Margin fix in setupRowOrColumnData().
//	01/29/96 dgm	Size-group changes.
//	02/16/96 dgm	Support to query VWindow associated with VGeoManager.
//	02/23/96 dgm	SizeGroup changes.
//	04/15/96 dgm	Added archiving of preferredMinorSpacing (oversight).
//	08/27/96 dgm	Fix for preferredSpacing and NoSpacingOpposite.
//	08/27/96 dgm	Implemented previously stubbed
//					setPreferredNumberOfRowsOrColumns()
//					and added getNumberOfRowsOrColumns().
//	08/30/96 dgm	Fix for preferred-number-of-rows-or-columns.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "georoc.h"
#include "ordcllct.h"
#include "iterator.h"

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
defineClass(VGeoRowOrColumn, VGeoManager)
//
// Revision 0: Original.
// Revision 1: preferred-minor-spacing.
//
defineArchiveRevision(VGeoRowOrColumn, 1)

// --------------------------------------------------------------------------
//
extern VClass *VGeoRowCls;
extern VClass *VGeoColumnCls;

// --------------------------------------------------------------------------
//
#undef  getChildData
#define getChildData(child)	((RowOrColumnChildData *)((child)->childData))

#undef  C
#define C					((RowOrColumnChildData *)((child)->childData))

#undef  max
#define max(a, b)			(((a) >= (b)) ? (a) : (b))

// --------------------------------------------------------------------------
// The RowOrColumnData struct is used to represent the data pertaining to a
// single row or column within a row or column manager.  It is filled in and
// used each time the manager is resized or is queried as to its preferred
// size.  Note that there may be more than one sub rows or columns within
// a row or column manager if wrapping is enabled.
//
struct RowOrColumnData {
	short			nchildren;
	short			preferred_major_size;
	short			height;
	short			max_equal_major_size;
	short			max_equal_minor_size;
	short			space;
	short			nspaces;
	short			insufficient_equal_space;
	short			max_margin;
	short			extra_minor_space;
	short			insufficient_minor_space;
	short			top_margin;
	short			bottom_margin;
};

#if 0
	typedef RowOrColumnData RowOrColumnDataArray[1024];
#else
// --------------------------------------------------------------------------
// The RowOrColumnDataArray class provides a fast dynamic array of
// RowOrColumnData objects.
//
class RowOrColumnDataArray {
public:
								RowOrColumnDataArray();
							   ~RowOrColumnDataArray();
	RowOrColumnData&			operator[](int);
private:
	enum Constants {			totalBasicRowDataElements = 32 };
	RowOrColumnData				rowData[totalBasicRowDataElements];
	RowOrColumnData			   *moreRowData;
	int							totalRowDataElements;
};

inline RowOrColumnDataArray::RowOrColumnDataArray()
{
	moreRowData = 0;
	totalRowDataElements = totalBasicRowDataElements;
}

inline RowOrColumnDataArray::~RowOrColumnDataArray()
{
	if (moreRowData != 0) {
		delete [] moreRowData;
	}
}

RowOrColumnData& RowOrColumnDataArray::operator[](int i)
{
	if (i <= 0) {
		return rowData[0];
	}
	else if (i < totalBasicRowDataElements) {
		return rowData[i];
	}
	else if (i < totalRowDataElements) {
		return moreRowData[i - totalBasicRowDataElements];
	}
	if (i > totalRowDataElements) {
		i = totalRowDataElements;
	}
	if (moreRowData == 0) {
		moreRowData = new RowOrColumnData[totalBasicRowDataElements];
	}
	else {
		RowOrColumnData *new_row_data =
			new RowOrColumnData[totalRowDataElements +
								totalBasicRowDataElements];
		for (int n = 0 ; n < totalRowDataElements -
							 totalBasicRowDataElements ; n++) {
			new_row_data[n] = moreRowData[n];
		}
		delete [] moreRowData;
		moreRowData = new_row_data;
	}
	totalRowDataElements += totalBasicRowDataElements;
	return moreRowData[i - totalBasicRowDataElements];
}
#endif

// --------------------------------------------------------------------------
//
struct LayoutData {
	RowOrColumnDataArray	row;
	short					nrows;
	short					nchildren;
	short					preferred_major_size;
	short					preferred_minor_size;
};

// --------------------------------------------------------------------------
// The SpaceBroker class provides a simple means by which extra (or
// insufficient) space can be divided up evenly between a number of
// items or a number of parts each desiring a relative amount.
//
class SpaceBroker {
public:
			SpaceBroker();
			SpaceBroker(int total_space, int nspaces,
						float total_space_parts = 0);
	void	initialize(int total_space, int nspaces,
					   float total_space_parts = 0);
	int		next();
	int		next(float space_part);
	void	giveback(int);
	void	reset();
private:
	int		totalSpace;
	int		numberOfSpaces;
	float	totalSpaceParts;
	int		spaceIndex;
	int		totalUsedSpace;
	float	totalUsedSpaceParts;
};

inline SpaceBroker::SpaceBroker()
{
}

inline void SpaceBroker::initialize(int total_space, int nspaces,
									float total_space_parts)
{
	totalSpace = total_space;
	numberOfSpaces = nspaces;
	totalSpaceParts = total_space_parts;
	spaceIndex = 1;
	totalUsedSpace = 0;
	totalUsedSpaceParts = 0.0F;
}

inline SpaceBroker::SpaceBroker(int total_space, int nspaces,
								float total_space_parts)
{
	initialize(total_space, nspaces, total_space_parts);
}

inline int SpaceBroker::next()
{
	if (spaceIndex > numberOfSpaces) {
		return 0;
	}
	int a = int(((float(spaceIndex) / numberOfSpaces) *
				 float(totalSpace)) + 0.5);
	int b = a - totalUsedSpace;
	totalUsedSpace = a;
	spaceIndex++;
	return b;
}

inline int SpaceBroker::next(float space_part)
{
	if (totalSpaceParts > 0.0F) {
		totalUsedSpaceParts += space_part;
		if (totalUsedSpaceParts > totalSpaceParts) {
			return 0;
		}
		int a = int(((float(totalUsedSpaceParts) / totalSpaceParts) *
					 float(totalSpace)) + 0.5);
		int b = a - totalUsedSpace;
		totalUsedSpace = a;
		return b;
	}
	else {
		return next();
	}
}

inline void SpaceBroker::giveback(int space)
{
	totalUsedSpace -= space;
}

inline void SpaceBroker::reset()
{
	spaceIndex = 1;
	totalUsedSpace = 0;
	totalUsedSpaceParts = 0.0F;
}

// --------------------------------------------------------------------------
// The MinorMargin class is used to track the minor dimension margins of
// a row or column.
//
class MinorMargin {
public:
			MinorMargin();
	void	add(VGeoRowOrColumn::RowOrColumnChildData *, int child_height);
	void	get(int max_child_height, int& top_margin, int& bottom_margin);
	void	reset();
private:
	int		maxTopMargin[3];
	int		maxTopMarginChildHeight[3];
	int		maxBottomMargin[3];
	int		maxBottomMarginChildHeight[3];
};

inline void	MinorMargin::add(VGeoRowOrColumn::
							 RowOrColumnChildData *child_data,
							 int child_height)
{
	if (child_data->justifyStyle == VGeoRowOrColumn::TopJustify) {
		if (child_data->topMargin > maxTopMargin[0]) {
			maxTopMargin[0] = child_data->topMargin;
			maxTopMarginChildHeight[0] = child_height;
		}
		if ((child_data->bottomMargin + child_height) >
			(maxBottomMargin[0] + maxBottomMarginChildHeight[0])) {
			maxBottomMargin[0] = child_data->bottomMargin;
			maxBottomMarginChildHeight[0] = child_height;
		}
	}
	else if (child_data->justifyStyle == VGeoRowOrColumn::BottomJustify) {
		if ((child_data->topMargin + child_height) >
			(maxTopMargin[1] + maxTopMarginChildHeight[1])) {
			maxTopMargin[1] = child_data->topMargin;
			maxTopMarginChildHeight[1] = child_height;
		}
		if (child_data->bottomMargin > maxBottomMargin[1]) {
			maxBottomMargin[1] = child_data->bottomMargin;
			maxBottomMarginChildHeight[1] = child_height;
		}
	}
	else {
		int half_ch = child_height / 2;
		if ((half_ch + child_data->topMargin) >
		 	((maxTopMarginChildHeight[2] / 2) + maxTopMargin[2])) {
			maxTopMargin[2] = child_data->topMargin;
			maxTopMarginChildHeight[2] = child_height;
		}
		if ((half_ch + child_data->bottomMargin) >
		 	((maxBottomMarginChildHeight[2] / 2) + maxBottomMargin[2])) {
			maxBottomMargin[2] = child_data->bottomMargin;
			maxBottomMarginChildHeight[2] = child_height;
		}
	}
}

inline void	MinorMargin::get(int max_child_height,
							 int& top_margin, int& bottom_margin)
{
	maxBottomMargin[0] -= (max_child_height - maxBottomMarginChildHeight[0]);
	if (maxBottomMargin[0] < 0) {
		maxBottomMargin[0] = 0;
	}
	maxTopMargin[1] -= (max_child_height - maxTopMarginChildHeight[1]);
	if (maxTopMargin[1] < 0) {
		maxTopMargin[1] = 0;
	}
	maxTopMargin[2] -= (max_child_height - maxTopMarginChildHeight[2]) / 2;
	if (maxTopMargin[2] < 0) {
		maxTopMargin[2] = 0;
	}
	maxBottomMargin[2] -= (max_child_height -
						   maxBottomMarginChildHeight[2]) / 2;
	if (maxBottomMargin[2] < 0) {
		maxBottomMargin[2] = 0;
	}
	if (maxTopMargin[0] > maxTopMargin[1]) {
		if (maxTopMargin[0] > maxTopMargin[2]) {
			top_margin = maxTopMargin[0];
		}
		else {
			top_margin = maxTopMargin[2];
		}
	}
	else if (maxTopMargin[1] > maxTopMargin[2]) {
		top_margin = maxTopMargin[1];
	}
	else {
		top_margin = maxTopMargin[2];
	}
	if (maxBottomMargin[0] > maxBottomMargin[1]) {
		if (maxBottomMargin[0] > maxBottomMargin[2]) {
			bottom_margin = maxBottomMargin[0];
		}
		else {
			bottom_margin = maxBottomMargin[2];
		}
	}
	else if (maxBottomMargin[1] > maxBottomMargin[2]) {
		bottom_margin = maxBottomMargin[1];
	}
	else {
		bottom_margin = maxBottomMargin[2];
	}
}

inline void	MinorMargin::reset()
{
	maxTopMargin[0] = 0;
	maxTopMargin[1] = 0;
	maxTopMargin[2] = 0;
	maxBottomMargin[0] = 0;
	maxBottomMargin[1] = 0;
	maxBottomMargin[2] = 0;
	maxTopMarginChildHeight[0] = 0;
	maxTopMarginChildHeight[1] = 0;
	maxTopMarginChildHeight[2] = 0;
	maxBottomMarginChildHeight[0] = 0;
	maxBottomMarginChildHeight[1] = 0;
	maxBottomMarginChildHeight[2] = 0;
}

inline MinorMargin::MinorMargin()
{
	reset();
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 const VFrame& frame, VGeoManager *parent)
	: VGeoManager(frame, parent)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 int x, int y, int w, int h,
								 VGeoManager *parent)
	: VGeoManager(x, y, w, h, parent)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 int w, int h, VGeoManager *parent)
	: VGeoManager(w, h, parent)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 VGeoManager *parent)
	: VGeoManager(parent)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 const VFrame& frame,
								 VWindow *parent_window)
	: VGeoManager(frame, parent_window)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension,
								 VWindow *parent_window)
	: VGeoManager(parent_window)
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn(VGeoRowOrColumn::Dimension dimension)
	: VGeoManager()
{
	initialize(dimension);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::VGeoRowOrColumn()
	: VGeoManager()
{
	initialize(Row);
}

// --------------------------------------------------------------------------
//
VGeoRowOrColumn::~VGeoRowOrColumn()
{
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::initialize(VGeoRowOrColumn::Dimension d)
{
	isGeoRowOrColumn				= char(1);

	if (d == Row) {
		isGeoRow					= char(1);
		isGeoColumn					= char(0);
	}
	else {
		isGeoRow					= char(0);
		isGeoColumn					= char(1);
	}

	spacingStyle					= EqualSpacing;
	preferredSpacing				= 0;
	preferredMinorSpacing			= 0;
	preferredNumberOfRowsOrColumns	= 1;
	minimizeConstrainedMajor		= char(1);

	assumeWrappingOnGetConstrainedSize = 0;

	leftMargin						= 0;
	rightMargin						= 0;
	topMargin						= 0;
	bottomMargin					= 0;

	equalChildMajor					= char(0);
	equalChildMinor					= char(0);

	spaceStretchAbility				= 1.0F;
	spaceShrinkAbility				= 1.0F;

	wrappingEnabled					= char(0);

	defaultChildJustifyStyle		= char(CenterJustify);

	defaultChildLeftMargin			= 0;
	defaultChildRightMargin			= 0;
	defaultChildTopMargin			= 0;
	defaultChildBottomMargin		= 0;

	defaultChildStretchAbility		= 0.0F;
	defaultChildShrinkAbility		= 0.0F;
	defaultChildMinorStretchAbility	= 0.0F;
	defaultChildMinorShrinkAbility	= 0.0F;
}

// --------------------------------------------------------------------------
// Set the internal margin thickness for this VGeoRowOrColumn.
//
void VGeoRowOrColumn::setMargins(int all)
{
	if (all < 0) { all = 0; }

	leftMargin   = all;
	rightMargin  = all;
	topMargin    = all;
	bottomMargin = all;
}
void VGeoRowOrColumn::setMargins(int major, int minor)
{
	if (major < 0) { major = 0; }
	if (minor < 0) { minor = 0; }

	leftMargin   = major;
	rightMargin  = major;
	topMargin    = minor;
	bottomMargin = minor;
}
void VGeoRowOrColumn::setMargins(int left, int right, int top, int bottom)
{
	if (left   < 0) { left   = 0; }
	if (right  < 0) { right  = 0; }
	if (top    < 0) { top    = 0; }
	if (bottom < 0) { bottom = 0; }

	leftMargin   = left;
	rightMargin  = right;
	topMargin    = top;
	bottomMargin = bottom;
}
void VGeoRowOrColumn::getMargins(int *left, int *right,
								 int *top, int *bottom) const
{
	if (left   != 0) { *left   = leftMargin;   }
	if (right  != 0) { *right  = rightMargin;  }
	if (top    != 0) { *top    = topMargin;    }
	if (bottom != 0) { *bottom = bottomMargin; }
}

// --------------------------------------------------------------------------
//
boolean VGeoRowOrColumn::setDimension(Dimension d)
{
	if (d == Row) {
		if (isGeoRow) {
			return TRUE;
		}
		else if (isA(VGeoColumnCls)) {
			return FALSE;
		}
		else {
			isGeoRow = 1;
			isGeoColumn = 0;
			return TRUE;
		}
	}
	else if (d == Column) {
		if (isGeoColumn) {
			return TRUE;
		}
		else if (isA(VGeoRowCls)) {
			return FALSE;
		}
		else {
			isGeoRow = 0;
			isGeoColumn = 1;
			return TRUE;
		}
	}
	else {
		return FALSE;
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setDefaultChildMargins(int all)
{
	if (all < 0) { all = 0; }

	defaultChildLeftMargin   = all;
	defaultChildRightMargin  = all;
	defaultChildTopMargin    = all;
	defaultChildBottomMargin = all;
}
void VGeoRowOrColumn::setDefaultChildMargins(int major, int minor)
{
	if (major < 0) { major = 0; }
	if (minor < 0) { minor = 0; }

	defaultChildLeftMargin   = major;
	defaultChildRightMargin  = major;
	defaultChildTopMargin    = minor;
	defaultChildBottomMargin = minor;
}
void VGeoRowOrColumn::setDefaultChildMargins(int left, int right,
											 int top, int bottom)
{
	if (left   < 0) { left   = 0; }
	if (right  < 0) { right  = 0; }
	if (top    < 0) { top    = 0; }
	if (bottom < 0) { bottom = 0; }

	defaultChildLeftMargin   = left;
	defaultChildRightMargin  = right;
	defaultChildTopMargin    = top;
	defaultChildBottomMargin = bottom;
}
void VGeoRowOrColumn::getDefaultChildMargins(int *left, int *right,
											 int *top, int *bottom) const
{
	if (left   != 0) { *left   = defaultChildLeftMargin;   }
	if (right  != 0) { *right  = defaultChildRightMargin;  }
	if (top    != 0) { *top    = defaultChildTopMargin;    }
	if (bottom != 0) { *bottom = defaultChildBottomMargin; }
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenJustifyStyle(VGeoRowOrColumn::JustifyStyle j)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildJustifyStyle(n, j);
	}
}
void VGeoRowOrColumn::setChildJustifyStyle(int child_index,
										   VGeoRowOrColumn::JustifyStyle j)
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		C->justifyStyle = j;
	}
}
VGeoRowOrColumn::JustifyStyle VGeoRowOrColumn::
							  getChildJustifyStyle(int child_index) const
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		return JustifyStyle(C->justifyStyle);
	}
	else {
		return JustifyStyle(defaultChildJustifyStyle);
	}
}
void VGeoRowOrColumn::setChildrenJustify(VGeoRowOrColumn::JustifyStyle j)
{
	setChildrenJustifyStyle(j);
}
void VGeoRowOrColumn::setChildJustify(int child_index,
									  VGeoRowOrColumn::JustifyStyle j)
{
	setChildJustifyStyle(child_index, j);
}
VGeoRowOrColumn::JustifyStyle VGeoRowOrColumn::
							  getChildJustify(int child_index) const
{
	return getChildJustifyStyle(child_index);
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenMargins(int all)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildMargins(n, all);
	}
}
void VGeoRowOrColumn::setChildrenMargins(int major, int minor)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildMargins(n, major, minor);
	}
}
void VGeoRowOrColumn::setChildrenMargins(int left, int right,
										 int top, int bottom)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildMargins(n, left, right, top, bottom);
	}
}
void VGeoRowOrColumn::setChildMargins(int child_index,
									  int left, int right,
									  int top, int bottom)
{
	if (left   < 0) { left   = 0; }
	if (right  < 0) { right  = 0; }
	if (top    < 0) { top    = 0; }
	if (bottom < 0) { bottom = 0; }

	VGeoFrame *child = getChild(child_index);

	if (child == 0) {
		return;
	}

	C->leftMargin   = left;
	C->rightMargin  = right;
	C->topMargin    = top;
	C->bottomMargin = bottom;
}
void VGeoRowOrColumn::setChildMargins(int child_index, int all)
{
	if (all < 0) { all = 0; }

	VGeoFrame *child = getChild(child_index);

	if (child == 0) {
		return;
	}

	C->leftMargin   = all;
	C->rightMargin  = all;
	C->topMargin    = all;
	C->bottomMargin = all;
}
void VGeoRowOrColumn::setChildMargins(int child_index, int major, int minor)
{
	if (major < 0) { major = 0; }
	if (minor < 0) { minor = 0; }

	VGeoFrame *child = getChild(child_index);

	if (child == 0) {
		return;
	}

	C->leftMargin   = major;
	C->rightMargin  = major;
	C->topMargin    = minor;
	C->bottomMargin = minor;
}
void VGeoRowOrColumn::getChildMargins(int child_index,
									  int *left, int *right,
									  int *top, int *bottom) const
{
	VGeoFrame *child = getChild(child_index);

	if (child == 0) {
		return;
	}

	if (left   != 0) { *left   = C->leftMargin;   }
	if (right  != 0) { *right  = C->rightMargin;  }
	if (top    != 0) { *top    = C->topMargin;    }
	if (bottom != 0) { *bottom = C->bottomMargin; }
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenStretchability(float f)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildStretchability(n, f);
	}
}
void VGeoRowOrColumn::setChildStretchability(int child_index, float f)
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		if (f < 0.0F) { f = 0.0F; }
		C->stretchAbility = f;
	}
}
float VGeoRowOrColumn::getChildStretchability(int child_index) const
{
	VGeoFrame *child = getChild(child_index);
	if (child == 0) {
		return 0.0F;
	}
	else {
		return C->stretchAbility;
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenShrinkability(float f)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildShrinkability(n, f);
	}
}
void VGeoRowOrColumn::setChildShrinkability(int child_index, float f)
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		if (f < 0.0F) { f = 0.0F; }
		C->shrinkAbility = f;
	}
}
float VGeoRowOrColumn::getChildShrinkability(int child_index) const
{
	VGeoFrame *child = getChild(child_index);
	if (child == 0) {
		return 0.0F;
	}
	else {
		return C->shrinkAbility;
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenMinorStretchability(float f)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildMinorStretchability(n, f);
	}
}
void VGeoRowOrColumn::setChildMinorStretchability(int child_index, float f)
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
		C->minorStretchAbility = f;
	}
}
float VGeoRowOrColumn::getChildMinorStretchability(int child_index) const
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		return C->minorStretchAbility;
	}
	else {
		return 0.0F;
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::setChildrenMinorShrinkability(float f)
{
	for (int n = getNumberOfChildren() - 1 ; n >= 0 ; n--) {
		setChildMinorShrinkability(n, f);
	}
}
void VGeoRowOrColumn::setChildMinorShrinkability(int child_index, float f)
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		if (f > 1.0F) { f = 1.0F; } else if (f < 0.0F) { f = 0.0F; }
		C->minorShrinkAbility = f;
	}
}
float VGeoRowOrColumn::getChildMinorShrinkability(int child_index) const
{
	VGeoFrame *child = getChild(child_index);
	if (child != 0) {
		return C->minorShrinkAbility;
	}
	else {
		return 0.0F;
	}
}

// --------------------------------------------------------------------------
// Allocate a new instance of a ChildData to hold the child specific
// "constraint" data necessary for this VGeoRowOrColumn to perform its geometry
// management duties.  This virtual function is called from the (VGeoFrame)
// base class whenever a VGeoFrame is added as a child of this VGeoRowOrColumn.
//
VGeoFrame::ChildData *VGeoRowOrColumn::newChildData() const
{
	return new RowOrColumnChildData(this);
}

// --------------------------------------------------------------------------
// Return in (w, h) the preferred size of this VGeoRowOrColumn.  If this
// VGeoRowOrColumn has any children then the preferred size will be based
// on the preferred sizes and current constraint settings of the children
// as well as the current settings of, and the layout policy defined by
// this VGeoRowOrColumn.
//
void VGeoRowOrColumn::getPreferredSize(int *w, int *h)
{
	if (!hasChildren()) {
		VGeoManager::getPreferredSize(w, h);
		return;
	}

	setupSizeGroups();
	if (isGeoRow) {
		getPreferredSizeWithChildren(w, h, FALSE);
	}
	else {
		getPreferredSizeWithChildren(h, w, FALSE);
	}
	checkWindowPreferredSize(w, h);
	addNonClientAreaSize(w, h);
}

// --------------------------------------------------------------------------
//
boolean VGeoRowOrColumn::getConstrainedSize(int *w, int *h,
										    int constrained_w,
											int constrained_h)
{
	if (!hasChildren()) {
		return VGeoManager::
			   getConstrainedSize(w, h, constrained_w, constrained_h);
	}

	setupSizeGroups();
	subtractInternalBorderSize(constrained_w > 0 ? &constrained_w : 0,
							   constrained_h > 0 ? &constrained_h : 0);
	if (isGeoRow) {
		getPreferredSizeWithChildren(w, h, FALSE, constrained_w,
												  constrained_h);
	}
	else {
		getPreferredSizeWithChildren(h, w, FALSE, constrained_h,
												  constrained_w);
	}
	checkWindowConstrainedSize(w, h, constrained_w, constrained_h);
	addNonClientAreaSize(w, h);
	return TRUE;
}

// --------------------------------------------------------------------------
// Return in (w, h) the minimum size for this VGeoRowOrColumn.  If this
// VGeoRowOrColumn has any children then the minimum size will be based
// on the minimum sizes and current constraint settings of the children
// as well as the current settings of, and the layout policy defined by,
// this VGeoRowOrColumn.  If any of (w, h) are null, then they'll be ignored.
//
void VGeoRowOrColumn::getMinimumSize(int *w, int *h)
{
	if (!hasChildren()) {
		VGeoManager::getMinimumSize(w, h);
		return;
	}

	setupSizeGroups();
	if (isGeoRow) {
		getPreferredSizeWithChildren(w, h, TRUE);
	}
	else {
		getPreferredSizeWithChildren(h, w, TRUE);
	}
	checkWindowMinimumSize(w, h);
	addNonClientAreaSize(w, h);
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the size which this VGeoRowOrColumn, which has
// children, wants to be.  The size is based then the preferred or minimum
// (depending on the given minimum flag) sizes and current constraint
// settings of the children as well as the current settings of, and the
// layout policy defined by, this VGeoRowOrColumn.  Note that the preferred
// size cache is not checked.  Note that If any of (w, h) are null, then
// they will be ignored.
//
void VGeoRowOrColumn::getPreferredSizeWithChildren(int *w, int *h,
												   boolean minimum,
												   int constrained_w,
												   int constrained_h)
{
	int save_wrapping_enabled = wrappingEnabled;
	if (assumeWrappingOnGetConstrainedSize) {
		wrappingEnabled = TRUE;
	}

	LayoutData layout;

	if (constrained_h > 0) {
		if (constrained_w > 0) {
			//
			// Here, both dimensions are constrained!
			// What's the right thing to do?
			// We used to just report our original non-constrianed
			// idea of our preferred size, but I think it's more
			// useful to treat this identically to the case where
			// the major dimension is constrained and the minor
			// dimension is not (or maybe the other way around?).
			// Actually, we should probably figure the optimal
			// size based on both constraints as you would expect,
			// giving precedence to the major dimension.  ToDo!!!
			//
			//	setupLayoutData(&layout);
			//
			goto ConstrainedMajor;
		}
		else {
			//
			// Here, the height is constrained
			// but the major dimension is not.
			//
			setupLayoutData(&layout);
			int preferred_major_size = layout.preferred_major_size;
			int preferred_minor_size = layout.preferred_minor_size;
			if (preferred_minor_size < constrained_h) {
				while (1) {
					setupLayoutData(&layout, preferred_major_size - 1);
					if ((layout.preferred_minor_size > constrained_h) ||
						(layout.preferred_major_size >=
								preferred_major_size)) {
						break;
					}
					preferred_minor_size = layout.preferred_minor_size;
					preferred_major_size = layout.preferred_major_size;
				}
			}
			layout.preferred_major_size = preferred_major_size;
			layout.preferred_minor_size = preferred_minor_size;
		}
	}
	else if (constrained_w > 0) {
		//
		// Here, the major dimension is constrained but the minor
		// dimension is not.  We are being asked the question:
		// "Given the specified major dimension size, what would
		// you like your minor dimension size to be?".  In addition,
		// if the given major dimension pointer (w) is not null, we
		// are being asked the additional question: "Would you like
		// your major dimension size to be even smaller?".
		//
	ConstrainedMajor:
		setupLayoutData(&layout, constrained_w, constrained_h);
		if (wrappingEnabled && minimizeConstrainedMajor &&
			(layout.nrows > 1) && (layout.nrows < layout.nchildren)) {
			//
			// Now, we would like to try to minimize our major dimension
			// size given then number of sub-rows or sub-columns we have.
			// E.g. if this is a row containing four children each having
			// an equal width, and the given constrained width dictated
			// that the there would be three children in the first sub-row
			// and one in the second sub-row, it would be nice if we could
			// indicate to our caller that (since we require two sub-rows
			// anyways) we can *really* be even thinner, that is, we can
			// be thin enough to allow two children on the first sub-row
			// and two on the second.
			//
			int nrows = layout.nrows;
			int preferred_major_size = layout.preferred_major_size;
			int preferred_minor_size = layout.preferred_minor_size;
			while (1) {
				setupLayoutData(&layout, preferred_major_size - 1, 0);
				if ((layout.nrows != nrows) ||
					(layout.preferred_minor_size != preferred_minor_size) ||
					(layout.preferred_major_size >= preferred_major_size)) {
					break;
				}
				preferred_major_size = layout.preferred_major_size;
			}
			layout.preferred_major_size = preferred_major_size;
			layout.preferred_minor_size = preferred_minor_size;
		}
	}
	else if (minimum) {
		if (wrappingEnabled) {
			//
			// Here, the minimum size is being requested, and wrapping is
			// enabled.  This is a bit of a weird case; we respond with the
			// minimum width assuming *maximum* wrapping (i.e. one child
			// per sub-row), and the minimum height assuming *no* wrapping.
			//
			if (w != 0) {
				setupLayoutData(&layout, 1, 32767);
				*w = layout.preferred_major_size;
			}
			if (h != 0) {
				setupLayoutData(&layout, -1);
				*h = layout.preferred_minor_size;
			}
			goto Done;
		}
		else {
			setupLayoutData(&layout, -1);
		}
	}
	else {
		if (wrappingEnabled && (preferredNumberOfRowsOrColumns > 1)) {
			setupLayoutData(&layout);
			int preferred_major_size = 32767, preferred_minor_size;
			while (1) {
				setupLayoutData(&layout, layout.preferred_major_size - 1, 0);
				if (layout.nrows == preferredNumberOfRowsOrColumns) {
					if (minimizeConstrainedMajor) {
						constrained_w = layout.preferred_major_size;
						goto ConstrainedMajor;
					}
					break;
				}
				else if (layout.nrows > preferredNumberOfRowsOrColumns) {
					layout.preferred_major_size = preferred_major_size;
					layout.preferred_minor_size = preferred_minor_size;
					break;
				}
				else if (layout.preferred_major_size >=
								preferred_major_size) {
					break;
				}
				else {
					preferred_major_size = layout.preferred_major_size;
					preferred_minor_size = layout.preferred_minor_size;
				}
			}
		}
		else {
			setupLayoutData(&layout);
		}
	}
	if (w != 0) {
		*w = layout.preferred_major_size;
	}
	if (h != 0) {
		*h = layout.preferred_minor_size;
	}
Done:
	wrappingEnabled = save_wrapping_enabled;
}

// --------------------------------------------------------------------------
//
int VGeoRowOrColumn::getNumberOfRowsOrColumns() const
{
	if (wrappingEnabled) {
		int w, h;
		LayoutData layout;
		((VGeoRowOrColumn *)this)->getPhysicalClientSize(&w, &h);
		((VGeoRowOrColumn *)this)->setupLayoutData(&layout, w, 0);
		return layout.nrows;
	}
	else {
		return 1;
	}
}

// --------------------------------------------------------------------------
// Layout the children of this VGeoRowOrColumn manager for the given the
// width and height.  If the given width and/or height is not greater than
// zero, then the current actual width and/or height will be obtained by
// calling VGeoManager::getPhysicalClientSize().  If this VGeoRowOrColumn
// has no children, then nothing will be done.
//
void VGeoRowOrColumn::frameChildren(int w, int h)
{

	if (!hasChildren()) {
		return;
	}

	beginFrameChildren(w, h);

	if (isGeoColumn) { int tmp = w; w = h; h = tmp; }

	// Set up the layout data and layout the children accordingly.

	LayoutData layout;
	setupLayoutData(&layout, w, h);
	layoutFromData(&layout, w);

	endFrameChildren();
}

// --------------------------------------------------------------------------
// Setup the given LayoutData object  to contain all the data we need
// to layout the children of this VGeoRowOrColumn object.  If the given
// major-size is zero then this setup is for a preferred-size request;
// if it is negative, then this setup is for a minimum-size request;
// otherwise it refers to the major-size of this VGeoRowOrColumn object --
// in this case (only) the given minor-size argument is interpreted as
// the minor size of this VGeoRowOrColumn object.
//
// ToDo!!! Note that we do not currently properly handle shrinking
//         in the minor dimension in conjunction with wrapping.
//
// ToDo!!! Note that we do not currently call getConstrainedSize()
//         when shrinking in the minor dimension.
//
// ToDo!!! Note that we do not currently properly handle margin
//         adjustment in conjuction with minor dimension shrinking.
//    
//
void VGeoRowOrColumn::setupLayoutData(void *layout_data, int this_major_size,
														 int this_minor_size)
{
	LayoutData& layout = *((LayoutData *)layout_data);

	ChildIterator children(this);

	if (wrappingEnabled) {
		//
		// Wrapping mode; obtain row data until there are no more rows.
		//
		layout.nrows = 0;
		layout.nchildren = 0;
		layout.preferred_major_size = 0;
		int last_margin = topMargin, total_margin = 0, total_row_height = 0;
		int max_equal_major_size, max_equal_minor_size;
		while (1) {
			RowOrColumnData& current_row = layout.row[layout.nrows];
			int done = setupRowOrColumnData(children, &current_row,
											this_major_size);
			layout.nchildren += current_row.nchildren;
			if (current_row.preferred_major_size >
				layout.preferred_major_size) {
				layout.preferred_major_size =
					current_row.preferred_major_size;
			}
			current_row.extra_minor_space = 0;
			current_row.insufficient_minor_space = 0;
			total_margin += max(last_margin, current_row.top_margin);
			last_margin = current_row.bottom_margin;
			total_row_height += current_row.height;
			layout.nrows++;
			if (done) {
				total_margin += max(last_margin, bottomMargin);
				break;
			}
			else {
				if (layout.nrows == 1) {
					max_equal_major_size = current_row.max_equal_major_size;
					max_equal_minor_size = current_row.max_equal_minor_size;
				}
				RowOrColumnData& next_row = layout.row[layout.nrows];
				next_row.max_equal_major_size = max_equal_major_size;
				next_row.max_equal_minor_size = max_equal_minor_size;
			}
		}
		layout.preferred_minor_size = total_row_height + total_margin;
		//
		// New, 11/19/95, dgm.
		// Preferred minor spacing.
		//
		if ((layout.nrows > 1) && (preferredMinorSpacing > 0)) {
			layout.preferred_minor_size +=
				(layout.nrows - 1) * preferredMinorSpacing;
		}
		//
		// Determine the the vertical spacing of the rows.  We want
		// to space the rows vertically as evenly as possible while
		// respecting margins.  Equalize/maximize the vertical space
		// used by each row.
		//
		if (this_minor_size > 0) {
			if (this_minor_size > layout.preferred_minor_size) {
				//
				// Here, we have more than enough room to fit the rows
				// without going off the bottom; simply dole out the
				// extra space evenly among the rows.
				//
				SpaceBroker spacebroker(this_minor_size -
										layout.preferred_minor_size,
										layout.nrows);
				for (int n = 0 ; n < layout.nrows ; n++) {
					RowOrColumnData& current_row = layout.row[n];
					int space = spacebroker.next();
					if (space > 0) {
						current_row.extra_minor_space = space;
						current_row.height += space;
					}
				}
			}
			else {
				if (layout.nrows == 1) {
					RowOrColumnData& row = layout.row[0];
					row.insufficient_minor_space =
						layout.preferred_minor_size - this_minor_size;
				}
				else {
					//
					// ToDo!!!
					//
				}
			}
		}
	}
	else {
		//
		// This is the non-wrapping case.
		//
		layout.nrows = 1;
		RowOrColumnData& current_row = layout.row[0];
		setupRowOrColumnData(children, &current_row, this_major_size);
		layout.nchildren = current_row.nchildren;
		layout.preferred_major_size = current_row.preferred_major_size;
		layout.preferred_minor_size =
			current_row.height +
			max(topMargin, current_row.top_margin) +
			max(bottomMargin, current_row.bottom_margin);
		if (this_minor_size > 0) {
			if (this_minor_size > layout.preferred_minor_size) {
				current_row.extra_minor_space =
					this_minor_size - layout.preferred_minor_size;
				current_row.height += current_row.extra_minor_space;
				current_row.insufficient_minor_space = 0;
			}
			else {
				current_row.extra_minor_space = 0;
				current_row.insufficient_minor_space =
					layout.preferred_minor_size - this_minor_size;
			}
		}
	}
}

// --------------------------------------------------------------------------
//
int VGeoRowOrColumn::setupRowOrColumnData(VGeoManager::ChildIterator& children,
										  void *row_or_column_data,
										  int major_size)
{
	RowOrColumnData&	row = *((RowOrColumnData *)row_or_column_data);
	int					total_cw, cw, ch, status;
	int					last_margin, margin, total_margin;
	int					end_margin, end_margins, preferred_equal_space;
	int					max_minor_size;
	int					nchildren_stretchable, nchildren_shrinkable;
	boolean				already_got_max_equal_sizes;
	boolean				is_equal_spacing, is_equal_flush_spacing;
	float				space_stretchability, total_child_stretchability;
	float				space_shrinkability, total_child_shrinkability;
	VGeoFrame		   *child, *first_child;
	MinorMargin			minor_margin;

	if (wrappingEnabled && (equalChildMajor || equalChildMinor)) {
		if (children.atVeryBeginning()) {
			//
			// If equal-major or equal-minor modes are on, then we must
			// make a preliminary traversal of the children to obtain
			// the maximum major and minimum child dimensions.
			//
			row.max_equal_major_size = 0;
			row.max_equal_minor_size = 0;
			while ((child = children.next()) != 0) {
				int cw, ch;
				setupChildSize(child, &cw, &ch, major_size < 0);
				if (C->majorSize > row.max_equal_major_size) {
					row.max_equal_major_size = C->majorSize;
				}
				if (C->minorSize > row.max_equal_minor_size) {
					row.max_equal_minor_size = C->minorSize;
				}
			}
			children.begin();
		}
		already_got_max_equal_sizes = TRUE;
	}
	else {
		row.max_equal_major_size = 0;
		row.max_equal_minor_size = 0;
		already_got_max_equal_sizes = FALSE;
	}

	// Initialize.

	row.nchildren			   = 0;
	row.max_margin			   = 0;
	max_minor_size			   = 0;
	total_cw				   = 0;
	total_margin			   = 0;
#if 0
	end_margin				   = spacingStyle == NoSpacingRight ?
								 rightMargin : leftMargin;
	last_margin				   = end_margin;
#else
	end_margin				   = rightMargin;
	last_margin				   = leftMargin;
#endif
	nchildren_stretchable	   = 0;
	nchildren_shrinkable	   = 0;
	total_child_stretchability = 0.0F;
	total_child_shrinkability  = 0.0F;
	is_equal_spacing		   = (spacingStyle == EqualSpacing) ||
								 (spacingStyle == EqualFlushSpacing);
	space_stretchability	   = is_equal_spacing ?
								 spaceStretchAbility : 0.0F;
	space_shrinkability		   = is_equal_spacing ?
								 spaceShrinkAbility : 0.0F;
	first_child				   = children.current();
	status					   = 1;

	// Loop through the children, gathering information.

	while ((child = children.next()) != 0) {
		//
		// Get the desired size of this child,
		// and keep track of the maximum size.
		//
		if (already_got_max_equal_sizes) {
			if (equalChildMajor) {
				if (equalChildMinor) {
					cw = C->majorSize = row.max_equal_major_size;
					ch = C->minorSize = row.max_equal_minor_size;
					C->ignoreEqualMajorSize = 1;
					C->ignoreEqualMinorSize = 1;
				}
				else {
					cw = C->majorSize = row.max_equal_major_size;
					ch = C->minorSize;
					C->ignoreEqualMajorSize = 1;
					C->ignoreEqualMinorSize = 0;
				}
			}
			else {
				cw = C->majorSize;
				ch = C->minorSize = row.max_equal_minor_size;
				C->ignoreEqualMajorSize = 0;
				C->ignoreEqualMinorSize = 1;
			}
		}
		else {
			setupChildSize(child, &cw, &ch, major_size < 0);
			C->ignoreEqualMajorSize = 0;
			C->ignoreEqualMinorSize = 0;
		}
		//
		// Obtain this margin and set up the last margin for next time.
		//
		if (spacingStyle == NoSpacingRight) {
			margin = max(last_margin, C->rightMargin);
		}
		else {
			margin = max(last_margin, C->leftMargin);
		}
		//
		// New, 11/14/95, dgm.
		//
		if (((spacingStyle == NoSpacing) ||
			 (spacingStyle == NoSpacingOpposite)) &&
			(preferredSpacing > 0) &&
			(preferredSpacing > margin) &&
			(child != first_child)) {
			margin = preferredSpacing;
		}
		//
		// If wrapping mode, then see if this row needs to wrap.
		// Note that wrapping is done before shrinking.
		//
		if (wrappingEnabled && (row.nchildren > 0)) {
			int extent, right_margin, equal_spacing;
			if (spacingStyle == NoSpacingRight) {
				right_margin = max(C->leftMargin, end_margin);
			}
			else {
				right_margin = max(C->rightMargin, end_margin);
			}
			if (is_equal_spacing) {
				equal_spacing = max(row.max_margin, margin);
				equal_spacing = max(equal_spacing, right_margin);
				if (spacingStyle == EqualFlushSpacing) {
					extent = total_cw + cw + row.nchildren * equal_spacing;
				}
				else {
					extent = total_cw + cw +
							 (row.nchildren + 2) * equal_spacing;
				}
			}
			else {
				extent = total_cw + cw +
						 total_margin + margin + right_margin;
			}
			if ((major_size > 0) && (extent > major_size)) {
				//
				// Wrap.
				//
				children.back();
				status = 0;
				break;
			}
		}
		//
		// No wrapping; up the child count and continue.
		//
		row.nchildren++;
		//
		// Keep track of the maximum width/height for
		// this row they haven't already been obtained.
		//
		if (!already_got_max_equal_sizes) {
			if (cw > row.max_equal_major_size) {
				row.max_equal_major_size = cw;
			}
			if (ch > row.max_equal_minor_size) {
				row.max_equal_minor_size = ch;
			}
		}
		//
		// Keep track of the total width.
		//
		total_cw += cw;
		//
		// Keep track of how many children are potentially stretchable.
		//
		if (C->stretchAbility > 0) {
			nchildren_stretchable++;
			total_child_stretchability += C->stretchAbility;
		}
		//
		// Keep track of how many children are potentially shrinkable.
		//
		if (C->shrinkAbility > 0) {
			nchildren_shrinkable++;
			total_child_shrinkability += C->shrinkAbility;
		}
		//
		// Keep track of the maximum and total horizontal margins.
		//
		total_margin += margin;
		if (spacingStyle == EqualFlushSpacing) {
			if (row.nchildren == 1) {
				end_margins = margin;
			}
			else if (margin > row.max_margin) {
				row.max_margin = margin;
			}
			last_margin = C->rightMargin;
		}
		else {
			if (margin > row.max_margin) {
				row.max_margin = margin;
			}
			if (spacingStyle == NoSpacingRight) {
				last_margin = C->leftMargin;
			}
			else {
				last_margin = C->rightMargin;
			}
		}
		//
		// Keep track of the vertical margins.
		//
		minor_margin.add(C, ch);
	}
	//
	// The row is done!
	//
	if (equalChildMajor) {
		total_cw = row.nchildren * row.max_equal_major_size;
	}
	max_minor_size = row.max_equal_minor_size;
	//
	// Update the maximum/total horizontal margins for the row.
	//
	margin = max(last_margin, end_margin);
	if (spacingStyle == EqualFlushSpacing) {
		if (row.nchildren == 1) {
			is_equal_flush_spacing = 0;
			if (end_margins > row.max_margin) {
				row.max_margin = end_margins;
			}
			total_margin += end_margins;
			if (margin > row.max_margin) {
				row.max_margin = margin;
			}
			end_margins = 0;
		}
		else {
			is_equal_flush_spacing = 1;
			end_margins += margin;
		}
	}
	else {
		is_equal_flush_spacing = 0;
		if (margin > row.max_margin) {
			row.max_margin = margin;
		}
		end_margins = 0;
	}
	total_margin += margin;
	//
	// Determine the preferred size for the major dimension.
	//
	if (!is_equal_spacing) {
		row.preferred_major_size = total_cw + total_margin;
	}
	else {
		preferred_equal_space = max(row.max_margin, preferredSpacing);
		if (is_equal_flush_spacing) {
			row.nspaces = row.nchildren - 1;
			row.preferred_major_size =
				total_cw + preferred_equal_space * row.nspaces + end_margins;
		}
		else {
			row.nspaces = row.nchildren + 1;
			row.preferred_major_size =
				total_cw + preferred_equal_space * row.nspaces;
		}
		if (major_size < 0) {
			//
			// Get the preferred minimum size.
			//
			row.preferred_major_size = total_cw + total_margin;
		}
	}
	//
	// Perform any major dimension shrinking/stretching
	// and set up for any spacing equal spacing.
	//
	if (major_size <= 0) {
		//
		// Here, we only want to get the preferred
		// or the the minimum size, so we're done.
		//
		goto Done;
	}
	else if (major_size < row.preferred_major_size) {
		//
		// Here, the manager is less than its preferred major
		// dimension size, see if we need to shrink any children.
		//
		int available_space_for_space = major_size - total_cw - total_margin;
		if ((space_shrinkability < 1.0F) &&
			(total_child_shrinkability > 0.0F)) {
			//
			// Here, the manager is less than its preferred major
			// dimension size and some of the children want to shrink.
			//
			int insufficient_space = row.preferred_major_size - major_size;
			int insufficient_space_for_children;
			if (available_space_for_space > 0) {
				int insufficient_space_for_space =
					int(insufficient_space * space_shrinkability);
				if (insufficient_space_for_space >
					available_space_for_space) {
					insufficient_space_for_space =
						available_space_for_space;
				}
				insufficient_space_for_children =
					insufficient_space - insufficient_space_for_space;
			}
			else {
				insufficient_space_for_children = insufficient_space;
			}
			SpaceBroker	insufficient_space_for_child
						(insufficient_space_for_children,
						 nchildren_shrinkable,
						 total_child_shrinkability);
			int overshrink = 0, first_shrink_loop = 1;
			//
			// Since a child may not take its full share of shrinkage
			// due to its minimum size constraint, we must keep looping
			// until there is no more shrinkage space available to the
			// children; i.e. after one child is shrunk to its minimum
			// size, the other shrinkable child take up the slack.
			//
			while (1) {
				total_cw = 0;
				children.begin(first_child);
				for (int n = 0 ; (n < row.nchildren) &&
								 (child = children.next()) != 0 ; n++) {
					cw = equalChildMajor ?
						 row.max_equal_major_size : C->majorSize;
					if ((C->shrinkAbility > 0) &&
						(!C->ignoreShrinkAbility || first_shrink_loop)) {
						int shrink = insufficient_space_for_child.
									 next(C->shrinkAbility);
						if (shrink > 0) {
							int min_major_size;
							if (isGeoColumn) {
								child->getMinimumSize(0, &min_major_size);
							}
							else {
								child->getMinimumSize(&min_major_size, 0);
							}
							if ((cw - shrink) < min_major_size) {
								//
								// We've shrunk this child to its minimum
								// size; allow the other children to take
								// up the slack.
								//
								overshrink += min_major_size - (cw - shrink);
								cw = min_major_size;
								C->ignoreShrinkAbility = 1;
								total_child_shrinkability -=
									C->shrinkAbility;
								nchildren_shrinkable--;
							}
							else {
								cw -= shrink;
								C->ignoreShrinkAbility = 0;
							}
							//
							// Ok, get this child's preferred minor
							// dimension size as well as an updated
							// major dimension size based on its
							// constrained major dimension size.
							//
							int new_minor_size, new_major_size;
							boolean got_constrained_size;
#if 0
							// ToDo!! This really isn't correct; we
							// should only be call getConstrainedSize()
							// if we would have otherwise have called
							// getPreferredSize(); we should go through
							// setupChildSize() to check.
#else
							if (isGeoRow) {
								got_constrained_size =
									child->getConstrainedSize
										(&new_major_size,
										 &new_minor_size, cw, 0);
							}
							else {
								got_constrained_size =
									child->getConstrainedSize
										(&new_minor_size,
										 &new_major_size, 0, cw);
							}
#endif
							if (got_constrained_size) {
								if (!equalChildMinor ||
									(new_minor_size >
									 row.max_equal_minor_size)) {
									minor_margin.add(C, new_minor_size);
									C->minorSize = new_minor_size;
									C->ignoreEqualMinorSize = 1;
								}
								if (new_minor_size > max_minor_size) {
									max_minor_size = new_minor_size;
								}
								if (new_major_size < cw) {
									cw = new_major_size;
								}
							}
						}
					}
					C->majorSize = cw;
					C->ignoreEqualMajorSize = 1;
					total_cw += C->majorSize;
				}
				if ((overshrink <= 0) ||
					(nchildren_shrinkable <= 0) ||
					(total_child_shrinkability <= 0)) {
					break;
				}
				else {
					insufficient_space_for_child.initialize
						(overshrink, nchildren_shrinkable,
									 total_child_shrinkability);
					overshrink = 0;
					first_shrink_loop = 0;
				}
			}
			available_space_for_space = major_size - total_cw - total_margin;
		}
#if 1
		//
		// Determine the preferred size for the major dimension (again).
		//
		if (!is_equal_spacing) {
			row.preferred_major_size = total_cw + total_margin;
		}
		else {
			row.preferred_major_size =
				total_cw + preferred_equal_space * row.nspaces;
			if (is_equal_flush_spacing) {
				row.preferred_major_size += end_margins;
			}
		}
#endif
		//
		// Set up any major dimension spacing.
		//
		if (!is_equal_spacing) {
			//
			// Here, since we are not doing any sort of
			// equal spacing, there is no space to distribute.
			//
			row.space = 0;
		}
		else if (available_space_for_space > 0) {
			//
			// Here, we want to do equal spacing; although we are less
			// then preferred size, we do at least have enough room to
			// place the children without going off the end (while, as
			// always, respecting margins).
			//
			if (major_size <
				total_cw + row.max_margin * row.nspaces + end_margins) {
				//
				// Here, we do not have enough room to do equal-spacing,
				// but we do have more than enough room to place the children
				// without going off of the end (while, as always, respecting
				// margins); we need to distribute the (insufficient) extra
				// space evenly to the spaces which are less than the ideal
				// size; to do this we must count the number of such spaces!
				// 
				row.nspaces = 0;
				last_margin = leftMargin;
				children.begin(first_child);
				for (int n = 0 ; (n < row.nchildren) &&
								 (child = children.next()) != 0 ; n++) {
					if ((spacingStyle == EqualFlushSpacing) && (n == 0)) {
						continue;
					}
					margin = max(last_margin, C->leftMargin);
					last_margin = C->rightMargin;
					if (margin < row.max_margin) {
						row.nspaces++;
					}
				}
				if ((spacingStyle != EqualFlushSpacing) ||
					 (row.nchildren == 1)) {
					margin = max(last_margin, rightMargin);
					if (margin < row.max_margin) {
						row.nspaces++;
					}
				}
				//
				// This space will be incrementally added to any space
				// whose margin is less than the maximum margin for the
				// row; this will be done in layoutFromData().
				//
				row.space = major_size - total_cw - total_margin;
				row.insufficient_equal_space = 1;
			}
			else {
				//
				// Here, we do have enough room to do equal spacing.
				// This space will be equally divided among the spaces in
				// layoutFromData(); there will be no need to look at
				// margins at that time since they are included in this space.
				//
				row.insufficient_equal_space = 0;
				row.space = major_size - total_cw;
				if (is_equal_flush_spacing) {
					row.space -= end_margins;
				}
			}
		}
		else {
			//
			// Here, there is not even enough room to place the children
			// without going off of the end (while, as always, respecting
			// margins); there is no space to distribute :-(.
			//
			row.space = 0;
		}
	}
	else if (major_size > row.preferred_major_size) {
		//
		// Here, the manager is greater than its preferred major
		// dimension size, see if we need to stretch any children.
		//
		if ((space_stretchability < 1.0F) &&
			(total_child_stretchability > 0)) {
			//
			// Here, the manager is greater than its preferred major
			// dimension size and some of the children want to stretch.
			//
			int extra_space = major_size - row.preferred_major_size;
			int extra_space_for_space =
				int(space_stretchability * extra_space);
			SpaceBroker extra_space_for_child
						(extra_space - extra_space_for_space,
						 nchildren_stretchable,
						 total_child_stretchability);
			total_cw = 0;
			children.begin(first_child);
			for (int n = 0 ; (n < row.nchildren) &&
							 (child = children.next()) != 0 ; n++) {
				if (C->stretchAbility > 0) {
					C->majorSize +=
						extra_space_for_child.next(C->stretchAbility);
					C->ignoreEqualMajorSize = 1;
				}
				total_cw += C->majorSize;
			}
		}
		if (is_equal_spacing) {
			//
			// Here, we do have enough room to do equal spacing.
			// This space will be equally divided among the spaces in
			// layoutFromData(); there will be no need to look at
			// margins at that time since they are included in this space.
			//
			row.insufficient_equal_space = 0;
			row.space = major_size - total_cw;
			if (is_equal_flush_spacing) {
				row.space -= end_margins;
			}
		}
		else {
			row.space = 0;
		}
	}
	else {
		//
		// Here, we are exactly our preferred size! :-)
		//
		if (is_equal_spacing) {
			//
			// Here, we do have enough room to do equal spacing.
			// This space will be equally divided among the spaces in
			// layoutFromData(); there will be no need to look at
			// margins at that time since they are included in this space.
			//
			row.insufficient_equal_space = 0;
			row.space = major_size - total_cw;
			if (is_equal_flush_spacing) {
				row.space -= end_margins;
			}
		}
		else {
			row.space = 0;
		}
	}

Done:
	//
	// Set the height, and figure out the top/bottom margins for the row.
	//
	row.height = max_minor_size;
	int top_margin, bottom_margin;
	minor_margin.get(row.height, top_margin, bottom_margin);
	row.top_margin = top_margin;
	row.bottom_margin = bottom_margin;
	//
	// Return 1 if there are no more sub-rows, otherwise
	// return 0 (the latter occurs only in wrapping mode).
	//
	return status;
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::layoutFromData(void *layout_data, int major_size)
{
	LayoutData&			layout = *((LayoutData *)layout_data);
	int					cx, cy, y, h, last_margin, margin;
	int					is_equal_flush_spacing;
	SpaceBroker			spacer;
	RowOrColumnData	   *row;
	VGeoFrame		   *child;
	ChildIterator		children(this);
	int					row_index;

	if (wrappingEnabled) {
		row_index = 0;
	}

	if (spacingStyle == NoSpacingRight) {
		children.reverse();
		children.begin();
	}

	row = &layout.row[0];
	y = max(topMargin, row->top_margin);

Begin:

	if (spacingStyle == NoSpacingRight) {
		is_equal_flush_spacing = 0;
		last_margin = rightMargin;
		cx = major_size;
	}
	else if ((spacingStyle == EqualFlushSpacing) && (row->nchildren > 1)) {
		is_equal_flush_spacing = 1;
		last_margin = leftMargin;
		cx = 0;
	}
	else {
		is_equal_flush_spacing = 0;
		last_margin = leftMargin;
		cx = 0;
	}
	if (row->space > 0) {
		spacer.initialize(row->space, row->nspaces);
	}

	for (int n = 0 ; (n < row->nchildren) &&
					 (child = children.next()) != 0 ; n++) {
		if (equalChildMajor && !C->ignoreEqualMajorSize) {
			C->majorSize = row->max_equal_major_size;
		}
		if (equalChildMinor && !C->ignoreEqualMinorSize) {
			C->minorSize = row->max_equal_minor_size;
		}
		//
		// Get this margin and set up for the next one.
		//
		if (spacingStyle == NoSpacingRight) {
			if (last_margin > C->rightMargin) {
				margin = last_margin;
			}
			else {
				margin = C->rightMargin;
			}
			last_margin = C->leftMargin;
		}
		else {
			if (last_margin > C->leftMargin) {
				margin = last_margin;
			}
			else {
				margin = C->leftMargin;
			}
			last_margin = C->rightMargin;
		}
		//
		// New, 11/15/95, dgm.
		//
		if (((spacingStyle == NoSpacing) ||
			 (spacingStyle == NoSpacingOpposite)) &&
			(preferredSpacing > 0) &&
			(preferredSpacing > margin) &&
			(n > 0)) {
			margin = preferredSpacing;
		}
		//
		// Set up the major dimension spacing.
		//
		if (row->space > 0) {
			if (is_equal_flush_spacing && (n == 0)) {
				margin = leftMargin;
			}
			else if (row->insufficient_equal_space) {
				if (margin < row->max_margin) {
					margin += spacer.next();
				}
			}
			else {
				margin = spacer.next();
			}
		}
		if (spacingStyle == NoSpacingRight) {
			cx -= (C->majorSize + margin);
		}
		else {
			cx += margin;
		}
		//
		// If the minor dimension is larger than preferred then
		// stretch the child by a specified percentage, if any.
		//
		if (row->extra_minor_space > 0) {
			if (C->minorStretchAbility > 0) {
				C->minorSize += int(C->minorStretchAbility *
									   row->extra_minor_space);
			}
			h = row->height;
		}
		//
		// If the minor dimension is smaller than preferred then
		// shrink the child by the specified percentage, if any.
		//
		else if ((row->insufficient_minor_space > 0) &&
				 (C->minorShrinkAbility > 0)) {
			int min_ch;
			if (isGeoColumn) {
				child->getMinimumSize(&min_ch, 0);
			}
			else {
				child->getMinimumSize(0, &min_ch);
			}
			if ((C->minorSize -=
				 int(C->minorShrinkAbility *
				 	 row->insufficient_minor_space)) < min_ch) {
				C->minorSize = min_ch;
			}
			h = row->height - row->insufficient_minor_space;
		}
		else {
			h = row->height;
		}
		//
		// Finalize the height and vertical position based on justification.
		//
		switch (C->justifyStyle) {
		case CenterJustify:
			cy = (h > C->minorSize) ? (h - C->minorSize) / 2 : 0;
			break;
		case TopJustify:
			cy = 0;
			break;
		case BottomJustify:
			cy = (h > C->minorSize) ? h - C->minorSize : 0;
			break;
		case FilledJustify:
			cy = 0;
			if (h > C->minorSize) { C->minorSize = h; }
			break;
		}
		//
		// Finally, place the child.
		//
		if (isGeoColumn) {
			child->setPhysicalGeometry
				(cy + y, cx, C->minorSize, C->majorSize);
		}
		else {
			child->setPhysicalGeometry
				(cx, cy + y, C->majorSize, C->minorSize);
		}
		if (spacingStyle != NoSpacingRight) {
			cx += C->majorSize;
		}
	}

	if (wrappingEnabled && (++row_index < layout.nrows)) {
		int last_row_height = row->height;
		int last_minor_margin = row->bottom_margin;
		row = &layout.row[row_index];
		y += last_row_height + max(last_minor_margin, row->top_margin);
		//
		// New, 11/19/95, dgm.
		// Preferred minor spacing.
		//
		y += preferredMinorSpacing;
		goto Begin;
	}
}

// --------------------------------------------------------------------------
// Return in (w, h) the preferred or minimum (depending on the given minimum
// flag) size of the given child VGeoFrame; the child is *assumed* not to be
// null and a child of this VGeoRowOrColumn!  If it is the minimum size which
// is desired, then simply getMinimumSize() is simply called for the child.
// If it is the preferred size which is desired, then first check the
// child-specific cache for the preferred size if it is valid, otherwise
// it updates the cache for next time around after it obtains the preferred
// size.  If any of (w, h) are null, then they will be ignored.
//
void VGeoRowOrColumn::setupChildSize(VGeoFrame *child,
									 int *major_size, int *minor_size,
									 boolean minimum)
{
	if (minimum) {
		float space_shrinkability = (spacingStyle == EqualSpacing) ||
									(spacingStyle == EqualFlushSpacing) ?
									spaceShrinkAbility : 0;
		//
		// N.B. This code ensures that we only get the actual minimum
		// size of a child if it is shrinkable in that dimension,
		// if its not shrinkable, then we get the preferred size.
		//
		if ((C->shrinkAbility > 0) && (space_shrinkability < 1.0F)) {
			if (C->minorShrinkAbility > 0) {
				if (isGeoColumn) {
					child->getSize(minor_size, major_size, TRUE);
				}
				else {
					child->getSize(major_size, minor_size, TRUE);
				}
			}
			else {
				if (isGeoColumn) {
					child->getSize(0, major_size, TRUE);
					child->getSize(minor_size, 0, FALSE);
				}
				else {
					child->getSize(major_size, 0, TRUE);
					child->getSize(0, minor_size, FALSE);
				}
			}
		}
		else if (C->minorShrinkAbility > 0) {
			if (isGeoColumn) {
				child->getSize(minor_size, 0, TRUE);
				child->getSize(0, major_size, FALSE);
			}
			else {
				child->getSize(0, minor_size, TRUE);
				child->getSize(major_size, 0, FALSE);
			}
		}
		else if (isGeoColumn) {
			child->getSize(minor_size, major_size, FALSE);
		}
		else {
			child->getSize(major_size, minor_size, FALSE);
		}
	}
	else {
		if (isGeoColumn) {
			child->getSize(minor_size, major_size);
		}
		else {
			child->getSize(major_size, minor_size);
		}
	}
	if (major_size != 0) {
		C->majorSize = *major_size;
	}
	if (minor_size != 0) {
		C->minorSize = *minor_size;
	}
}

// ==========================================================================

#if !defined(CV_NOARCHIVER)

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::putTo(VArchiver& a, boolean)
{
	// Write the base class data.

	VGeoManager::putTo(a, FALSE);

	// Write the revision tag.

	a << VGeoRowOrColumnArchiveTag;

	// Write the class specific data.

	a << float(spaceStretchAbility);
	a << float(spaceShrinkAbility);
	a << float(defaultChildStretchAbility);
	a << float(defaultChildShrinkAbility);
	a << float(defaultChildMinorStretchAbility);
	a << float(defaultChildMinorShrinkAbility);
	a << short(leftMargin);
	a << short(rightMargin);
	a << short(topMargin);
	a << short(bottomMargin);
	a << short(preferredSpacing);
	a << short(preferredNumberOfRowsOrColumns);
	a << short(defaultChildLeftMargin);
	a << short(defaultChildRightMargin);
	a << short(defaultChildTopMargin);
	a << short(defaultChildBottomMargin);
	a << char(defaultChildJustifyStyle);
	a << char(spacingStyle);

	char c;

	c = equalChildMajor;			a << c;
	c = equalChildMinor;			a << c;
	c = wrappingEnabled;			a << c;
	c = minimizeConstrainedMajor;	a << c;

    if (VGeoRowOrColumnTagRev >= 1) {
		a << char(preferredMinorSpacing);
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::getFrom(VArchiver& a)
{
	// Read the base class data.

	VGeoManager::getFrom(a);

	// Read and verify the revision tag.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoRowOrColumnArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoRowOrColumnArchiveTag));
		return;
	}

	// Read the class specific data.

	a >> spaceStretchAbility;
	a >> spaceShrinkAbility;
	a >> defaultChildStretchAbility;
	a >> defaultChildShrinkAbility;
	a >> defaultChildMinorStretchAbility;
	a >> defaultChildMinorShrinkAbility;
	a >> leftMargin;
	a >> rightMargin;
	a >> topMargin;
	a >> bottomMargin;
	a >> preferredSpacing;
	a >> preferredNumberOfRowsOrColumns;
	a >> defaultChildLeftMargin;
	a >> defaultChildRightMargin;
	a >> defaultChildTopMargin;
	a >> defaultChildBottomMargin;
	a >> defaultChildJustifyStyle;
	a >> spacingStyle;

	char c;

	a >> c; equalChildMajor = c;
	a >> c; equalChildMinor = c;
	a >> c; wrappingEnabled = c;
	a >> c; minimizeConstrainedMajor = c;

    if (getArchiveTagRev(tag) >= 1) {
		a >> preferredMinorSpacing;
	}
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::RowOrColumnChildData::putTo(VArchiver& a)
{
	a << float(stretchAbility);
	a << float(shrinkAbility);
	a << float(minorStretchAbility);
	a << float(minorShrinkAbility);
	a << short(leftMargin);
	a << short(rightMargin);
	a << short(topMargin);
	a << short(bottomMargin);
	a << char(justifyStyle);
}

// --------------------------------------------------------------------------
//
void VGeoRowOrColumn::RowOrColumnChildData::getFrom(VArchiver& a)
{
	a >> stretchAbility;
	a >> shrinkAbility;
	a >> minorStretchAbility;
	a >> minorShrinkAbility;
	a >> leftMargin;
	a >> rightMargin;
	a >> topMargin;
	a >> bottomMargin;
	a >> justifyStyle;
}

#endif
