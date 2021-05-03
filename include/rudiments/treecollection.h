// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TREECOLLECTION_H
#define RUDIMENTS_TREECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The treecollection class is the parent class for all rudiments tree
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC treecollection : public nodecollection {
	public:

		/** Creates an instance of the treecollection class. */
		treecollection() : nodecollection() {};

		/** Creates an instance of the treecollection class. */
		treecollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the treecollection class. */
		virtual	~treecollection() {};
};

#endif
