/*
$Header:   Q:/views/win/vcs/font.h_v   1.19   Jun 03 1997 09:51:46   scottt  $ 
*/
//	font.h
//
//	VFont class interface
//
//	Allegris Foundation 1.1.00 for Microsoft Windows
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
//	04/09/93 pat	changed id to VObject *
//	05/10/93 pat	preened;
//					added getFrom(), putTo(), realize();
//					added reference to VArchiver;
//	07/10/93 jmd	made VFontSelect a friend
//	07/20/93 jmd	minor preening, removed free()
//	07/20/93 jmd	made face use const VString
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//	09/01/93 pat	removed VArchiver stuff
//	10/10/93 jmd	SC6 changes (typdef'd enums).
//
//	02/02/94 pat	added _height (positive, height of cell) distinct from
//					LOGFONT.lfHeight (negative, height of glyph) to ease
//					point size calculation.
//	03/04/94 pat	STRICT: hFont is now HFONT, not HANDLE.
//
//  09/08/95 evc    1)	Font handle creation and deletion now tracks
//					VControls that 	are using the VFont, and updates
//					then if the font object changes. Also fixed places where
//					VFont usage by ports  needed to be tracked, but wasn't.
//					Added method, updateControls(), to do this.
//					2) added a fontinfo object to class data.
//					4) added constructor that takes a VFontInfo
//					and more....
//  10/12/95 evc	removed assignment operator and added inUse
//  11/15/95 evc	added getNativeWindowFont(VWindow *);
//  04/26/96 dgm	Added useOldDefaultFont().
//  09/09/96 dgm	Added system().
// ---------------------------------------------------------------------------

#ifndef VFONT_H
#define VFONT_H

#include "winclass.h"
#include "object.h"
#include "ordcllct.h"


CLASS VPort;
CLASS VWindow;
CLASS VControl;
CLASS VFontSelect;
CLASS VString;

CLASS VFontInfo;

extern VClass *VFontCls;

//	VFont orientation
typedef enum {
				Normal 		=    0, 
				Left90 		=  900, 
				Right90 	= 2700, 
				UpsideDown 	= 1800 
} Orient;

//	The VFont class defines properties of the text drawn with the VPort
//	class. The fundamental properties of fonts are those of character
//	style ('typeface') and character type size (expressed in units called
//	'points'). Additional character style properties can be specified such
//	as the weight of the characters (for example, bold), italics,
//	underlining and strikeout. A VFont object based on the default,
//	porportional width system font can be created with the VFont()
//	constructor, while a VFont object using the fixed width system font
//	can be built with the VFont(boolean) constructor. The `usefont()`
//	method of the VPort class is used to specify a VFont object for text
//	output, while the `setFont()` method of the VControl class is used to
//	provide a VFont to any of the various subclasses of VControl.

CLASS VFont : public VObject {
public:
	enum SystemFont {	Default			= 0x0000,
						Dialog			= 0x0001,	// currently internal
						Control			= 0x0002,	// currently internal
						Icon			= 0x0003,	// currently internal
						Info			= 0x0004,	// currently internal
						Menu			= 0x0005,	// currently internal
						Caption			= 0x0006,	// currently internal
						SmallCaption	= 0x0007,	// currently internal
						Bold			= 0x0100,
						Italics			= 0x0200,
						Underline		= 0x0400,
						Strikeout		= 0x0800
	};
	enum Constants {	SystemFontCount	= 8 };

public:

	//	Constructors/destructor

				VFont();

				VFont(	boolean fixed);

				VFont(	const VString& face, 
						int 	points, 
						boolean bold 	= FALSE, 
						boolean italics = FALSE, 
						boolean undLin 	= FALSE, 
						boolean strike 	= FALSE);

				VFont(VFontInfo&);


				~VFont();

	VClass 	   *iam();

	// Seeing if the font has any clients
	boolean		inUse();
	//VFont&      operator= (const VFont& );

	//	Setting and Interrogating Attributes
	void 		orientation(int o);
	int 		orientation();
	void 		pointSize(int ps);
	int 		pointSize();
	void 		bold(boolean b);
	boolean	    bold();	
	void 		italic(boolean i);
	boolean	    italic();
	void 		underline(boolean u);
	boolean	    underline();
	void 		strikeOut(boolean so);
	boolean	    strikeOut();
	void 		face(const VString& f);
	char	   *face();

	int 		height();
	int 		width();
	short 		maxWidth();

	boolean		isSystemFont() const;

   	static VFont *  getNativeWindowFont(VWindow *);

	static void	useOldDefaultFont(boolean);

	//	Access to GUI

	HFONT 		getHndl();
	void 		logfont(LOGFONT& logfnt);

	void 		bind(VObject *p, boolean b);

	static VFont&	system(int = Default);
	static void		system(int, VFont *);
	static void		systemFontsChanged();

protected:

	friend 	VPort;
	friend 	VWindow;
	friend 	VControl;
	friend 	VFontSelect;
	friend  VFontInfo;

	VOrdCollect    *ports;    // ports that use this font
	VOrdCollect    *controls; // VControls that use this font
	// this fontinfo object contains the crossplatform logical font 
	// information.




	method 			pSet;		//	method to   set port font
	method 			pReset;		//	method to reset port font
	method 			scale;		//	method to scale a port's font height

	int 			ptSize;		//	point size of font
	short 			mWidth;		//	max width of a character in this VFont
	int 			stock : 1;	//	TRUE if built-in GUI system font
	int 			isSystem : 1;
	// contains the constant that designates the stock object, or 0.

	HFONT	  		hFont;		//	GUI object handle
	LOGFONT 		lf;			//	GUI structure to maintain attributes
	
	//	Note:		actualLog is TRUE if LOGFONT structure contains the 
	//				actual info, i.e., it has been selected into a device 
	//				context - MS Windows only.

	boolean 		actualLog;

	void 			copyFace(const VString& f);

	void 			getActual();
	HFONT			select(HDC, VObject *);
	boolean 		make(int h);
	boolean 		make();

	void 			updateControls();
	void 			clean();

protected:

	int 			_height;

	VFontInfo   *fontinfo;
private:
	VFont(int, int); // used for StockObject font creation
	VFont(LOGFONT&);

protected:
	void			replaceFont(VFont *);
	static VFont   *newSystemFont(int, int, int, int, int);
public:
	static void		deleteSystemFonts();
	static void		systemFontDeleted(VFont *);
};

// ---------------------------------------------------------------------------
//	 Return the angle of orientation of this `VFont` object. The
//	 angle is measured in a counterclockwise rotation and is
//	 returned scaled by a factor of ten, for example, 90 degrees to
//	 the left is returned as 900.
//
inline int VFont::orientation() 
{ 
	return lf.lfEscapement; 
}

// ---------------------------------------------------------------------------
//	Return the point size of this font.
//
inline int VFont::pointSize()
{
	if (ptSize == 0) {
		getActual();
	}
	return ptSize;
}

// Return TRUE if the font has a client in the controls or ports collection.

inline boolean		VFont::inUse(){
	if (ports && !ports->empty()){
		return TRUE;
	}
	if (controls&& !controls->empty()){
		return TRUE;
	}
	return FALSE;
}

inline boolean VFont::isSystemFont() const
{
	return isSystem;
}

#endif	//	!VFONT_H 
