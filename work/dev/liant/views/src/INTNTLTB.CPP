/*
$Header:   Q:/views/common/vcs/INTNTLTB.CPV   1.12   Mar 24 1997 16:09:28   scottt  $
*/
//	intntltb.cpp
//
//	VInternationalTable class implementation [Common]
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
//	11/02/95 pkt	changed 'getFrom' not to restore a locale that won't work
//					in the current context.
//	11/28/95 pkt	enhanced for the concept of "supporting a locale".
//	02/16/96 pkt	changed to open resource using specific class.
//	04/24/96 pkt	added 'setBackupLocale'.
//	08/29/96 pkt	changed 'getFrom' to apply locale using 'setBackupLocale'.
//	10/02/96 pkt	removed memory leaks.
//	10/04/96 pkt	fixed memory leak due to REVERSEDO removing from VSet.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "intntltb.h"
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
// The keys and values involved in a VInternationalTable all belong to the 
// VInternationalTable.  Since they are not automatically managed by VDictionary
// and VMultiSet, they are managed here.
//
// The following heap-allocated objects are managed:
//		one VObject per tag (can be found in 'tags')
//		one VLocale per locale (can be found in 'messages')
//		one VDictionary per locale (can be found in 'messages')
//		one VObject per message (can be found in the locale-specific VDictionaries)

defineAbstractClass(VInternationalTable, VObject)
defineArchiveRevision(VInternationalTable, 0)

// ---------------------------------------------------------------------------
// Construct a non-operational string table.
//
VInternationalTable::VInternationalTable() :
	openRscArch(0),
	locale(VLocale::current())
{
}

// ---------------------------------------------------------------------------
//
VInternationalTable::~VInternationalTable()
{
	// Destroy the locales, the locale-specific dictionaries,
	// and their messages.
	REVERSEDO (messages, VAssoc, locale_assoc)
		VDictionary *dict = (VDictionary *)locale_assoc->value;
		REVERSEDO (dict, VAssoc, a)
			delete a->value;
		END
		delete dict;
		delete (VLocale *)locale_assoc->key;
	END
	messages.reset();
	
	// Destroy the tags.
	REVERSEDO (tags, VMultiSet::MultiAssoc, a)
		VObject *tag = a->key;
		removingTag(tag);
		delete tag;
	END
	tags.reset();		
}

// ---------------------------------------------------------------------------
// Retrieve the message for a given tag. Return FALSE if no
// message was found appropriate to the current locale.
//
boolean VInternationalTable::get(VObject *tag, VObject *message) const
{
	VDictionary *msgs = localeDictionary();
	if (msgs == 0 || tag == 0) {
		return FALSE;
	}
	VObject *obj = msgs->getValueAtKey(tag);
	return obj != 0 && (message == 0 || message->copyFrom(obj));
}
	
// ---------------------------------------------------------------------------
// Return the message dictionary specific to the given locale.
// Create a new message dictionary for the given VLocale, and add it to
// 'messages', if one is not there already.
// Return 0 if the locale is invalid or if none can be created.
//
VDictionary *VInternationalTable::createLocaleDictionary(const VLocale& loc)
{
	VDictionary *dict = (VDictionary *)messages.getValueAtKey(&loc);
	if (dict != 0) {
		return dict;
	}
	if (loc.isValid()) {
		dict = new VDictionary;
		if (dict != 0) {
			messages.atKeyPut(new VLocale(loc), dict);
			return dict;
		}
	}
	return 0;
}
	
// ---------------------------------------------------------------------------
// Add the association of 'tag' with 'message' to the string table for the
// current locale.  Returns FALSE if this fails, e.g. if there is no valid
// current locale, or if the string table was constructed with an optimized
// connection to a resource archiver, or if the tag uses non-portable
// characters.  If 'message' is null, then success or failure depends on 
// whether a message is already present for the tag.  If 'tag' already has an 
// association in the current locale, replace it.
//
boolean VInternationalTable::put(VObject *tag_param, VObject *message)
{
	if (openRscArch != 0 || tag_param == 0) {
		return FALSE;
	}
	VDictionary *msgs = createLocaleDictionary(locale);
	if (msgs == 0) {
		// Memory or some nasty problem prevented creation of dictionary.
		return FALSE;
	}
	boolean tag_present = msgs->includesKey(tag_param);
	if (message == 0) {
		return tag_present;
	}
	VObject *tag;
	VObject *tag_copy = 0;
	if (tag_present) {
		tag = tags.findKey(tag_param);
		delete msgs->getValueAtKey(tag_param);
		msgs->removeKey(tag);
	}
	else if (isValidTag(tag_param)) {
		tag_copy = tag_param->copy();
		if (tag_copy == 0) {
			return FALSE;
		}
		tag = tags.addKey(tag_copy);
		if (tag != tag_copy) {
			// already in the set of tags
			delete tag_copy;
			tag_copy = 0;
		}
	}
	else {
		// tag_param is not a valid tag.
		return FALSE;
	}
	// tag is the tag object to use.
	// tag_copy is non-zero if it needs to be deleted, should adding it fail.

	VObject *message_copy = message->copy();

	// Continue only if the message is copied, and if the tag object
	// is new we must also have successful notification of the tag
	// being added.
	boolean proceed = message_copy != 0 && (tag_copy == 0 || addingTag(tag_copy));
	if (proceed) {
		return msgs->atKeyPut(tag, message_copy);
	}
	else {
		if (tag_copy != 0) {
			tags.removeKey(tag_copy);
			delete tag_copy;
		}
		return FALSE;
	}
}
	
// ---------------------------------------------------------------------------
// Remove the association of 'tag' from the string table for the
// current locale.  Returns FALSE if this fails, e.g. if there is no valid
// current locale, or if the string table was constructed with an optimized
// connection to a resource archiver.  If 'tag' has no association in
// the current locale, does nothing and returns TRUE.
//
boolean VInternationalTable::remove(VObject *tag_param)
{
	if (openRscArch != 0 || !locale.isValid() || tag_param == 0) {
		return FALSE;
	}
	VDictionary *msgs = localeDictionary();
	if (msgs == 0) {
		return TRUE;
	}
	VObject *tag;
	if (msgs->includesKey(tag_param)) {
		tag = tags.removeKey(tag_param);
		delete msgs->getValueAtKey(tag_param);
		msgs->removeKey(tag_param);
		if (tag != 0 && tags.getValueAtKey(tag) <= 0) {
			// The tag is no longer in the string table anywhere.
			removingTag(tag);
			delete tag;
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Return the sub-dictionary of message strings associated with this
// string table's current locale.
//
VDictionary *VInternationalTable::localeDictionary() const
{
	if (locale.isValid()) {
		return (VDictionary *)messages.getValueAtKey(&locale);
	}
	return 0;
}

// ---------------------------------------------------------------------------
// Set the string table to use a particular locale when getting message
// strings.  Returns a negative value if the locale could not be set (e.g.
// if the OS's current code page does not support the requested locale).
// Returns 0 if the locale could be set but the string table does not 
// support the locale, i.e. has no messages for that locale.
//
int VInternationalTable::setLocale(const VLocale& loc)
{
	if (loc.isValid() && supports(loc)) {
		VDictionary *d = createLocaleDictionary(loc);
		if (d != 0) {
			locale = loc;
			return d->empty() ? 0 : 1;
		}
	}
	return -1;		
}

// ---------------------------------------------------------------------------
// Like setLocale, except that no suitability testing is performed on 'loc'.
// For instance, if this is a string table then it can be set to a locale
// whose strings use the wrong code page.
//
// This may be useful in case the program is not prepared for the code page
// under which it is run. The backup locale can be set to an English locale,
// and at least some words in a generally recognizable language will show up.
//
int VInternationalTable::setBackupLocale(const VLocale& loc)
{
	if (loc.isValid()) {
		VDictionary *d = createLocaleDictionary(loc);
		if (d != 0) {
			locale = loc;
			return d->empty() ? 0 : 1;
		}
	}
	return -1;		
}

// ---------------------------------------------------------------------------
//
boolean VInternationalTable::supports(const VLocale& loc) const
{
	return VLocale::current() <= (VLocale &)loc;		
}

// ---------------------------------------------------------------------------
// Return the locale which the string table is currently using.
//
VLocale VInternationalTable::getLocale() const
{
	return locale;
}

// ---------------------------------------------------------------------------
// Import a simple message file to the current locale's portion of this
// string table.  If any associations already exist for the imported
// keys, they are overwritten.
//
// Returns FALSE if the file could not be opened or if there were memory
// problems.  If there is difficulty parsing the file, struggles on and
// returns TRUE.
//
boolean VInternationalTable::import(const VPathString& file_name)
{
	if (openRscArch != 0) {
		return FALSE;
	}
	VDictionary *msgs = createLocaleDictionary(locale);
	if (msgs == 0) {
		return FALSE;
	}
	VFile file(file_name);
	if (!file.open(ReadOnly)) { 
		return FALSE;
	}
	const max_line = 2000;
	char buf[max_line];
	while (!file.eof()) {
		integer count = file.getln(buf, max_line - 1);
		boolean OK = TRUE;
		while (count >= max_line - 1) {
			// didn't find the line termination
			OK = FALSE;
			count = file.getln(buf, max_line - 1);
		}
		if (OK && count > 0) {
			VStream line;
			line.puts(buf);
			line.at(0);
			line.strip();
			importMessage(line);
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
// Export a simple message file from the current locale's portion of this
// string table.  
//
// Returns FALSE if the file could not be opened or if there were memory
// problems.
//
boolean VInternationalTable::export(const VPathString& file_name) const
{
	VDictionary *msgs = localeDictionary();
	if (msgs == 0) {
		return FALSE;
	}
	VFile file(file_name);
	boolean exists = ((VPathString&)file_name).exists();
	if (!file.open(exists ? WriteOnly : Create)) { 
		return FALSE;
	}
	DO (msgs, VAssoc, a)
		if (exportMessage(file, a->key, a->value)) {
			file.putln("");
		}
	END
	return TRUE;	
}

// ---------------------------------------------------------------------------
// Return a new VSet object containing all of the locales present in this
// string table.  Each locale is indicated by a new VLocale object.  Deletion
// of the new VSet and VLocales is up to the caller.
//
// Note that the success of 'setLocale' depends on more than just the presence
// of the locale in the string table -- the locale must be supported on the
// system where Views is running.
//
VSet *VInternationalTable::setOfLocales() const
{
	VSet *s = new VSet;
	DO (messages, VAssoc, loc)
		s->add(new VLocale(*(VLocale *)loc->key));
	END
	return s;
}

// ---------------------------------------------------------------------------
// Return a new VSet object containing all of the tags present in this string
// table.  Each tag is indicated by a new VString object.  Deletion of the new
// VSet and VStrings is up to the caller.
//
VSet *VInternationalTable::setOfTags() const
{
	VSet *s = new VSet;
	DO (tags, VMultiSet::MultiAssoc, loc)
		s->add(new VString(*(VString *)loc->key));
	END
	return s;
}

// ---------------------------------------------------------------------------
// Called when a tag has been added to the set of keys.
//
boolean VInternationalTable::addingTag(VObject * /* tag */)
{
	// Return TRUE to keep the tag.
	return TRUE;
}

// ---------------------------------------------------------------------------
// Called when a tag is about to be removed from the set of keys.
//
void VInternationalTable::removingTag(VObject * /* tag */)
{
}

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
// Store the contents of this VInternationalTable into VArchiver a.
//
void VInternationalTable::putTo(VArchiver& a)
{
	a << VInternationalTableArchiveTag;

	a.putObject(&locale);
	a.putObject(&tags);
	a.putObject(&messages);
}

// ---------------------------------------------------------------------------
// Load the contents of this VInternationalTable from VArchiver a.
//
void VInternationalTable::getFrom(VArchiver& a)
{
	long tag;
	a >> tag;
	if (!archiveTagOk(tag, VInternationalTableArchiveTag)) {
	 	a.abort(archiveTagAbortCheck(tag, VInternationalTableArchiveTag));
		return;
	}

	VLocale loc;
	a.getObject(&loc);
	a.getObject(&tags);
	a.getObject(&messages); 
	setBackupLocale(loc);
}

// ---------------------------------------------------------------------------
//
void VInternationalTable::getFrom(VArchiver& a,
								  VObject * /* data */, boolean /* do_realize */)
{
	getFrom(a);
}

// ---------------------------------------------------------------------------
// Store the contents of this VInternationalTable to a resource archiver with the
// resource name 'rsc_name'.  The named resource is shrunken, and additional
// information is stored into the resource file so that the remainder of
// the string table can be recovered efficiently using
//		getFrom(VRscArchiver&, const VString&)
//
boolean VInternationalTable::putTo(VRscArchiver& rsca, const VString& rsc_name) const
{
	return rsca.putObject(rsc_name, (VInternationalTable *)this);
}

// ---------------------------------------------------------------------------
// Return a newly constructed VInternationalTable object, representing the string
// table from the resource file open in 'rsca', and named 'rsc_name'.
// 
// Eventually this will be implemented efficiently, so that all of the strings
// in the string table need not be loaded into memory by this function.
// The newly constructed VInternationalTable object will maintain a connection to
// the resource file it came from.
//
// The specified resource must have been written using
//		putTo(VRscArchiver&, const VString&) const
// because a special archive format is required.
//
VInternationalTable *VInternationalTable::getFrom(VRscArchiver& rsca,
												  const VString& rsc_name,
												  boolean optimize,
												  VClass *obj_class)
{
	VObject *sto = 0;
	if (obj_class != 0) {
		sto = rsca.getObject(rsc_name, 0, obj_class);
	}
	if (sto == 0) {
		sto = rsca.getObject(rsc_name, 0, 0);
	}
	if (sto == 0 || !sto->isA(VInternationalTableCls)) {
		delete sto;
		return 0;
	}
	VInternationalTable *st = (VInternationalTable *)sto;
	if (optimize) {
		st->openRscArch = &rsca;
	}
	return st;
}

#endif


