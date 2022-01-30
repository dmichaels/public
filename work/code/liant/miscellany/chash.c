/*
/* The "perfect" hashing function
/*
/* The following code should do the trick.  If anyone is interested in
/* obtaining the perfect hash function generator program that created the
/* code below try anonymous ftp to ics.uci.edu ( 182.195.0.1 ), in the
/* ~ftp/pub directory.  There are 2 versions available there:
/*
/* perfect.tar.1.1.Z ( an older, functional, if inelegant solution written 
/*                     for GNU C )
/* gperf.tar.1.0.Z ( a far more elegant solution, written for GNU G++
/*                   1.31 or 1.32 )
/**/

/*
/* -------------------------------( reserved.c )-----------------------------
/**/

static char * wordlist [ ] =
{
         "", "", "",
         "int", 
         "goto", 
         "short", 
         "struct", 
         "",
         "register", 
         "auto", 
         "break", 
         "switch", 
         "if", 
         "for", 
         "long", 
         "float", 
         "sizeof", 
         "typedef", 
         "", "",
         "union", 
         "return", 
         "", "",
         "char", 
         "const", 
         "static", 
         "", "",
         "enum", 
         "while", 
         "",
         "do", 
         "volatile", 
         "void", 
         "",
         "signed", 
         "default", 
         "unsigned", 
         "", "", "", "", "", "", "",
         "extern", 
         "", "",
         "case", 
         "", "", "",
         "continue", 
         "else", 
         "", "", "", "", "", "",
         "double", 
};

#define MIN_WORD_SIZE 2
#define MAX_WORD_SIZE 8
#define MIN_HASH_VALUE 3
#define MAX_HASH_VALUE 61

/*
/* 32 keywords
/* 59 is the maximum key range
/**/

static
int
hash (str, len)
register char   *str;
register int    len;
{
        static int cval [ ] = {
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,  61,  61,  61,
                  61,  61,  61,  61,  61,  61,  61,   5,   5,  20,
                  30,  25,  10,   0,   5,   0,  61,   0,  10,   0,
                  15,   0,  61,  61,   0,   0,   0,   0,   0,   0,
                  61,  61,  61,  61,  61,  61,  61,  61,
        };
        return ( len + cval [ str [ 0 ] ] + cval [ str [ len - 1 ] ] );
}

int
in_word_set (str, len)
register char *str;
register int len;
{
        if ( len <= MAX_WORD_SIZE && len >= MIN_WORD_SIZE ) {
                register int key = hash ( str,len );

                if ( key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE ) {
                        register char *s = wordlist [ key ];
                        return ( *s == *str && ! strcmp ( str + 1, s + 1 ) );
                }
        }
        return ( 0 );
}

/*
/* ----------------------------( test.c )------------------------------
/**/

/*
/* Tests the generated perfect has function.
/* The -v option prints diagnostics as to whether a word is in 
/* the set or not.  Without -v the program is useful for timing.
/**/ 
  
#include <stdio.h>

#define MAX_LEN 80

int
main (argc, argv)
int	 argc;
char	*argv[];
{
   int  verbose = ( argc > 1 ) ? 1 : 0;
   char buf [ MAX_LEN ];

   while ( gets ( buf ) ) {
      if ( in_word_set ( buf, strlen ( buf ) ) && verbose ) {
         printf ( "in word set %s\n", buf );
      }
      else if ( verbose ) {
         printf ( "NOT in word set %s\n", buf );
      }
   }

   return ( 0 );
}

