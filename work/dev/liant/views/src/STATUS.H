// $Header:   Q:/views/win/vcs/STATUS.H_v   1.6   Mar 04 1997 14:13:54   gregm  $

//	status.h
//
//	VStatus class interface
//
//	Description of file:  The purpose of this class is generalize status
//	codes.  To accomplish this, the implementation of this class uses very
//	specific methods to detect success or failure.  Since the specifics are
//	encapsulated inside the class, the client is able to determine the state
//	of the status code in a platform and technology independent way.
// 
//	This class could serve as an abstract base class that defines status code
//	protocol.  I did not take the time to do this and currently, this class
//	encapsulates the notion of a OLE SCODE.
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
//	01/23/96 rhw	Initial Revision.
//	01/26/96 dss	Name change: VComponentProgrammable.
//	02/06/96 dss	use quotes and not angle brackets.
// ----------------------------------------------------------------------------

#ifndef VSTATUS_H
#define VSTATUS_H

#include "oledefs.h"

#include "object.h"

extern VClass *VStatusCls;

CLASS VStatus : public VObject
{
public:
	VStatus();
	VStatus(const VStatus& status);
	VStatus(const long statusCode);

	// Returns TRUE, if this status was set to a successful status code
	boolean			isSuccess();
	// Returns FALSE, if this status was set to a successful status code
	boolean			isFailure();

	// Returns a error message that describes the failure
	VString			getErrorMsg();

	VStatus& 		operator=(const VStatus& rhs);

					// OLE Methods return HRESULTS which are ptrs on WIN16
					// and longs on WIN32.
					// This operator will take care of this case.
	VStatus&		operator=(HRESULT r);

#if defined(CV_WIN16)					
					// On Win16, SCODEs are a different type than HRESULTs.
	VStatus&		operator=(SCODE sc);
#endif					

	// ----------- Overridden Views framework methods -----------
	// Required meta-method
	virtual VClass*	iam();
	// Framework equivalent to the assignment operator
	virtual boolean	copyFrom(VObject *pObject);
#	ifndef CV_NOARCHIVER
	virtual void	getFrom(VArchiver& a);  //Override
	virtual void	putTo(VArchiver& a);    //Override
#	endif

private:
	// Give VProg and VVariable access to OLE specific routines, but no one else
	friend CLASS VVariable;
	friend CLASS VComponentProgrammable;

	SCODE			getStatusCode();
	void			setStatusCode(SCODE statusCode);
					operator SCODE ();

	SCODE			m_statusCode;		// Holds the native status code

};

inline SCODE VStatus::getStatusCode()
{
	return m_statusCode;
}

inline void VStatus::setStatusCode(SCODE  statusCode)
{
	m_statusCode = statusCode;
}

inline boolean VStatus::isSuccess()
{
	return SUCCEEDED(m_statusCode);
}

inline boolean	VStatus::isFailure()
{
	return FAILED(m_statusCode);
}

inline VStatus::operator SCODE()
{
	return m_statusCode;
}

// OLE Methods return HRESULTS which are ptrs on WIN16
// and longs on WIN32.
// This operator will take care of this case.
inline VStatus& VStatus::operator=(HRESULT r) 
{
	m_statusCode = GetScode(r); 
	return *this;
}

#if defined(CV_WIN16)					
// On Win16, SCODEs are a different type than HRESULTs.
inline VStatus& VStatus::operator=(SCODE sc) 
{
	m_statusCode = sc; 
	return *this;
}
#endif

#endif // VSTATUS_H
