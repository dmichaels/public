// $Header:   Q:/views/common/vcs/TOOLBAR.H_V   1.29   Mar 24 1997 16:13:54   scottt  $

//  toolbar.h
//
//  VToolBar interface [Common]
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
//  10/19/95 dgm    Original.
//  11/01/95 dgm    Updated.
//  11/03/95 dgm    Changed initialize() to take one parameter as before.
//  11/15/95 dgm    Updated.
//  11/20/95 dgm    Changed setSpacing(), setGroupSpacing(), and beginGroup()
//                  to take an optional minor spacing parameter.
//                  Added set/getMinorSpacing()
//                  and set/getMinorGroupSpacing().
//  11/29/95 dgm    Added enableBool().
//  11/30/95 dgm    Added isEnabled() and isDisabled().
//  12/01/95 dgm    Added newSeparator() for backward compatibility.
//  12/05/95 dgm    Changed to use VColorScheme::systemXXX().
//  12/08/95 dgm    Motif additions.
//  12/09/95 dss    friend VStdMdiAppView: Motif only.
//  02/20/96 dgm    Minor constructor declaration fix.
//	02/22/96 dgm	Overrode getPreferredSize().
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef TOOLBAR_H
#define TOOLBAR_H

// --------------------------------------------------------------------------
//
#include "control.h"

// --------------------------------------------------------------------------
//
CLASS   VBrush;
CLASS   VClass;
CLASS   VControl;
CLASS   VGeoRowOrColumn;
CLASS   VPen;
CLASS   VStyle;
CLASS   VWindow;
CLASS   VAppView;

#ifndef CV_NOARCHIVER
CLASS   VArchiver;
#endif

#if defined(CV_MOTIF)
CLASS   VAppView;
CLASS   VStdMdiAppView;
#endif

// For backward compatibility with old VToolbar only.

CLASS   VSeparator;

// --------------------------------------------------------------------------
//
CLASS VToolBar : public VControl {
public:
    enum ShadeStyle {
                        NoShade,
                        RaisedShade,
                        SunkenShade
    };

public:
                        VToolBar(const VFrame&, VWindow *,
                                 const VStyle& = StyleDefault);
                        VToolBar(VWindow *, const VStyle& = StyleDefault);
                        VToolBar(const VString&,
                                 const VFrame&, VWindow *,
                                 const VStyle& = StyleDefault);
                        VToolBar(const VString&,
                                 VWindow *, const VStyle& = StyleDefault);
                        VToolBar();
                       ~VToolBar();
    VClass             *iam();

    void                setRow();
    void                setColumn();

    boolean             isRow() const;
    boolean             isColumn() const;

    void                setWrapping(boolean);
    boolean             getWrapping() const;

    void                setSpacing(int, int = -1);
    int                 getSpacing() const;

    void                setMinorSpacing(int);
    int                 getMinorSpacing() const;

    void                setMargins(int);
    void                setMargins(int, int);
    void                setMargins(int, int, int, int);
    void                getMargins(int *, int *, int *, int *);

    VGeoRowOrColumn    *beginGroup(int = -1, int = -1);
    void                endGroup();

    void                setGroupSpacing(int, int = -1);
    int                 getGroupSpacing() const;

    void                setMinorGroupSpacing(int);
    int                 getMinorGroupSpacing() const;

    void                addSeparator(int = 0);
    void                addSpace(int = 0);

    void                setShadeStyle(int);
    int                 getShadeStyle() const;

    boolean             isEnabled() const;
    boolean             isDisabled() const;

    void                updateGeometry(int = 0);

#ifndef CV_NOARCHIVER
    virtual void        putTo(VArchiver&);
    virtual void        getFrom(VArchiver&);
    virtual void        getFrom(VArchiver&, VObject *, boolean = TRUE);
    virtual void        realize(VWindow *);
#endif

    // For backward compatibility with old VToolBar.

                        VToolBar(const VFrame&, VWindow *, boolean);
                        VToolBar(const VString&,
                                 const VFrame&, VWindow *, boolean);
    void                setAutoPlace(boolean, short = 2);
    VSeparator         *newSeparator(int = 4);


    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // Protected section; please KEEP OUT if you're a non-deriving user!
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
    virtual void        addChild(VWindow *);
    virtual boolean     paint();
    virtual void    	getPreferredSize(int *, int *) const;
    virtual void        getInternalBorderGeometry(int *, int *,
                                                  int *, int *) const;
public:
    virtual void        enableBool(boolean);
    virtual VStyle&     getDefaultStyle() const;
    static VStyle&      getClassDefaultStyle();

protected:

#ifndef CV_NOARCHIVER
    void                putTo(VArchiver&, boolean);
#endif

protected:
    void                initialize(boolean = TRUE);
    void                construct();
    void                setupManager(VClass * = 0);
protected:
    static void         constructStaticData();
    static void         deleteStaticData();

    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    // Private section!
    // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
    unsigned char       majorSpacing;
    unsigned char       minorSpacing;
    unsigned char       majorGroupSpacing;
    unsigned char       minorGroupSpacing;
    unsigned char       majorNearMargin;
    unsigned char       majorFarMargin;
    unsigned char       minorNearMargin;
    unsigned char       minorFarMargin;
    char                shadeStyle;
    VBrush             *shadedBackgroundBrush;
    VPen               *shadedHighlightPen;
    VPen               *shadedShadowPen;
    VGeoRowOrColumn    *currentGroup;
    short               currentlyDisabled : 1;

    // This junk is currently only used by Motif; this needs clean up!

    VAppView           *appView;
    short               isStdMdiViewToolBar : 1;
#if defined(CV_MOTIF)
    friend              VStdMdiAppView;
#endif

};

extern VClass *VToolBarCls;

// --------------------------------------------------------------------------
//
inline int VToolBar::getShadeStyle() const
{
    return shadeStyle;
}

// --------------------------------------------------------------------------
//
inline boolean VToolBar::isEnabled() const
{
    return !currentlyDisabled;
}

// --------------------------------------------------------------------------
//
inline boolean VToolBar::isDisabled() const
{
    return currentlyDisabled;
}

#endif // TOOLBAR_H
