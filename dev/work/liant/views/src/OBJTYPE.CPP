//	objtype.cpp -- [UNDER DEVELOPMENT]
//
//	VObjectType implementation
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
//	06/28/95 dgm	Original
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
//
#include "objtype.h"
#include "ordcllct.h"

// --------------------------------------------------------------------------
//
#if defined(CV_WINDOWS)

#include "windows.h"

#elif defined(CV_MOTIF)

#include "notifier.h"

#endif

// --------------------------------------------------------------------------
//
defineClass(VObjectType, VObject)

// --------------------------------------------------------------------------
//
#if defined(CV_OLE_DRAG_DROP) && !defined(CV_WIN16)

// --------------------------------------------------------------------------
//
static char *strnew(const char *s)
{
	char *news = new char [strlen(s) + 1];
	strcpy(news, s);
	return news;
}

// --------------------------------------------------------------------------
//
VObjectType::VObjectType(const VObjectType& ot)
{
	typeHandle = ot.typeHandle;
	typeClass = ot.typeClass;
	if ((ot.typeHandle == 0) && (ot.typeClass == 0) && (ot.typeName != 0)) {
		typeName = strnew(ot.typeName);
	}
	else {
		typeName = 0;
	}
	nameSpace = ot.nameSpace;
}

// --------------------------------------------------------------------------
//
void VObjectType::set(const char *type_name, int name_space)
{
	destroy();
	construct(type_name, name_space);
}

// --------------------------------------------------------------------------
//
void VObjectType::set(const VClass *type_class)
{
	destroy();
	construct(type_class);
}

// --------------------------------------------------------------------------
//
void VObjectType::set(const VObjectType& ot)
{
	set(ot.typeName, ot.nameSpace);
}

// --------------------------------------------------------------------------
//
VObjectType& VObjectType::operator=(const VObjectType& ot)
{
	set(ot);
	return *this;
}

// --------------------------------------------------------------------------
//
VObjectType& VObjectType::operator=(const char *type_name)
{
	set(type_name);
	return *this;
}

// --------------------------------------------------------------------------
//
VObjectType& VObjectType::operator=(const VClass *class_name)
{
	set(class_name);
	return *this;
}

// --------------------------------------------------------------------------
//
boolean VObjectType::operator==(const VObjectType& ot) const
{
	if ((typeHandle == ot.typeHandle) && (typeHandle != 0)) {
		return TRUE;
	}
	else if ((typeClass == ot.typeClass) && (typeClass != 0)) {
		return TRUE;
	}
	else if (nameSpace != ot.nameSpace) {
		return FALSE;
	}
	else if ((typeName == ot.typeName) && (typeName != 0)) {
		return TRUE;
	}
	else {
		if (typeName == 0) {
			((VObjectType *)this)->setupTypeName();
		}
		if (ot.typeName == 0) {
			((VObjectType&)ot).setupTypeName();
		}
		if ((typeName != 0) && (ot.typeName != 0)) {
			return (typeName == ot.typeName) ||
				   (strcmp(typeName, ot.typeName) == 0);
		}
		else {
			return FALSE;
		}
	}
}

// --------------------------------------------------------------------------
//
boolean VObjectType::operator==(const char *s) const
{
	if (s == 0) {
		return typeName == 0;
	}
	if (typeName == 0) {
		((VObjectType *)this)->setupTypeName();
	}
	return (typeName != 0) && (strcmp(s, typeName) == 0);
}

// --------------------------------------------------------------------------
//
void VObjectType::construct(const char *type_name, int name_space)
{
	if ((type_name == 0) || (type_name[0] == '\0')) {
		typeName = 0;
	}
	else {
		typeName = strnew(type_name);
	}
	typeHandle = 0;
	typeClass = 0;
	nameSpace = name_space;
}

// --------------------------------------------------------------------------
//
void VObjectType::realize()
{
	if (typeHandle != 0) {
		return;
	}

	setupTypeName();

	if (typeName == 0) {
		return;
	}

#if defined(CV_WINDOWS)

	if (nameSpace == 0) {
		typeHandle = Handle(RegisterClipboardFormat(LPSTR(typeName)));
	}
	else if (nameSpace == 1) {
		typeHandle = Handle(GlobalAddAtom(typeName));
	}

#elif defined(CV_MOTIF)

	if (name_space == 0) }
		typeHandle = Handle(XmInternAtom
							(notifier->defaultDisplay(),
							 type_name, False));
	}
	else if (name_space == 1) {
		typeHandle = Handle(XmClipboardRegisterFormat
							(notifier->defaultDisplay(),
							 type_name, strlen(type_name)));
	}

#else

	;

#endif

}

// --------------------------------------------------------------------------
//
void VObjectType::destroy()
{
	if (typeName != 0) {
		delete [] typeName;
		typeName  = 0;
	}

	if (typeHandle == 0) {
		return;
	}

#if defined(CV_WINDOWS)

	if (nameSpace == 0) {
		;
	}
	else if (nameSpace == 1) {
		GlobalDeleteAtom(ATOM(typeHandle));
	}

#elif defined(CV_MOTIF)

	if (nameSpace == 0) {
		;
	}
	else if (nameSpace == 1) {
		;
	}

#else

	;

#endif

}

// --------------------------------------------------------------------------
//
void VObjectType::setupTypeName()
{
	if (typeName != 0) {
		return;
	}

	if (typeClass != 0) {
		const char *class_name = typeClass->nameOf();
		if (class_name != 0) {
			char buffer[512];
			typeName = strnew(strcat(strcpy(buffer, "CV_"), class_name));
		}
		return;
	}

#if defined(CV_WINDOWS)

	switch (UINT(typeHandle)) {
		case CF_TEXT:
			typeName = strnew("TEXT");			return;
		case CF_BITMAP:
			typeName = strnew("BITMAP");		return;
		case CF_METAFILEPICT:
			typeName = strnew("METAFILEPICT");	return;
		case CF_SYLK:
			typeName = strnew("SYLK");			return;
		case CF_DIF:
			typeName = strnew("DIF");			return;
		case CF_TIFF:
			typeName = strnew("TIFF");			return;
		case CF_OEMTEXT:
			typeName = strnew("OEMTEXT");		return;
		case CF_DIB:
			typeName = strnew("DIB");			return;
		case CF_PALETTE:
			typeName = strnew("PALETTE");		return;
		case CF_PENDATA:
			typeName = strnew("PENDATA");		return;
		case CF_RIFF:
			typeName = strnew("RIFF");			return;
		case CF_WAVE:
			typeName = strnew("WAVE");			return;
#if defined(CV_WIN32)
		case CF_UNICODETEXT:
			typeName = strnew("UNICODETEXT");	return;
		case CF_ENHMETAFILE:
			typeName = strnew("ENHMETAFILE");	return;
#endif
		default:
		{
			char buffer[512];
			if (GetClipboardFormatName(typeHandle, buffer, 511) > 0) {
				typeName = strnew(buffer);
			}
			return;
		}
	}

#elif defined(CV_MOTIF)

	typeName = strnew(XmGetAtomName());

#endif

}

#endif // CV_OLE_DRAG_DROP && !CV_WIN16
