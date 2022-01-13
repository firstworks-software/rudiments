// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WCHARSTRING_H
#define RUDIMENTS_WCHARSTRING_H

#include <rudiments/private/wcharstringincludes.h>

/** The wcharstring class provides static methods for manipulating C-style
 *  wide character strings.
 * 
 *  In addition to some unique methods, analogs for the standard C wide string
 *  functions are provided.  However, unlike the standard C wide string
 *  functions, the wcharstring methods are NULL safe.  Your application will
 *  not crash if a NULL is passed in, and instead, will give intuitive
 *  results. */
class RUDIMENTS_DLLSPEC wcharstring {
	public:

		/** Returns the number of characters in "string". */
		static	size_t	length(const wchar_t *string);

		/** Returns the number of bytes in "string", including the
		 *  null terminator. */
		static	size_t	size(const wchar_t *string);

		/** Returns true if "string" is NULL or the empty string. */
		static	bool	isNullOrEmpty(const wchar_t *string);

		/** Returns true if "string" starts with the standalone "word":
		 *  "Yes" (case-insensitive), "True" (case-insensitive), 
		 *  "Y" (case-insensitive), "T" (case-insensitive), or
		 *  "1" and false otherwise.
		 *
		 *  (Eg. returns true for "Yes,", "True ", and "1 2 3" but
		 *  returns false for "Yesterday", "Truest", and "123") */
		static	bool	isYes(const wchar_t *string);

		/** Returns true if "string" starts with the standalone word:
		 *  "No" (case-insensitive), "False" (case-insensitive), or
		 *  "0" and false otherwise.
		 *
		 *  (Eg. returns true for "No,", "False ", and "0 1 2" but
		 *  returns false for "Nothing", "Falsest", and "012") */
		static	bool	isNo(const wchar_t *string);

		/** Sets "len" characters of "str" to NULL. */
		static	void	zero(wchar_t *str, size_t len);

		/** Appends "source" to "dest".  Assumes that there is
		 *  enough room remaining in "dest" to accommodate the new
		 *  string.  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, const wchar_t *source);

		/** Appends "size" bytes of "source" to "dest".  Assumes
		 *  that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, const wchar_t *source,
								size_t size);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, int64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, uint64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, double number);

		/** Converts "number" to a string using "scale" and appends it
		 *  to "dest".  Assumes that there is enough room remaining in
		 *  "dest" to accommodate the new string.  Returns a pointer to
		 *  "dest". */
		static	wchar_t	*append(wchar_t *dest, double number,
							uint16_t scale);

		/** Converts "number" to a string using "precision" and "scale"
		 *  and appends it to "dest".  Assumes that there is enough
		 *  room remaining in "dest" to accommodate the new string.
		 *  Returns a pointer to "dest". */
		static	wchar_t	*append(wchar_t *dest, double number,
						uint16_t precision,
						uint16_t scale);

		/** Replaces "dest" with "source".  Assumes that there is
		 *  enough room in "dest" to accommodate "source".  Returns a
		 *  pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, const wchar_t *source);

		/** Replaces "dest" with "source".  Assumes that there is
		 *  enough room in "dest" to accommodate "source".  Returns a
		 *  pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, const char *source);

		/** Replaces the first "size" bytes of "dest" with "source".
		 *  Assumes that "dest" is at least "size" bytes long.
		 *  Returns a pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest,
					const wchar_t *source, size_t size);

		/** Replaces the first "size" bytes of "dest" with "source".
		 *  Assumes that "dest" is at least "size" bytes long.
		 *  Returns a pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest,
					const char *source, size_t size);

		/** Replaces "dest" with "source", starting "location" bytes
		 *  into "dest".  Assumes that there is enough room in "dest"
		 *  (after "location" bytes) to accommodate "source".  Returns
		 *  a pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, size_t location,
						const wchar_t *source);

		/** Replaces "dest" with "source", starting "location" bytes
		 *  into "dest".  Assumes that there is enough room in "dest"
		 *  (after "location" bytes) to accommodate "source".  Returns
		 *  a pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, size_t location,
						const char *source);

		/** Replaces "size" bytes of "dest" with "source", starting
		 *  "location" bytes into "dest".  Assumes that there are
		 *  "size" bytes in "dest" (after "location" bytes).  Returns a
		 *  pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, size_t location,
					const wchar_t *source, size_t size);

		/** Replaces "size" bytes of "dest" with "source", starting
		 *  "location" bytes into "dest".  Assumes that there are
		 *  "size" bytes in "dest" (after "location" bytes).  Returns a
		 *  pointer to "dest". */
		static	wchar_t	*copy(wchar_t *dest, size_t location,
					const char *source, size_t size);

		/** Replaces "dest" with "source" unless the character length
		 *  of "source" is greater than "destlen", in which case only
		 *  "destlen" bytes of "dest" will be replaced.  Returns a
		 *  pointer to "dest". */
		static wchar_t	*safeCopy(wchar_t *dest, size_t destlen,
							const wchar_t *source);

		/** Replaces "dest" with "source" unless the character length
		 *  of "source" is greater than "destlen", in which case only
		 *  "destlen" bytes of "dest" will be replaced.  Returns a
		 *  pointer to "dest". */
		static wchar_t	*safeCopy(wchar_t *dest, size_t destlen,
							const char *source);

		/** Replaces "sourcesize" characters of "dest" with "source"
		 *  unless "sourcesize" is greater than "destlen", in which
		 *  case only "destlen" characters of "dest" will be replaced.
		 *  Returns a pointer to "dest". */
		static wchar_t	*safeCopy(wchar_t *dest,
						size_t destlen,
						const wchar_t *source,
						size_t sourcesize);

		/** Replaces "sourcesize" characters of "dest" with "source"
		 *  unless "sourcesize" is greater than "destlen", in which
		 *  case only "destlen" characters of "dest" will be replaced.
		 *  Returns a pointer to "dest". */
		static wchar_t	*safeCopy(wchar_t *dest,
						size_t destlen,
						const char *source,
						size_t sourcesize);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2". */
		static	int32_t	compare(const wchar_t *str1,
						const wchar_t *str2);

		/** Returns -1,0 or 1 if "size" bytes of "str1" are greater
		 * than, equal to or less than "size" bytes of "str2". */
		static	int32_t	compare(const wchar_t *str1,
						const wchar_t *str2,
						size_t size);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", ignoring case. */
		static	int32_t	compareIgnoringCase(const wchar_t *str1,
							const wchar_t *str2);

		/** Returns -1,0 or 1 if "size" bytes of "str1" are greater
		 *  than, equal to or less than "size" bytes of "str2",
		 *  ignoring case. */
		static	int32_t	compareIgnoringCase(const wchar_t *str1,
							const wchar_t *str2,
							size_t size);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2".
		 *
		 * Embedded (version) numbers will be interpreted "naturally"
		 * (eg. 12 > 8) */
		static	int32_t	compareNatural(const wchar_t *str1,
						const wchar_t *str2);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2".
		 *
		 * Embedded (version) numbers will be interpreted "naturally"
		 * (eg. 12 > 8)
		 *
		 * "delimiters" should be set to the set of characters that
		 * will be interpreted as equivalent delimiters.  Eg. "._"
		 * means that either .'s or _'s may be used to delimit
		 * numbers. */
		static	int32_t	compareNatural(const wchar_t *str1,
						const wchar_t *str2,
						const wchar_t *delimiters);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of decimal delimiters. */
		static	int32_t	compareVersions(const wchar_t *str1,
						const wchar_t *str2);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of delimiters.
		 *
		 * "delimiters" should be set to the set of characters that
		 * will be interpreted as equivalent delimiters.  Eg. "._"
		 * means that either .'s or _'s may be used to delimiters
		 * numbers. */
		static	int32_t	compareVersions(const wchar_t *str1,
						const wchar_t *str2,
						const wchar_t *delimiters);

		/** Compares the first "stringlength" characters of "string"
		 *  to the first "patternlength" characters of "pattern" which
		 *  may contain instances of single-character wildcard markers
		 *  "singlewildcard" or multi-character wildcard markers
		 *  "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const wchar_t *string,
							size_t stringlength,
							const wchar_t *pattern,
							size_t patternlength,
							wchar_t singlewildcard,
							wchar_t multiwildcard);

		/** Compares "string" to the first "patternlength" characters
		 *  of "pattern" which may contain instances of single-character
		 *  wildcard markers "singlewildcard" or multi-character
		 *  wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const wchar_t *string,
							const wchar_t *pattern,
							size_t patternlength,
							wchar_t singlewildcard,
							wchar_t multiwildcard);

		/** Compares the first "stringlength" characters of "string"
		 *  to "pattern" which may contain instances of
		 *  single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const wchar_t *string,
							size_t stringlength,
							const wchar_t *pattern,
							wchar_t singlewildcard,
							wchar_t multiwildcard);

		/** Compares "string" to "pattern" which may contain instances
		 *  of single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const wchar_t *string,
							const wchar_t *pattern,
							wchar_t singlewildcard,
							wchar_t multiwildcard);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set".  Also returns true if "str"
		 *  is NULL and set is NULL or contains only a NULL-terminator.
		 *  Otherwise returns false. */
		static	bool	inSet(const wchar_t *str,
					const wchar_t * const *set);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set", ignoring case.  Also returns
		 *  true if "str" is NULL and set is NULL or contains only a
		 *  NULL-terminator.  Otherwise returns false. */
		static	bool	inSetIgnoringCase(const wchar_t *str,
						const wchar_t * const *set);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const wchar_t *haystack,
							wchar_t needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const wchar_t *haystack,
								wchar_t needle);

		/** Returns true if "haystack" starts with "needle" or
		 *  false otherwise. */
		static	bool	startsWith(const wchar_t *haystack,
						const wchar_t *needle);

		/** Returns true if "haystack" ends with "needle" or
		 *  false otherwise. */
		static	bool	endsWith(const wchar_t *haystack,
						const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const wchar_t	*findFirst(const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const wchar_t	*findFirstIgnoringCase(
							const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const wchar_t	*findFirst(const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const wchar_t	*findFirstIgnoringCase(
							const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const wchar_t	*findFirstOrEnd(const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const wchar_t	*findFirstOrEndIgnoringCase(
							const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const wchar_t	*findFirstOrEnd(const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const wchar_t	*findFirstOrEndIgnoringCase(
							const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const wchar_t	*findLast(const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const wchar_t	*findLast(const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	wchar_t	*findFirst(wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	wchar_t	*findFirstIgnoringCase(wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	wchar_t	*findFirst(wchar_t *haystack, wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	wchar_t	*findFirstIgnoringCase(wchar_t *haystack,
								wchar_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	wchar_t	*findFirstOrEnd(wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	wchar_t	*findFirstOrEndIgnoringCase(wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	wchar_t	*findLast(wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const wchar_t	*findLastIgnoringCase(
							const wchar_t *haystack,
							const wchar_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	wchar_t	*findLast(wchar_t *haystack, wchar_t needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const wchar_t	*findLastIgnoringCase(
							const wchar_t *haystack,
							wchar_t needle);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const wchar_t	*findFirstOfSet(const wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static wchar_t		*findFirstOfSet(wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const wchar_t	*findFirstOfSetOrEnd(
							const wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static wchar_t		*findFirstOfSetOrEnd(
							wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const wchar_t	*findLastOfSet(const wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static wchar_t		*findLastOfSet(wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const wchar_t	*findLastOfSetOrEnd(
							const wchar_t *haystack,
							const wchar_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static wchar_t		*findLastOfSetOrEnd(
							wchar_t *haystack,
							const wchar_t *set);

		/** Returns the string from the beginning of "str" until
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static wchar_t	*before(const wchar_t *str,
						const wchar_t *delimiter);

		/** Returns the string between "start" and "end".
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static wchar_t	*between(const wchar_t *str,
						const wchar_t *start,
						const wchar_t *end);

		/** Returns the string from the "delimiter" until the end
		 *  of the string.
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static wchar_t	*after(const wchar_t *str,
						const wchar_t *delimiter);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters in "set". */
		static size_t	lengthContainingSet(const wchar_t *haystack,
							const wchar_t *set);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters not in "set". */
		static size_t	lengthNotContainingSet(const wchar_t *haystack,
							const wchar_t *set);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.  Note that this method allocates a buffer for
		 *  the duplicate string internally and returns it.  The
		 *  calling program must deallocate this buffer. */
		static	wchar_t	*duplicate(const wchar_t *str);

		/** Creates a duplicate of the first "len" characters of
		 *  "str" and returns a pointer to it.  Note that this
		 *  method allocates a buffer for the duplicate string
		 *  internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	wchar_t	*duplicate(const wchar_t *str, size_t len);

		/** Creates a duplicate of the first "len" characters of
		 *  "str", converts it to a wide string, and returns a pointer
		 *  to it.  Note that this method allocates a buffer for the
		 *  duplicate string internally and returns it.  The calling
		 *  program must deallocate this buffer. */
		static	wchar_t	*duplicate(const char *str, size_t len);

		/** Creates a duplicate of "str", converts it to a wide string,
		 *  and returns a pointer to it.  Note that this method
		 *  allocates a buffer for the duplicate string internally
		 *  and returns it.  The calling program must deallocate this
		 *  buffer. */
		static	wchar_t	*duplicate(const char *str);

		/** Converts "str" to uppercase. */
		static	void	upper(wchar_t *str); 

		/** Converts "str" to lowercase. */
		static	void	lower(wchar_t *str); 

		/** Captitalizes "str", converting characters to upper
		 *  or lower case as necessary. */
		static void	capitalize(wchar_t *str);

		/** Trims all spaces off of the right hand side of "str". */
		static	void	rightTrim(wchar_t *str);

		/** Trims all "character"'s off of the right hand side
		 *  of "str". */
		static	void	rightTrim(wchar_t *str, wchar_t character);

		/** Trims all spaces off of the left hand side of "str". */
		static	void	leftTrim(wchar_t *str);

		/** Trims all "character"'s off of the left hand side
		 *  of "str". */
		static	void	leftTrim(wchar_t *str, wchar_t character);

		/** Trims all spaces off of both sides of "str". */
		static	void	bothTrim(wchar_t *str);

		/** Trims all characters off of both sides of "str". */
		static	void	bothTrim(wchar_t *str, wchar_t character);

		/** Strips all instances of "character" from "str".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(wchar_t *str, wchar_t character);

		/** Strips all instances of "str2" from "str1".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(wchar_t *str1, const wchar_t *str2);

		/** Strips all instances of any character in "set" from
		 *  "str1".  Returns true if any characters were stripped
		 *  and false if no characters were stripped. */
		static	bool	stripSet(wchar_t *str1, const wchar_t *set);

		/** Replaces all instances of "oldchar"
		 *  in "str" with "newchar" */
		static	void	replace(wchar_t *str,
					wchar_t oldchar,
					wchar_t newchar);

		/** Replaces all instances of any of the characters in
		 *  the set "oldchar" in "str" with "newchar" */
		static	void	replace(wchar_t *str,
					const wchar_t *oldchars,
					wchar_t newchar);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of "oldstr" have been replaced with
		 *  "newstr" */
		static	wchar_t	*replace(const wchar_t *str,
						const wchar_t *oldstr,
						const wchar_t *newstr);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", the number of characters of which are
		 *  given by the array "oldstrlen", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset" */
		static	wchar_t	*replace(const wchar_t *str,
					const wchar_t * const *oldstrset,
					size_t *oldstrlen,
					const wchar_t * const *newstrset);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(int64_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint16_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint32_t number);

		/** Returns the number of characters needed to represent
		 *  "number" as a string. */
		static	uint16_t	integerLength(uint64_t number);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const wchar_t *val);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const wchar_t *val, int32_t size);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const wchar_t *val);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const wchar_t *val, int32_t size);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(int64_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(uint64_t number,
						uint16_t zeropadding);


		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(float number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(float number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(float number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(double number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(long double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(long double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	wchar_t	*parseNumber(long double number,
						uint16_t precision,
						uint16_t scale);

		/** Converts "string" to a 64-bit integer. */
		static	int64_t	toInteger(const wchar_t *string);

		/** Converts "string" to a 64-bit integer.  If non-NULL,
		 *  endptr will be set to the first character in the
		 *  string after the number. */
		static	int64_t	toInteger(const wchar_t *string,
						const wchar_t **endptr);

		/** Converts "string" to a 64-bit integer of base "base". */
		static	int64_t	toInteger(const wchar_t *string, int32_t base);

		/** Converts "string" to a 64-bit integer of base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	int64_t	toInteger(const wchar_t *string,
						const wchar_t **endptr,
						int32_t base);

		/** Converts "string" to a 64-bit unsigned integer. */
		static	uint64_t	toUnsignedInteger(
						const wchar_t *string);

		/** Converts "string" to a 64-bit unsigned integer.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	uint64_t	toUnsignedInteger(const wchar_t *string,
							const wchar_t **endptr);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base". */
		static	uint64_t	toUnsignedInteger(const wchar_t *string,
								int32_t base);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	uint64_t	toUnsignedInteger(const wchar_t *string,
							const wchar_t **endptr,
							int32_t base);


		/** Converts "string" to a floating point number. */
		static	long double	toFloat(const wchar_t *string);

		/** Converts "string" to a floating point number.
		 *
		 *  If "string" uses a "." as a decimal point (per the "C"
		 *  locale) then the "." is first converted to the appropriate
		 *  decimal-delimiter for the current locale.
		 *
		 *  (Currently only supported on linux/unix platforms that
		 *  provide the locale.h header.  On other platforms, it just
		 *  falls through to toFloat().)
		 */
		static	long double	toFloatC(const wchar_t *string);

		/** Converts "string" to a floating point number.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	long double	toFloat(const wchar_t *string,
							const wchar_t **endptr);

		/** Converts "amount" which is assumed to be a dollar amount
		 *  into pennies. */
		static	int64_t	convertAmount(const wchar_t *amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string. */
		static	wchar_t	*convertAmount(int64_t amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string where there
 		 *  are "padding" places between the dollar sign and decimal
 		 *  point.   These will be space padded if the amount is
 		 *  small enough not to fill them.  */
		static	wchar_t	*convertAmount(int64_t amount,
						uint16_t padding);


		/** escapes all characters in "characters" found in
		 *  "input" using \'s and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	wchar_t	*escape(const wchar_t *input,
					const wchar_t *characters);

		/** unescapes all \-escaped characters found in
		 *  "input" and returns the result in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	wchar_t	*unescape(const wchar_t *input);

		/** similar to escape() above, but takes an "inputsize"
		 *  parameter and returns the result in "output" and
		 *  "outputsize" rather than in a return value */
		static	void	escape(const wchar_t *input, uint64_t inputsize,
					wchar_t **output, uint64_t *outputsize,
					const wchar_t *characters);

		/** similar to unescape() above, but takes an "inputsize"
		 *  parameter and returns the result in "output" and
		 *  "outputsize" rather than in a return value */
		static	void	unescape(const wchar_t *input,
						uint64_t inputsize,
						wchar_t **output,
						uint64_t *outputsize);

		/** Moves leading spaces to the end of "str" for
		 *  "len" characters.
		 *
		 *  Example: "   hello   " -> "hello      " */
		static	void	leftJustify(wchar_t *str, int32_t len);

		/** Moves trailing spaces to the beginning of "str" for
		 *  "len" characters.
		 *  
		 *  Example: "   hello   " -> "      hello"  */
		static	void	rightJustify(wchar_t *str, int32_t len);

		/** Centers the text of "str" for "len" characters.
		 *
		 *  Example: "hello      " -> "   hello   "  */
		static	void	center(wchar_t *str, int32_t len);

		/** Returns a copy of "string", padded with "padchar" to a
		 *  character length of "totallength".  Set "direction"
		 *  to -1 to left-pad, 0 to center-pad and 1 to right-pad.
		 *  Note that this method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate this
		 *  buffer. */
		static wchar_t	*pad(const wchar_t *string,
						wchar_t padchar,
						int16_t direction,
						uint64_t totallength);

		/** Parses "string" of character length "stringlength"
		 *  delimited by "delimiter" of character length
		 *  "delimiterlength" and allocates "listlength" elements of
		 *  "list" to return the parts.  If "collapse" is true then
		 *  multiple instances of the delimiter in a row will be
		 *  interpreted as one instance of the delimiter.  Each member
		 *  of "list" and "list" itseslf must be deallocated by the
		 *  calling program. */
		static void	split(const wchar_t *string,
					size_t stringlength,
					const wchar_t *delimiter,
					size_t delimiterlength,
					bool collapse,
					wchar_t ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by "delimiter" of
		 *  character length "delimiterlength" and allocates
		 *  "listlength" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const wchar_t *string,
					const wchar_t *delimiter,
					size_t delimiterlength,
					bool collapse,
					wchar_t ***list,
					uint64_t *listlength);

		/** Parses "string" of character length "stringlength"
		 *  delimited by NULL-terminated "delimiter" and allocates
		 *  "listlength" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const wchar_t *string,
					size_t stringlength,
					const wchar_t *delimiter,
					bool collapse,
					wchar_t ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by
		 *  NULL-terminated "delimiter" and allocates "listlength"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const wchar_t *string,
					const wchar_t *delimiter,
					bool collapse,
					wchar_t ***list,
					uint64_t *listlength);

		/** Returns a copy of the segment of "str"
		 *  between string indices "start" and "end",
		 *  inclusive. */
		static wchar_t	*subString(const wchar_t *str,
						size_t start, size_t end);

		/** Returns a copy of the segment of "str"
		 *  between string index "start" and the end
		 *  of the string, inclusive. */
		static wchar_t	*subString(const wchar_t *str, size_t start);

		/** Creates a new string with "src" inserted into "dest" at
                 *  "index". */
		static wchar_t	*insertString(const wchar_t *dest,
						const wchar_t *src,
						uint64_t index);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  Where 1K = 1024. */
		static wchar_t	*humanReadable(int64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static wchar_t	*humanReadable(int64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static wchar_t	*humanReadable(uint64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static wchar_t	*humanReadable(uint64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static wchar_t	*humanReadable(long double number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static wchar_t	*humanReadable(long double number,
							bool onethousand);

		/** Appends "..." to "buffer" of character length "len" using
		 *  "format" which should comply with standard printf
		 *  formatting rules.
		 *
		 *  Returns the number of characters written to "buffer"
		 *  excluding the NULL terminator.  If string wasn't large
		 *  enough to accommodate the data then it only writes
		 *  "len" bytes to "buffer" but still returns the number
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
		 *  NOTE: This method is unsupported on platforms where
		 *  supportsPrintf() returns false.  On those platforms this
		 *  method returns -1 and sets ENOSYS. */
		static ssize_t	printf(wchar_t *buffer, size_t len,
						const wchar_t *format, ...);

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
		 *  "len" bytes to "buffer" but still returns the number
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
		 *  NOTE: This method is unsupported on platforms where
		 *  supportsPrintf() returns false.  On those platforms this
		 *  method returns -1 and sets ENOSYS. */
		static ssize_t	printf(wchar_t *buffer, size_t len,
						const wchar_t *format,
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
		 *  NOTE: This method is unsupported on platforms where
		 *  supportsPrintf() returns false.  On those platforms this
		 *  method returns -1 and sets ENOSYS. */
		static ssize_t	printf(wchar_t **buffer,
						const wchar_t *format, ...);

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
		 *  NOTE: This method is unsupported on platforms where
		 *  supportsPrintf() returns false.  On those platforms this
		 *  method returns -1 and sets ENOSYS. */
		static ssize_t	printf(wchar_t **buffer,
						const wchar_t *format,
						va_list *argp);

		/** Returns true if the platform supports wide characters and
		 *  rudiments was build with wide character support and false
		 *  otherwise. */
		static bool	supported();

		/** Returns true if the platform supports wide characters and
		 *  rudiments was build with wide character support and false
		 *  otherwise. */
		static bool	supportsPrintf();

	#include <rudiments/private/wcharstring.h>
};

#endif
