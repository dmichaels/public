// $Header:   Q:/views/win/vcs/ddecls.h_v   1.6   Mar 04 1997 16:32:12   gregm  $ 

//	ddecls.h
//
//	VDde class interface
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
//	04/09/93 pmcm	setData() now takes a const char * not char *
//	08/09/93 jmd	removed free
//	09/26/93 jmd	preened
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//	03/09/94 pmcm	STRICT:
//					hWndRemote is now HWND, not handle
//					msg of user() is now UINT, not int
//	04/15/94 pmcm	preened.
//	06/22/94 jld	Redefined WindowsNT DDE message packing
//					functions for WIN16 use in ddeserv and ddeclnt.
//	03/02/95 jld	FreeDDElParam macro for WIN16 frees memory matching
//					WIN32 behavior
//	07/18/95 dgm	Miscellaneous const char * changes.
// ---------------------------------------------------------------------------

#ifndef VDDECLS_H
#define VDDECLS_H
#include "window.h"

extern VClass *VDdeCls;

#define CFtext 1 // clipboard text format

typedef UINT DdeResult;
const DdeResult DdeTimeout = 1;
const DdeResult DDEack = 0x8000;
const DdeResult DDEnak = 0;

#define DDEackTimeout 4000 // in milliseconds
#define DDErequestTimeout 10000 // in milliseconds

// VEvent reason notification codes
//
typedef enum { 
	// Client Events
	DdeItemData, DdeItemChange, 

	// Server Events
	DdeService, DdeLink, DdeUnlink, DdeRequest, DdePokeData, DdeExecute, 

	// Both Events
	DdeTerminate 

} DdeEvent;

// ---------------------------------------------------------------------------
//      WindowsNT DDE message packing macros redefined for use with WIN16 
//
#if defined(CV_WINDOWS) && !defined(CV_WIN32)

#define PackDDElParam(M,LO,HI) ((LPARAM) MAKELONG((LO),(HI)))

#define UnpackDDElParam(M,LP,PL,PH)             \
	{       (*PH) = HIWORD((LP));                   \
			(*PL) = LOWORD((LP)); }

#define FreeDDElParam(M,LP) GlobalFree((HGLOBAL)LOWORD((LP)))

#endif

// 
// The VDde class is an abstract class which implements the Dynamic
// Data Exchange (DDE) protocol provided in MS Windows and OS/2 common to both
// the server and client components. 
// Up on construction the VDde class creates an invisable VWindow which serves
// as the recipient and source for the DDE messages. 
// VDde objects are intended to have client objects. A single callback method
// is used for all callbacks and the calling VDde object is passed in as the
// only argument. The reason for the callback is obtained via the 'reason' member
// function. This function returns one of the enumerated type types of 
// enum DdeEvent. The VDde class generates only the 'DdeTerminate' callback. 
// The callback function should return TRUE if the VDde object accepts the
// operation defined by the callback otherwise it should return FALSE. 
// The remainder are generated in the subclasses of VDde. The 'DdeTerminate'
// callback is made when the remove VDde object has called its 'disconnect'
// method. The VDde class provides virtual methods for mapping between DDE
// message data handles and C++ class objects. The methods implemented
// in this class handle CF_TEXT and CF_OBJECT formats in the same
// manner as the VClipBoard class. These allow for the transfer of NULL
// terminated strings and archivable objects between applications. 
// The DDE protocol implemented by this VDde framework is synchronous, that
// is, calls to VDde objects 'block' until the atomic operations are complete.
// 

CLASS VDde : public VWindow {

public:
						VDde();

	VClass                     *iam();

	void                            ackTime(UINT n);
	VObject                    *dataItem();
	char                       *item();
	UINT                    format();
	DdeEvent                        reason();
	void                            uponEvent(VObject *, method);
	void                            setData(UINT, long, const char *);

	virtual boolean         disconnect();
	virtual VObject    *dataToId(UINT, const char *, long);
	virtual boolean         idToData(VObject *, UINT);

	static UINT     objectFormat();

protected:

	HWND            hWndRemote;             // Windows,OS/2 handle of remote VDde object
	handle          hData;                  // memory handle of the current VDde data
	VObject    *client;                     // client object that receives callbacks
	method          mthd;                   // client callback method
	DdeEvent        wReason;                // reason for the callback
	DdeResult       wResult;                // result of a wait for message
	char       *pItem;                      // name of the item received
	ushort          wFormat;                // format of the item received
	ushort          ackTimeout;             // timeout for waiting for an acknowledgement
	VObject    *pData;                      // received data converted to an object
	int                     pendingRequest; // TRUE if waiting for data object
	ushort          terminating;    // TRUE if waiting for termination message
	ushort          yielding;               // TRUE if waiting for a message

	boolean                         sndTerminate(boolean);
	boolean                         sndAck(UINT, UINT, int immed = 0);
	handle                          strToHandle(const char *);
	boolean                         callback(DdeEvent, char *, VObject *);
	void                            yieldEnd(UINT);
	boolean                         yieldAck(UINT);
	void                            getInfo(VWinInfo &);

	virtual void            rcvTerminate();
	virtual void            rcvAck(UINT, UINT, UINT);
	virtual boolean         user(UINT msg, WPARAM wParam, LPARAM lParam);
};

#define  CF_OBJECT      VDde::objectFormat()

// ---------------------------------------------------------------------------
// Sets the acknowledge timeout to 'n' milliseconds.
//
inline void VDde::ackTime(UINT n)
{
	ackTimeout = n;
}

// ---------------------------------------------------------------------------
// Returns the last object received by this VDde.
//
inline VObject *VDde::dataItem()
{
	return pData;
}

// ---------------------------------------------------------------------------
// Returns the name of the last object received by this VDde.
//
inline char *VDde::item()
{
	return pItem;
}

// ---------------------------------------------------------------------------
// Returns the MS Windows or OS/2 format type of the last object 
// received by this VDde.
//
inline UINT VDde::format()
{
	return wFormat;
}

// ---------------------------------------------------------------------------
// Returns the reason for this callback which is one of the enum
// DdeEvent types: 
// Client Events:
//              DdeItemData or DdeItemChange; 
// Server Events:
//              DdeService, DdeLink, DdeUnlink, DdeRequest, DdePokeData, and DdeExecute;
// Both Events:
//              DdeTerminate.
//
inline DdeEvent VDde::reason()
{
	return wReason;
}

// ---------------------------------------------------------------------------

#endif // VDDECLS_H
