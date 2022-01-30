// $Header:   Q:/views/win/vcs/ddeclnt.cpv   1.9   Mar 04 1997 17:54:54   gregm  $ 

//	ddeclnt.cpp
//
//	VDdeClient
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
//	08/09/93 jmd    removed free
//	09/03/93 pat	per teb, fixed atom cleanup in used()
//	10/04/93 jmd    moved references to 'data' to before rcvData in ::user
//	10/04/93 jmd    preened
//	03/09/94 pat	STRICT:
//					msg of user() is now UINT, not int;
//					cast appropriately in PostMessage() and SendMessage()
//	04/15/94 pat	CV_WIN32: dealt with WM_DDE_EXECUTE message packing
// 					under sndExecute().
//	06/22/94 jld    Added CV_WIN32 message packing macros and rewrote
//					user(), sndAdvise() and sndPoke() to use them.
//  07/07/94 jmd	changed void * to HANDLE
//	07/14/94 jld	replaced NULL with 0
//	03/02/95 jld	replace VMemory::memhndl() with
//					FreeDDElParam macro for WIN16
//	07/18/95 dgm	Miscellaneous const char * changes.
//  08/14/95 evc    Got rid of references to VMemory
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "ddeclnt.h"
#include "atom.h"

extern "C" {
#       include <dde.h>
}

defineClass(VDdeClient, VDde)

// ---------------------------------------------------------------------------
// Construct an idle DDE client window.
//
VDdeClient::VDdeClient()
{
	state = ClientIdle;
	requestTimeout = DDErequestTimeout;
	hExec = 0;
}

// ---------------------------------------------------------------------------
VDdeClient::~VDdeClient()
{
	disconnect();
}

// ---------------------------------------------------------------------------
// Handles all client DDE messages.
//
boolean VDdeClient::user(UINT msg, WPARAM w, LPARAM l)
{
	switch (msg) {
	default: 
		return VDde::user(msg, w, l);

	case WM_DDE_DATA: // at CLIENT only
	{
		UINT hi, lo;
		UnpackDDElParam(WM_DDE_DATA, l, &lo, &hi);
		HGLOBAL hDDeData        = (HGLOBAL) lo;
		ATOM    atm             = (ATOM)    hi;
		VAtom   a(atm);

		DDEDATA *data = (DDEDATA *) GlobalLock(hDDeData);
		if (!data) { // warm link notification
			hi = HIWORD(l);
			VAtom   a(hi);
			rcvData((char*)a, 0, 0, 0, 0);
			!a;
			break;
		}
	
		long sz = GlobalSize(hDDeData) - sizeof(DDEDATA) + 1;

		DdeResult rst = rcvData((char *)a, data->cfFormat,
							(char *)data->Value, sz, data->fResponse);
		if ((rst & DDEnak) || data->fAckReq) {
			sndAck(rst, atm);
		}
		else {
			// no acknowlegement...we must delete the atom
			GlobalDeleteAtom(hi); 
		}

		GlobalUnlock(hDDeData);
		if (data->fRelease) {
			FreeDDElParam(WM_DDE_DATA, l);
		}

		// VAtom increments use count ... decrement it back
		!a;     
		}
		break;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Sends the given command string to the server to be executed.
//
boolean VDdeClient::sndExecute(const char *cmd)
{
	if (hWndRemote == 0) {
		return FALSE;   //      no other DDE to receive
	}

	//      Allocate global, shared memory (suitable to passing to the server)
	//      and copy the command string to it.
	//
	//      Note that we are responsible for deleting the command memory 
	//      that the server returns when it acknowledges our execute message.
	//      Also note, that we assume that the server reuses this memory--
	//      VDdeClient::rcvAck() compares hExec to the memory handle that 
	//      the server returns to ascertain whether the server is 
	//      acknowledging an execute message.
	
	hExec = strToHandle(cmd);

	LPARAM l = PackDDElParam( WM_DDE_EXECUTE, 0, (UINT) hExec ) ;
	if (!PostMessage(hWndRemote, WM_DDE_EXECUTE, (WPARAM) hWnd, l )) {
		FreeDDElParam(WM_DDE_EXECUTE, l);
		GlobalFree(hExec);
		hExec = 0;
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
// Called when a DDE data message has been received. If 'value' is NULL
// then this is a warm link notify via DdeItemChange. If there was a pending
// request then convert the data to an object and end the wait, else
// notify client via DdeItemData after converting the data to an object.
//
DdeResult VDdeClient::rcvData(const char *item, UINT format,
							  const char *value, long sz, UINT response)
{
	if (!value) { // a warm-link notify
		callback(DdeItemChange, (char *)item, 0);
	}
	else if (pendingRequest && response) { // data has arrived
		pData = dataToId(format, value, sz);
		yieldEnd(DDEack);
	}
	else {
		callback(DdeItemData, (char *)item, dataToId(format, value, sz));
	}
	return DDEack;
}

// ---------------------------------------------------------------------------
// This implementation of rcvAck handles the acknowledgement
// of a DDE execute operation and the cleanup of multiple acknowlements
// of server objects. The first acknowledging server is accepted.
//
void VDdeClient::rcvAck(UINT w, UINT lo, UINT hi)
{
	if (hExec != 0 && hExec == (HANDLE) hi) { // ACK/NAK of a DDE_EXECUTE
		GlobalFree(hExec); //      delete the command memory store
		hExec = 0;
		yieldEnd(lo);
	}
	else if (state == ClientInitiating) {
		if (hWndRemote) {
			HWND tmp = hWndRemote; hWndRemote = (HWND) w;
			sndTerminate(TRUE);
			hWndRemote = tmp;
		}
		else {
			hWndRemote = (HWND) w;
		}
	}
	else  {
		VDde::rcvAck(w, lo, hi);
	}
}

// ---------------------------------------------------------------------------
// Broadcast a DDE initiate message for application 'appl' and topic
// 'topic', then wait until a connection is made. Return TRUE if there was
// at least one acknowledgement else return FALSE.
//
boolean VDdeClient::sndInitiate(const char *appl, const char *topic)
{
	VAtom a(appl);
	VAtom t(topic);
	state = ClientInitiating;
	if (!SendMessage((HWND)-1, WM_DDE_INITIATE, 
		(WPARAM) hWnd, (LPARAM) MAKELONG((UINT)a, (UINT)t))) {

		!a;
		!t;
		return FALSE;
	}
	!a;
	!t;
	if (hWndRemote) {
		state = ClientConnected;
	}
	if (state == ClientInitiating) {
		state = ClientIdle;
		return FALSE;
	}
	return TRUE; 
}

// ---------------------------------------------------------------------------
// Send a DDE request message to the remote DDE for item 'item' of 
// 'format'. Return TRUE if the request was successful else return FALSE.
//
boolean VDdeClient::sndRequest(const char *item, UINT format)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	VAtom a(item);

	LPARAM l = PackDDElParam(WM_DDE_REQUEST, 0, (UINT)a) ;
	if (!PostMessage(hWndRemote, WM_DDE_REQUEST, (WPARAM) hWnd, l )) {
		FreeDDElParam( WM_DDE_REQUEST, l);
		!a;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Send a DDE advise message to the remote DDE for item 'item' for format
// 'format'. If 'warmLink' is TRUE, then request defered notification.
// Return TRUE if the advise was successful else return FALSE.
//
boolean VDdeClient::sndAdvise(const char *item, boolean warmLink, UINT format)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	HGLOBAL dh = 
		  GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(DDEADVISE));
	DDEADVISE *d = (DDEADVISE *)GlobalLock(dh);
	VAtom a(item);
	d->cfFormat = format;
	d->fAckReq = TRUE;
	d->fDeferUpd = warmLink;
	GlobalUnlock(dh);

	LPARAM l = PackDDElParam(WM_DDE_ADVISE, (UINT)dh, (UINT)a) ;
	if (!PostMessage(hWndRemote, WM_DDE_ADVISE, (WPARAM) hWnd, l)) {
		FreeDDElParam(WM_DDE_ADVISE, l);
		!a;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Send a DDE unadvise message to the remote DDE for item 'item'.
// Return TRUE if the unadvise was successful else return FALSE.
//
boolean VDdeClient::sndUnadvise(const char *item)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	VAtom a(item);

	if (!PostMessage(hWndRemote, WM_DDE_UNADVISE, (WPARAM) hWnd, 
					(LPARAM) MAKELONG(0, (UINT)a) )) {
		!a;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Send a DDE poke message to the remote DDE for item 'item' and data 'h'.
// Return TRUE if the poke was successful else return FALSE.
//
boolean VDdeClient::sndPoke(const char *item, UINT h)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	DDEPOKE *poke = (DDEPOKE *)GlobalLock((HANDLE) h);
	poke->fRelease = TRUE;
	GlobalUnlock((HANDLE) h);
	VAtom a(item);

	LPARAM l = PackDDElParam(WM_DDE_POKE, h, (UINT)a) ;
	if (!PostMessage(hWndRemote, WM_DDE_POKE, (WPARAM) hWnd, l )) {
		FreeDDElParam(WM_DDE_POKE, l);
		!a;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// This verion of rcvTerminate handles terminations of multiple server ACK\'s.
// 
void VDdeClient::rcvTerminate()
{
	if (state == ClientInitiating) {
		return;
	}
	VDde::rcvTerminate();
}

// ---------------------------------------------------------------------------
// Connect to a remote DDE for application 'appl' and topic 'topic' and
// return TRUE, otherwise return FALSE if no DDE server responded.
//
boolean VDdeClient::connect(const char *appl, const char *topic)
{
	return sndInitiate(appl, topic);
}

// ---------------------------------------------------------------------------
// Arrange so that no notification is sent from the server DDE for item 'item'. 
//
boolean VDdeClient::coldLink(const char *item)
{
	if (sndUnadvise(item)) {
		return yieldAck(DDEackTimeout);
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Arrange so that a DdeItemChange callback will occur whenever
// the server deems that the item 'item' has changed. This VDdeClient object
// may then obtain the item via a call to requestItem().
//
boolean VDdeClient::warmLink(const char *item, UINT format)
{
	if (sndAdvise(item, TRUE, format)) {
		return yieldAck(DDEackTimeout);
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Arrange so that a DdeItemData callback will occur whenever
// the server deems that the item 'item' has changed. The server will send
// the 'item' in format 'format' when it changes. From within the notification
// callback, this VDdeClient object may obtain the item via a call to
// dataItem().
//
boolean VDdeClient::hotLink(const char *item, UINT format)
{
	if (sndAdvise(item, FALSE, format)) {
		return yieldAck(DDEackTimeout);
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Request the server DDE to send the data associated with item 'item'
// in format 'format' and return the data object, or NIL if the data is not
// availible or the request timed-out.
//
VObject *VDdeClient::requestItem(const char *item, UINT format)
{
	VObject *obj = 0;
	pendingRequest = 1;
	if (sndRequest(item, format)) {
		if (yieldAck(requestTimeout)) {
			obj = dataItem();
		}
	}
	pendingRequest = 0;
	return obj;
}

// ---------------------------------------------------------------------------
// Send the data object 'obj' as the data associated with item 'item'
// using format 'format' and return TRUE. Return FALSE if the server
// did not acknowledge the send. The default format is 'CF_TEXT'.
//
boolean VDdeClient::sendItem(const char *item, VObject *obj, UINT format)
{
	int result = FALSE;
	if (!idToData(obj, format)) {
		return FALSE;
	}
	if (sndPoke(item, (UINT)hData)) {
		result =  yieldAck(DDEackTimeout);
	}
	return result;
	
}

// ---------------------------------------------------------------------------
//      Execute the given command on the DDE server.
//      Return TRUE if the command was successfully executed; FALSE, otherwise.
//
boolean VDdeClient::execute(const char *cmd)
{
	if (sndExecute(cmd)) {
		return yieldAck(DDEackTimeout);
	}
	return FALSE;
}
