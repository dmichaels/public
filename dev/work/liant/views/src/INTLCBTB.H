// $Header:   Q:/views/common/vcs/INTLCBTB.H_V   1.5   Mar 24 1997 16:09:26   scottt  $

//	intlcbtb.h
//
//	VInternationalComboTable class iinterface [Common]
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
//	10/19/95 pkt	created.
//	02/16/96 pkt	'getFrom(VRscArchiver&...)' now accepts a VClass pointer.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VINTNTLCOMBOTABLE_H
#define VINTNTLCOMBOTABLE_H

#include "intntltb.h"

// This is a thrown together to see what might be added to
// VInternationalTable to make it extensible for associating a
// severity with each tag.

CLASS VInternationalComboTable : public VInternationalTable {

public:
	// constructors/destructor

					VInternationalComboTable();
					~VInternationalComboTable();

#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);

	// Optimizing archival
	boolean			putTo(VRscArchiver& rsca, const VString& rsc_name) const;
	static VInternationalComboTable
				   *getFrom(VRscArchiver& rsca, const VString& rsc_name,
				   			VClass *obj_class = 0);
#endif

	VClass		   *iam();
	
protected:
	boolean			get(VObject *tag, VObject *assoc, VObject *message) const;
	boolean			put(VObject *tag, VObject *assoc, VObject *message);

	virtual boolean	addingTag(VObject *tag);
	virtual void	removingTag(VObject *tag);

	// Override to create a new, independent object having the
	// default value for a tag's association.
	virtual VObject *getAssociationDefault() = 0;

	VDictionary		associations; // for each tag, an associated object
};

extern VClass *VInternationalComboTableCls;

#endif	//	VINTNTLCOMBOTABLE_H


