// $Header:   Q:/views/common/vcs/notepage.h_v   1.12   May 30 1997 17:16:02   scottt  $ 

//  notepage.h
//
//  VNotePage interface [Common]
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
//  06/23/95 dgm    Original check-in; original code from jmd.
//  08/15/95 krb    Added variable size tabs.
//  08/21/95 dgm    Changed #ifdef ARCHIVER to #ifndef CV_NOARCHIVER.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VNOTEPAGE_H
#define VNOTEPAGE_H
#include "object.h"
#include "str.h"
#include "window.h"

CLASS VNoteBook;
CLASS VbNoteBookInspector;
CLASS VBitMap;

extern VClass *VNotePageCls;

CLASS VNotePage : public VObject {
public:
            VNotePage();
            VNotePage(const VString& caption, 
                      const VString& winname, VWindow *win, boolean vrfwin);

            ~VNotePage();
    VClass *iam();

    VWindow* getWindow() {return theWindow;}

    friend  VNoteBook;
    friend  VbNoteBookInspector;

#ifndef CV_NOARCHIVER
    void    getFrom(VArchiver& a);
    void    getFrom(VArchiver& a, VObject *parent, boolean do_realize);
    void    putTo(VArchiver& a);
#endif

private:
    VString     theCaption;
    VString     theWindowName;          // name of page in .vrf file
    VString     theBitMapName;          // name of tab bmap in .vrf file

    boolean     isEnabled;
    boolean     isVrfWindow;            // window loaded from .vrf file

    int         width;                  // width of tab
    VWindow    *theWindow;              // pointer to page window
    VBitMap    *theBitMap;              // pointer to tab bitmap
};

#endif // VNOTEPAGE_H
