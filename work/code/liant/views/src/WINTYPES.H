// $Header:   Q:/views/win/vcs/wintypes.h_v   1.7   Mar 04 1997 16:33:18   gregm  $

//	wintypes.h
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
//	09/09/92 jmd	made CV_WIN16 auto define CV_WINDOWS
//	03/19/93 pat	preened
//	07/19/93 jmd	added VColor class
//  02/03/93 dgm    fixed K_braceright
//	03/05/94 pat	STRICT: included windows.h; preened.
//	03/24/94 pat	CV_WIN32: auto defined CV_WINDOWS
//	07/14/94 jld	added defs.h dependency for CV_WINDOWS
//	11/03/94 dss	added K_Mod_Control, K_Mod_Alt.
//	11/07/94 dss	circular dependency with winclass.h.
//	10/30/95 dss	Define OLE2ANSI before including windows.h.
//	11/09/95 dss	take out the OLE2ANSI definition.
// ----------------------------------------------------------------------------

#ifndef VWINTYPE_H
#define VWINTYPE_H

#include "defs.h"
#include "color.h"

// KEYBOARD MODIFIER CODES:

#define K_Mod_Button    0x8000   /* Modifier keys, keys which can be in a */
#define K_Mod_Shift     0x4000   /* down state to 'modify' the key pressed */
#define K_Mod_Meta1     0x2000
#define K_Mod_Meta2     0x1000
#define K_Mod_Capslock  0x0800

#define K_Mod_Control	K_Mod_Meta1
#define K_Mod_Alt			K_Mod_Meta2

//	Definitions of meta keys for use in strings with MS Windows:

#define S_Meta1    		Ctrl
#define S_Meta2    		Alt

//	VIRTUAL KEYBOARD CODES:

#define K_space         VK_SPACE
#define K_exclam        '1'
#define K_quotedbl      0xDE
#define K_numbersign    '3'
#define K_dollar        '4'
#define K_percent       '5'
#define K_ampersand     '7'
#define K_parenleft     '9'
#define K_parenright    '0'
#define K_asterisk      '8'
#define K_plus          0xBB
#define K_comma         0xBC
#define K_minus         0xBD
#define K_period        0xBE
#define K_slash         0xBF
#define K_colon         0xBA
#define K_semicolon     0xBA
#define K_less          0xBC
#define K_equal         0xBB
#define K_greater       0xBE
#define K_question      0xBF
#define K_at            '2'

#define K_bracketleft   0xDB
#define K_bracketright  0xDD // !!!dgm 0xDC
#define K_braceleft     0xDB
#define K_braceright    0xDD
#define K_backslash     0xDC
#define K_bar           0XDC
#define K_circumflex    '6' 
#define K_underscore    0xBD
#define K_quoteleft     0xC0
#define K_quoteright    0xDE
#define K_tilde         0xC0

#define K_0             '0'
#define K_1             '1'
#define K_2             '2'
#define K_3             '3'
#define K_4             '4'
#define K_5             '5'
#define K_6             '6'
#define K_7             '7'
#define K_8             '8'
#define K_9             '9'

#define K_A             'A'
#define K_B             'B'
#define K_C             'C'
#define K_D             'D'
#define K_E             'E'
#define K_F             'F'
#define K_G             'G'
#define K_H             'H'
#define K_I             'I'
#define K_J             'J'
#define K_K             'K'
#define K_L             'L'
#define K_M             'M'
#define K_N             'N'
#define K_O             'O'
#define K_P             'P'
#define K_Q             'Q'
#define K_R             'R'
#define K_S             'S'
#define K_T             'T'
#define K_U             'U'
#define K_V             'V'
#define K_W             'W'
#define K_X             'X'
#define K_Y             'Y'
#define K_Z             'Z'

#define K_Backspace     0x08
#define K_Tab           0x09
#define K_Clear         0x0C
#define K_Return        0x0D
#define K_Escape        0x1B
#define K_Insert        0x2D
#define K_Delete        0x2E

#define K_NULL          0

#define K_Home          0x24
#define K_Left          0x25
#define K_Right         0x27
#define K_Up            0x26
#define K_Down          0x28
#define K_Prior         0x21
#define K_Next          0x22
#define K_End           0x23
#define K_Begin         0x24


#define K_Capslock      0x14
#define K_Numlock       0x90

#define K_Shift         VK_SHIFT
#define K_Meta1         VK_CONTROL
#define K_Meta2         VK_MENU			// Alt

#define K_Control		K_Meta1
#define K_Alt			K_Meta2
#define	K_Meta			K_Meta3

#define K_F1	    	0x70
#define K_F2	    	0x71
#define K_F3	    	0x72
#define K_F4	    	0x73
#define K_F5	    	0x74
#define K_F6	    	0x75
#define K_F7	    	0x76
#define K_F8	    	0x77
#define K_F9	    	0x78
#define K_F10	    	0x79
#define K_F11	    	0x7A
#define K_F12	    	0x7B
#define K_F13	    	0x7C
#define K_F14	    	0x7D
#define K_F15	    	0x7E
#define K_F16	    	0x7F

//	(end of VIRTUAL KEYBOARD CODES)

#ifdef CV_WINDOWS
//#	if !defined(winclass_h) && (defined(CV_WIN32) || defined(STRICT))
//!!!dss 11/04/94 we have a circular dependency so always include
//		windows.h.

#	if 1
		
		//	Our headers include windows.h via our winclass.h.
		//	Where winclass.h is not included, we include the 
		//	entire windows.h header, rather than try to
		//	pick out what we need for Win32 or STRICT.

#		include "windows.h"
#	else

		//	These are typedef's and define's from the MS Windows's windows.h.
		//	Our headers include windows.h via our winclass.h.
		//	Where winclass.h is not included, we typedef and define 
		//	some of the windows.h stuff to avoid having to include the
	    //	entire header.

		typedef unsigned int UINT;

#		if defined(CV_WIN16)
			typedef unsigned short 	WORD;
			typedef UINT 			WPARAM; 
			typedef long 			LPARAM;
#		else
			typedef unsigned int 	WORD;
#			define 					WPARAM 	  	WORD
#			define 					LPARAM 	  	long
#			define 					LONG 	  	long
#		endif

		typedef UINT HDC;
		typedef UINT HWND;
		typedef UINT HANDLE;
		typedef UINT HBITMAP;
		typedef UINT HRGN;
		typedef UINT HMENU;
		typedef int (far pascal *FARPROC)();

#		define RGN_AND 	    		1
#		define RGN_OR		   		2
#		define RGN_XOR 	    		3
#		define RGN_DIFF	    		4
#		define RGN_COPY	    		5

#		define R2_NOTMERGEPEN	    2		//	DPon
#		define R2_MASKNOTPEN	    3		//	DPna
#		define R2_NOTCOPYPEN	    4		//	PN
#		define R2_MASKPENNOT	    5		//	PDna
#		define R2_NOT		    	6		//	Dn
#		define R2_XORPEN	    	7		//	DPx
#		define R2_NOTMASKPEN	    8		//	DPan
#		define R2_MASKPEN	    	9		//	DPa
#		define R2_NOTXORPEN	 		10		//	DPxn
#		define R2_NOP				11		//	D
#		define R2_MERGENOTPEN	    12		//	DPno
#		define R2_COPYPEN	    	13		//	P
#		define R2_MERGEPENNOT	    14		//	PDno
#		define R2_MERGEPEN	    	15		//	DPo

#		define PS_SOLID	 		   	0
#		define PS_DASH		    	1	    //	-------
#		define PS_DOT		    	2	    //	.......
#		define PS_DASHDOT	    	3	    //	_._._._
#		define PS_DASHDOTDOT	    4	    //	_.._.._
#		define PS_NULL 	    		5
#		define PS_INSIDEFRAME 	    6

#endif	//	!winclass_h && !CV_WIN32

#	define ACCELcode ((int)0x8000)

	enum MessageType { 	MesgInformation, 
						MesgQuestion, 
						MesgAction, 
						MesgWarning };

	extern unsigned mesgToType(enum MessageType);

#endif //	CV_WINDOWS

#endif //	VWINTYPE_H
