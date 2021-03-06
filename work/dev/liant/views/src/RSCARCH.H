// $Header:   Q:/views/common/vcs/RSCARCH.H_V   1.8   Mar 24 1997 16:13:16   scottt  $ 

//	rscarch.h
//
//	VRscArchiver class interface [Common]
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
//	06/11/93 pmcm	changed getFileName() to getPathString()
//	07/09/93 pmcm	changed char * params to const VString&;
//					changed getPathString() to file().
//	07/22/93 dgm	added final default boolean argument to getObject()
//					to optionally suppress actual realization of the
//					retrieved object (this only pertains to GUI objects);
//					this was necessary for our dialog-editor.
//	08/03/93 jmd	made getFile() public (for error access)
//  08/09/93 jmd	removed free
//  08/10/93 pmcm	removed classToMatch, closeSets()
// 	08/10/93 pmcm	used new protected interface to classSet and instSet
//	08/12/93 pmcm	removed all the i/o methods that deal with basic types;
//					added read(), write(), getch(), putch(), getTrimLine(),
//					putTrimLine().
//	01/13/94 dgm	merged and preened
//	08/08/94 jmd	added begin/endWrite
//	03/06/95 jld	added #ifndef CV_NOARCHIVER wrapper
//	05/19/95 pkt	selected Windows as common source.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VRSCARCH_H
#define VRSCARCH_H

#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
#include "archiver.h"
#include "blockfil.h"

CLASS VRscArchiver : public VArchiver {

public:
								VRscArchiver();
								VRscArchiver(const VString& fname);
							   ~VRscArchiver();
	VClass	 	   			   *iam();

	//	Opening and closing the archive

	virtual boolean				open(int access);
	virtual boolean				open(int access, const VString& fname);
	virtual void				close();

	//	Directory interrogation/manipulation

	VPathString&				file(); 
	VBlockFileDir&				getDirectory();	
	unsigned					getDirectoryCount();
	const VBlockFileDirItem	   *getCurrentDirItem();

	boolean 	   				contains(	const VString&	name, 
											VClass 		   *cls = 0);

	void						remove(		const VString& 	name, 
											VClass 		   *cls = 0);

	//	VObject archival and retrieval

	virtual boolean 	putObject(	VObject    	   *obj, 
									boolean 		temp 	= FALSE);

	boolean 			putObject(	const VString& 	rscName,
									VObject    	   *obj,  
									boolean 		temp 	= FALSE);
									
	virtual VObject	   *getObject(	VObject    	   *obj);	

 	virtual VObject	   *getObject(	VObject    	   *obj,  
 									VObject 	   *data,
 									boolean			doRealize = TRUE);
	
	VObject    	   	   *getObject(	const VString& 	rscName,
									VObject    	   *obj, 	
									VClass 	   	   *cls		= 0,
									VObject	   	   *data 	= 0,
									boolean			doRealize	= TRUE);

	//	Data archival and retrieval

	virtual char		getch();
	virtual boolean		putch(const char c);
 	virtual integer		read(char *buf, integer n);
	virtual integer		write(const char *buf, integer n);
	virtual integer		getTrimLine(char *buf, integer maxBytes);
	virtual integer		putLine(const char *buf);

	//	Miscellany

	virtual void		rewind();
	VBlockFile&			getFile();

	void				beginWrite();
	void				endWrite();

protected:
	void				init(const VString& fname);
	boolean				inResource();
	void				inResource(boolean b);

private:
	VBlockFile		resourceFile;	
	unsigned short	inResourceFlag : 1;

	int				writeFlag;
};			 		

extern VClass *VRscArchiverCls;

// ---------------------------------------------------------------------------
//
inline VPathString& VRscArchiver::file() 
{
	return resourceFile.file();
}

// ---------------------------------------------------------------------------
//
inline VBlockFile& VRscArchiver::getFile() 
{
	return resourceFile;
}
		
// ---------------------------------------------------------------------------
//
inline boolean VRscArchiver::inResource() 
{
	return inResourceFlag == 0 ? FALSE : TRUE;
}

// ---------------------------------------------------------------------------
//
inline void VRscArchiver::inResource(boolean b) 
{
	if (b) {
		inResourceFlag = 1;
	}
	else {
		inResourceFlag = 0;
	}
}

#endif // CV_NOARCHIVER

#endif // VRSCARCH_H
