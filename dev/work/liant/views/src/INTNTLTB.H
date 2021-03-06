// $Header:   Q:/views/common/vcs/INTNTLTB.H_V   1.10   Mar 24 1997 16:09:28   scottt  $

//	intntltb.h
//
//	VInternationalTable class interface [Common]
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
//	10/18/95 pkt	created.
//	10/19/95 pkt	debugged.
//	11/02/95 pkt	added 'optimize' flag to getFrom(VRscArchiver&,...).
//	11/28/95 pkt	enhanced for the concept of "supporting a locale".
//	02/16/96 pkt	'getFrom(VRscArchiver&...)' now accepts a VClass pointer.
//	04/24/96 pkt	added 'setBackupLocale'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VINTERNATIONALTABLE_H
#define VINTERNATIONALTABLE_H

#include "object.h"
#include "assoc.h"
#include "dictinry.h"
#include "multiset.h"
#include "vlocale.h"

CLASS VString;
CLASS VPathString;
CLASS VRscArchiver;
CLASS VStream;

// A VInternationalTable is a collection of objects that varies from locale to locale.
// In the context of the current locale, a string can be looked up by key.
// 
// A VInternationalTable is normally open for reading only, and the archive
// form will be optimized for that.  Modifications to an international table are
// usually made from a special-purpose table editor.
//
// A VInternationalTable has its own current locale, which defaults to the
// locale in which the Views program is running.

CLASS VInternationalTable : public VObject {

public:
	// constructors/destructor

					VInternationalTable();
					~VInternationalTable();

	int				setLocale(const VLocale&);
	int				setBackupLocale(const VLocale&);
	VLocale			getLocale() const;

	VSet		   *setOfLocales() const;
	VSet		   *setOfTags() const;

	boolean			import(const VPathString& file_name);
	boolean 		export(const VPathString& file_name) const;

#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);

	// Optimizing archival
	boolean			putTo(VRscArchiver& rsca, const VString& rsc_name) const;
	static VInternationalTable
				   *getFrom(VRscArchiver& rsca, const VString& rsc_name,
				   								boolean optimize = TRUE,
				   								VClass *obj_class = 0);
#endif

	VClass		   *iam();
	
protected:
	boolean			get(VObject *tag, VObject *message) const;
	boolean			put(VObject *tag, VObject *message);
	boolean			remove(VObject *tag);

	// Override to validate tag prior to its placement into the
	// set of tags.  This should return FALSE if the tag does not
	// have the dynamic type required.
	virtual boolean	isValidTag(VObject *tag) = 0;

	// Override to take a line of a text file representing a message, 
	// and put its tag and message into this table. 
	virtual void	importMessage(VStream& line) = 0;

	// Override to put a tag and message to a text stream.
	// Assume 'tag' and 'message' parameters are not null.
	virtual boolean	exportMessage(VStream& file,
								  VObject *tag, 
								  VObject *message) const = 0;
	
	// Called when a tag has been added to the set of keys.
	// If FALSE is returned, the addition is cancelled.
	virtual boolean	addingTag(VObject *tag);

	// Called when a tag is about to be removed from the set of keys.
	virtual void	removingTag(VObject *tag);

	// Override to tell whether the given locale can be supported.
	virtual boolean	supports(const VLocale& loc) const;

	VDictionary	   *localeDictionary() const;
	VDictionary	   *createLocaleDictionary(const VLocale&);

	VLocale			locale;
	VMultiSet		tags;		// for each tag, a count of occurrences
	VDictionary		messages;	// for each locale, a message dictionary
	VRscArchiver   *openRscArch;

};

extern VClass *VInternationalTableCls;

#endif	//	VINTERNATIONALTABLE_H


