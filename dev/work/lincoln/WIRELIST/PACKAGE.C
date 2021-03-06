/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		package.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

extern FILE	*Errfp;

/*
 *  pkg_desc
 *
 *  See if the package description entry for package `pkgname'
 *  is in the wirelist package table, if so return a pointer
 *  to it, otherwise allocate a new package table structure,
 *  read the package's description file, and fill in the
 *  appropriate information.  Return a pointer to the found
 *  or newly added package table or NULL if an error occurred.
 *
 *  Package description file information format:
 *  -------------------------------------------------------
 *  TYPE	<DIP, SIP, or BABY>
 *  NPINS	<number of pins>
 *  PIN1	<relative x offset>	<relative y offset>
 *  PIN2	<relative x offset>	<relative y offset>
 *  ...			...			...
 *  ...			...			...
 *  ...			...			...
 *  PINn	<relative x offset>	<relative y offset>
 *  -------------------------------------------------------
 */

#define NWORDS	3	/* max number of words per line */

PACKAGE *
pkg_desc(pkgname,w)
char *pkgname;
WIRELIST *w;
{

	void			fatal();
	PACKAGE			*find_pkg();
	char			*descfile, *vf_pkgfile(), *index();
	int			wordvec();
	char			line[STR_BUFSIZE], *wv[NWORDS], *p;
	int			wc, pinno, xoff, yoff;
	register PACKAGE	*pkg;
	unsigned int		npkg = w->w_npkg;
	FILE			*f;
	bool			got_npins = FALSE;
	bool			got_type = FALSE;

	if((pkg = find_pkg(pkgname,w)) != NULL)
		return(pkg);
	else if(w->w_npkg == MAXPKG - 1) {
		fprintf(Errfp,
		"Too many packages (increase MAXPKG (%u))\n",MAXPKG);
		fclose(f);
		return(NULL);
	}
	else	/* alloc new PACKAGE table entry */
		pkg = w->w_pkg + npkg;

	if((descfile = vf_pkgfile(pkgname)) == NULL) {
		fprintf(Errfp,"Can't access \"%s\" description file\n",pkgname);
		return(NULL);
	}
	if((f = fopen(descfile,"r")) == NULL) {
		fprintf(Errfp,"Can't open \"%s\" (%s description)\n",
		descfile, pkgname);
		return(NULL);
	}
	while(fgets(line,STR_BUFSIZE,f) != NULL) {
	   if((p = index(line,'#')) != NULL)
		*p = EOS;
	   if((wc = wordvec(line,wv,NWORDS," \t\n")) <= 0)
		continue;
	   if(str_eq("TYPE",wv[0])) {
		if(wc < 2){
			fprintf(Errfp,
			"Bad TYPE entry in \"%s\" (%s description)\n",
			descfile,pkgname);
			fclose(f);
			return(NULL);
		}
		if(str_eq(wv[1],"DIP"))
			pkg->pk_type = IS_DIP;
		else if(str_eq(wv[1],"SIP"))
			pkg->pk_type = IS_SIP;
		else if(str_eq(wv[1],"BABY"))
			pkg->pk_type = IS_BABY;
		else {
			fprintf(Errfp,
			"Unknown TYPE in \"%s\" (%s description)\n",
			descfile,pkgname);
			fclose(f);
			return(NULL);
		}
		got_type = TRUE;
	   }
	   else if(str_eq("NPINS",wv[0])) {
		if(wc < 2 || (pkg->pk_npins = atoi(wv[1])) <= 0) {
			fprintf(Errfp,"Bad NPINS in \"%s\" (%s description)\n",
			descfile,pkgname);
			fclose(f);
			return(NULL);
		}
		if((pkg->pk_pin = (PINOFF *)
	    	malloc(pkg->pk_npins * sizeof(PINOFF))) == NULL)
			fatal(ENOMEM,"package pin list");
		pinno = 0;
		got_npins = TRUE;
	   }
	   else if(strn_eq("PIN",wv[0],3)) {
		if(!got_npins || atoi(wv[0]+3) != ++pinno ||
		   pinno > pkg->pk_npins || wc < 3 ||
		   (xoff = atoi(wv[1])) < 0 || (yoff = atoi(wv[2])) < 0) {
			fprintf(Errfp,"Bad PIN in \"%s\" (%s description)\n",
			descfile,pkgname);
			fclose(f);
			return(NULL);
		}
		pkg->pk_pin[pinno-1].po_xoff = xoff;
		pkg->pk_pin[pinno-1].po_yoff = yoff;
	   }
	}
	if(!got_type || !got_npins || pinno != pkg->pk_npins) {
		fprintf(Errfp,"Bad format in \"%s\" (%s description)\n",
		descfile,pkgname);
		fclose(f);
		return(NULL);
	}
	fclose(f);
	strncpy(pkg->pk_name,pkgname,PKGLEN);
	pkg->pk_name[PKGLEN] = EOS;
	w->w_npkg++;
	return(pkg);
}

/*
 *  find_pkg	(local)
 *
 *  Return a pointer to the package table entry (pkg_tbl)
 *  with the given package name `pkgname'.  NULL if not found.
 */

PACKAGE *
find_pkg(pkgname,w)
char *pkgname;
register WIRELIST *w;
{
	register PACKAGE *pkg = w->w_pkg;
	register unsigned int i, npkg = w->w_npkg;

	for(i = 0 ; i < npkg ; i++)
		if(str_eq(pkgname,pkg[i].pk_name))
			return(pkg + i);
	return(NULL);
}
