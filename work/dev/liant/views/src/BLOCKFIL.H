// $Header:   Q:/views/common/vcs/BLOCKFIL.H_V   1.9   Mar 24 1997 16:08:14   scottt  $ 

//	blockfil.h
//
//	VBlockFile class interface [Common]
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
//	03/29/93 pmcm	created to have an Oakland bfile.
//	07/09/93 pmcm	re-wrote to have a VFile
//	07/29/93 pmcm	added get/setRevisionNumber(),get/setComment(),getError();
//					find(), findEnd(), contains(), and remove() now take
//					const VBlockFileDirItem& and/or integer dir index.
//	08/04/93 pmcm	changed unsigned new_find to boolean append
//	08/10/93 pmcm	changed some member names, changed some ints to longs;
//					enumerated additional error codes.
//  09/01/93 dgm	M8 changes; fixed signature of read().
//	09/08/93 pmcm	changed BlockChainNotSet error code to NoCurrentItem
//	10/18/93 dgm	UNIX changes; changed name AccessType to BlockAccessType
//					to avoid naming conflict when using cfront 2.1.
//	12/23/93 pkt	changed enum Error to typedef for Symantec compiler.
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	merged.
//	01/17/94 dgm	cfront C++ changes
//	02/24/94 dgm	old C preprocessor changes
//	04/07/94 pmcm	added getCurrentBlockSize(), isOpen(), setErrorCode(),
//					getFileBlockSize(), isError(), peekError().
//	08/08/94 jmd	renamed dirDirty flag, added autoUpdate
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VBLOCKFIL_H
#define VBLOCKFIL_H

// ---------------------------------------------------------------------------
//
#include "object.h"
#include "file.h"
#include "pathstr.h"	/* for VPathString  */
#include "bfdir.h"		/* for VBlockFileDir */
#include "stack.h"		/* for VStack member */

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VOrdCollect;
CLASS VBlockFileDirItem;

// ---------------------------------------------------------------------------
//
CLASS VBlockFile : public VObject {

public:
	enum BSize		{	DefaultSize = 500 	};

	enum BlockAccessType { 	Create, ReadOnly, Update, Temporary };

	typedef enum	{	NoError,
						InvalidAccessType,
						InvalidNameOrTypeName,						
						AccessFailure,
						NotABlockFile, 
						OldBlockFile, 
						InvalidLineTerminator,
						DirItemNotFound,
						NoCurrentItem,
						IoNotOpen,
						IoFailure
	} Error;

	//	constructors/destructor

				   		VBlockFile();
				   		VBlockFile(const VString& fname);
						VBlockFile(BlockAccessType access);
				 		~VBlockFile();

	//	class identity
						
	VClass 	   		   *iam();

	//	opening and closing files

	boolean 			open(BlockAccessType access, const VString& fname);
	boolean 			open(BlockAccessType access); 
	void				close();
	boolean	   			isOpen();

	//	finding named blocks

	boolean 			find(const VBlockFileDirItem& item);
	boolean 			find(integer dirIndex);
	boolean 			findEnd(const VBlockFileDirItem& item);
	boolean 			findEnd(integer dirIndex);

	//	reading
							
	integer		 		read(char *buf, integer len);
	char 				getch(); 

	//	note that the 'buf' for the get*() methods must have the 
	//	capacity for at least 'n' + 1 characters

	integer  			gettrim(char *buf, integer n);
	integer 	 		gets(char *buf, integer n);
	integer  			getln(char *buf, integer n);

	//	writing

	integer				write(const char *data, integer len);
	integer				putln(const char *s);
	boolean				putch(const char c);

	//	directory interrogation/manipulation

	VPathString&				file();
	void						file(const VString& fname);

	VBlockFileDir& 				getDirectory();		
	unsigned 	   	   			getDirectoryCount();
	boolean						updateDirectory();
	const VBlockFileDirItem    *getCurrentDirItem();

	boolean						contains(const VBlockFileDirItem& item) const;
	boolean						remove(const VBlockFileDirItem& item);
	boolean						remove(integer dirIndex);

	boolean						isError() const;
	Error						peekError() const;
	Error						getError();

	long						getRevisionNumber() const;
	void						setRevisionNumber(long revisionNumber);

	const VString&				getComment() const;
	void						setComment(const VString& s);

	void						autoUpdate(boolean tf);

	VFile&						getFile();

protected:
	
	VFile				f;

	void		   		init();	
	boolean				findDirectory();
	boolean				readDirectory();
	boolean				writeDirectory();
	integer				getln(char *buf, integer n, char replaceNewLine);

	long				getFileBlockSize() const;
	long				getCurrentBlockSize() const;
	void				setError(Error e);

private:

	Error				errorCode;

	BlockAccessType	   	fileAccess;					
	VPathString	   	   	fileName;
	VBlockFileDir		dir;
	boolean				dirDirty;

	long				blockFileRevisionNumber;
	long				userRevisionNumber;
	VString				comment;
	long	 			fileBlockSize;
	long 				freeBlock;

	long 				currentBlock;	
	long 				currentNextBlock;	
	long	 			currentBlockSize;
	integer				currentDirItem;

	boolean				append;
	VStack				contextStack;

	boolean				updateHeader;

	boolean				readFileHeader();
	boolean				writeFileHeader();

	boolean 			atBlockData(long blockNumber);
	boolean 			padBlock(long blockNumber);

	boolean 			readBlockHeader(	long blockNumber, 
											long *bytesUsed, 
											long *nextBlockNumber);

	boolean 			writeBlockHeader(	long blockNumber, 
											long bytesUsed, 
											long nextBlockNumber);

	boolean 			deleteBlock(long blockNumber);
	long				newBlock();

	boolean				push();
	boolean				pop();

};

extern VClass *VBlockFileCls;

// ---------------------------------------------------------------------------
//
inline VPathString& VBlockFile::file() 
{
	return f.file();
}

// ---------------------------------------------------------------------------
//
inline void VBlockFile::file(const VString& fname)
{
 	f.file(fname);
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::find(integer dirIndex)
{
	VObject *obj = dir.getItems()->idAt(dirIndex);
	return obj == 0  ? FALSE : find(*((VBlockFileDirItem *) obj));
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::findEnd(integer dirIndex)
{
	VObject *obj = dir.getItems()->idAt(dirIndex);
	return obj == 0  ? FALSE : findEnd(*((VBlockFileDirItem *) obj));
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::remove(integer dirIndex)
{
	VObject *obj = dir.getItems()->idAt(dirIndex);
	return obj == 0  ? FALSE : remove(*((VBlockFileDirItem *) obj));
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::contains(const VBlockFileDirItem& item) const
{
	integer i;
 	return dir.contains(item, i);
}

// ---------------------------------------------------------------------------
//
inline const VString& VBlockFile::getComment() const
{
	return comment;
}

// ---------------------------------------------------------------------------
//
inline long VBlockFile::getRevisionNumber() const
{
	return userRevisionNumber;
}

// ---------------------------------------------------------------------------
//
inline VBlockFileDir& VBlockFile::getDirectory() 
{
	if (dirDirty) {
	 	readDirectory();
	}
	return dir;
}

// ---------------------------------------------------------------------------
//
inline unsigned VBlockFile::getDirectoryCount() 
{
	if (dirDirty) {
	 	readDirectory();
	}
	return (unsigned) dir.count();
}

// ---------------------------------------------------------------------------
//	Return error code and clear error.
//
inline VBlockFile::Error VBlockFile::getError()
{
	VBlockFile::Error e = errorCode;
	errorCode = VBlockFile::NoError;
	return e;
}

// ---------------------------------------------------------------------------
//	Set error code (protected).
//
inline void VBlockFile::setError(VBlockFile::Error e)
{
	errorCode = e;
}

// ---------------------------------------------------------------------------
//
inline long VBlockFile::getCurrentBlockSize() const
{
	return currentBlockSize;
}

// ---------------------------------------------------------------------------
//
inline long VBlockFile::getFileBlockSize() const
{
	return fileBlockSize;
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::isOpen()
{
	return f.isOpen();
}

// ---------------------------------------------------------------------------
//
inline boolean VBlockFile::isError() const
{
 	return errorCode != NoError;
}

// ---------------------------------------------------------------------------
//
inline VBlockFile::Error VBlockFile::peekError() const
{
 	return errorCode;
}

// ---------------------------------------------------------------------------
//
inline VFile& VBlockFile::getFile()
{
	return f;
}

#endif // VBLOCKFIL_H
