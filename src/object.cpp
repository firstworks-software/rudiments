// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/object.h>

bool object::incOrErr(ssize_t *retval, ssize_t val) const {

	// add val to *retval unless:
	// * retval is already negative, indicating that an error condition
	//   occurred previously, in this case leave retval set to the error
	//   condition
	// * val is negative, indicating an error condition just occurred, in
	//   this case set retval to the error condition
	// return true on success or false if an error condition occurred

	if (*retval>-1) {
		if (val>-1) {
			(*retval)+=val;
			return true;
		} else {
			(*retval)=val;
		}
	}
	return false;
}

bool object::incOrErr(ssize_t *retval, ssize_t val, ssize_t expected) const {

	// like incOrErr above, but if val!=expected (eg. in a short read/write
	// condition), then increment retval but also return false
	if (val!=expected) {
		(*retval)+=val;
		return false;
	}
	return incOrErr(retval,val);
}
