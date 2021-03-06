// $Header:   Q:/views/common/vcs/BLOCKFIL.CPV   1.12   Mar 24 1997 16:08:12   scottt  $ 

//	blockfil.cpp
//
//	VBlockFile class implementation [Common]
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
//	07/29/93 pmcm	added time stamp (but not this comment!)
//	08/04/93 pmcm	maintained dir_dirty state;
//					changed unsigned new_find to boolean append;
//					fixed writeDirectory so that it does not append.
//	08/07/93 jmd	added spaces to sprintf formats
//	08/10/93 pmcm	changed some member names;
//					changed block and size ints to integers;
//					changed field widths and spacings in file and block
//					headers, and in directory items;
//					added more error checks.
//	09/01/93 dgm	M8 changes.
//	09/07/93 pmcm	fixed putln() to use "\n", not LINE_TERM
//					incremented BlockFileRevisionNumber
//	09/08/93 pmcm	changed BlockChainNotSet error code to NoCurrentItem
//	10/10/93 jmd	SC6 changes.
//	10/14/93 dgm	M80 changes; increased buffer size in writeFileHeader().
//	10/18/93 dgm	UNIX changes; changed name AccessType to BlockAccessType
//					to avoid naming conflict when using cfront 2.1.
//	11/09/93 pkt	added extern declaration of VBlockContextCls.
//	11/11/93 pmcm	added wsscanf fudge for M80 IN_DLL
//	11/23/93 pkt	changed \n to \x0A, for portability to Mac.
//	11/25/93 pkt	made room for 0 character at end writeBlockHeader's buf.
//	11/30/93 pkt	adjusted time stamp for difference between C++/Views and
//					Macintosh times.
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	merged.
//	01/17/94 dgm	cfront C++ changes
//	01/30/94 dgm	simplified -- expression in gettrim() to
//					avoid apparent Borland C++ 4.0 compiler bug.
//	02/24/94 dgm	old C preprocessor changes
//	03/09/94 pkt	renamed #include of timecls.h to vtime.h.
//	06/02/94 dgm	Simplified ?: expressions in function calls to
//					work-around SPARCworks C++ compiler (4.0) bug.
//	07/12/94 jmd	CV_WIN32: excluded wsscanf fudge for M80 IN_DLL
//	07/12/94 jmd	fixed NoCurrentItem error code in getCurrentDirItem().
//	07/12/94 jmd	reset errorCode from NoCurrentItem to NoError on find().
//	08/08/94 jmd	renamed dirDirty flag, added autoUpdate
//	08/24/94 jmd	updated write logic
//	11/04/94 jmd	fixed problem with append/write logic
//	11/04/94 jmd	sped up padBlock
//	11/07/94 dss	removed dup const on declaration.
//	12/30/94 dgm	Fix in readFileHeader.
//	03/30/95 pkt	updated to force 'f' to operate byte-wise.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	09/06/95 pkt	updated Mac discriminator to V_MACINTOSH.
//	04/04/96 pkt	cycle test to prevent hangs in 'deleteBlock' and 'read'.
//	04/05/96 pkt	fixed 'write' to not create chains with cycles, even
//					temporarily.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#include "blockfil.h"
#include "iterator.h"
#include "ordcllct.h"
#include "str.h"

#ifdef V_MACINTOSH
#	include "vtime.h"
#endif

extern "C" {
#	include <time.h>	/* for time() */
#	include <string.h>	/* for strlen() */
#	include <stdio.h>	/* for sprintf(), sscanf() */
#	if _MSC_VER >= 800
#		if defined(IN_DLL) && !defined(CV_WIN32) 
#			include "wsscanf.h"	/* because M80 lacks this IN_DLL context */
#			define sscanf wsscanf
#		endif
#	endif
}

// ---------------------------------------------------------------------------
//	Constants
// ---------------------------------------------------------------------------

//	The revision number of this block file (stored in file header).
//
static const integer 	BlockFileRevisionNumber = 2L;

static char *const 	FileCommentPad 		=
	"                                                  \x0A";
//	 012345678901234567890123456789012345678901234567890  
//	 0         1         2         3         4         5
// -------------------------------------------------------

static const int	FileCommentPadLen	= strlen(FileCommentPad);

//	String containing the character used to pad out partial blocks.
//
#if 0
static char *const 	BlockPad	 		= " ";
#else
static const char BlockPadChar	= ' ';
#endif

//	The file and block header lengths are determined by the 
//	values hard-coded in writeFileHeader()/readFileHeader() and 
//	writeBlockHeader()/readBlockHeader(), respectively.
//
static const int 	FileHeaderLen		= 98;
static const int 	BlockHeaderLen		= 35;

//	The length of an item in the file directory is determined by the
//	values hard-coded in writeDirectory()/readDirectory().  Two of the
//	fields, the maximum length of the name of an item, and the maximum
//	length of the typename of an item are broken out here for easier
//	modification.
//
static const int 	DirItemName 	   	= 50;
static const int 	DirItemTypeName    	= 50;
static const int 	DirItemLen		   	= DirItemName
											+ DirItemTypeName
											+ 29;

static const integer DirItemBad 			= -1L;
static const integer FileDirectoryBlock 	=  0L;
static const integer FileBadBlock 	   		= -1L;
static const integer FileDirectoryHandle	= -2L;
static const integer FileNoMoreBlocks   	= -3L;

// ---------------------------------------------------------------------------
//	Little class for push() and pop()

CLASS VBlockContext : public VObject {

public:

	integer 	pos;		//	file position
	integer		index;	 	//	index in directory
	integer 	block;		//	blocknumber
	integer  	used;		//	bytes used
	integer 	next;		//	next block
	boolean		append;		//	append to current block on write?

			VBlockContext();

			VBlockContext(integer position, integer dirIndex, 
							integer blockNumber, integer bytesUsed, 
							integer nextBlock, 	boolean appendFlag);

			~VBlockContext();

	VClass *iam();
};

extern VClass *VBlockContextCls;

// ---------------------------------------------------------------------------
//
defineClass(VBlockContext, VObject)

// ---------------------------------------------------------------------------
//
VBlockContext::VBlockContext() 
		: 	pos(-1L), index(DirItemBad),	
			block(FileBadBlock), used(0), next(FileNoMoreBlocks), 
			append(0)
{
	;	
}

// ---------------------------------------------------------------------------
//
VBlockContext::VBlockContext(integer position, integer dirIndex,
								integer blockNumber, integer bytesUsed, 
								integer nextBlock, boolean appendFlag)
		: 	pos(position), index(dirIndex), 
			block(blockNumber), used(bytesUsed), next(nextBlock), 
			append(appendFlag)
			
{
	;
}

// ---------------------------------------------------------------------------
//
VBlockContext::~VBlockContext() 
{
	;
}


// ---------------------------------------------------------------------------
//	Block File class definition

defineClass(VBlockFile, VObject)

// ---------------------------------------------------------------------------
//	Construct a VBlockFile object that is not bound to any file.
//
VBlockFile::VBlockFile()
{
	init();
}

// ---------------------------------------------------------------------------
//	Construct a VBlockFile on the file of the specified name. 
//
//	The filename can be supplied as a C string, VString, or VPathString.
//	Internally, the filename is maintained as a VPathString (accessible
//	by the file() method) so that you may easily perform directory and path 
//	operations.
//
VBlockFile::VBlockFile(const VString& fname)
{
	init();
	f.file(fname);
}

// ---------------------------------------------------------------------------
//	Construct a BlockFile which is open on a special file.
//	Currenty, this means only a Temporary file.
//
VBlockFile::VBlockFile(BlockAccessType access)
{
	init();

	switch (access) {
	case Temporary:
		open(access, VString(""));	
		break;

	default:
		break;
	}
}

// ---------------------------------------------------------------------------
//	
VBlockFile::~VBlockFile()
{
	close();
}

// ---------------------------------------------------------------------------
//	Common initializer for the VBlockFile constructors
//
void VBlockFile::init() 
{
	updateHeader = TRUE;
	dirDirty = TRUE;
	userRevisionNumber = 0L;
	blockFileRevisionNumber = BlockFileRevisionNumber;
	errorCode = VBlockFile::NoError;
	comment.reset();
	f.asByteString(TRUE);
}

// ---------------------------------------------------------------------------
//	Open this VBlockFile on the file of the specified name with the given
//	AcessType.  The access type is one of the following:
//	
//		Create
//		ReadOnly
//		Update  
//
//	The filename can be supplied as a C string, VString, or VPathString.
//	Internally, the filename is maintained as a VPathString (accessible
//	by the file() method) so that you may easily perform directory and path 
//	operations.
//
boolean VBlockFile::open(BlockAccessType access, const VString& fname)
{
	if (fname.size() == 0) {
		f.file((char *)0);
	}
	else {
		f.file(fname.gets());
	}
	return open(access);
}

// ---------------------------------------------------------------------------
//	Open this VBlockFile on its current file with the given BlockAccessType.
//	The access type may be one of the following:
//
//		Create
//		ReadOnly	
//		Update  
//		Temporary (makes up name, read/write)
//
boolean VBlockFile::open(BlockAccessType access)
{
	enum fileMode m;

	switch (access) {		//	translate to VFile fileMode
	case Temporary:		
		m = ::Temporary;
		break;
	case ReadOnly:
		m = ::ReadOnly;
		break;
	case Create:
		m = ::Create;
		break;
	case Update:
		m = ::Update;
		break;
	default:
		errorCode = InvalidAccessType;
		return FALSE;
	}

	close();
	if (!f.open(m)) {
		errorCode = AccessFailure;
	 	return FALSE;
	}

	if ((fileAccess = access) == Create) {	
		//	we need to read and write new files

		f.close();
		fileAccess = Update;
		if (!f.open(::Update)) {
			errorCode = AccessFailure;
		 	return FALSE;
		}
	}

	if (comment.size() == 0) {
		comment   		= "C++/Views VBlockFile\x0A";
	}
	fileBlockSize 		= (integer) VBlockFile::DefaultSize;
	currentDirItem 		= DirItemBad;
	currentBlock 		= FileBadBlock;
	currentNextBlock 	= FileNoMoreBlocks;
	currentBlockSize 	= 0L;
	append	 			= FALSE;
	freeBlock 	 		= FileNoMoreBlocks;

	dir.clearContents();
	dirDirty = TRUE;

	if (fileAccess == Update && access == Create) {
		if (writeFileHeader()) {
			integer block = newBlock();
			if (block != FileNoMoreBlocks && block == FileDirectoryBlock) {
				currentBlock = block;
				return TRUE;
			}
		}
	}
	else if (readFileHeader() && readDirectory()) {		
		return TRUE;
	}

	f.close();
	return FALSE;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFile::updateDirectory()
{
	if (!f.isOpen()) {
		errorCode = IoNotOpen;
	 	return FALSE;
	}
	writeDirectory();
	dirDirty = FALSE;
	return TRUE;
}

// ---------------------------------------------------------------------------
//
void VBlockFile::close()
{
	if (dirDirty) {
	 	writeDirectory();
	}
	dirDirty = TRUE;
	f.close();	
	comment.reset();
	errorCode = VBlockFile::NoError;
}

// ---------------------------------------------------------------------------
//	Finds the start of the named block chain.  If the chain doesn't exist 
//	then it is created. 
//
//	Returns TRUE, if able to find 'name'; FALSE, otherwise.
//							   
//	Note that this function will fail if of the following are true:
//
//		'name' contains more than DirItemName (50) characters
//		'type' contains more than DirItemTypeName (50) characters
//		'name' is empty
//		'name' or 'type' contain a newline ('\x0A')
//
boolean VBlockFile::find(const VBlockFileDirItem& dirItem)
{
	if (!f.isOpen()) {
		errorCode = IoNotOpen;
	 	return FALSE;
	}

	integer typeSize;
	if (dirItem.getName().size() == 0							||
		dirItem.getName().size() > DirItemName	 				|| 
		(typeSize = dirItem.getType().size()) > DirItemTypeName	|| 
		dirItem.getName().charIn('\x0A') 						||
		dirItem.getType().charIn('\x0A') 							) {

		errorCode = InvalidNameOrTypeName;
		return FALSE;
	}

 	append = FALSE;
	
	//	ask the directory if it contains the named item.
	//	if the given type name is empty, then ask it to skip type checking.

	VBlockFileDirItem item(dirItem);
	item.setWildType(typeSize == 0);
	if (dir.contains(item, currentDirItem)) {	//	(sets currentDirItem)
		//	found! 

		VBlockFileDirItem *i = (VBlockFileDirItem *) 
								dir.getItems()->idAt(currentDirItem);

		currentBlock = i->getStartBlock();
	}
	else {
		//	not found, create new block chain

		currentBlockSize = 0L;
		currentBlock = newBlock();
		currentNextBlock = FileNoMoreBlocks;

		item.setStartBlock(currentBlock);
		item.setSequence(dir.count());
#ifdef V_MACINTOSH
		item.setTime(VTime().asSeconds());
#else
		item.setTime((long) time(0));
#endif
		item.setWildType(FALSE);
		dir.addItem(item);
		dirDirty = TRUE;

		if (!writeBlockHeader(currentBlock, currentBlockSize, 
			currentNextBlock)) {

			return FALSE;
		}

		//	write something to the end of the new block 
		//	so it can't be gotten as free

		if (!atBlockData(currentBlock) 				||	
			f.at(f.at() + fileBlockSize - 1) == -1L ||
			f.write("x", (integer) 1) <= 0				) {

			errorCode = errorCode == NoError ? IoFailure : errorCode;
			return FALSE;
		}
		atBlockData(currentBlock);
		push();
		writeDirectory();
		pop();
		errorCode = NoError;
		return TRUE;
	}

	if (!readBlockHeader(currentBlock, &currentBlockSize, &currentNextBlock)) {
		return FALSE;
	}
	errorCode = NoError;
	return TRUE;
}
	
// ---------------------------------------------------------------------------
//	Finds the end of the named block chain.
//	If the chain doesn't exist then it is created. 
//	Returns TRUE, if able to find 'name'; FALSE, otherwise.
//							   
boolean VBlockFile::findEnd(const VBlockFileDirItem& dirItem)
{
	if (!find(dirItem)) {
		return FALSE;
	}

	append = TRUE;

	while (currentNextBlock != FileNoMoreBlocks) {
		currentBlock = currentNextBlock;
	}

	if (!readBlockHeader(currentBlock, &currentBlockSize, &currentNextBlock) ||
		!padBlock(currentBlock)											) {

		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Sets the directory the current block, if found.
//
boolean VBlockFile::findDirectory()
{
	currentBlock 	= FileDirectoryBlock;
	currentDirItem	= FileDirectoryHandle;

	return readBlockHeader(currentBlock, &currentBlockSize, &currentNextBlock);
}

// ---------------------------------------------------------------------------
//	Removes the first occurence of the named, typed block chain.
//
//	Note that an entry in the directory that bears a type is not removed
//	if that type is not specified in the item given to to this function.  
//
boolean VBlockFile::remove(const VBlockFileDirItem& dirItem)
{
	integer i;
	if ((i = dir.indexOf(dirItem)) == DirItemBad) {
		errorCode = DirItemNotFound;
		return FALSE;
	}

	VBlockFileDirItem *itemp = (VBlockFileDirItem *) dir.getItems()->idAt(i);

	if (deleteBlock(itemp->getStartBlock()) 	&&
		dir.getItems()->removeAt(i) != 0		&&
		push()									&&
		writeDirectory()						&&	
		pop()										) {

		return TRUE;
	}
	errorCode = errorCode == NoError ? DirItemNotFound : errorCode;
	dirDirty = TRUE;
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Reads, at most, 'n' bytes of data into 'buf'. 
//	Returns the number of bytes read.
//
integer VBlockFile::read(char *buf, integer n)
{
	if (!f.isOpen()) {
		errorCode = IoNotOpen;
	 	return 0;
	}

	if (currentBlock == FileBadBlock) {
		errorCode = NoCurrentItem;
		return 0;
	}

	char *b;
	integer size;
	integer work_size;
	integer readN = 0;

	// Prevent hangs (caused by a corrupted block file)
	// by checking for cycles as the blocks are read.
	long current_max_block_cycle = 0; // looking for cycles up to this size
	long current_block_cycle = 0;
	long seen_block = FileNoMoreBlocks;

	for (b = buf, readN = 0; readN < n; ) {
		work_size = currentBlockSize;

		if ((size = n - readN) < work_size) {
			size = f.read(b, size);
			readN += size;
			currentBlockSize -= size;
			break;
		}				

		work_size = f.read(b, work_size);
		readN += work_size;
		b += work_size;

		if (   currentNextBlock == FileNoMoreBlocks
			|| currentNextBlock == seen_block) {
			currentBlock = FileBadBlock;
			currentBlockSize = 0L;
			break;
		}	 
	
		currentBlock = currentNextBlock;

		if(!readBlockHeader(currentBlock, &currentBlockSize, &currentNextBlock)) {
			return 0;
		}

		current_block_cycle += 1;
		if (current_block_cycle > current_max_block_cycle) {
			seen_block = currentBlock;
			current_block_cycle = 1;

			// Gradually increase the size of cycles detectable.
			current_max_block_cycle += 1;
		}
	}

	return readN;
}

// ---------------------------------------------------------------------------
//	Red and return a single character from this BlockFile
//	Returns -1, if unable to read a character.
//
char VBlockFile::getch()
{
	char c;

	if (read(&c, 1) < 1) {
		return -1;
	}
	return (c & 0377);	//	valid character values 0 - 255 (0x00 to 0xFF)
}

// ---------------------------------------------------------------------------
//	Reads a line or, at most, 'n' bytes of data and discards the newline, 
//	if present. If a complete line is read, then 'buf' contains, 
//	at most, 'n' - 1 bytes of data and terminating '\0'.
//	If the end of a line is not reached, then 'buf' contains 'n' 
//	bytes of data and a terminating '\0'.  This means that 'buf'
//	must be at least 'n' + 1 characters in length;
//
//	Returns the number of bytes read.
//
//	Note that 'buf' must be a character array of at least 
//	'n' + 1 characters!
//
integer VBlockFile::gettrim(char *buf, integer n)
{
	integer i = getln(buf, n); 
	while (i) {
		if (buf[i - 1] != '\x0A') {
			break;
		}
		else {
			i--;
			buf[i] = '\0';
		}
	}
	return i;						//	count, exclusive of newline.
}

// ---------------------------------------------------------------------------
//	Read up to 'n' bytes of data from this BlockFile at the current position
//	into 'buf'.  Add a NULL byte to 'buf' and return the number of bytes 
//	read.  The return value will be less than 'n' if the end of this 
//	BlockFile chain was reached.
//
//	Returns the number of bytes read.
//
//	Note that 'buf' must be a character array of at least 
//	'n' + 1 characters!
//
integer VBlockFile::gets(char *buf, integer n)
{
	integer i = read(buf, n);
	buf[i] = '\0';
	return i;
}

// ---------------------------------------------------------------------------
//	Reads a line or, at most, 'n' bytes of data.  
//	If a complete line is read, then 'buf' contains, at most, 'n' - 1 
//	bytes of data, the newline character, and a terminating '\0'.
//	If the end of a line is not reached, then 'buf' contains 'n' 
//	bytes of data and a terminating '\0'. 
//
//	Returns the number of bytes read, inclusive of the newline.
//
//	Note that 'buf' must be a character arrary of at least 
//	'n' + 1 characters!
//
integer VBlockFile::getln(char *buf, integer n)
{
	integer i = getln(buf, n, '\x0A');  //	read a line or n bytes
	buf[i] = '\0';						//	the null terminate it 
	return i;							//	count, inclusive of newline
}

// ---------------------------------------------------------------------------
//	Reads a line (delimited by the '\x0A' characters) or 'n' bytes--which ever 
//	comes first--into 'buf'.  Replaces the newline, if any, with 'end_char'.
//	Returns the number of bytes read, inclusive of the newline.
//
//	Note that 'buf' must have the capacity for 'n' bytes.
//
integer VBlockFile::getln(char *buf, integer n, char replaceNewLine)
{
	char *p;
	integer count;
	
    if (n != 0) {			
		*buf = replaceNewLine;	// the buf must be delimited, even if no read.
	}

	for (p = buf, count = 0; read(p, 1) == 1; p++) {
	
		count++;
		if (*p == '\x0A') {
			*p = replaceNewLine;
			break;
		}
		else if (count == n) {
			break;
		}
	}	

	return count;
}

// ---------------------------------------------------------------------------
//	Saves 'len' bytes of 'data' into the current block in the current 
//	block chain.
//	Returns the number of bytes written.
//	
integer VBlockFile::write(const char *data, integer len)
{
	if (!f.isOpen()) {
		errorCode = IoNotOpen;
		return FALSE;
	}

	if (currentBlock == FileBadBlock) {
		errorCode = NoCurrentItem;
		return 0;
	}
	
	//	timestamp the current directory item on each write
	if (updateHeader) {
		VBlockFileDirItem *currd = (VBlockFileDirItem *) 
									dir.getItems()->idAt(currentDirItem);
		if (currd != 0) {
#ifdef V_MACINTOSH
			currd->setTime(VTime().asSeconds());
#else
			currd->setTime((long) time(0));
#endif
			dirDirty = TRUE;
		}

		// jmd 08/08/94
		if (len == 0) {
			// update header only, move stream pointer to end of block
			if (!writeBlockHeader(currentBlock, currentBlockSize, FileNoMoreBlocks) ||
				!f.at(f.at() + (integer) currentBlockSize))	{
				errorCode = errorCode == NoError ? IoFailure : errorCode;
			}

			return 0;
		}
	}

	if (currentNextBlock != FileNoMoreBlocks) {
		//	throw away the rest of the block chain
		if (!deleteBlock(currentNextBlock)) {
			return 0;
		}
		currentNextBlock = FileNoMoreBlocks;

		// jmd 11/04/94
		// write the new block header
		// in order to move the stream pointer
		currentBlockSize = 0;
		if (!writeBlockHeader(currentBlock, currentBlockSize, FileNoMoreBlocks)) {
			errorCode = (errorCode == NoError) ? IoFailure : errorCode;
			return 0;
		}
	}

	if (!append) {
		currentBlockSize = 0;

		// note: padBlock restores the stream position
		if (!padBlock(currentBlock)) {
		 	return 0;		//	cleanup of deleteBlock?
		}
		append = TRUE;
	}

	integer block;
	integer work_size;
	integer new_size;
	integer wroteN = 0;

	while (len > 0) {

		if (currentBlockSize == fileBlockSize) {
			//	block is full, spin off a new one

			block = newBlock();				//	FileBadBlock check?
	
			if (!writeBlockHeader(currentBlock, currentBlockSize, block)) {
				errorCode = (errorCode == NoError) ? IoFailure : errorCode;
				return 0;			  		//	cleanup?
			}
			currentBlock = block;
			currentBlockSize = 0;

			if (!updateHeader) {
				// write the new block header
				// in order to move the stream pointer

				if (!writeBlockHeader(currentBlock, currentBlockSize, FileNoMoreBlocks)) {
					errorCode = (errorCode == NoError) ? IoFailure : errorCode;
					return 0;
				}
			}
		}		

		//	write as much of the data as will fit in the current block

		work_size = (len > fileBlockSize - currentBlockSize) ? 
							fileBlockSize - currentBlockSize : len;

		//	update the block header's used byte count and append the data

		new_size = currentBlockSize + work_size;

		// if we are in updateHeader mode, we must update the block
		// header on every write and reset the stream pointer,
		// otherwise we assume the stream pointer is in the correct
		// position

		if ((updateHeader && !writeBlockHeader(currentBlock, new_size, FileNoMoreBlocks))	||
			(updateHeader && !f.at(f.at() + (integer) currentBlockSize))					||
			!f.write(data, (integer) work_size)	 	 					  ) {

			errorCode = (errorCode == NoError) ? IoFailure : errorCode;
			return 0;
		}

		//	get ready for next chunk to write
		
		wroteN += work_size;
		len -= work_size;
		data += work_size;
		currentBlockSize = new_size;
	}
	return wroteN;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFile::putch(const char c)
{
	char buf[2];

	buf[0] = c;
	buf[1] = '\0';

	return write(buf, 1) == 1 ? TRUE : FALSE;
}

// ---------------------------------------------------------------------------
//	Write the given string to the file, terminated by a newline.
//
integer VBlockFile::putln(const char *s)
{
	integer n = 0;
	if (s != 0 && (n = strlen(s)) > 0) {
		write(s, n);
		s += n - 1;
		if (*s == '\x0A') {
			return n;
		}
	}
	
	return n + write("\x0A", 1);
}

// ---------------------------------------------------------------------------
//	Returns a pointer to a constant VBlockFileDirItem representing the 
//	directory entry for the current bfile block chain;
//	-1, if there is no current block or if this VBlockFile is not open 
//	on a bfile.
//
const VBlockFileDirItem *VBlockFile::getCurrentDirItem()
{
	if (currentDirItem == DirItemBad) {
		errorCode = NoCurrentItem;
		return 0;
	}
	const VBlockFileDirItem *item = (const VBlockFileDirItem *) 
								dir.getItems()->idAt(currentDirItem);
	if (item == 0) {
		errorCode = DirItemNotFound;
	}
	return item;
}

// ---------------------------------------------------------------------------
//	Re-writes the bfile directory contents from the VBlockFileDir member.
//
boolean VBlockFile::writeDirectory()
{
	if (!f.isOpen()) {
		errorCode = IoNotOpen;
	 	return FALSE;
	}
	if (!findDirectory()) {
		return FALSE;
	}

	boolean old_append = append;
	append = FALSE;

	//	the directory contents is just block file data
	//	use	VBlockFile::write(), not VFile::write()

	VStream buf((DirItemLen + 1) * dir.count());
	char littleBuf[DirItemLen + 1];
	DO(*(dir.getItems()), VBlockFileDirItem, item) 
		sprintf(littleBuf, "% 10ld % 10ld % 5d %s %s\x0A",	
								item->getStartBlock(),
					 			item->getTime(),
					 			(int) 	item->getType().size(), 
								item->getType().gets(),	
								item->getName().gets());
		buf += littleBuf;
		*littleBuf = '\0';
	END
	
	if (dir.count() == 0) {
		//	an empty directory needs a newline since readDirectory()
		//	read lines!

		currentBlockSize = 0;	//	set up for write()
		buf = "\x0A";
	}

	write(buf, buf.size());	
	append = old_append;
	dirDirty = TRUE;

	return TRUE;
}

// ---------------------------------------------------------------------------
//	Reads bfile directory information into our VBlockFileDir member.
//
boolean VBlockFile::readDirectory()
{
	dir.clearContents();

 	currentBlock = FileDirectoryBlock;
	if (!findDirectory()) {
		return FALSE; 
	}

	VString name;
	VString type;
	name.expandTo(DirItemName + 1);
	type.expandTo(DirItemTypeName + 1);

	//	since the block directory is just bfile data, we must use
	//	VBlockFile::read(), not VFile::read().

	integer block = FileBadBlock;
	integer typelen = 0;
	long 	timestamp = 0L;
	char 	buf[DirItemLen + 1];
	char   *bufp = buf;

	while (getln(bufp, DirItemLen, '\0') > 1) {
		sscanf(bufp, "%10ld %10ld %5d\x0A", &block, &timestamp, &typelen);		
		bufp += 28;
		bufp[typelen] = '\0';	//	replace newline for VString assignment
		type = bufp;
		name = bufp + typelen + 1;

		VBlockFileDirItem *item = new VBlockFileDirItem(name, type, block);
		item->setTime(timestamp);
		item->setSequence(dir.count());
		dir.addItem(item);
		bufp = buf;
	}

	dirDirty = FALSE;

	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFile::push()
{
	integer pos = f.at();
	if (pos == -1L) {
		errorCode = IoFailure;
		return FALSE;
	}
	VBlockContext *bc = new VBlockContext(pos, currentDirItem, 
								currentBlock, currentBlockSize, currentNextBlock, 
								append);

	contextStack.push(bc);
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFile::pop()
{
	VBlockContext *bc = (VBlockContext *) contextStack.pop();

	if (bc != 0) {
		currentDirItem 		= bc->index;
		currentBlock 		= bc->block;
		currentBlockSize 	= bc->used;
		currentNextBlock 	= bc->next;
		append 				= bc->append;

		if (f.at(bc->pos) > 0) {
			return TRUE;
		}
	}
	errorCode = IoFailure;
	return FALSE;
}

// ---------------------------------------------------------------------------
//	Position block file at the start of the data portion of the given block 
//	(i.e., the byte after the block header).
//
boolean VBlockFile::atBlockData(integer blockNumber)
{
	if (f.at((integer) (FileHeaderLen + BlockHeaderLen + 
		(blockNumber * (fileBlockSize + BlockHeaderLen)))) == -1L) {
		
		errorCode = IoFailure;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//
boolean VBlockFile::padBlock(integer blockNumber)
{
	integer wasAt = f.at();					//	remember where we are

	//	skip the used bytes of the current block

	integer size;
	integer next;
	if (blockNumber == currentBlock) {
		size = currentBlockSize;
		if (!atBlockData(blockNumber)) {
			return FALSE;
		}
	}
	else if (!readBlockHeader(blockNumber, &size, &next)		) {		
		return FALSE;
	}
	if (f.at(f.at() + ((integer) size)) == -1) {
		errorCode = IoFailure;
		return FALSE;
	}

	//	pad to the end of the block

#if 0
	for (integer i = size; i < fileBlockSize; i++) {
		if (f.write(BlockPad, (integer) 1) <= 0) {
			errorCode = IoFailure;
			return FALSE;
		}
	}
#else
	// newer, faster.  jmd 11/04/94
	integer	padlen = fileBlockSize - size;
	char *padding = new char[padlen];
	memset(padding, BlockPadChar, (size_t) padlen);

	if (f.write(padding, padlen) <= 0) {
		errorCode = IoFailure;
		delete padding;
		return FALSE;
	}

	delete padding;
#endif

	if (f.at(wasAt) == -1L) {
		errorCode = IoFailure;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Frees all blocks in given block chain by adding them to the chain of 
//	free blocks.
//
boolean VBlockFile::deleteBlock(integer blockNumber)
{
	if (freeBlock == FileNoMoreBlocks) {
		freeBlock = blockNumber;
	}
	else {
		//	go to end of given block chain
							
		integer block;
		integer size;
		integer next = blockNumber;

		// Prevent hangs (caused by a corrupted block file)
		// by checking for cycles as the blocks are read.
		long current_max_block_cycle = 0; // looking for cycles up to this size
		long current_block_cycle = 0;
		long seen_block = FileNoMoreBlocks;

		while (next != FileNoMoreBlocks) {
			if (next == seen_block) {
				// corrupt block chain; do not add to free list
				return FALSE;
			}

			block = next;
			if (!readBlockHeader(block, &size, &next)) {
				return FALSE;
			}
			current_block_cycle += 1;
			if (current_block_cycle > current_max_block_cycle) {
				seen_block = block;
				current_block_cycle = 1;

				// Gradually increase the size of cycles detectable.
				current_max_block_cycle += 1;
			}
		}

		//	add the given block chain at the head of the free blocks chain

		if (!writeBlockHeader(block, size, freeBlock)) {
			return FALSE;
		}
		freeBlock = blockNumber;   				
	}

	return writeFileHeader();
}


// ---------------------------------------------------------------------------
//	Gets a block from the free block chain.
//
integer VBlockFile::newBlock() 
{
	integer blck = FileNoMoreBlocks;

	if (freeBlock == FileNoMoreBlocks) {
		//	the free blocks chain is exhausted.
		//	create a new one by writing at the end of the file

		integer size = f.size() - ((integer) FileHeaderLen);
		if (size > 0) {
			blck = (int) (size / ((integer) (fileBlockSize + BlockHeaderLen)));
		}
		else if (size == 0) {
		 	blck = 0;
		}
		if (blck >= 0) {
			writeBlockHeader(blck, 0, FileNoMoreBlocks);
			padBlock(blck);	
		}
	}
	else {
		//	remove a block from the head of the free chain

		integer size;
		integer next;

		blck = freeBlock;
		readBlockHeader(freeBlock, &size, &next);			
		writeBlockHeader(freeBlock, 0, FileNoMoreBlocks);	
		padBlock(freeBlock);
		freeBlock = next;									
		writeFileHeader();
	}

	return blck;
}

// ---------------------------------------------------------------------------
void VBlockFile::setRevisionNumber(integer revisionNumber)
{
	if (userRevisionNumber != revisionNumber && f.isOpen()) {
		userRevisionNumber = revisionNumber;
		push();
		writeFileHeader();
		pop();
	}
	else {
		userRevisionNumber = revisionNumber;
	}
}

// ---------------------------------------------------------------------------
void VBlockFile::setComment(const VString& s)
{
	if (comment != s.gets() && f.isOpen()) {
		comment = s;
		push();
		writeFileHeader();
		pop();
	}
	else {
		comment = s;
	}
}

// ---------------------------------------------------------------------------
//	Write the file header.
//	The file header contains three lines, as follows.
//	
//	BlockFileRevision UserRevision SignatureRevisionChecksum\x0A
//	|                ||           ||                        |  
//	|                ||           ||                         ---- newline  
//	|                ||           | ---- 2 digits
//	|                ||            ---- blank
//	|                | ---- 10 digits
//	|                 ---- blank
//	 ---- 10 digits
//
//	Comment\x0A
// 	|      |
// 	|       ---- newline
// 	 ---- (FileCommentPadLen - 1) (50) characters, right-padded with blanks
//
//	SizeofABlock BlockNumberThatStartsFreeBlockChain\x0A
//	|           ||                                  | 
//	|           ||                                   ---- newline
//	|           | ---- 10 digits
//	|            ---- blank
//	 ---- 10 digits
//
//	NOTE:  The field widths of the file header are hard-coded in this
//	routine.  If you alter the field widths, you must update the value of the
//	FileHeaderLen constant and readFileHeader().
//
boolean VBlockFile::writeFileHeader()
{	
	if (f.at(0) != 0) {
		errorCode = IoFailure;
		return FALSE;
	}

	char buf[FileHeaderLen+1024];	// 10.15.93, dgm
	char *bufp = buf;

	//	Revision numbers for block file and user

	sprintf(bufp, "% 10ld % 10ld ",
			BlockFileRevisionNumber, userRevisionNumber);

	//	Signature checksum of revision numbers

	unsigned char c = 0;
	for (int i = 0; i < 22; i++, bufp++) {
		c = int(c + *bufp) > 0xFF ? int(*bufp - (0xFF - int(c))) : c + *bufp;
	}
	sprintf(bufp, "% 2x\x0A", (unsigned int) c);
	bufp += 3;

	//	Comment

	int len = int(comment.size());
	len = len > FileCommentPadLen ? FileCommentPadLen : len; 
	strncpy(bufp, comment.gets(), len);
	bufp += len;
	strncpy(bufp, FileCommentPad + len, FileCommentPadLen - len);	
	bufp += FileCommentPadLen - len;

	//	Block size and Free block chain

	sprintf(bufp, "% 10ld % 10ld\x0A", fileBlockSize, freeBlock);
	f.write(buf, (integer) FileHeaderLen);

	if (f.at() != FileHeaderLen) {
		errorCode = IoFailure;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Read the file header.
//	See the comment to writeFileHeader() for the format of the file header.
//
//	NOTE:  The field widths of the file header are hard-coded in this
//	routine.  If you alter the field widths, you must update the value of the
//	FileHeaderLen constant and writeFileHeader().
//	
boolean VBlockFile::readFileHeader()
{
	if (f.at(0) != 0) {
		errorCode = IoFailure;
		return FALSE;
	}

	char buf[FileHeaderLen+1024];
	buf[0] = '\0';
	char *bufp = buf;

	f.read(bufp, FileHeaderLen);

	//	Revision numbers for block file and user
	//	Signature checksum of revision numbers

	unsigned int check;
	sscanf(bufp, "%10ld %10ld %2x\x0A", &blockFileRevisionNumber, 
										&userRevisionNumber,
										&check);

	//	Error checks

	unsigned char c = 0;
	for (int i = 0; i < 22; i++, bufp++) {
		c = int(c + *bufp) > 0xFF ? int(*bufp - (0xFF - c)) : c + *bufp;
	}
	bufp += 2;	// (skip signature checksum)

	if (c != (unsigned char) check) {
	 	errorCode = NotABlockFile;			//	checksum failure
		return FALSE;	
	}

	if (blockFileRevisionNumber != BlockFileRevisionNumber) {
		errorCode = OldBlockFile;			//	out-dated block file format
	 	return FALSE;
	}	

	if (*bufp++ != '\x0A') {
		errorCode = InvalidLineTerminator;	//	file transfer may have messed up
		return FALSE;					//	newlines
	}

	//	Comment

	bufp[FileCommentPadLen - 1] = '\0'; //	(replace newline for scan)
	comment = (char *) bufp;
	bufp += FileCommentPadLen;

	//	Block size and Free block chain

	bufp[21] = '\0';					//	(replace newline for scan)
	sscanf(bufp, "%10ld %10ld", &fileBlockSize, &freeBlock);

	if (f.at() != FileHeaderLen) {
		errorCode = IoFailure;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Finds the given block and writes given information to the block header.
//	After a successful write, the file is positioned at the start of the
//	block data.
//
//	Return TRUE, if successful; FALSE, if unable to locate the given block
//	or an I/O error occurs.
//
//	The block header looks like this:
//	
//	\x0A!BlockNumber BytesUsedInBlock NextBlockNumber\x0A
//	| ||          ||               ||               |  
//	| ||          ||               ||                ---- newline  
//	| ||          ||               | ---- 10 digits
//	| ||          ||                ---- blank
//	| ||          | ---- 10 digits
//	| ||           ---- blank
//	| | ---- 10 digits
//	|  ---- bang
//	 ---- newline
//
//	NOTE:  The field widths of the block header are hard-coded in this
//	routine.  If you alter the field widths, you must update the value of the
//	BlockHeaderLen constant and readBlockHeader().
//
boolean VBlockFile::writeBlockHeader(integer blockNumber, integer bytesUsed, 
									integer nextBlockNumber) 
{
	if (f.at((integer) (FileHeaderLen + 
		(blockNumber * (fileBlockSize + BlockHeaderLen)))) == -1L) {

		errorCode = IoFailure;
		return FALSE;
	}

	char buf[BlockHeaderLen + 1];
	sprintf(buf, "\x0A!% 10ld % 10ld %10ld\x0A", 
				blockNumber, bytesUsed, nextBlockNumber);

	if (f.write(buf, (integer) BlockHeaderLen) == -1L) {
		errorCode = IoFailure;
		return FALSE;
	}
	return TRUE;
}

// ---------------------------------------------------------------------------
//	Finds the given block and reads the block header.
//	After a successful read, the file is positioned at the start of the
//	block data.
//
//	Return TRUE, if successful; FALSE, if unable to locate the given block
//	or an I/O error occurs.
//
//	See writeBlockHeader() for the block header format.
//
//	NOTE:  The field widths of the block header are hard-coded in this
//	routine.  If you alter the field widths, you must update the value of the
//	BlockHeaderLen constant and readBlockHeader().
//
boolean VBlockFile::readBlockHeader(integer blockNumber, integer *bytesUsed, 
									integer *nextBlockNumber)
{
	if (f.at((integer) (FileHeaderLen + 
		(blockNumber * (fileBlockSize + BlockHeaderLen)))) == -1L) {

		errorCode = IoFailure;
		return FALSE;
	}
	
	char buf[BlockHeaderLen];
	char *bufp = buf;

	if (f.read(bufp, BlockHeaderLen) == BlockHeaderLen) {
		bufp += 13;			//	skip newline, bang, block number, blank.
		bufp[21] = '\0';	//	replace newline for scan

		if (sscanf(bufp,"%10ld %10ld", bytesUsed, nextBlockNumber) == 2) {
			return TRUE;
		}
	}
	errorCode = IoFailure;
	return FALSE;
}

// ---------------------------------------------------------------------------
// Set the state of the autoUpdate flag.
// If TRUE, write block header info with each write operation
//
void VBlockFile::autoUpdate(boolean tf)
{
	if (tf) {
		if (!updateHeader) {
			updateHeader = tf;

			// write out header data
		 	write(0, 0);
			return;
		}
	}

	updateHeader = tf;
}

// ---------------------------------------------------------------------------
