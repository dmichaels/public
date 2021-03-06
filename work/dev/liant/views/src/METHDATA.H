// $Header:   Q:/views/win/vcs/methdata.h_v   1.8   Jun 03 1997 09:51:48   scottt  $

//	methdata.h
//
//	VMethodData class interface
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
//	Copyright (c) 1997 by INTERSOLV, Inc.
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
//
// ----------------------------------------------------------------------------

#ifndef VMETHODDATA_H
#define VMETHODDATA_H

#include "object.h"
#include "ordcllct.h"

#include "status.h"
#include "variable.h"

extern VClass *VMethodDataCls;

//-----------------------------------------------------------------------------
// This class defines technology independent definition of method related
// information.  (Currently, its is a little slanted toward MS COM, but we
// put all the COM stuff in the private section.  When we figure out just how
// we are going to change this code to support other object models we will
// change this code)
//-----------------------------------------------------------------------------
CLASS VMethodData : public VObject
{
public:
	VMethodData();
	VMethodData(const VString& methodName);
	~VMethodData();

	// Associates parameters to be passed to invoked method.
	// Parameters are passed in the order they are associated.
	VMethodData&	operator<<(VVariable param);

	// Set the return variable to be used by the invoked method
	void			setReturnVariable(VVariable returnVariable);

	VMethodData&	operator=(const VMethodData& rhs);
	void			setMethodName(const VString& methodName);

	// ----------- Overridden Views framework methods -----------
	// Required meta-method
	virtual VClass*	iam();
	// Framework equivalent to the assignment operator
	virtual boolean	copyFrom(VObject *pObject);
#	ifndef CV_NOARCHIVER
	virtual void	getFrom(VArchiver& a);  //Override
	virtual void	putTo(VArchiver& a);    //Override
#	endif
	void addParameter(VVariable param);

private:
	// Give VComponentProgrammable access to OLE specific routines, but no one else
	friend CLASS VComponentProgrammable;

	// COM-specific methods
	VString&		getMethodName();
	VARIANTARG*		getReturnVariantArg();
	DISPPARAMS*		getDispparms(INVOKEKIND invocationType);

	// Non-COM provate methods
	void			releaseReturnVariantArg();
	void			releaseVParms();
	void			releaseEverything();
	void			setProperty(VVariable& param);
	void			getProperty(VVariable& param);
	void			updateVariables();
	void			initializeObjectSystem();
	void			uninitializeObjectSystem();

	VARIANTARG*		m_pReturnVariant;	// Argument to IDispatch::Invoke
	VVariable		m_returnVariable;	// Will hold the value of the method
	VString			m_methodName;		// Name of automation method
	VOrdCollect		m_parms;			// Holds all the client parameters
	boolean			m_isInitialized;	// Indicates initialization status

	// COM specific members
	DISPPARAMS*		m_pDisp;			// OLE struct passed to IDispatch::Invoke
	VARIANTARG*		m_pVParms;			// Array of Arguments for invoked method
	DISPID*			m_pNamedArgs;		// Named arguments array

};

inline void VMethodData::addParameter(VVariable param) {
	(*this) << param;
}
#endif // VMETHODDATA_H
