/*
    C Runtime library - Copyright 1983,1984,1985,1986 by Green Hills Software Inc.
    This program is the property of Green Hills Software, Inc,
    its contents are proprietary information and no part of it
    is to be disclosed to anyone except employees of Green Hills
    Software, Inc., or as agreed in writing signed by the President
    of Green Hills Software, Inc.
*/
#include <stdio.h>

typedef struct free_hdr {
    long length;
    struct free_hdr *next ;
} FREE ;

static FREE *first = NULL;	/* head of free list */
static FREE *next = NULL;	/* and pointer to make Knuth happy */

char *malloc(siz)
unsigned siz;
{
    FREE *search;
    FREE *prev;
    FREE *temp;
    FREE *get_free();
    char *pt = NULL;
    int size;
    /* allocate size memory.  we only allocate memory in blocks of FREE's
	so size must be made a multiple of the size of a FREE, we also allocate
	an extra free so the the free routine can know how much to to free
	but this leaves us with an unused pointer, (the "next" field has no
	meaning if we aren't on the free list) so we can use this as data
	(note that we use units of FREE's rather than bytes)
    */

    if (siz <= 0)
	return(NULL);
    size = (siz + sizeof(long) + sizeof(FREE) - 1)/sizeof(FREE);
    if (next != NULL || get_free(size) != NULL) {
	prev = next;
	search = prev->next;
	while (1) {
	    if (search->length >= size) {
		if (search->length == size) {
		    if (prev == search)
			first = prev = NULL;
		    else
			prev->next = search->next;
		} else {
		    temp = search+size;
		    temp->length = search->length-size;
		    if (prev == search)
			prev = temp;
		    else
			temp->next = search->next;
		    prev->next = temp;
		    search->length = size;
		}
		next = prev;
		if (search == first && prev != NULL)
		    first = prev->next;
		pt = (char *)&search->next;  /* "next" field is start of data */
		break;
	    } else if (search == next) {
		if (get_free(size) == NULL) {
		    pt = NULL;
		    break;
		}
	    }
	    prev = search;
	    search = search->next;
	}
    }
    return(pt);
}

static FREE *get_free(size)
unsigned size;
{
    char *sbrk();
    FREE *new;

    size = (size * sizeof(FREE) + (BLOCKSIZE-1))/BLOCKSIZE*BLOCKSIZE;
    if ((new = (FREE *)sbrk(size)) == NULL || (int)new == -1)
	return(NULL);
    new->length = size/sizeof(FREE);
    free(((char *)new) + sizeof(long));
    return(new);
}

free(pt)
char *pt;
{
    FREE *prev;
    FREE *item = (FREE *)(pt - sizeof(long));

    if (pt == NULL) {
    } else if (first == NULL) {
	first = next = item->next = item;
    } else {
	/*item<=first: prev=next; while (prev->next!=first) prev=prev->next; */
	prev = (item >= next || item <= first)? next:first;
	while (prev->next != first && !(prev <= item && prev->next > item))
	    prev = prev->next;
	if (prev == item || item == first)
	    eprintf( "Already free %x\r\n", item );
	else if (prev->next == prev) {
	    if (item+item->length == prev) {
		item->length += prev->length;
		item->next = item;
		first = next = item;
	    } else if (prev+prev->length == item)
		prev->length += item->length;
	    else {
		prev->next = item;
		item->next = prev;
		if (item < prev)
		    first = item;  /* WHY NOT next = first */
	    }
	} else {
	    if (item+item->length == prev->next) {
		item->length += prev->next->length;
		item->next = prev->next->next;
	    } else if (item+item->length < prev->next || item > prev->next)
		/* item > prev->next implies prev->next = first && item<first*/
		item->next = prev->next;
	    else {
		eprintf("Free list corrupted\r\n");
		return;
	    }
	    if (prev+prev->length == item) {
		prev->length += item->length;
		prev->next = item->next;
	    } else if (prev+prev->length < item || prev > item)
		/* prev > item implies prev->next == first  && item < first*/
		prev->next = item;
	    else {
		eprintf("Free list corrupted\r\n");
		return;
	    }
	    next = prev;
	    if (prev < first)  /* THIS IS IMPOSSIBLE, prev is in list!*/
		first = prev;
	    else if (prev->next < first)  /*only if prev->next == item!*/
		first = prev->next;
	}
    }
}

char *realloc(old, new_size)
char *old;
unsigned new_size;
{
    char *pt;

    if (old == NULL)
	return(malloc(new_size));
    if ((pt = malloc(new_size)) != NULL) {
	bufcpy(pt, old, new_size);
	cfree(old);
    }
    return(pt);
}
