// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UCS2CHARSTRING_H
#define RUDIMENTS_UCS2CHARSTRING_H

#include <rudiments/private/ucs2charstringincludes.h>

#define ucs2literal(___ucs2charstrvar,___charstrvar) \
	ucs2_t	___ucs2charstrvar##array[sizeof(___charstrvar)/sizeof(char)]; \
	{ \
		const char	*___charstrptr=___charstrvar; \
		ucs2_t		*___ucs2strptr=___ucs2charstrvar##array; \
		while (*___charstrptr) { \
			*___ucs2strptr=(ucs2_t)(*___charstrptr); \
			___charstrptr++; \
			___ucs2strptr++; \
		} \
		*___ucs2strptr=(ucs2_t)'\0'; \
	} \
	const ucs2_t	*___ucs2charstrvar=___ucs2charstrvar##array

/** The ucs2charstring class provides static methods for manipulating UCS-2
 *  encoded character strings. */
class RUDIMENTS_DLLSPEC ucs2charstring {
	public:

		/** Returns the number of characters in "string". */
		static	size_t	length(const ucs2_t *string);

		/** Returns the number of bytes in "string", including the
		 *  null terminator. */
		static	size_t	size(const ucs2_t *string);

		/** Returns true if "string" is NULL or the empty string. */
		static	bool	isNullOrEmpty(const ucs2_t *string);

		/** Returns true if "string" starts with the standalone "word":
		 *  "Yes" (case-insensitive), "True" (case-insensitive), 
		 *  "Y" (case-insensitive), "T" (case-insensitive), or
		 *  "1" and false otherwise.
		 *
		 *  (Eg. returns true for "Yes,", "True ", and "1 2 3" but
		 *  returns false for "Yesterday", "Truest", and "123") */
		static	bool	isYes(const ucs2_t *string);

		/** Returns true if "string" starts with the standalone word:
		 *  "No" (case-insensitive), "False" (case-insensitive), or
		 *  "0" and false otherwise.
		 *
		 *  (Eg. returns true for "No,", "False ", and "0 1 2" but
		 *  returns false for "Nothing", "Falsest", and "012") */
		static	bool	isNo(const ucs2_t *string);

		/** Sets "len" characters of "str" to NULL.
		 *
		 *  Note that this method expects to operate on strings encoded
		 *  using a single-byte encoding, and will give unexpected
		 *  results for multi-byte-encoded strings. */
		static	void	zero(ucs2_t *str, size_t len);

		/** Appends "source" to "dest".  Assumes that there is
		 *  enough room remaining in "dest" to accommodate the new
		 *  string.  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						const ucs2_t *source);

		/** Appends "len" characters of "source" to "dest".  Assumes
		 *  that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						const ucs2_t *source,
						size_t len);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						int64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						uint64_t number);

		/** Converts "number" to a string and appends it to "dest".
		 *  Assumes that there is enough room remaining in "dest" to
		 *  accommodate the new string.  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						double number);

		/** Converts "number" to a string using "scale" and appends it
		 *  to "dest".  Assumes that there is enough room remaining in
		 *  "dest" to accommodate the new string.  Returns a pointer to
		 *  "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						double number,
						uint16_t scale);

		/** Converts "number" to a string using "precision" and "scale"
		 *  and appends it to "dest".  Assumes that there is enough
		 *  room remaining in "dest" to accommodate the new string.
		 *  Returns a pointer to "dest". */
		static	ucs2_t	*append(ucs2_t *dest,
						double number,
						uint16_t precision,
						uint16_t scale);

		/** Replaces "dest" with "source".  Assumes that there is
		 *  enough room in "dest" to accommodate "source".  Returns a
		 *  pointer to "dest". */
		static	ucs2_t	*copy(ucs2_t *dest,
						const ucs2_t *source);

		/** Replaces the first "length" characters of "dest" with
		 *  "source".  Assumes that "dest" is at least "size" bytes
		 *  long.  Returns a pointer to "dest". */
		static	ucs2_t	*copy(ucs2_t *dest,
						const ucs2_t *source,
						size_t len);

		/** Replaces "dest" with "source", starting "location" bytes
		 *  into "dest".  Assumes that there is enough room in "dest"
		 *  (after "location" bytes) to accommodate "source".  Returns
		 *  a pointer to "dest". */
		static	ucs2_t	*copy(ucs2_t *dest,
						size_t location,
						const ucs2_t *source);

		/** Replaces "len" characters of "dest" with "source",
		 *  starting "location" bytes into "dest".  Assumes that there
		 *  are "size" bytes in "dest" (after "location" bytes).
		 *  Returns a pointer to "dest". */
		static	ucs2_t	*copy(ucs2_t *dest,
						size_t location,
						const ucs2_t *source,
						size_t len);

		/** Replaces "dest" with "source" unless the character length
 		 *  of "source" is greater than "destlen", in which case only
 		 *  "destlen" characters of "dest" will be replaced.  Returns a
 		 *  pointer to "dest". */
		static ucs2_t	*safeCopy(ucs2_t *dest,
						size_t destlen,
						const ucs2_t *source);

		/** Replaces "sourcelen" characters of "dest" with "source"
		 *  unless "sourcelen" is greater than "destlen", in which case
		 *  only "destlen" characters of "dest" will be replaced.
		 *  Returns a pointer to "dest". */
		static ucs2_t	*safeCopy(ucs2_t *dest,
						size_t destlen,
						const ucs2_t *source,
						size_t sourcelen);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2". */
		static	int32_t	compare(const ucs2_t *str1,
						const ucs2_t *str2);

		/** Returns -1,0 or 1 if "length" characters of "str1" are
		 *  greater than, equal to or less than "size" bytes of
		 *  "str2". */
		static	int32_t	compare(const ucs2_t *str1,
						const ucs2_t *str2,
						size_t length);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", ignoring case. */
		static	int32_t	compareIgnoringCase(const ucs2_t *str1,
							const ucs2_t *str2);

		/** Returns -1,0 or 1 if "len" characters of "str1" are greater
		 *  than, equal to or less than "size" bytes of "str2",
		 *  ignoring case. */
		static	int32_t	compareIgnoringCase(const ucs2_t *str1,
							const ucs2_t *str2,
							size_t len);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2".
		 *
		 * Embedded (version) numbers will be interpreted "naturally"
		 * (eg. 12 > 8) */
		static	int32_t	compareNatural(const ucs2_t *str1,
						const ucs2_t *str2);

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
		static	int32_t	compareNatural(const ucs2_t *str1,
						const ucs2_t *str2,
						const ucs2_t *delimiters);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of decimal delimiters. */
		static	int32_t	compareVersions(const ucs2_t *str1,
						const ucs2_t *str2);

		/** Returns -1,0 or 1 if "str1" is greater than, equal to or
		 * less than "str2", where "str1" and "str2" are version
		 * numbers with arbitrary numbers of delimiters.
		 *
		 * "delimiters" should be set to the set of characters that
		 * will be interpreted as equivalent delimiters.  Eg. "._"
		 * means that either .'s or _'s may be used to delimiters
		 * numbers. */
		static	int32_t	compareVersions(const ucs2_t *str1,
						const ucs2_t *str2,
						const ucs2_t *delimiters);

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
		static	bool	compareWithWildcards(const ucs2_t *string,
							size_t stringlength,
							const ucs2_t *pattern,
							size_t patternlength,
							ucs2_t singlewildcard,
							ucs2_t multiwildcard);

		/** Compares "string" to the first "patternlength" characters
		 *  of "pattern" which may contain instances of single-character
		 *  wildcard markers "singlewildcard" or multi-character
		 *  wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const ucs2_t *string,
							const ucs2_t *pattern,
							size_t patternlength,
							ucs2_t singlewildcard,
							ucs2_t multiwildcard);

		/** Compares the first "stringlength" characters of "string"
		 *  to "pattern" which may contain instances of
		 *  single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const ucs2_t *string,
							size_t stringlength,
							const ucs2_t *pattern,
							ucs2_t singlewildcard,
							ucs2_t multiwildcard);

		/** Compares "string" to "pattern" which may contain instances
		 *  of single-character wildcard markers "singlewildcard" or
		 *  multi-character wildcard markers "multiwildcard".
		 *
		 *  If either "singlewildcard" or "multiwildcard" are '\0' then
		 *  they are ignored.
		 *
		 *  Returns true if a match is found or false otherwise. */
		static	bool	compareWithWildcards(const ucs2_t *string,
							const ucs2_t *pattern,
							ucs2_t singlewildcard,
							ucs2_t multiwildcard);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set".  Also returns true if "str"
		 *  is NULL and set is NULL or contains only a NULL-terminator.
		 *  Otherwise returns false. */
		static	bool	isInSet(const ucs2_t *str,
						const ucs2_t * const *set);

		/** Returns true if "str" is found among the values in the
		 *  NULL-terminated array "set", ignoring case.  Also returns
		 *  true if "str" is NULL and set is NULL or contains only a
		 *  NULL-terminator.  Otherwise returns false. */
		static	bool	isInSetIgnoringCase(const ucs2_t *str,
						const ucs2_t * const *set);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns true if "haystack" contains "needle" or
		 *  false otherwise. */
		static	bool	contains(const ucs2_t *haystack,
							ucs2_t needle);

		/** Returns true if "haystack" contains "needle",
		 *  ignoring case, or false otherwise. */
		static	bool	containsIgnoringCase(const ucs2_t *haystack,
							ucs2_t needle);

		/** Returns true if "haystack" starts with "needle" or
		 *  false otherwise. */
		static	bool	startsWith(const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns true if "haystack" ends with "needle" or
		 *  false otherwise. */
		static	bool	endsWith(const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const ucs2_t	*findFirst(const ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const ucs2_t	*findFirstIgnoringCase(
						const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const ucs2_t	*findFirst(const ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const ucs2_t	*findFirstIgnoringCase(
						const ucs2_t *haystack,
						ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const ucs2_t	*findFirstOrEnd(
						const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const ucs2_t	*findFirstOrEndIgnoringCase(
						const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	const ucs2_t	*findFirstOrEnd(
						const ucs2_t *haystack,
						ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	const ucs2_t	*findFirstOrEndIgnoringCase(
						const ucs2_t *haystack,
						ucs2_t needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const ucs2_t	*findLast(const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const ucs2_t	*findLastIgnoringCase(
						const ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	const ucs2_t	*findLast(const ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	const ucs2_t	*findLastIgnoringCase(
						const ucs2_t *haystack,
						ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	ucs2_t	*findFirst(ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	ucs2_t	*findFirstIgnoringCase(
						ucs2_t *haystack,
						const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	ucs2_t	*findFirst(ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	ucs2_t	*findFirstIgnoringCase(
							ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack" or a pointer to the NULL terminator
		 *  at the end of the string if not found. */
		static	ucs2_t	*findFirstOrEnd(ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns a pointer to the first occurrance of "needle"
		 *  in "haystack", ignoring case, or a pointer to the NULL
		 *  terminator at the end of the string if not found. */
		static	ucs2_t	*findFirstOrEndIgnoringCase(
							ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	ucs2_t	*findLast(ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	ucs2_t	*findLastIgnoringCase(
							ucs2_t *haystack,
							const ucs2_t *needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack" or NULL if not found. */
		static	ucs2_t	*findLast(ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the last occurrance of "needle"
		 *  in "haystack", ignoring case, or NULL if not found. */
		static	ucs2_t	*findLastIgnoringCase(
							ucs2_t *haystack,
							ucs2_t needle);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const ucs2_t	*findFirstOfSet(
						const ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static ucs2_t		*findFirstOfSet(ucs2_t *haystack,
							const ucs2_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const ucs2_t	*findFirstOfSetOrEnd(
						const ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns a pointer to the first occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static ucs2_t		*findFirstOfSetOrEnd(
						ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static const ucs2_t	*findLastOfSet(
						const ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or NULL if not found. */
		static ucs2_t		*findLastOfSet(ucs2_t *haystack,
							const ucs2_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set"
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static const ucs2_t	*findLastOfSetOrEnd(
						const ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns a pointer to the last occurence in
		 *  "haystack" of any of the characters in "set".
		 *  or a pointer to the NULL terminator at the end
		 *  of the string if not found. */
		static ucs2_t		*findLastOfSetOrEnd(
						ucs2_t *haystack,
						const ucs2_t *set);

		/** Returns the string from the beginning of "str" until
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static ucs2_t	*before(const ucs2_t *str,
						const ucs2_t *delimiter);

		/** Returns the string between "start" and "end".
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static ucs2_t	*between(const ucs2_t *str,
						const ucs2_t *start,
						const ucs2_t *end);

		/** Returns the string from the "delimiter" until the end
		 *  of the string.
		 *  Note that this method allocates a buffer for the return
		 *  value internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static ucs2_t	*after(const ucs2_t *str,
						const ucs2_t *delimiter);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters in "set". */
		static size_t	lengthContainingSet(const ucs2_t *haystack,
							const ucs2_t *set);

		/** Returns the number of characters, starting at the
		 *  beginning of "haystack" which consists entirely of
		 *  characters not in "set". */
		static size_t	lengthNotContainingSet(const ucs2_t *haystack,
							const ucs2_t *set);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.
		 *
		 *  The return value will be in the byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const char *str);

		/** Creates a duplicate of the first "len" characters of
		 *  "str" and returns a pointer to it.
		 *
		 *  The return value will be in the byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const char *str, size_t len);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const char *str, bool bigendian);

		/** Creates a duplicate of the first "len" characters of
		 *  "str" and returns a pointer to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const char *str,
						size_t len,
						bool bigendian);

		/** Creates a duplicate of "str" and returns a pointer
		 *  to it.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const ucs2_t *str);

		/** Creates a duplicate of the first "len" characters of
		 *  "str" and returns a pointer to it.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const ucs2_t *str, size_t len);

		/** Creates a duplicate "str", converting each wide character
 		 *  to a UCS-2 character, replacing characters that can't be
 		 *  represented in UCS-2 with a UCS-2 '?', and returns a
 		 *  pointer to it.
		 *
		 *  The return value will be in the byte-order of the system.
 		 *
 		 *  Note that this method allocates a buffer for the duplicate
 		 *  string internally and returns it.  The calling program must
 		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented '?', and returns a
		 *  pointer to it.
		 *
		 *  The return value will be in the byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str, size_t len);

		/** Creates a duplicate "str", converting each wide character
 		 *  to a UCS-2 character, replacing characters that can't be
 		 *  represented in UCS-2 with a UCS-2 '?', and returns a
 		 *  pointer to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
 		 *
 		 *  Note that this method allocates a buffer for the duplicate
 		 *  string internally and returns it.  The calling program must
 		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str, bool bigendian);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented '?', and returns a
		 *  pointer to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str, size_t len,
								bool bigendian);

		/** Creates a duplicate of "str", converting each wide character
		 *  to a character per the character set of the current locale,
		 *  replacing characters that can't be represented with
		 *  "replacement", and returns a pointer to it.
		 *
		 *  The return value will be in the byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str,
							ucs2_t replacement);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented with "replacement",
		 *  and returns a pointer to it.
		 *
		 *  The return value will be in the byte-order of the system.
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str,
							size_t len,
							ucs2_t replacement);

		/** Creates a duplicate of "str", converting each wide character
		 *  to a character per the character set of the current locale,
		 *  replacing characters that can't be represented with
		 *  "replacement", and returns a pointer to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str,
							ucs2_t replacement,
							bool bigendian);

		/** Creates a duplicate of the first "len" wide characters of
		 *  "str", converting each wide character to a character per
		 *  the character set of the current locale, replacing
		 *  characters that can't be represented with "replacement",
		 *  and returns a pointer to it.
		 *
		 *  If "bigendian" is true then the return value will be in big
		 *  endian byte-order.  If "bigendian" is false then the return
		 *  value will be in little endian byte-order. 
		 *
		 *  Note that this method allocates a buffer for the duplicate
		 *  string internally and returns it.  The calling program must
		 *  deallocate this buffer. */
		static	ucs2_t	*duplicate(const wchar_t *str,
							size_t len,
							ucs2_t replacement,
							bool bigendian);

		/** Converts "str" to uppercase. */
		static	void	upper(ucs2_t *str); 

		/** Converts "str" to lowercase. */
		static	void	lower(ucs2_t *str); 

		/** Captitalizes "str", converting characters to upper
		 *  or lower case as necessary. */
		static void	capitalize(ucs2_t *str);

		/** Trims all spaces off of the right hand side of "str". */
		static	void	rightTrim(ucs2_t *str);

		/** Trims all "character"'s off of the right hand side
		 *  of "str". */
		static	void	rightTrim(ucs2_t *str, ucs2_t character);

		/** Trims all spaces off of the left hand side of "str". */
		static	void	leftTrim(ucs2_t *str);

		/** Trims all "character"'s off of the left hand side
		 *  of "str". */
		static	void	leftTrim(ucs2_t *str, ucs2_t character);

		/** Trims all spaces off of both sides of "str". */
		static	void	bothTrim(ucs2_t *str);

		/** Trims all characters off of both sides of "str". */
		static	void	bothTrim(ucs2_t *str, ucs2_t character);

		/** Strips all instances of "character" from "str".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(ucs2_t *str, ucs2_t character);

		/** Strips all instances of "str2" from "str1".
		 *  Returns true if any characters were stripped and
		 *  false if no characters were stripped. */
		static	bool	strip(ucs2_t *str1, const ucs2_t *str2);

		/** Strips all instances of any character in "set" from
		 *  "str1".  Returns true if any characters were stripped
		 *  and false if no characters were stripped. */
		static	bool	stripSet(ucs2_t *str1, const ucs2_t *set);

		/** Replaces all instances of "oldchar"
		 *  in "str" with "newchar" */
		static	void	replace(ucs2_t *str,
					ucs2_t oldchar,
					ucs2_t newchar);

		/** Replaces all instances of any of the characters in
		 *  the set "oldchar" in "str" with "newchar" */
		static	void	replace(ucs2_t *str,
					const ucs2_t *oldchars,
					ucs2_t newchar);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of "oldstr" have been replaced with
		 *  "newstr" */
		static	ucs2_t	*replace(const ucs2_t *str,
						const ucs2_t *oldstr,
						const ucs2_t *newstr);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset" */
		static	ucs2_t	*replace(const ucs2_t *str,
					const ucs2_t * const *oldstrset,
					const ucs2_t * const *newstrset);

		/** Returns a new string which is a copy of "str" in which
		 *  all instances of strings found in the NULL-terminated
		 *  array "oldstrset", the number of characters of which are
		 *  given by the array "oldstrlen", have been replaced with the
		 *  commensurate string found in the NULL-terminated array
		 *  "newstrset", which must have the same number of members as
		 *  "oldstrset" */
		static	ucs2_t	*replace(const ucs2_t *str,
					const ucs2_t * const *oldstrset,
					size_t *oldstrlen,
					const ucs2_t * const *newstrset);

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
		static	bool	isInteger(const ucs2_t *val);

		/** Returns true if the string "val" is an integer and
		 *  false if it is not an integer. */
		static	bool	isInteger(const ucs2_t *val, int32_t len);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const ucs2_t *val);

		/** Returns true the string "val" is a number and false
		 *  if it is not a number */
		static	bool	isNumber(const ucs2_t *val, int32_t len);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint16_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint16_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint32_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint32_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint64_t number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(int64_t number,
						uint16_t zeropadding);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(uint64_t number,
						uint16_t zeropadding);


		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(float number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(float number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(float number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(double number,
						uint16_t precision,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(long double number);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(long double number,
						uint16_t scale);

		/** Returns a string representing "number".  The string
		 *  is allocated inside the function and must be deleted
		 *  by the calling program. */
		static	ucs2_t	*parseNumber(long double number,
						uint16_t precision,
						uint16_t scale);

		/** Converts "string" to a 64-bit integer. */
		static	int64_t	toInteger(const ucs2_t *string);

		/** Converts "string" to a 64-bit integer.  If non-NULL,
		 *  endptr will be set to the first character in the
		 *  string after the number. */
		static	int64_t	toInteger(const ucs2_t *string,
						const ucs2_t **endptr);

		/** Converts "string" to a 64-bit integer of base "base". */
		static	int64_t	toInteger(const ucs2_t *string, int32_t base);

		/** Converts "string" to a 64-bit integer of base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	int64_t	toInteger(const ucs2_t *string,
						const ucs2_t **endptr,
						int32_t base);

		/** Converts "string" to a 64-bit unsigned integer. */
		static	uint64_t	toUnsignedInteger(
						const ucs2_t *string);

		/** Converts "string" to a 64-bit unsigned integer.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	uint64_t	toUnsignedInteger(
						const ucs2_t *string,
						const ucs2_t **endptr);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base". */
		static	uint64_t	toUnsignedInteger(
						const ucs2_t *string,
						int32_t base);

		/** Converts "string" to a 64-bit unsigned integer of
		 *  base "base".
		 *  If non-NULL, endptr will be set to the first
		 *  character in the string after the number. */
		static	uint64_t	toUnsignedInteger(
						const ucs2_t *string,
						const ucs2_t **endptr,
						int32_t base);


		/** Converts "string" to a floating point number. */
		static	long double	toFloat(const ucs2_t *string);

		/** Converts "string" to a floating point number.
		 *
		 *  If "string" uses a "." as a decimal point (per the "C"
		 *  locale) then the "." is first converted to the appropriate
		 *  decimal-delimiter for the current locale.
		 *
		 *  (Currently only supported on linux/unix platforms that
		 *  provide the locale.h header.  On other platforms, it just
		 *  falls through to toFloat().) */
		static	long double	toFloatC(const ucs2_t *string);

		/** Converts "string" to a floating point number.  If
		 *  non-NULL, endptr will be set to the first character
		 *  in the string after the number. */
		static	long double	toFloat(const ucs2_t *string,
							const ucs2_t **endptr);

		/** Converts "amount" which is assumed to be a dollar amount
		 *  into pennies. */
		static	int64_t	convertAmount(const ucs2_t *amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string. */
		static	ucs2_t	*convertAmount(int64_t amount);

		/** Converts "amount" which is assumed to be a number of
 		 *  pennies into a dollar amount string where there
 		 *  are "padding" places between the dollar sign and decimal
 		 *  point.   These will be space padded if the amount is
 		 *  small enough not to fill them. */
		static	ucs2_t	*convertAmount(int64_t amount,
						uint16_t padding);


		/** escapes all characters in "characters" found in
		 *  "input" using \'s and returns it in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	ucs2_t	*escape(const ucs2_t *input,
					const ucs2_t *characters);

		/** unescapes all \-escaped characters found in
		 *  "input" and returns the result in a buffer
		 *  allocated inside the function.  This buffer must be
		 *  deleted by the calling program. */
		static	ucs2_t	*unescape(const ucs2_t *input);

		/** similar to escape() above, but takes an "inputlength"
		 *  parameter and returns the result in "output" and
		 *  "outputlength" rather than in a return value */
		static	void	escape(const ucs2_t *input,
						uint64_t inputlength,
						ucs2_t **output,
						uint64_t *outputlength,
						const ucs2_t *characters);

		/** similar to unescape() above, but takes an "inputlength"
		 *  parameter and returns the result in "output" and
		 *  "outputlength" rather than in a return value */
		static	void	unescape(const ucs2_t *input,
						uint64_t inputlength,
						ucs2_t **output,
						uint64_t *outputlength);

		/** Moves leading spaces to the end of "str" for
		 *  "len" characters.
		 *
		 *  Example: "   hello   " -> "hello      "  */
		static	void	leftJustify(ucs2_t *str, int32_t len);

		/** Moves trailing spaces to the beginning of "str" for
		 *  "len" characters.
		 *  
		 *  Example: "   hello   " -> "      hello" */
		static	void	rightJustify(ucs2_t *str, int32_t len);

		/** Centers the text of "str" for "len" characters.
		 *
		 *  Example: "hello      " -> "   hello   " */
		static	void	center(ucs2_t *str, int32_t len);

		/** Returns a copy of "string", padded with "padchar" to a
		 *  character length of "totallength".  Set "direction"
		 *  to -1 to left-pad, 0 to center-pad and 1 to right-pad.
		 *  Note that this method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate this
		 *  buffer. */
		static ucs2_t	*pad(const ucs2_t *string,
						ucs2_t padchar,
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
		static void	split(const ucs2_t *string,
					size_t stringlength,
					const ucs2_t *delimiter,
					size_t delimiterlength,
					bool collapse,
					ucs2_t ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by "delimiter" of
		 *  character length "delimiterlength" and allocates
		 *  "listlength" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const ucs2_t *string,
					const ucs2_t *delimiter,
					size_t delimiterlength,
					bool collapse,
					ucs2_t ***list,
					uint64_t *listlength);

		/** Parses "string" of character length "stringlength"
		 *  delimited by NULL-terminated "delimiter" and allocates
		 *  "listlength" elements of "list" to return the parts.  If
		 *  "collapse" is true then multiple instances of the delimiter
		 *  in a row will be interpreted as one instance of the
		 *  delimiter.  Each member of "list" and "list" itseslf must
		 *  be deallocated by the calling program. */
		static void	split(const ucs2_t *string,
					size_t stringlength,
					const ucs2_t *delimiter,
					bool collapse,
					ucs2_t ***list,
					uint64_t *listlength);

		/** Parses NULL-terminated "string" delimited by
		 *  NULL-terminated "delimiter" and allocates "listlength"
		 *  elements of "list" to return the parts.  If "collapse" is
		 *  true then multiple instances of the delimiter in a row will
		 *  be interpreted as one instance of the delimiter.  Each
		 *  member of "list" and "list" itseslf must be deallocated by
		 *  the calling program. */
		static void	split(const ucs2_t *string,
					const ucs2_t *delimiter,
					bool collapse,
					ucs2_t ***list,
					uint64_t *listlength);

		/** Returns a copy of the segment of "str"
		 *  between string indices "start" and "end",
		 *  inclusive. */
		static ucs2_t	*subString(const ucs2_t *str,
						size_t start, size_t end);

		/** Returns a copy of the segment of "str"
		 *  between string index "start" and the end
		 *  of the string, inclusive. */
		static ucs2_t	*subString(const ucs2_t *str, size_t start);

		/** Creates a new string with "src" inserted into "dest" at
                 *  "index". */
		static ucs2_t	*insertString(const ucs2_t *dest,
						const ucs2_t *src,
						uint64_t index);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  Where 1K = 1024. */
		static ucs2_t	*humanReadable(int64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static ucs2_t	*humanReadable(int64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static ucs2_t	*humanReadable(uint64_t number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static ucs2_t	*humanReadable(uint64_t number,
							bool onethousand);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc. */
		static ucs2_t	*humanReadable(long double number);

		/** Returns a human-readable version of the number.
		 *  
		 *  Eg. 128, 1.2K, 2.4M, 3.5G, 4.6T, etc.
		 *
		 *  If "onethousand" = true then 1K = 1000, otherwise
		 *  1K = 1024. */
		static ucs2_t	*humanReadable(long double number,
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
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of UCS-2 characters. */
		static ssize_t	printf(ucs2_t *buffer, size_t len,
						const ucs2_t *format, ...);

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
		 *  NOTE: wide character sequences will be converted to
		 *  sequences of UCS-2 characters. */
		static ssize_t	printf(ucs2_t *buffer, size_t len,
						const ucs2_t *format,
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
		 *  sequences of UCS-2 characters. */
		static ssize_t	printf(ucs2_t **buffer,
						const ucs2_t *format, ...);

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
		 *  sequences of UCS-2 characters. */
		static ssize_t	printf(ucs2_t **buffer,
						const ucs2_t *format,
						va_list *argp);

		/** Returns true if the platform supports printf()ing UCS-2
		 *  characters and false otherwise. */
		static bool	supportsPrintf();

	#include <rudiments/private/ucs2charstring.h>
};

#endif
