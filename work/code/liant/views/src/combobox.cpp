// $Header:   Q:/views/win/vcs/combobox.cpv   1.28   Mar 04 1997 17:54:50   gregm  $

//  combobox.cpp
//
//  VComboBox implementation
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
//  05/25/93 jmd    preened, added getEditHeight, removed free (added shadow)
//  06/21/93 pat    added support for VArchiver.
//                  added constructor with name arg.
//                  added virtual VString *getString(int i) for realize();
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//                  Also made realize() virtual.
//  07/27/93 dgm    pass final argument to getChildrenFrom().
//  08/09/93 jmd    made VFrame const
//  08/16/93 jmd    made name const VString
//  08/25/93 dgm    added get/setStyle(), setDropDown(), and isDroppedDown().
//  09/20/93 dgm    Fixed painting problem.
//  09/22/93 pat    added archive tag
//  01/12/94 jmd    made getStringAt public
//  01/27/94 dgm    changed return type for SendMessage() to LRESULT in
//                  getStringAt().
//  02/02/94 pat    added archive tag check
//  02/04/94 dgm    minor Constructor related fix in getDropDownHeight()
//  02/09/94 pat    updated defineArchiveTag use
//  03/05/94 pat    STRICT:
//                  cast to HGDIOBJ for DeleteObject();
//                  changed HANDLE to HBRUSH;
//  03/26/94 pat    CV_WIN32: added explicit int cast on RECT long members.
//  03/27/94 pat    CV_WIN32: cast appropriately for SendMessage().
//  08/11/94 jld    added textLimit, setLimit(), and getLimit() members.
//  11/03/94 dgm    backed out new revision for saving.
//  11/11/94 dss    Archiver tags macros.
//  03/16/95 jld    erased() uses clipping frame instead of background RECT
//  03/16/95 jld    removed  WS_EX_NOPARENTNOTIFY from comboinfo.style
//  03/16/95 jld    added const to args of insertString()
//  03/16/95 jld    changed notify() to check 'returnThis' 
//                  before performing callback
//  03/16/95 jld    changed isDroppedDown() to test for StyleComboEdit 
//                  instead of StyleComboEdit
//  06/01/95 pkt    made 'count', 'selectedString', and 'selectedItem' const.
//  06/24/95 jld    made some parameters const
//  06/24/95 jld    added findPrefix() member function
//  08/10/95 dgm    Changed findPrefix() to findString().
//  09/01/95 dss    VStyle changes.
//  09/28/95 dss    setStyle: bug fix: was testing the arg instead of the obj.
//  11/21/95 dgm    Changed getInfo() to give a 3D border for CV_WIN32.
//  01/29/96 dgm    Changed getEditHeight() to return total edit-line height.
//  02/08/96 evc    added DestroyWindow(combolbox_hwnd) to destructor
//  04/19/96 glm    removed v1.1 code/preened
//  09/18/96 dgm    Fixed getPreferredSize().
//  09/18/96 dgm    Fixed memory leak in getPreferredSize().
//  10/14/96 dgm    Fixed resource leak (ComboLBox) in destructor.
//	04/03/97 dgm	Call VWindow::checkLosingFocus() before takeFocus().
//	07/24/97 dgm	Fix for blatant bug/typo in setGeometry()
//					and setGeometrySize() related height to width.
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "combobox.h"
#include "brush.h"
#include "port.h"       //!!!jmd
#include "str.h"
#include "notifier.h"
#include "cllbckls.h"

#ifndef CV_NOARCHIVER
#include "archiver.h"
#endif

defineClass(VComboBox, VListBox)
//
// revision 0   original
// revision 1   added textLimit
//
defineArchiveRevision(VComboBox, 2)

// ---------------------------------------------------------------------------
// Construct a VComboBox with no presentation window.
//
VComboBox::VComboBox()
{
    combolbox_hwnd = (HWND) 0;
    selectMthd = dblClkMthd = 0;

    changeMethod = 0;
    changeClient = 0;
#ifndef CV_NOARCHIVER
    changeMethodIndex = -1L;
    changeClientIndex = -1L;
#endif

    droppedDown = FALSE;
    textLimit = 0;
}

// ---------------------------------------------------------------------------
// Construct a VComboBox from the resource control-id 'cntrl' of the
// resource associated with 'pWin'. This is a non-portable GUI system method.
//
VComboBox::VComboBox(unsigned cntrlId, VWindow *pWin) 
{
    combolbox_hwnd = (HWND) 0;
    selectMthd = dblClkMthd = 0;

    changeMethod = 0;
    changeClient = 0;
#ifndef CV_NOARCHIVER
    changeMethodIndex = -1L;
    changeClientIndex = -1L;
#endif

    droppedDown = FALSE;
    textLimit = 0;
    getRscWindow(cntrlId, pWin);
}

// ---------------------------------------------------------------------------
VComboBox::~VComboBox()
{
	destroyDropDown();
}

// ---------------------------------------------------------------------------
//
void VComboBox::destroyDropDown()
{
    if (hwnd() != 0) {
		if (combolbox_hwnd == 0) {
			if (!IsWindowEnabled(hwnd())) {
	        	EnableWindow(hwnd(), TRUE);
			}
			MoveWindow(hWnd, 0, 0, 10, 40, TRUE);
			SendMessage(hwnd(), CB_SHOWDROPDOWN, WPARAM(TRUE), LPARAM(0));
		}
    }
    if (combolbox_hwnd != 0) {
        DestroyWindow(combolbox_hwnd);
        combolbox_hwnd = 0;
    }
	else {
       showDropDown(FALSE);
	}
}

// ---------------------------------------------------------------------------
// Construct a VComboBox with frame 'frame', parent 'win' and style 'style'. 
// The 'style' parameter can be one of  `StyleComboDrop`, `StyleComboEditDrop`
// or `StyleComboEdit` combined with `StyleSorted` or `StyleBorder` (default).
//
VComboBox::VComboBox(const VFrame& f, VWindow *win, const VStyle& style) 
{
    combolbox_hwnd = (HWND) 0;
    selectMthd = dblClkMthd = 0;

    changeMethod = 0;
    changeClient = 0;
#ifndef CV_NOARCHIVER
    changeMethodIndex = -1L;
    changeClientIndex = -1L;
#endif

    droppedDown = FALSE;
    textLimit = 0;
    (*this)(f, win, style);
}

// ---------------------------------------------------------------------------
// Construct a named VComboBox with frame 'frame', parent 'win' and 
// style 'style'.  The 'style' parameter can be one of  `StyleComboDrop`,
// `StyleComboEditDrop`, or `StyleComboEdit` combined with `StyleSorted`
// or `StyleBorder` (default).
//
VComboBox::VComboBox(const VString& wname, const VFrame& f, 
                        VWindow *win, const VStyle& style) 
{
    combolbox_hwnd = (HWND) 0;
    selectMthd = dblClkMthd = 0;

    changeMethod = 0;
    changeClient = 0;
#ifndef CV_NOARCHIVER
    changeMethodIndex = -1L;
    changeClientIndex = -1L;
#endif

    droppedDown = FALSE;
    textLimit = 0;
    (*this)(f, win, style, wname);
}

// ---------------------------------------------------------------------------
// Clear the list of this VComboBox.
//
void VComboBox::clearList()
{
    SendMessage(hWnd, CB_RESETCONTENT, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the number of items in the list of this VComboBox.
//
int VComboBox::count() const
{
    return (int) SendMessage(hWnd, CB_GETCOUNT, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Remove the 'n'th list item from this VComboBox.
//
void VComboBox::deleteStringAt(int n)
{
    SendMessage(hWnd, CB_DELETESTRING, (WPARAM) n, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Fill this `VListBox` from the current directory with the file names, 
// that match the C filter string 'str'.  To obtain file names, subdirectories
// and/or drives, use one or more of the `listAll`, `listFiles` (uses the filter
// string 'str'), `listDrives`, or `listSubDirectories` types as the parameter
// `e`.  These can be or\'d together to select combinations (`listAll` is
// equivalent to `listFiles`|`listDrives`|`listSubDirectories`).  Return the
// number of items placed in this `VListBox`.  Note: Passing a NIL for 'str'
// will result in the string '*.*' being used as the filter.
//
int VComboBox::dir(VString *str, unsigned e)
{
    int      i;
    unsigned a = 0;
    char     filter[40];

    if (!str) {
        lstrcpy(filter, "*.*");
    }
    else {
        str->gets(filter, 39);
    }
    clearList();
    if (e & listFiles) {
        i = (int) SendMessage(hWnd, CB_DIR, (WPARAM) 0, (LPARAM) filter);
    }
    a |= ExclusiveList;
    if (e & listDrives) {
        a |= DriveList;
    }
    if (e & listSubDirectories) {
        a |= SubDirectoryList;
    }

    if ((e & listDrives) || (e & listSubDirectories)) {
        i += (int) SendMessage(hWnd, CB_DIR, (WPARAM) a, (LPARAM)(LPSTR)  "*.*");
    }
    return i;
}

// ---------------------------------------------------------------------------
// Insert C string 'str' in the the list of this VComboBox in
// alphabetical order.
//
void VComboBox::insertString(const char *str)
{
    SendMessage(hWnd, CB_ADDSTRING, (WPARAM) 0, (LPARAM) str);
	updateGeometry();
}

// ---------------------------------------------------------------------------
// Insert C string 'str' into the the list of this VComboBox at
// index 'idx'.  If the 'idx' is a -1, then append the string to the
// end of the list.
//
void VComboBox::insertString(const char *str, int idx)
{
    SendMessage(hWnd, CB_INSERTSTRING, (WPARAM)idx, (LPARAM) str);
	updateGeometry();
}

// ---------------------------------------------------------------------------
//
int VComboBox::findString(const char* str, int start)
{
    return findPrefix(str, start);
}

// ---------------------------------------------------------------------------
// Return the first index of the first item in this VComboBox matching the given
// prefix. Matching begins with the item whose index is 'start'. If no match is
// found, -1 is returned.
//
int VComboBox::findPrefix(const char* str, int start)
{
    LRESULT idx = SendMessage( hWnd, CB_FINDSTRING, (WPARAM) start--, (LPARAM) str );
    return (idx == CB_ERR) ? -1 : (int) idx;
}

// ---------------------------------------------------------------------------
// Select the item at the index 'idx' of this VComboBox.
//
void VComboBox::selectItem(integer idx)
{
    SendMessage(hWnd, CB_SETCURSEL, (WPARAM) idx, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the index of the currently selected item of this VComboBox.
//
int VComboBox::selectedItem() const
{
    return (int) SendMessage(hWnd, CB_GETCURSEL, (WPARAM) 0, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
//
int VComboBox::getDropDownLineItemHeight() const
{
    return SendMessage(((VComboBox *)this)->hwnd(),
					   CB_GETITEMHEIGHT, WPARAM(0), LPARAM(0));
}

// ---------------------------------------------------------------------------
//
int VComboBox::getDropDownHeight()
{
    RECT r;
    SendMessage(hWnd, CB_GETDROPPEDCONTROLRECT, (WPARAM) 0, (LPARAM) &r);
    return int(r.bottom - r.top - getEditHeight());
}

// ---------------------------------------------------------------------------
//
int VComboBox::getDropDownLineHeight() const
{
    if (!style.contains(StyleComboEdit) && (hWnd != 0)) {
		int ddh = ((VComboBox *)this)->getDropDownHeight();
    	int ddih = getDropDownLineItemHeight();
		return ddh / ddih;
	}
	return 0;
}

// ---------------------------------------------------------------------------
//
void VComboBox::setDropDownLineHeight(int nlines)
{
    if (!style.contains(StyleComboEdit) && (hWnd != 0)) {
    	int ddih = getDropDownLineItemHeight();
		int current_nlines = count();
		if (nlines > current_nlines) {
			nlines = current_nlines;
		}
		setDropDownHeight(nlines * ddih + 2);
	}
}

// ---------------------------------------------------------------------------
//
void VComboBox::setDropDownHeight(int h)
{
    if (!style.contains(StyleComboEdit) && (hWnd != 0)) {
		RECT r;
		GetWindowRect(hWnd, LPRECT(&r));
		int ddih = getDropDownLineItemHeight();
		if (h < ddih + 2) {
			h = ddih + 2;
		}
		VListBox::setGeometrySize(r.right - r.left, h + getEditHeight());
	}
}

// ---------------------------------------------------------------------------
//
int VComboBox::getEditHeight()
{
    int h = SendMessage(hWnd, CB_GETITEMHEIGHT, WPARAM(-1), LPARAM(0));
    int system_border_h;
    getSystemBorderSize(0, &system_border_h);
    return h + system_border_h;
}

// ---------------------------------------------------------------------------
//
void VComboBox::getPreferredSize(int *width, int *height) const
{

    if (width != 0) {
		queryPreferredSize(width, 0, FALSE, TRUE);
		int n = count();
		if (n > 0) {
			int max = *width;
			while (--n >= 0) {
				VString *vs = ((VComboBox *)this)->getStringAt(n);
				if (vs != 0) {
						const char *s = vs->gets();
					if ((s != 0) && (*s != '\0')) {
			        	queryPreferredSize
							(width, 0, FALSE, TRUE, FALSE, 0, s);
						if (*width > max) {
							max = *width;
						}
					}
					delete vs;
				}
			}
			if (max == 0) {
				queryPreferredSize(width, 0, FALSE, TRUE);
			}
			else {
				*width = max;
			}
		}
    	if (style.contains(StyleComboDrop) ||
			style.contains(StyleComboEditDrop)) {
			static int thumb_width = -1;
			if (thumb_width < 0) {
#				if defined(CV_WIN32) && defined(SPI_GETNONCLIENTMETRICS)
					NONCLIENTMETRICS ncm; ncm.cbSize = sizeof(ncm);
					SystemParametersInfo(SPI_GETNONCLIENTMETRICS, 0, &ncm, 0);
					thumb_width = ncm.iScrollWidth;
#				else
					thumb_width = 14;
#				endif
			}
			*width += thumb_width + 4;
		}
    }

    if (height != 0) {
        if (hWnd != 0) {
            *height = SendMessage(hWnd, CB_GETITEMHEIGHT,
                                        WPARAM(-1), LPARAM(0));
        }
        else {
            *height = 16;
        }
    }
}

// --------------------------------------------------------------------------
//
void VComboBox::getSystemBorderSize(int *width, int *height) const
{
    int w, h;
    if (has3dEffects(V_WIN3D_EFFECTS)) {
        w = h = GetSystemMetrics(SM_CYEDGE) * 2 + 2;
    }
    else {
        w = h = GetSystemMetrics(SM_CYBORDER) * 2 + 2;
    }
    if (width  != 0) { *width  = w; }
    if (height != 0) { *height = h; }
}

// --------------------------------------------------------------------------
//  Set the maximum number of characters allowed for user entries
//  into this VComboBox to 'n' characters.
//
void VComboBox::setLimit(unsigned n)
{
    textLimit = n;
    SendMessage(hWnd, CB_LIMITTEXT, (WPARAM) n, (LPARAM) 0);
}

// ---------------------------------------------------------------------------
// Return the currently selected string of the this VComboBox as a 
// newly created `VString` object.
//
VString *VComboBox::selectedString() const
{
    int     i;
    char    temp[100];
    VString *s = NIL;

    for (i = 0; i < 99; i++) {
        temp[i] = ' ';
    }
    temp[i] = '\0';

    i = (int) SendMessage(hWnd, CB_GETLBTEXT, 
                            (WPARAM) selectedItem(), (LPARAM) temp);

    if (i > 0) {
        s = new VString(temp);
    }
    return s;
}

// ---------------------------------------------------------------------------
// Return MS Windows Information
//
void VComboBox::getInfo(VWinInfo &comboinfo)
{
    VControl::getInfo(comboinfo);
    comboinfo.wClass = "combobox"; 
    comboinfo.style |= WS_TABSTOP | CBS_AUTOHSCROLL | WS_VSCROLL;

    if (style.contains(StyleSorted)) {
        comboinfo.style |= CBS_SORT;
    }
    if (style.contains(StyleBorder)) {
        comboinfo.style |= WS_BORDER;
    }

    if (style.contains(StyleComboDrop)) {
        comboinfo.style |= CBS_DROPDOWNLIST;
    }
    else if (style.contains(StyleComboEditDrop)) {
        comboinfo.style |= CBS_DROPDOWN;
    }
    else { // default = StyleComboEdit
        comboinfo.style |= CBS_SIMPLE;
    }

#if defined(CV_WIN32)
    if (has3dEffects(V_WIN3D_EFFECTS)) {
        comboinfo.exStyle |= WS_EX_CLIENTEDGE;
        comboinfo.style &= ~WS_BORDER;
    }   
#endif
}

// ---------------------------------------------------------------------------
void VComboBox::getMinSize(short &w, short &h)
{
    w = GetSystemMetrics(SM_CXMIN)+ GetSystemMetrics(SM_CXVSCROLL);
    h = GetSystemMetrics(SM_CYVSCROLL) *2;
}

// ---------------------------------------------------------------------------
// code: notification code
//
boolean VComboBox::notify(int code)
{
    method mthd = 0; 
    VObject *clnt = 0;
    boolean ret = FALSE;

    switch (code) {
    case CBN_ERRSPACE:
        error("Out of memory for listbox!!");
        break;
    case CBN_SELCHANGE:
        mthd = selectMthd;
        clnt = client;
        ret = returnThis;
        break;
    case CBN_DBLCLK:
        mthd = dblClkMthd;
        clnt = dblClkClient;
        ret = dblClkReturnThis;
        break;
    case CBN_EDITCHANGE:
        mthd = changeMethod;
        clnt = changeClient;
        ret = TRUE;
        break;
    case ACCELcode:
		if (!VWindow::checkLosingFocus()) {
    	    takeFocus();
    	    break;
		}
		return FALSE;
    case CBN_DROPDOWN:
        droppedDown = TRUE;
        break;
    case CBN_CLOSEUP:
        droppedDown = FALSE;
        break;
    }
    if (clnt && mthd != NIL_METHOD) {
        if (ret) {
            clnt->perform(mthd, (long)this);
        }
        else {
            integer item = selectedItem();
            clnt->perform(mthd, (long)item);
        }
        return TRUE;
    }
    else {
        return FALSE;
    }
}

// ---------------------------------------------------------------------------
// Called when this `VWindow`\'s background is about to be erased. If
// a background `VBrush` is set ( via the `setBackground(VBrush *)` method),
// then that `VBrush` object will be used to paint the background of this
// `VWindow`. Otherwise the background VBrush of the parent window, or a 
// default system color will be used. 
//
boolean VComboBox::erased()
{
	return VListBox::erased();
}

// ---------------------------------------------------------------------------
// if shadow is turned on, draw it on the parent window
// !!!jmd-- do this a better way...
//
boolean VComboBox::paint()
{
	return VListBox::paint();
}

// ---------------------------------------------------------------------------
const VStyle& VComboBox::getStyle() const
{
    if (style.contains(StyleComboDrop)) {
        return StyleComboDrop;
    }
    else if (style.contains(StyleComboEditDrop)) {
        return StyleComboEditDrop;
    }
    else if (style.contains(StyleComboEdit)) {
        return StyleComboEdit;
    }
    else {
        return StyleDefault;
    }
}

// ---------------------------------------------------------------------------
void VComboBox::setStyle(const VStyle& style)
{
    if (style == StyleComboDrop) {
        if (this->style.contains(StyleComboDrop)) {
            return;
        }
        this->style += StyleComboDrop;
        this->style -= StyleComboEditDrop + StyleComboEdit;
    }
    else if (style == StyleComboEditDrop) {
        if (this->style.contains(StyleComboEditDrop)) {
            return;
        }
        this->style += StyleComboEditDrop;
        this->style -= StyleComboDrop + StyleComboEdit;
    }
    else if (style == StyleComboEdit) {
        if (this->style.contains(StyleComboEdit)) {
            return;
        }
        this->style += StyleComboEdit;
        this->style -= StyleComboDrop + StyleComboEditDrop;
    }
    notifier->recreateWin(this);
}

// ---------------------------------------------------------------------------
void VComboBox::showDropDown(boolean b)
{
    if (b) {
        if (isDroppedDown()) {
            return;
        }
        else {
            SendMessage(this->hwnd(), CB_SHOWDROPDOWN, 
                                                (WPARAM) TRUE, (LPARAM) 0);

            droppedDown = TRUE;
        }
    }
    else if (!isDroppedDown()) {
        return;
    }
    else {
        SendMessage(this->hwnd(), CB_SHOWDROPDOWN, (WPARAM) FALSE, (LPARAM) 0);
        droppedDown = FALSE;
    }
}

// ---------------------------------------------------------------------------
boolean VComboBox::isDroppedDown() const
{
    if (style.contains(StyleComboEdit)) {
        return TRUE;
    }
    else {
        return droppedDown;
    }
}

// ---------------------------------------------------------------------------
//
VString *VComboBox::getStringAt(int i)
{
    if (i < count()) {
        LRESULT len = SendMessage(hWnd, CB_GETLBTEXTLEN, (WPARAM)i, (LPARAM)0);
        if (len != LB_ERR && len > 0) {
            char *buf = new char [(int) len + 1];

            if (buf != 0) {
                *buf = '\0';

                SendMessage(hWnd, CB_GETLBTEXT, (WPARAM)i, (LPARAM)(LPCSTR)buf);

                VString *s = new VString(buf);
                delete [] buf;
                return s;
            }
        }
    }
    return 0;
}

//---------------------------------------------------------------------------
void VComboBox::uponChange(VObject *clnt, method mthd)
{
  changeClient = clnt;
  changeMethod = mthd;
}

// ----------------------------------------------------------------------------
// Here's the alleged deal with VComboBox height:
//
// - getRel(): returns the the *editline* height.
// - getDropDownHeight(): returns the *editline* height (editline + dropdown).
// - move() and resize(): if the given height equals the editline height,
//   then the height is *not* *changed* (!), otherwise the given height
//   refers to the *total* height (editline + dropdown).
//
void VComboBox::setGeometry(int x, int y, int w, int h)
{
    if (!style.contains(StyleComboEdit) && (hWnd != 0)) {
		RECT r;
		GetWindowRect(hWnd, LPRECT(&r));
		int new_h = r.bottom - r.top;
		if (new_h > 0) {
			h = new_h;
		}
	}
	VListBox::setGeometry(x, y, w, h);
}

// ----------------------------------------------------------------------------
//
void VComboBox::setGeometrySize(int w, int h)
{
    if (!style.contains(StyleComboEdit) && (hWnd != 0)) {
		RECT r;
		GetWindowRect(hWnd, LPRECT(&r));
		h = r.bottom - r.top;
	}
	VListBox::setGeometrySize(w, h);
}

// ----------------------------------------------------------------------------
//
UINT VComboBox::getCtlColor()
{
    if (style.contains(StyleComboEdit)) {
#if defined(CV_WIN32)
		return WM_CTLCOLORSTATIC;
#else
		return CTLCOLOR_STATIC;
#endif
	}
	else {
		return 0xFFFF;
	}
}

// ----------------------------------------------------------------------------
//
void VComboBox::destroyed()
{
	destroyDropDown();
}

// ===========================================================================
#ifndef CV_NOARCHIVER   

// --------------------------------------------------------------------------
//
void VComboBox::putTo(VArchiver& a)
{
    VComboBox::putTo(a, TRUE);
}

// --------------------------------------------------------------------------
//
void VComboBox::putTo(VArchiver& a, boolean most_derived)
{
    VListBox::putTo(a, FALSE);              //  base object data

    a << VComboBoxArchiveTag;

    if (VComboBoxTagRev >= 1) {
        a << textLimit;
    }

    if (VComboBoxTagRev >= 2) {
        // client
        a.putObject(notifier->getClientName(changeClient, changeClientIndex),                                       TRUE);
        // method
        //  name of change callback and the class to which it belongs

        const char *method_name = 0;            
        const char *class_name  = 0;            

        VCallbackList::findCallback(changeMethod, class_name,                                                       method_name, changeMethodIndex);

        a << (char *) class_name; 
        a << (char *) method_name;

    }

    if (most_derived) {                     //  children
        VWindow::putChildrenTo(a);
    }
}

// --------------------------------------------------------------------------
//
void VComboBox::getFrom(VArchiver& a)
{
    VListBox::getFrom(a);                       //  base object data

    long tag;
    a >> tag;
    if (!archiveTagOk(tag, VComboBoxArchiveTag)) {
        a.abort(archiveTagAbortCheck(tag, VComboBoxArchiveTag));
        return;
    }

    if (getArchiveTagRev(tag) > 0) {
        a >> textLimit;
    }
    if (getArchiveTagRev(tag) > 1) {
        VString client_name;                            //  name of client
        a.getObject(&client_name);
        changeClient = notifier->getClient(client_name, &changeClientIndex);
    
        //  change method
        char        method_name[256];
        char *      method_name_ptr = method_name;
        char        class_name[256];
        char *      class_name_ptr  = class_name;

        a >> class_name_ptr;    
        a >> method_name_ptr;   

        method sm = VCallbackList::findCallback(class_name_ptr, method_name_ptr, 
                                                            &changeMethodIndex);

        if (changeClient != 0 && sm != 0) {
            if (!changeClient->isA(VClass::of(class_name_ptr))) {
                a.abort(VArchiver::ClientCallbackMismatch);
                return;
            }
        }

        uponChange(changeClient, sm);

    }

}

// --------------------------------------------------------------------------
//
void VComboBox::getFrom(VArchiver& a, VObject *data, boolean do_realize)
{
    VComboBox::getFrom(a);              

    if (do_realize) {
        VComboBox::realize((VWindow *)data);
    }

    if (data != 0) {
        VWindow::getChildrenFrom(a, do_realize);
    }
}

// --------------------------------------------------------------------------
//
void VComboBox::realize(VWindow *pwin)
{
    VListBox::realize(pwin);
    setLimit(textLimit);
}

#endif

