// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UTF8CHARACTER_H
#define RUDIMENTS_UTF8CHARACTER_H

#include <rudiments/private/utf8characterincludes.h>

/** The utf8character class provides static methods for evaluating and
 *  manipulating UTF-8 encoded characters. */
class RUDIMENTS_DLLSPEC utf8character {
	public:

		/** Returns the number of bytes needed to NULL-terminate a
		 *  string of UTF-8-encoded characters. */
		static uint8_t	getNullSize();

		/** Returns the number of bytes in the UTF-8 byte-order mark. */
		static uint8_t	getBomSize();
};

#endif
