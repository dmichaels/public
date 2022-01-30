/* blkcpy.c */

#define SHORT_ALIGNMENT(x)	((long)(x) & 01L)
#define LONG_MISALIGNED(x)	(((long)(x) & 03L) != 0L)

void
blkcpy (destination, source, count)
register char *destination;
register char *source;
register long  count;
{
	if ((count >= sizeof(long)) &&
	    (SHORT_ALIGNMENT(destination) == SHORT_ALIGNMENT(source))) {
		for ( ; LONG_MISALIGNED(destination) ; count--)
			*destination++ = *source++;
		for ( ; count >= 4 ; count -= 4)
			*((long *)destination)++ = *((long *)source)++;
	}
	while (count-- > 0L)
		*destination++ = *source++;
}

