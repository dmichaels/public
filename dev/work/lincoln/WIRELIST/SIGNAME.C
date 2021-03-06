/***********************************************************************
* PROGRAM:      VALID wirelist program
* FILE:         signame.c
* AUTHOR:       David Michaels (david@ll-sst)
* DATE:         April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern BOARD    *Bgeom;

/*
 *  sig_name
 *
 *  Transform the given VALID net name to a unique signal name
 *  to be put on the wirelist; it must be of length SIGLEN or less.
 *  All unnamed signals will be transformed into a normal form
 *  (ex. 'UN$1$"LS04"$34P$Q$187'<0> to 1$34PQ1870).  The characters
 *  in `badchars` will be removed from all signal names.  After
 *  removing bad characters and making (unnamed) transformations,
 *  if a signal name is still to long, it will be replaced by a
 *  (hopefully) unique signal name of the form _UN_number, where
 *  `number` is a unique integer.   Return the transformed string.
 *  The given net name is not changed.
 */

char *
sig_name(netname)
register char *netname;
{
        char *index(), *cindex(), *sindex();
        int rmchar();
        static char badchars[] = "<>'\" \t\n";
        static unsigned int uniqnum = 0;
        static char newname[SIGLEN+1];
        register char *p, str[STR_BUFSIZE];

        strcpy(str,netname);
        rmchar(str,badchars,0);
        /* unnamed */
        if(strn_eq(str,"UN",2) || strn_eq(str,"-UN",3)) {
                rmchar(sindex(str,"UN",0),"UN",2);
                rmchar(str,"$",1);
                p = cindex(str,"$",2);
                *(index(str,'$')) = EOS;
                strcat(str,p);
                rmchar(index(str,'$')+1,"$",0);
        }
        /* still too long! */
        if(strlen(str) > SIGLEN)
                sprintf(newname,"_UN_%d",uniqnum++);
        else
                strcpy(newname,str);
        newname[SIGLEN] = EOS;
        return(newname);
}

/*
 *  cnn_gnd_sig
 *
 *  Return a pointer to a string containing a final wirelist
 *  signal name for the given ground connector pin.  Note that
 *  the returned pointer refers to static storage space whose
 *  content is overwritten by each call.
 */

char *
cnn_gnd_sig(cnn,pin)
register CNN *cnn;
register int pin;
{
        static  char    s[LSTLEN+1];

        sprintf(s,"%s%s",GND_SIG,(Bgeom->b_cnngnd)(cnn,pin));
        return(s);
}

/*
 *  cnn_vcc_sig
 *
 *  Return a pointer to a string containing a final wirelist
 *  signal name for the given voltage connector pin.  Note that
 *  the returned pointer refers to static storage space whose
 *  content is overwritten by each call.
 */

char *
cnn_vcc_sig(cnn,pin)
register CNN *cnn;
register int pin;
{
        static  char            s[LSTLEN+1];

        sprintf(s,"%s%s",VCC_SIG,(Bgeom->b_cnnvcc)(cnn,pin));
        return(s);
}

/*
 *  gnd_sig_name
 *
 *  Create a signal name for the given ground pin list consisting of
 *  the string GND_SIG followed by the location (in proper board
 *  geometry form) on the board of the ground pin nearest to the
 *  first pin on the list.  This may want to be changed in the future,
 *  to (for example) choose the ground pin nearest the highest numbered
 *  pin on the list.  The given pin is assumed to belong to a device
 *  not a connector.  Note that the returned pointer refers to static
 *  storage space whose content is overwritten by each call.
 *
 *  given:  <ground pin list>
 *  return: GND<location of the gnd pin nearest the 1st pin on the list>
 */

char *
gnd_sig_name(p)
register PIN *p;
{
        static char s[LSTLEN+1];
        register unsigned int chosenpin;

        chosenpin = p->p_pin;   /* 1st on the list */

        sprintf(s,"%s%s",GND_SIG,
                (Bgeom->b_neargnd)(dev_xpin(p->p_owner.dev,chosenpin),
                dev_ypin(p->p_owner.dev,chosenpin)));
        return(s);
}

/*
 *  vcc_sig_name
 *
 *  Create a signal name for the given voltage pin list consisting of
 *  the string VCC_SIG followed by the location (in proper board
 *  geometry form) on the board of the voltage pin nearest to the
 *  first pin on the list.  This may want to be changed in the future,
 *  to (for example) choose the voltage pin nearest the highest numbered
 *  pin on the list.  Note that the returned pointer refers to static
 *  storage space whose content is overwritten by each call.
 *
 *  given:  <voltage pin list>
 *  return: VCC<location of the vcc pin nearest the 1st pin on the list>
 */

char *
vcc_sig_name(p)
register PIN *p;
{
        static char s[LSTLEN+1];
        register unsigned int chosenpin;

        chosenpin = p->p_pin;   /* 1st on the list */

        sprintf(s,"%s%s",VCC_SIG,
                (Bgeom->b_nearvcc)(dev_xpin(p->p_owner.dev,chosenpin),
                dev_ypin(p->p_owner.dev,chosenpin)));
        return(s);
}

/*
 *  gnd_loc
 *
 *  Return in board geometry form, the location of the ground
 *  pin on the board to which this ground net will be wired to.
 *
 *  The net is assumed to be a ground net (i.e. its signal is
 *  a ground signal), and it is therefore the callers responsiblity
 *  to make sure that it is.
 *
 *  Implementation:
 *  Since we know that the desired location is contained within the
 *  signal name as GND<location> (because the signal name was created
 *  using the above "gnd_sig_name" function), just extract and return it. 
 */

char *
gnd_loc(net)
register NET *net;
{
        char *sindex();
        register char *s;

        s = sindex(net->n_sig,GND_SIG,1);
        return(s == NULL ? NULL : s + 1);
}

/*
 *  vcc_loc
 *
 *  Return in board geometry form, the location of the ground
 *  pin on the board to which this voltage net will be wired to.
 *
 *  The net is assumed to be a voltage net (i.e. its signal is
 *  a voltage signal), and it is therefore the callers responsiblity
 *  to make sure that it is.
 *
 *  Implementation:
 *  Since we know that the desired location is contained within the
 *  signal name as VCC<location> (because the signal name was created
 *  using the above "vcc_sig_name" function), just extract and return it. 
 */

char *
vcc_loc(net)
register NET *net;
{
        char *sindex();
        register char *s;

        s = sindex(net->n_sig,VCC_SIG,1);
        return(s == NULL ? NULL : s + 1);
}
