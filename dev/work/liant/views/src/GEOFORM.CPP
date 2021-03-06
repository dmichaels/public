// $Header:   Q:/views/common/vcs/GEOFORM.CPV   1.13   Mar 24 1997 16:09:02   scottt  $

//	geoform.cpp
//
//	VGeoForm implementation [Common]
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
//	11/20/95 dgm	Call begin/endFrameChildren() from frameChildren().
//	11/28/95 dgm	Fix in getConstrainedSize() to take internal borders
//					into proper account.
//	11/30/95 dgm	Call update() in attachChild().
//	01/29/96 dgm	Size-group changes.
//	02/05/96 dgm	Fix for [SP].
//	02/06/96 dgm	Another fix for [SP].
//	02/06/96 dgm	Yet another fix for [SP]!
//	02/16/96 dgm	Support to query VWindow associated with VGeoManager.
//	02/23/96 dgm	SizeGroup changes.
//	02/26/96 dgm	Fix in detachChild() -- iterate through *all* children.
//	01/21/97 dgm	Added oldAttachData.
// --------------------------------------------------------------------------

#include "geoform.h"
#include "window.h"
#include "ordcllct.h"
#include "iterator.h"
#include "cvassert.h"

#ifndef CV_NO_OLD_GEO
#include "attach.h"
#endif

// --------------------------------------------------------------------------
//
#if !defined(CV_NOARCHIVER)
#include "archiver.h"
#endif

// --------------------------------------------------------------------------
//
static int test_mode = 0;
static int frame_error = 0;

// --------------------------------------------------------------------------
//
defineClass(VGeoForm, VGeoManager)
defineArchiveRevision(VGeoForm, 0)

// --------------------------------------------------------------------------
//
#undef  getChildData
#define getChildData(child)		((FormChildData *)((child)->childData))

#undef  C
#define C						((FormChildData *)((child)->childData))

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(const VFrame& frame, VGeoManager *parent)
	: VGeoManager(frame, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(int x, int y, int w, int h, VGeoManager *parent)
	: VGeoManager(x, y, w, h, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(int w, int h, VGeoManager *parent)
	: VGeoManager(w, h, parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(VGeoManager *parent)
	: VGeoManager(parent)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(const VFrame& frame, VWindow *parent_window)
	: VGeoManager(frame, parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm(VWindow *parent_window)
	: VGeoManager(parent_window)
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::VGeoForm()
	: VGeoManager()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VGeoForm::~VGeoForm()
{
}

// --------------------------------------------------------------------------
//
VGeoFrame::ChildData *VGeoForm::newChildData() const
{
	return new FormChildData(this);
}

// --------------------------------------------------------------------------
//
void VGeoForm::childUpdated(VGeoFrame *child)
{
	if ((child != 0) && (C != 0)) {
#if 0
		C->leftTop[0].setInvalid();
		C->leftTop[1].setInvalid();
		C->widthHeight[0].setInvalid();
		C->widthHeight[1].setInvalid();
		C->rightBottomDep[0].setInvalid();
		C->rightBottomDep[1].setInvalid();
#else
		//
		// We need to do this since sibling of this child could
		// depend on it, and other siblings could depend on those
		// dependent siblings, etc.  We could do the minimal
		// work here, but it's probably not worth it.
		//
		ChildIterator children(this); VGeoFrame *c;
		while ((c = children.next()) != 0) {
			getChildData(c)->leftTop[0].setInvalid();
			getChildData(c)->leftTop[1].setInvalid();
			getChildData(c)->widthHeight[0].setInvalid();
			getChildData(c)->widthHeight[1].setInvalid();
			getChildData(c)->rightBottomDep[0].setInvalid();
			getChildData(c)->rightBottomDep[1].setInvalid();
		}
#endif
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::childRemoved(VGeoFrame *child)
{
	if (child != 0) {
		detachChild(child);
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::childReplaced(VGeoFrame *old_child, VGeoFrame *new_child)
{
	ChildIterator children(this);
	VGeoFrame *child;

	while ((child = children.next()) != 0) {
		if ((child == old_child) || (C->theAttachments == 0)) {
			continue;
		}
		for (int i = 0 ; i < 4 ; i++) {
			if ((C->theAttachments->theSibling[i] == old_child) &&
				((C->theAttachments->theType[i] == Sibling) ||
				 (C->theAttachments->theType[i] == SiblingSame))) {
				C->theAttachments->theSibling[i] = new_child;
			}
		}
	}

	new_child->update();
}

// --------------------------------------------------------------------------
//
boolean VGeoForm::frameChildrenOk(int w, int h)
{
	test_mode = 1;
	frameChildren();
	test_mode = 0;
	if (frame_error) {
		frame_error = 0;
		return FALSE;
	}
	else {
		return TRUE;
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::frameChildren(int w, int h)
{
	if (!hasChildren()) {
		return;
	}

	beginFrameChildren(w, h);

	int cx, cy, cw, ch;

	resizingDimension[0] = w;
	resizingDimension[1] = h;

	ChildIterator children(this);
	VGeoFrame *child;

	while ((child = children.next()) != 0) {
		if (!setupChild(child)) { continue; }
		cx = C->leftTop[0].getChildPosition(w);
		cy = C->leftTop[1].getChildPosition(h);
		cw = C->widthHeight[0].getChildDimension(w);
		ch = C->widthHeight[1].getChildDimension(h);
		child->setPhysicalGeometry(cx, cy, cw, ch);
	}

	resizingDimension[0] = 0;
	resizingDimension[1] = 0;

	endFrameChildren();
}

// --------------------------------------------------------------------------
//
void VGeoForm::getPreferredSize(int *w, int *h)
{
	if (!hasChildren()) {
		VGeoManager::getPreferredSize(w, h);
		return;
	}

	setupSizeGroups();
	getPreferredSizeWithChildren(w, h);
	checkWindowPreferredSize(w, h);
	addNonClientAreaSize(w, h);
}

// --------------------------------------------------------------------------
//
boolean VGeoForm::getConstrainedSize(int *w, int *h,
									 int constrained_w, int constrained_h)
{
	if (!hasChildren()) {
		return VGeoManager::
			   getConstrainedSize(w, h, constrained_w, constrained_h);
	}

	setupSizeGroups();
	subtractInternalBorderSize(constrained_w > 0 ? &constrained_w : 0,
							   constrained_h > 0 ? &constrained_h : 0);
	getPreferredSizeWithChildren(w, h, FALSE, constrained_w, constrained_h);
	checkWindowConstrainedSize(w, h, constrained_w, constrained_h);
	addNonClientAreaSize(w, h);
	return TRUE;
}

// --------------------------------------------------------------------------
//
void VGeoForm::getMinimumSize(int *w, int *h)
{
	if (!hasChildren()) {
		VGeoManager::getMinimumSize(w, h);
		return;
	}

	setupSizeGroups();
	getPreferredSizeWithChildren(w, h, TRUE);
	checkWindowMinimumSize(w, h);
	addNonClientAreaSize(w, h);
}

// --------------------------------------------------------------------------
//
boolean VGeoForm::attachChild(VGeoFrame *child,
							  Side side,
							  Type type,
							  VGeoFrame *sibling,
							  int offset)
{
	if ((child == 0) || (C == 0) ||
		(type < 0) || (type > 4) ||
		(side < 0) || (side > 3) ||
		(((type == Sibling) ||
		  (type == SiblingSame)) && (sibling == 0))) {
		return FALSE;
	}
	if (C->theAttachments == 0) {
		C->theAttachments = new Attachment;
	}
	C->theAttachments->theType[side] = type;
	C->theAttachments->theOffset[side] = offset;
	C->theAttachments->theSibling[side] = sibling;
	child->update(); update();
	return TRUE;
}

// --------------------------------------------------------------------------
// Remove the specified attachment from the given VGeoFrame child of this
// VGeoForm.
//
void VGeoForm::unattachChild(VGeoFrame *child, Side side)
{
	if ((child == 0) || (C == 0) ||
		(C->theAttachments == 0) || (side < 0) || (side > 3)) {
		return;
	}
	C->theAttachments->theType[side] = UnAttached;
	C->theAttachments->theOffset[side] = 0;
	C->theAttachments->theSibling[side] = 0;
}

// --------------------------------------------------------------------------
// Remove all of the attachment from the given VGeoFrame child of this
// VGeoForm.
//
void VGeoForm::unattachChild(VGeoFrame *child)
{
	if ((child == 0) || (C == 0) || (C->theAttachments == 0)) {
		return;
	}
	delete C->theAttachments;
	C->theAttachments = 0;
}

// --------------------------------------------------------------------------
//
void VGeoForm::unattachChild(VWindow *child_window, Side side)
{
	if (child_window != 0) {
		unattachChild(child_window->getGeoFrame(), side);
	}
}

// --------------------------------------------------------------------------
// Remove all of the attachment from the given VGeoFrame child of this
// VGeoForm.
//
void VGeoForm::unattachChild(VWindow *child_window)
{
	if (child_window != 0) {
		unattachChild(child_window->getGeoFrame());
	}
}

// --------------------------------------------------------------------------
// Remove all attachment references to the given VGeoFrame child of this
// VGeoForm.
//
void VGeoForm::detachChild(VGeoFrame *geoframe)
{
	if ((geoframe == 0) || !hasChildren()) {
		return;
	}

	ChildIterator children(this, TRUE);
	VGeoFrame *child;

	while ((child = children.next()) != 0) {
		if (C->theAttachments != 0) {
			for (int i = 0 ; i < 4 ; i++) {
				if ((C->theAttachments->theSibling[i] == geoframe) &&
					((C->theAttachments->theType[i] == Sibling) ||
					 (C->theAttachments->theType[i] == SiblingSame))) {
					C->theAttachments->theType[i] = UnAttached;
					C->theAttachments->theOffset[i] = 0;
					C->theAttachments->theSibling[i] = 0;
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
VGeoForm::Attachment *VGeoForm::getAttachments(VGeoFrame *child) const
{
	if ((child != 0) && (child->getParent() == (VGeoFrame *)this)) {
		return C->theAttachments;
	}
	else {
		return 0;
	}
}

// --------------------------------------------------------------------------
//
VGeoForm::FormChildData::~FormChildData()
{
	// detachChild(0);
	if (theAttachments != 0) {
		delete theAttachments;
	}
}

// --------------------------------------------------------------------------
// Return in (*w, *h) the preferred (or minimum) size which this VGeoForm
// would like to be assuming it has one or more child windows.  To accomplish
// this we simply look at each child whose size is a a ratio of its parent
// and see what size the parent must be in order to accomodate the child's
// preferred (or minimum) size; the final preferred (or minimum) size is
// the maximum of these values.
//
void VGeoForm::getPreferredSizeWithChildren(int *w, int *h,
											boolean minimum,
											int constrained_w,
											int constrained_h)
{
	int cx, cy, cw, ch, pw, ph, max_w = 0, max_h = 0;
	int max_right_extent = 0, max_bottom_extent = 0;

	ChildIterator children(this);
	VGeoFrame *child;

	while ((child = children.next()) != 0) {
		if (!setupChild(child, minimum)) { continue; }
		if (C->widthHeight[0].isRatio()) {
			//
			// Here, the width of this child is dependent
			// on this parent; see if the height also is.
			//
			if (C->widthHeight[1].isRatio()) {
				//
				// Here, both the width and height of this
				// child are dependent on this parent.
				//
				if (minimum) {
					if (w != 0) {
						if (h != 0) {
							child->getSize(&cw, &ch, TRUE);
						}
						else {
							child->getSize(&cw, 0, TRUE);
						}
					}
					else if (h != 0) {
						child->getSize(0, &ch, TRUE);
					}
				}
				else {
					if (constrained_w > 0) {
						if (constrained_h == 0) {
							int constrained_cw =
								 C->widthHeight[0].
								    getChildDimension(constrained_w);
							if (constrained_cw == 0) {
								constrained_cw = 1;
							}
							child->getSize(&cw, &ch, FALSE,
										   constrained_cw, 0);
						}
						else {
							// child->getSize(&cw, &ch);
							int constrained_cw =
								 C->widthHeight[0].
								    getChildDimension(constrained_w);
							int constrained_ch =
								 C->widthHeight[1].
								    getChildDimension(constrained_h);
							if (constrained_cw == 0) {
								constrained_cw = 1;
							}
							if (constrained_ch == 0) {
								constrained_ch = 1;
							}
							child->getSize(&cw, &ch, FALSE,
										   constrained_cw, constrained_ch);
						}
					}
					else if (constrained_h > 0) {
						int constrained_ch =
							C->widthHeight[1].
							   getChildDimension(constrained_h);
						if (constrained_ch == 0) {
							constrained_ch = 1;
						}
						child->getSize(&cw, &ch, FALSE, 0, constrained_ch);
					}
					else {
						child->getSize(&cw, &ch);
					}
				}
				pw = C->widthHeight[0].getParentDimension(cw);
				ph = C->widthHeight[1].getParentDimension(ch);
				if (C->widthHeight[0].getRatio() < 0.0) {
					//
					// Here, the width of this child is inversely
					// proportional to the width of the parent;
					// we will ignore this is a weird case.
					//
				}
				else if (pw > max_w) {
					max_w = pw;
				}
				if (C->widthHeight[1].getRatio() < 0.0) {
					//
					// Here, the height of this child is inversely
					// proportional to the height of the parent;
					// we will ignore this is a weird case.
					//
				}
				else if (ph > max_h) {
					max_h = ph;
				}
			}
			else {
				//
				// Here, the height of this child
				// is not dependent on this parent.
				//
				child->getSize(&cw, 0, minimum);
				pw = C->widthHeight[0].getParentDimension(cw);
				if (C->widthHeight[0].getRatio() < 0.0) {
					//
					// Here, the width of this child is inversely
					// proportional to the width of the parent;
					// we will ignore this is a weird case.
					//
				}
				else if (pw > max_w) {
					max_w = pw;
				}
			}
		}
		else if (C->widthHeight[1].isRatio()) {
			//
			// Here, the height of this child is dependent
			// on this parent, but the width is not.
			//
			child->getSize(0, &ch, minimum);
			ph = C->widthHeight[1].getParentDimension(ch);
			if (C->widthHeight[1].getRatio() < 0.0) {
				//
				// Here, the height of this child is inversely
				// proportional to the height of the parent;
				// we will ignore this is a weird case.
				//
			}
			else if (ph > max_h) {
				max_h = ph;
			}
		}
	}

	// NEW, 10/25/95

	for (children.begin() ; (child = children.next()) != 0 ; ) {
		if (!setupChild(child, minimum)) { continue; }
		cx = C->leftTop[0].getChildDimension(max_w);
		cy = C->leftTop[1].getChildDimension(max_h);
		cw = C->widthHeight[0].getChildDimension(max_w);
		ch = C->widthHeight[1].getChildDimension(max_h);
		if (cx + cw > max_right_extent) {
			max_right_extent = cx + cw;
		}
		if (cy + ch > max_bottom_extent) {
			max_bottom_extent = cy + ch;
		}
	}
	if (max_right_extent > max_w) {
		max_w = max_right_extent;
	}
	if (max_bottom_extent > max_h) {
		max_h = max_bottom_extent;
	}

	if (w != 0) {
		*w = max_w;
	}
	if (h != 0) {
		*h = max_h;
	}
}

// --------------------------------------------------------------------------
//
int VGeoForm::setupChild(VGeoFrame *child, boolean minimum, int d)
{
	static const Side	LeftTopSide[2]	   = { Left,  Top    };
	static const Side	RightBottomSide[2] = { Right, Bottom };

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	GeoComponent	x, y, w, h;
	int				left_top_side, right_bottom_side;
	VGeoFrame	   *left_top_sibling, *right_bottom_sibling;
	boolean			do_other_dim;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	if      (d == 0) { do_other_dim = FALSE; }
	else if (d == 1) { do_other_dim = FALSE; }
	else             { do_other_dim = TRUE; d = 0; }

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Begin:
	if ((resizingDimension[d] == 0) &&
		C->leftTop[d].isComplete(minimum) &&
		C->widthHeight[d].isComplete(minimum) &&
		C->rightBottomDep[d].isComplete(minimum)) {
		goto Done;
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	if (d == 0) {
		if (C->calculatingHorizontal) {
			C->calculatingHorizontal = 0;
			if (test_mode) {
				frame_error = 1;
			}
			else {
				print("VGeoForm Error: Recursive (H)!");
			}
			return 0;
		}
		else {
			C->calculatingHorizontal = 1;
		}
	}
	else {
		if (C->calculatingVertical) {
			C->calculatingVertical = 0;
			if (test_mode) {
				frame_error = 1;
			}
			else {
				print("VGeoForm Error: Recursive (V)!");
			}
			return 0;
		}
		else {
			C->calculatingVertical = 1;
		}
	}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

	if (C->theAttachments != 0) {
		left_top_side = C->theAttachments->getType(LeftTopSide[d]);
		right_bottom_side = C->theAttachments->getType(RightBottomSide[d]);
	}
	else {
		left_top_side = UnAttached;
		right_bottom_side = UnAttached;
	}

	switch (left_top_side) {

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Left-or-top     is attached to nothing.							[N?]
	// Right-or-bottom is ...
	//
	case UnAttached:
	{
		switch (right_bottom_side) {

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to nothing.						[NN]
		// Right-or-bottom is attached to nothing.
		//
		case UnAttached:
		{
			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->rightBottomDep[d].setValid();

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to nothing.						[NP]
		// Right-or-bottom is attached to parent.
		//
		case Parent:
		{
			if (!setupChildGeometry
				(child, d, FALSE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].set
				(1.0F - C->widthHeight[d].getRatio(),
				 -C->theAttachments->getOffset(RightBottomSide[d]) -
				  C->widthHeight[d].getOffset());
			C->leftTop[d].setValid();

			C->leftTop[d].setPreferredSizeDependencies
				(C->widthHeight[d].getPreferredSizeDependencies());

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to nothing.						[NS]
		// Right-or-bottom is attached to sibling.
		//
		case Sibling:
		case SiblingSame:
		{
			if (!setupChildGeometry
				(child, d, FALSE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			right_bottom_sibling =
				C->theAttachments->getSibling(RightBottomSide[d]);

			if (!setupChild(right_bottom_sibling, minimum, d)) {
				goto Error;
			}

			if (right_bottom_side == Sibling) {
				C->leftTop[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio() -
					 C->widthHeight[d].getRatio(),
					 getChildData(right_bottom_sibling)->
						leftTop[d].getOffset() -
					 C->theAttachments->getOffset(RightBottomSide[d]) -
					 C->widthHeight[d].getOffset());
				C->leftTop[d].setPreferredSizeDependencies
					(C->widthHeight[d].getPreferredSizeDependencies() |
					 getChildData(right_bottom_sibling)->
					 	leftTop[d].getPreferredSizeDependencies());
				C->rightBottomDep[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->
					 leftTop[d].getPreferredSizeDependencies());
			}
			else {
				C->leftTop[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio() +
					 getChildData(right_bottom_sibling)->
						widthHeight[d].getRatio() -
					 C->widthHeight[d].getRatio(),
					 getChildData(right_bottom_sibling)->
						leftTop[d].getOffset() +
					 getChildData(right_bottom_sibling)->
						widthHeight[d].getOffset() -
					 C->theAttachments->getOffset(RightBottomSide[d]) -
					 C->widthHeight[d].getOffset());
				C->leftTop[d].setPreferredSizeDependencies
					(C->widthHeight[d].getPreferredSizeDependencies() |
					 getChildData(right_bottom_sibling)->
					 	rightBottomDep[d].getPreferredSizeDependencies());
				C->rightBottomDep[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->
					 rightBottomDep[d].getPreferredSizeDependencies());
			}

			C->leftTop[d].setValid();

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to nothing.						[NF]
		// Right-or-bottom is attached to frame.
		//
		case Frame:
		{
			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].set
				(C->leftTop[d].getOffset() -
				 C->theAttachments->getOffset(RightBottomSide[d]));

			goto Done;
		}
	}}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Left-or-top     is attached to parent.							[P?]
	// Right-or-bottom is ...
	//
	case Parent:
	{
		switch (right_bottom_side) {

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to parent.						[PN]
		// Right-or-bottom is attached to nothing.
		//
		case UnAttached:
		{
			C->leftTop[d].set(C->theAttachments->getOffset(LeftTopSide[d]));
			C->leftTop[d].setValid();

			if (!setupChildGeometry
				(child, d, FALSE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->rightBottomDep[d].setPreferredSizeDependencies
				(C->widthHeight[d].getPreferredSizeDependencies());

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to parent.						[PP]
		// Right-or-bottom is attached to parent.
		//
		case Parent:
		{
			C->leftTop[d].set(C->theAttachments->getOffset(LeftTopSide[d]));
			C->leftTop[d].setValid();

			C->widthHeight[d].set
				(1.0F, -C->theAttachments->getOffset(RightBottomSide[d]) -
						C->theAttachments->getOffset(LeftTopSide[d]));
			C->widthHeight[d].setValid();

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to parent.						[PS]
		// Right-or-bottom is attached to sibling.
		//
		case Sibling:
		case SiblingSame:
		{
			C->leftTop[d].set(C->theAttachments->getOffset(LeftTopSide[d]));
			C->leftTop[d].setValid();

			right_bottom_sibling =
				C->theAttachments->getSibling(RightBottomSide[d]);

			if (!setupChild(right_bottom_sibling, minimum, d)) {
				goto Error;
			}

			if (right_bottom_side == Sibling) {
				C->widthHeight[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio(),
					 getChildData(right_bottom_sibling)->
						leftTop[d].getOffset() -
						C->theAttachments->getOffset(RightBottomSide[d]) -
						C->theAttachments->getOffset(LeftTopSide[d]));
				C->widthHeight[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->leftTop[d].
						getPreferredSizeDependencies());
			}
			else {
				C->widthHeight[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio() +
					 getChildData(right_bottom_sibling)->
						widthHeight[d].getRatio(),
					 getChildData(right_bottom_sibling)->
						leftTop[d].getOffset() +
					 getChildData(right_bottom_sibling)->
						widthHeight[d].getOffset() -
						C->theAttachments->getOffset(RightBottomSide[d]));
				C->widthHeight[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->rightBottomDep[d].
						getPreferredSizeDependencies());
			}

			C->widthHeight[d].setValid();

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to parent.						[PF]
		// Right-or-bottom is attached to frame.
		//
		case Frame:
		{
			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->widthHeight[d].set
				(C->leftTop[d].getRatio() +
				 C->widthHeight[d].getRatio(),
				 C->leftTop[d].getOffset() +
				 C->widthHeight[d].getOffset() -
				 C->theAttachments->getOffset(RightBottomSide[d]) -
				 C->theAttachments->getOffset(LeftTopSide[d]));

			C->leftTop[d].set
				(C->theAttachments->getOffset(LeftTopSide[d]));

			goto Done;
		}
	}}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Left-or-top     is attached to sibling.							[S?]
	// Right-or-bottom is ...
	//
	case Sibling:
	case SiblingSame:
	{
		switch (right_bottom_side) {

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to sibling.						[SN]
		// Right-or-bottom is attached to nothing.
		//
		case UnAttached:
		{
			left_top_sibling =
				C->theAttachments->getSibling(LeftTopSide[d]);

			if (!setupChild(left_top_sibling, minimum, d)) {
				goto Error;
			}

			if (left_top_side == Sibling) {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->
						leftTop[d].getRatio() +
					 getChildData(left_top_sibling)->
					 	widthHeight[d].getRatio(),
					 getChildData(left_top_sibling)->
					 	leftTop[d].getOffset() +
					 getChildData(left_top_sibling)->
					 	widthHeight[d].getOffset() +
					 C->theAttachments->getOffset(LeftTopSide[d]));
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->rightBottomDep[d].
						getPreferredSizeDependencies());
			}
			else {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->
						leftTop[d].getRatio(),
					 getChildData(left_top_sibling)->
					 	leftTop[d].getOffset() +
						C->theAttachments->getOffset(LeftTopSide[d]));
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->leftTop[d].
						getPreferredSizeDependencies());
			}

			C->leftTop[d].setValid();

			if (!setupChildGeometry
				(child, d, FALSE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->rightBottomDep[d].setPreferredSizeDependencies
				(C->leftTop[d].getPreferredSizeDependencies() |
				 C->widthHeight[d].getPreferredSizeDependencies());

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to sibling.						[SP]
		// Right-or-bottom is attached to parent.
		//
		case Parent:
		{
			left_top_sibling =
				C->theAttachments->getSibling(LeftTopSide[d]);

			if (!setupChild(left_top_sibling, minimum, d)) {
				goto Error;
			}

			if (left_top_side == Sibling) {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->
						leftTop[d].getRatio() +
					 getChildData(left_top_sibling)->
						widthHeight[d].getRatio(),
					 getChildData(left_top_sibling)->
						leftTop[d].getOffset() +
					 getChildData(left_top_sibling)->
						widthHeight[d].getOffset() +
					 C->theAttachments->getOffset(LeftTopSide[d]));
				C->widthHeight[d].set
					(1.0F - C->leftTop[d].getRatio(),
					 -C->theAttachments->getOffset(RightBottomSide[d]) -
					  C->leftTop[d].getOffset());
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->rightBottomDep[d].
						getPreferredSizeDependencies());
			}
			else {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->
						leftTop[d].getRatio(),
					 getChildData(left_top_sibling)->
						leftTop[d].getOffset() +
					 C->theAttachments->getOffset(LeftTopSide[d]));
				C->widthHeight[d].set
					(1.0F - C->leftTop[d].getRatio(),
					 -C->theAttachments->getOffset(RightBottomSide[d]) -
					 C->leftTop[d].getOffset());
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->leftTop[d].
						getPreferredSizeDependencies());
			}

			C->leftTop[d].setValid();
			C->widthHeight[d].setValid();

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to sibling.						[SS]
		// Right-or-bottom is attached to sibling.
		//
		case Sibling:
		case SiblingSame:
		{
			left_top_sibling =
				C->theAttachments->getSibling(LeftTopSide[d]);
			right_bottom_sibling =
				C->theAttachments->getSibling(RightBottomSide[d]);

			if (!setupChild(left_top_sibling, minimum, d) ||
				!setupChild(right_bottom_sibling, minimum, d)) {
				goto Error;
			}

			if (left_top_side == Sibling) {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->
						leftTop[d].getRatio() +
					 getChildData(left_top_sibling)->
					 	widthHeight[d].getRatio(),
					 getChildData(left_top_sibling)->
					 	leftTop[d].getOffset() +
					 getChildData(left_top_sibling)->
					 	widthHeight[d].getOffset() +
					 C->theAttachments->getOffset(LeftTopSide[d]));
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->rightBottomDep[d].
						getPreferredSizeDependencies());
			}
			else {
				C->leftTop[d].set
					(getChildData(left_top_sibling)->leftTop[d].getRatio(),
					 getChildData(left_top_sibling)->leftTop[d].getOffset() +
					 C->theAttachments->getOffset(LeftTopSide[d]));
				C->leftTop[d].setPreferredSizeDependencies
					(getChildData(left_top_sibling)->leftTop[d].
						getPreferredSizeDependencies());
			}
			C->leftTop[d].setValid();

			if (right_bottom_side == Sibling) {
				C->widthHeight[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio() -
					 C->leftTop[d].getRatio(),
					 getChildData(right_bottom_sibling)->
					 	leftTop[d].getOffset() -
					 C->leftTop[d].getOffset() -
					 C->theAttachments->getOffset(RightBottomSide[d]));
				C->rightBottomDep[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->leftTop[d].
						getPreferredSizeDependencies());
			}
			else {
				C->widthHeight[d].set
					(getChildData(right_bottom_sibling)->
						leftTop[d].getRatio() +
					 getChildData(right_bottom_sibling)->
						widthHeight[d].getRatio() -
					 C->leftTop[d].getRatio(),
					 getChildData(right_bottom_sibling)->
					 	leftTop[d].getOffset() +
					 getChildData(right_bottom_sibling)->
					 	widthHeight[d].getOffset() -
					 C->leftTop[d].getOffset() -
					 C->theAttachments->getOffset(RightBottomSide[d]));
				C->rightBottomDep[d].setPreferredSizeDependencies
					(getChildData(right_bottom_sibling)->rightBottomDep[d].
						getPreferredSizeDependencies());
			}
			C->widthHeight[d].setValid();

			C->widthHeight[d].setPreferredSizeDependencies
				(C->leftTop[d].getPreferredSizeDependencies() |
				 C->rightBottomDep[d].getPreferredSizeDependencies());

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to sibling.						[SF]
		// Right-or-bottom is attached to frame.
		//
		case Frame:
		{
			left_top_sibling =
				C->theAttachments->getSibling(LeftTopSide[d]);

			if (!setupChild(left_top_sibling, minimum, d)) {
				goto Error;
			}

			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->widthHeight[d].set
				(C->leftTop[d].getRatio() +
				 C->widthHeight[d].getRatio() -
				 getChildData(left_top_sibling)->leftTop[d].getRatio() -
				 getChildData(left_top_sibling)->widthHeight[d].getRatio(),
				 C->leftTop[d].getOffset() +
				 C->widthHeight[d].getOffset() -
				 C->theAttachments->getOffset(RightBottomSide[d]) -
				 getChildData(left_top_sibling)->leftTop[d].getOffset() -
				 getChildData(left_top_sibling)->widthHeight[d].getOffset() -
				 C->theAttachments->getOffset(LeftTopSide[d]));

			C->leftTop[d].set
				(getChildData(left_top_sibling)->leftTop[d].getRatio() +
				 getChildData(left_top_sibling)->widthHeight[d].getRatio(),
				 getChildData(left_top_sibling)->leftTop[d].getOffset() +
				 getChildData(left_top_sibling)->widthHeight[d].getOffset() +
				 C->theAttachments->getOffset(LeftTopSide[d]));
		}
	}}

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	// Left-or-top     is attached to frame.							[F?]
	// Right-or-bottom is ...
	//
	case Frame:
	{
		switch (right_bottom_side) {

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to frame.						[FN]
		// Right-or-bottom is attached to nothing.
		//
		case UnAttached:
		{
			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].setOffset
				(C->leftTop[d].getOffset() +
				 C->theAttachments->getOffset(LeftTopSide[d]));

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to frame.						[FP]
		// Right-or-bottom is attached to parent.
		//
		case Parent:
		{
			if (!setupChildGeometry
				(child, d, TRUE, FALSE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].setOffset
				(C->leftTop[d].getOffset() +
				 C->theAttachments->getOffset(LeftTopSide[d]));

			C->widthHeight[d].set
				(1.0F - C->leftTop[d].getRatio(),
				 -C->theAttachments->getOffset(RightBottomSide[d]) -
				  C->leftTop[d].getOffset());

			goto Done;
		}

		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to frame.						[FS]
		// Right-or-bottom is attached to sibling.
		//
		case Sibling:
		case SiblingSame:
		{
			right_bottom_sibling =
				C->theAttachments->getSibling(RightBottomSide[d]);

			if (!setupChild(right_bottom_sibling, minimum, d)) {
				goto Error;
			}

			if (!setupChildGeometry
				(child, d, TRUE, FALSE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].setOffset
				(C->leftTop[d].getOffset() +
				 C->theAttachments->getOffset(LeftTopSide[d]));

			C->widthHeight[d].set
				(getChildData(right_bottom_sibling)->
					leftTop[d].getRatio() -
				 C->leftTop[d].getRatio(),
				 getChildData(right_bottom_sibling)->
				 	leftTop[d].getOffset() -
				 C->theAttachments->getOffset(RightBottomSide[d]) -
				 C->leftTop[d].getOffset());
			C->widthHeight[d].setValid();

			goto Done;
		}
	
		// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
		// Left-or-top     is attached to frame.						[FF]
		// Right-or-bottom is attached to frame.
		//
		case Frame:
		{
			if (!setupChildGeometry
				(child, d, TRUE, TRUE, minimum, do_other_dim)) {
				goto Error;
			}

			C->leftTop[d].setOffset
				(C->leftTop[d].getOffset() +
				 C->theAttachments->getOffset(LeftTopSide[d]));

			C->widthHeight[d].setOffset
				(C->widthHeight[d].getOffset() -
				 C->theAttachments->getOffset(RightBottomSide[d]) -
				 C->theAttachments->getOffset(LeftTopSide[d]));

			goto Done;
		}
	}}}

Done:
	if (d == 0) {
		C->calculatingHorizontal = 0;
		if (do_other_dim) {
			d = 1;
			goto Begin;
		}
	}
	else {
		C->calculatingVertical = 0;
	}
	return 1;

Error:
	if (test_mode) {
		frame_error = 1;
	}
	else {
		print("VGeoForm Error: Recursive!");
	}
	return 0;
}

// --------------------------------------------------------------------------
// This is a helper routine for setupChild().
//
int VGeoForm::setupChildGeometry(VGeoFrame *child, int dimension,
								 boolean get_position, boolean get_size,
								 boolean minimum,
								 boolean &do_other_dimension)
{
	if (get_size) {
		if (dimension == 0) {
			if (get_position) {
				child->getGeometry
					(&C->leftTop[0], 0, &C->widthHeight[0], 0,
					 Ratios | NoGetPreferredSize | NoGetFontSize);
			}
			else {
				child->getGeometry
					(0, 0, &C->widthHeight[0], 0,
					 Ratios | NoGetPreferredSize | NoGetFontSize);
			}
		}
		else {
			if (get_position) {
				child->getGeometry
					(0, &C->leftTop[1], 0, &C->widthHeight[1],
					 Ratios | NoGetPreferredSize | NoGetFontSize);
			}
			else {
				child->getGeometry
					(0, 0, 0, &C->widthHeight[1],
					 Ratios | NoGetPreferredSize | NoGetFontSize);
			}
		}
	}
	else if (get_position) {
		if (dimension == 0) {
			child->getGeometry(&C->leftTop[0], 0, 0, 0,
							   Ratios | GetFontSize);
		}
		else {
			child->getGeometry(0, &C->leftTop[1], 0, 0,
							   Ratios | GetFontSize);
		}
		do_other_dimension = TRUE;
		return 1;
	}
	else {
		do_other_dimension = TRUE;
		return 1;
	}

	if (C->widthHeight[dimension].isPreferredSizeNeeded()) {
		//
		// Here, we need to query the preferred size for this dimension,
		// but since we would like to minimize/consolidate such queries,
		// we will first take a look at the other dimension to see if
		// either its preferred size needs to be queried so that we may
		// obtain both dimensions with one query, *or* see if its size
		// is constrained by the size of this VGeoForm parent so that the
		// value of that constrained size may be passed as additional
		// information to the preferred size query; this latter case
		// only happens when this is a top-down calculation.
		//
		if (C->calculatingOtherDimension) {
			//
			// This can only occur when both dimensions need
			// to have their preferred sizes queried; we can
			// just perform the query right here and continue.
			//
			if (get_position) {
				child->completeGeometry
					(&C->leftTop[0], &C->leftTop[1],
					 &C->widthHeight[0], &C->widthHeight[1]);
			}
			else if (dimension == 0) {
				child->completeGeometry
					(0, &C->leftTop[1],
					 &C->widthHeight[0], &C->widthHeight[1]);
			}
			else {
				child->completeGeometry
					(&C->leftTop[0], 0,
					 &C->widthHeight[0], &C->widthHeight[1]);
			}
		}
		else {
			int other_dimension = dimension == 0 ? 1 : 0;

#if 1
			if (!C->widthHeight[other_dimension].isComplete(minimum)) {
				C->calculatingOtherDimension = 1;
				if (!setupChild(child, minimum, other_dimension)) {
					C->calculatingOtherDimension = 0;
					return 0;
				}
				C->calculatingOtherDimension = 0;
			}
#else
			C->calculatingOtherDimension = 1;
			if (!setupChild(child, minimum, other_dimension)) {
				C->calculatingOtherDimension = 0;
				return 0;
			}
			C->calculatingOtherDimension = 0;
#endif
			if ((resizingDimension[other_dimension] > 0) &&
				C->widthHeight[other_dimension].isRatio()) {
				//
				// This is the case where this dimension needs its
				// preferred size queried and the other dimension
				// is constrained by this VGeoForm parent; pass
				// the value of the constrained size to the
				// preferred size query as an additional argument.
				//
				if (other_dimension == 0) {
					if (get_position) {
						child->completeGeometry
							(0, &C->leftTop[1], 0, &C->widthHeight[1],
							 GetConstrainedPreferredSize,
							 C->widthHeight[0].getChildDimension
							 	(resizingDimension[0]), 0);
					}
					else {
						child->completeGeometry
							(0, 0, 0, &C->widthHeight[1],
							 GetConstrainedPreferredSize,
							 C->widthHeight[0].getChildDimension
							 	(resizingDimension[0]), 0);
					}
				}
				else {
					if (get_position) {
						child->completeGeometry
							(&C->leftTop[0], 0, &C->widthHeight[0], 0,
							 GetConstrainedPreferredSize,
							 0, C->widthHeight[1].getChildDimension
							 	(resizingDimension[1]));
					}
					else {
						child->completeGeometry
							(0, 0, &C->widthHeight[0], 0,
							 GetConstrainedPreferredSize,
							 0, C->widthHeight[1].getChildDimension
							 	(resizingDimension[1]));
					}
				}
			}
			else if (C->widthHeight[other_dimension].
						isPreferredSizeNeeded()) {
				if (get_position) {
					if (minimum) {
						child->completeGeometry
							(&C->leftTop[0], &C->leftTop[1],
							 &C->widthHeight[0], &C->widthHeight[1],
							 GetMinimumPreferredSize);
					}
					else {
						child->completeGeometry
							(&C->leftTop[0], &C->leftTop[1],
							 &C->widthHeight[0], &C->widthHeight[1]);
					}
				}
				else if (dimension == 0) {
					if (minimum) {
						child->completeGeometry
							(0, &C->leftTop[1],
							 &C->widthHeight[0], &C->widthHeight[1],
							 GetMinimumPreferredSize);
					}
					else {
						child->completeGeometry
							(0, &C->leftTop[1],
							 &C->widthHeight[0], &C->widthHeight[1]);
					}
				}
				else {
					if (minimum) {
						child->completeGeometry
							(&C->leftTop[0], 0,
							 &C->widthHeight[0], &C->widthHeight[1],
							 GetMinimumPreferredSize);
					}
					else {
						child->completeGeometry
							(&C->leftTop[0], 0,
							 &C->widthHeight[0], &C->widthHeight[1]);
					}
				}
			}
			else {
				if (minimum) {
					if (get_position) {
						if (dimension == 0) {
							child->completeGeometry
								(&C->leftTop[0], 0, &C->widthHeight[0], 0,
								 GetMinimumPreferredSize);
						}
						else {
							child->completeGeometry
								(0, &C->leftTop[1], 0, &C->widthHeight[1],
								 GetMinimumPreferredSize);
						}
					}
					else {
						if (dimension == 0) {
							child->completeGeometry
								(0, 0, &C->widthHeight[0], 0,
								 GetMinimumPreferredSize);
						}
						else {
							child->completeGeometry
								(0, 0, 0, &C->widthHeight[1],
								 GetMinimumPreferredSize);
						}
					}
				}
				else {
					if (dimension == 0) {
						if (get_position) {
							child->completeGeometry
								(&C->leftTop[0], 0, &C->widthHeight[0], 0);
						}
						else {
							child->completeGeometry
								(0, 0, &C->widthHeight[0], 0);
						}
					}
					else {
						if (get_position) {
							child->completeGeometry
								(0, &C->leftTop[1], 0, &C->widthHeight[1]);
						}
						else {
							child->completeGeometry
								(0, 0, 0, &C->widthHeight[1]);
						}
					}
				}
			}
		}
		do_other_dimension = FALSE;
	}
	else {
		if (dimension == 0) {
			if (get_position) {
				child->completeGeometry
					(&C->leftTop[0], 0, &C->widthHeight[0], 0);
			}
			else {
				child->completeGeometry
					(0, 0, &C->widthHeight[0], 0);
			}
		}
		else {
			if (get_position) {
				child->completeGeometry
					(0, &C->leftTop[1], 0, &C->widthHeight[1]);
			}
			else {
				child->completeGeometry(0, 0, 0, &C->widthHeight[1]);
			}
		}
		do_other_dimension = TRUE;
	}
	return 1;
}

// ==========================================================================

#ifndef CV_NO_OLD_GEO

// --------------------------------------------------------------------------
// This implements compatibility with the old VWindow::attachWin() scheme.
//
boolean VGeoForm::oldAttachWin(VWindow *parent, VWindow *child,
							   VAttach::Side side, VAttach::Type type,
							   int offset, VWindow *sibling)
{
	if ((parent == 0) ||
		(child == 0) ||
		(child->geoFrame == 0) ||
		(child->geoFrame->getParent() != parent->geoFrame)) {
		return FALSE;
	}
	if ((parent->geoFrame == 0) || !parent->geoFrame->isFormManager()) {
		if (type == VAttach::UnAttached) {
			return TRUE;
		}
		parent->setGeoForm();
	}
	((VGeoForm *)parent->geoManager)->
		attachChild(child, VGeoForm::Side(side),
						   VGeoForm::Type(type), sibling, offset);
	return TRUE;
}

// --------------------------------------------------------------------------
//
boolean VGeoForm::oldSetAttach(VWindow *window,
							   VAttach::Side side, VAttach::Type type,
							   int offset, VWindow *sibling)
{
	if (window == 0) {
		return FALSE;
	}
	if ((window->geoFrame != 0) &&
		(window->geoFrame->getParent() != 0) &&
		 window->geoFrame->getParent()->isFormManager()) {
		((VGeoForm *)(window->geoFrame->getParent()))->
			attachChild(window->geoFrame,
						VGeoForm::Side(side),
						VGeoForm::Type(type), sibling, offset);
	}
	return TRUE;
}

// --------------------------------------------------------------------------
//
VAttach *VGeoForm::oldGetAttach(const VWindow *window)
{
	if ((window == 0) ||
		(window->geoFrame == 0) ||
		(window->geoFrame->getParent() == 0) ||
		!window->geoFrame->getParent()->isFormManager()) {
		return 0;
	}
	VGeoForm::Attachment *attachments =
		((VGeoForm *)window->geoFrame->getParent())->
			getAttachments(window->geoFrame);
	if (attachments == 0) {
		getChildData(window->geoFrame)->theAttachments = new Attachment;
	}
	if (attachments->oldAttachData == 0) {
		attachments->oldAttachData = new VAttach;
	}
	VGeoForm::Side geo_side;
	VGeoFrame *geo_sibling;
	VAttach::Side side;
	VAttach::Type type;
	int offset;
	VWindow *sibling;
	for (int i = 0 ; i < 4 ; i++) {
		geo_side = VGeoForm::Side(i);
		side = VAttach::Side(i);
		type = VAttach::Type(attachments->getType(geo_side));
		offset = attachments->getOffset(geo_side);
		geo_sibling = attachments->getSibling(geo_side);
		if (geo_sibling != 0) {
			sibling = geo_sibling->getWindow();
		}
		else {
			sibling = 0;
		}
		attachments->oldAttachData->
			setAttach(side, type, offset, sibling);
	}
	return attachments->oldAttachData;
}

// --------------------------------------------------------------------------
//
void VGeoForm::oldDetachWin(VWindow *parent, VWindow *child)
{
	if ((parent == 0) ||
		(child == 0) ||
		(parent->geoFrame == 0) ||
		!parent->geoFrame->isFormManager() ||
		(child->geoFrame == 0) ||
		(child->geoFrame->getParent() != parent->geoManager)) {
		return;
	}
	((VGeoForm *)parent->geoFrame)->detachChild(child->geoFrame);
}

// --------------------------------------------------------------------------
//
boolean VGeoForm::oldToNewArchiveConversion(VWindow *window)
{
	if (window == 0) {
		return FALSE;
	}
	 
	VOrdCollect *window_children = window->childrenOf();
	if (window_children == 0) {
		return TRUE;
	}
	 
	VGeoForm *window_manager = window->setGeoForm();
	if (window_manager == 0) {
		return FALSE;
	}

	DO (window_children, VWindow, child_window)
		if (!child_window->isTopLevel()) {
			window_manager->addChild(child_window);
			if (child_window->oldAttachArchiveData != 0) {
				for (int i = 0 ; i < 4 ; i++) {
					int type = child_window->oldAttachArchiveData->
							   attach.getType(VAttach::Side(i));
					int offset = child_window->oldAttachArchiveData->
								 attach.getOffset(VAttach::Side(i));
					VWindow *sibling;
					if ((type == Sibling) || (type == SiblingSame)) {
 						int sibling_index =
							int(child_window->
								oldAttachArchiveData->attachSibling[i]);
						if (sibling_index > 0) {
							sibling = (VWindow *)window_children->
												 idAt(sibling_index - 1);
						}
						else {
							sibling = child_window->oldAttachArchiveData->
									  attach.getSibling(VAttach::Side(i));
						}
						if (sibling == 0) {
							type = UnAttached;
						}
					}
					else {
						sibling = 0;
					}
					window_manager->
						attachChild(child_window,
									Side(i), Type(type), sibling, offset);
				}
			}
		}
	END

	return TRUE;
}

#endif

// ==========================================================================

#if !defined(CV_NOARCHIVER)

// --------------------------------------------------------------------------
//
void VGeoForm::putTo(VArchiver& a)
{
	putTo(a, TRUE);
	putToDone(a);
}

// --------------------------------------------------------------------------
//
void VGeoForm::putTo(VArchiver& a, boolean)
{
	// Write the base class data.

	VGeoManager::putTo(a, FALSE);

	// Write the revision tag.

	a << VGeoFormArchiveTag;

	// Write the class specific data.

	;
}

// --------------------------------------------------------------------------
//
void VGeoForm::getFrom(VArchiver& a, VObject *object, boolean)
{
	getFrom(a);
	getFromDone(a, object);
}

// --------------------------------------------------------------------------
//
void VGeoForm::getFrom(VArchiver& a)
{
	// Read the base class data.

	VGeoManager::getFrom(a);

	// Read and verify the revision tag.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VGeoFormArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VGeoFormArchiveTag));
		return;
	}

	// Read the class specific data.

	;
}

// --------------------------------------------------------------------------
//
void VGeoForm::gotChildren()
{
	// Fix up the sibling indices to point to actual VGeoFrame objects.

	if (hasChildren()) {
		ChildIterator children(this, TRUE);
		VGeoFrame *child;
		while ((child = children.next()) != 0) {
			if (C->theAttachments != 0) {
				for (int i = 0 ; i < 4 ; i++) {
					if ((C->theAttachments->theType[i] == Sibling) ||
						(C->theAttachments->theType[i] == SiblingSame)) {
						int index = C->theAttachments->theSiblingIndex[i];
						C->theAttachments->theSibling[i] = getChild(index);
					}
				}
			}
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::FormChildData::putTo(VArchiver& a)
{
#ifndef __SC__
	ManagerChildData::putTo(a);
#else
	putTo(a);
#endif

	if (theAttachments != 0) {
		a << char(1);
		theAttachments->putTo(a);
	}
	else {
		a << char(0);
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::FormChildData::getFrom(VArchiver& a)
{
#ifndef __SC__
	ManagerChildData::getFrom(a);
#else
	getFrom(a);
#endif

	char flag;
	a >> flag;
	if (flag != char(0)) {
		theAttachments = new Attachment;
		theAttachments->getFrom(a);
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::Attachment::putTo(VArchiver& a)
{
	for (int i = 0 ; i < 4 ; i++) {
		a << char(theType[i]);
		a << short(theOffset[i]);
		if ((theType[i] != Sibling) && (theType[i] != SiblingSame)) {
			a << long(-1);
		}
		else if (theSibling[i] != 0) {
			a << long(theSibling[i]->getIndex());
		}
		else {
			a << long(-2);
		}
	}
}

// --------------------------------------------------------------------------
//
void VGeoForm::Attachment::getFrom(VArchiver& a)
{
	char type;
	short offset;
	long index;
	for (int i = 0 ; i < 4 ; i++) {
		a >> type;
		a >> offset;
		a >> index;
		theType[i] = type;
		theOffset[i] = offset;
		if ((theType[i] == Sibling) ||
			(theType[i] == SiblingSame) && (index >= 0)) {
			theSiblingIndex[i] = int(index);
		}
	}
}

#endif
