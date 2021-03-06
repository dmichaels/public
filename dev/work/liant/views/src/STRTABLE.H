// $Header:   Q:/views/common/vcs/STRTABLE.H_V   1.11   Mar 24 1997 16:13:34   scottt  $

//	strtable.h
//
//	VStringTable class interface [Common]
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
//	10/03/95 pkt	created.
//	10/18/95 pkt	derived from VInternationalTable class.
//	10/19/95 pkt	debugged.
//	10/26/95 pkt	fixed by removing declarations of 'export' and 'import'.
//	11/02/95 pkt	added 'optimize' flag to getFrom(VRscArchiver&,...).
//	11/20/95 pkt	added 'canPut' and 'canGet'.
//	11/24/95 pkt	added 'supports'.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VSTRTABLE_H
#define VSTRTABLE_H

#include "intntltb.h"

CLASS VString;
CLASS VPathString;
CLASS VRscArchiver;
CLASS VStream;

// A VStringTable is a collection of strings that varies from locale to locale.
// In the context of the current locale, a string can be looked up by key
// (which is a restricted VString).
// 
// A VStringTable is normally open for reading only, and the archive
// form will be optimized for that.  Modifications to a string table are
// usually made from the string table editor.
//
// A VStringTable has its own current locale, which defaults to the
// locale in which the Views program is running.

CLASS VStringTable : public VInternationalTable {

public:
	// constructors/destructor

					VStringTable();
					~VStringTable();

	boolean			get(const VString& tag, VString& message) const;
	boolean			put(const VString& tag, const VString& message);
	boolean			remove(const VString& tag);

#ifndef CV_NOARCHIVER
	// Archival
	void			putTo(VArchiver&);
	void			getFrom(VArchiver&);
	virtual void 	getFrom(VArchiver& a, VObject *data,
							boolean do_realize = TRUE);

	// Optimizing archival
	boolean			putTo(VRscArchiver& rsca, const VString& rsc_name) const;
	static VStringTable
				   *getFrom(VRscArchiver& rsca, const VString& rsc_name,
				   								boolean optimize = TRUE);
#endif

	VClass		   *iam();

	static boolean	isValidTag(const VString& tag);
	static boolean	parseString(VStream& strm, VString& str);
	static void		skipWhitespace(VStream& strm);
	static boolean	escapeString(VStream& strm, const VString& str);
	static boolean	canPut(const VLocale& loc);
	static boolean	canGet(const VLocale& loc);

protected:
	virtual void	importMessage(VStream& line);
	virtual boolean	exportMessage(VStream& file,
								  VObject *tag, 
								  VObject *message) const;
	virtual boolean	isValidTag(VObject *tag);
	virtual boolean	supports(const VLocale& loc) const;

};

extern VClass *VStringTableCls;

#endif	//	VSTRTABLE_H


