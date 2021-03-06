// $Header:   Q:/views/win/vcs/ddeserv.h_v   1.5   Mar 04 1997 16:32:14   gregm  $ 

//	ddeserv.h
//
//	VDdeServer class interface
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
// ---------------------------------------------------------------------------

#ifndef VDDESERV_H
#define VDDESERV_H
#include "ddecls.h"

CLASS VSet;

extern VClass *VDdeServerCls;

// The VDdeServer class implements an MS Windows or OS/2 DDE server. 
// It operates in two modes: as an object that receives service requests from
// DDE clients (top-level) and as a server for a single DDE client
// (non-top-level). It provides methods to send item data to the client and
// to notify the client DDE that an item has changed. The VDde callback
// mechanism is used to notify its client C++ object (NOT the DDE client) that
// a DDE client wishes to obtain service (DdeService), warm or hot link a
// given item (DdeLink), cold link a given item (DdeUnlink), wants the data
// associated with a given item (DdeRequest), send data (DdePokeData) or
// execute a command (DdeExecute). 
// As with the VDde class, the callback function must return TRUE or FALSE
// depending on whether the operation is accepted or rejected. 
// When a connection request is accepted, then a separate VDdeServer object is
// constructed to handle the iteraction with the remove DDE client. This object
// is destroyed when the connection is terminated either by the server or client
// application. All instances of the VDdeServer class will share the same C++
// client object, therefore the callback routine should call methods on the
// VDdeServer object passed to it, not to the VDdeServer object that handles
// the connection requests.

CLASS VDdeServer : public VDde {
public:
			VDdeServer();
			~VDdeServer();
	VClass *iam();

	char *topic();
	char *application();
	char *command();

	virtual boolean 	disconnect();
	void				updateItem(char *);
	boolean 			sendItem(char *, VObject *, UINT format = CF_TEXT);
	void				ackData(boolean);

protected:
	char *pTopic;		// The name of the topic requested by the DDE client
	char *pAppl;		// The name of the application requested by the DDE client
	char *pCommand;		// The name of the command executed by the DDE client
	ushort acknowledge;	// TRUE if the server should request that data be acknowledged
	VSet *advise;		// The set of warm or hot linked items

	boolean 			sndData(char *, handle, UINT);
	virtual DdeResult 	rcvInitiate(char *, char *, handle);
	virtual DdeResult 	rcvUnadvise(char *);
	virtual DdeResult 	rcvRequest(char *, UINT);
	virtual DdeResult 	rcvAdvise(char *, UINT, UINT);
	virtual DdeResult 	rcvExecute(char *cmd);
	virtual DdeResult 	rcvPoke(char *, UINT, char *, long);
	void				serve(VDdeServer *);
	void				adviseOn(char *, boolean);
	void				adviseOff(char *);

	virtual void		rcvTerminate();
	virtual boolean 	user(UINT msg, WPARAM wParam, LPARAM lParam);

};

// ---------------------------------------------------------------------------
// Return the topic requested by the client DDE (upon a DdeService callback).
//
inline char *VDdeServer::topic()
{
	return pTopic;
}

// ---------------------------------------------------------------------------
// Return the application name requested by the client DDE (upon a
// DdeService callback).
//
inline char *VDdeServer::application()
{
	return pAppl;
}

// ---------------------------------------------------------------------------
// Return the command to be executed (upon a DdeExecute callback).
//
inline char *VDdeServer::command()
{
	return pCommand;
}

#endif // VDDESERV_H
