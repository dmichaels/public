// $Header:   Q:/views/win/vcs/control.h_v   1.27   Mar 04 1997 16:32:06   gregm  $ 

//	control.h
//
//	VControl class interface
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
//	07/15/92 jmd	preened
//	04/28/93 pmcm	added constructor that takes name argument;
//	              	added getFrom(), putTo(), realize();
//					added reference to VArchiver.
//	05/13/93 pmcm	preened, VString *controlText for realize().
//	05/20/93 jmd	added fontSize()
//	05/24/93 jmd	preened, removed free(), added shadow
//	06/28/93 dgm	added setValue(), getValue(), and value.
//	07/14/93 dgm	added setClient() and getClient().
//	07/21/93 pmcm	removed class keyword in friend declaration
//	07/22/93 dgm	added final default boolean argument to getFrom()
//					to optionally suppress realization of the (GUI) object.
//					Also made realize() virtual.
//	07/27/93 pmcm	added clientIndex;
//	07/28/93 dgm	added getClientIndex() and setClientIndex().
//	08/08/93 jmd	renamed value to idNo, made VFrame const
//	08/16/93 jmd	made name const VString
//	08/30/93 pmcm	made get/setClientIndex() take integer not int.
//	09/01/93 dgm	M8 changes; added declaration of VNotifier & VAccelerator.
//	03/01/94 pmcm	putText() now takes const VString&; included str.h
//	08/12/94 dss	Ctl3d.
//	10/30/94 jmd	added init(), stubs for tooltip
//	06/12/95 jld	adding tooltip support
//	07/12/95 evc	Added cvassert.h
//	07/13/95 dss	Added winclass.h
//	08/22/95 dss	Removed ctl3d stuff. It inherits from VWindow, now.
//	09/01/95 dss	VStyle changes.
//  10/04/95 evc    Moved font manipulation to VWindow
//	10/12/95 dss	add default style functionality.
//	11/02/95 dgm	Added getNonClientArea().
//	11/10/95 dgm	Removed getNonClientArea(); added getSystemBorderSize().
//	01/04/96 dgm	Added another version of setControlHelpMsg().
//	01/04/96 dgm	Overrode getPreferredSize(); added queryPreferredSize().
//	01/29/96 dgm	Preferred-size changes.
//	09/12/96 dgm	Added systemFontChanged().
//	09/20/96 dgm	Added fontSize().
//	09/30/96 dgm	Added hack useBrokenFontSize() as a temporary workaround
//					for VRFs which were created using a PRW which was built
//					without the 9/20 fontSize() fix.
// ---------------------------------------------------------------------------

#ifndef VCONTROL_H
#define VCONTROL_H

#include "window.h"
#include "str.h"
#include "cvassert.h"
#include "winclass.h"

CLASS VFont;

#ifndef CV_NOARCHIVER
CLASS VArchiver;
#endif

CLASS VNotifier;
CLASS VGroup;
CLASS VAccelerator;

CLASS VTimer;
CLASS VControlHelp;

extern VClass *VControlCls;

//	The VControl class is an abstract class that provides various
//	control mechanisms for interactive applications.  Each VControl
//	object has a client object and a pointer to a method of the client which
//	is used to notify the client when some subclass specific event occurs. 
//	Most subclasses of VControl have a text content.  Methods are
//	defined which allow interrogation and setting of that text.

CLASS VControl : public VWindow {

public:

	//	Constructors/destructors

					VControl();

					VControl(	unsigned 	cntrl, 
								VWindow    *pwin);

					VControl(	const VFrame& frame, 
								VWindow    *pwin, 
								const VStyle& style = StyleDefault);

					VControl(	const VString& wname, 
								const VFrame& frame, 
								VWindow    *pwin, 
								const VStyle&	style = StyleDefault);

					~VControl();

	VClass 		   *iam();

	boolean 		getRscWindow(unsigned cntrl, VWindow *pwin);

	// Interrogating and Setting Text Content

	virtual int		getText(VString& s);
	virtual int 	getText(char *buffer, unsigned len);
	unsigned		sizeOfText();
	void			putText(const VString& s);

	VString 		getText();

	// Interrogating and Setting Font

#if 0
	VFont  	   	   *getFont() const;
#endif

	void			fontSize(int *, int *);
	void			setFont(VFont *);

	// Interrogating Size

	virtual void 	sizeOf(int *w, int *h);
	static void 	stdSize(VClass *cls, int *w, int *h);

	// VWindow event method overrides
	boolean 		givenFocus();
	boolean			erased();
	boolean 		key(int k, char ch);
	boolean			paint();

	void	 setIdNo(int v);
	int		 getIdNo() const;
	void	 setClient(VObject *);
	VObject *getClient() const;

	inline virtual enum WinType winType() {return WinControl;}

	// tooltip support
	void			setControlHelp(VControlHelp* help);
	void			setControlHelpId(long id);
	long			setControlHelpMsg(VString* msg, long id=-1);
	long			setControlHelpMsg(const VString&, long id = -1);

	VControlHelp*	getControlHelp() const;
	long			getControlHelpId() const;

	boolean			mouseEnter(int);
	boolean			mouseExit(int);
	boolean			mouseDn(int, int);
	boolean			rmouseDn(int, int);

	// styles

	static VStyle& getClassDefaultStyle(void);
	virtual VStyle& getDefaultStyle(void) const;

	// Geometry.

    virtual void	getPreferredSize(int *w, int *h) const;
    virtual void	getSystemBorderSize(int *, int *) const;
    virtual void	getInternalBorderGeometry(int *, int *,
											  int *, int *) const;
    void			queryPreferredSize(int *, int *,
									   boolean = FALSE, boolean = FALSE,
									   boolean = FALSE,
									   VFont * = 0, const char * = 0) const;

	// Archival and Retrieval

#ifndef CV_NOARCHIVER
	integer			getClientIndex() const;
	void			setClientIndex(integer);

	void			putTo(VArchiver& a);
	void			getFrom(VArchiver& a);
	void			getFrom(VArchiver& a, VObject *pwin,
							boolean do_realize = TRUE);
	virtual void	realize(VWindow *pwin);
#endif

protected:

	friend VNotifier;
	friend VAccelerator;
	friend VGroup;
		   	
	VObject    	   *client;			// client object for callback method
	//VFont  	   	   *ctrlFont;		// font object for this control

	int				idNo;			// control id number

	void			getInfo(VWinInfo &cntrlinfo);
	virtual boolean notify(int code );
	void			getMinSize(short& w, short& h);

	boolean			getShadow();

#ifndef CV_NOARCHIVER
	void			putTo(VArchiver& a, boolean most_derived);
#endif

	void			systemFontChanged();
	void			init();

private:
	unsigned		hasShadow : 1;
	unsigned		amUsingBrokenFontSize : 1;

protected:

#ifndef CV_NOARCHIVER
	integer			clientIndex;	
	VString 	   *controlText;	//	(realization data)
#endif

	VControlHelp*	_ctrlHelp;
	long			_ctrlHelpId;

};

// ---------------------------------------------------------------------------
inline void VControl::setIdNo(int v)
{
	idNo = v;
}

// ---------------------------------------------------------------------------
inline int VControl::getIdNo() const
{
	return idNo;
}

// ---------------------------------------------------------------------------
inline VControlHelp* VControl::getControlHelp() const
{
	return _ctrlHelp;
}

// ---------------------------------------------------------------------------
inline long VControl::getControlHelpId() const
{
	return _ctrlHelpId;
}

inline void VControl::setControlHelpId(long id)
{
	_ctrlHelpId = ( id < 0 ) ? 0 : id;
}

#if 0
// ---------------------------------------------------------------------------
inline VFont *VControl::getFont()  const
{ 
	return ctrlFont; 
}
#endif

// ---------------------------------------------------------------------------
inline boolean VControl::getShadow() 
{ 
	return (boolean) hasShadow; 
}

// ---------------------------------------------------------------------------
inline void VControl::setClient(VObject *client_object)
{
	if (client_object != 0) {
		client = client_object;
	}
}

// ---------------------------------------------------------------------------
inline VObject *VControl::getClient() const
{
	return client;
}

#ifndef CV_NOARCHIVER
// ---------------------------------------------------------------------------
inline integer VControl::getClientIndex() const
{
	return clientIndex;
}

// ---------------------------------------------------------------------------
inline void VControl::setClientIndex(integer index)
{
	clientIndex = index;
}
#endif

#endif // VCONTROL_H
