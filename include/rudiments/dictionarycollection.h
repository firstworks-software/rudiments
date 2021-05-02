// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DICTIONARYCOLLECTION_H
#define RUDIMENTS_DICTIONARYCOLLECTION_H

#include <rudiments/collection.h>

/** The dictionarycollection class is the parent class for all rudiments
 *  dictionary collections. */
class RUDIMENTS_DLLSPEC dictionarycollection : public collection {
	public:

		/** Creates an instance of the dictionarycollection class. */
		dictionarycollection() : collection() {};

		/** Creates an instance of the dictionarycollection class. */
		dictionarycollection(const collection &c) : collection(c) {};

		/** Deletes this instance of the listcollection class. */
		virtual	~dictionarycollection() {};
};

#endif
