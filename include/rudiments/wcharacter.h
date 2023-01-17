// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WCHARACTER_H
#define RUDIMENTS_WCHARACTER_H

#include <rudiments/private/wcharacterincludes.h>

/** The wcharacter class provides static methods for evaluating and manipulating
 *  wide characters. */
class RUDIMENTS_DLLSPEC wcharacter {
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
		static bool	inSet(wchar_t c, const wchar_t *set);

		/** Returns the wide character representation of character "c"
		 *  as interpreted by the character set of the current
		 *  locale. */
		static wchar_t	duplicate(char c);

		/** Returns the wide character representation of character "c"
		 *  as interpreted by the character set of the current locale.
		 *
		 *  Returns "replacement" if character "c" cannot be converted
		 *  to a wide character. */
		static wchar_t	duplicate(char c, wchar_t replacement);

		/** Returns the wide character representation of UCS-2
		 *  character "c".
		 *
		 *  "c" is presumed to be in the byte-order of the system.
		 *
		 *  Returns '?' if UCS-2 character "c" cannot be converted to a
		 *  character. */
		static wchar_t	duplicateUcs2(ucs2_t c);

		/** Returns the wide character representation of UCS-2
		 *  character "c".
		 *
		 *  "c" is presumed to be in the byte-order of the system.
		 *
		 *  Returns "replacement" if UCS-2 character "c" cannot be
		 *  converted to a character. */
		static wchar_t	duplicateUcs2(ucs2_t c, wchar_t replacement);

		/** Returns the wide character representation of UCS-2
		 *  character "c".
		 *
		 *  If "bigendian" is true then "c" is presumed to be big
		 *  endian.  If "bigendian" is false, then "c" is presumed to
		 *  be little endian.
		 *
		 *  Returns '?' if UCS-2 character "c" cannot be converted to a
		 *  wide character. */
		static wchar_t	duplicateUcs2(ucs2_t c, bool bigendian);

		/** Returns the wide character representation of UCS-2
		 *  character "c".
		 *
		 *  If "bigendian" is true then "c" is presumed to be big
		 *  endian.  If "bigendian" is false, then "c" is presumed to
		 *  be little endian.
		 *
		 *  Returns "replacement" if UCS-2 character "c" cannot be
		 *  converted to a wide character. */
		static wchar_t	duplicateUcs2(ucs2_t c,
						wchar_t replacement,
						bool bigendian);
};

#endif
