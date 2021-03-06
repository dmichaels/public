// $Header:   Q:/views/win/vcs/help.h_v   1.9   Mar 04 1997 16:32:26   gregm  $

//  help.h
//
//  VHelp class interface
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  ---------------------------------------------------------------------------

#ifndef help_h
#define help_h
#include "window.h"
#include "str.h"

#define USECONTEXTPOPUP 0x80000000

extern VClass *VHelpCls;

CLASS VHelp : public VWindow {

protected:
    VString     _file_name;
    VWindow*    _had_focus;
    boolean     _pop_it_up;
    boolean     _ask_parent;
    void        destroyed();

public:

    VHelp();
    ~VHelp();
    VClass *iam();

    VHelp(VWindow*pwin, const VString&fname, boolean askParent = FALSE, boolean popupStyle = FALSE);

    void        setPopupStyle(boolean value);
    boolean     getPopupStyle();

    void        setAskParent(boolean value);
    boolean     getAskParent();

    boolean     search(const VString&key_word, boolean exact = FALSE);
    boolean     topicFromMouse();
    boolean     topicFromFocus();
    boolean     topic(long topic_id);
    boolean     topic(long topic_id, boolean popupStyle);
    boolean     contents();
    boolean     index();
    boolean     close();   
    boolean     general() { return contents(); };

    boolean     captureMouse();
    boolean     mouseUp(int x,int y);
    boolean     key(int key, char ch);

};

inline void VHelp::setPopupStyle( boolean value )
{
    _pop_it_up = value;
}

inline boolean VHelp::getPopupStyle()
{
    return _pop_it_up;
}

inline void VHelp::setAskParent( boolean value )
{
    _ask_parent = value;
}

inline boolean VHelp::getAskParent()
{
    return _ask_parent;
}

#endif // help_h
