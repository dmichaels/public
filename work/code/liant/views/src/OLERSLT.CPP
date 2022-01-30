// $Header:   Q:/views/win/vcs/OLERSLT.CPv   1.4   Mar 04 1997 14:13:22   gregm  $

//	olerslt.cpp
//
//	OLE Result Class
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
//	04/20/94 dss	Initial release.
//	12/07/95 dss	Moved oleResultReport to globals2.cpp.
//	---------------------------------------------------------------------------

#include "pre.h"

//**********************************************************************
//
const VString& OLEResultReport::asSystemMessage(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	                         
	SCODE sc = GetScode(*(HRESULT*)theResult.asAddress());	                         
	                         
	switch (SCODE_FACILITY(sc))
	{
	case FACILITY_STORAGE:
		StorageMessage(sc);
		break;
	case FACILITY_RPC:
		RPCMessage(sc);
		break;
	case FACILITY_DISPATCH:
		DispatchMessage(sc);
		break;
	case FACILITY_ITF:
		ITFMessage(sc);
		break;
	case FACILITY_NULL:
		NULLMessage(sc);
		break;
	default:
		resultText.prints("OLE result code 0x%0.8x", 
			long(*(HRESULT*)theResult.asAddress()));
		break;
	}
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::StorageMessage(SCODE sc)
{
	switch (SCODE_CODE(sc))
	{
	case SCODE_CODE(STG_E_FILENOTFOUND):
		resultText.puts("file not found");
		break;
	case SCODE_CODE(STG_E_PATHNOTFOUND):
		resultText.puts("path not found");
		break;
	case SCODE_CODE(STG_E_ACCESSDENIED):
		resultText.puts("access denied");
		break;
	case SCODE_CODE(STG_E_DISKISWRITEPROTECTED):
		resultText.puts("storage is write protected");
		break;
	case SCODE_CODE(STG_E_FILEALREADYEXISTS):
		resultText.puts("already exists");
		break;
	case SCODE_CODE(STG_E_MEDIUMFULL):
		resultText.puts("storage is full");
		break;
	case SCODE_CODE(STG_E_INUSE):
		resultText.puts("in use");
		break;
	case SCODE_CODE(STG_E_CANTSAVE):
		resultText.puts("unknown error");
		break;
	case SCODE_CODE(STG_E_SHAREVIOLATION):
		resultText.puts("sharing violation");
		break;
	case SCODE_CODE(STG_E_READFAULT):
		resultText.puts("read error");
		break;
	case SCODE_CODE(STG_E_WRITEFAULT):
		resultText.puts("write error");
		break;
	default:
		resultText.prints("OLE Storage status code 0x%0.8x", long(sc));
		break;
	}
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::DispatchMessage(SCODE sc)
{
	resultText.prints("OLE Dispatch status code 0x%0.8x", long(sc));
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::ITFMessage(SCODE sc)
{
	resultText.prints("OLE %s status code 0x%0.8x", 
					  SCodeITFToText(sc), 
					  long(sc));
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::RPCMessage(SCODE sc)
{
	resultText.prints("OLE RPC status code 0x%0.8x", long(sc));
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::NULLMessage(SCODE sc)
{
	switch (SCODE_CODE(sc))
	{
	case SCODE_CODE(E_NOINTERFACE):
		resultText.puts("interface not supported");
		break;
	case SCODE_CODE(E_NOTIMPL):
		resultText.puts("function not implemented");
		break;
	case SCODE_CODE(E_FAIL):
		resultText.puts("general failure");
		break;
	case SCODE_CODE(E_ABORT):
		resultText.puts("abort");
		break;
	case SCODE_CODE(E_POINTER):
		resultText.puts("invalid interface pointer");
		break;
	case SCODE_CODE(E_HANDLE):
		resultText.puts("invalid handle");
		break;
	case SCODE_CODE(E_INVALIDARG):
		resultText.puts("invalid argument to method");
		break;
	case SCODE_CODE(E_OUTOFMEMORY):
		resultText.puts("out of memory");
		break;
	case SCODE_CODE(E_UNEXPECTED):
		resultText.puts("unexpected function called");
		break;
	default:
		resultText.prints("OLE status code 0x%0.8x", long(sc));
		break;
	}
	return resultText;
}

//**********************************************************************
//
const VString& OLEResultReport::asResultType(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	
	switch (SCODE_FACILITY(GetScode(*(HRESULT*)theResult.asAddress())))
	{
	case FACILITY_STORAGE:
		resultType.puts("OLE Structured Storage");
		break;
	default:
		resultType.puts("OLE");
		break;
	}
	return resultType;
}

//**********************************************************************
//
const VString& OLEResultReport::asUserResultType(VResult& theResult)
{
	cv_assert(&theResult.getReport() == this);
	
	switch (SCODE_FACILITY(GetScode(*(HRESULT*)theResult.asAddress())))
	{
	case FACILITY_STORAGE:
		userResultType.puts("OLE Structured Storage");
		break;
	default:
		userResultType.puts("OLE");
		break;
	}
	return userResultType;
}


