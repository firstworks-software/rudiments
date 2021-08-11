// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ARRAYCOLLECTION_H
#define RUDIMENTS_ARRAYCOLLECTION_H

#include <rudiments/collection.h>

/** The arraycollection class is the parent class for all rudiments array
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC arraycollection : public collection {
	public:

		/** Creates an instance of the arraycollection class. */
		arraycollection() : collection() {};

		/** Creates an instance of the arraycollection class that is
		 *  a copy of "c". */
		arraycollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the arraycollection class. */
		virtual	~arraycollection() {};

		/** Provides access ot the "index"th element of the array. */
		virtual	valuetype	&operator[](uint64_t index)=0;

		/** Returns the number of elements in the array. */
		virtual	uint64_t	getLength() const=0;

		/** Clears the array, deleting all of its values. */
		virtual	void		clear()=0;
};

#endif
