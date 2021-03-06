//	imanim.h
//
//	VAnimatedImageBox interface [Common]
//
//  Allegris Foundation 1.1.00
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
//	04/01/96 dgm	Original.
//	02/05/97 tlf	Fixed to compile on HPUX.
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.

#ifndef VIMANIM_H
#define VIMANIM_H

// --------------------------------------------------------------------------
//
#include "imbox.h"

// --------------------------------------------------------------------------
//
CLASS	VAnimatedImageBox;
CLASS	VClass;
CLASS	VImage;
CLASS	VImageBox;
CLASS	VObject;
CLASS	VPathString;
CLASS	VString;
CLASS	VTimer;
CLASS	VWindow;

#ifndef CV_NOARCHIVER
CLASS	VArchiver;
#endif

// --------------------------------------------------------------------------
//
CLASS VAnimatedImageBox : public VImageBox {
public:
	enum Speed {	Slowest		= 1,
					VerySlow	= 2,
					Slow		= 4,
					Normal		= 8,
					Fast		= 12,
					VeryFast	= 18,
					Fastest		= 30,
					Default		= Normal };

public:
					VAnimatedImageBox(VWindow *,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox(const VFrame&, VWindow *,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox(const VString&,
									  VWindow *,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox(const VString&,
									  const VFrame&, VWindow *,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox(VWindow *, VImage *, int index = 0,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox(const VString&,
									  VWindow *, VImage *, int index = 0,
									  const VStyle& = StyleDefault);
					VAnimatedImageBox();
				   ~VAnimatedImageBox();
	VClass		   *iam();

	void			operator()(VWindow *);
	void			operator()(VWindow *, const VStyle&);
	void			operator()(VWindow *, const VStyle&, const VString&);
	void			operator()(const VFrame&, VWindow *);
	void			operator()(const VFrame&, VWindow *, const VStyle&);
	void			operator()(const VFrame&, VWindow *, const VStyle&,
							   const VString&);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	void			play(boolean = TRUE);
	void			stop(boolean = TRUE);

	void			showNext();
	void			showPrevious();
	void			showFirst();
	void			showLast();

	void			setSpeed(Speed);
	void			setFramesPerSecond(int);
	void			setMilliSeconds(int);

	void			setRowMajor(boolean = TRUE);
	void			setColumnMajor(boolean = TRUE);

	void			setPlayBackwards(boolean = TRUE);
	void			setPlayForewards(boolean = TRUE);
	void			setPlayRandom(boolean = TRUE);

	void			uponImageChanged(VObject *, method);

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

	boolean			isPlaying() const;
	boolean			isStopped() const;

	int				getSpeed() const;
	int				getFramesPerSecond() const;
	int				getMilliSeconds() const;

	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Protected section; please KEEP OUT if you're a non-deriving user!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

protected:
	virtual void		updateImage();
	virtual void		imageChanged();
	VTimer			   *getTimer() const;

protected:
	void				initialize();
	void				construct();

#ifndef CV_NOARCHIVER
public:
	virtual void		putTo(VArchiver&);
	void				putTo(VArchiver&, boolean);
	virtual void		getFrom(VArchiver&);
	virtual void		getFrom(VArchiver&, VObject *, boolean = TRUE);
	virtual void		realize(VWindow *);
protected:
#endif


	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
	// Private section!
	// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

private:
						VAnimatedImageBox(const VAnimatedImageBox&) {}
	VAnimatedImageBox&	operator=(const VAnimatedImageBox&) { return *this; }

private:
	VTimer			   *theTimer;
	void	(VObject::*imageChangedMethod)(VAnimatedImageBox *);
	short				milliSeconds;
	unsigned char		rowMajor : 1;
	unsigned char		playBackwards : 1;
	unsigned char		playRandom : 1;
	unsigned char		currentlyPlaying : 1;
};

extern VClass *VAnimatedImageBoxCls;

// --------------------------------------------------------------------------
//
inline void VAnimatedImageBox::stop(boolean b)
{
	play(!b);
}

inline boolean VAnimatedImageBox::isPlaying() const
{
	return currentlyPlaying;
}

inline boolean VAnimatedImageBox::isStopped() const
{
	return !currentlyPlaying;
}

inline void VAnimatedImageBox::setColumnMajor(boolean b)
{
	setRowMajor(!b);
}

inline void VAnimatedImageBox::imageChanged()
{
	if ((imageChangedMethod != 0) && (getClient() != 0)) {
#if !defined(CPPV_HPUX)
		(getClient()->*imageChangedMethod)(this);
#else
		VObject *tmpObject = getClient();
		(tmpObject->*imageChangedMethod)(this);
#endif
	}
}

inline int VAnimatedImageBox::getSpeed() const
{
	return 1000 / milliSeconds;
}

inline int VAnimatedImageBox::getFramesPerSecond() const
{
	return 1000 / milliSeconds;
}

inline int VAnimatedImageBox::getMilliSeconds() const
{
	return milliSeconds;
}

inline void VAnimatedImageBox::setPlayBackwards(boolean b)
{
	playBackwards = b;
}

inline void VAnimatedImageBox::setPlayForewards(boolean b)
{
	playBackwards = !b;
}
inline void VAnimatedImageBox::setPlayRandom(boolean b)
{
	playRandom = b;
}

#endif // VIMANIM_H
