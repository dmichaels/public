// $Header:   Q:/views/win/vcs/radibttn.h_v   1.14   Mar 04 1997 16:32:56   gregm  $ 

//  radibttn.h
//
//  VRadioButton interface.
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1995-1997 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  05/13/93 pat    preened;
//                  added constructor that takes name argument;
//                  added getFrom(), putTo(), realize() and ref to VArchiver
//  05/14/93 pat    added destructor (virtual)
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  08/09/93 jmd    made VFrame const
//  08/09/93 jmd    removed free
//  08/16/93 jmd    made name const VString
//  09/01/95 dss    VStyle changes.
//	01/24/95 evc    Overrode VButton::clicked()
//	04/19/96 glm	removed v1.1 code/preened
// ----------------------------------------------------------------------------

#ifndef VRADIOBUTTON_H
#define VRADIOBUTTON_H

#include "checkbox.h"

extern VClass *VRadioButtonCls;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

CLASS VExclusiveGroup;

//  Introduction
//
//  A VRadioButton is a button displayed with a small circle
//  to the left of its text content (label).  The circle can be empty
//  to imply unchecked or filled with black to imply checked.  If a
//  VRadioButton object has the automatic attribute, then it will
//  automatically toggle between the checked and unchecked state.
//

CLASS VRadioButton : public VCheckBox {

public:

    //  Constructors

                VRadioButton();
                VRadioButton(   unsigned cntrlid, 
                                VWindow *pwin);
                VRadioButton(   VRatio x, VRatio y, 
                                char *label, 
                                VWindow *pwin, 
								const VStyle& style = StyleAutoToggle);
                VRadioButton(   const VFrame& frame, 
                                VWindow *pwin, 
								const VStyle& style = StyleAutoToggle, 
                                char *label = 0);
                VRadioButton(   const VString& wname, 
                                const VFrame& frame, 
                                VWindow *pwin, 
								const VStyle& style = StyleAutoToggle, 
                                char *label = 0);
                ~VRadioButton();

    VClass     *iam();

	boolean			toggle();
	boolean 		clicked(); // added 

    void            check(boolean);

    boolean         givenFocus();
    boolean         clearFocus();


    void groupNo(short a_No) { m_groupNo = a_No; }
    short groupNo() {return m_groupNo; }

    //  Archival and Retrieval

#ifndef CV_NOARCHIVER
    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
    void            putTo(VArchiver& a, boolean most_derived);
    void            getFrom(VArchiver& a, VObject *pwin,
                            boolean do_realize = TRUE);
    virtual void    realize(VWindow *pwin);
#endif

protected:

	boolean	notify(int);
    void	getInfo(VWinInfo&);
	friend VExclusiveGroup;

private:
    short m_groupNo;

};

#endif  //  VRADIOBUTTON_H
