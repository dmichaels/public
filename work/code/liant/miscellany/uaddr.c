#include <stdio.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/user.h>

main ()
{
	unsigned long uregoffset;

	uregoffset = (unsigned long)(&(((struct user *)0L)->u_ar0));
	printf ("\nOffset to \"u_ar0\" in \"struct user\" is: ");
	printf ("0x%lx = 0%lo = %lu bytes\n\n",
		uregoffset, uregoffset, uregoffset);
	exit (0);
}
