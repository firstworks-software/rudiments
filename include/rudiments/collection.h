// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_COLLECTION_H
#define RUDIMENTS_COLLECTION_H

#include <rudiments/object.h>

/** The collection class is the parent class for all rudiments collections. */
class RUDIMENTS_DLLSPEC collection : public object {
	public:

		/** Creates an instance of the collection class. */
		collection() : object() {};

		/** Creates an instance of the collection class that is a copy
		 *  of "c". */
		collection(const collection &c) : object(c) {};

		/** Deletes this instance of the collection class. */
		virtual	~collection() {};
};

#endif
