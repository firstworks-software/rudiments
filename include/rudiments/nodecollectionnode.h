// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_NODECOLLECTIONNODE_H
#define RUDIMENTS_NODECOLLECTIONNODE_H

#include <rudiments/object.h>

/** The collection class is the parent class for the node contained by all
 *  rudiments node-based collections. */
class RUDIMENTS_DLLSPEC nodecollectionnode : public object {
	public:

		/** Creates an instance of the nodecollectionnode class. */
		nodecollectionnode() : object() {};

		/** Creates an instance of the nodecollectionnode class. */
		nodecollectionnode(const collection &c) : object(c) {};

		/** Deletes this instance of the nodecollectionnode class. */
		virtual	~nodecollectionnode() {};
};

#endif
