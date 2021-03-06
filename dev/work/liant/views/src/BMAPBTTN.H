// $Header:   Q:/views/win/vcs/bmapbttn.h_v   1.11   Mar 04 1997 16:31:54   gregm  $ 

//	bmapbttn.h
//
//	VBitMapButton class interface
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
//	07/18/93 jmd	created
//	07/31/93 jmd	added prompt
//	08/09/93 jmd	made VFrame const
//	08/15/93 jmd	made changes to frame/seq stuff
//	08/16/93 jmd	added name, archiving
//	08/16/93 jmd	added bmapName
//	08/20/93 jmd	added loadResources()
//	04/08/94 jmd	added popup prompt support
//	10/11/94 dss	added displayPrompt().
//	10/28/94 jmd	added tooltip support
//	08/10/95 dgm	Changed private data members to protected.
//	09/01/95 dss	VStyle changes.
// ---------------------------------------------------------------------------

#ifndef VBMAPBTTN_H
#define VBMAPBTTN_H
#include "bitmap.h"
#include "pushbttn.h"

#ifndef CV_NOARCHIVER
CLASS VRscArchiver;
#endif

extern VClass *VBitMapButtonCls;

CLASS VBitMapButton : public VPushButton {
public:
			VBitMapButton(	const VFrame& f,
							VWindow *pwin,
							const VStyle& style = StyleDefault);

			VBitMapButton(	const VString& wname, 
						  	const VFrame& f,
						  	VWindow *pwin,
							const VStyle& style = StyleDefault);

			VBitMapButton(	VRatio x,
						  	VRatio y,
							VBitMap *bm,
							short fcount,
					  		VWindow *pwin,
							const VStyle& style = StyleDefault,
							short seqlen = 4,
							short startseq = 0);

			VBitMapButton(	VRatio x,
							VRatio y,
							const VString& bmname,
							short fcount,
					  		VWindow *pwin,
							const VStyle& style = StyleDefault,
							short seqlen = 4,
							short startseq = 0);

			VBitMapButton();
			~VBitMapButton();

	void	setBitMap(VBitMap* bm, short fcount, short seqlen = 4);
	void	setBitMap(const VString& bmname, short fcount, short seqlen = 4);
	void	setDefault(boolean tf);

	VBitMap 	*getBitMap() const;

	short		getFrameCount() const;
	short		getStartSequence() const;
	short		getSequenceLength() const;
	void		setStartSequence(short start);

	void		setPrompt(const VString& p);
	VString& 	getPrompt();

	void		hilite(boolean);
	boolean		isHilited();

	void		setBitMapName(const VString& p);
	VString& 	getBitMapName();

	boolean 	paint();
	boolean 	erased();
	VClass 		*iam();

#ifdef CV_MESSAGE_ON_CLICK
	boolean 	mouseDn(int mx, int my);
	boolean 	mouseUp(int mx, int my);
#else
	boolean 	mouseEnter(int bstat);
	boolean 	mouseExit(int bstat);
#endif

	boolean 		displayPrompt(VString *msg);

	//	Archival and Retrieval
#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			putTo(VArchiver& a, boolean most_derived);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);

	boolean			loadResources(VRscArchiver& a);
#endif

protected:
	HPALETTE		getPalette();			

protected:
	VBitMap *imageBitMap;		// the bitmap image
	short	 frameCount;		// number of frames in the bitmap
	short	 seqLen;			// number of frames per sequence
	short	 startSeq;			// index of starting seqence in the bitmap
	short	 frameHeight;		// height of frames in the bitmap strip
	boolean  internal;			// VBitMap object was created internally
	VString	 promptString;		// status bar message
	VString	 bmapName;			// name of bitmap in rsc file

private:
	void	cleanUp();

	void	init(int*w,int*h,VBitMap*bmap,short fcount,short seqlen= 4);
	void	getInfo(VWinInfo& winfo);
	void	drawDefFrame();

};

// ---------------------------------------------------------------------------
inline short VBitMapButton::getFrameCount() const
{
	return frameCount;
}

// ---------------------------------------------------------------------------
inline short VBitMapButton::getStartSequence() const
{
	return startSeq;
}

// ---------------------------------------------------------------------------
inline short VBitMapButton::getSequenceLength() const
{
	return seqLen;
}

// ---------------------------------------------------------------------------
inline VBitMap *VBitMapButton::getBitMap() const
{
	return imageBitMap;
}

// ---------------------------------------------------------------------------
inline VString& VBitMapButton::getPrompt()
{
	return promptString;
}

// ---------------------------------------------------------------------------
inline void VBitMapButton::setPrompt(const VString& p)
{
	promptString = p;
}

// ---------------------------------------------------------------------------
inline VString& VBitMapButton::getBitMapName()
{
	return bmapName;
}

// ---------------------------------------------------------------------------
inline void VBitMapButton::setBitMapName(const VString& p)
{
	bmapName = p;
}

#endif // VBMAPBTTN_H
