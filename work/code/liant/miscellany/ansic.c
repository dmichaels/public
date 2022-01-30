
main()
{

/* ------------------------------------------------------------------- */

	{

#	define s(a)	"a"
	if (s(x) [0] == 'x') {
		printf ("*** Not ANSI-C!");
		printf ("  Formal macro parameter substitution");
		printf (" within a string literal.\n");
	}
#	undef s

	}

/* ------------------------------------------------------------------- */

	{

#	define a
#	define x	a/**/b
	int b = 1, ab = 2;
	if (x == 2) {
		printf ("*** Not ANSI-C!");
		printf ("  Comments do not necessarily");
		printf (" serve to separate tokens.\n");
	}
#	undef a
#	undef x

	}

/* ------------------------------------------------------------------- */

	{

#	define xplus	+x
	int x = 1;
	x = +xplus;
	if (x == 2) {
		printf ("*** Not ANSI-C!");
		printf ("  Preprocessing is not token based.\n");
	}
#	undef xplus

	}

/* ------------------------------------------------------------------- */

	{

#	define f
#	define abc de\
f
	int de = 1, def = 2;
	if (abc == 1) {
		printf ("*** Not ANSI-C!");
		printf ("  Backslash/newline doesn't continue");
		printf (" a preprocessing directive.\n");
	}

	}

/* ------------------------------------------------------------------- */

	{

	unsigned short us = 0xFFFF; 

	if (((us << 16) / 2) >= 0) {
		printf ("*** Not ANSI-C!");
		printf ("  Unsigned preserving");
		printf (" rather than value preserving conversions.\n");
	}

	}

/* ------------------------------------------------------------------- */

	{

	struct A { int x; int y; } a;
	a.x = 1;
	a.y = 2;
	{
		struct A;
		struct B { struct A *p; } bb;
		struct A { int y; int x; } aa;
		aa.x = 1;
		aa.y = 2;
		bb.p = &aa;
		if (!((bb.p->x == 1) && (bb.p->y == 2))) {
			printf ("*** Not ANSI-C!");
			printf ("  Ignored empty struct/union declaration.\n");
		}
	}

	}

/* ------------------------------------------------------------------- */

}
