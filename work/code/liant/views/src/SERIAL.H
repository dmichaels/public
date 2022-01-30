// $Header:   Q:/views/win/vcs/serial.h_v   1.8   Mar 04 1997 16:33:04   gregm  $ 

//	serial.h
//
//	VSerial interface
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
//	04/09/93 pat	changed id to VObject *
//	05/17/93 pat	merged MOTIF and MS-Windows versions
//					!!!	MOTIF has sigPoll(), lacks tick() and timer member
//	08/09/93 jmd	removed free, made error() take const char *
//	08/10/93 pat	write() and putch() take const arg now
//	03/26/94 pat	preened
//	07/07/94 jmd	added inline functions
//	04/24/95 pkt	updated for internationalization.
//	06/01/95 pkt	merged international changes into 3.0.6.
//	07/12/96 glm	merged CV_WIN32 changes from 3.0.5
// ----------------------------------------------------------------------------

#ifndef VSERIAL_H
#define VSERIAL_H

#include "strm.h"

#include "control.h"
#include "timer.h"

extern VClass *VSerialCls;

//	The VSerial class allows buffered input and output with the 
//	computer's asynchronous ports.  The VSerial class implements 
//	error handling, flow control, hardware handshaking, and 
//	serial port configuration.  Instances of the VSerial class 
//	can be used to perform both polled or interrupt driven 
//	serial I/O.  VSerial objects employ the concept of a client 
//	object with callback messages to support asynchronous, interrupt 
//	driven communications.  Client objects can be any type 
//	of C++/Views objects and callback methods can be specified for 
//	receive (input) events, transmit (output) events, or both. 
//	A VSerial object will fail to open on a particular serial port 
//	if it is already in use.

CLASS VSerial : public VStream {

public:

				VSerial();
				VSerial(int portNumber);
				~VSerial();

	VClass 	   *iam();


	//	Operations

	boolean 	open();
	boolean 	open(int portNumber);
	boolean 	close();
	void 		flush(int whichBuffer);	//	receive=0,transmit=1,both=2

	//	Reading and writing

	integer 	read(char *s, integer n);
#ifdef CV_WCHAR
	integer		read(VWideChar *, integer);
#endif
	int 		getch();
	boolean		getch(VWideChar&);

	integer 	write(const char *s, integer n);
	integer		write(const VString&);
#ifdef CV_WCHAR
	integer		write(const VWideChar *, integer);
#endif
	boolean 	putch(const char c);
#ifdef CV_WCHAR
	boolean		putch(const VWideChar);
#endif

	//	Buffer status

	unsigned 	inCount();
	boolean 	inCount(int);

	unsigned 	outCount();
	boolean 	outEmpty();
	
	//	Device characteristics

	boolean 	device(	int    *dataBits, 
						int    *stopBits, 
						char   *parity, 
						int	   *baudRate);

	boolean 	device(	int 	dataBits, 
						int 	stopBits, 
						char 	parity, 		
						int 	baudRate);

	//	Callbacks

	void 		setClient(VObject *theClient);
	void 		uponError(method errorMthd);
	void 		uponCommReady(method readyMthd, method emptyMthd);
	void 		receiveNull(boolean enable);

	//	Device signal interrogation and setting

	boolean 	dtr();
	void 		dtr(boolean enable);
	boolean 	rts();
	void 		rts(boolean enable);
	void 		cdWait(int timeout);
	void 		ctsWait(int timeout);
	void 		dsrWait(int timeout);

	//	Flow control

	void 		dataFlow(char on, char off, int hiWater, int loWater);
	void 		outFlow(boolean enable, method outFlowMthd);
	void 		inFlow(boolean enable, method inFlowMthd);

	boolean 	tick(VTimer *t);
	void 		bufferBy(int in, int out);
	boolean 	error();
	boolean 	error(int errNumber);
	void 		error(const char *errMsg);


protected:

#if defined(CV_WIN32)
	HANDLE 		dev; 			//	system device identifier
#else
	int 		dev; 			//	system device identifier
#endif
	int 		port;			//	comm port identifier
	int 		iSize;			//	receive buffer size
	int 		oSize;			//	transmit buffer size
	VObject    *client; 		//	client object

	method 		errorSel;		//	error callback
	method 		XInSel;      	//	receive XON/XOFF callback
	method 		XOutSel;      	//	transmit XON/XOFF callback
	method 		txSel;       	//	transmit buffer not empty callback
	method 		rxSel;       	//	receive buffer not empty callback

	VTimer 	   *timer;			//	timer for asynch communication

	boolean 	init();

private:
#if defined(CV_WIN32) 
	boolean		isValidDevice(HANDLE dev);
	boolean		setDCB(HANDLE dev, DCB& dcb);
	boolean		getDCB(HANDLE dev, DCB& dcb);
#else
	boolean		isValidDevice(int dev);
	boolean		setDCB(int dev, DCB& dcb);
	boolean		getDCB(int dev, DCB& dcb);
#endif
};

#endif // VSERIAL_H

// ---------------------------------------------------------------------------

#define CERR_RXOVERRUN    0x0002      // Receive queue overrun 
#define CERR_RXPARITY     0x0004      // Receive queue parity 
#define CERR_RXFRAMING    0x0008      // Receive queue framing 
#define CERR_RXBREAK      0x0010      // Receive queue break 

#define CERR_CTSTIMEOUT   0x0020      // CTS Timeout 
#define CERR_DSRTIMEOUT   0x0040      // DSR Timeout 
#define CERR_CDTIMEOUT    0x0080      // CD  Timeout 

#define CERR_RXOVERFLOW   0x0001      // Receive queue overflow 
#define CERR_TXQUEUEFULL  0x0100      // Transmit queue full 

#define CERR_NOTOPEN      0x4000      // VPort not open 
#define CERR_INVALIDMODE  0x8000      // Unsupported communications mode 

// ---------------------------------------------------------------------------
#if defined(CV_WIN32) 

inline boolean VSerial::isValidDevice(HANDLE dev) 
{
 	return dev != (HANDLE) 0;
}

inline boolean VSerial::setDCB(HANDLE dev, DCB& dcb)
{
	return (boolean) SetCommState(dev, &dcb);
}

inline boolean VSerial::getDCB(HANDLE dev, DCB& dcb)
{
	return (isValidDevice(dev)) ? GetCommState(dev, &dcb) : error(CERR_NOTOPEN);
}

#else

inline boolean VSerial::isValidDevice(int dev) 
{
 	return dev >= 0;
}

inline boolean VSerial::setDCB(int dev, DCB& dcb)
{
	dev = dev;
	return SetCommState(&dcb) == 0;
}

inline boolean VSerial::getDCB(int dev, DCB& dcb)
{
	return (isValidDevice(dev)) ? GetCommState(dev, &dcb) : error(CERR_NOTOPEN);
}

#endif	//	!CV_WIN32
