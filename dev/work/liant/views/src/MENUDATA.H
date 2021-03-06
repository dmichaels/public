// $Header:   Q:/views/common/vcs/MENUDATA.H_V   1.10   Mar 24 1997 16:12:24   scottt  $ 

//	menudata.h
//
//	VMenuData class interface [Common]
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
//	06/30/93 dgm	original.
//	07/19/93 pmcm	changed class to CLASS.
//	11/19/93 pkt	fixed enum Flag so it can hold all of the necessary values.
//	05/25/95 pkt	merged for common source.
//	09/12/95 pkt	miscellaneous improvements
//  10/26/95 evc    Added a string id (name) and an integer help id
//	10/26/95 tlf	Fixed coding error in initialization of menu_name.
//	11/29/95 tlf	updated for HPUX.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VMENUDATA_H
#define VMENUDATA_H

// ---------------------------------------------------------------------------
// Include files
// ---------------------------------------------------------------------------

#include "object.h"
#include "str.h"

// ---------------------------------------------------------------------------
// External class declarations
// ---------------------------------------------------------------------------

CLASS VMenu;
CLASS VMenuItem;
CLASS VPopupMenu;
CLASS VObject;
CLASS VString;

#ifdef CPPV_HPUX
extern const VString VMenuData_default_menu_name;
#endif

// ---------------------------------------------------------------------------
// Class definition
// ---------------------------------------------------------------------------

CLASS VMenuData {

public:
		enum Flag {
					Disabled	= 0x0001,
					Checked		= 0x0002,
					MaxFlag		= 0x1FFF
		};

public:
					VMenuData(const VString& menu_label,
							  const VString& menu_prompt,
							  method menu_client_method		= 0,
							  VObject *menu_client_object	= 0,
							  int menu_identifier			= -1,
							  int menu_accelerator			= 0,
							  Flag menu_flags				= Flag(0),
#ifndef CPPV_HPUX
							  const VString& menu_name = VString("none"),
#else
							  const VString& menu_name = VMenuData_default_menu_name,
#endif
							  int hId = 0);
					VMenuData(const VString& menu_label);
					VMenuData(VObject *popup_menu);
					VMenuData();
				   ~VMenuData();

private:

	enum PrivateFlag {
					Normal			= 0x0100,
					Cascade			= 0x0200,
					Separator		= 0x0400,
					ClientObject	= 0x0800,
					EndOfList		= 0x1000
	};

	Flag			flags;
	VString			label;
	VString			prompt;
	method			clientMethod;
	union {
		VObject	   *clientObject;
		VPopupMenu *popupMenu;
	};
	int				identifier;
	int				accelerator;


	// new
	VString name;
	int helpId;

private:

	friend		VPopupMenu;
};

// ---------------------------------------------------------------------------
// Inline member-function definitions
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//
inline VMenuData::VMenuData()
{
	flags 			= Flag(Separator);
}

// ---------------------------------------------------------------------------
//
inline VMenuData::VMenuData(const VString& menu_label,
							const VString& menu_prompt,
							method menu_client_method,
							VObject *menu_client_object,
							int menu_identifier,
							int menu_accelerator,
							VMenuData::Flag menu_flags,
							const VString& menu_name, 
							int h_id
							)
:
	label(menu_label),
	prompt(menu_prompt)
{
	flags			= Flag(menu_flags | Normal);
	clientMethod	= menu_client_method;
	clientObject	= menu_client_object;
	identifier		= menu_identifier;
	accelerator		= menu_accelerator;
	name = menu_name;
	helpId = h_id;
}

// ---------------------------------------------------------------------------
//
inline VMenuData::VMenuData(const VString& menu_label)
:
	label(menu_label),
	prompt(""),
	name(""),
	helpId(0)
{
	flags			= Flag(Normal);
	clientMethod	= 0;
	clientObject	= 0;
	identifier		= -1;
	accelerator		= 0;
}

// ---------------------------------------------------------------------------
//
inline VMenuData::VMenuData(VObject *object)
{
	extern VClass *VPopupMenuCls;

	if (object == 0) {
		flags		= Flag(EndOfList);
	}
	else if (object->isA(VPopupMenuCls)) {
		flags			= Flag(Cascade);
		popupMenu		= (VPopupMenu *)object;
	}
	else {
		flags			= Flag(ClientObject);
		clientObject	= object;
	}
}

// ---------------------------------------------------------------------------
//
inline VMenuData::~VMenuData()
{
}

#endif // VMENUDATA_H
