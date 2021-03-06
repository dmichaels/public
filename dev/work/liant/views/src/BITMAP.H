// $Header:   Q:/views/win/vcs/bitmap.h_v   1.8   Mar 04 1997 16:31:54   gregm  $ 

//	bitmap.h
//
//	VBitMap interface
//
//  Allegris Foundation 1.1.00 for Microsoft Windows
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
//	01/23/97 dgm	Original.
// --------------------------------------------------------------------------

#ifndef VBITMAP_H
#define VBITMAP_H

// --------------------------------------------------------------------------
//
#include "display.h"

// --------------------------------------------------------------------------
//
CLASS VClass;
CLASS VColor;
CLASS VDisplay;
CLASS VLocation;
CLASS VObject;
CLASS VPort;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

class FileOrArchive;

// --------------------------------------------------------------------------
//
CLASS VBitMap : public VDisplay {
public:
					VBitMap();
					VBitMap(const char *);
					VBitMap(int, int, boolean);
					VBitMap(int, int, VDisplay *);
					VBitMap(HANDLE);
				   ~VBitMap();
	VClass		   *iam();

	boolean			loadFile(const char *);
	boolean			saveFile(const char *);

	virtual void	sizeOfImage(int *, int *);
	virtual int		depthOfImage();
	void			getByteDim(int *, int *) const;
	int				planes() const;

	VColor			readPixel(const VLocation&) const;
	void			readPixelRow(const VLocation&, VColor *, int) const;
	VColor			writePixel(const VLocation&, const VColor&);
	void			eraseBlack();
	void			eraseWhite();

	void			setCompObj(VDisplay *);
	void			setBitmap(HBITMAP);

	HBITMAP			getBitmap() const;
	virtual HDC		getHndl();
	virtual void	ungetHndl();

	short			getFrameCount() const;
	short			getSequenceLength() const;
	void			setFrameCount(short);
	void			setSequenceLen(short);

	boolean 		isValid() const;

#ifndef CV_NOARCHIVER
	void			getFrom(VArchiver&);
	void			getFrom(VArchiver&, VObject *, boolean = TRUE);
	void			putTo(VArchiver&);
#endif

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Undocumented
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	boolean			saveFile(const char *filename, short, long = 0);
	void			bitblt(VDisplay *, int, int, long);
	void			bitblt(VDisplay *, int, int, int, int, int, int, long);
	void			bitblt(VDisplay *,
						   int, int, int, int, int, int, int, int, long);
	HANDLE			getDIB(short = 8, long = 0) const;
	void			setOptimizePalette(boolean);
	boolean			isOptimizePalette() const;
	boolean			pasteFromClipboard();
	boolean			copyToClipboard();
	static boolean	canPasteFromClipboard();

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	void			initialize();
	void			clean();
	int				getBitCount() const;
	long			getCompression() const;
	boolean			loadFile(FileOrArchive&);
	boolean			saveFile(FileOrArchive&, short, long = 0);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section; please KEEP OUT
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
	HANDLE			hBitmap;
	HANDLE			hDib;
	HANDLE			hOldPalette;
	HANDLE			hOldBitmap;
	VDisplay	   *compatibleDisplay;
	short			frameCount;
	short			sequenceLength;
	unsigned char	haveValidBitmap : 1;
	unsigned char	optimizePalette : 1;
};

extern VClass *VBitMapCls;

// --------------------------------------------------------------------------
//
inline boolean VBitMap::isValid() const
{
	return haveValidBitmap;
}

inline short VBitMap::getFrameCount() const
{
	return frameCount;
}

inline short VBitMap::getSequenceLength() const
{
	return sequenceLength;
}

inline void VBitMap::setFrameCount(short fc)
{
	frameCount = fc;
}

inline void VBitMap::setSequenceLen(short sl)
{
	sequenceLength = sl;
}

inline void VBitMap::setOptimizePalette(boolean b)
{
	optimizePalette = b;
}

inline boolean VBitMap::isOptimizePalette() const
{
	return optimizePalette;
}

#endif // VBITMAP_H
