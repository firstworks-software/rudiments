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

		/** Returns true if "c" is alphabetical character, including
		 *  accented alphabetical characters from the extended ascii
		 *  character set, and false otherwise. */
		static bool	isAlphabeticalExtended(int32_t c);

		/** Returns true if "c" is lower case and false otherwise. */
		static bool	isLowerCase(int32_t c);

		/** Returns true if "c" is lower case, including
		 *  accented alphabetical characters from the extended ascii
		 *  character set, and false otherwise. */
		static bool	isLowerCaseExtended(int32_t c);

		/** Returns true if "c" is upper case and false otherwise. */
		static bool	isUpperCase(int32_t c);

		/** Returns true if "c" is upper case, including
		 *  accented alphabetical characters from the extended ascii
		 *  character set, and false otherwise. */
		static bool	isUpperCaseExtended(int32_t c);

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
		 *  in the ascii character set or false otherwise. */
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

		/** Returns the wide character representation of
		 *  character "c". */
		static wchar_t	duplicate(char c);

		/** Returns true if the duplicate methods that convert to a
		 *  wchar_t from a char require a mutex to operate safely in
		 *  a threaded environment and false otherwise. */
		static bool	duplicateFromCharacterNeedsMutex();

		/** Returns true if the platform supports wide characters and
		 *  rudiments was build with wide character support and false
		 *  otherwise. */
		static bool	supported();
};

#endif
