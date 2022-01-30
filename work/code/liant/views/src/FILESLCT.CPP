// $Header:   Q:/views/win/vcs/fileslct.cpv   1.9   Mar 04 1997 17:55:04   gregm  $ 

//	fileslct.cpp
//
//	VFileSelect implementation
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
//	05/27/93 jmd	preened
//	06/28/93 jmd	added new members and features
//	08/03/93 jmd	now looks at index going in
//	08/05/93 jmd	now it really looks at index going in
//	09/02/93 dgm	M8 changes; miscellanouse integral type conversions.
//	07/14/94 jld	replaced NULL with 0
//	10/11/94 dss	Added operator(). Constructor calls operator().
//	07/03/95 dgm	Merged in Intersolv changes:
//					- Set initial directory and file to the
//					  given directory path and base file name.
//	05/17/96 dgm	Added setRememberInitialPath().
// ----------------------------------------------------------------------------

#include "fileslct.h"
#include "notifier.h"
#include "pathstr.h"

extern "C" {
#	include "commdlg.h"
#	include <ctype.h>
#	undef NULL
#	include <string.h>
#	include <stdio.h>
#	include <stdarg.h>
}

defineClass(VFileSelect, VDialog)

// ---------------------------------------------------------------------------
//
static boolean	rememberInitialPath = TRUE;
static VString	lastOpenPath;
static VString	lastSavePath;

// ---------------------------------------------------------------------------
//
void VFileSelect::setRememberInitialPath(boolean b)
{
	rememberInitialPath = b;
}

// ---------------------------------------------------------------------------
//
boolean VFileSelect::isRememberInitialPath()
{
	return rememberInitialPath;
}

// ---------------------------------------------------------------------------
//
VFileSelect::VFileSelect()
{
}

// ---------------------------------------------------------------------------
// Construct a file-open or a file-save-as dialog object.  If dstyle is SaveAs
// is TRUE, then construct a save-as dialog, otherwise construct the standard
// file-open dialog.  Construct either dialog with a title title,
// initial file name path, directory path filter filter, 
// and parent VWindow parentwin.  Filter is a | delimited string.
//
VFileSelect::VFileSelect(DlgStyle dstyle,
							const VString& title,
							VPathString& path,
							VString *filter,
							boolean& success,
							int& filter_index,
							VWindow *parentwin)
{
	success = (*this)(dstyle, title, path, filter, filter_index, parentwin);
}

// ---------------------------------------------------------------------------
// Construct a file-open or a file-save-as dialog object.  If saveAs
// is TRUE, then construct a save-as dialog, otherwise construct the standard
// file-open dialog.  Construct either dialog with a title title, a prompt
// string prompt, initial file name fileName, directory path filter filter, 
// and parent VWindow parentwin. 
// THIS IS FOR V2.0 COMPATABILITY ONLY (prompt is ignored)
//
VFileSelect::VFileSelect(boolean saveas, char *title, char *prompt,
						 VString **filename, VString *filter,
						 VWindow *parentwin)
{
	if (filename == 0) {
		return;
	}

	boolean  success;
	int		 filter_index = 0;
	DlgStyle dstyle = (saveas) ? SaveAs : FileOpen;

	VPathString path;

	if (*filename != 0)  {
		path.puts(filename[0]->gets());
	}
	success = (*this)(dstyle, title, path, filter, filter_index, parentwin);

	if (!success) {
		*filename = 0;
	}
	else {
		*filename = new VString(path);
	}
}

// ---------------------------------------------------------------------------
VFileSelect::~VFileSelect()
{
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, initial file
// path path in a dialog titled title. If a file is
// selected, the path of the selected file is copied to path and a value of
// TRUE is returned.  If the dialog is canceled, path is unaffected and a
// value of FALSE is returned.
// If dstyle == SaveAs, then perform a save-as
// dialog where the user may type in a new file name rather than select an
// existing one.
// if dstyle == FileMustExist, the selected file must exist
// optionally, a pointer to a | separated filter string can be passed in
//
boolean VFileSelect::dialog(DlgStyle dstyle,
							const VString& title,
							VPathString& path,
							VWindow *win, 
							VString *filter,
							int *indexp)
{
	boolean success;
	int		filter_index = (indexp == 0) ? 0 : *indexp;

	VFileSelect f(dstyle, title, path, filter, success, filter_index, win);

	if (success && indexp != 0) {
		*indexp = filter_index;
	}

	return success;
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, with prompt
// prompt, initial file name file, and filter filter in a dialog titled
// title. Return a new VString object with the selected file, or return NIL
// if the dialog was cancelled. (prompt is ignored)
// THIS IS FOR V2.0 COMPATABILITY ONLY
//
VString *VFileSelect::dialog(VString *file, VString *filter, char *title,
							 VWindow *win, char *prompt ...)
{
	VFileSelect f(FALSE, title, 0, &file, filter, win);

	return file;
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, with prompt
// prompt, initial file name file in a dialog titled title. Return
// a new VString object with the selected file or return NIL if the dialog was
// cancelled. If saveAs is TRUE, then perform a save-as dialog where the
// user may type in a new file name rather than select an existing one.
// (prompt is ignored)
// THIS IS FOR V2.0 COMPATABILITY ONLY
//
VString *VFileSelect::dialog(boolean saveAs, VString *file, char *title, VWindow *win, char *prompt...)
{
	VFileSelect f(saveAs, title, 0, &file, NIL, win);

	return file;
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, with prompt
// prompt, initial file name file, and filter filter. Return a new
// VString object with the selected file, or return NIL if the dialog was
// cancelled.  (prompt is ignored)
// THIS IS FOR V2.0 COMPATABILITY ONLY
//
VString *VFileSelect::dialog(VWindow *win, char *prompt, VString *file,
							 VString *filter)
{
	VFileSelect f(FALSE, 0, prompt, &file, filter, win);

	return file;
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, with prompt
// prompt, and initial file name file. Return a new VString object with
// the selected file, or return NIL if the dialog was cancelled. If saveAs
// is TRUE, then perform a save-as dialog where the user may type in a new file
// name rather than select an existing one.	(prompt is ignored)
// THIS IS FOR V2.0 COMPATABILITY ONLY
//
VString *VFileSelect::dialog(boolean saveAs, VWindow *win, char *prompt,
							 VString *file)
{
	VFileSelect f(saveAs, 0, prompt, &file, NIL, win);

	return file;
}

// ---------------------------------------------------------------------------
// Perform a file selection dialog within window win, with prompt
// prompt, initial file path path in a dialog titled title. If a file is
// selected, the path of the selected file is copied to path and a value of
// TRUE is returned.  If the dialog is canceled, path is unaffected and a
// value of FALSE is returned. If saveAs is TRUE, then perform a save-as
// dialog where the user may type in a new file name rather than select an
// existing one.  (prompt is ignored)
// THIS IS FOR V2.0 COMPATABILITY ONLY
//
boolean VFileSelect::dialog(boolean saveAs, VPathString &path, char *title,
							VWindow *win, char *prompt ...)
{
	boolean ret = FALSE;

	VString filter;

	VString *fp = &path;
	const char *p = 0;
	char b[200];

	if (path.suffixOf(filter)) {
		sprintf(b, "*.%s", filter.gets());
		filter = b;
		p = filter.gets();
	}

	VFileSelect f(saveAs, title, 0, &fp, p ? &filter : 0, win);

	if (fp != 0) {
		ret = TRUE;
		path = *fp;
		delete fp;
	}

	return ret;
}

// ---------------------------------------------------------------------------
// If uFilter is a VStream object (v2.0 only), then assume it contains an MS
// Windows Common Dialog filter specification and copy its contents to the
// instance variable filter.  Otherwise, if uFilter is a VString object, 
// assume it is file filter specification separated by \ characters and turn it
// into an MS Windows Common VDialog filter specification by inserting it twice
// into the instance variable filter.
//
void VFileSelect::processFilter(VString *uFilter)
{
	integer slen;

	if (uFilter == 0) {
		return;
	}

	if (uFilter->isA(VStreamCls)) {
		wcFilter.copyFrom(uFilter);
	}
	else if ((slen = uFilter->size()) > 0) {
		// separate into filter pairs
		char *s = new char[slen + 1];
		uFilter->gets(s, slen);

		char	*p, *q;
		boolean  done = FALSE;
		p = q = s;
		while (1) {
			// skip double byte characters
			//if ( isleadbyte( *p ) ) {
			//	p = p + 2;
			//}
			if (*p == '|' || *p == '\0') {
				if (*p == '\0') {
					done = TRUE;
				}

				*p = '\0';
				wcFilter << q;
				wcFilter << '\0';	// separate each with a NULL byte

				if (done) {
					wcFilter << '\0';	// terminate with two NULL bytes
					break;
				}

				q = p + 1;
			}

			p++;
		}

		delete s;
	}
	else {
		// default
		wcFilter << *uFilter;
		wcFilter << '\0';	// separate each with a NULL byte
		wcFilter << *uFilter;
		wcFilter << '\0';	// terminate with two NULL bytes
		wcFilter << '\0';
	}
}

// ---------------------------------------------------------------------------
// Where it actually happens...
//
boolean VFileSelect::operator()(DlgStyle dstyle,
								const VString& title,
								VPathString& path,
								VString *filter,
								int& filter_index,		// 1-indexed!
								VWindow *parentwin)
{
	char	filetitle[128];
	char	filepath[256];
	boolean success = FALSE;

	if (rememberInitialPath && (path == "")) {
		if (dstyle & FileOpen) {
			path = lastOpenPath;
		}
		else if (dstyle & SaveAs) {
			path = lastSavePath;
		}
	}

	if (filter == 0) {
		wcFilter.write("All files (*.*)\0*.*\0", 20);
		wcFilter << '\0';
		wcFilter << '\0';
	}
	else {
		processFilter(filter);
	}

	OPENFILENAME ofn;

	ofn.lStructSize		= sizeof(OPENFILENAME);
	ofn.hwndOwner		= ((parentwin == 0) ? 0 : parentwin->hwnd());
	ofn.lpstrFilter		= wcFilter.gets();
	ofn.lpstrCustomFilter	= 0;
	ofn.nMaxCustFilter	= 0;
	ofn.nFilterIndex	= filter_index;

	VString filename, directory;
	path.nameOf(filename);
	path.directoryOf(directory);
	strcpy(filepath, filename.gets());
	ofn.lpstrInitialDir = directory.gets();

	ofn.lpstrFile		= filepath;
	ofn.nMaxFile		= sizeof(filepath);
	ofn.lpstrFileTitle	= filetitle;
	ofn.nMaxFileTitle	= sizeof(filetitle);
	ofn.lpstrTitle		= (title.gets());
	ofn.lpstrDefExt		= 0;
	ofn.Flags			= OFN_NOCHANGEDIR | OFN_HIDEREADONLY;

	if (dstyle & FileMustExist) {
		ofn.Flags		|= OFN_FILEMUSTEXIST;
	}

	if (dstyle & ConfirmReplace) {
		ofn.Flags		|= OFN_OVERWRITEPROMPT;
	}

	nativeDialog = TRUE;
	notifier->startModal( this);

	if (dstyle & FileOpen) {
		success = (boolean)GetOpenFileName((LPOPENFILENAME)&ofn);
	}
	else {
		success = (boolean)GetSaveFileName((LPOPENFILENAME)&ofn);
	}

	// don't hide because the common dialog was destroyed when OK or Cancel
	// was hit.
	notifier->endModal(FALSE);

	if (success) {
		path.puts(filepath);
		filter_index =	int(ofn.nFilterIndex);
		if (rememberInitialPath) {
			if (dstyle & FileOpen) {
				lastOpenPath = path;
			}
			else if (dstyle & SaveAs) {
				lastSavePath = path;
			}
		}
	}

	return success;
}
