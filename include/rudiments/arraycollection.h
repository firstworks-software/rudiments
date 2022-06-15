// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ARRAYCOLLECTION_H
#define RUDIMENTS_ARRAYCOLLECTION_H

#include <rudiments/collection.h>

/** The arraycollection class is the parent class for all rudiments array
 *  collections. */
template <class valuetype>
class arraycollection : public collection {
	public:
		/** Provides read/write access ot the "index"th element of the
		 *  array. */
		virtual	valuetype	&operator[](uint64_t index)=0;

		/** Provides read-only access ot the "index"th element of the
		 *  array. */
		virtual	valuetype	operator[](uint64_t index) const=0;

		/** Returns the number of elements in the array. */
		virtual	uint64_t	getLength() const=0;
};

#endif
