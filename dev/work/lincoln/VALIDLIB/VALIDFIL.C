/***********************************************************************
* PROGRAM:	VALID Applications Library
* FILE:		valid.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		December 1984
***********************************************************************/

/*
 * Theses functions return in (static storage),
 * various VALID data file and directory names.
 * -----------------------------------------------------------------------
 *  vf_brdlsthdr - return the wirelist header file name
 *		   of the given board name.
 * -----------------------------------------------------------------------
 *  vf_ged	 - return the graphics editor file of the given board name
 * -----------------------------------------------------------------------
 *  vf_lsthdr 	 - return the per-board wirelist header file name (stub)
 * -----------------------------------------------------------------------
 *  vf_pgnd	 - return the post-processor power/ground list file name
 * -----------------------------------------------------------------------
 *  vf_pkgdesc	 - return the per-package description file name (stub)
 * -----------------------------------------------------------------------
 *  vf_pkgdir	 - return the per-package directory file name
 *		   of the given package name.
 * -----------------------------------------------------------------------
 *  vf_pkgfile	 - return the per-package description file name
 *		   of the given package name.
 * -----------------------------------------------------------------------
 *  vf_pkgmap	 - return the package-name to description-directory-name
 *		   mappping file name
 * -----------------------------------------------------------------------
 *  vf_prtmap	 - return the part-name to package-name mapping file name
 * -----------------------------------------------------------------------
 *  vf_sysdir	 - return the VALID system directory name
 * -----------------------------------------------------------------------
 *  vf_xnet	 - return the post-processor expanded net list name
 * -----------------------------------------------------------------------
 *  vf_xprt	 - return the post-processor expanded parts list name
 * -----------------------------------------------------------------------
 */

#include "cdef.h"
#include <stdio.h>

#define VF_GED		"logic.1.1"
#define VF_LSTHDR	"header"
#define VF_PGND		"pstpgnd.dat"
#define VF_PKGDESC	"pkg_desc"
#define VF_PKGMAP	"/u0/lincoln/layout/layout.lib"
#define VF_PRTMAP	"/u0/lincoln/part_map"
#define VF_SYSDIR	"/u0/lincoln/layout"
#define VF_XPRT		"pstxprt.dat"
#define VF_XNET		"pstxnet.dat"

#define VF_BUFSIZE	257

#define str_eq(a,b)	(strcmp((a),(b)) == 0)
#define strn_eq(a,b,n)	(strcmp((a),(b),(n)) == 0)

/*
 *  vf_pkgfile
 *
 *  Return the name of the description file for the package named
 *  "pkgname" (e.g. "DIP8").  If there is a problem, return NULL,
 *  otherwise return:
 *
 *		vf_pkgdir(pkgname)/VF_PKGDESC
 */

char *
vf_pkgfile(pkgname)
register char *pkgname;
{
	char		*vf_pkgdir();
	FILE		*f;
	char		*dir;
	static char	pkgdesc[VF_BUFSIZE];

	if((dir = vf_pkgdir(pkgname)) == NULL)
		return(NULL);
	sprintf(pkgdesc,"%s/%s",dir,VF_PKGDESC);
	return(pkgdesc);
}

/*
 *  vf_pkgdir
 *
 *  Return the name of the package description directory
 *  for the given package name.  Return NULL if unable to
 *  access it, otherwise return:
 *
 *	VF_SYSDIR/package_description_directory_name
 *
 *  --------------------------------------------------------
 *  All package description files reside in VF_SYSDIR.  The
 *  file which maps package names into description directory
 *  names is VF_PKGMAP.  Each entry is of the form:
 *  --------------------------------------------------------
 *  "package_name" 'package_description_directory_name';
 *  --------------------------------------------------------
 */

char *
vf_pkgdir(pkgname)
char *pkgname;
{
	register FILE	*f;
	char		*cindex(), *findline();
	static char	pkgdir[VF_BUFSIZE];
	static char	nospaces[] = "^ \t\n";
	register char	*p, *q;
	char		pkgbuf[VF_BUFSIZE];
	char		line[VF_BUFSIZE];

	if(pkgname == NULL || *pkgname == EOS)
		return(NULL);
	if((f = fopen(VF_PKGMAP,"r")) == NULL) {
#ifdef DEBUG
		fprintf(stderr,"Can't open \"%s\"",VF_PKGMAP);
#endif DEBUG
		return(NULL);
	}
	sprintf(pkgbuf,"\"%s\"",pkgname);
	while(findline(pkgbuf,line,VF_BUFSIZE,f) != NULL) {
		if((p = cindex(line,nospaces,1)) == NULL || *p++ != '"')
			continue;
		if((q = cindex(p+1,"\"",1)) == NULL)
			continue;
		*q++ = EOS;
		if(!str_eq(p,pkgname))
			continue;
		if((p = cindex(q,nospaces,1)) == NULL || *p++ != '\'')
			continue;
		if((q = cindex(p,"'",1)) == NULL ||
		   *cindex(q+1,nospaces,1) != ';')
			continue;
		*q = EOS;
		sprintf(pkgdir,"%s/%s",VF_SYSDIR,p);
		fclose(f);
		return(pkgdir);
	}
#ifdef DEBUG
	fprintf(stderr,"Package \"%s\" not found in \"%s\"",pkgname,VF_PKGMAP);
#endif DEBUG
	fclose(f);
	return(NULL);
}

char *
vf_brdlsthdr(board)
register char *board;
{
	static char buf[VF_BUFSIZE];

	sprintf(buf,"%s/%s",board,VF_LSTHDR);
	return(buf);
}

char *
vf_ged(board)
register char *board;
{
	static char buf[VF_BUFSIZE];

	sprintf(buf,"%s/%s",board,VF_GED);
	return(buf);
}

char *
vf_lsthdr()
{
	return(VF_LSTHDR);
}

char *
vf_pgnd()
{
	return(VF_PGND);
}

char *
vf_pkgdesc()
{
	return(VF_PKGDESC);
}

char *
vf_pkgmap()
{
	return(VF_PKGMAP);
}

char *
vf_prtmap()
{
	return(VF_PRTMAP);
}

char *
vf_sysdir()
{
	return(VF_SYSDIR);
}

char *
vf_xnet()
{
	return(VF_XNET);
}

char *
vf_xprt()
{
	return(VF_XPRT);
}
