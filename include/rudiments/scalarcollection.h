// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SCALARCOLLECTION_H
#define RUDIMENTS_SCALARCOLLECTION_H

#include <rudiments/collection.h>

/** The collection class is the parent class for all rudiments scalar
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC scalarcollection : public collection {
	public:

		/** Creates an instance of the scalarcollection class. */
		scalarcollection() : collection() {};

		/** Creates an instance of the scalarcollection class. */
		scalarcollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the scalarcollection class. */
		virtual	~scalarcollection() {};

		virtual	void		setValue(valuetype v)=0;
		virtual	valuetype	getValue() const=0;
};

#endif
