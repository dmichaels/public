//========================================================
//============== Author :: Scott Tattrie =================
//============== Date   :: Dec 5- 1994 ===================
//========================================================

/////////////////////  Includes  /////////////////////////

#include "testdial.h"
defineClass(TESTDIALOG, VDialog)

//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////
///////// Constructors  - Destructors  ///////////////////
//////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////


TESTDIALOG::TESTDIALOG()
{


}
TESTDIALOG::TESTDIALOG(VFrame f, VWindow *pwin, char *title) :
  VDialog(f,pwin,title)
{
modal();
}

TESTDIALOG::~TESTDIALOG()
{


}

