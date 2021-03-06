/***********************************************************************
* PROGRAM:	VALID Graphics Editor Layout Program
* FILE:		xprt_parse.c
* AUTHOR:	David Michaels (david@l-sst)
* DATE:		December 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>
#include <ctype.h>

/*
 *  xprt_parse
 *
 *  Read the the VALID expanded parts list and fill in the
 *  following the reference designator `layout->l_dev->d_refdes'
 *  and the device name `layout->l_dev->d_name'.
 *
 *  VALID Expanded Parts List (partial) format:
 *  ----------------------------------------------------------
 *  PART_NAME
 *  <reference_designator> <part_name> : ;
 *  <logical_part_properties>
 *  ----------------------------------------------------------
 *  We are interested only in the <reference_designator>, and
 *  the <part_name>.  The file is free format (i.e. white space
 *  may occur between any two tokens.  The keyword "PART_NAME"
 *  is used to mark the beginning of a part entry.  The reference
 *  designator is a string of letters and digits.  The part name
 *  is a single quoted string of letters.  Below is a little example:
 *  ----------------------------------------------------------
 *  		PART_NAME
 *  		U32 'LS04':;
 *  ----------------------------------------------------------
 *  Here we would want the part name (i.e. LS04) and reference
 *  designator (i.e. U32).  Return the number of designators.
 */

#define XPRT_BUFSIZE	512

#define COLON		':'
#define SEMICOLON	';'
#define SQUOTE		'\''

int
xprt_parse(f,layout)
register FILE *f;
register LAYOUT *layout;
{
	DEVICE		*add_dev();
	int		rmchar();
	long		fseek_pat();
	char		*cindex();
	void		error();
	static char	spaces[] = " \t\n";
	static char	nospaces[] = "^ \t\n";
	char		buf[XPRT_BUFSIZE], errbuf[ERR_BUFSIZE];
	register char	*refdes, *prtname;
	register char	*endrefdes, *endprtname;
	char		*colon;
	unsigned int	record = 0;

ReadPart:
	while(fseek_pat(f,"PART_NAME") != -1) {
		record++;
		if(getstring(f,"^;",buf,XPRT_BUFSIZE) == NULL)
			break;
		if(getc(f) != SEMICOLON) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: delim ';' not found",
			record);
			error(errbuf);
			continue;
		}
		if((refdes = cindex(buf,nospaces,1)) == NULL) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: null entry",record);
			error(errbuf);
			continue;
		}
		if((endrefdes = cindex(refdes,spaces,1)) == NULL) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: no part name",record);
			error(errbuf);
			continue;
		}
		if((prtname = cindex(endrefdes,nospaces,1)) == NULL) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: no part name",record);
			error(errbuf);
			continue;
		}
		if(*prtname != SQUOTE) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: start delim (') misssing",
			record);
			error(errbuf);
			continue;
		}
		if((endprtname = cindex(++prtname,"'",1)) == NULL) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: end delim (') missing",
			record);
			error(errbuf);
			continue;
		}
		if(*(colon = cindex(endprtname+1,nospaces,1)) != COLON) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: delim (:) missing",
			record);
			error(errbuf);
			continue;
		}
		if(cindex(colon+1,nospaces,1) != NULL) {
			sprintf(errbuf,
			"XPRT: PART_NAME rec %u: illegal chars after delim (:)",
			record);
			error(errbuf);
			continue;
		}

		*endrefdes = EOS;
		*endprtname = EOS;

		for(endrefdes = refdes ; *endrefdes != EOS ; endrefdes++)
			if(!isalnum(*endrefdes)) {
				sprintf(errbuf,
				"XPRT: illegal refdes name \"%s\"",refdes);
				error(errbuf);
				goto ReadPart;
			}
		for(endprtname = prtname ; *endprtname != EOS ; endprtname++)
			if(!isalnum(*endprtname)) {
				sprintf(errbuf,
				"XPRT: illegal part name \"%s\"",prtname);
				error(errbuf);
				goto ReadPart;
			}
		if(endrefdes - refdes > REFLEN) {
			sprintf(errbuf,
			"XPRT: refdes name \"%s\" too long; truncating",refdes);
			refdes[REFLEN] = EOS;
		}
		if(endprtname - prtname > REFLEN) {
			sprintf(errbuf,
			"XPRT: part name \"%s\" too long; truncating",refdes);
			prtname[REFLEN] = EOS;
		}
		add_dev(refdes,prtname,layout);
	}
	return(layout->l_ndev);
}
