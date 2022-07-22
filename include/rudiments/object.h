// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OBJECT_H
#define RUDIMENTS_OBJECT_H

#include <rudiments/private/objectincludes.h>

/** The object class is the parent class for all rudiments classes. */
class object {
	public:

		/** Deletes this instance of the object class. */
		virtual	~object() {};

	#include <rudiments/private/object.h>
};

#endif
