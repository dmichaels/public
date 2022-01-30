// $Header:   Q:/views/common/vcs/CLASS.H_V   1.19   Mar 24 1997 16:08:24   scottt  $ 

//	class.h
//
//	VClass class interface [Common]
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
//	09/09/92 jmd	added CV_NEARDATA to class macros
//	09/19/92 jmd	preened
//	02/23/92 pmcm	exploded/explained defineClass macro & its ilk
//	04/28/93 pmcm	preened
//	04/28/93 pmcm	added callbackData member, findCallback(),
//					updateCallbackData()
//	07/27/93 pmcm	added ghost callback facility
//	08/25/93 pmcm	callback facility is now through VCallbackList
//	09/01/93 dgm	added useClass() macro to allow force-loading of classes.
//	10/07/93 dgm	modified the linkClass() macro to declare the
//					necessary extern xyzCls object.
//	09/29/93 pkt	eliminated dead code supporting Mac memory pool.
//	10/29/93 pkt	eliminated #include "callback.h".
//	12/23/93 pkt	minor change for Symantec C++ compiler.
//	01/12/94 jmd	removed class identity macros and asA()
//	01/13/94 dgm	merged and preened
//	01/17/94 pkt	preened.
//	01/22/94 pkt	added hack to initialize all Views modules on Macintosh.
//	01/17/94 dgm	cfront C++ changes
//	01/27/94 pkt	fixed initialization hack.
//	02/01/94 pmcm	re-included "callback.h" for defineCallback() use
//	03/29/94 pkt	moved #include of "callback.h" to end.
//	03/30/94 pkt	simplified Mac initialization hack.
//	07/13/94 jld	removed defines contained in defs.h
//	08/26/94 pkt	added VDECLAREXX hack for Metrowerks C++.
//	08/29/94 pkt	updated documentation of meta-class mechanism.
//	10/17/94 dgm	Added OLD_CPP version of linkClass().
//	10/24/94 pkt	removed 'alwaysFalse'.
//	04/11/95 pkt	made 'construct' and 'of' char* parameters const.
//	04/11/95 pkt	changed 'name' and 'nameOf' to const.
//	04/12/95 pkt	merged for NT; made VDECLAREXX dependent on macintosh.
//	05/19/95 pkt	merged for common source.
//	05/26/95 pkt	combined international changes into common version.
//	09/06/95 pkt	removed OFI code.
//	10/03/95 pkt	added 'defineNestedClass'.
//	10/05/95 pkt	made 'construct()' const.
//	10/13/95 pkt	changed 'defineNestedClass' for CSet compiler.
//	10/20/95 pkt	changed 'defineNestedClass' so that multiset.cpp will
//					compile with Win 3.1.
//	12/15/95 tlf	changed 'defineNestedClass' so that multiset.cpp will
//					compile with HPUX.
//	02/19/96 pkt	changed 'declareClassNested' and 'defineNestedClass' to
//					declare their static data member right for Win 16.
//  02/20/96		Changes to allow for exporting
// 					metaclass objects and their pointers
//					from one DLL to another for 16 bit borland only.
//	06/04/96 glm	removed far from Borland externs, preened
//	06/24/96 glm	removed Borland 16 dll specific defines
//					no longer needed with multi-dlls
//	01/19/97 dgm	Added getBase().
// ---------------------------------------------------------------------------

#ifndef VCLASS_H
#define VCLASS_H

// ---------------------------------------------------------------------------
//
#include "defs.h"

// ---------------------------------------------------------------------------
//
typedef void *(*idfunc)();

// ---------------------------------------------------------------------------
//	The VClass class is used to implement class identity. An instance
//	of it is constructed for each class in the Allegris hierarchy via the 
//	defineClass(), defineTopClass(), defineSubClass(), defineSubSubClass()
//	macros. It holds the name, a null-constructor function, the size and a
//	referent to the super class of each defined class. Its methods are never
//	called directly. They are instead called through the use of the macros:
//	clsName(), getSize(), getPrivateSize(), getIsA() and isA(). The connection
//	between an instance of a class and its corresponding instance of VClass is
//	made through the iam() member function of each Allegris object.
//
CLASS VClass {
public:
	// Constructors & Destructor
					VClass(	VClass **, const char *);

					VClass(	VClass **, const char *, idfunc, int, VClass **); 

					VClass(	VClass **, const char *, idfunc, int, VClass **, 
							VClass **); 

					VClass(	VClass **, const char *, idfunc, int, VClass **, 
							VClass **, VClass **); 

				   ~VClass();

	static void    *construct(const char *);
	void   		   *construct() const;
	boolean 		isa(VClass *);
	void   		   *asa(char *, VClass *, int reverse = 0);
	static VClass  *of(const char *cls);
	const char	   *nameOf();
	int 		   	sizeOf();
	int				getprivate();
	VClass		   *getBase(int = 1) const;

protected:
	void 			setClass(VClass **, const char *, idfunc, int);

	idfunc			constructor; //	can manufacture an instance of the class
	VClass		  **super[3];	 //	meta-class instances of base class(es)
	const char	   *name;		 //	class name
	ushort			size;		 //	size (in bytes) of instances of the class
};

// ---------------------------------------------------------------------------
// Return a newly constructed instance of the class this instance
// of VClass represents.
//
inline void *VClass::construct() const 
{
	return constructor(); 
}

// ---------------------------------------------------------------------------
//
inline const char *VClass::nameOf() 
{
	return name; 
}

// ---------------------------------------------------------------------------
//
inline int VClass::sizeOf() 
{
	return size; 
}

// ---------------------------------------------------------------------------
//

#	define VDECLAREXX(c)	extern void *c##c();
#	define VDECLARECLS(c)	VClass *c##Cls=0;
#	define VDEFINECLX(c)
#	define VIAMRETURN(c)	c##Cls

#ifndef CVP_OLD_CPP

#define defineSubSubAbstractClass(c, s, s2, s3) \
	extern	VClass *s##Cls, *s2##Cls, *s3##Cls; \
	void	*c##c() { return (void*)0; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), &s##Cls, &s2##Cls, &s3##Cls); \
	VClass	*c::iam() { return c##Cls; }

#define defineSubAbstractClass(c, s1, s2) \
	extern	VClass * s1 ## Cls, *s2 ## Cls; \
	VDECLAREXX(c) \
	void	*c##c() { return (void*)0; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), &s1##Cls, &s2##Cls); \
	VClass	*c::iam() { return c##Cls; }

#define defineAbstractClass(c, s) \
	extern	VClass *s##Cls; \
	VDECLAREXX(c) \
	void	*c##c() { return (void *)0; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), &s##Cls); \
	VClass	*c::iam() { return c##Cls; }

#define defineTopAbstractClass(c) \
	VDECLAREXX(c) \
	void	*c##c() { return (void *)0; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), 0); \
	VClass	*c::iam() { return c##Cls; }

#define defineSubSubClass(c, s, s2, s3) \
	extern	VClass *s##Cls, * s2##Cls, * s3##Cls; \
	VDECLAREXX(c) \
	void	*c##c() { return (void*)new c; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), &s##Cls, &s2##Cls, &s3##Cls); \
	VClass	*c::iam() { return c##Cls; }

#define defineSubClass(c, s1, s2) \
	extern	VClass *s1##Cls, *s2##Cls; \
	VDECLAREXX(c) \
	void	*c##c() { return (void *)new c; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), &s1##Cls, &s2##Cls); \
	VClass	*c::iam() { return c##Cls; }

#define defineClass(c, s) \
	extern	VClass *s##Cls; \
	VDECLAREXX(c) \
	void	*c##c() { return (void *)new c; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA  _##c(&c##Cls, #c, c##c, sizeof(c), &s##Cls); \
	VClass	*c::iam() { return c##Cls; }


// ---------------------------------------------------------------------------
// 	defineClass constructs a meta-class instance and initializes a 
//	pointer to the meta-class instance.  A meta-class instance is a
//	VClass object that implements class identity.
//
//	Place this macro in the source module that contains the non-inline 
//	methods of the class you wish to define.  The module containing the
//	class declaration typically contains an extern VClass * declaration 
//	of the meta-class instance to allow users of the class access to the
//	meta-class instance.  For classes defined in a DLL, DLL clients gain 
//	access to the meta-class by using the importClass macro.
//
// 	Consider this example:
//
//		defineClass(myDerivation, myBase)
//
//	It does the following:
//
// 		1)	declares the external pointer to the meta-class instance
//			of the base class.
//
// 				extern VClass *myBaseCls; 
//
// 		2) 	defines a handy function named "myDerivationmyDerivation" that 
// 			will construct an object of the myDerivation class.	 The meta-
//			class instance contains a pointer to this function in order
//			to create new instances of the myDerivation class.
// 
// 				void *myDerivationmyDerivation() 
//				{ 
//					return (void *)new myDerivation; 
//				} 		   
//
// 		3)	defines a variable named "myDerivationCls" which points to
//			the meta-class instance of the derived class.  Later in the
//			macro (when _myDerivation is constructed) this pointer is
//			set to point to the meta-class instance.
//
//				VClass *myDerivationCls = 0; 
//		
//		4)	constructs a VClass object which serves as the meta-class 
//			instance of the derived class.  The meta-class instance is
//			named "_myDerivation".  During construction, the meta-class
//			initializes "myDerivationCls" to point to itself and enters
//			itself in the global table of classes (classTable).
//
//				VClass CV_NEARDATA _myDerivation(&myDerivationCls, 
//											"myDerivation", 
//											myDerivationmyDerivation,
//											sizeof(myDerivation), 
//											&myBaseCls); 
//
//		5)	defines the method to identify an object by its meta-class.
//
//				VClass *myDerivation::iam() 
//				{ 
//					return(myDerivationCls); 
//				}
//
// ---------------------------------------------------------------------------


#define defineTopClass(c) \
	VDECLAREXX(c) \
	void	*c##c() { return (void *)new c; } \
	VClass	*c##Cls=0; \
	VClass	CV_NEARDATA _##c(&c##Cls, #c, c##c, sizeof(c), 0); \
	VClass	*c::iam() { return c##Cls; }

#define declareClassNested(c) \
	static	void	*c##c(); \
	static	VClass	*c##Cls; \
	static	VClass	CV_NEARDATA _##c;

#define defineNestedClass(n, c, s) \
	extern	VClass *s##Cls; \
	void	*n::c##c() { return (void *)new c; } \
	VClass	*n::c##Cls = 0; \
	VClass	CV_NEARDATA n::_##c(&n::c##Cls, #n"::"#c, &(n::c##c), sizeof(n::c), &s##Cls); \
	VClass	*n::c::iam() { return c##Cls; }



#define importClass(c) \
		VClass	*c##Cls=0; \
		VClass	CV_NEARDATA _##c(&c##Cls, #c);

// ---------------------------------------------------------------------------
// 	importClass grants a DLL client access to meta-class instances that are 
//	defined in a DLL.
//
// For example, importClass(myClass)
//	
// 		1)	defines a variable named "myClassCls" which points to
//			a meta-class instance of the myClass class.
//
//				VClass *myClassCls = 0;
//
//		2)	initializes "myClassCls" to point to the meta-class instance of
//			myClass.  The meta-class instance is constructed by the DLL
//			with the defineClass macro (see above).
//
//				VClass _myClass(&myClassCls, "myClass");
//
// ---------------------------------------------------------------------------


#if !defined (DLL)

#define linkClass(class_name)	extern VClass *class_name##Cls; \
								static VClass *__xyzzy##class_name = \
								class_name##Cls;

#else

#define linkClass(class_name)

#endif

// ---------------------------------------------------------------------------
//

#else  // CVP_OLD_CPP defined

#define defineSubSubAbstractClass(c, s1, s2, s3) \
	extern	VClass *s/**/Cls, *s2/**/Cls, *s3/**/Cls; \
	void	*c/**/c() { return(void*)0;} \
	VClass	*c/**/Cls=0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), &s/**/Cls, &s2/**/Cls, &s3/**/Cls); \
	VClass	*c::iam() { returnc/**/Cls;}

#define defineSubAbstractClass(c, s1, s2) \
	extern	VClass *s1/**/Cls, *s2/**/Cls; \
	void	*c/**/c()  { return (void *)0; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c" , c/**/c , sizeof(c), & s1/**/Cls, & s2/**/Cls); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineAbstractClass(c, s) \
	extern	VClass *s/**/Cls; \
	void	*c/**/c() { return (void *)0; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c" , c/**/c , sizeof(c), & s/**/Cls); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineTopAbstractClass(c) \
	void	*c/**/c() { return (void *)0; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), 0); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineSubSubClass(c, s, s2, s3) \
	extern	VClass *s/**/Cls, *s2/**/Cls, *s3/**/Cls; \
	void	*c/**/c() { return(void*)new c; } \
	VClass	*c/**/Cls=0;\
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), &s/**/Cls, &s2/**/Cls, &s3/**/Cls); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineSubClass(c, s1, s2) \
	extern	VClass *s1/**/Cls, *s2/**/Cls; \
	void	*c/**/c() { return (void *)new c; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), & s1/**/Cls, & s2/**/Cls); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineClass(c, s) \
	extern	VClass *s/**/Cls; \
	void	*c/**/c() { return (void *)new c; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), & s/**/Cls); \
	VClass	*c::iam() { return c/**/Cls; }

#define defineTopClass(c) \
	void	*c/**/c() { return (void *)new c; } \
	VClass	*c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c", c/**/c, sizeof(c), 0); \
	VClass	*c::iam() { return c/**/Cls; }

#define importClass(c) VClass * c/**/Cls = 0; \
	VClass	CV_NEARDATA _/**/c(&c/**/Cls, "c");

#define linkClass(class_name)	extern VClass *class_name/**/Cls; \
								static VClass *__xyzzy/**/class_name = \
								class_name/**/Cls;

#endif // CVP_OLD_CPP

// ---------------------------------------------------------------------------
// Callback.h is not documented, so users may #include class.h to get it.
// Include this last, because it needs VClass defined first.
//
#include "callback.h"

#ifdef CV_NOARCHIVER

#ifdef defineArchiveTag
#undef defineArchiveTag
#endif
#ifdef defineArchiveRevision
#undef defineArchiveRevision
#endif

#define defineArchiveTag(c, r)
#define defineArchiveRevision(c, r)

#endif

#endif // VCLASS_H
