// $Header:   Q:/views/win/vcs/ddeserv.cpv   1.7   Mar 04 1997 17:54:56   gregm  $ 

//	ddeserv.cpp
//
//	VDdeServer
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
//	10/04/93 jmd    preened
//	03/09/94 pat	STRICT: msg of user() is now UINT, not int.
//	03/14/94 pat	CV_WIN32:  changed type of user() message param to UINT
//	06/22/94 jld    Added CV_WIN32 message packing macros and rewrote
//					user() and sndData() to use them.
//	07/07/94 jmd	changed void * to HANDLE
//	07/14/94 jld    replaced NULL with 0
//	03/02/95 jld    replaced VMemory::memhndl() with
//					FreeDDElParam macro for WIN16
//	03/03/95 jld    set 'rst' for WM_DDE_REQUEST in user()
//  08/14/95 evc	got rid of VMemory references
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "ddeserv.h"
#include "atom.h"
#include "set.h"
#include "str.h"
#include "iterator.h"

extern "C" {
#       include <dde.h>
#       include <string.h>
}

defineClass(VDdeServer, VDde)

// ---------------------------------------------------------------------------
// This is a private class that hold warm/hot link information on a given item.
//
class VAdvise : public VString {
public:
			VAdvise(char *);
			VAdvise(char *, boolean, UINT);

	int             defer()         { return wDefered; }
	UINT            format()        { return wFormat; }

private:
	int             wDefered;
	UINT    wFormat;
};

// ---------------------------------------------------------------------------
VAdvise::VAdvise(char *s) :
	VString(s)
{
	;
}

// ---------------------------------------------------------------------------
VAdvise::VAdvise(char *s, boolean defer, UINT format) :
	VString(s)
{
	wDefered = defer;
	wFormat = format;
}

// ---------------------------------------------------------------------------
// Construct a per-client DDE VDdeServer object.
//
void VDdeServer::serve(VDdeServer *prnt)
{
	hWndRemote = prnt->hWndRemote;
	client = prnt->client;
	mthd = prnt->mthd;
	parent = prnt;
	advise = 0;
	acknowledge = 0;
	pAppl = new char [ strlen(prnt->pAppl) + 1 ];
	strcpy( pAppl, prnt->pAppl );
	pTopic = new char [ strlen(prnt->pTopic) + 1 ];
	strcpy( pTopic, prnt->pTopic );
	VAtom a(prnt->pAppl);
	VAtom b(prnt->pTopic);
	sndAck((UINT)a, (UINT)b, TRUE);
	!a;
	!b;
}

// ---------------------------------------------------------------------------
//      Construct a VDdeServer object to handle service requests.
//
VDdeServer::VDdeServer()
{
	pAppl = pTopic = 0;
	hWndRemote = 0;
	parent = 0;
	advise = 0;
	acknowledge = 0;
}

// ---------------------------------------------------------------------------
VDdeServer::~VDdeServer()
{
	if (parent != 0) {
		// only children keep track of appl and topic
		delete [] pAppl;
		delete [] pTopic;
	}
	VDde::disconnect();
	if (advise) {
		delete advise;
	}
}

// ---------------------------------------------------------------------------
// Handles all server DDE messages
//
boolean VDdeServer::user(UINT msg, WPARAM w, LPARAM l)
{
	UINT hi, lo;
	long sz; 
	DdeResult rst;

	switch (msg) {
	default: 
		return VDde::user(msg, w, l);

	case WM_DDE_INITIATE: { // at SERVER only
			UnpackDDElParam(WM_DDE_INITIATE, l, &lo, &hi);
			rst = rcvInitiate((char*)VAtom(lo), (char*)VAtom(hi), (HANDLE)w);
		}
		break;

	case WM_DDE_REQUEST: { // at SERVER only
			hi = HIWORD(l);
			lo = LOWORD(l);
			rst = rcvRequest((char*)VAtom(hi), lo);
			if (rst == DDEnak) {
				sndAck(DDEnak, hi);
			}
		}
		// data responses handled by rcvRequest
		break;

	case WM_DDE_ADVISE: {  // at SERVER only
			UnpackDDElParam(WM_DDE_ADVISE, l, &lo, &hi);
			DDEADVISE *advise = (DDEADVISE *)GlobalLock((HANDLE)lo);
			rst = rcvAdvise((char*)VAtom(hi), advise->cfFormat, advise->fDeferUpd);
			if ((rst & DDEnak) || advise->fAckReq) {
				sndAck(rst, hi);
			}
			GlobalUnlock((HANDLE)lo); // DDEADVISE owned by CLIENT
		}
		break;

	case WM_DDE_UNADVISE: { // at SERVER only
			hi = HIWORD(l);
			lo = LOWORD(l);
			rst = rcvUnadvise((char*)VAtom(hi));
			sndAck(rst, hi); // always ACK/NAK
		}
		break;

	case WM_DDE_EXECUTE: {  // at SERVER only
			UnpackDDElParam(WM_DDE_EXECUTE, l, &lo, &hi);
			char *cmd = (char*)GlobalLock((HANDLE)hi);
			rst = rcvExecute(cmd);
			GlobalUnlock((HANDLE)hi); // owned by client
			sndAck(rst, hi); // always ACK/NAK
		}
		break;

	case WM_DDE_POKE: {  // at SERVER only
			UnpackDDElParam(WM_DDE_POKE, l, &lo, &hi);
			DDEPOKE *data = (DDEPOKE *)GlobalLock((HANDLE)lo);
			sz = GlobalSize((HANDLE)lo) - sizeof(DDEPOKE) + 1;
			rst = rcvPoke((char*)VAtom(hi), data->cfFormat, (char*)data->Value, sz);
			sndAck(rst, hi); // Always ACK/NAK
			boolean release = data->fRelease;
			GlobalUnlock((HANDLE)lo);
			if (release) {
				FreeDDElParam(WM_DDE_POKE, l);
			}
		}
		break;
	}

	return rst;
}

// ---------------------------------------------------------------------------
// Called when a initiate message has been received. Performs
// the callback and ACKs if it was accepted.
//
DdeResult VDdeServer::rcvInitiate(char*app, char*topic, handle h)
{
	VDdeServer *srv;
	if (parent) {
		return FALSE;
	}
	boolean r;
	hWndRemote = (HWND) h;
	pAppl = app;
	pTopic = topic;
	if (r = callback(DdeService, pTopic, 0)) { 
		addChild(srv = (VDdeServer *)iam()->construct());
		srv->serve(this);
	}
	hWndRemote = 0;
	pAppl = 0;
	pTopic = 0;
	return r;
}

// ---------------------------------------------------------------------------
// Called when the DDE client has requested data for item 'item'
// in format 'format'. Performs the callback and ACK/NAKs as appropriate.
// The callback method is resposible for sending the item.
//
DdeResult VDdeServer::rcvRequest(char*item, UINT format)
{
	wFormat = format;
	pendingRequest = TRUE;
	boolean r = callback(DdeRequest, item, 0);
	pendingRequest = FALSE;
	return (r) ? DDEack : DDEnak;
}

// ---------------------------------------------------------------------------
// Called when a DDE advise message has arrived for item 'item' and format
// 'format'. If 'defer' is TRUE, then a hot-link is requested, else a warm-link
// is. Performs the callback and ACK/NAKs as appropriate.
//
DdeResult VDdeServer::rcvAdvise(char*item, UINT format, UINT defer)
{
	wFormat = format;
	boolean r = callback(DdeLink, item, 0);
	if (r) { // add to advisory set
		adviseOn(item, defer);
	}
	return (r) ? DDEack : DDEnak;
}

// ---------------------------------------------------------------------------
// Called when an DDE unadvise message has arrived. Performs the callback
// and ACK/NAKs as appropriate.
// 
DdeResult VDdeServer::rcvUnadvise(char*item)
{
	boolean r = callback(DdeUnlink, item, 0);
	if (r) { // remove from advisory set
		adviseOff(item);
	}
	return (r) ? DDEack : DDEnak;
}

// ---------------------------------------------------------------------------
// Called when a DDE execute message has arrived giving command 'cmd'. 
// Performs the callback and ACK/NAKs as appropriate.
//
DdeResult VDdeServer::rcvExecute(char *cmd)
{
	pCommand = cmd;
	return callback(DdeExecute, pCommand, 0) ? DDEack : DDEnak;
}

// ---------------------------------------------------------------------------
// Called when a DDE poke message has arrived giving data 'value'
// of 'sz' bytes of format 'format' for item 'item'.
// Performs the callback and ACK/NAKs as appropriate.
// The callback method is responsible for retrieving the data object.
//
DdeResult VDdeServer::rcvPoke(char*item, UINT format, char *value, long sz)
{
	wFormat = format;
	return callback(DdePokeData, item, dataToId(format, value, sz)) ? DDEack : DDEnak;
}

// ---------------------------------------------------------------------------
// Sends a DDE data message for item 'item' and data 'h'. 'InResponse'
// is TRUE if the data sent is in response to a DDE request message.
//
boolean VDdeServer::sndData(char *item, handle h, UINT inResponse)
{
	if (hWndRemote == 0) {
		return FALSE;
	}
	boolean result = TRUE;
	VAtom a(item);

	if (!h) {
		if (!PostMessage(hWndRemote, WM_DDE_DATA, (WPARAM)hWnd,
						(LPARAM) MAKELONG(h, (UINT)a) )) {
			!a;
		}
		return TRUE;
	}

	LPARAM l = PackDDElParam(WM_DDE_DATA, (UINT)h, (UINT)a);
	DDEDATA *data = (DDEDATA *)GlobalLock(h);
	data->fResponse = inResponse;
	data->fRelease = !acknowledge;
	data->fAckReq = acknowledge;
	GlobalUnlock(h);

	if (!PostMessage(hWndRemote, WM_DDE_DATA, (WPARAM)hWnd, l )) {
		FreeDDElParam(WM_DDE_DATA, l);
		!a;
		return FALSE;
	}
	if (acknowledge) {
		result = yieldAck(ackTimeout);
		FreeDDElParam(WM_DDE_DATA, l);
	}
	return result;
}

// ---------------------------------------------------------------------------
// Send the object 'obj' as the data associated with item 'item'
// (encoded in format 'format') to the client DDE, or to all client DDEs if
// this VDdeServer is the service request VDdeServer.
//
boolean VDdeServer::sendItem(char*item, VObject *obj, UINT format)
{
	if (!parent) {
		if (children) {
			DO(children, VDdeServer, s)
			s->sendItem(item, obj, format);
			END 
			return TRUE;
		}
		else {
			return FALSE;
		}
	}
	if (!idToData(obj, format)) {
		return FALSE;
	}
	return sndData(item, hData, pendingRequest);
}

// ---------------------------------------------------------------------------
// Enable the acknowledgment of data by this Server if 'ack' is TRUE. 
// Otherwise, don't acknowledge data.
//
void VDdeServer::ackData(boolean ack)
{
	acknowledge = ack;
}

// ---------------------------------------------------------------------------
// Adds item 'item' to the advisory set associated with this VDdeServer.
// If 'defer' is TRUE then a warm-link is indicated.
//
void VDdeServer::adviseOn(char*item, boolean defer)
{
	VObject *obj;
	VObject *p;
	if (!advise) {
		advise = new VSet;
	}
	if (obj = advise->remove(p = new VAdvise(item, defer, wFormat))) {
		delete obj;
	}
	advise->add(p);
}

// ---------------------------------------------------------------------------
// Removes item 'item' from the advisory set associated with this VDdeServer.
//
void VDdeServer::adviseOff(char*item)
{
	VObject *obj;
	VString s(item);
	if (!advise) {
		return;
	}
	if (obj = advise->remove(&s)) {
		delete obj;
	}
}

// ---------------------------------------------------------------------------
// Activate all links associated with 'item'. This method should only
// be called on the top-level VDdeServer.
//
void VDdeServer::updateItem(char*item)
{
	if (parent) {
		if (advise) {
			VString s(item);
			VAdvise *ap;
			if (ap = (VAdvise *)advise->find(&s)) {
				wFormat = ap->format();
				if (ap->defer()) {
					sndData(item, 0, 0);
				}
				else {
					callback(DdeRequest, item, 0);
				}
			}
		}
	}
	else if (children) {
		DO(children, VDdeServer, s)
		s->updateItem(item);
		END 
	}
}

// ---------------------------------------------------------------------------
// Closes all open connections if this is a top-level VDdeServer
// object, or otherwise, close only this connection.
//
boolean VDdeServer::disconnect()
{
	if (parent)  {
		delete this;
	}
	else if (children) {
		DO(children, VDdeServer, s)
			s->disconnect();
		END 
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Called when a DDE terminate message is received
// This implementation calls the destructor for a non-top-level
// VDdeServer object. 
//
void VDdeServer::rcvTerminate()
{
	if (parent) {
		VDde::rcvTerminate();
		if (!terminating) {
			delete this;
		}
	}
}

// ---------------------------------------------------------------------------
