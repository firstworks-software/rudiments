// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OBJECT_H
#define RUDIMENTS_OBJECT_H

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

/** The object class is the parent class for all rudiments classes. */
class RUDIMENTS_DLLSPEC object {
	public:

		/** Creates an instance of the object class. */
		object() {};

		/** Creates an instance of the object class. */
		object(const object &o) {};

		/** Deletes this instance of the object class. */
		virtual	~object() {};
};

#endif
