// $Header:   Q:/views/win/vcs/serial.cpv   1.12   Mar 04 1997 17:55:48   gregm  $ 

//	serial.cpp
//
//	VSerial class implementation
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
//	04/07/93 pat	changed id to VObject *
//	08/09/93 jmd	removed free, made error take a const char *
//	08/10/93 pat	write() and putch() take const arg now
//	03/26/94 pat	preened.
//	07/07/94 jmd	moved inlines to serial.h
//	04/24/95 pkt	updated for internationalization.
//	06/01/95 pkt	merged international changes into 3.0.6.
//	09/18/95 pkt	fixed to set up 'charCount' and 'eleCount' upon return
//					from 'read' and 'write'.
//	09/18/95 pkt	minor changes to eliminate MSVC 1.5 warnings.
//	02/22/96 pkt	fixed 'inFlow' and 'outFlow' enabling/disabling.
//	07/12/96 glm	merged CV_WIN32 changes from 3.0.5
// ----------------------------------------------------------------------------

#include "winclass.h"
#include "serial.h"

extern "C" {
#	include <stdio.h>
#	include <string.h>
}

static const int MaxPort = 9; 	//	maximum number of ports
static const int TimeOut = 10;	//	timeout for polling timer

defineClass(VSerial, VStream)

// ----------------------------------------------------------------------------
// Construct a VSerial object with default input (receive) and output
// (transmit) buffer sizes of 1024 and 512, respectively.
//	
VSerial::VSerial() 
:	port(0), 
	iSize(1024), 
	oSize(512),
#if defined(CV_WIN32)
	dev((HANDLE) 0)
#else
	dev(-1)
#endif
{
	init();
}

// ----------------------------------------------------------------------------
// Construct a VSerial object on the given port number
// (1 for COM1, 2 for COM2, etc.).  
// The default input and output buffer sizes are set to
// 1024 and 512, respectively.
//
VSerial::VSerial(int portNum)
:	port(portNum), 
	iSize(1024), 
	oSize(512),
#if defined(CV_WIN32)
	dev((HANDLE) 0)
#else
	dev(-1)
#endif
{
	init();
}

// ----------------------------------------------------------------------------
VSerial::~VSerial()
{
	close();
	if (timer != 0) {
		delete timer;
	}
}

// ----------------------------------------------------------------------------
// Private member to initialize the event timer and the port reference array.
// Return TRUE if not initialized and return FALSE if already initialized.
//
boolean VSerial::init()
{
	rxSel = txSel = XInSel = XOutSel = errorSel = 0;
	timer = new VTimer(this, methodOf(VSerial, tick));
	return TRUE;
}

// ----------------------------------------------------------------------------
// Open this VSerial object using the port number specified when it was 
// constructed, or COMM 1 if a port number was not specified.
//
boolean VSerial::open()
{
	if (port < 1 || port > MaxPort) {
		port = 1;
	}

	return open(port);
}

// ----------------------------------------------------------------------------
// Open this VSerial on the given communications device port number
// (1 for COM1, 2 for COM2, etc.).
// Return TRUE if successful.  Return FALSE if the port is already open,
// the communications device is in use, or the communications device
// is not a valid device. 
//
boolean VSerial::open(int portNum)
{
	if (isValidDevice(dev) || portNum < 1 || portNum > MaxPort) {
		return FALSE;
	}

	port = portNum;
	char mode[90];
	strcpy(mode, "COM");
	itoa(port, &mode[3], 10);

#ifdef CV_WIN32
	dev	= CreateFile(	(LPCSTR) mode, 
						(DWORD) GENERIC_READ | GENERIC_WRITE, 
						(DWORD) 0,
						(LPSECURITY_ATTRIBUTES) 0, 
						(DWORD) OPEN_EXISTING,
						(DWORD) FILE_ATTRIBUTE_NORMAL,
						(HANDLE) 0);
#else
	dev = OpenComm((LPCSTR) mode, iSize, oSize);
#endif

	if (!isValidDevice(dev)) {
	 	return FALSE;
	}

	char  	buffer[82];
	strcat(mode, ":");
	GetProfileString("ports", (LPCSTR) mode, (LPCSTR) "9600,N,8,1", 
												(LPSTR) buffer, (DWORD) 80);
	strcat(mode, buffer);

	//	request current device information in device control block

	DCB  dcb;	
	if (!GetCommState(dev, &dcb)) {
		close();
		return FALSE;
	}

#if defined (CV_WIN32)

	BuildCommDCB((LPCSTR) mode, (LPDCB) &dcb);

	//	enable the DTR line on device open and leave it on
	dcb.fDtrControl		= DTR_CONTROL_ENABLE;
	//	enable the RTS line on device open and leave it on
	dcb.fRtsControl		= RTS_CONTROL_ENABLE;
	//	minimum number of characters in reception queue before sending XON
	dcb.XonLim 			= (WORD) iSize / 8;	
	//	maximum number of characters in reception queue before sending XOFF
	dcb.XoffLim 		= (WORD) iSize / 8;
	//	replacement for byte received with parity error
	dcb.ErrorChar 		= (char) 0;		
	dcb.fErrorChar 		= TRUE;			//	use ErrorChar on parity error

#else

	BYTE Id 			= dcb.Id;	//	device driver sets this id
	BuildCommDCB((LPCSTR) mode, (LPDCB) &dcb);
	dcb.Id 				= Id;
									//	max milliseconds to wait for:
	dcb.RlsTimeout 		= 0;		//		receive line signal (carrier detect)
	dcb.CtsTimeout		= 0;		//		clear to send
	dcb.DsrTimeout 		= 0;		//		data set ready
	dcb.fDtrflow 		= FALSE;	//	do not use DTR f/input flow control
	dcb.fDtrDisable 	= FALSE;	//	raise DTR when open/lower when closed
	dcb.fRtsflow 		= FALSE;	//	do not use RTS f/input flow control
	dcb.fRtsDisable 	= FALSE;	//	raise RTS when open/lower when closed
	//	minimum number of characters in reception queue before sending XON
	dcb.XonLim 			= (UINT) iSize / 8;
	//	maximum number of characters in reception queue before sending XOFF
	dcb.XoffLim 		= (UINT) iSize / 8;
	//	replacement for byte received with parity error
	dcb.PeChar 			= (char) 0;	
	dcb.TxDelay 		= (UINT) 0;		//	(not used)

	dcb.fChEvt 			= FALSE;	//	receipt of EvtChar is not an event

#endif

	dcb.fBinary 		= TRUE;		//	set binary mode
	dcb.fParity 		= FALSE;	//	disable parity checking

	dcb.fOutxCtsFlow 	= FALSE;	//	output flow control.do not use CTS
	dcb.fOutxDsrFlow 	= FALSE;	//	output flow control.do not use DSR
	dcb.fOutX 			= FALSE;	//	output flow control.do not use XON/XOFF
	dcb.fInX 			= TRUE;		//	input flow control.use XON/XOFF 

	dcb.fNull 			= TRUE;		//	discard received null characters

	dcb.EvtChar 		= (char) 0;		//	character to denote an event
	
	dcb.XonChar 		= (char) 17;	//	XON  char f/both input and output 
	dcb.XoffChar 		= (char) 19;	//	XOFF char f/both input and output
	dcb.EofChar 		= (char) 26;	//	character to denote end of data

	setDCB(dev, dcb);

#ifdef CV_WIN32
	SetCommMask(dev, EV_RXCHAR | EV_TXEMPTY);
#else
	SetCommEventMask(dev, EV_RXCHAR | EV_TXEMPTY);
#endif

	timer->start(TimeOut);

	return TRUE;
}

// ----------------------------------------------------------------------------
// Stop callbacks from this VSerial and release the communications port. 
// Return TRUE if successful; FALSE, otherwise
//
boolean VSerial::close()
{
	if (!isValidDevice(dev)) {
		error(CERR_NOTOPEN);
		return FALSE;
	}

#if defined(CV_WIN32)
	return CloseHandle(dev);
#else
	if (CloseComm(dev) == 0) {
		dev = -1;
		return TRUE;
	}
	return FALSE;
#endif
}

// ----------------------------------------------------------------------------
// Discard the contents of the receive buffer, transmit buffer, 
// or both if which is 0, 1 or 2, respectively. 
//
void VSerial::flush(int which)
{
	if (!isValidDevice(dev)) {
		error(CERR_NOTOPEN);
		return;
	}

	if (which == 0 || which == 2) {
#ifdef CV_WIN32
		//	clear all characters from the input buffer
		PurgeComm(dev, PURGE_RXCLEAR);
#else
		//	flush the input buffer
		FlushComm(dev, 1);
#endif
	}

	if (which == 1 || which == 2) {
		//	flush the transmit buffer
#ifdef CV_WIN32
		FlushFileBuffers(dev);	//	(asynchronous)
#else
		FlushComm(dev, 0);		
#endif
	}
}

// ----------------------------------------------------------------------------
// Read up to n bytes from the receive buffer into s.  Return
// the actual number of bytes read. 
//
integer VSerial::read(char *s, integer n)
{
	int stat = 0;

	if (!isValidDevice(dev)) {
		error(CERR_NOTOPEN);
		return 0;
	}
		
#ifdef CV_WIN32
	DWORD dwRead;
	if (ReadFile(dev,
				(LPVOID) s,
				(DWORD) n,
				&dwRead, 
				(LPOVERLAPPED) 0)) {
		stat = integer(dwRead);
	}
	else {
	 	error();
	}
#else
	if ((stat = ReadComm(dev, (VOID FAR *) s, (int) n)) <= 0) {
		error();
		stat = abs(stat);
	}
#endif

	eleCount = stat;
	charCount = VWideCharAux::mbslen(s, stat);

	return stat;
}

#ifdef CV_WCHAR
// ----------------------------------------------------------------------------
// Read up to n characters from the receive buffer into s.  Return
// the actual number of characters read. 
//
integer VSerial::read(VWideChar *s, integer n)
{
	charCount = 0;
	eleCount = 0;
	if (dev < 0) {
		error(CERR_NOTOPEN);
		return 0;
	}

	char *buf = new char[n * CV_MB_CUR_MAX];
	if (buf == 0) {
		error();
		return 0;
	}
	integer n_complete = 0;	 // number of wide chars obtained
	int stat = 0;
	integer buf_counter = 0; // number of bytes stuffed into the buffer
	integer buf_done = 0;	 // number of bytes translated to wide chars
	boolean err = FALSE;
	while (n_complete < n) {

#if defined(CV_WIN32)
		DWORD dwRead;
		if (ReadFile(dev,
					(LPVOID)(buf + buf_counter),
					(DWORD)(n - n_complete),
					&dwRead,
					(LPOVERLAPPED) 0 )) {
			stat = integer(dwRead);
		}
		else {
			err = TRUE;
	 		error();
		}
#else
		if ((stat = ReadComm(dev,
							(LPSTR)(buf + buf_counter),
							(unsigned)(n - n_complete))) <= 0) {
			err = TRUE;
			stat = abs(stat);
		}
#endif
		if (err) {
			break;
		}
		buf_counter += stat;
 		while (n_complete < n && buf_done < buf_counter) {
			int count = VWideCharAux::mbtowc(s + n_complete,
			                                 buf + buf_done,
			                                 size_t(buf_counter - buf_done));
			if (count < 0 && (buf_counter - buf_done) < CV_MB_CUR_MAX) {
				// It appears that the conversion failed on account
				// of an incomplete multibyte character.
				break;
			}
			else if (count <= 0) {
				// Treat illegal and null characters as 1 byte chars.
				count = 1;
			}
			n_complete += 1;
			buf_done += count;
		}
	}
	delete [] buf;

	if (err) {
		error();
		charCount = stat;
		eleCount = stat;
		return stat;
	}
	charCount = n_complete;
	eleCount = n_complete;
	return n_complete;
}
#endif

// ----------------------------------------------------------------------------
// Return the next byte from the receive buffer.  Return a -1 if
// there is an error or if no bytes are available.
//
int VSerial::getch()
{
	char c;
	if (!read(&c, 1)) {
		return -1;
	}
	return c;
}

// ----------------------------------------------------------------------------
// Set ch to the next character from the receive buffer.  Return a FALSE if
// there is an error or if no characters are available.
//
boolean VSerial::getch(VWideChar& ch)
{
	return read(&ch, 1) == 1;
}

// ----------------------------------------------------------------------------
// Write upto n bytes from s into the transmit buffer.  Return the
// number of bytes actually written.
//
integer VSerial::write(const char *s, integer n)
{
	int stat = 0;

	if (!isValidDevice(dev)) {
		error(CERR_NOTOPEN);
		return 0;
	}
		

#ifdef CV_WIN32
	DWORD dwWritten;
	if (WriteFile(dev, (LPCVOID) s, (DWORD) n, &dwWritten, 
		(LPOVERLAPPED) 0)) {

		stat = int(dwWritten);
	}
	else {
	 	error();
	}
#else
	if ((stat = WriteComm(dev, (const void FAR *)s, (int) n)) <= 0) {
		error();
		stat = abs(stat);
	}
#endif

	eleCount = stat;
	charCount = VWideCharAux::mbslen(s, stat);

	return stat;
}

// ----------------------------------------------------------------------------
// Write the string s into the transmit buffer.  Return the
// number of elements actually written.  This simply forwards 
// VStream::write.
//
integer VSerial::write(const VString& s)
{
	return VStream::write(s);
}

#ifdef CV_WCHAR
// ----------------------------------------------------------------------------
// Write upto n characters from s into the transmit buffer.  Return the
// number of characters actually written.
//
integer VSerial::write(const VWideChar *s, integer n)
{
	charCount = 0;
	eleCount = 0;
	if (dev < 0) {
		error(CERR_NOTOPEN);
		return 0;
	}
	
	// Allocate a buffer of char to hold a multibyte string translation
	// of the given wide characters.	
	int stat = 0;
	char *buf = new char[n * CV_MB_CUR_MAX];
	if (buf == 0) {
		error();
		return 0;
	}

	// Translate the wide character string to multibyte.  Don't use
	// mbstowcs because we can't just give up or stop when the wide
	// character is illegal or null.
	char *bufp = buf;
	const VWideChar *pwcs = s;
	size_t modified_count = 0;
	size_t read_count = 0;
	while (read_count < (size_t)n) {
		VWideChar wc = *pwcs;
		int count = wctomb(bufp, wc);
		if (count < 0) {
			count = 1;
		}
		pwcs += 1;
		bufp += count;
		read_count += 1;
		modified_count += count;
	}

#if defined(CV_WIN32)
	DWORD dwWritten;
	if (WriteFile(dev, (LPCVOID) s, (DWORD) n, &dwWritten, 
		(LPOVERLAPPED) 0)) {

		stat = int(dwWritten);
	}
	else {
	 	error();
	}
#else
	stat = WriteComm(dev, (pointer)buf, (unsigned)modified_count);
#endif
	if (stat <= 0) {
		error();
		stat = abs(stat);
	}
	charCount = VWideCharAux::mbslen(buf, stat);
	eleCount = charCount;
	delete buf;
	return charCount;
}
#endif

// ----------------------------------------------------------------------------
// Write the byte ch into the transmit buffer. Return TRUE if successful, 
// otherwise return FALSE.
//
boolean VSerial::putch(const char ch)
{
	return write(&ch, 1) == 1;
}

#ifdef CV_WCHAR
// ----------------------------------------------------------------------------
// Write the character ch into the transmit buffer. Return TRUE if 
// successful, otherwise return FALSE.
//
boolean VSerial::putch(const VWideChar ch)
{
	return write(&ch, 1) == 1;
}
#endif

// ----------------------------------------------------------------------------
// Return the number of characters in the receive buffer. 
//
unsigned VSerial::inCount()
{
	if (!isValidDevice(dev)) {
		error(CERR_NOTOPEN);
		return FALSE;
	}

	COMSTAT comstat;

#if defined(CV_WIN32)
	DWORD errors;
	ClearCommError(dev, &errors, &comstat);
#else
	error(GetCommError(dev, &comstat));
#endif

	return unsigned(comstat.cbInQue);
}

// ----------------------------------------------------------------------------
// Event notification indicating that the receive buffer is holding n
// bytes of data.  If a callback method selector has been registered for this
// event, then notify the client object that n characters are waiting to be
// read.
//
boolean VSerial::inCount(int n)
{
	if (client && rxSel != NIL_METHOD) {
		client->perform(rxSel, (long)n);
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Return the number of characters in the transmit buffer. 
//
unsigned VSerial::outCount()
{
	COMSTAT comstat;

	if (isValidDevice(dev)) {
#ifdef CV_WIN32
		DWORD errors;
		ClearCommError(dev, &errors, &comstat);
#else
		error(GetCommError(dev, &comstat));
#endif
		return comstat.cbOutQue;
	}
	else {
		error(CERR_NOTOPEN);
		return 0;
	}
}

// ----------------------------------------------------------------------------
// Event notification indicating that the output (transmit) buffer has
// become empty.  If a callback method has been registered, then notify the
// client object of this event.   
//
boolean VSerial::outEmpty()
{
	if (client && txSel != NIL_METHOD) {
		client->perform(txSel);
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Retrieve into *dataBits, *stopBits, *parity, and *baudRate
// the number of data bits, stop bits, parity setting, and baud rate, respectively.
//
boolean VSerial::device(int *dataBits, int *stopBits, char *parity, int *baudRate)
{
	DCB dcb;
	getDCB(dev, dcb);
	if (parity) {
		if (!dcb.fParity) {
			*parity = 'N';
		}
		else { 
			switch (dcb.Parity) {
			case EVENPARITY:   	// even 
				*parity = 'E';
				break;
			case ODDPARITY:   	// odd
				*parity = 'O';
				break;
			case NOPARITY:   	// none 
				*parity = 'N';
				break;
			case MARKPARITY:   	// mark
				*parity = 'M';
				break;
			case SPACEPARITY:   // space
				*parity = 'S';
				break;
			default:
				break;
			}
		}
	}
	if (stopBits) {
		if (dcb.StopBits == ONESTOPBIT) {
			*stopBits = 1;
		}
		if (dcb.StopBits == TWOSTOPBITS) {
			*stopBits = 2;
		}
	}

	if (baudRate) {
		*baudRate = dcb.BaudRate;
	}

	if (dataBits) {
		*dataBits = dcb.ByteSize;
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Set the number of data bits, stop bits, parity setting, and baud
// rate of the communication device associated with this VSerial to dataBits, 
// stopBits, parity, and baudRate, respectively.  Parity is one of: 
// N - none, E - even, O - odd, M - mark, or S - space.  If N is
// given, then no parity checking is performed.
//
boolean VSerial::device(int dataBits, int stopBits, char parity, int baudRate)
{
	DCB dcb;
	getDCB(dev, dcb);
	dcb.fParity = 1;

	switch (parity) {
	case 'E':   //	even 
		dcb.Parity = EVENPARITY;
		break;
	case 'O':   //	odd 
		dcb.Parity = ODDPARITY;
		break;
	case 'N':   //	none 
		dcb.Parity = NOPARITY;
		dcb.fParity = 0;
		break;
	case 'M':   //	mark 
		dcb.Parity = MARKPARITY;
		break;
	case 'S':   //	space 
		dcb.Parity = SPACEPARITY;
		break;
	default:
		break;
	}

	dcb.ByteSize = dataBits;
	dcb.BaudRate = baudRate;

	if (stopBits == 1) {
		dcb.StopBits = ONESTOPBIT;
	}
	else if (stopBits == 2) {
		dcb.StopBits = TWOSTOPBITS;
	}
	return setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// Set the client of this VSerial object to clnt. The client object
// will receive all callback messages that are enabled. 
//
void VSerial::setClient(VObject *clnt)
{
	client = clnt;
}

// ----------------------------------------------------------------------------
// Arrange to call mthd on the client object when a communications
// error occurs.  The single argument of this callback is one of the following:
//
// CERR_RXOVERRUN		0x0002       Receive queue overrun
// CERR_RXPARITY		0x0004       Receive queue parity
// CERR_RXFRAMING		0x0008       Receive queue framing
// CERR_RXBREAK		0x0010       Receive queue break
//
// CERR_CTSTIMEOUT		0x0020       CTS Timeout
// CERR_DSRTIMEOUT		0x0040       DSR Timeout
// CERR_CDTIMEOUT		0x0080       CD  Timeout
//
// CERR_RXOVERFLOW		0x0001       Receive queue overflow
// CERR_TXQUEUEFULL	0x0100       Transmit queue full
//
// CERR_NOTOPEN		0x4000       VPort not open
// CERR_INVALIDMODE	0x8000       Unsupported communications mode
//
void VSerial::uponError(method mthd)
{
	errorSel = mthd;
}

// ----------------------------------------------------------------------------
// If read is non-zero, then arrange to call the method read of
// the client object when there are some characters in the receive buffer to
// be read.  If empty is non-zero, then arrange to call the method empty
// of the client object when the transmit buffer is empty.
//
void VSerial::uponCommReady(method read, method empty)
{
	rxSel = read;
	txSel = empty;
}

// ----------------------------------------------------------------------------
// If enable is TRUE, then pass back any received ASCII NUL characters, 
// otherwise discard them.
//
void VSerial::receiveNull(boolean enable)
{
	DCB dcb;
	getDCB(dev, dcb);
	dcb.fNull = enable ? 0 : 1;
	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// Return TRUE if the data-terminal-ready (DTR) line is on,
// otherwise return FALSE.
//
boolean VSerial::dtr()
{
#if defined(CV_WIN32)
	return TRUE;
#else
	DCB dcb;
	getDCB(dev, dcb);
	return dcb.fDtrDisable == 0 ? TRUE: FALSE;
#endif
}

// ----------------------------------------------------------------------------
// If enable is TRUE, then assert the data-terminal-ready (DTR)
// signal, otherwise negate it.
//
void VSerial::dtr(boolean enable)
{
	DCB dcb;
	getDCB(dev, dcb);

#if defined(CV_WIN32)
	dcb.fDtrControl = enable ? 0 : 1;
#else
	dcb.fDtrDisable = enable ? 0 : 1;
#endif
	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// Return TRUE if the request-to-send (RTS) line is on,
// otherwise return FALSE.
//
boolean VSerial::rts()
{
#if defined(CV_WIN32)
	return TRUE;
#else
	DCB dcb;
	getDCB(dev, dcb);
	return dcb.fRtsDisable == 0;
#endif
}

// ----------------------------------------------------------------------------
// If enable is TRUE, then assert the request-to-send (RTS) signal, 
// otherwise negate it.
//
void VSerial::rts(boolean enable)
{
	DCB dcb;
	getDCB(dev, dcb);
#if defined(CV_WIN32)
	dcb.fRtsControl = enable ? 0 : 1;
#else
	dcb.fRtsDisable = enable ? 0 : 1;
#endif
	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// If timeout is non-zero, then wait timeout milliseconds for
// a carrier-detect (CD) signal before transmitting. 
//
void VSerial::cdWait(int timeout)
{
	//	!!!
#if !defined(CV_WIN32)
	DCB dcb;
	getDCB(dev, dcb);
	dcb.RlsTimeout = timeout;
	setDCB(dev, dcb);
#endif
}

// ----------------------------------------------------------------------------
// If timeout is non-zero, then wait timeout milliseconds for
// a clear-to-send (CTS) signal before transmitting. 
//
void VSerial::ctsWait(int timeout)
{
#if !defined(CV_WIN32)
	DCB dcb;
	getDCB(dev, dcb);

	if (timeout) {
		dcb.fOutxCtsFlow = 1;
		dcb.CtsTimeout = timeout;
	}
	else {
		dcb.fOutxCtsFlow = dcb.RlsTimeout = 0;
	}

	setDCB(dev, dcb);
#endif
}

// ----------------------------------------------------------------------------
// If timeout is non-zero, then wait timeout milliseconds for
// a data-setReady (DSR) signal before transmitting. 
//
void VSerial::dsrWait(int timeout)
{
#if !defined(CV_WIN32)
	DCB dcb;
	getDCB(dev, dcb);

	if (timeout) {
		dcb.fOutxDsrFlow = 1;
		dcb.DsrTimeout = timeout;
	}
	else {
		dcb.fOutxDsrFlow = dcb.RlsTimeout = 0;
	}

	setDCB(dev, dcb);
#endif
}

// ----------------------------------------------------------------------------
// Set the XON and XOFF characters to on and off, respectively. 
// Stop the flow when hiWater characters have been received but not read. 
// Start the flow when lowWater characters have been received but not read
//
void VSerial::dataFlow(char on, char off, int hiWater, int lowWater)
{
	DCB dcb;
	getDCB(dev, dcb);

	dcb.XonChar 	= on;
	dcb.XoffChar 	= off;
#if defined(CV_WIN32) 
	dcb.XonLim 		= (WORD) lowWater;
	dcb.XoffLim 	= (WORD) hiWater;
#else
	dcb.XonLim 		= (UINT) lowWater;
	dcb.XoffLim 	= (UINT) hiWater;
#endif

	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// If enable is TRUE, then enable transmit flow control, otherwise
// disable it.  If mthd is non-zero, then that method will be called
// on the client object when the output flow is stopped or started.
//
void VSerial::outFlow(boolean enable, method mthd)
{
	XOutSel = mthd;

	DCB dcb;
	getDCB(dev, dcb);

	dcb.fOutX = enable ? 1 : 0;

	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// If enable is TRUE, then enable receive flow control, otherwise
// disable it.  If mthd is non-zero, then that method will be called
// on the client object when the input flow is stopped or started.
//
void VSerial::inFlow(boolean enable, method mthd)
{
	XInSel = mthd;

	DCB dcb;
	getDCB(dev, dcb);

	dcb.fInX = enable ? 1 : 0;

	setDCB(dev, dcb);
}

// ----------------------------------------------------------------------------
// This method is called periodically by the VTimer object to process 
// communication port events.
//
boolean VSerial::tick(VTimer *t)
{
	COMSTAT comstat;

	if (isValidDevice(dev)) {
		t->pause();

#if defined(CV_WIN32)
		//	(retrieves error status, then clears device error flag)
		DWORD errors;
		ClearCommError(dev, &errors, &comstat);
#else
		//	(retrieves events, then clear RXCHAR and TXEMPTY event errors)
		UINT event = GetCommEventMask(dev, EV_RXCHAR | EV_TXEMPTY);
		error(GetCommError(dev, &comstat));
#endif

		if (comstat.cbInQue > 0) {
			inCount(int(comstat.cbInQue));	
		}

#if defined(CV_WIN32)
		if (comstat.cbOutQue == 0)
//		if (comstat.fTxim)
#else
		if (event & EV_TXEMPTY) 
#endif
		{
			//	notify client that the output buffer is empty
			outEmpty();
		}

		t->resume();
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Set the default receive buffer and transmit buffer to in and
// out, repectively
//
void VSerial::bufferBy(int in, int out)
{
	iSize = in;
	oSize = out;
}

// ----------------------------------------------------------------------------
// Report the error through the client errorSel callback.
//
boolean VSerial::error(int err)
{
	err &= 0x81FF;
	if (err && client && errorSel != NIL_METHOD) {
		client->perform(errorSel, (long)err);
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
// Print the error message s.
//
void VSerial::error(const char *s)
{
	VObject::error(s);
}

// ----------------------------------------------------------------------------
// A communications error has occurred.  Retrieve the error and report it 
// to the user.  This method is called by read() and write().   
//
boolean VSerial::error()
{
	if (isValidDevice(dev)) {
#ifdef CV_WIN32
		COMSTAT comstat;
		DWORD errors;
		ClearCommError(dev, &errors, &comstat);
#else
		error(GetCommError(dev, (COMSTAT *)0));
#endif
	}
	else {
		error(CERR_NOTOPEN);
	}
	return TRUE;
}

// ----------------------------------------------------------------------------
