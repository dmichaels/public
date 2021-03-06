// $Header:   Q:/views/win/vcs/MONEY.H_v   1.5   Mar 04 1997 14:13:10   gregm  $

//	money.h
//
//	VMethodData class interface
//
//	This class represents currency values. 
//	It supports internationalization.
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
//	02/06/96 dss	use quotes and not angle brackets.
// ----------------------------------------------------------------------------

#ifndef VMONEY_H
#define VMONEY_H

#include "oledefs.h"
#include "object.h"

extern VClass *VMoneyCls;

#if defined(CV_WIN16)
	// equate the 32 bit CURRENCY to the 16 bit identifier.
	typedef CY	CURRENCY;	
#endif

CLASS VMoney : public VObject
{
public:
	VMoney();

	VMoney& 		operator=(const VMoney& rhs);

	// Microsoft specific methods
	VMoney& 		operator=(const CURRENCY& rhs);
					operator CURRENCY();

	// Sanjay Start
	VMoney& 		operator=(double rhs);
                    operator double() const;
	// Sanjay End

	// Views specific
	VClass*			iam();
	virtual boolean	copyFrom(VObject *pObject);
#	ifndef CV_NOARCHIVER
    virtual void            getFrom(VArchiver& a);  //Override
    virtual void            putTo(VArchiver& a);    //Override
#	endif
private:
	// Give VVariable access to OLE specific routines, but no one else
	friend CLASS VVariable;

	// COM specific methods
	CURRENCY*				getCurrency();
	void					setCurrency(CURRENCY* currency);

	// Com specific members
	CURRENCY				m_currency;
};

inline CURRENCY* VMoney::getCurrency()
{
	return &m_currency;
}

inline void VMoney::setCurrency(CURRENCY* currency)
{
	m_currency = *currency;
}

inline VMoney& VMoney::operator=(const CURRENCY& rhs)
{
	m_currency = rhs;

	return *this;
}

inline VMoney::operator CURRENCY()
{
	return m_currency;
}

inline VMoney& VMoney::operator=(double rhs)
{
	VarCyFromR8(rhs, &m_currency);
	return *this;
}

inline VMoney::operator double() const
{
	double retVal;
	VarR8FromCy(m_currency, &retVal);
    return retVal;
}

#endif // VMONEY_H
