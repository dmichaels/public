// $Header:   Q:/views/common/vcs/RSCARCH.CPV   1.10   Mar 24 1997 16:13:14   scottt  $ 

//	rscarch.cpp
//
//	VRscArchiver class implementation [Common]
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
//	05/14/93 pmcm	object definitions of temporary object are now denoted by
//					't' in the archive; getObject() is now in sync with
//					putObject() with respect to the exclusion of temporaries
//					from the instance set.
//	07/09/93 pmcm	changed char * params to const VString&
//	07/22/93 dgm	added final default boolean argument to getObject()
//					to optionally suppress actual realization of the
//					retrieved object (this only pertains to GUI objects);
//					this was necessary for our dialog-editor.
//	07/28/93 pmcm	close() now resets the open flag;
//					putObject() now copies const name to fiddle with
//  08/09/93 jmd	removed free (and use thereof)
//  08/10/93 pmcm	removed VWindow dependencies.
//  08/10/93 pmcm	removed classToMatch, closeSets() and used new protected
//					VArchiver interface to class and instances sets.
//	08/12/93 pmcm	removed all the i/o methods that deal with basic types;
//					added read(), write(), getch(), putch(), getTrimLine(),
//					putTrimLine() and used 'em in getObject() and putObject().
//	08/12/93 pmcm 	preened.
//	08/19/93 pmcm 	added abort stuff
//	09/01/93 dgm	M8 changes.
//	09/01/93 pmcm	bumped RscArchiveRevisionNumber for VFontInfo, VPenInfo,
//					VBrushInfo, window minimum size, and control validation
//					string changes in various window class archivals.
//	09/23/93 pmcm	bumped revision number for adding archive tags, etc.
//	10/10/93 jmd	SC6 changes.
//	10/18/93 dgm	UNIX changes; changed name VBlockFile::AccessType to
//					VBlockFile::BlockAccessType to avoid naming conflict
//					when using cfront 2.1.
//	11/12/93 pmcm	added integer cast in getDirectoryCount().
//	01/13/94 dgm	merged and preened
//	01/19/94 pkt	preened.
//  02/02/94 pmcm	updated VArchiver::abort() use
//	06/02/94 dgm	Simplified ?: expressions in function calls to
//					work-around SPARCworks C++ compiler (4.0) bug.
//	08/08/94 jmd	added begin/endWrite
//	03/06/95 jld	added #ifndef CV_NOARCHIVER wrapper
//	04/12/95 pkt	updated for const class name.
//	05/19/95 pkt	selected Windows as common source.
//	05/26/95 pkt	combined international changes into common version.
//	07/30/96 pkt	added check for I/O error to set abort flag.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef CV_NOARCHIVER
#include "rscarch.h"
#include "dictinry.h"
#include "ordcllct.h"
#include "intassoc.h"
#include "memry.h"

extern "C" {
#	include <ctype.h>
#	include <stdio.h>
#	include <string.h>
}

static const long RscArchiverRevisionNumber = 3000003L;	//	rev 3.0, number 3

defineClass(VRscArchiver, VArchiver)

static boolean translateAccess(int access,
							   VBlockFile::BlockAccessType *bfile_access);

// ---------------------------------------------------------------------------
//	Construct an archiver that is not bound to any resource file.
//	Note that the resource archiver does not support in-memory archives.
//
VRscArchiver::VRscArchiver() : VArchiver(FALSE)
{
	init("");
}

// ---------------------------------------------------------------------------
//	Construct a resource archiver that is bound to the given resource file.
//
//	You may open the file later, with VFile::open(enum fileMode m).  
//	Bind the archiver to another file with VFile::close(), 
//	VFile::open(enum fileMode m, const VString& fname).
//
VRscArchiver::VRscArchiver(const VString& fname)
{ 
	init(fname);
}

// ---------------------------------------------------------------------------
//	Destroy this resource archiver.
//
VRscArchiver::~VRscArchiver()
{
	if (isOpen()) {
		close();
	}
}

// ---------------------------------------------------------------------------
//	Common initializer for the constructors.
//
void VRscArchiver::init(const VString& fname)
{
	writeFlag = 0;

	closeSets();
	resourceFile.setRevisionNumber(RscArchiverRevisionNumber);
	resourceFile.file(fname.gets());
   	ascii(TRUE);
   	isOpen(FALSE);
   	isMemoryArchive(FALSE);				
	inResource(FALSE);
	abort(NoAbort);
}

// ---------------------------------------------------------------------------
//	Close the resource file and the class and instance sets.
//
void VRscArchiver::close()
{
	closeSets();
	resourceFile.close();
  	isOpen(FALSE); 
	abort(NoAbort);
}

// ---------------------------------------------------------------------------
//	Rewinds the resource archiver.
//	Should be used between getObject() and putObject().
//
void VRscArchiver::rewind()
{
	closeSets();
	inResource(FALSE);
}

// ---------------------------------------------------------------------------
//	Opens the resource file to which this resource archiver is bound with 
//	the given mode of file access. The mode should be one of 
//	VArchiver::AccessType, i.e.:
//
//		Create, ReadOnly, WriteOnly, Update, or Temporary.
//
//	Returns TRUE, if able to open the resource file; FALSE, if not.
//
boolean VRscArchiver::open(int access)
{
	if (isOpen()) {
		close();
	}

	VBlockFile::BlockAccessType bfile_access;

	if (translateAccess(access, &bfile_access)) {
		isOpen(resourceFile.open(bfile_access));
	}
	return isOpen();
}

// ---------------------------------------------------------------------------
//	Binds the resource archiver to the named resource file and 
//	opens the file with the given mode of file access. The mode should be 
//	one of VArchiver::AccessType, i.e:
//
//		Create, ReadOnly, WriteOnly, Update, or Temporary.
//
//	Returns TRUE, if able to open the resource file; FALSE, if not.
//
boolean VRscArchiver::open(int access, const VString& fname)
{
	if (isOpen()) {
		close();
	}

	VBlockFile::BlockAccessType bfile_access;

	if (translateAccess(access, &bfile_access)) {
		isOpen(resourceFile.open(bfile_access, fname.gets()));
	}
	return isOpen();
}

// ---------------------------------------------------------------------------
//	Retrieves the named resource from the open resource file to which this
//	is bound.
//
//	Required arguments:
//
//	rscName		The name of the resource in the resource file.
//	obj			A pointer to an existing object of the same class as that of
//				the named resource.  If non-zero, then the object data will be
//				retrieved into this object.  If 0, then a new object of the 
//				class of the named resource will be created and initialized 
//				with the archived object data.
//
//	Optional arguments:
//
//	cls			The class of the named resource.  If non-zero, this method
//				will only retrieve the object if the named resource is of the
//				specified class.  By default, no class type-checking is 
//				performed.
//	data		A pointer to an object to be passed to the object during
//				retrieval.  The nature and purpose of this object are
//				dependent upon the calling context and the class of the 
//				resource.  For example, VWindow resources interpret this 
//				as the parent VWindow.
//	doRealize	If TRUE, this boolean indicates that the object should realize
//				its GUI core; FALSE, that it should not.  Currently, this is
//				only used by window object editors.
//
//	Returns a pointer to the retrieved object, if successful.  This pointer
//	may be null if the resource is null and the given object is null.
//	
//	Returns 0, if the resource file is not open, the resource could not be 
//	found, the named resource is of a class other than requested, an I/O
//	error occurred, or if the resource is null and the given object is null.
//
//	Note that this implementation does not support named references
//	across resource boundaries.  This means that it does not allow the 
//	components of an resource to be named.  If this method is called while 
//	retrieving a component of a resource, then the name is ignored.
//	
VObject *VRscArchiver::getObject(	const VString& 	rscName, 
									VObject    	   *obj, 
									VClass 	   	   *cls,
									VObject    	   *data,
									boolean			doRealize)
{
	if (!inResource()) {
		const char *clsname;
		if (cls == 0) {
			clsname = 0;
		}
		else {
			clsname = cls->nameOf();
		}
		VBlockFileDirItem item(rscName, VString(clsname));
		item.setWildType(cls == 0);

		if (!resourceFile.contains(item) || !resourceFile.find(item)) {

			//	the resource file is not open,
			//	the named resource does not exist,
			//	the named resource is of a class other than requested,
			//	or an I/O error occurred.

		 	return 0;
		}

		inResource(TRUE);
		closeSets();	//	(class and instance sets are per resource)
		abort(NoAbort);
   	}

   	VObject *retObject;
   	if (data != 0) {
		retObject = getObject(obj, data, doRealize);
	}
	else {
		retObject = getObject(obj);
	}

	if (inResource()) {
		inResource(FALSE);
		closeSets();
	}

	return retObject;
}

// ---------------------------------------------------------------------------
//	Retrieves an un-named component object from the current resource file 
//	as part of the retrieval of some named resource.  Supplies the object
//	with the given data.  This method performs no class checking.
//
//	Returns the object retrieved, if successful.
//	Returns 0, if the object in the resource file is null or the resource
//	archiver is not currently servicing some named resource.
//
//	Optional arguments:
//
//	data		A pointer to an object to be passed to the resouce object 
//				during retrieval.  The nature and purpose of this object are
//				dependent upon the calling context and the class of the 
//				resource.  For example, VWindow resources interpret this 
//				as the parent VWindow.
//	doRealize	If TRUE, this boolean indicates that the object should realize
//				its GUI core; FALSE, that it should not.  Currently, this is
//				only used by window object editors.
//
//	Note that this implementation does not support named references
//	across resource boundaries.  Therefore, it does not allow the components 
//	of an resource to be named.
//
VObject *VRscArchiver::getObject(VObject *obj, VObject *data, 
									boolean doRealize)
{
 	return inResource() ? VArchiver::getObject(obj, data, doRealize) : 0;
}

// ---------------------------------------------------------------------------
//	Retrieves an un-named component object from the current resource file 
//	as part of the retrieval of some named resource.
//	This method performs no class checking.
//
//	Returns the object retrieved, if successful.
//	Returns 0, if the object in the resource file is null or the resource
//	archiver is not currently servicing some named resource.
//
//	Note that this implementation does not support named references
//	across resource boundaries.  Therefore, it does not	allow the 
//	components of an resource to be named.
//
VObject *VRscArchiver::getObject(VObject *obj)
{
	return inResource() ? getObject(obj, 0) : 0;
}

// ---------------------------------------------------------------------------
//	Saves an object under the given name to the open resource file to which 
//	this resource archiver is bound.
//
//	Returns TRUE, if successful.
//	Returns FALSE and does not save the object if the object not named,
//	the name is invalid (over 50 characters or contains a newline--see
//	VBlockFile), the resource file is not open, or an I/O error occurred.
//
//	The optional temp argument specifies whether this object is temporary.
//	A object should be flagged as temporary if it exists on the stack.  By
//	default, the object is presumed to be non-temporary.
//
//	Note that this implementation does not support named references
//	across resource boundaries.  Therefore, it does not	allow the components 
//	of an resource to be named.  If this method is called while archiving a 
//	component of a resource, then the name is ignored.
//
boolean VRscArchiver::putObject(const VString& 	rscName,
								VObject 	   *obj, 
								boolean 		temp)
{					 
	if (obj == 0) {
		return FALSE;
	}

	if (!inResource()) {
		if (rscName.size() == 0) {
		 	return FALSE;	//	unnamed objects cannot be saved
		}

		VBlockFileDirItem item(rscName, obj->iam()->nameOf());
		if (!resourceFile.find(item)) {

			//	the resource file is not open,
			//	the named resource does not exist and can not be
			//	created in the file,
			//	or an I/O error occurred.

		 	return FALSE;	
		}

		inResource(TRUE);
		closeSets();	//	(class and instance sets are per named resource)
		abort(NoAbort);
   	}

	putObject(obj, temp);

	if (inResource()) {
		inResource(FALSE);
		closeSets();
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Saves an un-named component object to the resource file 
//	as part of the archiveal of some named resource.
//
//	The optional temp argument specifies whether this object is temporary.
//	A object should be flagged as temporary if it exists on the stack or if
//	the object may be referenced more than once and is not expected to 
//	persist over the life of archiving or retrieving the named resource.
//	By	default, the object is presumed to be non-temporary.
//
//	The resource archiver does not archive duplicates of non-temporary 
//	objects which are components of some named resource.  If an non-temporary
//	object has been previously archived as part of the same named resource, 
//	then only a reference to the previous instance will be archived, not 
//	another complete object.  Temporary objects do not benefit from this
//	space-saving feature.
//	
//	Note that this implementation does not support named references
//	across resource boundaries.  Therefore, it does not	allow the components 
//	of an resource to be named.
//
boolean VRscArchiver::putObject(VObject *obj, boolean temp)
{
	return inResource() ? VArchiver::putObject(obj, temp) : FALSE;
}

// ---------------------------------------------------------------------------
//	This is a helper function that is used internally by open() to validate
//	VArchiver::Access file access modes and convert then to one 
//	of VBlockFile::BlockAccessType.
//
//	Returns TRUE and *bfile_access contains the translation, if the 
//	access mode is a VArchiver::AccessType; FALSE and *bfile_access 
//	remains bogus, if not.
//
static boolean translateAccess(int access,
							   VBlockFile::BlockAccessType *bfile_access)
{
	switch ((VArchiver::AccessType) access) {

	case VArchiver::ReadOnly:
		*bfile_access = VBlockFile::ReadOnly;
		break;
		
   	case VArchiver::Create:	
		*bfile_access = VBlockFile::Create;
		break;

	case VArchiver::Update:
		*bfile_access = VBlockFile::Update;
		break;

	case VArchiver::Temporary:
		*bfile_access = VBlockFile::Temporary;
		break;

	default:
		return FALSE;
	}

	return TRUE;
}

// ---------------------------------------------------------------------------
//
VBlockFileDir& VRscArchiver::getDirectory()
{
	return resourceFile.getDirectory();
}
	
// ---------------------------------------------------------------------------
//
unsigned VRscArchiver::getDirectoryCount()
{
	return unsigned(resourceFile.getDirectory().count());
}

// ---------------------------------------------------------------------------
//
const VBlockFileDirItem *VRscArchiver::getCurrentDirItem()
{
	return resourceFile.getCurrentDirItem();
}

// ---------------------------------------------------------------------------
//
boolean VRscArchiver::contains(const VString& name, VClass *cls)
{
	const char *clsname;
	if (cls == 0) {
		clsname = 0;
	}
	else {
		clsname = cls->nameOf();
	}
	VBlockFileDirItem item(name, VString(clsname));
	item.setWildType(cls == 0);
	return resourceFile.contains(item);
}

// ---------------------------------------------------------------------------
//
void VRscArchiver::remove(const VString& name, VClass *cls)
{
	const char *clsname;
	if (cls == 0) {
		clsname = 0;
	}
	else {
		clsname = cls->nameOf();
	}
	VBlockFileDirItem item(name, VString(clsname));
	item.setWildType(cls == 0);
	resourceFile.remove(item);
}

// ---------------------------------------------------------------------------
//
integer VRscArchiver::write(const char *buf, integer n)
{
	if (!isAbort() && isOpen() && inResource()) {
	 	integer count = resourceFile.write(buf, n);
		if (count < n) {
			abort(WriteFailure);
		}
		return count;
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
integer VRscArchiver::read(char *buf, integer n)
{
	if (!isAbort() && isOpen() && inResource()) {
	 	return resourceFile.read(buf, n);
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
boolean VRscArchiver::putch(const char c)
{
	if (!isAbort() && isOpen() && inResource()) {
	 	boolean ok = resourceFile.putch(c);
		if (!ok) {
			abort(WriteFailure);
		}
	 	return ok;
	}	
	return FALSE;
}

// ---------------------------------------------------------------------------
//
char VRscArchiver::getch()
{
	if (!isAbort() && isOpen() && inResource()) {
	 	return resourceFile.getch();
	}
	return -1;
}

// ---------------------------------------------------------------------------
//
integer	VRscArchiver::putLine(const char *buf)
{
	if (buf == 0) {
		return 0;
	}
	if (!isAbort() && isOpen() && inResource()) {
		// Figure the number of chars that should be written.
		size_t len = strlen(buf);
	 	integer count = resourceFile.putln(buf);
		if (count < integer(len)) {
			abort(WriteFailure);
		}
	 	return count;
	}
	return -1L;
}

// ---------------------------------------------------------------------------
//
integer	VRscArchiver::getTrimLine(char *buf, integer maxBytes)
{
	if (!isAbort() && isOpen() && inResource()) {
	 	return resourceFile.gettrim(buf, maxBytes);
	}
 	return -1L;
}

// ---------------------------------------------------------------------------
// Called at the start of a putObject operation
//
void VRscArchiver::beginWrite()
{
	if (writeFlag == 0 && !isAbort() && isOpen() && inResource()) {
		resourceFile.autoUpdate(FALSE);
	}

	writeFlag++;
}

// ---------------------------------------------------------------------------
// Called at the end of a putObject operation
//
void VRscArchiver::endWrite()
{
	writeFlag--;

	if (writeFlag == 0 && !isAbort() && isOpen() && inResource()) {
		resourceFile.autoUpdate(TRUE);
	}
}

// ---------------------------------------------------------------------------

#endif // CV_NOARCHIVER
