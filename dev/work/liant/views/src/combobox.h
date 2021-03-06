// $Header:   Q:/views/win/vcs/combobox.h_v   1.20   Aug 06 1997 14:04:54   DAVIDMI  $ 

//  combobox.h
//
//  VComboBox class interface
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
//  05/25/93 jmd    preened, added getEditHeight, removed free()
//  06/21/93 pmcm   added support for VArchiver;
//                  added constructor with name argument;
//                  added virtual VString *getString(int i) for realize();
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/25/93 dgm    added get/setStyle(), is/setAutoSort(),
//                  showDropDown(), isDroppedDown(), and getDropDownHeight().
//  11/10/93 pkt    made selectedString const.
//  01/12/94 jmd    made getStringAt public
//  01/31/94 pkt    made count and selectedItem const.
//  08/11/94 jld    added textLimit, setLimit(), and getLimit() members.
//  03/16/95 jld    added const to args of insertString()
//  04/11/95 pkt    made some parameters const.
//  06/01/95 pkt    merged international changes into source control.
//  08/10/95 dgm    Changed findPrefix() to findString().
//  09/01/95 dss    VStyle changes.
//  01/29/96 dgm    Added getSystemBorderSize().
//  02/08/96 evc    Added getComboLboxHwnd() and setComboLBoxHwnd(HWND) to
//                  track the "comboLBox" so we can delete it in VComboBox
//	04/19/96 glm	removed v1.1 code/preened
//	08/04/97 dgm	Added setDropDownLineHeight(), getDropDownLineHeight().
// ---------------------------------------------------------------------------

#ifndef VCOMBOBOX_H
#define VCOMBOBOX_H

#include "listbox.h"

#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VString;
#endif

extern VClass *VComboBoxCls;

// NOTE: obsolete
#define StaticDropDn    StyleComboDrop
#define EditDropDn  StyleComboEditDrop
#define EditList        StyleComboEdit

// NOTE: obsolete
typedef const VStyle& comboStyle;

//  Introduction
//
//  The VComboBox is a subclass of `VListBox` that operates in
//  the same way except that, in addition to the list box, it provides a
//  text control similar to that of an `VEditLine` or `Textbox` object.  There
//  are three styles of VComboBox objects that can be specified in the
//  constructor:  `StyleComboDrop` (formerly `StaticDropDn`) that provides a
//  selection `VListBox` that drops down off of a 'static' `VTextBox` object; 
//  `StyleComboEditDrop` (formerly `EditDropDn`) that provides a selection
//  list that drops down off of an `VEditLine` object; and `StyleComboEdit`
//  (formerly `EditList`) that provides a selection `VListBox`
//  attached to a `VEditLine` object that is always visible.

CLASS VComboBox : public VListBox {

public:
            VComboBox();
            VComboBox(  unsigned cntrlId, VWindow *pwin);
            VComboBox(  const VFrame& frame, 
                        VWindow *pwin, 
                        const VStyle& style = StyleBorder);

            VComboBox(  const VString& wname, 
                        const VFrame& frame, 
                        VWindow *pwin, 
                        const VStyle& style = StyleBorder);

            ~VComboBox();

    VClass *iam();

    // Operations on Lists

    void    clearList();
    int     count()const ;
    void    deleteStringAt(int i);
    void    insertString(const char *s, int i);
    void    insertString(const char *s);
    int     findString(const char *str, int start = -1);
    virtual int findPrefix(const char *str, int start = -1);

    virtual VString    *getStringAt(int i);     

    // Loading Directory Contents

    //  (use `filter` to match filename entries in current directory
    //  `listwhat` is one of the following: 
    //  ListAll, listFiles, listDrives, or listSubDirectories)

    int     dir(VString *filter, unsigned listwhat);

    // Selection

    VString *selectedString() const;
    int     selectedItem() const;
    void    selectItem(integer i);

    // Events

    void        uponChange(VObject *obj,    
                           method mthd);

    boolean erased();
    boolean paint();

    int			getEditHeight();
    int			getDropDownLineHeight() const;
    int			getDropDownLineItemHeight() const;
    int			getDropDownHeight();
    void		setDropDownLineHeight(int);
    void		setDropDownHeight(int);

    void    showDropDown(boolean);
    boolean isDroppedDown() const;

    void        setLimit(unsigned n);
    unsigned    getLimit() const;
    const VStyle&   getStyle() const;
    void        setStyle(const VStyle& style);
    boolean isAutoSort() const;
    void        setAutoSort(boolean);

#ifndef CV_NOARCHIVER
    //  Archival and Retrieval

    void            putTo(VArchiver& a);
    void            getFrom(VArchiver& a);
    void            getFrom(VArchiver& a, VObject *pwin,
                            boolean do_realize = TRUE);
    virtual void    realize(VWindow *pwin);
    integer             getChangeMethodIndex() const;
    void                setChangeMethodIndex(integer);
    integer             getChangeClientIndex() const;
    void                setChangeClientIndex(integer);


#endif

    virtual void    setGeometry(int, int, int, int);
    virtual void    setGeometrySize(int, int);

    // return the handle of the comboLBox for a 
    // combobox. Its owned by the desktop.
    HWND getComboLboxHwnd();
    void setComboLBoxHwnd(HWND);

protected:
    boolean notify(int code);
    void    getInfo(VWinInfo& comboinfo);
    void    getMinSize(short& w, short& h);
    UINT    getCtlColor();

    virtual void    getPreferredSize(int *w, int *h) const;
    virtual void    getSystemBorderSize(int *, int *) const;

    virtual void	destroyed();
	void			destroyDropDown();

    VObject *changeClient;
    method  changeMethod;

#ifndef CV_NOARCHIVER
    void                putTo(VArchiver& a, boolean most_derived);
    integer             changeClientIndex;
    integer             changeMethodIndex;
#endif

private:
    boolean droppedDown;
    unsigned textLimit;
    HWND combolbox_hwnd; // handle of the dropdown list owned by the desktop

};

// ---------------------------------------------------------------------------
inline unsigned VComboBox::getLimit() const
{
    return textLimit;
}
// ---------------------------------------------------------------------------
inline boolean VComboBox::isAutoSort() const
{
    return style.contains(StyleSorted);
}
// ---------------------------------------------------------------------------
inline void VComboBox::setAutoSort(boolean b)
{
    if (b) {
        style += StyleSorted;
    }
    else {
        style -= StyleSorted;
    }
}
// ---------------------------------------------------------------------------
inline  HWND VComboBox::getComboLboxHwnd(){
    return combolbox_hwnd;
}
// ---------------------------------------------------------------------------
inline void VComboBox::setComboLBoxHwnd(HWND hwnd){
    combolbox_hwnd = hwnd;
}

//============================================================================
#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
inline integer VComboBox::getChangeClientIndex() const
{
    return changeClientIndex;
}

// ---------------------------------------------------------------------------
inline void VComboBox::setChangeClientIndex(integer index)
{
    changeClientIndex = index;
}
// ---------------------------------------------------------------------------
inline integer VComboBox::getChangeMethodIndex() const
{
    return changeMethodIndex;
}

// ---------------------------------------------------------------------------
inline void VComboBox::setChangeMethodIndex(integer index)
{
    changeMethodIndex = index;
}
// ---------------------------------------------------------------------------

#endif
//============================================================================

#endif // VCOMBOBOX_H
