// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_LISTCOLLECTION_H
#define RUDIMENTS_LISTCOLLECTION_H

#include <rudiments/nodecollection.h>

/** The listcollection class is the parent class for all rudiments list
 *  collections. */
class RUDIMENTS_DLLSPEC listcollection : public nodecollection {
	public:

		/** Creates an instance of the listcollection class. */
		listcollection() : nodecollection() {};

		/** Creates an instance of the listcollection class. */
		listcollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~listcollection() {};
};

#endif
