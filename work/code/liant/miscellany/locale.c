From uunet!sequoia.com!burn Tue Oct 15 12:31:15 1991
Date: Tue, 15 Oct 91 11:12:22 EDT
From: uunet!sequoia.com!burn (burn)
To: david@lpi.liant.com
Subject: locale


/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  _locale.c, Version 6.500
|  Latest Change on 4/4/91 at 16:19:36
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:gen/_locale.c	1.5"
#include "synonyms.h"
#include <locale.h>
#include "_locale.h"
#include <string.h>
#include <stdlib.h>

/* return value for category with "" locale */
char *
_nativeloc(cat)
int cat;
{
	static const char lang[] = "LANG";
	static const char * const _loc_envs[LC_ALL][4] = /* env. vars for "" */
	{
		{"LC_CTYPE",	lang, 	"CHRCLASS",	0},
		{"LC_NUMERIC",	lang,	0},
		{"LC_TIME",	lang,	"LANGUAGE",	0},
		{"LC_COLLATE",	lang,	0},
		{"LC_MONETARY",	lang,	0},
		{"LC_MESSAGES",	lang,	0},
	};
	static char ans[LC_NAMELEN];
	register char *s;
	register char const * const *p;

	for (p = _loc_envs[cat]; *p != 0; p++)
		if ((s = getenv(*p)) != 0 && s[0] != '\0')
			goto found;
	return "C";
found:
	(void)strncpy(ans, s, LC_NAMELEN - 1);
	ans[LC_NAMELEN - 1] = '\0';
	return ans;
}

char *
_fullocale(loc, file)	/* "/usr/lib/locale/<loc>/<file>" */
const char *loc, *file;
{
	static char ans[18 + 2 * LC_NAMELEN] = "/usr/lib/locale/";
	register char *p = ans + 16;

	(void)strcpy(p, loc);
	p += strlen(loc);
	p[0] = '/';
	(void)strcpy(p + 1, file);
	return ans;
}

/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  localeconv.c, Version 6.500
|  Latest Change on 4/4/91 at 16:22:10
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:gen/localeconv.c	1.7"
#include "synonyms.h"
#include "shlib.h"
#include <locale.h>
#include "_locale.h"
#include <stdio.h>
#include <limits.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

static struct lconv	lformat = {
	".",	/* decimal_point */
	"",	/* thousands grouping */
	"",	/* grouping */
	"",	/* int_curr_symbol */
	"",	/* currency symbol */
	"",	/* mon_decimal_point */
	"",	/* mon_thousands_sep */
	"",	/* mon_grouping */
	"",	/* positive sign */
	"",	/* negative sign */
	CHAR_MAX,	/* int_frac_digits */
	CHAR_MAX,	/* frac_digits */
	CHAR_MAX,	/* p_cs_precedes */
	CHAR_MAX,	/* p_sep_by_space */
	CHAR_MAX,	/* n_scs_precedes */
	CHAR_MAX,	/* n_sep_by_space */
	CHAR_MAX,	/* p_sign_posn */
	CHAR_MAX,	/* n_sign_posn */
};

static char	sv_lc_numeric[LC_NAMELEN] = "C";
static char	sv_lc_monetary[LC_NAMELEN] = "C";


struct lconv *
localeconv()
{
	int	fd;
	VOID	*str;
	struct stat	buf;
	static char	*ostr = NULL; /* pointer to last set locale for LC_MONETARY */
	static char	*onstr = NULL; /* pointer to last set locale for LC_NUMERIC */
	struct lconv	*monetary;

	if (strcmp(_cur_locale[LC_NUMERIC], sv_lc_numeric) != 0) {
		lformat.decimal_point[0] = _numeric[0];
		lformat.thousands_sep[0] = _numeric[1];
		if ((fd = open(_fullocale(_cur_locale[LC_NUMERIC],"LC_NUMERIC"), O_RDONLY)) == -1)
			goto err4;
		if ((fstat(fd, &buf)) != 0 || (str = malloc(buf.st_size)) == NULL)
			goto err5;
		if (buf.st_size > 2) {
			if ((read(fd, str, buf.st_size)) != buf.st_size)
				goto err6;

			/* if a previous locale was set for LC_NUMERIC, free it */
			if (onstr != NULL)
				free(onstr);
			onstr = str;

			lformat.grouping = (char *)str + 2;
		} else
			lformat.grouping = "";
		close(fd);
		strcpy(sv_lc_numeric, _cur_locale[LC_NUMERIC]);
	}

	if (strcmp(_cur_locale[LC_MONETARY], sv_lc_monetary) == 0)
		return(&lformat);

	if ((fd = open(_fullocale(_cur_locale[LC_MONETARY],"LC_MONETARY"), O_RDONLY)) == -1)
		goto err1;
	if ((fstat(fd, &buf)) != 0 || (str = malloc(buf.st_size + 2)) == NULL)
		goto err2;
	if ((read(fd, str, buf.st_size)) != buf.st_size)
		goto err3;
	close(fd);

	/* if a previous locale was set for LC_MONETARY, free it */
	if (ostr != NULL)
		free(ostr);
	ostr = str;

	monetary = (struct lconv *)str;
	str = (char *)str + sizeof(struct lconv);
	lformat.int_curr_symbol = (char *)str + (int)monetary->int_curr_symbol;
	lformat.currency_symbol = (char *)str + (int)monetary->currency_symbol;
	lformat.mon_decimal_point = (char *)str + (int)monetary->mon_decimal_point;
	lformat.mon_thousands_sep = (char *)str + (int)monetary->mon_thousands_sep;
	lformat.mon_grouping = (char *)str + (int)monetary->mon_grouping;
	lformat.positive_sign = (char *)str + (int)monetary->positive_sign;
	lformat.negative_sign = (char *)str + (int)monetary->negative_sign;
	lformat.int_frac_digits = monetary->int_frac_digits;
	lformat.frac_digits = monetary->frac_digits;
	lformat.p_cs_precedes = monetary->p_cs_precedes;
	lformat.p_sep_by_space = monetary->p_sep_by_space;
	lformat.n_cs_precedes = monetary->n_cs_precedes;
	lformat.n_sep_by_space = monetary->n_sep_by_space;
	lformat.p_sign_posn = monetary->p_sign_posn;
	lformat.n_sign_posn = monetary->n_sign_posn;

	strcpy(sv_lc_monetary, _cur_locale[LC_MONETARY]);
	return(&lformat);

err3:	free(str);
err2:	close(fd);
err1:	strcpy(_cur_locale[LC_MONETARY], sv_lc_monetary);
	return(&lformat);

err6:	free(str);
err5:	close(fd);
err4:	strcpy(_cur_locale[LC_NUMERIC], sv_lc_numeric);
	return(&lformat);
}

/* Copyright (c) 1990, Sequoia Systems Inc., Marlboro, MA. */
/********************************************************************
|  setlocale.c, Version 6.500
|  Latest Change on 4/4/91 at 16:23:25
|
*********************************************************************/
/* Modification History
mm/dd/yy name		change

End-of-modification-history */


/*	Copyright (c) 1988 AT&T	*/
/*	  All Rights Reserved  	*/

/*	THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF AT&T	*/
/*	The copyright notice above does not evidence any   	*/
/*	actual or intended publication of such source code.	*/

#ident	"@(#)libc-port:gen/setlocale.c	1.9"
/*
* setlocale - set and query function for all or parts of a program's locale.
*/
#include "synonyms.h"
#include "shlib.h"
#include <locale.h>
#include "_locale.h"	/* internal to libc locale data structures */
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>

static char *set_cat();

char *
setlocale(cat, loc)
int cat;
const char *loc;
{
	char part[LC_NAMELEN];

	if (loc == 0)	/* query */
	{

#if DSHLIB
		static char *ans;

		if (!ans && (ans = malloc(LC_ALL * LC_NAMELEN + 1)) == 0)
			return 0;
#else
		static char ans[LC_ALL * LC_NAMELEN + 1];
#endif

		if (cat != LC_ALL)
			(void)strcpy(ans, _cur_locale[cat]);
		else
		{
			register char *p, *q;
			register int flag = 0;
			register int i;

			/*
			* Generate composite locale description.
			*/
			p = ans;
			for (i = LC_CTYPE; i < LC_ALL; i++)
			{
				*p++ = '/';
				q = _cur_locale[i];
				(void)strcpy(p, q);
				p += strlen(q);
				if (!flag && i > LC_CTYPE)
					flag = strcmp(q, _cur_locale[i - 1]);
			}
			if (!flag)
				return q;
		}
		return ans;
	}
	/*
	* Handle LC_ALL setting specially.
	*/
	if (cat == LC_ALL)
	{
		static int reset = 0;
		register const char *p;
		register int i;
		register char *sv_loc;

		if (!reset)
			sv_loc = setlocale(LC_ALL, NULL);
		cat = LC_CTYPE;
		if ((p = loc)[0] != '/')	/* simple locale */
		{
			loc = strncpy(part, p, LC_NAMELEN - 1);
			part[LC_NAMELEN - 1] = '\0';
		}
		do	/* for each category other than LC_ALL */
		{
			if (p[0] == '/')	/* piece of composite locale */
			{
				i = strcspn(++p, "/");
				(void)strncpy(part, p, i);
				part[i] = '\0';
				p += i;
			}
			if (set_cat(cat++, part) == 0) {
				reset = 1;
				setlocale(LC_ALL, sv_loc);
				reset = 0;
				return 0;
			}
		} while (cat < LC_ALL);
		return setlocale(LC_ALL, NULL);
	}
	return(set_cat(cat, loc));
}

static char *
set_cat(cat, loc)
int cat;
const char *loc;
{
	char part[LC_NAMELEN];

	/*
	* Set single category's locale.  By default,
	* just note the new name and handle it later.
	* For LC_CTYPE and LC_NUMERIC, fill in their
	* tables now.
	*/
	if (loc[0] == '\0')
		loc = _nativeloc(cat);
	else
	{
		loc = strncpy(part, loc, LC_NAMELEN - 1);
		part[LC_NAMELEN - 1] = '\0';
	}
	if (cat <= LC_NUMERIC)
	{
		if (strcmp(loc, _cur_locale[cat]) != 0 && _set_tab(loc, cat) != 0)
			return 0;
	}
	else {
		int fd;
		static const char *name[LC_ALL] = 
			{ "LC_CTYPE", 
			  "LC_NUMERIC",
			  "LC_TIME",
			  "LC_COLLATE",
			  "LC_MONETARY",
			  "LC_MESSAGES"
 			};
		if (strcmp(loc, _cur_locale[cat]) != 0) {
			if ((fd = open(_fullocale(loc, name[cat]), O_RDONLY)) == -1)
				return 0;
			(void)close(fd);
		}
	}
	return strcpy(_cur_locale[cat], loc);
}


