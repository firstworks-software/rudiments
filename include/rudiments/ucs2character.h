// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UCS2CHARACTER_H
#define RUDIMENTS_UCS2CHARACTER_H

#include <rudiments/private/ucs2characterincludes.h>

/** The ucs2character class provides static methods for evaluating and
 *  manipulating UCS-2 encoded characters. */
class RUDIMENTS_DLLSPEC ucs2character {
	public:
		/** Returns true if "c" is an alphanumeric character
		 *  and false otherwise. */
		static bool	isAlphanumeric(int32_t c);

		/** Returns true if "c" is alphabetical character
		 *  and false otherwise. */
		static bool	isAlphabetical(int32_t c);

		/** Returns true if "c" is lower case and false otherwise. */
		static bool	isLowerCase(int32_t c);

		/** Returns true if "c" is upper case and false otherwise. */
		static bool	isUpperCase(int32_t c);

		/** Returns true if "c" is a printable character that is not
		 *  a space or alphanumeric character and false otherwise. */
		static bool	isPunctuation(int32_t c);

		/** Returns true if "c" is a printable character
		 *  and false otherwise. */
		static bool	isPrintable(int32_t c);

		/** Returns true if "c" is a printable character
		 *  other than a space and false otherwise. */
		static bool	isPrintableNonSpace(int32_t c);

		/** Returns true if "c" is a control character
		 *  and false otherwise. */
		static bool	isControlCharacter(int32_t c);

		/** Returns true if "c" is a digit and false otherwise. */
		static bool	isDigit(int32_t c);

		/** Returns true if "c" is a hexadecimal digit
		 *  and false otherwise. */
		static bool	isHexDigit(int32_t c);

		/** Returns true if "c" is a space or tab
		 *  and false otherwise. */
		static bool	isBlank(int32_t c);

		/** Returns true if "c" is any whitespace
		 *  character and false otherwise. */
		static bool	isWhitespace(int32_t c);

		/** Returns true if "c" is one of the values
		 *  in the ASCII character set or false otherwise. */
		static bool	isAscii(int32_t c);

		/** Returns the upper case version of "c". */
		static int32_t	toUpperCase(int32_t c);

		/** Returns the lower case version of "c". */
		static int32_t	toLowerCase(int32_t c);

		/** Converts "c" to a 7-bit value by clearing
		 *  the higher order bits. */
		static int32_t	toAscii(int32_t c);

		/** Returns true if "c" is in the set of
		 *  characters in "set" and false otherwise. */
		static bool	inSet(ucs2_t c, const ucs2_t *set);

		/** Returns the UCS-2 character representation of character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns '?' if character "c" cannot be converted to a
		 *  UCS-2 character.
		 *
		 *  The return value will be in the byte-order of the system. */
		static ucs2_t	duplicate(char c);

		/** Returns the UCS-2 character representation of character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns "replacement" if character "c" cannot be converted
		 *  to a UCS-2 character.
		 *
		 *  The return value will be in the byte-order of the system. */
		static ucs2_t	duplicate(char c, ucs2_t replacement);

		/** Returns the UCS-2 character representation of character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns '?' if character "c" cannot be converted to a
		 *  UCS-2 character.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. */
		static ucs2_t	duplicate(char c, bool bigendian);

		/** Returns the UCS-2 character representation of character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns "replacement" if character "c" cannot be converted
		 *  to a UCS-2 character.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. */
		static ucs2_t	duplicate(char c,
					ucs2_t replacement,
					bool bigendian);

		/** Returns the UCS-2 character representation of wide
		 *  character "c" per the character set of the current locale.
		 *
		 *  Returns '?' if wide character "c" cannot be converted to a
		 *  UCS-2 character.
		 *
		 *  The return value will be in the byte-order of the system. */
		static ucs2_t	duplicate(wchar_t c);

		/** Returns the UCS-2 character representation of wide
		 *  character "c" per the character set of the current locale.
		 *
		 *  Returns "replacement" if wide character "c" cannot be
		 *  converted to a UCS-2 character.
		 *
		 *  The return value will be in the byte-order of the system. */
		static ucs2_t	duplicate(wchar_t c, ucs2_t replacement);

		/** Returns the UCS-2 character representation of wide
		 *  character "c" per the character set of the current locale.
		 *
		 *  Returns '?' if wide character "c" cannot be converted to a
		 *  UCS-2 character. 
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. */
		static ucs2_t	duplicate(wchar_t c, bool bigendian);

		/** Returns the UCS-2 character representation of wide
		 *  character "c" per the character set of the current locale.
		 *
		 *  Returns "replacement" if wide character "c" cannot be
		 *  converted to a UCS-2 character.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. */
		static ucs2_t	duplicate(wchar_t c,
						ucs2_t replacement,
						bool bigendian);
};

#endif
