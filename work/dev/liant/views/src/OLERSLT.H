// $Header:   Q:/views/win/vcs/OLERSLT.H_v   1.8   Mar 04 1997 14:13:24   gregm  $

//  OLE Result Class
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
//  Copyright (c) 1995-1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/20/94 dss    Initial release.
//  12/07/95 dss    Added some includes. Name change: OLERESULT_H.
//  12/12/95 dss    Removed operator=(int), operator=(HRESULT). 
//                  Added operator=(long).
//  02/25/95 dss    Added op==() and op!=() for WIN16.
//  ---------------------------------------------------------------------------

#ifndef OLERESULT_H
#define OLERESULT_H

#include "cvassert.h"
#include "oledefs.h"

CLASS OLEResultReport : public VResultReport
{ 
public: 
    const VString&  asSystemMessage(VResult&);

    const VString&  asResultType(VResult&);
    const VString&  asUserResultType(VResult&); 
    
protected:
    const VString&  StorageMessage(SCODE sc);
    const VString&  DispatchMessage(SCODE sc);
    const VString&  ITFMessage(SCODE sc);
    const VString&  NULLMessage(SCODE sc);
    const VString&  RPCMessage(SCODE sc);
};

// To DO: CV_EXPORT <= need platform independent export
#if defined (CV_WIN16) && !defined (DLL)
extern OLEResultReport CV_NEARDATA oleResultReport;
#else
extern OLEResultReport  CV_DLL_EXPORT FAR oleResultReport;
#endif

//**********************************************************************
//
CLASS OLEResult : public VResult {
public:
                    OLEResult()
                    {cv_assert(sizeof(HRESULT) <= sizeof(resultCode));}
    
                    // need this otherwise different VResult derived
                    // classes will use explicit op=(), of which this
                    // class defined op=(int). op=(int) only copies
                    // the result value, not the result object.
    OLEResult&      operator= (const VResult& r)
                    {VResult::operator=(r); return *this;}

    // ***************************
    //
    // HRESULT behavior     

                    // OLE Methods return HRESULTS which are ptrs on WIN16
                    // and longs on WIN32.
                    // This operator will take care of this case.
    OLEResult&      operator=(HRESULT r) 
                    {
                        *(HRESULT*)asAddress() = r; 
                        report = &oleResultReport; 
                        return *this;
                    }

#if defined(CV_WIN16)                   
	// The operator=()'s handles the ambiguities arising from converting (see the 
	// operator casts) to a constant SCODE.
	// (which is a long) to either a SCODE to an HRESULT.
	boolean			operator==(long l) 
					{
						return (SCODE)*this == SCODE(l);
					}

	boolean			operator!=(long l) 
					{
						return (SCODE)*this != SCODE(l);
					}

                    // On Win16, SCODEs are a different type than HRESULTs.
    OLEResult&      operator=(SCODE sc) 
                    {
                        *(SCODE*)asAddress() = (SCODE)sc; 
                        report = &oleResultReport; 
                        return *this;
                    }
#endif                  
                    // Warning: this cast will compare against pointers
                    // on WIN16 and longs on WIN32 because of the way
                    // HRESULT is defined.
                    // Use asResult() for safety.
                    operator HRESULT()  {return *(HRESULT*)asAddress();}
#if defined(CV_WIN16)                   
                    operator SCODE()    {return *(SCODE*)asAddress();}
#endif                  
            
    HRESULT         asResult() const {return *(const HRESULT*)asAddress();}
 };
 
#endif // OLERESULT_H
