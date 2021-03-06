// $Header:   Q:/views/win/vcs/clipbord.cpv   1.8   Mar 04 1997 17:16:48   gregm  $ 

//  clipbord.cpp
//
//  VClipBoard implementation
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
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
//  04/08/93 pat    preened, changed id to VObject *, added destructor
//  04/08/93 pat    updated get() and put() for new VArchiver
//  08/09/93 jmd    removed free
//  08/10/93 pat    put() now copies directly from the archive memory buffer.
//  08/12/93 pat    added rewind() before getObject() in get() per atb.
//  10/06/93 dgm    changed get() to support GUI objects.
//  07/14/94 jld    replaced NULL with 0
//  09/14/95 evc    Got rid of all references to VMemory
//  01/25/97 dgm    Added real bitmap/dib support.
// ----------------------------------------------------------------------------

#include "clipbord.h"
#include "winclass.h"
#include "window.h"
#include "bitmap.h"
#include "str.h"
#include "archiver.h"
#include "notifier.h"
#include <string.h>

// --------------------------------------------------------------------------
//
defineClass(VClipBoard, VObject)

// --------------------------------------------------------------------------
//
VClipBoard clipboard;

// --------------------------------------------------------------------------
//
static unsigned int CF_OBJECT; // clipboard format for a VObject

// --------------------------------------------------------------------------
//  Construct an instance of the VClipBoard class.  Instances of
//  VClipBoard can be constructed temporarily for get(), put(), clear(),
//  contains() and isEmpty() operations.
//
VClipBoard::VClipBoard()
{
    CF_OBJECT = RegisterClipboardFormat((LPSTR)"CV_VObject");
}

// --------------------------------------------------------------------------
// Destroy this VClipBoard.
//
VClipBoard::~VClipBoard()
{
}

// --------------------------------------------------------------------------
// Empty the contents of the clipboard.  
// Return TRUE if succesful; FALSE, otherwise.
//
boolean VClipBoard::clear()
{
    VWindow *window = notifier->getFocus();
    if (window == 0) {
		return TRUE;
	}
	HWND hwnd = window->hwnd();
    if (hwnd == 0) {
		return TRUE;
	}
	if (!OpenClipboard(hwnd)) {
        return FALSE;
    }
    EmptyClipboard();
    CloseClipboard();
    return TRUE;
}

// --------------------------------------------------------------------------
// Return TRUE if there is nothing on the clipboard, 
// otherwise return FALSE.
//
boolean VClipBoard::isEmpty()
{
	if (IsClipboardFormatAvailable(CF_TEXT) ||
		IsClipboardFormatAvailable(CF_OBJECT) ||
		IsClipboardFormatAvailable(CF_BITMAP) ||
		IsClipboardFormatAvailable(CF_DIB)) {
		return FALSE;
	}
	else {
		return TRUE;
	}
}

// --------------------------------------------------------------------------
// Return TRUE if there is an instance of an object with the 
// specified class 'c' on the clipboard, otherwise return FALSE.
//
boolean VClipBoard::contains(VClass *c)
{
	if (c == 0) {
		return FALSE;
	}

	if (c == VBitMapCls) {
		return VBitMap::canPasteFromClipboard();
	}

    if (isEmpty()) {
		return FALSE;
	}

    VWindow *window = notifier->getFocus();
	if (window == 0) {
		return FALSE;
	}

	HWND hwnd = window->hwnd();
	if (hwnd == 0) {
		return FALSE;
	}

    if (!OpenClipboard(hwnd)) {
		return FALSE;
	}

	if ((c == VStringCls) && IsClipboardFormatAvailable(CF_TEXT)) {
		CloseClipboard();
		return TRUE;
	}

	HANDLE hdata;

	if ((hdata = GetClipboardData(CF_OBJECT)) != 0) {
		LPSTR p;
		if ((p = (char *)GlobalLock(hdata)) != 0) {
			char ch, *q;
			for (q = p + 2 ; (*q != '\r') && (*q != '\n') ; q++) { 
				; 
			}
			ch = *q;
			*q = 0;
			if (strcmp(c->nameOf(), p + 2) == 0) {
				GlobalUnlock(hdata);
				CloseClipboard();
				return TRUE;
			}
			*q = ch;
		}
		GlobalUnlock(hdata);
		CloseClipboard();
		return FALSE;
	}
	CloseClipboard();
    return FALSE;
}

// --------------------------------------------------------------------------
// Return the object on the clipboard, otherwise return NIL
// if there is no object on the clipboard or the object is not recognizable.
//
VObject *VClipBoard::get(VObject *data, boolean do_realize)
{
	if (VBitMap::canPasteFromClipboard()) {
		VBitMap *bitmap = new VBitMap;
		if (bitmap->pasteFromClipboard()) {
			return bitmap;
		}
		else {
			delete bitmap;
		}
	}

    VWindow *window = notifier->getFocus();
	if (window == 0) {
		return FALSE;
	}

    HWND hwnd = window->hwnd();
	if (hwnd == FALSE) {
		return FALSE;
	}

    if (!OpenClipboard(window->hwnd())) {
		return FALSE;
	}

    VObject *object = 0;
    HANDLE hdata = 0;

	if ((hdata = GetClipboardData(CF_OBJECT)) != 0) {
		VArchiver a;
		a.rewind();
		char *p = (char *)GlobalLock(hdata);
		if (p != 0) {
			a.write(p, integer(GlobalSize(hdata)));
		}
		a.rewind();
		if (data == 0) {
			object = a.getObject(0);
		}
		else {
			object = a.getObject(0, data, do_realize);
		}
		GlobalUnlock(hdata);
	} 
	else if ((hdata = GetClipboardData(CF_TEXT)) != 0) {
    	LPSTR p;
		if ((p = (char *)GlobalLock(hdata)) != 0) {
			object = new VString(p);
			GlobalUnlock(hdata);
		}
	}
	CloseClipboard();
	return object;
}

// --------------------------------------------------------------------------
// Store object 'o' on the clipboard.  Text data is put onto the clipboard
// by passing a VString object containing the text as parameter 'o'.
// Otherwise, except for VBitMap objects, all objects that are to be put
// on the clipboard must be archivable.
//
// If the class of the specified object is one that is supported with a 
// standard clipboard format, then the contents of the object will be 
// converted to the standard format and stored on the clipboard along with 
// the object. For example, if the specified object is an instance of the 
// VString class, then both the object itself is stored on the clibpoard, 
// and the contents of the object is extracted and stored in standard text 
// format.  
//
boolean VClipBoard::put(VObject *object)
{
	if (object == 0) {
		return 0;
	}

	if (object->iam() == VBitMapCls) {
		if (((VBitMap *)object)->copyToClipboard()) {
			return TRUE;
		}
	}

    VWindow *window = notifier->getFocus();
	if (window == 0) {
		return FALSE;
	}

    HWND hwnd = window->hwnd();
	if (hwnd == FALSE) {
		return FALSE;
	}

	if (!OpenClipboard(hwnd)) {
		return FALSE;
	}

	EmptyClipboard();

	boolean is_string = object->getIsA() == VStringCls;

	constPointer	source;
	char		   *destination;
	unsigned long	size;
	VArchiver		a;

	if (is_string) {
		source = ((VString *)object)->gets();
		size = ((VString *)object)->size();
	}
	else {
		a.rewind();
		a.putObject(object);
		const char *buf;
		unsigned long buflen;
		(void)a.getMemoryArchiveBuffer(buf, buflen);
		source = buf;
		size = buflen;
	}
	//
	// Allocate a memory store and copy the object into it.
	//
	HGLOBAL h = 0;
	h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, size+1);
	if (h != 0 && (destination = (char *) GlobalLock(h)) != 0) {
		while (size-- != 0) {
			*destination++ = *source++;
		}
		*destination = 0;
		GlobalUnlock(h);
		SetClipboardData(is_string ? CF_TEXT: CF_OBJECT, h);
		CloseClipboard();
    	return TRUE;
	}
	CloseClipboard();
    return FALSE;
}
