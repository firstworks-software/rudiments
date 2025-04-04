// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UTF8CHARSTRING_H
#define RUDIMENTS_UTF8CHARSTRING_H

#include <rudiments/private/utf8charstringincludes.h>

/** The utf8charstring class provides static methods for manipulating UCS-2
 *  encoded character strings. */
class RUDIMENTS_DLLSPEC utf8charstring {
	public:

		/** Returns true if the current character in "str" is the byte
		 *  order mark and false otherwise. */
		static bool	isByteOrderMark(const utf8_t *str);

		/** Returns the number of characters in "string". */
		static	size_t	getLength(const utf8_t *string);

		/** Returns the number of bytes in "string", including the
		 *  null terminator. */
		static	size_t	getSize(const utf8_t *string);

	#include <rudiments/private/utf8charstring.h>
};

#endif
