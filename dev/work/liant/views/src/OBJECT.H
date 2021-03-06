// $Header:   Q:/views/common/vcs/OBJECT.H_V   1.12   Mar 24 1997 16:12:46   scottt  $ 

//  object.h
//
//  VObject class interface [Common]
//
//  Allegris Foundation 1.1.00
//  Copyright (c) 1997 by INTERSOLV, Inc.
//  +-----------------------------------------------------------------+
//  | This product is the property of INTERSOLV, Inc. and is licensed |
//  | pursuant to a written license agreement.  No portion of  this   |
//  | product may be reproduced without the written permission of     |
//  | INTERSOLV, Inc. except pursuant to the license agreement.       |
//  +-----------------------------------------------------------------+
//
//  Revision History:
//  -----------------
//  04/09/93 pmcm   changed id to VObject *
//  04/28/93 pmcm   added virtual void getFrom(VArchiver& a, VObject *data);
//  05/14/93 pmcm   added virtual destructor
//  05/17/93 pmcm   merged MOTIF and Windows header;
//                  operator==, etc now specify boolean return value; preened.
//  06/21/93 pmcm   added isTemporaryObject(boolean),
//                  boolean isTemporaryObject(), and temporaryObjectFlag
//                  f/use w/VArchiver.
//  06/21/93 pmcm   added explicit default constructor.
//  07/22/93 dgm    added final default boolean argument to getFrom()
//                  to optionally suppress realization of the (GUI) object.
//  07/31/93 jmd    made isEqual, comparison operators const
//  08/07/93 jmd    preened, made error() take const char *
//  09/01/93 dgm    M8 changes; moved global "method" typedef to after the
//                  VObject definition and added a nested typedef in VObject.
//  10/07/93 dgm    added another deepCopy() to support GUI objects.
//  10/19/93 pkt    eliminated NULL_METHOD.
//  10/25/93 pkt    added parameter to deepCopy.
//  11/02/93 pkt    made method typedef work with VFLATTYPES.
//  11/18/93 pkt    added isTemporaryObject definitions.
//  12/23/93 pkt    changed typedef of method for Symantec compiler.
//  01/12/94 jmd    made class identity macros into member functions
//  01/12/94 jmd    removed self
//  01/13/94 dgm    merged and preened
//  01/18/94 pkt    merged with Macintosh; moved virtual functions nil and
//                  reset to object.cpp.
//  01/20/94 pkt    added definition of isA for VWARNMULTINLINE.
//  01/17/94 dgm    cfront C++ changes
//  01/28/94 dgm    made nil() and reset() non-inline (again).
//  04/11/95 pkt    made 'clsName' return const char *.
//  05/19/95 pkt    merged for common source.
//  05/26/95 pkt    combined international changes into common version.
//  09/06/95 pkt    removed OFI code.
//  10/10/95 tomf   VStyle changes. 
//	02/24/97 tlf	Updated version number to 1.00.02 and 
//			copyright date to 1997.
// ---------------------------------------------------------------------------

#ifndef VOBJECT_H
#define VOBJECT_H

// ---------------------------------------------------------------------------
//
#include "defs.h"

// ---------------------------------------------------------------------------
//
CLASS VClass;
CLASS VObject;

// ---------------------------------------------------------------------------
//
typedef long HashKey;

// ---------------------------------------------------------------------------
//
#define HASH_KEY_MASK 0x7fffffff

#define MAP(fm,to)      ((to)->copyFrom(fm) || (fm)->copyTo(to))
#define MAPREF(fm)      return (!copyFrom(fm.ref) && !fm.ref->copyTo(this))
#define ASSIGN(fm)      MAPREF(fm)
#define COPYINIT(fm)    init(); copyFrom(&fm)

// ---------------------------------------------------------------------------
//
CLASS VArchiver;

// ---------------------------------------------------------------------------
//
#if !defined(_MSC_VER) && !defined(PM)

#   if defined(UNIX)

#       if defined(__ultrix)    // !!! gjl OFI
            typedef long    (VObject::*method)(long, long, long);
#       else
            typedef long    (VObject::*method)();
#       endif

        typedef     long    (VObject::*method0)();
        typedef     long    (VObject::*method1)(long);
        typedef     long    (VObject::*method2)(long, long);
        typedef     long    (VObject::*method3)(long, long, long);

#   else    // !!! pmcm why the difference in return types?

        typedef     boolean (VObject::*method)();
        typedef     boolean (VObject::*method0)();
        typedef     boolean (VObject::*method1)(long);
        typedef     boolean (VObject::*method2)(long, long);
        typedef     boolean (VObject::*method3)(long, long, long);

#   endif

#endif

// ---------------------------------------------------------------------------
//  The VObject class is the principle root of the C++/Views class
//  hierarchy.  The VObject is an abstract superclass supporting the notions
//  of class object copying, performing methods, archival, comparison and
//  equivalence.
//
CLASS VObject {
public:
#if defined(_MSC_VER) || defined(PM)
    typedef boolean (VObject::*method)();
#endif

    // Constructor & Virtual Destructor
                    VObject();
    virtual        ~VObject();

    virtual VClass *iam();

    boolean         nil();
    virtual void    reset();

    //  Copying
    VObject         *deepCopy(VObject *obj = 0);
    VObject         *deepCopy(VObject *obj,
                              VObject *data, boolean do_realize = TRUE);
    VObject         *copy();
    virtual boolean  copyTo(VObject *obj);
    virtual boolean  copyFrom(VObject *obj);

    //  Performing
#ifdef __ultrix
    boolean         perform(method, long param=0, long param1=0, long param2=0);
#else
    boolean         perform(method m);
    boolean         perform(method m, long param);
    boolean         perform(method m, long param1, long param2);
    boolean         perform(method m, long param1, long param2, long param3);
#endif

#ifndef CV_NOARCHIVER
    //  Archiving
    virtual void    getFrom(VArchiver& a);
    virtual void    getFrom(VArchiver& a, VObject *data,
                            boolean do_realize = TRUE);
    virtual void    putTo(VArchiver& a);

    boolean         isTemporaryObject();
    void            isTemporaryObject(boolean tf);
#endif

    // Equivalence and Comparision
    virtual boolean isEqual(VObject *obj) const;
    virtual HashKey hash() const;
    virtual int     compare(VObject *obj) const;

    boolean         operator<=(VObject& obj) const;
    boolean         operator>=(VObject& obj) const;
    boolean         operator<(VObject& obj) const;
    boolean         operator>(VObject& obj) const;
    boolean         operator==(VObject& obj) const;
    boolean         operator!=(VObject& obj) const;

    //  Miscellaneous Methods
    virtual void    error(const char *s);
    virtual boolean free();

    // Type methods (jmd 1/12/94)
    const char     *clsName() const;
    int             getSize() const;
    int             getPrivateSize() const;
    VClass         *getIsA() const;
    boolean         isA(VClass *c) const;

private:

#ifndef CV_NOARCHIVER
    //  Archiving
    boolean         temporaryObjectFlag;
#endif

};

extern VClass *VObjectCls;

// ---------------------------------------------------------------------------
//
#if defined(_MSC_VER) || defined(PM)
    typedef     boolean (VObject::*method)();
    typedef     boolean (VObject::*method0)();
    typedef     boolean (VObject::*method1)(long);
    typedef     boolean (VObject::*method2)(long, long);
    typedef     boolean (VObject::*method3)(long, long, long);
#endif

#if defined(ATT)
#   define      defMethod(o,m)  (&o::m)
#   define      methodOf(o,m)   (method)(&o::m)
#   define      NIL_METHOD      defMethod(VObject, nil)
#elif defined(OREGON)
#   define      defMethod(o,m)  (method)(o::m)
#   define      methodOf(o,m)   (method)(o::m)
#   define      NIL_METHOD      0
#elif _MSC_VER >= 700
#   define      defMethod(o,m)  (method)(boolean (o::*)() )(&o::m)
#   define      methodOf(o,m)   (method)(boolean (o::*)() )(&o::m)
#   define      NIL_METHOD      0
#else
#   define      defMethod(o,m)  (method)(&o::m)
#   define      methodOf(o,m)   (method)(&o::m)
#   define      NIL_METHOD      0
#endif

// ---------------------------------------------------------------------------
//
#include "class.h"


#ifndef CV_NOARCHIVER

// ---------------------------------------------------------------------------
//
inline boolean VObject::isTemporaryObject()
{
    return temporaryObjectFlag;
}

// ---------------------------------------------------------------------------
//
inline void VObject::isTemporaryObject(boolean tf)
{
    temporaryObjectFlag = tf;
}

#endif // ARCHIVER

// ---------------------------------------------------------------------------
// Returns non-zero if this object is <= 'obj' else returns 0.
//
inline boolean VObject::operator<= (VObject& obj) const
{
    return compare(&obj) <= 0;
}

// ---------------------------------------------------------------------------
// Returns non-zero if this object is >= 'obj' else returns 0.
//
inline boolean VObject::operator>= (VObject& obj) const
{
    return compare(&obj) >= 0;
}

// ---------------------------------------------------------------------------
// Returns non-zero if this object is < 'obj' else returns 0.
//
inline boolean VObject::operator< (VObject& obj) const
{
    return compare(&obj) < 0;
}

// ---------------------------------------------------------------------------
// Returns non-zero if this object is > 'obj' else returns 0.
//
inline boolean VObject::operator> (VObject& obj) const
{
    return compare(&obj) > 0;
}

// ---------------------------------------------------------------------------
// This method  is equivalent to calling  `isEqual(&obj)`.
//
inline boolean VObject::operator== (VObject& obj) const
{
    return isEqual(&obj);
}

// ---------------------------------------------------------------------------
// This method  is equivalent to calling  `!isEqual(&obj)`.
//
inline boolean VObject::operator!= (VObject& obj) const
{
    return !isEqual(&obj);
}

// ---------------------------------------------------------------------------
//
inline const char *VObject::clsName() const
{
    return ((VObject *)this)->iam()->nameOf();
}

// ---------------------------------------------------------------------------
//
inline int VObject::getSize() const
{
    return ((VObject *)this)->iam()->sizeOf();
}

// ---------------------------------------------------------------------------
//
inline int VObject::getPrivateSize() const
{
    return ((VObject *)this)->iam()->getprivate();
}

// ---------------------------------------------------------------------------
//
inline VClass *VObject::getIsA() const
{
    return ((VObject *)this)->iam();
}

// ---------------------------------------------------------------------------
//
inline boolean VObject::isA(VClass *c) const
{
    return ((VObject *)this)->iam()->isa(c);
}

#endif // VOBJECT_H
