// $Header:   Q:/views/common/vcs/EDITNOT.H_V   1.3   Mar 24 1997 16:08:46   scottt  $

//	editnot.h
//
//	VEditLineNotify interface. [Common]
//
//	Allegris Foundation 1.1.00
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
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//					copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VEDITNOT_H
#define VEDITNOT_H

#include "editline.h"

extern VClass *VEditLineNotifyCls;


CLASS VEditLineNotify : public VEditLine {

public:
				VEditLineNotify();
				VEditLineNotify(const VFrame& frame, 
								VWindow *pwin, 
								const VStyle& style = StyleDefault);
				~VEditLineNotify();

	VClass 		*iam();

	boolean		 enterHit();
	boolean		 escapeHit();
	void		 uponAccept(VObject* clnt, method mthd);
	void		 uponCancel(VObject* clnt, method mthd);
    virtual boolean     clearFocus();

private:
	VObject *acceptClient;
	VObject *cancelClient;
	method   acceptMethod;
	method	 cancelMethod;

};

#endif	
