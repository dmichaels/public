/* bsplit (byte split) program-
splits a file into n-byte chunks

format:

split [ -size file-size ] filename
or    [ -s file-size ] filename
*/

#define FALSE 0
#define TRUE  1
#define NOT(x)  (!x)

#include <stdio.h>


main(argc,argv)
     int argc;
     char *argv[];
{
  char *filename = NULL, splitfn[10];
  int i, sflag = FALSE, filenum, c;
  long l, filesize = 0L, atol();
  FILE *fopen(), *infile, *splitfile;

  for(i=1; i<argc; i++) {
    if (sflag) {		/* previous s or size option... */
      sflag = FALSE;

      if (filesize) {		/* duplicate parameter */
	fprintf(stderr,"%s:  file size specified more than once.\n",argv[0]);
	exit(1);
      }

      filesize = atol(argv[i]);

      if (filesize <= 0) {
	fprintf(stderr,"%s:  Illegal size (%ld)\n",argv[0],filesize);
	exit(1);
      }
      
      /* check to see if we have a suffix */
      switch(argv[i][strlen(argv[i])-1]) { /* check last char of this arg */
      case 'm':
      case 'M':
	filesize *= 1024000L;
	break;

      case 'k':
      case 'K':
	filesize *= 1024L;
	break;

      case 'b':
      case 'B':
	filesize *= 512L;
	break;

      case 'w':
      case 'W':
	filesize *= 2L;
	break;
      }
    }
    else {
      if (!strcmp(argv[i],"-size")
	  || !strcmp(argv[i],"-s")) {
	  sflag = TRUE;
	}
      else {
	filename = argv[i];
      }
    }
  }
  

  if (!filesize) {
    fprintf(stderr,"%s:  no file size specified\n",argv[0]);
    fprintf(stderr,"Usage:   %s -s[ize] <file size> <filename>\n\n",argv[0]);
    fprintf(stderr,"You may specify the file size in decimal, hex, or\n");
    fprintf(stderr,"octal.   Adding W, B, K, or M to the number specifies\n");
    fprintf(stderr,"words (2 bytes), blocks (512 bytes), kilobytes, or\n");
    fprintf(stderr,"megabytes, respectively.\n");
    exit(1);
  }

  if (!filename) {
    fprintf(stderr,"%s:  no file name specified\n",argv[0]);
    exit(1);
  }


  if ((infile=fopen(filename,"r")) == NULL) {
    fprintf(stderr,"%s:  can't open input file\n",argv[0]);
    perror(filename);
    exit(1);
  }

  filenum = 0;
  do {
    sprintf(splitfn,"%.4s%05d",filename,filenum); /* create uniq filename */

    if ((splitfile=fopen(splitfn,"w")) == NULL) {
      fprintf(stderr,"%s:  can't open output file\n",argv[0]);
      perror(splitfn);
      exit(1);
    }
    
    for(l=1L; l <= filesize && ((c=getc(infile)) != EOF); l++)
      putc(c,splitfile);

    fclose(splitfile);
    filenum++;
  }
  while (c != EOF);
}
  
