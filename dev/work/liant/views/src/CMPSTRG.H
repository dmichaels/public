// $Header:   Q:/views/win/vcs/CMPSTRG.H_v   1.2   Mar 04 1997 14:12:06   gregm  $

//	cmpstrg.h
//
//  Compound Document Storage
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
//	01/30/95 dss	Initial release.
//	01/09/96 dss	VComponent name change.
//	---------------------------------------------------------------------------

#ifndef COMPONENTSTORAGE_H
#define COMPONENTSTORAGE_H

// Views Compound Document Types
#include "oledefs.h"

// OLE Includes
#include "unknown.h" 
#include "storag.h" 

// Views GUI Includes
#include "yesno.h"

// TO DO: This is used by CDStorage and Storage. Find a way
// to disconnect the two.
#define CONTAIN_ISTORAGE_INTERFACE2(prefix) \
public: \
	CONTAIN_ACCESS2(prefix, I, Storage, *Storage) \
protected: \
	prefix##Storage		*Storage;
	
// View Compound Document Includes
#include "storag.h"

//**********************************************************************
//
// VComponentStorage Class
//
//**********************************************************************

CLASS VComponentStorage : public VObject
{
public:	

	VComponentStorage();
	~VComponentStorage();

	virtual LPTSTR		getClassName() = 0;

	// Storage
	CONTAIN_INTERFACE2(CR, Storage)

public:	
	boolean				isRealized() const
						{return hasStorage() && getStorage().isRealized();}

	VString&			getStorageName() const
							{cv_assert(hasStorage()); return (VString&)getStorage().getStorageName();}
	boolean				hasStorageName() const
							{cv_assert(hasStorage()); return getStorage().hasStorageName();}

	boolean				isRoot(void) const
							{return hasStorage() && getStorage().isRoot();}
	boolean				isTemporary(void) const
							{return hasStorage() && getStorage().isTemporary();}
	boolean				isDirty(void) const
							{return hasStorage() && getStorage().isDirty();}
							
	virtual enum yesNoCancel	create()
								{cv_assert(hasStorage()); return getStorage().create() ? YesButton : CancelButton;}
	virtual enum yesNoCancel	open()
								{cv_assert(hasStorage()); return getStorage().open() ? YesButton : CancelButton;}
	virtual enum yesNoCancel	close(boolean bSave, boolean bKeep)
								{cv_assert(hasStorage());
								 return getStorage().close(bSave, bKeep) ? YesButton : CancelButton;}
	virtual enum yesNoCancel	save(boolean deep = FALSE, boolean ignoreErrors = FALSE)
								{cv_assert(hasStorage());
								 return getStorage().save(deep, ignoreErrors) ? YesButton : CancelButton;}
	virtual enum yesNoCancel	rename(const VPathString& name);
	virtual enum yesNoCancel	remove() const
								{cv_assert(hasStorage()); 
								 return getStorage().remove() ? YesButton : CancelButton;}
	virtual enum yesNoCancel	move(VPathString& dst) {return rename(dst);}	// rename alias
	virtual enum yesNoCancel	undo(void)
								{cv_assert(hasStorage());
							 return getStorage().undo() ? YesButton : CancelButton;}

	
	boolean				exists() const
							{cv_assert(hasStorage()); return getStorage().exists();}
							
	OLEResult&			getResult(void) 
							{cv_assert(hasStorage()); return getStorage().getResult();}
							
private:
	void				init(void);
};

//**********************************************************************
//
// VComponentRootStorage Class
//
//**********************************************************************

CLASS VComponentRootStorage : public VComponentStorage
{
public:	

	VComponentRootStorage(VString *lpStorageName = 0);

	LPTSTR			getClassName() {return _T("VComponentRootStorage");}

};

//**********************************************************************
//
// VComponentSubStorage Class
//
//**********************************************************************

CLASS VComponentSubStorage : public VComponentStorage
{
public:	

	VComponentSubStorage(VComponentStorage &storageParent, VString *lpStorageName = 0);

	LPTSTR			getClassName() {return _T("VComponentSubStorage");}
};

#endif // COMPONENTSTORAGE_H

