// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OBJECT_H
#define RUDIMENTS_OBJECT_H

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

/** The object class is the parent class for all rudiments classes. */
class object {
	public:

#if 0
		/** Creates an instance of the object class. */
		object() {};

		/** Creates an instance of the object class. */
		object(const object &o) {};
#endif

		/** Deletes this instance of the object class. */
		virtual	~object() {};
};

#endif
