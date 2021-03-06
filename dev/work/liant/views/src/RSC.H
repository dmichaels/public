// $Header:   Q:/views/common/vcs/RSC.H_V   1.17   Mar 24 1997 16:13:14   scottt  $ 

//  rsc.h
//
//  VResource interface [Common]
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
//  12/04/95 dgm    Added this header.
//  12/04/95 dgm    Added a new loadContents() -- from KerstinB.
//  01/29/96 pkt    added 'getFileName', preened.
//  01/31/96 pkt    fixed omission of 'inline' keyword in 1/29 change.
//	07/03/96 pkt	added HTML support.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef V_RSC_H
#define V_RSC_H

#include "defs.h"
#if defined(CV_REMOTE_GUI)
#define CV_HTML
#include "file.h"
#endif

CLASS VWindow;
CLASS VClass;
CLASS VMenu;
CLASS VFont;
CLASS VBrush;
CLASS VBitMap;
CLASS VPopupMenu;
#include "str.h"
#include "rscarch.h"

CLASS VResource {
public:
                    VResource();
                    ~VResource();

    boolean         open();
    boolean         open(const VString& vrfName);
    void            close();

    boolean         loadWindow(const VString& wname,
                               VWindow *window,
                               VWindow *parent,
                               VClass *cls = 0);
    boolean         loadContents(const VString& wname,
                                 VWindow *parent,
                                 VClass *cls = 0,
                                 int *ww = 0,
                                 int *wh = 0,
                                 boolean fSetTitle = TRUE);
    boolean         loadContents(const VString&, VWindow *, VClass *, VWindow *);
    VMenu          *loadMenu(const VString& wname, VWindow* parent);
    VPopupMenu     *loadPopupMenu(const VString& wname,VWindow* parent);
    VBitMap        *loadBitMap(const VString& wname);
    const VString&  getFileName() const;
    boolean         realizeWindow(const VString& wname,
                                  VWindow* window,
                                  VClass* cls,
                                  VWindow* parent);

protected:
//  boolean         loadHelpTable(const VString& wname, VWindow* window);

private:
    VString         m_vrfName;      // resource file name
    VRscArchiver    m_vrf;          // resource file handle
#if defined(CV_HTML)
	VFile			htmlFile;
	boolean			isHtml;
#endif

};

// ---------------------------------------------------------------------------
//
inline const VString& VResource::getFileName() const
{
    return m_vrfName;
}

#endif // V_RSC_H
