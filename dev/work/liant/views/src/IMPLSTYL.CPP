// $Header:   Q:/views/common/vcs/IMPLSTYL.CPV   1.14   Mar 24 1997 16:09:20   scottt  $

//  implstyl.cpp
//
//  Implict Style Class [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  09/01/95 dss    created.
//  09/26/95 dss    convertOldToNew(): account for overlap in old styles
//                  using isA().
//  10/12/95 dss    don't allow set/add on ourselves.
//  10/18/95 dss    convertOldToNew(): ignore old justification: always
//                  convert to left due to a Painter bug.
//  10/18/95 dss    convertOldToNew(): restrict StyleLeft to only VEditLine's.
//  12/06/95 dss    name change: VStyleExpression.
//
//	02/03/97 pkt	updated 'implicit' to be a pointer, allocating as needed.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "defs.h"
#include "archiver.h"

#include "cvassert.h"

#include "bitset.h"
#include "style.h"
#include "exclstyl.h"
#include "implstyl.h"
#include "stylexpr.h"
#include "styles.h"
#include "window.h"

extern VClass *VButtonCls;
extern VClass *VComboBoxCls;
extern VClass *VEditLineCls;
extern VClass *VEditBoxCls;
extern VClass *VListBoxCls;
extern VClass *VTextBoxCls;
extern VClass *VViewCls;
extern VClass *VDialogCls;

#define OldStyleDefault         (short(0x0))
#define OldStyleBorder          (short(0x1))
#define OldStyleHorizontal      (short(0x2))
#define OldStyleVertical        (short(0x4))
#define OldStyleScrolls         (short(0x6))

#if !defined(macintosh)
#   define OldStyleCloseBox         (short(0x8))
#   define OldStyleSizable          (short(0x10))
#   define OldStyleCaption          (short(0x20))
#elif defined(macintosh)
#   define OldStyleCloseBox         (short(0x4000))
#   define OldStyleSizable          (short(0x2000))
#   define OldStyleCaption          (short(0x1000))
#endif

#define OldStyleAutoToggle      (short(0x8))
#define OldStyleDefaultButton   (short(0x10))

#if !defined(MOTIF)
#   define OldStyleNoAutoDefault    (short(0x02))
#elif defined(MOTIF)
#   define OldStyleNoAutoDefault    (short(0x20))
#endif

#define OldStyleSorted          (short(0x8))
#define OldStyleComboEdit       (short(0x10))
#define OldStyleComboEditDrop   (short(0x20))
#define OldStyleComboDrop       (short(0x40))
#define OldStyleLeft            (short(0x0))
#define OldStyleRight           (short(0x8))
#define OldStyleCenter          (short(0x10))

#if !defined(macintosh)
#   define OldStyleWordWrap     (short(0x200))
#elif defined(macintosh)
#   define OldStyleWordWrap     (short(0x20))
#endif

#define OldStyleModalBorder     (short(0x40))
#define OldStyleMaximize        (short(0x80))
#define OldStyleMinimize        (short(0x100))
#define OldStyleMaxMin          (short(0x180))
#define OldStyleMaximized       (short(0x200))
#define OldStyleMinimized       (short(0x400))
#define OldStyleHidden          (short(0x800))

#if defined(MOTIF)
#   define OldStyleAutoScroll       (short(0x1000))
#endif

#define OldStyleEditSecure      (short(0x2000))
#define OldStyleReadOnly        (short(0x4000))

#if !defined(PM)
#   define OldStyleShadow       (short(0x8000))
#elif defined(PM)
#   define OldStyleShadow           (short(0x0))
#   define OldStyleSizeRedraw       (short(0x8000))
#endif

#define OldStyleDialogPM        (short(0x1000))
#define OldStyleDialogWin       (short(0x2000))

defineClass(VImplicitStyle, VExclusiveStyle)

// -----------------------------------------------------------------------
void VImplicitStyle::setExclusive(class VImplicitStyle **a) 
{ 
    VStyle  x;
    class VImplicitStyle **p = a;
    
    // assemble the elements into one collection.
    while (*p) {
        x += **p++;
        // What!! 50 styles in the same set. Someone musta forgot the
        // terminating zero in the array.
        cv_assert(p < a + 50);
    }
    
    // stick the collection back into the indiviual elements.
    p = a;
    while (*p) {
        (*p++)->exclude = x;
    }
}

// -----------------------------------------------------------------------
void VImplicitStyle::setImplicit(const class VImplicitStyle **a) 
{
	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) { 
		// do the intital set (clears implicit of leftovers)
		*implicit = **a++;

		while (*a) {
			// assert that the implicit style is NOT exclusive with this style.
			// Thats an amibguity. Otherwise setting the implied style would
			// unset the explicit style.
			cv_assert(!exclude.contains(**a));
			*implicit += **a++;
		}
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::addImplicit(const class VImplicitStyle **a) 
{ 
	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) {
		while (*a) {
			// assert that the implicit style is NOT exclusive with this style.
			// Thats an amibguity. Otherwise adding the implied style would
			// remove the explicit style.
			cv_assert(!exclude.contains(**a));
			*implicit += **a++;
		}
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::removeImplicit(const class VImplicitStyle **a) 
{ 
	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) {
		while (*a) {
			*implicit -= **a++;
		}
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::setImplicit(const class VImplicitStyle& a) 
{ 
 	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) {
		// assert that the implicit style is NOT exclusive with this style.
		// Thats an amibguity. Otherwise setting the implied style would
		// unset the explicit style.
		cv_assert(!exclude.contains(a));
		*implicit = a;
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::addImplicit(const class VImplicitStyle& a) 
{ 
	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) {
		// assert that the implicit style is NOT exclusive with this style.
		// Thats an amibguity. Otherwise adding the implied style would
		// remove the explicit style.
		cv_assert(!exclude.contains(a));
		*implicit += a;
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::removeImplicit(const class VImplicitStyle& a) 
{ 
	if (implicit == 0) {
		implicit = new VExclusiveStyle("implicit");
	}
	if (implicit != 0) {
		*implicit -= a;
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::set(const VStyleExpression& a)
{
    set(a.getResult());
}

// -----------------------------------------------------------------------
void VImplicitStyle::add(const VStyleExpression& a)
{
    add(a.getResult());
}

// -----------------------------------------------------------------------
void VImplicitStyle::remove(const VStyleExpression& a)
{
    remove(a.getResult());
}

// -----------------------------------------------------------------------
void VImplicitStyle::set(const VImplicitStyle& a)
{
    if (this == &a) {
        return;
    }

    VExclusiveStyle::set(a);

    if (a.implicit != 0 && !a.implicit->isEmpty()) {
        VImplicitStyle::add(*a.implicit);
    }
}

// -----------------------------------------------------------------------
void VImplicitStyle::add(const VImplicitStyle& a) 
{ 
    if (this == &a) {
        return;
    }

    VExclusiveStyle::add(a);

    if (a.implicit != 0 && !a.implicit->isEmpty()) {
        VImplicitStyle::add(*a.implicit);
    }

}

// -----------------------------------------------------------------------
void VImplicitStyle::remove(const VImplicitStyle& a) 
{ 
    VExclusiveStyle::remove(a);

    if (a.implicit != 0 && !a.implicit->isEmpty()) {
        VImplicitStyle::remove(*a.implicit);
    }
}

// -----------------------------------------------------------------------
void VImplicitStyle::set(const VStyle& a)
{
    if (a.isA(VImplicitStyleCls)) {
        VImplicitStyle::set((const VImplicitStyle&)a);
    }
    else if (a.isA(VExclusiveStyleCls)) {
        VExclusiveStyle::set((VExclusiveStyle&)a);
    }
    else {
        VStyle::set(a);
    }
}

// -----------------------------------------------------------------------
void VImplicitStyle::add(const VStyle& a)
{
    if (a.isA(VImplicitStyleCls)) {
        VImplicitStyle::add((VImplicitStyle&)a);
    }
    else if (a.isA(VExclusiveStyleCls)) {
        VExclusiveStyle::add((VExclusiveStyle&)a);
    }
    else {
        VStyle::add(a);
    }
}

// -----------------------------------------------------------------------
void VImplicitStyle::remove(const VStyle& a)
{
    if (a.isA(VImplicitStyleCls)) {
        VImplicitStyle::remove((VImplicitStyle&)a);
    }
    else if (a.isA(VExclusiveStyleCls)) {
        VExclusiveStyle::remove((VExclusiveStyle&)a);
    }
    else {
        VStyle::remove(a);
    }
}

#if 0 // !defined(USE_EXPRESSIONS)

// -----------------------------------------------------------------------
VImplicitStyle operator+(const VImplicitStyle& a, const VImplicitStyle& b)
{
    VImplicitStyle tmp(a);
    return tmp += b;
}

// -----------------------------------------------------------------------
VImplicitStyle operator-(const VImplicitStyle& a, const VImplicitStyle& b)
{
    VImplicitStyle tmp(a);
    return tmp -= b;
}
#endif

#if defined(V_UNIT_TEST)
// -----------------------------------------------------------------------
void VImplicitStyle::dump(ostream& s) const
{   
    VExclusiveStyle::dump(s);

    if (implicit != 0 && !implicit->isEmpty()) {
        s << ", ";
        // Use VStyle::dump to prevent dumping the extraneous
        // stuff we don't care about.
        implicit->VStyle::dump(s);
    }

}
#endif

// ---------------------------------------------------------------------------
//
VImplicitStyle::~VImplicitStyle()
{
	delete implicit;
}

#ifndef CV_NOARCHIVER
// -----------------------------------------------------------------------
void VImplicitStyle::convertOldToNew(short oldStyle, VWindow& win)
{
#if !defined(V_UNIT_TEST)
    if (oldStyle & OldStyleBorder) {
        win.getStyle() += StyleBorder;
    }
    if (oldStyle & OldStyleHorizontal) {
        win.getStyle() += StyleHorizontal;
    }
    if (oldStyle & OldStyleVertical) {
        win.getStyle() += StyleVertical;
    }
    if (oldStyle & OldStyleCloseBox) {
        win.getStyle() += StyleCloseBox;
    }
    if (oldStyle & OldStyleSizable) {
        win.getStyle() += StyleSizable;
    }
    if (oldStyle & OldStyleCaption) {
        win.getStyle() += StyleCaption;
    }

    if (win.isA(VButtonCls)) {
        if (oldStyle & OldStyleAutoToggle) {
            win.getStyle() += StyleAutoToggle;
        }
        if (oldStyle & OldStyleDefaultButton) {
            win.getStyle() += StyleDefaultButton;
        }
        if (oldStyle & OldStyleNoAutoDefault) {
            win.getStyle() += StyleNoAutoDefault;
        }
    } // VButton

    if (win.isA(VListBoxCls)) {
        if (oldStyle & OldStyleSorted) {
            win.getStyle() += StyleSorted;
        }
    } // VListBox

    if (win.isA(VComboBoxCls)) {
        if (oldStyle & OldStyleComboEdit) {
            win.getStyle() += StyleComboEdit;
        }
        if (oldStyle & OldStyleComboEditDrop) {
            win.getStyle() += StyleComboEditDrop;
        }
        if (oldStyle & OldStyleComboDrop) {
            win.getStyle() += StyleComboDrop;
        }
    } // VComboBox

    if (win.isA(VTextBoxCls)) {

        if (win.isA(VEditBoxCls)) {
            //  Ignore old justification: always convert to left due to a 
            // Constructor bug that set the default to StyleCenter.
            win.getStyle() += StyleLeft;
        } // is a VEditBox
        else {
            if (oldStyle & OldStyleRight) {
                win.getStyle() += StyleRight;
            }
            else if (oldStyle & OldStyleCenter) {
                win.getStyle() += StyleCenter;
            }
            else { // StyleLeft
                win.getStyle() += StyleLeft;
            }
        } // else not a VEditBox

        if (oldStyle & OldStyleWordWrap) {
            win.getStyle() += StyleWordWrap;
        }
        if (oldStyle & OldStyleEditSecure) {
            win.getStyle() += StyleEditSecure;
        }
        if (oldStyle & OldStyleReadOnly) {
            win.getStyle() += StyleReadOnly;
        }

    } // VTextBox

    if (win.isA(VViewCls)) {
        if (oldStyle & OldStyleModalBorder) {
            win.getStyle() += StyleModalBorder;
        }
        if (oldStyle & OldStyleMaximize) {
            win.getStyle() += StyleMaximize;
        }
        if (oldStyle & OldStyleMinimize) {
            win.getStyle() += StyleMinimize;
        }
        if (oldStyle & OldStyleMaxMin) {
            win.getStyle() += StyleMaxMin;
        }
        if (oldStyle & OldStyleMaximized) {
            win.getStyle() += StyleMaximized;
        }
        if (oldStyle & OldStyleMinimized) {
            win.getStyle() += StyleMinimized;
        }
#       if defined(MOTIF)
        if (oldStyle & OldStyleAutoScroll) {
            win.getStyle() += StyleAutoScroll;
        }
#       endif
    } // VView

    if (oldStyle & OldStyleHidden) {
        win.getStyle() += StyleHidden;
    }

    if (oldStyle & OldStyleShadow) {
        win.getStyle() += StyleShadow;
    }
#if defined(PM)
    if (oldStyle & OldStyleSizeRedraw) {
        win.getStyle() += StyleSizeRedraw;
    }
#endif

    if (win.isA(VDialogCls)) {
#       if defined(PM)
        if (oldStyle & OldStyleDialogPM) {
            win.getStyle() += StyleDialogPM;
        }
#       endif
        if (oldStyle & OldStyleDialogWin) {
            win.getStyle() += StyleDialogWin;
        }
    } // VDialog
#endif
}

// -----------------------------------------------------------------------
void VImplicitStyle::getFrom(VArchiver& a)
{
#if defined(V_UNIT_TEST)
    myCout << "retrieving Style " << name << endl;
#endif
    
    VExclusiveStyle::getFrom(a);

	if (implicit != 0) {
		a.getObject(implicit);
	}
	else {
		implicit = (VExclusiveStyle *)a.getObject(0);
		if (implicit != 0 && implicit->isEmpty()) {
			delete implicit;
			implicit = 0;
		}
	}
}

// -----------------------------------------------------------------------
void VImplicitStyle::putTo(VArchiver& a)
{
#if defined(V_UNIT_TEST)
    myCout << "archiving Style " << name << endl;
#endif

    VExclusiveStyle::putTo(a);

	if (implicit != 0) {
		a.putObject(implicit);
	}
	else {
		VExclusiveStyle imp("implicit");
		a.putObject(&imp);
	}
}
#endif

