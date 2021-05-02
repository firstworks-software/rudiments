// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TABLECOLLECTION_H
#define RUDIMENTS_TABLECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The tablecollection class is the parent class for all rudiments table
 *  collections. */
class RUDIMENTS_DLLSPEC tablecollection : public nodecollection {
	public:

		/** Creates an instance of the tablecollection class. */
		tablecollection() : nodecollection() {};

		/** Creates an instance of the tablecollection class. */
		tablecollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the tablecollection class. */
		virtual	~tablecollection() {};
};

#endif
