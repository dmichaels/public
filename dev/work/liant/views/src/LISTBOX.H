// $Header:   Q:/views/win/vcs/listbox.h_v   1.16   Mar 04 1997 16:32:32   gregm  $ 

//  listbox.h
//
//  VListBox class interface
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
//  04/09/93 pat    changed id to VObject *
//  05/13/93 pat    preened;
//                  added constructor that takes name argument;
//                  added getFrom(), putTo(), realize(); 
//                  added reference to VArchiver.
//  05/13/93 pat    added destructor (virtual)
//  05/25/93 jmd    preened, removed free()
//  06/21/93 pat    added for realize(): 
//                      VOrdCollect *listOStrings, int iSelect, 
//                      virtual VString *getString(int i), and
//                      VOrdCollect *getList();         
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 pat    added selectMthdIndex, and dblClkMthdIndex.
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/25/93 dgm    added get/setSingleClickMethodIndex(),
//                  get/setSingleClickMethodIndex(),
//                  get/setPassIndexToCallbacks().
//  09/01/93 dgm    M8 changes; cast return value of
//                  getSingleClickMethodIndex() & getDoubleClickMethodIndex().
//  09/26/93 jmd    fixed ARCHIVER ifdef
//  10/11/93 jmd    fixed IndexToCallbacks logic
//  11/10/93 pkt    made count const.
//  01/12/94 jmd    made getStringAt public
//  01/31/94 pkt    made selectedString and selectedItem const.
//  08/31/94 dss    Added getCtlColor().
//  09/08/94 jld    Added replaceStringAt().
//  04/11/95 pkt    made some parameters const.
//  06/01/95 pkt    merged international changes for source control.
//  06/24/95 jld    added const declarations
//  06/24/95 jld    added findPrefix() and selectPrefix()
//  07/18/95 dgm    Miscellaneous const char * changes.
//  08/10/95 dgm    Changed findPrefix() to findString().
//  09/01/95 dss    VStyle changes.
//  12/15/95 evc    Added horizontal scroll bar support
//  01/29/95 evc    Improved horizontal scroll bar support.
//  02/01/95 dgm    Added getPreferredSize().
// ---------------------------------------------------------------------------

#ifndef LISTBOX_H
#define LISTBOX_H

#include "control.h"

CLASS VString;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
CLASS VOrdCollect;  //  !!! pat
#endif

extern VClass *VListBoxCls;

// Allegris Directory selection defines used in dir method

#define listFiles           0x01
#define listDrives          0x02
#define listSubDirectories  0x04
#define listAll             0x07

// DOS File Attributes

#define NormalFilesList     0x0
#define ExclusiveList       0x8000
#define DriveList           0x4000
#define SubDirectoryList    0x10

// Introduction
//
// The `VListbox` maintains and displays a list of text strings. 
// The list can be scrolled up and down by the user via a vertical
// scrollbar. ` Listbox` objects have two callbacks: one for when the
// user clicks on an item in the list; and a second for when the
// user double clicks on an item in the list.  Both callback methods
// pass the index of the selected item to the client object.

CLASS VListBox : public VControl {

public:

    // Constructors

                        VListBox();
                        VListBox(   unsigned cntrlId, VWindow *pwin);

                        VListBox(   const VFrame& frame, 
                                    VWindow    *pwin, 
                                    const VStyle& style = StyleBorder);

                        VListBox(   const VString& wname, 
                                    const VFrame& frame, 
                                    VWindow    *pwin, 
                                    const VStyle& style = StyleBorder);

                        ~VListBox();

    VClass             *iam();

    // Callback Methods

    void                uponClick(  VObject    *clnt, 
                                    method      mthd, 
                                    method      dblmthd, 
                                    boolean     returnList = FALSE);

    void                uponClick(  VObject    *clnt,
                                    method      mthd,
                                    boolean     returnList = FALSE);

    void                uponDblClick(VObject    *clnt,
                                    method      mthd,
                                    boolean     returnList = FALSE);

    void            setFont(VFont *f);
    // Operations on List

    void                appendList(const char **s);
    void                insertList(const char **s, int i);
    void                insertList(const char **s);
    boolean             appendString(const char *s);
    virtual void        insertString(const char *s, int i);
    virtual void        insertString(const char *s);
    virtual void        clearList();
    virtual int         count() const;
    virtual void        deleteStringAt(int i);
    virtual void        replaceStringAt(int i, const char *s);
    int                 findString(const char *s, int idx = -1);
    virtual int         findPrefix(const char *s, int idx = -1);
    virtual int         selectPrefix(const char *s, int idx = -1);

    virtual VString    *getStringAt(int i); 


    // Loading Directory Contents

    virtual int         dir(VString *filter, unsigned listEnumerator);

    // Selection

    virtual int         selectedItem() const;
    virtual VString    *selectedString() const;
    virtual void        selectItem(integer i);

    void                refresh(boolean b);         // List Update
    void                sizeOf(int *w, int *h);     // Control Size
    boolean             resized(int w, int h);      // Event Handling
    enum WinType        winType();                  // Window Type

    // Geometry

    virtual void        getPreferredSize(int *, int *) const;

    // Archival and Retrieval

#ifndef CV_NOARCHIVER
    void                putTo(VArchiver& a);
    void                getFrom(VArchiver& a);
    void                getFrom(VArchiver& a, VObject *pwin,
                                boolean do_realize = TRUE);
    virtual void        realize(VWindow *pwin);
    integer             getSingleClickMethodIndex() const;
    void                setSingleClickMethodIndex(integer);
    integer             getDoubleClickMethodIndex() const;
    void                setDoubleClickMethodIndex(integer);
    boolean             getPassIndexToCallbacks() const;
    void                setPassIndexToCallbacks(boolean);
    integer             getDblClkClientIndex() const;
    void                setDblClkClientIndex(integer);


#endif


protected:


    void                getInfo(VWinInfo& listinfo);
    UINT                getCtlColor();
    void                getMinSize(short& w, short& h);
    boolean             notify(int code);
    void                init(const char *title);

    method              selectMthd;     // single click callback
    VObject*            dblClkClient;
    method              dblClkMthd;     // double click callback
    boolean             returnThis;     // TRUE,return this upon click; 
                                        // FALSE,return index of selecteditem
    boolean             dblClkReturnThis; // TRUE,return this upon click; 
                                        // FALSE,return index of selecteditem


#ifndef CV_NOARCHIVER
    integer             dblClkClientIndex;
    void                putTo(VArchiver& a, boolean most_derived);
    VOrdCollect        *getList();          
#endif

private:
// used to track horizontal extent

int                 calculateExtent(const char * );
// calculates the extent of a string if it were inserted into this listbox

void                setHorizontalExtent(int);
// sets the horizontal extent in pixels

int                 getHorizontalExtent();
// gets the horizontal extent in pixels thats set for the listbox

void                refreshHorizontalScrollBars();
// calculates the proper horizontal extent using the longest
// extent in the listbox to calculate it.


#ifndef CV_NOARCHIVER
    integer             selectMthdIndex;
    integer             dblClkMthdIndex;
    VOrdCollect        *listOStrings;       
    int                 iSelect;            
#endif


};

#ifndef CV_NOARCHIVER

inline integer VListBox::getSingleClickMethodIndex() const
{
    return selectMthdIndex;
}

inline void VListBox::setSingleClickMethodIndex(integer index)
{
    selectMthdIndex = index;
}

inline integer VListBox::getDoubleClickMethodIndex() const
{
    return dblClkMthdIndex;
}

inline void VListBox::setDoubleClickMethodIndex(integer index)
{
    dblClkMthdIndex = index;
}

inline boolean VListBox::getPassIndexToCallbacks() const
{
    return !returnThis;
}

inline void VListBox::setPassIndexToCallbacks(boolean b)
{
    returnThis = !b;
}
// ---------------------------------------------------------------------------
inline integer VListBox::getDblClkClientIndex() const
{
    return dblClkClientIndex;
}

// ---------------------------------------------------------------------------
inline void VListBox::setDblClkClientIndex(integer index)
{
    dblClkClientIndex = index;
}


#endif // ARCHIVER

#endif // LISTBOX_H
