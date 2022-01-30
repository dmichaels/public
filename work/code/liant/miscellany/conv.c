/* --------------------------------------------------------------------
 * Miscellaneous converstion routines
 * ------------------------------------------------------------------ */

/* --------------------------------------------------------------------
 * Include files
 * ------------------------------------------------------------------ */

#include <stdio.h>
#include <ctype.h>

/* --------------------------------------------------------------------
 * Local definitions
 * ------------------------------------------------------------------ */

#define EOS	'\0'

/* --------------------------------------------------------------------
 * str_to_oct
 *
 * If the given string "s" represents an octal number then convert it
 * to a long integer and return it in "a".  If all is well return 1,
 * otherwise return 0.
 * ------------------------------------------------------------------ */

int
str_to_oct (s,a)
register char *s;
register unsigned long *a;
{
	register char *p;
	register unsigned long m;

	if ((s == (char *)NULL) || (*s == EOS) || (*s != '0'))
		return (0);
	for (p = s ; *p != EOS ; p++)
		if ((*p < '0') || (*p > '7'))
			return (0);
	for (*a = 0, m = 1 ; --p >= s ; ) {
		*a += (ascii_to_number(*p) * m);
		m *= 8;
	}
	return (1);
}

/* --------------------------------------------------------------------
 * str_to_dec
 *
 * If the given string "s" represents a decimal number then convert it
 * to a long integer and return it in "a".  If all is well return 1,
 * otherwise return 0.
 * ------------------------------------------------------------------ */

int
str_to_dec (s,a)
register char *s;
register unsigned long *a;
{
	register char *p;
	register unsigned long m;

	if ((s == (char *)NULL) || (*s == EOS))
		return (0);
	for (p = s ; *p != EOS ; p++)
		if (!isdigit(*p))
			return (0);
	for (*a = 0, m = 1 ; --p >= s ; ) {
		*a += (ascii_to_number(*p) * m);
		m *= 10;
	}
	return (1);
}

/* --------------------------------------------------------------------
 * str_to_hex
 *
 * If the given string "s" represents a hex number then convert it
 * to a unsigned long integer and return it in "a".  If all is well
 * return 1, otherwise return 0.
 * ------------------------------------------------------------------ */

int
str_to_hex (s,a)
register char *s;
register unsigned long *a;
{
	register char *p;
	register unsigned long m, n;

	if ((s == (char *)NULL) || (*s == EOS))
		return (0);
	if (*s == '0') {
		s++;
		if ((*s == 'x') || (*s == 'X'))
			s++;
	}
	for (p = s ; *p != EOS ; p++)
		if (!isalpha(*p) && !isdigit(*p))
			return (0);
	for (*a = 0, m = 1 ; --p >= s ; ) {
		if (isdigit(*p)) n = ascii_to_number(*p);
		else if ((*p == 'a') || (*p == 'A')) n = 10;
		else if ((*p == 'b') || (*p == 'B')) n = 11;
		else if ((*p == 'c') || (*p == 'C')) n = 12;
		else if ((*p == 'd') || (*p == 'D')) n = 13;
		else if ((*p == 'e') || (*p == 'E')) n = 14;
		else if ((*p == 'f') || (*p == 'F')) n = 15;
		*a += (n * m);
		m *= 16;
	}
	return (1);
}

/* --------------------------------------------------------------------
 * str_to_addr
 *
 * Given the string "string", verify that it seems to represent a valid
 * machine address and return in "address" the long integer value of
 * that address.  If all is well return 1, otherwise return 0.
 * ------------------------------------------------------------------ */

int
str_to_addr (string)
register char *string;
register unsigned long *address;
{
	register char *p;

	if (((string != (char *)NULL) && (*string == EOS)) &&
	    (str_to_oct(string,address) ||
	     str_to_dec(string,address) ||
	     str_to_hex(string,address)))
		return (1);
	return (0);
}

/* --------------------------------------------------------------------
 * Convert the given unsigned long integer "address" to a string
 * representing the number in the given base (2 thru 16) and
 * return it in "string".
 * ------------------------------------------------------------------ */

void
addr_to_str (address, base, string)
unsigned long	address;
short	 	base;
char		*string;
{
	static char	digits[] = "0123456789ABCDEF";

	*string = EOS;
	if ((base < 2) || (base > 16))
		return;
	for (*string = EOS ; address != 0 ; address /= base)
		*string++ = digits [address % base];
	*string = EOS;
}
