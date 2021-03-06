// $Header:   Q:/views/win/vcs/editbox.h_v   1.20   Mar 04 1997 16:32:18   gregm  $ 

//  editbox.h
//
//  VEditBox class interface
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
//	07/15/92 jmd	preened
//	04/09/93 pmcm	changed id to VObject *
//	05/14/93 pmcm	added destructor (virtual)
//	06/16/93 pmcm	preened, added constructor that takes name argument;
//					added getFrom(), putTo(), realize();
//					added reference to VArchiver;
//					added private nCharsPerTab and textLimit for archiving.
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	removed free()
//					added outOfSpaceMthdIndex, changedIndex.
//	08/09/93 jmd	made VFrame const
//	08/16/93 jmd	made name const VString
//	08/20/93 dgm	added get/setChangeMethodIndex(),
//					get/setNoSpaceMethodIndex(), tabsEnabled(),
//					getTabs(), getLimit(), hasVScroll(), hasHScroll(),
//					addVScroll(boolean), addHScroll(boolean),
//					setReadOnly(boolean), and isReadOnly().
//	09/01/93 dgm	M8 changes; cast return values.
//	09/26/93 jmd	fixed ARCHIVER ifdef
//	09/26/93 jmd	added canUndo
//	02/18/94 pmcm	added private hInstance member for VNotifier use
//	02/26/94 jmd	removed private hInstance member for VNotifier use
//	08/31/94 dss	Added getCtlColor().
//	10/24/94 jld	Added setSet, getSel, and getSelBegin
//	07/18/95 dgm	Miscellaneous const char * changes.
//	09/01/95 dss	VStyle changes.
//	02/09/96 dgm	Added getPreferredSize().
// ---------------------------------------------------------------------------

#ifndef EDITBOX_H
#define EDITBOX_H

#include "textbox.h"

CLASS VPopupMenu;
CLASS VMenuItem;
CLASS VFont;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

extern VClass *VEditBoxCls;

//  Introduction
//
//  The `VEditBox` class defines text which the user can modify. 
//  An `VEditBox` object may have scrollbars to scroll its contents left
//  and right, or up and down.  Its contents can be aligned to the left, 
//  right or center.  An `VEditBox` object supports clipboard cutting and
//  pasting as well as a single level undo. It can also return a ready-made
//  `VPopupMenu` object to support all editing operations.

CLASS VEditBox : public VTextBox {

public:

                VEditBox();

                VEditBox(   unsigned cntrlId, VWindow *pwin);

                VEditBox(   const VString& wname,
                            const VFrame& frame, 
                            VWindow *pwin, 
                            const VStyle& style = StyleDefault);

                VEditBox(   const VFrame& frame, 
                            VWindow *pwin, 
                            const VStyle&  style = StyleDefault);


                ~VEditBox();

    VClass     *iam();

    //  Establish callbacks for when out of memory and when contents change
    
    void        uponEvent(VObject *clnt, method outOfSpace, method changed = 0);
    void        uponChange(VObject *clnt, method changed);
    void        uponNoSpace(VObject *clnt, method outOfSpace);

    void        enableTabs(boolean enable);
    boolean     tabsEnabled() const;
    void        setChanged(boolean b);
    void        clearChanged();
    boolean     changed();
    void        setTabs(int n);
    int         getTabs() const;
    void        setLimit(unsigned n);
    unsigned    getLimit() const;

    void        setSel(int begin, int  end);
    void        getSelBegin(int *begin, int *end);  // delete next release
    void        getSel(int *begin, int *end);
    boolean     clearSel(VMenuItem *m);
    boolean     copySel(VMenuItem *m);
    boolean     cutSel(VMenuItem *m);
    boolean     paste(VMenuItem *m);
    boolean     selectAll(VMenuItem *m);
    boolean     undo(VMenuItem *m);
    boolean     canUndo() const;

    void		setFont(VFont*);
    //  Getting and Setting Text

    int         sizeOfText(); 
    int         getText(VString& s);
    int         getText(char *s, unsigned len); 
    VString		getText();

    VEditBox&   operator= (const char *s);

    //  Edit

    VPopupMenu *stdEditMenu();

    //  Event Handling

    boolean     enterHit();
    boolean     key(int key, char ch);
    boolean     resized(int w, int h);
	boolean 	takeFocus();
	boolean		nextFocus();
	boolean		controlNextFocus();
	boolean		controlPrevFocus();

    //  Geometry

    virtual void    getPreferredSize(int *w, int *h) const;

#ifndef CV_NOARCHIVER
    //  Archival and Retrieval

    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
    void            getFrom(VArchiver& a, VObject *pwin,
                            boolean do_realize = TRUE);
    virtual void    realize(VWindow *pwin);

    integer         getChangeMethodIndex() const;
    void            setChangeMethodIndex(integer);

    integer         getNoSpaceMethodIndex() const;
    void            setNoSpaceMethodIndex(integer);

    integer         getChangedClientIndex() const;
    void            setChangedClientIndex(integer);

#endif

    void        addVScroll(boolean);
    void        addHScroll(boolean);
    boolean     hasVScroll() const;
    boolean     hasHScroll() const;
    void        setReadOnly(boolean);
    boolean     isReadOnly() const;

    void	    enableBool(boolean);

protected:

    void        getInfo(VWinInfo& editinfo);
    UINT        getCtlColor();
    void        getMinSize(short& w, short& h);
    boolean     notify(int);

protected:

#ifndef CV_NOARCHIVER
    void        putTo(VArchiver& a, boolean most_derived);
#endif

    VObject*    changedClient;  //  separate clients for each callback
    method      changedMthd;    //  callback when contents change
    method      outOfSpaceMthd; //  callback when out of memory
    boolean     useTabs;        //  TRUE for edit; FALSE for focus transfer

private:

    int         nCharsPerTab;       //  n chars per tab 
    unsigned    textLimit;          //  text content maximum of n chars 

#ifndef CV_NOARCHIVER
    integer     changedClientIndex;
    integer     changedMthdIndex;
    integer     outOfSpaceMthdIndex;
#endif

};

// ---------------------------------------------------------------------------
//
inline void VEditBox::enableTabs(boolean enable) 
{ 
    useTabs = enable; 
}

// ---------------------------------------------------------------------------
//
inline VEditBox& VEditBox::operator= (const char *s) 
{ 
    putText(s); 
    return *this; 
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
inline integer VEditBox::getChangeMethodIndex() const
{
    return changedMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VEditBox::setChangeMethodIndex(integer index)
{
    changedMthdIndex = index;
}

// ---------------------------------------------------------------------------
inline integer VEditBox::getNoSpaceMethodIndex() const
{
    return outOfSpaceMthdIndex;
}

// ---------------------------------------------------------------------------
inline void VEditBox::setNoSpaceMethodIndex(integer index)
{
    outOfSpaceMthdIndex = index;
}

// ---------------------------------------------------------------------------
inline integer VEditBox::getChangedClientIndex() const
{
    return changedClientIndex;
}

// ---------------------------------------------------------------------------
inline void VEditBox::setChangedClientIndex(integer index)
{
    changedClientIndex = index;
}
#endif // ARCHIVER

// ---------------------------------------------------------------------------
inline boolean VEditBox::tabsEnabled() const
{
    return useTabs;
}

// ---------------------------------------------------------------------------
inline int VEditBox::getTabs() const
{
    return nCharsPerTab;
}

// ---------------------------------------------------------------------------
//
inline void VEditBox::getSelBegin(int *begin, int *end) 
{ 
    getSel(begin, end);
}

// ---------------------------------------------------------------------------
inline unsigned VEditBox::getLimit() const
{
    return textLimit;
}

// ---------------------------------------------------------------------------
inline VString VEditBox::getText()
{
	return VControl::getText();
}

#endif // EDITBOX_H 
