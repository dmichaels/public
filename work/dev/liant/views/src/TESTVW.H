//	testvw.h
//
//	TestView interface
//
//	Allegris Workshop 1.0
//	Copyright (c) 1996 by INTERSOLV, Inc.
//	+-----------------------------------------------------------------+
//	| This product is the property of INTERSOLV, Inc. and is licensed |
//	| pursuant to a written license agreement.  No portion of  this   |
//	| product may be reproduced without the written permission of     |
//	| INTERSOLV, Inc. except pursuant to the license agreement.       |
//	+-----------------------------------------------------------------+
//
// --------------------------------------------------------------------------

#ifndef TESTVW_H
#define TESTVW_H

// --------------------------------------------------------------------------
//
#include "appview.h"


// --------------------------------------------------------------------------
//
CLASS VAppView;
CLASS VClass;

// --------------------------------------------------------------------------
//
CLASS TestView : public VAppView {
public:
					TestView();
				   ~TestView();
	VClass		   *iam();
                boolean rmouseDn(int x, int y);

protected:
	void			initialize();
	void			construct();
	

};

extern VClass *TestViewCls;

#endif // TESTVW_H
