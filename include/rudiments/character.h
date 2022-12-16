// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARACTER_H
#define RUDIMENTS_CHARACTER_H

#include <rudiments/private/characterincludes.h>

/** The character class provides static methods for evaluating and manipulating
 *  ASCII characters. */
class RUDIMENTS_DLLSPEC character {
	public:
		/** Returns true if "c" is an alphanumeric character
		 *  and false otherwise. */
		static bool	isAlphanumeric(int32_t c);

		/** Returns true if "c" is alphabetical character
		 *  and false otherwise. */
		static bool	isAlphabetical(int32_t c);

		/** Returns true if "c" is alphabetical character, including
		 *  accented alphabetical characters from the IBM PC Extended
		 *  ASCII character set (Code page 437), and false otherwise. */
		static bool	isAlphabeticalExtended(int32_t c);

		/** Returns true if "c" is lower case and false otherwise. */
		static bool	isLowerCase(int32_t c);

		/** Returns true if "c" is lower case, including
		 *  accented alphabetical characters from the IBM PC Extended
		 *  ASCII character set (Code page 437), and false otherwise. */
		static bool	isLowerCaseExtended(int32_t c);

		/** Returns true if "c" is upper case and false otherwise. */
		static bool	isUpperCase(int32_t c);

		/** Returns true if "c" is upper case, including
		 *  accented alphabetical characters from the IBM PC Extended
		 *  ASCII character set (Code page 437), and false otherwise. */
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
		static bool	inSet(char c, const char *set);

		/** Returns the character representation of wide character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns '?' if wide character "c" cannot be converted to a
		 *  character. */
		static char	duplicate(wchar_t c);

		/** Returns the character representation of wide character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns "replacement" if wide character "c" cannot be
		 *  converted to a character. */
		static char	duplicate(wchar_t c, char replacement);

		/** Returns the character representation of UCS-2 character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns '?' if UCS-2 character "c" cannot be converted to a
		 *  character. */
		static char	duplicate(ucs2_t c);

		/** Returns the character representation of UCS-2 character "c"
		 *  per the character set of the current locale.
		 *
		 *  Returns "replacement" if UCS-2 character "c" cannot be
		 *  converted to a character. */
		static char	duplicate(ucs2_t c, char replacement);

		/** Returns true if the duplicate methods that convert to a
		 *  char from a wchar_t require a mutex to operate safely in
		 *  a threaded environment and false otherwise. */
		static bool	duplicateFromWideCharacterNeedsMutex();
};

#endif
