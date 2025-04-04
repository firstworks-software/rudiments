// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UTF16CHARSTRING_H
#define RUDIMENTS_UTF16CHARSTRING_H

#include <rudiments/private/utf16charstringincludes.h>

/** The utf16charstring class provides static methods for manipulating UCS-2
 *  encoded character strings. */
class RUDIMENTS_DLLSPEC utf16charstring {
	public:

		/** Returns true if the current character in "str" is the byte
		 *  order mark and false otherwise. */
		static bool	isByteOrderMark(const utf16_t *str);

		/** Returns true if the current character in "str" is a
		 *  bigendian byte-order mark and false otherwise. */
		static bool	isBigEndian(const utf16_t *str);

		/** Returns the number of characters in "string". */
		static	size_t	getLength(const utf16_t *string,
							bool bigendian);

		/** Returns the number of bytes in "string", including the
		 *  null terminator. */
		static	size_t	getSize(const utf16_t *string,
							bool bigendian);

	#include <rudiments/private/utf16charstring.h>
};

#endif
