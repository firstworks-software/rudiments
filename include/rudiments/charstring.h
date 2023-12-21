// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CHARSTRING_H
#define RUDIMENTS_CHARSTRING_H

#include <rudiments/private/charstringincludes.h>

/** The charstring class provides static methods for manipulating C-style
 *  character strings.
 * 
 *  In addition to some unique methods, analogs for the standard C string
 *  functions are provided.  However, unlike the standard C string functions,
 *  the charstring methods are NULL safe.  Your application will not crash if a
 *  NULL is passed in, and instead, will give intuitive results.
 *
 *  Note that meny method of this class expect to operate on strings encoded
 *  using ASCII or extended ASCII, and may give unexpected results for
 *  differently-encoded strings. */
class RUDIMENTS_DLLSPEC charstring {
	public:

		/** Returns the number of characters in "string".
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using a single-byte encoding, and will give unexpected
		 *  results for multi-byte-encoded strings. */
		static	size_t	getLength(const char *string);

		/** Returns the number of bytes in "string", including the
		 *  null terminator. */
		static	size_t	getSize(const char *string);

		/** Returns true if "string" is NULL or the empty string. */
		static	bool	isNullOrEmpty(const char *string);

		/** Returns true if "string" starts with the standalone "word":
		 *  "Yes" (case-insensitive), "True" (case-insensitive), 
		 *  "Y" (case-insensitive), "T" (case-insensitive), or
		 *  "1" and false otherwise.
		 *
		 *  (Eg. returns true for "Yes,", "True ", and "1 2 3" but
		 *  returns false for "Yesterday", "Truest", and "123")
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	bool	isYes(const char *string);

		/** Returns true if "string" starts with the standalone word:
		 *  "No" (case-insensitive), "False" (case-insensitive), or
		 *  "0" and false otherwise.
		 *
		 *  (Eg. returns true for "No,", "False ", and "0 1 2" but
		 *  returns false for "Nothing", "Falsest", and "012")
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	bool	isNo(const char *string);

		/** Sets "len" characters of "str" to NULL.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using a single-byte encoding, and will give unexpected
		 *  results for multi-byte-encoded strings. */
		static	void	zero(char *str, size_t len);

		/** Appends "source" to "dest".  Assumes that there is
		 *  enough room remaining in "dest" to accommodate the new
		 *  string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, const char *source);

		/** Appends "len" characters of "source" to "dest".  Assumes
		 *  that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, const char *source,
								size_t len);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, int64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, uint64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	char	*append(char *dest, double number);

		/** Converts "number" to a string using "scale" and appends it
		 *  to "dest".  Assumes that there is enough room remaining in
		 *  "dest" to accommodate the new string.  Returns a pointer to
		 *  "dest". */
		static	char	*append(char *dest, double number,
							uint16_t scale);

		/** Converts "number" to a string using "precision" and "scale"
		 *  and appends it to "dest".  Assumes that there is enough
		 *  room remaining in "dest" to accommodate the new string.
		 *  Returns a pointer to "dest". */
		static	char	*append(char *dest, double number,
						uint16_t precision,
						uint16_t scale);

		/** Replaces "dest" with "source".  Assumes that there is
		 *  enough room in "dest" to accommodate "source".  Returns a
		 *  pointer to "dest". */
		static	char	*copy(char *dest, const char *source);

		/** Replaces the first "len" characters of "dest" with
		 *  "source".  Assumes that "dest" is at least "len" characters
		 *  long.  Returns a pointer to "dest". */
		static	char	*copy(char *dest,
					const char *source, size_t len);

		/** Replaces "dest" with "source", starting "location"
		 *  characters into "dest".  Assumes that there is enough room
		 *  in "dest" (after "location" characters) to accommodate
		 *  "source".  Returns a pointer to "dest". */
		static	char	*copy(char *dest, size_t location,
						const char *source);

		/** Replaces "len" characters of "dest" with "source",
		 *  starting "location" characters into "dest".  Assumes that
		 *  there are "len" characters in "dest" (after "location"
		 *  characters).  Returns a pointer to "dest". */
		static	char	*copy(char *dest, size_t location,
					const char *source, size_t len);

		/** Replaces "dest" with "source" unless the character length
 		 *  of "source" is greater than "destlen", in which case only
 		 *  "destlen" characters of "dest" will be replaced.  Returns a
 		 *  pointer to "dest". */
		static char	*safeCopy(char *dest, size_t destlen,
							const char *source);

		/** Replaces "sourcelen" characters of "dest" with "source"
		 *  unless "sourcelen" is greater than "destlen", in which case
		 *  only "destlen" characters of "dest" will be replaced.
		 *  Returns a pointer to "dest". */
		static char	*safeCopy(char *dest, size_t destlen,
					const char *source, size_t sourcelen);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2". */
		static	int32_t	compare(const char *str1, const char *str2);

		/** Returns -1,0 or 1 if "len" characters of "str1" are
		 *  greater than, equal to or less than "len" characters of
		 *  "str2". */
		static	int32_t	compare(const char *str1, const char *str2,
								size_t len);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", ignoring case. */
		static	int32_t	compareIgnoringCase(const char *str1,
							const char *str2);

		/** Returns -1,0 or 1 if "len" characters of "str1" are greater
		 *  than, equal to or less than "len" characters of "str2",
		 *  ignoring case. */
		static	int32_t	compareIgnoringCase(const char *str1,
							const char *str2,
							size_t len);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2".
		 *
		 * Embedded (version) numbers will be interpreted "naturally"
		 * (eg. 12 > 8)
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	int32_t	compareNatural(const char *str1,
						const char *str2);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2".
		 *
		 * Embedded (version) numbers will be interpreted "naturally"
		 * (eg. 12 > 8)
		 *
		 * "delimiters" should be set to the set of characters that
		 * will be interpreted as equivalent delimiters.  Eg. "._"
		 * means that either .'s or _'s may be used to delimit
		 * numbers.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	int32_t	compareNatural(const char *str1,
						const char *str2,
						const char *delimiters);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of decimal delimiters.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	int32_t	compareVersions(const char *str1,
						const char *str2);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of delimiters.
		 *
		 * "delimiters" should be set to the set of characters that
		 * will be interpreted as equivalent delimiters.  Eg. "._"
		 * means that either .'s or _'s may be used to delimiters
		 * numbers.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	int32_t	compareVersions(const char *str1,
						const char *str2,
						const char *delimiters);

		/** Compares the first "stringlen" characters of "string"
		 *  to the first "patternlen" characters of "pattern" which
		 *  may contain instances of single-character wildcard markers
		 *  "singlewildcard" or multi-character wildcard markers
		 *  "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const char *string,
							size_t stringlen,
							const char *pattern,
							size_t patternlen,
							char singlewildcard,
							char multiwildcard);

		/** Compares "string" to the first "patternlen" characters
		 *  of "pattern" which may contain instances of single-character
		 *  wildcard markers "singlewildcard" or multi-character
		 *  wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const char *string,
							const char *pattern,
							size_t patternlen,
							char singlewildcard,
							char multiwildcard);

		/** Compares the first "stringlen" characters of "string"
		 *  to "pattern" which may contain instances of
		 *  single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const char *string,
							size_t stringlen,
							const char *pattern,
							char singlewildcard,
							char multiwildcard);

		/** Compares "string" to "pattern" which may contain instances
		 *  of single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const char *string,
							const char *pattern,
							char singlewildcard,
							char multiwildcard);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set".  Also returns true if "str"
		 *  is NULL and set is NULL or contains only a NULL-terminator.
		 *  Otherwise returns false. */
		static	bool	isInSet(const char *str,
						const char * const *set);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set", ignoring case.  Also returns
		 *  true if "str" is NULL and set is NULL or contains only a
		 *  NULL-terminator.  Otherwise returns false. */
		static	bool	isInSetIgnoringCase(const char *str,
						const char * const *set);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const char *haystack,
							const char *needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const char *haystack,
							const char *needle);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const char *haystack, char needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const char *haystack,
								char needle);

		/** Returns true if "haystack" starts with "needle" or
		 *  false otherwise. */
		static	bool	startsWith(const char *haystack,
						const char *needle);

		/** Returns true if "haystack" ends with "needle" or
		 *  false otherwise. */
		static	bool	endsWith(const char *haystack,
						const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findFirst(const char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const char	*findFirstIgnoringCase(
							const char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findFirst(const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const char	*findFirstIgnoringCase(
							const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const char	*findFirstOrEnd(const char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const char	*findFirstOrEndIgnoringCase(
							const char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const char	*findFirstOrEnd(const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const char	*findFirstOrEndIgnoringCase(
							const char *haystack,
							char needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findLast(const char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const char	*findLastIgnoringCase(
							const char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const char	*findLast(const char *haystack,
							char needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const char	*findLastIgnoringCase(
							const char *haystack,
							char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findFirst(char *haystack, const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	char	*findFirstIgnoringCase(char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findFirst(char *haystack, char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	char	*findFirstIgnoringCase(char *haystack,
								char needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	char	*findFirstOrEnd(char *haystack,
							const char *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	char	*findFirstOrEndIgnoringCase(char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findLast(char *haystack, const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	char	*findLastIgnoringCase(char *haystack,
							const char *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	char	*findLast(char *haystack, char needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	char	*findLastIgnoringCase(char *haystack,
								char needle);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const char	*findFirstOfSet(const char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static char		*findFirstOfSet(char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const char	*findFirstOfSetOrEnd(
							const char *haystack,
							const char *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static char		*findFirstOfSetOrEnd(
							char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const char	*findLastOfSet(const char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static char		*findLastOfSet(char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const char	*findLastOfSetOrEnd(
							const char *haystack,
							const char *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static char		*findLastOfSetOrEnd(
							char *haystack,
							const char *set);

		/** Where "string" is presumed to contain a quoted string
		 *  (eg. "'hello'"), the first character of which is an opening
		 *  quote, this method returns a pointer to the position in the
		 *  string, immediately following the closing quote.
		 *
		 *  "quote" specifies the character to interpret as a quote,
		 *  eg. a single-quote, double-quote, back-quote, etc. but may
		 *  be any character.  Note that the first character of
		 *  "string" is not validated to be "quote".
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static char	*findEndOfQuotedString(char *string,
							char quote,
							bool backslash,
							bool doubling);

		/** Where "string" is presumed to contain a quoted string
		 *  (eg. "'hello'"), the first character of which is an opening
		 *  quote, this method returns a pointer to the position in the
		 *  string, immediately following the closing quote.
		 *
		 *  "quote" specifies the character to interpret as a quote,
		 *  eg. a single-quote, double-quote, back-quote, etc. but may
		 *  be any character.  Note that the first character of
		 *  "string" is not validated to be "quote".
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static const char	*findEndOfQuotedString(
							const char *string,
							char quote,
							bool backslash,
							bool doubling);

		/** Where "string", of length "stringlen", is presumed to
		 *  contain a quoted string (eg. "'hello'"), the first
		 *  character of which is an opening quote, this method
		 *  returns a pointer to the position in the string,
		 *  immediately following the closing quote.
		 *
		 *  "quote" specifies the character to interpret as a quote,
		 *  eg. a single-quote, double-quote, back-quote, etc. but may
		 *  be any character.  Note that the first character of
		 *  "string" is not validated to be "quote".
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static char	*findEndOfQuotedString(char *string,
							uint64_t stringlen,
							char quote,
							bool backslash,
							bool doubling);

		/** Where "string", of length "stringlen", is presumed to
		 *  contain a quoted string (eg. "'hello'"), the first
		 *  character of which is an opening quote, this method
		 *  returns a pointer to the position in the string,
		 *  immediately following the closing quote.
		 *
		 *  "quote" specifies the character to interpret as a quote,
		 *  eg. a single-quote, double-quote, back-quote, etc. but may
		 *  be any character.  Note that the first character of
		 *  "string" is not validated to be "quote".
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static const char	*findEndOfQuotedString(
							const char *string,
							uint64_t stringlen,
							char quote,
							bool backslash,
							bool doubling);

		/** Where "string" is presumed to contain a quoted string
		 *  (eg. "'hello'"), the first character of which is an opening
		 *  quote, this method returns a pointer to the position in the
		 *  string, immediately following the closing quote.
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  The character to interpret as a quote is determined by
		 *  the first character in the string.
		 *
		 *  Note, that in the case of the first character being an open
		 *  square bracket, the closing quote is set to a closing square
		 *  bracket, and doubling is disabled, independent of the value
		 *  of the parameter "doubling".
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static char	*findEndOfQuotedString(char *string,
							bool backslash,
							bool doubling);

		/** Where "string" is presumed to contain a quoted string
		 *  (eg. "'hello'"), the first character of which is an opening
		 *  quote, this method returns a pointer to the position in the
		 *  string, immediately following the closing quote.
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  The character to interpret as a quote is determined by
		 *  the first character in the string.
		 *
		 *  Note, that in the case of the first character being an open
		 *  square bracket, the closing quote is set to a closing square
		 *  bracket, and doubling is disabled, independent of the value
		 *  of the parameter "doubling".
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static const char	*findEndOfQuotedString(
							const char *string,
							bool backslash,
							bool doubling);

		/** Where "string", of length "stringlen", is presumed to
		 *  contain a quoted string (eg. "'hello'"), the first
		 *  character of which is an opening quote, this method
		 *  returns a pointer to the position in the string,
		 *  immediately following the closing quote.
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  The character to interpret as a quote is determined by
		 *  the first character in the string.
		 *
		 *  Note, that in the case of the first character being an open
		 *  square bracket, the closing quote is set to a closing square
		 *  bracket, and doubling is disabled, independent of the value
		 *  of the parameter "doubling".
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static char	*findEndOfQuotedString(char *string,
							uint64_t stringlen,
							bool backslash,
							bool doubling);

		/** Where "string", of length "stringlen", is presumed to
		 *  contain a quoted string (eg. "'hello'"), the first
		 *  character of which is an opening quote, this method
		 *  returns a pointer to the position in the string,
		 *  immediately following the closing quote.
		 *
		 *  If "backslash" is true then backslash-escaping is
		 *  considered.  If "doubling" is true then doubling to
		 *  represent a literal quote is considered.
		 *
		 *  The character to interpret as a quote is determined by
		 *  the first character in the string.
		 *
		 *  Note, that in the case of the first character being an open
		 *  square bracket, the closing quote is set to a closing square
		 *  bracket, and doubling is disabled, independent of the value
		 *  of the parameter "doubling".
		 *
		 *  Returns NULL if "string" is NULL.  Returns a pointer to
		 *  "string" if "stringlen" is 0.  If no closing quote is found
		 *  then it returns a pointer to the end of the string. */
		static const char	*findEndOfQuotedString(
							const char *string,
							uint64_t stringlen,
							bool backslash,
							bool doubling);

		/** Returns the string from the beginning of "str" until
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static char	*isBefore(const char *str,
						const char *delimiter);

		/** Returns the string between "start" and "end".
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static char	*isBetween(const char *str,
						const char *start,
						const char *end);

		/** Returns the string from the "delimiter" until the end
		 *  of the string.
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static char	*isAfter(const char *str,
						const char *delimiter);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters in "set". */
		static size_t	getLengthContainingSet(const char *haystack,
							const char *set);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters not in "set". */
		static size_t	getLengthNotContainingSet(const char *haystack,
							const char *set);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.  Note that this method allocates a buffer for
		 *  the duplicate string internally and returns it.  The
		 *  calling program must deallocate this buffer. */
		static	char	*duplicate(const char *str);

		/** Creates a duplicate of the first "len" characters of
		 *  "str" and returns a pointer to it.  Note that this
		 *  method allocates a buffer for the duplicate string
		 *  internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicate(const char *str, size_t len);

		/** Creates a duplicate of "str", converting each wide character
 		 *  to an ASCII character, replacing characters that can't be
 		 *  represented in ASCII with an ASCII '?', and returns a
 		 *  pointer to it.  Note that this method allocates a buffer
 		 *  for the duplicate string internally and returns it.  The
 		 *  calling program must deallocate this buffer. */
		static	char	*duplicate(const wchar_t *str);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented '?', and returns a
		 *  pointer to it.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicate(const wchar_t *str, size_t len);

		/** Creates a duplicate of "str", converting each wide character
		 *  to a character per the character set of the current locale,
		 *  replacing characters that can't be represented with
		 *  "replacement", and returns a pointer to it.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicate(const wchar_t *str,
							char replacement);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented with "replacement",
		 *  and returns a pointer to it.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicate(const wchar_t *str, size_t len,
							char replacement);

		/** Creates a duplicate of "str", converting each UCS-2
 		 *  character to an ASCII character, replacing characters that
 		 *  can't be represented in ASCII with an ASCII '?', and returns
 		 *  a pointer to it.
 		 *
 		 *  The UCS-2 characters in "str" are presumed to be in the
 		 *  byte-order of the system.
 		 *
 		 *  Note that this method allocates a buffer for the duplicate
 		 *  string internally and returns it.  The calling program must
 		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str);

		/** Creates a duplicate of the first "len" UCS-2 characters of
		 *  "str", converting each UCS-2 character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented '?', and returns a
		 *  pointer to it.
 		 *
 		 *  The UCS-2 characters in "str" are presumed to be in the
 		 *  byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str, size_t len);

		/** Creates a duplicate of "str", converting each UCS-2
 		 *  character to an ASCII character, replacing characters that
 		 *  can't be represented in ASCII with an ASCII '?', and returns
 		 *  a pointer to it.
 		 *
 		 *  If "bigendian" is true then "str" is presumed to be big
		 *  endian.  If "bigendian" is false, then "str" is presumed to
		 *  be little endian.
 		 *
 		 *  Note that this method allocates a buffer for the duplicate
 		 *  string internally and returns it.  The calling program must
 		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							bool bigendian);

		/** Creates a duplicate of the first "len" UCS-2 characters of
		 *  "str", converting each UCS-2 character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented '?', and returns a
		 *  pointer to it.
 		 *
 		 *  If "bigendian" is true then "str" is presumed to be big
		 *  endian.  If "bigendian" is false, then "str" is presumed to
		 *  be little endian.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							size_t len,
							bool bigendian);

		/** Creates a duplicate of "str", converting each UCS-2
		 *  character to a character per the character set of the
		 *  current locale, replacing characters that can't be
		 *  represented with "replacement", and returns a pointer to it.
 		 *
 		 *  The UCS-2 characters in "str" are presumed to be in the
 		 *  byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							char replacement);

		/** Creates a duplicate of the first "len" UCS-2 characters of
		 *  "str", converting each UCS-2 character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented with "replacement",
		 *  and returns a pointer to it.
 		 *
 		 *  The UCS-2 characters in "str" are presumed to be in the
 		 *  byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							size_t len,
							char replacement);

		/** Creates a duplicate of "str", converting each UCS-2
		 *  character to a character per the character set of the
		 *  current locale, replacing characters that can't be
		 *  represented with "replacement", and returns a pointer to it.
 		 *
 		 *  If "bigendian" is true then "str" is presumed to be big
		 *  endian.  If "bigendian" is false, then "str" is presumed to
		 *  be little endian.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							char replacement,
							bool bigendian);

		/** Creates a duplicate of the first "len" UCS-2 characters of
		 *  "str", converting each UCS-2 character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented with "replacement",
		 *  and returns a pointer to it.
 		 *
 		 *  If "bigendian" is true then "str" is presumed to be big
		 *  endian.  If "bigendian" is false, then "str" is presumed to
		 *  be little endian.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	char	*duplicateUcs2(const ucs2_t *str,
							size_t len,
							char replacement,
							bool bigendian);

		/** Converts "str" to uppercase.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	void	upper(char *str); 

		/** Converts "str" to lowercase.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	void	lower(char *str); 

		/** Captitalizes "str", converting characters to upper
		 *  or lower case as necessary.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static void	capitalize(char *str);

		/** Trims all spaces off of the right hand side of "str". */
		static	void	rightTrim(char *str);

		/** Trims all "character"'s off of the right hand side
		 *  of "str". */
		static	void	rightTrim(char *str, char character);

		/** Trims all spaces off of the left hand side of "str". */
		static	void	leftTrim(char *str);

		/** Trims all "character"'s off of the left hand side
		 *  of "str". */
		static	void	leftTrim(char *str, char character);

		/** Trims all spaces off of both sides of "str". */
		static	void	bothTrim(char *str);

		/** Trims all characters off of both sides of "str". */
		static	void	bothTrim(char *str, char character);

		/** Strips all instances of "character" from "str".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(char *str, char character);

		/** Strips all instances of "str2" from "str1".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(char *str1, const char *str2);

		/** Strips all instances of any character in "set" from
		 *  "str1".  Returns true if any characters were stripped
		 *  and false if no characters were stripped. */
		static	bool	stripSet(char *str1, const char *set);

		/** Replaces all instances of "oldchar"
		 *  in "str" with "newchar" */
		static	void	replace(char *str,
					char oldchar, char newchar);

		/** Replaces all instances of any of the characters in
		 *  the set "oldchar" in "str" with "newchar" */
		static	void	replace(char *str,
					const char *oldchars, char newchar);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of "oldstr" have been replaced with
		 *  "newstr" */
		static	char	*replace(const char *str,
						const char *oldstr,
						const char *newstr);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset" */
		static	char	*replace(const char *str,
						const char * const *oldstrset,
						const char * const *newstrset);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", the number of characters of which are
		 *  given by the array "oldstrlen", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset" */
		static	char	*replace(const char *str,
						const char * const *oldstrset,
						size_t *oldstrlen,
						const char * const *newstrset);

		/** Replaces all instances of "oldchar" in "str" with
		 *  "newchar", ignoring case */
		static	void	replaceIgnoringCase(char *str,
					char oldchar, char newchar);

		/** Replaces all instances of any of the characters in the set
		 *  "oldchar" in "str" with "newchar", ignoring case */
		static	void	replaceIgnoringCase(char *str,
					const char *oldchars, char newchar);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of "oldstr" have been replaced with
		 *  "newstr", ignoring case */
		static	char	*replaceIgnoringCase(const char *str,
							const char *oldstr,
							const char *newstr);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset", ignoring case */
		static	char	*replaceIgnoringCase(const char *str,
						const char * const *oldstrset,
						const char * const *newstrset);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", the number of characters of which are
		 *  given by the array "oldstrlen", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset", ignoring set */
		static	char	*replaceIgnoringCase(const char *str,
						const char * const *oldstrset,
						size_t *oldstrlen,
						const char * const *newstrset);

		/** Returns a new string which is a copy of "str" in which
		 *  parts that match "from" have been replaced with "to".
		 *
		 *  If "global" is true then all matching parts are
		 *  replaced.  If "global" is false then only the first
		 *  matching part is replaced. */
		static	char	*replace(const char *str,
						regularexpression *from,
						const char *to,
						bool global);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(int16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(int32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(int64_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(uint16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(uint32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	getIntegerLength(uint64_t number);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const char *val);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const char *val, int32_t len);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const char *val);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const char *val, int32_t len);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(int64_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(uint64_t number,
						uint16_t zeropadding);


		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(float number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(double number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	char	*parseNumber(long double number,
						uint16_t precision,
						uint16_t scale);

		/** Converts "string" to a 64-bit integer. */
		static	int64_t	convertToInteger(const char *string);

		/** Converts "string" to a 64-bit integer.  If non-NULL,
		 *  endptr will be set to the first character in the
		 *  string after the number. */
		static	int64_t	convertToInteger(const char *string,
						const char **endptr);

		/** Converts "string" to a 64-bit integer of base "base". */
		static	int64_t	convertToInteger(const char *string,
							int32_t base);

		/** Converts "string" to a 64-bit integer of base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	int64_t	convertToInteger(const char *string,
					const char **endptr, int32_t base);

		/** Converts "string" to a 64-bit unsigned integer. */
		static	uint64_t	convertToUnsignedInteger(
							const char *string);

		/** Converts "string" to a 64-bit unsigned integer.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	uint64_t	convertToUnsignedInteger(
							const char *string,
							const char **endptr);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base". */
		static	uint64_t	convertToUnsignedInteger(
							const char *string,
							int32_t base);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	uint64_t	convertToUnsignedInteger(
							const char *string,
							const char **endptr,
							int32_t base);


		/** Converts "string" to a floating point number. */
		static	long double	convertToFloat(const char *string);

		/** Converts "string" to a floating point number.
		 *
		 *  If "string" uses a "." as a decimal point (per the "C"
		 *  locale) then the "." is first converted to the appropriate
		 *  decimal-delimiter for the current locale.
		 *
		 *  (Currently only supported on linux/unix platforms that
		 *  provide the locale.h header.  On other platforms, it just
		 *  falls through to convertToFloat().)
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	long double	convertToFloatC(const char *string);

		/** Converts "string" to a floating point number.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	long double	convertToFloat(const char *string,
							const char **endptr);

		/** Converts "amount" which is assumed to be a dollar amount
		 *  into pennies.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	int64_t	convertAmount(const char *amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	char	*convertAmount(int64_t amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string where there
 		 *  are "padding" places between the dollar sign and decimal
 		 *  point.   These will be space padded if the amount is
 		 *  small enough not to fill them.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	char	*convertAmount(int64_t amount,
						uint16_t padding);


		/** escapes all characters in "characters" found in
		 *  "input" using \'s and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*escape(const char *input,
					const char *characters);

		/** unescapes all \-escaped characters found in
		 *  "input" and returns the result in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*unescape(const char *input);

		/** similar to escape() above, but takes an "inputlen"
		 *  parameter and returns the result in "output" and
		 *  "outputlen" rather than in a return value */
		static	void	escape(const char *input,
						uint64_t inputlen,
						char **output,
						uint64_t *outputlen,
						const char *characters);

		/** similar to unescape() above, but takes an "inputlen"
		 *  parameter and returns the result in "output" and
		 *  "outputlen" rather than in a return value */
		static	void	unescape(const char *input,
						uint64_t inputlen,
						char **output,
						uint64_t *outputlen);

		/** URL-encodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*urlEncode(const char *input);

		/** URL-decodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*urlDecode(const char *input);

		/** base64-encodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*base64Encode(const byte_t *input);

		/** similar to base64Encode above but only encodes
		 *  the first "inputsize" bytes of "input" */
		static	char	*base64Encode(const byte_t *input,
							uint64_t inputsize);

		/** similar to base64Encode() above, but returns the
		 *  result in "output" and "outputlen" (in characters)
		 *  rather than in a return value */
		static	void	base64Encode(const byte_t *input,
							uint64_t inputsize,
							char **output,
							uint64_t *outputlen);

		/** base64-decodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	byte_t	*base64Decode(const char *input);

		/** similar to base64Decode above but only decodes
		 *  the first "inputlen" characters of "input" */
		static	byte_t	*base64Decode(const char *input,
							uint64_t inputlen);

		/** similar to base64Decode() above, but returns the
		 *  result in "output" and "outputsize" (in bytes) rather
		 *  than in a return value */
		static	void	base64Decode(const char *input,
						uint64_t inputlen,
						byte_t **output,
						uint64_t *outputsize);

		/** Quoted-printable-encodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*quotedPrintableEncode(
						const byte_t *input);

		/** similar to quotedPrintableEncode above but only encodes
		 *  the first "inputsize" bytes of "input" */
		static	char	*quotedPrintableEncode(
						const byte_t *input,
						uint64_t inputsize);

		/** similar to quotedPrintableEncode() above, but returns the
		 *  result in "output" and "outputlen" (in characters)
		 *  rather than in a return value */
		static	void	quotedPrintableEncode(
						const byte_t *input,
						uint64_t inputsize,
						char **output,
						uint64_t *outputlen);

		/** Quoted-printable-decodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	byte_t	*quotedPrintableDecode(const char *input);

		/** similar to quotedPrintableDecode above but only decodes
		 *  the first "inputlen" characters of "input" */
		static	byte_t	*quotedPrintableDecode(const char *input,
							uint64_t inputlen);

		/** similar to quotedPrintableDecode() above, but returns the
		 *  result in "output" and "outputlen" (in characters) rather
		 *  than in a return value */
		static	void	quotedPrintableDecode(const char *input,
							uint64_t inputlen,
							byte_t **output,
							uint64_t *outputlen);


		/** hex-encodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	char	*hexEncode(const byte_t *input);

		/** similar to hexEncode above but only encodes
		 *  the first "inputsize" bytes of "input" */
		static	char	*hexEncode(const byte_t *input,
							uint64_t inputsize);

		/** similar to hexEncode() above, but returns the
		 *  result in "output" and "outputlen" (in characters) rather
		 *  than in a return value */
		static	void	hexEncode(const byte_t *input,
						uint64_t inputsize,
						char **output,
						uint64_t *outputlen);

		/** hex-decodes "input" and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	byte_t	*hexDecode(const char *input);

		/** similar to hexDecode above but only decodes
		 *  the first "inputlen" characters of "input" */
		static	byte_t	*hexDecode(const char *input,
							uint64_t inputlen);

		/** similar to hexDecode() above, but returns the
		 *  result in "output" and "outputsize" rather than in a
		 *  return value */
		static	void	hexDecode(const char *input,
						uint64_t inputlen,
						byte_t **output,
						uint64_t *outputsize);

		/** Obfuscates "str" by adding 128 to each character. */
		static void	obfuscate(char *str);

		/** Deobfuscates "str" which was obfusacted using the
		 *  obfuscate method of this class. */
		static void	deobfuscate(char *str);

		/** Moves leading spaces to the end of "str" for
		 *  "len" characters.
		 *
		 *  Example: "   hello   " -> "hello      " 
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	void	leftJustify(char *str, int32_t len);

		/** Moves trailing spaces to the beginning of "str" for
		 *  "len" characters.
		 *  
		 *  Example: "   hello   " -> "      hello"
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	void	rightJustify(char *str, int32_t len);

		/** Centers the text of "str" for "len" characters.
		 *
		 *  Example: "hello      " -> "   hello   "
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using ASCII or extended ASCII, and may give unexpected
		 *  results for differently-encoded strings. */
		static	void	center(char *str, int32_t len);

		/** Returns a copy of "string", padded with "padchar" to a
		 *  character length of "totallen".  Set "direction"
		 *  to -1 to left-pad, 0 to center-pad and 1 to right-pad.
		 *  Note that this method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate this
		 *  buffer. */
		static char	*pad(const char *string,
						char padchar,
						int16_t direction,
						uint64_t totallen);

		/** Parses "string" of character length "stringlen"
		 *  delimited by "delimiter" of character length
		 *  "delimiterlen" and allocates "listlen" elements of
		 *  "list" to return the parts.  If "collapse" is true then
		 *  multiple instances of the delimiter in a row will be
		 *  interpreted as one instance of the delimiter.  Each member
		 *  of "list" and "list" itseslf must be deallocated by the
		 *  calling program. */
		static void	split(const char *string,
					size_t stringlen,
					const char *delimiter,
					size_t delimiterlen,
					bool collapse,
					char ***list,
					uint64_t *listlen);

		/** Parses NULL-terminated "string" delimited by "delimiter" of
		 *  character length "delimiterlen" and allocates
		 *  "listlen" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const char *string,
					const char *delimiter,
					size_t delimiterlen,
					bool collapse,
					char ***list,
					uint64_t *listlen);

		/** Parses "string" of character length "stringlen"
		 *  delimited by NULL-terminated "delimiter" and allocates
		 *  "listlen" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const char *string,
					size_t stringlen,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlen);

		/** Parses NULL-terminated "string" delimited by
		 *  NULL-terminated "delimiter" and allocates "listlen"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const char *string,
					const char *delimiter,
					bool collapse,
					char ***list,
					uint64_t *listlen);

		/** Returns a copy of the segment of "str"
		 *  between string indices "start" and "end",
		 *  inclusive. */
		static char	*getSubString(const char *str,
						size_t start, size_t end);

		/** Returns a copy of the segment of "str"
		 *  between string index "start" and the end
		 *  of the string, inclusive. */
		static char	*getSubString(const char *str, size_t start);

		/** Creates a new string with "src" inserted into "dest" at
                 *  "index". */
		static char	*insertString(const char *dest,
						const char *src,
						uint64_t index);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  Where 1K = 1024. */
		static char	*getHumanReadable(int64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static char	*getHumanReadable(int64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static char	*getHumanReadable(uint64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static char	*getHumanReadable(uint64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static char	*getHumanReadable(long double number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static char	*getHumanReadable(long double number,
							bool onethousand);

		/** Appends "..." to "buffer" of character length "len" using
		 *  "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.  If string wasn't large
		 *  enough to accommodate the data then it only writes
		 *  "len" characters to "buffer" but still returns the number
		 *  of characters that would have been written had "buffer"
		 *  been large enough.
		 *
		 *  If "buffer" is NULL or "len" is 0 then nothing is written
		 *  to "buffer" but the number of characters that would have
		 *  been written had "buffer" been large enough is still
		 *  returned.
		 *
		 *  Returns -1 if an error occurred.
		 *
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of characters of the currently selected character
		 *  set (eg. selected by the locale) which may be ASCII,
		 *  extended ASCII (eg. Latin 15), or multibyte (eg. UTF8). */
		static ssize_t	printf(char *buffer, size_t len,
						const char *format, ...);

		/** Appends "argp" to "buffer" of character length "len" using
		 *  "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.  If "buffer" wasn't large
		 *  enough to accommodate the data then it only writes
		 *  "len" characters to "buffer" but still returns the number
		 *  of characters that would have been written had "buffer"
		 *  been large enough.
		 *
		 *  If "buffer" is NULL or "len" is 0 then nothing is written
		 *  to "buffer" but the number of characters that would have
		 *  been written had "buffer" been large enough is still
		 *  returned.
		 *
		 *  Returns -1 if an error occurred.
		 *
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of characters of the currently selected character
		 *  set (eg. selected by the locale) which may be ASCII,
		 *  extended ASCII (eg. Latin 15), or multibyte (eg. UTF8). */
		static ssize_t	printf(char *buffer, size_t len,
						const char *format,
						va_list *argp);

		/** Allocates "buffer" to sufficient size and writes "..." to
		 *  it using "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.
		 *
		 *  If "buffer" is NULL then nothing is written to "buffer" and
		 *  0 is returned.
		 *
		 *  Returns -1 if an error occurred.
		 *
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of characters of the currently selected character
		 *  set (eg. selected by the locale) which may be ASCII,
		 *  extended ASCII (eg. Latin 15), or multibyte (eg. UTF8). */
		static ssize_t	printf(char **buffer,
						const char *format, ...);

		/** Allocates "buffer" to sufficient size and writes "argp" to
		 *  it using "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.
		 *
		 *  If "buffer" is NULL then nothing is written to "buffer" and
		 *  0 is returned.
		 *
		 *  Returns -1 if an error occurred.
		 *
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of characters of the currently selected character
		 *  set (eg. selected by the locale) which may be ASCII,
		 *  extended ASCII (eg. Latin 15), or multibyte (eg. UTF8). */
		static ssize_t	printf(char **buffer,
						const char *format,
						va_list *argp);

	#include <rudiments/private/charstring.h>
};

#endif
