/**********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		map_parse.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		May 1984
**********************************************************************/

#include "cdef.h"
#include "main.h"
#include "valid.h"
#include <stdio.h>

/*
 *  map_parse
 *
 *  Go through each device in the DEVICE list, find the entry in
 *  the vf_prtmap for its device name, and make an entry in the
 *  PACKAGE table for its corresponding package name, set
 *  the DEVICE list field `d_pkg' to point to this entry.
 *
 *  Map_file Format:
 *  --------------------------------------------------------
 *  <device_name e.g. LS04>	<device_package e.g. DIP14>
 *  ...				...
 *  ...				...
 *  --------------------------------------------------------
 *  Each entry must end witha newline.  To accommodate
 *  comments, if a '#' appears on a line, it and the whole
 *  rest of the line will be ignored.
 */

map_parse(f,layout)
register FILE *f;
register LAYOUT *layout;
{
	PACKAGE		*add_pkg();
	char		*getentry();
	register DEVICE	*dp;
	register char	pkgname[PKGLEN+1];
	char		errbuf[ERR_BUFSIZE];

	for(dp = layout->l_dev ; dp != NULL ; dp = dp->d_next, rewind(f)) {
		if(getentry(f,dp->d_name,pkgname) == NULL) {
			sprintf(errbuf,
			"No %s entry in \"%s\"",dp->d_name,vf_prtmap());
			dp->d_pkg = NULL;
			error(errbuf);
			continue;
		}
		dp->d_pkg = add_pkg(pkgname,layout);
	}
}

/*
 *  getentry	(local)
 *
 *  Find the entry in the vf_prtmap for the given device (part name)
 *  `dev', and fill in the given package name string `pkg'.  If the
 *  device entry is not found, then return NULL.
 *
 *  Be very careful of lengths of things so as not to blow up; we never
 *  know what kind of garbage may be sitting in those VALID files.
 */

#define NARGS	2
#define SPLAT	'#'

static char *
getentry(f,dev,pkg)
register FILE *f;
register char *dev, *pkg;
{
	char		*findline();
	unsigned int	wordvec();
	register char	line[STR_BUFSIZE], *args[NARGS];
	char		errbuf[ERR_BUFSIZE];
	unsigned int	len;

	while(findline(dev,line,STR_BUFSIZE,f) != NULL) {
		if(wordvec(line,args,NARGS," \t\n") != NARGS)
			continue;
		if(**args == SPLAT)
			continue;
		if(!str_eq(dev,*args))
			continue;
		if((len = strlen(args[1]) > PKGLEN)) {
			sprintf(errbuf,
			"Package name for \"%s\" too long in \"%s\"",
			dev,vf_prtmap());
			error(errbuf);
			return(NULL);
		}
		strcpy(pkg,args[1]);
		return(pkg);
	}
	return(NULL);
}
