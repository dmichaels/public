// $Header:   Q:/views/common/vcs/INTLCBTB.CPV   1.6   Mar 24 1997 16:09:26   scottt  $

//	intlcbtb.cpp
//
//	VInternationalComboTable class implementation [Common]
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

#include "intlcbtb.h"
#include "iterator.h"
#include "str.h"
#include "file.h"
#if !defined(CV_NOARCHIVER)
# include "archiver.h"
# include "rscarch.h"
# include "archvrtg.h"
#endif

// Implementation Note
// 
// The keys and values involved in a VInternationalComboTable all belong to the 
// VInternationalComboTable.  Since they are not automatically managed by VDictionary
// and VMultiSet, they are managed here.
//
// The following heap-allocated objects are managed:
//		one VObject per association (can be found in 'associations')
//
// The other keys and values are managed by the base class.

defineAbstractClass(VInternationalComboTable, VInternationalTable)
defineArchiveRevision(VInternationalComboTable, 0)

// ---------------------------------------------------------------------------
// Construct a non-operational string table.
//
VInternationalComboTable::VInternationalComboTable()
{
}

// ---------------------------------------------------------------------------
//
VInternationalComboTable::~VInternationalComboTable()
{
}

// ---------------------------------------------------------------------------
// Retrieve the message for a given tag. Return FALSE if no
// message was found appropriate to the current locale.
//
boolean VInternationalComboTable::get(VObject *tag,
									  VObject *assoc,
									  VObject *message) const
{
	if (VInternationalTable::get(tag, message)) {		
		if (assoc != 0) {
			VObject *obj = associations.getValueAtKey(tag);
			if (obj != 0) {
				assoc->copyFrom(obj);
			}
			else {
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;
}
	
// ---------------------------------------------------------------------------
// Add the association of 'tag' with 'message' to the string table for the
// current locale.  Returns FALSE if this fails, e.g. if there is no valid
// current locale, or if the string table was constructed with an optimized
// connection to a resource archiver, or if the tag uses non-portable
// characters.  If 'tag' already has an association
// in the current locale, replace it.
//
// The locale-independent association 'assoc' is made with the tag only if
// the current locale has a message for the tag.
//
boolean VInternationalComboTable::put(VObject *tag,
									  VObject *assoc,
									  VObject *message)
{
	if (VInternationalTable::put(tag, message)) {
		if (assoc != 0) {
			tag = tags.findKey(tag);  // Use this table's own tag object.
			if (tag == 0) {
				return FALSE; // unexpected problem
			}
			if (associations.includesKey(tag)) {
				delete associations.getValueAtKey(tag);
				associations.removeKey(tag);
			}
			VObject *assoc_copy = assoc->copy();
			return assoc_copy != 0 && associations.atKeyPut(tag, assoc_copy);
		}
		return TRUE;
	}
	return FALSE;
}

// ---------------------------------------------------------------------------
// Called when a tag has been added to the set of keys.
//
boolean VInternationalComboTable::addingTag(VObject *tag)
{
	VObject *assoc = getAssociationDefault();
	return assoc != 0 && associations.atKeyPut(tag, assoc);
}

// ---------------------------------------------------------------------------
// Called when a tag is about to be removed from the set of keys.
//
void VInternationalComboTable::removingTag(VObject *tag)
{
	delete associations.getValueAtKey(tag);
	associations.removeKey(tag);
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store the contents of this VInternationalComboTable into VArchiver a.
//
void VInternationalComboTable::putTo(VArchiver& a)
{
	a << VInternationalComboTableArchiveTag;
	VInternationalTable::putTo(a);
	a.putObject(&associations);
}

// ---------------------------------------------------------------------------
// Load the contents of this VInternationalComboTable from VArchiver a.
//
void VInternationalComboTable::getFrom(VArchiver& a)
{

	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VInternationalComboTableArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VInternationalComboTableArchiveTag));
		return;
	}
	VInternationalTable::getFrom(a); 
	a.getObject(&associations); 
}

// ---------------------------------------------------------------------------
//
void VInternationalComboTable::getFrom(VArchiver& a,
									   VObject * /* data */,
									   boolean /* do_realize */)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store the contents of this VInternationalComboTable to a resource archiver
// with the resource name 'rsc_name'.  The named resource is shrunken, and
// additional information is stored into the resource file so that the
// remainder of the string table can be recovered efficiently using
//		getFrom(VRscArchiver&, const VString&)
//
boolean VInternationalComboTable::putTo(VRscArchiver& rsca,
										const VString& rsc_name) const
{
	return rsca.putObject(rsc_name, (VInternationalComboTable *)this);
}

// ---------------------------------------------------------------------------
// Return a newly constructed VInternationalComboTable object, representing
// the string table from the resource file open in 'rsca',
// and named 'rsc_name'.
// 
// Eventually this will be implemented efficiently, so that all of the strings
// in the string table need not be loaded into memory by this function.
// The newly constructed VInternationalComboTable object will maintain a
// connection to the resource file it came from.
//
// The specified resource must have been written using
//		putTo(VRscArchiver&, const VString&) const
// because a special archive format is required.
//
VInternationalComboTable *VInternationalComboTable::getFrom(
													VRscArchiver& rsca,
													const VString& rsc_name,
													VClass *obj_class)
{
	VObject *sto = 0;
	if (obj_class != 0) {
		sto = rsca.getObject(rsc_name, 0, obj_class);
	}
	if (sto == 0) {
		sto = rsca.getObject(rsc_name, 0, 0);
	}
	if (sto == 0 || !sto->isA(VInternationalComboTableCls)) {
		delete sto;
		return 0;
	}
	VInternationalComboTable *st = (VInternationalComboTable *)sto;
	st->openRscArch = &rsca;
	return st;
}

#endif


