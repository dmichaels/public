// $Header:   Q:/views/win/vcs/ddecls.cpv   1.10   Mar 04 1997 17:54:54   gregm  $ 

//	ddecls.cpp
//
//	VDde
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
//	Revision History:
//	-----------------
//	04/08/93 pat	updated idToData() and dataToId() for new VArchiver
//	04/07/93 pat	changed id to VObject *
//	08/09/93 jmd    removed free
//	10/04/93 jmd    preened
//	10/10/93 jmd    SC6 changes (typdef'd enums).
//	03/09/94 pat	STRICT: 
//					added casts in Post/SendMessage() calls;
//					msg of user() is now UINT, not int
//	04/15/94 pat	CV_WIN32: dealt with WM_DDE_TERMINATE message packing 
//					under sndTerminate().
//	06/22/94 jld    Added CV_WIN32 message packing macros and rewrote
//					sndTerminate() and user() to use them.
//	07/18/95 dgm	Miscellaneous const char * changes.
//  08/15/95 evc    Got rid of references to VMemory.
//	09/01/95 dss	VStyle changes.
// ----------------------------------------------------------------------------

#include "notifier.h"
#include "ddecls.h"
#include "atom.h"
#include "archiver.h"

extern "C" {
#       include <dde.h>
#       include <string.h>
}

defineClass(VDde, VWindow)


// ---------------------------------------------------------------------------
// Construct an idle VDde window.
//
VDde::VDde()
{
	pendingRequest = 0;
	terminating = 0;
	hWndRemote = 0;
	ackTimeout = DDEackTimeout;
	client = 0;
	mthd = 0;
	build(StyleDefault);
}

// ---------------------------------------------------------------------------
// Use a simple popup window style
//
void VDde::getInfo(VWinInfo &info)
{
	VWindow::getInfo(info);
	info.style = WS_POPUP;
}

// ---------------------------------------------------------------------------
// This version of 'user' handles the terminate and ack messages.
//
boolean VDde::user(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {

	case WM_DDE_TERMINATE:  // at CLIENT or SERVER
		rcvTerminate();         // response handled by rcvTerminate
		break;

	case WM_DDE_ACK:                // at CLIENT or SERVER
		UINT hi, lo;
		UnpackDDElParam(WM_DDE_ACK, l, &lo, &hi);
		rcvAck(w, lo, hi);
		FreeDDElParam(WM_DDE_ACK, l);
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Called when a DDE terminate message has been received. If this
// VDde object initiated the termination, then end the wait. Otherwise
// recipricate with a terminate message.
//
void VDde::rcvTerminate()
{
	if (terminating) {
		yieldEnd(DDEack);
		hWndRemote = 0;
	}
	else {  
		sndTerminate(TRUE);
		hWndRemote = 0;
		callback(DdeTerminate, 0, 0);
	}

}

// ---------------------------------------------------------------------------
// Called when a DDE acknowledge message is received. The action
// taken here is to simply end the wait.
//
void VDde::rcvAck(UINT w, UINT lo, UINT hi)
{
	VAtom a(hi);
	yieldEnd(lo);
	!a;
}

// ---------------------------------------------------------------------------
// Send a termination message to the remote VDde object. If 'immed'
// is TRUE, then use SendMessage rather than PostMessage.
//
boolean VDde::sndTerminate(boolean immed)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	if (immed) {
		return SendMessage(hWndRemote, WM_DDE_TERMINATE, (UINT)hWnd, 0)
				? TRUE : FALSE;
	}
	else {
		return PostMessage(hWndRemote, WM_DDE_TERMINATE, (UINT)hWnd, 0)
				? TRUE : FALSE;
	}
}

// ---------------------------------------------------------------------------
// Send an acknowledge message to the remote VDde object, using 'lo'
// and 'hi' as its long parameter. If 'immed' is TRUE, then use
// 'SendMessage' rather than 'PostMessage'.
//
boolean VDde::sndAck(UINT lo, UINT hi, int immed)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	if (immed) {
		return SendMessage(hWndRemote, WM_DDE_ACK, (UINT)hWnd,  
							PackDDElParam(WM_DDE_ACK, lo, hi) )
					? TRUE : FALSE;
	}
	else {
		return PostMessage(hWndRemote, WM_DDE_ACK, (UINT)hWnd,
							PackDDElParam(WM_DDE_ACK, lo, hi) )
					? TRUE : FALSE;
	}
}

// ---------------------------------------------------------------------------
// Called when there is DDE data to be converted to a C++ object. 
// 'Data' points to 'sz' bytes of data whose format is 'format'. This
// implementation handles string data, CF_TEXT, and archived C++ object
// data, CF_OBJ.
//
VObject *VDde::dataToId(UINT format, const char *data, long sz)
{
	if (format == CF_TEXT)  {
		return new VString(data);
	}
	else if (format == CF_OEMTEXT) {
		VArchiver a;
		a.write(data, sz);
		a.rewind();
		return a.getObject(0);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Called when 'obj' is to be converted to format 'format'.  Return TRUE 
// if the conversion is successful, otherwise return FALSE. The setData
// function is called from within this function to actually create the 
// necessary MS Windows or OS/2 data object. This implementation handles
// CF_TEXT and CF_OBJ formats.
//
boolean VDde::idToData(VObject *obj, UINT format)
{
	if (obj->isA(VStringCls) && format == CF_TEXT) {

		//      format a string for DDE

		VString *s = (VString * ) obj;
		setData(CF_TEXT, s->size() + 1, *s);
		return TRUE;
	}
	else if (format == CF_OEMTEXT) {
		//      format a VObject for DDE by archiving it to memory 
		//      and formatting the archive buffer

		VArchiver a;
		a.putObject(obj);

		const char *buf;
		unsigned long buflen;
		if (a.getMemoryArchiveBuffer(buf, buflen)) {
			setData(CF_OEMTEXT, buflen + 1, buf);
			return TRUE;
		}
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Called from within the idToData member function to actually
// construct the DDE data object. 'Format' is the format to be used and 
// 'data' points to 'sz' bytes of data comprising the object.
//
void VDde::setData(UINT format, long sz , const char *data)
{
	hData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
	sz + sizeof(DDEPOKE));
	char *p = (char *) GlobalLock(hData);
	((DDEDATA *)p)->cfFormat = format;
	p = (char*)((DDEDATA *)p)->Value;
	long i;
	if (!data) {
		*p = 0;
	}
	else {
		for (i = 0; i < sz; i++) {
			*p++ = *data++;
		}
	}
	GlobalUnlock(hData);
}

// ---------------------------------------------------------------------------
//      Return the handle of a memory store containing the given string.
//      This memory is global, shared memory--suitable for passing between
//      DDE client and DDE server.
//
handle VDde::strToHandle(const char *s)
{
	handle h = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE,
	strlen(s) + 1);
	
	if (h != 0) {
		char *p;
		if ((p = (char *) GlobalLock(h)) == 0) {
			GlobalFree(h);
			return 0;
		}
		strcpy(p, s);
		GlobalUnlock(h);
	}
	return h;
}

// ---------------------------------------------------------------------------
// Arrange for method 'm' to be called on object 'obj' when a DDE event occurs.
//
void VDde::uponEvent(VObject *obj, method m)
{
	client = obj;
	mthd = m;
}

// ---------------------------------------------------------------------------
// Perform the callback specifying reason r, item itm and data object obj
//
boolean VDde::callback(DdeEvent r, char *itm, VObject *obj)
{
	if (!client || mthd == NIL_METHOD) {
		return TRUE;
	}
	pItem = itm;
	pData = obj;
	wReason = r;
	return client->perform(mthd, (long)this);
}

// ---------------------------------------------------------------------------
// Terminate a wait loop with 'result' status.
// 
void VDde::yieldEnd(DdeResult result)
{
	yielding = 0;
	wResult = result;
}

// ---------------------------------------------------------------------------
// Wait 'timeout' milliseconds for an acknowledge message from the server.
// 
boolean VDde::yieldAck(UINT timeout)
{
	MSG msg;
	unsigned long t = timeout + GetTickCount();
	wResult = DDEack;
	yielding = 1;
	while (1) {
		if (PeekMessage(&msg, 0, WM_DDE_FIRST, WM_DDE_LAST, PM_REMOVE)) {
			DispatchMessage(&msg);
			yielding = 0;
		}
		if (!yielding) {
			break;
		}
		if (GetTickCount() > t) {
			wResult = DdeTimeout;
			break;
		}
	}
	yielding = 0;
	return (wResult == DDEack) ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
//      Terminate the current DDE connection.
//
boolean VDde::disconnect()
{
	int result = FALSE;
	if (hWndRemote) {
		terminating = 1;
		if (sndTerminate(FALSE)) {
			result = yieldAck(ackTimeout);
		}
		terminating = 0;
	}
	return result;
	
}

// ---------------------------------------------------------------------------
UINT VDde::objectFormat()
{
	return CF_OEMTEXT;
}

// ---------------------------------------------------------------------------
