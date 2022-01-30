/***********************************************************************
* PROGRAM:	VALID wirelist program
* FILE:		netpin.c
* AUTHOR:	David Michaels (david@ll-sst)
* DATE:		April 1984
***********************************************************************/

#include "cdef.h"
#include "main.h"
#include <stdio.h>

extern FILE	*Errfp;
extern BOARD	*Bgeom;

#define newpin()  ((PIN *)malloc(sizeof(PIN)))
#define newnet()  ((NET *)malloc(sizeof(NET)))

/*
 *  --------------------------------------------------------------------
 *  There are four classes of nets to be put on the wirelist:
 *  --------------------------------------------------------------------
 *  1. Regular 	 Pins to be wired to regular board pin locations.
 *		 There are two varieties of these:
 *
 *		 A. Device pins 
 *		    ----------------------------------
 *		    add_dev_pin(net_ptr, pin, dev_ptr)
 *		    ----------------------------------
 *		    these are put on the appropriate net
 *		    pinlist with a reference to the device.
 *
 *		 B. Connector pins
 *		    ----------------------------------
 *		    add_cnn_pin(net_ptr, pin, dev_ptr)
 *		    ----------------------------------
 *		    these are put on the appropriate net
 *		    pinlist with a reference to the connector.
 *  --------------------------------------------------------------------
 *  2. Ground	 add_gnd_pin(pin, dev_ptr)
 *		 -------------------------
 *		 Device pins to be wired to board gnd pin locations.
 * 		 These are put on the per-device gnd list of the
 *		 appropriate device.
 *  --------------------------------------------------------------------
 *  3. Voltage 	 add_vcc_pin(pin, dev_ptr)
 *		 -------------------------
 *		 Device pins to be wired to board vcc pin locations.
 * 		 These are put on the per-device vcc list of the
 *		 appropriate device.
 *		 
 *  --------------------------------------------------------------------
 *  4. Connector  add_cnn_gnd(pin, cnn_ptr), add_cnn_vcc(pin, cnn_ptr)
 *		 ----------------------------------------------------
 *		 Connector pins to be wrapped to board gnd or board vcc
 *		 pins.  These are put on the (geometry dependant) per
 *		 connector gnd/vcc pin list of the appropriate connector,
 *		 (analogous to the per device gnd/vcc pin list). 
 *  --------------------------------------------------------------------
 *  The front end call to all of these functions is the following:
 *
 *		`add_pin(net_name,pin_no,ref_des,w)'
 *
 *  the caller need know nothing about the various cases above, he
 *  merely needs to give a VALID net name, a pin number (to be wrapped),
 *  a VALID reference designator, and the wirelist itself.  `ref_des'
 *  tells us if a pin is the pin of a device, or of a connector.  The
 *  `net_name' tells us if a pin is to be connected to a board gnd pin,
 *  a board vcc pin, or a regular board pin.  The other (above) functions
 *  are lower level and need to be used in conjunction with `add_net()',
 *  `find_dev()', and `find_cnn()'.
 *  --------------------------------------------------------------------
 */


/*
 *  add_pin
 *
 *  Wrap pin number `pin' of the device referenced by reference
 *  designator `refdes' onto a net with signal name `signal'.
 *
 *  If the given netname is that of a GND or VCC signal, then a new
 *  pin_list node will be added to the per-device list of the given
 *  refdes.  Otherwise, get the appropriate net using `add_net()',
 *  and make a new pin entry on it.
 */

PIN *
add_pin(netname,pin,refdes,w)
register char *netname;
register unsigned int pin;
register char *refdes;
register WIRELIST *w;
{
	PIN	*add_gnd_pin(), *add_vcc_pin();
	PIN	*add_dev_pin(), *add_cnn_pin();
	PIN	*add_cnn_gnd(), *add_cnn_vcc();
	NET	*add_net();
	CNN	*cnn, *find_cnn();
	DEVICE	*dev, *find_dev();

	if((dev = find_dev(refdes,w)) != NULL) {
		if(is_gnd(netname))
			return(add_gnd_pin(pin,dev));
		else if(is_vcc(netname))
			return(add_vcc_pin(pin,dev));
		else
			return(add_dev_pin(add_net(netname,w),pin,dev));
	}
	else if((cnn = find_cnn(refdes,w)) != NULL) {
		if(is_gnd(netname))
			return(add_cnn_gnd(pin,cnn));
		else if(is_vcc(netname))
			return(add_cnn_vcc(pin,cnn));
		else
			return(add_cnn_pin(add_net(netname,w),pin,cnn));
	}
	else {
		fprintf(Errfp,
		"Ignoring net: %s (%s)-%u (unknown refdes)\n",
		netname,refdes,pin);
		return(NULL);
	}
}

/*
 *  add_dev_pin
 *
 *  Put a new pin list node on the net structure pointed to by
 *  `net', with pin number `pin' of a device pointed to by `dev'.
 */

PIN *
add_dev_pin(net,pin,dev)
NET *net;
register unsigned int pin;
register DEVICE *dev;
{
	register PIN *p;

	if(pin > dev->d_pkg->pk_npins) {
		fprintf(Errfp,
		"Ignoring device pin: (%s)-%d (bad %s pin)\n",
		dev->d_refdes,pin,dev->d_pkg->pk_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new device pin");
	p->p_type = dev->d_pkg->pk_type;
	p->p_owner.dev = dev;
	p->p_pin = pin;
	p->p_next = net->n_pinlist;
	net->n_pinlist = p;
	return(p);
}

/*
 *  add_cnn_pin
 *
 *  Put a new pin list node on the net structure pointed to by
 *  `net', with pin number `pin' of a connector pointed to by `cnn'.
 */

PIN *
add_cnn_pin(net,pin,cnn)
NET *net;
register unsigned int pin;
register CNN *cnn;
{
	register PIN *p;

	if(pin > cnn->c_npins) {
		fprintf(Errfp,
		"Ignoring CNN pin: (%s)-%d (bad %s pin)\n",
		cnn->c_refdes,pin,cnn->c_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new CNN/DEV pin");
	p->p_type = IS_CNN;
	p->p_owner.cnn = cnn;
	p->p_pin = pin;
	p->p_next = net->n_pinlist;
	net->n_pinlist = p;
	return(p);
}

/*
 *  add_gnd_pin
 *
 *  Put a new pin list node on the per-device gnd list of the device
 *  structure pointed to by `dev' (non-NULL), with pin number `pin'.
 */

PIN *
add_gnd_pin(pin,dev)
register unsigned int pin;
register DEVICE *dev;
{
	register PIN *p;

	if(pin > dev->d_pkg->pk_npins) {
		fprintf(Errfp,
		"Ignoring GND pin: (%s)-%d (bad %s pin)\n",
		dev->d_refdes,pin,dev->d_pkg->pk_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new GND pin");
	p->p_pin = pin;
	p->p_type = dev->d_pkg->pk_type;
	p->p_owner.dev = dev;
	p->p_next = dev->d_gndlist;
	dev->d_gndlist = p;
	return(p);
}

/*
 *  add_vcc_pin
 *
 *  Put a new pin list node on the per-device vcc list of the device
 *  structure pointed to by `dev' (non-NULL), with pin number `pin'.
 */

PIN *
add_vcc_pin(pin,dev)
register unsigned int pin;
register DEVICE *dev;
{
	register PIN *p;

	if(pin > dev->d_pkg->pk_npins) {
		fprintf(Errfp,
		"Ignoring VCC pin: (%s)-%d (bad %s pin)\n",
		dev->d_refdes,pin,dev->d_pkg->pk_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new VCC pin");
	p->p_pin = pin;
	p->p_type = dev->d_pkg->pk_type;
	p->p_owner.dev = dev;
	p->p_next = dev->d_vcclist;
	dev->d_vcclist = p;
	return(p);
}

/*
 *  add_cnn_gnd
 *
 *  Put a new pin list node on the per-connector gnd list of the
 *  connector structure pointed to by `cnn', with pin number `pin'.
 */

PIN *
add_cnn_gnd(pin,cnn)
register unsigned int pin;
register CNN *cnn;
{
	register PIN *p;

	if(pin > cnn->c_npins) {
		fprintf(Errfp,
		"Ignoring CNN/GND pin: (%s)-%d (bad %s pin)\n",
		cnn->c_refdes,pin,cnn->c_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new CNN/GND pin");
	p->p_pin = pin;
	p->p_type = IS_CNN;
	p->p_owner.cnn = cnn;
	p->p_next = cnn->c_gndlist;
	cnn->c_gndlist = p;
	return(p);
}

/*
 *  add_cnn_vcc
 *
 *  Put a new pin list node on the per-connector vcc list of the
 *  connector structure pointed to by `cnn', with pin number `pin'.
 */

PIN *
add_cnn_vcc(pin,cnn)
register unsigned int pin;
register CNN *cnn;
{
	register PIN *p;

	if(pin > cnn->c_npins) {
		fprintf(Errfp,
		"Ignoring CNN/VCC pin: (%s)-%d (bad %s pin)\n",
		cnn->c_refdes,pin,cnn->c_name);
		return(NULL);
	}
	if((p = newpin()) == NULL)
		fatal(ENOMEM,"new CNN/VCC pin");
	p->p_pin = pin;
	p->p_type = IS_CNN;
	p->p_owner.cnn = cnn;
	p->p_next = cnn->c_vcclist;
	cnn->c_vcclist = p;
	return(p);
}

/*
 *  add_net
 *
 *  Return a pointer to an entry on the wirelist net list with the
 *  VALID net name `name'.  If an entry does not yet exist, make one.
 *  The list is kept in alphabetical order ordered this name.
 *
 *  Implementation:
 *  The head of this doubly linked list points to the node with the lexi-
 *  cographically least signal name.  The next pointers of each node in the
 *  list point to nodes whose signal names are lexicographically greater,
 *  except for the prev pointer of the first node in the list which points
 *  to the last node in the list.   This scheme is chosen because the VALID
 *  net list seems to be in alphabetical order by net name, but we do not
 *  want to rely in this fact because the VALID file formats may "change
 *  without warning" (but we do want to take advantage of this apparent
 *  ordering).  This way, we can usually insert each successive net into
 *  the list by looking at (at most) one net if we search the list from the
 *  back to the front (lexicographically greatest to least).
 */

NET *
add_net(name,w)
char *name;
WIRELIST *w;
{
	char		*sig_name(), *strsave();
	register NET	*new, *p, *last;
	register int	cmp;

	/*
	 *  Null list
	 */
	if(w->w_net == NULL) {
		if((new = newnet()) == NULL)
			fatal(ENOMEM,"new net list");
		new->n_next = NULL;
		w->w_net = new;
		last = new;
		goto Out;
	}
	last = w->w_net->n_prev;
	w->w_net->n_prev = NULL;
	for(p = last ; p != NULL ; p = p->n_prev) {
		/*
		 *  Found node
		 */
		if((cmp=strcmp(name,p->n_name)) == 0) {
			w->w_net->n_prev = last;
			return(p);
		}
		/*
		 *  Put node after p
		 */
		if(cmp > 0) {
			if((new = newnet()) == NULL)
				fatal(ENOMEM,"new net node");
			new->n_next = p->n_next;
			new->n_prev = p;
			/*
			 *  New mid-list node
			 */
			if(p != last)
				p->n_next->n_prev = new;
			/*
			 *  New last node
			 */
			else
				last = new;
			p->n_next = new;
			goto Out;
		}
		/*
		 *  Otherwise, cmp < 0, so continue
		 */
	}
	/*
	 *  New first node
	 */
	if(p == NULL) {
		if((new = newnet()) == NULL)
			fatal(ENOMEM,"new net node");
		new->n_next = w->w_net;
		w->w_net->n_prev = new;
		w->w_net = new;
		goto Out;
	}
Out:
	if((new->n_name = strsave(name)) == NULL)
		fatal(ENOMEM,"new net name");
	strncpy(new->n_sig,sig_name(name),SIGLEN);
	new->n_sig[SIGLEN] = EOS;
	new->n_pinlist = NULL;
	w->w_net->n_prev = last;
	w->w_nnet++;
	return(new);
}

/*
 *  mv_pwrpins
 *
 *  Move (not copy) the per device GND/VCC lists, and the
 *  per connector GND/VCC lists to the main NET table.
 *
 *  Implementation:
 *  For the per device lists: the gnd/vcc_sig_name functions create a
 *  signal name which will indicate the nearest gnd/vcc board pins to
 *  one of the pins on the device (decided in that function), a net table
 *  entry will be allocated (or found) with that name (using "add_net()"),
 *  and then by manipulating pointers, the gnd/vcc list will be placed on
 *  that net table entry's pin list.
 *  For the per connector lists: for each pin on the gnd/vcc list, the
 *  cnn_gnd/vcc_sig functions will create a signal name indicating the
 *  nearest gnd/vcc pin to that pin, an net table entry will be allocated
 *  (or found) with that name, and that pin entry will be placed on that
 *  net table entry's pin list.
 */

void
mv_pwrpins(w)
WIRELIST *w;
{
	char		*gnd_sig_name();
	char		*vcc_sig_name();
	NET		*add_net();
	register DEVICE *d;
	register NET	*n;
	register PIN	*p;
	register CNN	*c;

	for(d = w->w_dev ; d != NULL ; d = d->d_next) {
		if(d->d_gndlist != NULL)
			n = add_net(gnd_sig_name(d->d_gndlist),w);
		while(d->d_gndlist != NULL) {
			p = d->d_gndlist;
			d->d_gndlist = p->p_next;
			p->p_next =  n->n_pinlist;
			n->n_pinlist = p;
		}
		if(d->d_vcclist != NULL)
			n = add_net(vcc_sig_name(d->d_vcclist),w);
		while(d->d_vcclist != NULL) {
			p = d->d_vcclist;
			d->d_vcclist = p->p_next;
			p->p_next =  n->n_pinlist;
			n->n_pinlist = p;
		}
	}
	for(c = Bgeom->b_cnn ; c->c_refdes != NULL ; c++) {
		while(c->c_gndlist != NULL) {
			n = add_net(cnn_gnd_sig(c,c->c_gndlist->p_pin),w);
			p = c->c_gndlist;
			c->c_gndlist = p->p_next;
			p->p_next =  n->n_pinlist;
			n->n_pinlist = p;
		}
		while(c->c_vcclist != NULL) {
			n = add_net(cnn_vcc_sig(c,c->c_vcclist->p_pin),w);
			p = c->c_vcclist;
			c->c_vcclist = p->p_next;
			p->p_next =  n->n_pinlist;
			n->n_pinlist = p;
		}
	}
}
