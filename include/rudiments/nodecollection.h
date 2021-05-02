// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NODECOLLECTION_H
#define RUDIMENTS_NODECOLLECTION_H

#include <rudiments/collection.h>
#include <rudiments/nodecollectionnode.h>

/** The collection class is the parent class for all rudiments node-based
 *  collections. */
class RUDIMENTS_DLLSPEC nodecollection : public collection {
	public:

		/** Creates an instance of the nodecollection class. */
		nodecollection() : collection() {};

		/** Creates an instance of the nodecollection class. */
		nodecollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the nodecollection class. */
		virtual	~nodecollection() {};
};

#endif
