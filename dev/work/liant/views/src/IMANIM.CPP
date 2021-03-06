//	imanim.cpp
//
//	VAnimatedImageBox implementation [Common]
//
//  Allegris Foundation 1.1.00
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
//	09/21/96 dgm	Original.
//	01/13/97 djs	#ifdef'd 'OutputDebugString' for PM
//	01/15/97 dgm	Removed OutputDebugString.
// --------------------------------------------------------------------------

#include "imanim.h"
#include "image.h"
#include "timer.h"
#include "notifier.h"
#include "str.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

#include <stdlib.h>
#include <time.h>

// --------------------------------------------------------------------------
//
defineClass(VAnimatedImageBox, VImageBox)

#ifndef CV_NOARCHIVER
defineArchiveRevision(VAnimatedImageBox, 0)
#endif

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox()
	: VImageBox()
{
	initialize();
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(VWindow *parentwin, const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(frame, parentwin, style);
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(VWindow *parentwin, VImage *image, int index,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(const VString& name,
						   VWindow *parentwin, const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(const VString& name,
						   const VFrame& frame, VWindow *parentwin,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(frame, parentwin, style, name);
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::VAnimatedImageBox(const VString& name,
						   VWindow *parentwin, VImage *image, int index,
						   const VStyle& style)
	: VImageBox()
{
	initialize();
	(*this)(VFrame(), parentwin, style, name);
	setImage(image, index);
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(VWindow *parentwin)
{
	(*this)(VFrame(), parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(VWindow *parentwin, const VStyle& style)
{
	(*this)(VFrame(), parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(VWindow *parentwin,
							  const VStyle& style, const VString& name)
{
	(*this)(VFrame(), parentwin, style, name);
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(const VFrame& frame, VWindow *parentwin)
{
	(*this)(frame, parentwin, StyleDefault, "");
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(const VFrame& frame, VWindow *parentwin,
							  const VStyle& style)
{
	(*this)(frame, parentwin, style, "");
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::operator()(const VFrame& frame, VWindow *parentwin,
							  const VStyle& style, const VString& name)
{
	VImageBox::operator()(frame, parentwin, style, name);
	construct();
}

// --------------------------------------------------------------------------
//
VAnimatedImageBox::~VAnimatedImageBox()
{
	delete theTimer;
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::initialize()
{
	theTimer = 0;
	imageChangedMethod = 0;
	milliSeconds = 1000 / Normal;
	currentlyPlaying = 0;
	rowMajor = 0;
	playBackwards = 0;
	playRandom = 0;
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::construct()
{
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::uponImageChanged(VObject *o, method m)
{
	setClient(o);
	imageChangedMethod = (void (VObject::*)(VAnimatedImageBox *))m;
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::setSpeed(VAnimatedImageBox::Speed speed)
{
	switch (speed) {
	default:
		return;
	case Slowest:
	case VerySlow:
	case Slow:
	case Normal:
	case Fast:
	case VeryFast:
	case Fastest:
		break;
	}

	setFramesPerSecond(speed);
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::setFramesPerSecond(int fps)
{
	setMilliSeconds(1000 / fps);
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::setMilliSeconds(int ms)
{
	if (ms != milliSeconds) {
		milliSeconds = ms;
		if (currentlyPlaying) {
			getTimer()->start(milliSeconds);
		}
	}
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::setRowMajor(boolean b)
{
	rowMajor = b;
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::play(boolean b)
{
	if (b) {
		if (!currentlyPlaying) {
			if ((getImageMajorCount() > 1) || (getImageMinorCount() > 1)) {
				setImmediateUpdate(FALSE);
				currentlyPlaying = 1;
				getTimer()->start(milliSeconds);
			}
		}
	}
	else if (currentlyPlaying) {
		currentlyPlaying = 0;
		getTimer()->stop();
		setImmediateUpdate(TRUE);
	}
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::showNext()
{
	if (playBackwards) {
		playBackwards = 0;
		showPrevious();
		playBackwards = 1;
		return;
	}

	int major_index = getImageMajorIndex();
	int minor_index = getImageMinorIndex();
	int major_count = getImageMajorCount();
	int minor_count = getImageMinorCount();

	if (playRandom) {
		static int not_seeded = 1;
		if (not_seeded) {
			not_seeded = 0;
			srand(unsigned(time(0)));
		}
		int i = rand() % major_count;
		int j = rand() % minor_count;
		if (i == major_index) {
			if (++i == major_count) { i = 0; }
		}
		if (j == minor_index) {
			if (++j == minor_count) { j = 0; }
		}
		major_index = i;
		minor_index = j;
	}
	else {
		if (rowMajor) {
			if (++major_index == major_count) {
				major_index = 0;
				if (++minor_index == minor_count) {
					minor_index = 0;
				}
			}
		}
		else {
			if (++minor_index == minor_count) {
				minor_index = 0;
				if (++major_index == major_count) {
					major_index = 0;
				}
			}
		}
	}

	setImageIndex(major_index, minor_index);
	repaintContents();
	imageChanged();
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::showPrevious()
{
	if (playBackwards) {
		playBackwards = 0;
		showNext();
		playBackwards = 1;
		return;
	}

	int major_index = getImageMajorIndex();
	int minor_index = getImageMinorIndex();
	int major_count = getImageMajorCount();
	int minor_count = getImageMinorCount();

	if (rowMajor) {
		if (--major_index < 0) {
			major_index = major_count - 1;
			if (--minor_index < 0) {
				minor_index = minor_count - 1;
			}
		}
	}
	else {
		if (--minor_index < 0) {
			minor_index = minor_count - 1;
			if (--major_index < 0) {
				major_index = major_count - 1;
			}
		}
	}

	setImageIndex(major_index, minor_index);
	repaintContents();
	imageChanged();
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::showFirst()
{
	setImageIndex(0, 0);
	repaintContents();
	imageChanged();
}

// --------------------------------------------------------------------------
//
void VAnimatedImageBox::showLast()
{
	setImageIndex(getImageMajorCount() - 1, getImageMinorCount() - 1);
	repaintContents();
	imageChanged();
}


// --------------------------------------------------------------------------
//
void VAnimatedImageBox::updateImage()
{
	showNext();
}

// --------------------------------------------------------------------------
//
VTimer *VAnimatedImageBox::getTimer() const
{
	if (theTimer == 0) {
		((VAnimatedImageBox *)this)->theTimer =
			new VTimer((VAnimatedImageBox *)this,
					    methodOf(VAnimatedImageBox, updateImage));
	}
	return theTimer;
}

// ===========================================================================

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
void VAnimatedImageBox::putTo(VArchiver& a)
{
	VAnimatedImageBox::putTo(a, TRUE);
}

// ---------------------------------------------------------------------------
//
void VAnimatedImageBox::putTo(VArchiver& a, boolean most_derived)
{
	// Write the base object data.

	VImageBox::putTo(a, FALSE);

	// Write the object version tag.

	a << VAnimatedImageBoxArchiveTag;

	// Write any object specific data.

	;
}

// ---------------------------------------------------------------------------
//
void VAnimatedImageBox::getFrom(VArchiver& a)
{
	// Read the base object data.

	VImageBox::getFrom(a);

	// Read object version tag and verify.

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VAnimatedImageBoxArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VAnimatedImageBoxArchiveTag));
		return;
	}

	// Read any object specific data.

	;
}

// ---------------------------------------------------------------------------
//
void VAnimatedImageBox::getFrom(VArchiver& a,
						   VObject *parentwin, boolean do_realize)
{
	VAnimatedImageBox::getFrom(a);

	if (do_realize) {
		VAnimatedImageBox::realize((VWindow *)parentwin);
	}
}

// ---------------------------------------------------------------------------
//
void VAnimatedImageBox::realize(VWindow *parentwin)
{
	VImageBox::realize(parentwin);
	construct();
}

#endif // !CV_NOARCHIVER
