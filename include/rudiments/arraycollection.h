// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ARRAYCOLLECTION_H
#define RUDIMENTS_ARRAYCOLLECTION_H

#include <rudiments/collection.h>

/** The arraycollection class is the parent class for all rudiments array
 *  collections. */
class RUDIMENTS_DLLSPEC arraycollection : public collection {
	public:

		/** Creates an instance of the arraycollection class. */
		arraycollection() : collection() {};

		/** Creates an instance of the arraycollection class. */
		arraycollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~arraycollection() {};
};

#endif
