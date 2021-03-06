#include "cdef.h"
#include <stdio.h>

/*
 *  settab
 *
 *  Sets the tab stops in a table which is used by fdetab(3L),
 *  fentab(3L), and is_tabpos(3L).  If a single tabstop is given
 *  (ntab == 1), then the tab stops will be set that many apart
 *  instead of the default TABSTOP.  If more than one tabstop
 *  is given, then tabs will be set at just those positions given
 *  in the list `list'.  If ntab <= 0 or list == NULL then the
 *  default tabstops will be set.  Illegal tabstop values in a
 *  list are ignored.
 */

/*
 *  `TabTab'  - The bit-mapped tab stop table.  It is one
 *		indexed for simplicity (waste first bit).
 */

#define MAXLINE	512	/* maximum line length */
#define TABSTOP	8	/* default tab stop setting */
#define BYTE_SZ	8

static char TabTab [ MAXLINE / BYTE_SZ + 1 ];

void
settab(list,ntab)
register int *list, ntab;
{
	register unsigned int i;
	register unsigned int tabstop;

	/*
	 *  Set the default
	 *  tab stops.
	 */

	if(list == NULL || ntab <= 0)
		tabstop = TABSTOP;

	/*
	 *  Set the list
	 *  of tab stops.
	 */

	if(ntab > 1 && list != NULL){
		for(i = 0 ; i < ntab ; i++)
			if(list[i] > 0 && list[i] <= MAXLINE)
				setbit(TabTab,list[i]);
		return;
	}

	/*
	 *  Set tab stops `tabstop'
	 *  spaces apart.
	 */

	if(ntab == 1 && list[0] > 0 && list[0] <= MAXLINE)
		tabstop = list[0];
	for(i = 1 ; i < MAXLINE ; i++)
		if(i % tabstop == 1)
			setbit(TabTab,i);
}

/*
 *  is_tabstop
 *
 *  Return TRUE if the given column number is a tab stop,
 *  otherwise return FALSE.  A column which extends past
 *  the last tabstop will be flaged as a tab stop (i.e.
 *  return TRUE).  A column which is less than 0 will be
 *  flagged as a non tab stop (i.e. return FALSE).
 */

bool
is_tabstop(col)
register unsigned int col;
{
	bool bit_is_set();

	if(col > MAXLINE)
		return(TRUE);
	if(col > 0)
		return(bit_is_set(TabTab,col));
	return(FALSE);
}
