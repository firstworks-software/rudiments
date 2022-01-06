// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SCALARCOLLECTION_H
#define RUDIMENTS_SCALARCOLLECTION_H

#include <rudiments/collection.h>

/** The scalarcollection class is the parent class for all rudiments scalar
 *  collections. */
template <class valuetype>
class scalarcollection : public collection {
	public:

		/** Deletes this instance of the scalarcollection class. */
		virtual	~scalarcollection() {};

		/** Sets the value stored in this instance to "v". */
		virtual	void		setValue(valuetype v)=0;

		/** Returns the value stored in this intance.  Returns NULL or
		 *  0 if no value has been stored. */
		virtual	valuetype	getValue() const=0;

		/** Removes the value currently stored in this instance, such
		 *  that getValue() will return NULL or 0.
		 *
		 *  The value is only deleted if setManageValues(true) or
		 *  setManageArrayValues(true) has been called. */
		virtual	void		clear()=0;
};

#endif
