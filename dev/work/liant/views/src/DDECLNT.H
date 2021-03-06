// $Header:   Q:/views/win/vcs/ddeclnt.h_v   1.6   Mar 04 1997 16:32:10   gregm  $ 

//	ddeclnt.h
//
//	VDdeClient class interface
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
//	04/09/93 pmcm	changed id to VObject *
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/09/94 pmcm	STRICT: msg of user() is now UINT, not int
//	07/18/95 dgm	Miscellaneous const char * changes.
// ---------------------------------------------------------------------------

#ifndef VDDECLNT_H
#define VDDECLNT_H
#include "ddecls.h"

extern VClass *VDdeClientCls;

// Client States
typedef	enum {
	ClientIdle,
	ClientInitiating,
	ClientConnected,
	ClientExecuting

} DdeClientState;

//
// The VDdeClient class implements a MS Windows or OS/2 DDE client. It
// provides methods to initiate a session with a DDE server, send data, 
// establish warm and hot 'item' links, and execute commands on the server. 
// It uses the callback mechanism to notify its C++ client object
// when a hot-linked data item has been sent by the server (DdeItemData)
// or when a warm-linked data item has changed (DdeItemChange). 
//
CLASS VDdeClient : public VDde {
public:

			VDdeClient();
			~VDdeClient();

	VClass *iam();

	boolean connect(const char *, const char *);
	boolean coldLink(const char *);
	boolean warmLink(const char *, UINT format = CFtext);
	boolean hotLink(const char *, UINT format = CFtext);
	VObject *requestItem(const char *, UINT format = CFtext);
	boolean sendItem(const char *, VObject *, UINT format = CFtext);
	boolean execute(const char *);
	void	requestTime(UINT n);

protected:

	DdeClientState state;		// The VDde client state which is one of
								// enum DdeClientState: 
								// ClientIdle, ClientInitiating,
								// ClientConnected, ClientExecuting

	ushort	requestTimeout;		// Timeout for waiting for on a request. */
	HANDLE	hExec;

	boolean sndInitiate(const char *, const char *);
	boolean sndRequest(const char *, UINT);
	boolean sndAdvise(const char *, boolean, UINT);
	boolean sndUnadvise(const char *);
	boolean sndPoke(const char *, UINT);
	boolean sndExecute(const char *);

	virtual DdeResult 	rcvData(const char *, UINT, const char *, long, UINT);
	virtual void		rcvAck(UINT, UINT, UINT);
	virtual void		rcvTerminate();
	virtual boolean 	user(UINT msg, WPARAM wParam, LPARAM lParam);

};

// ---------------------------------------------------------------------------
// Set the timeout for item data requests to 'n' milliseconds
//
inline void VDdeClient::requestTime(UINT n)
{
	requestTimeout = n;
}

#endif // VDDECLNT_H
