// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UTF16CHARACTER_H
#define RUDIMENTS_UTF16CHARACTER_H

#include <rudiments/private/utf16characterincludes.h>

/** The utf16character class provides static methods for evaluating and
 *  manipulating UTF-16 encoded characters. */
class RUDIMENTS_DLLSPEC utf16character {
	public:

		/** Returns the number of bytes needed to NULL-terminate a
		 *  string of UTF-16-encoded characters. */
		static uint16_t	getNullSize();

		/** Returns the number of bytes in the UTF-16 byte-order
		 *  mark. */
		static uint16_t	getBomSize();
};

#endif
