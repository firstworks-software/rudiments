// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_REGEXP_H
#define RUDIMENTS_REGEXP_H

#include <rudiments/private/regularexpressionincludes.h>

/** regular expression compile options, or'ed together and passed to
 *  regularexpression::setPattern() */
enum regularexpressionoption_t {
	REGULAR_EXPRESSION_CASE_INSENSITIVE=1,
	REGULAR_EXPRESSION_MULTILINE=2,
	REGULAR_EXPRESSION_DOT_ALL=4
};

/** The regularexpression class provides methods for making comparisons
 *  between text and regular expressions.
 * 
 *  Regular expressions are complex, powerful, used in command line
 *  programs like grep, sed and find, and extensively in Perl. */
class RUDIMENTS_DLLSPEC regularexpression : public object {
	public:

		/** Returns true if the first "length" characters of "str"
		 *  matches "pattern" and false otherwise. */
		static	bool	match(const char *str, size_t length,
							const char *pattern);

		/** Returns true if "str" matches "pattern" and false
		 *  otherwise. */
		static	bool	match(const char *str, const char *pattern);


		/** Creates an instance of the regularexpression class. */
		regularexpression();

		/** Creates an instance of the regularexpression class and
 		 *  sets the regular expression of this instance to
 		 *  "pattern". */
		regularexpression(const char *pattern);

		/** Deletes this instance of the regular expression class. */
		~regularexpression();

		/** Sets the regular expression of this instance to "pattern".
		 *
		 *  Returns true if the compilation succeeded and false
		 *  if it failed. */
		bool	setPattern(const char *pattern);

		/** Sets the regular expression of this instance to
		 *  "pattern", compiled with "options", which is 0 or
		 *  some of the following or'ed together.
		 *
		 *  REGULAR_EXPRESSION_CASE_INSENSITIVE - letters match
		 *  either case.
		 *
		 *  REGULAR_EXPRESSION_MULTILINE - ^ and $ match at every
		 *  newline in the subject, not just at its start and end.
		 *
		 *  REGULAR_EXPRESSION_DOT_ALL - . matches a newline too.
		 *
		 *  An option that the platform's regular expression
		 *  engine cannot honor is ignored rather than failing the
		 *  compilation.  Case-insensitivity is honored
		 *  everywhere.
		 *
		 *  On platforms without PCRE, where this class falls back
		 *  to a POSIX engine, multiline and dot-all are a single
		 *  switch rather than two.  Asking for multiline also
		 *  stops . and a negated character class like [^x] from
		 *  matching a newline, so asking for both gets multiline
		 *  and drops dot-all.  Note also that on those platforms
		 *  . matches a newline unless multiline is asked for, so a
		 *  pattern compiled with no options at all behaves there
		 *  as though dot-all were on.
		 *
		 *  An option that isn't one of the above is ignored.
		 *
		 *  Returns true if the compilation succeeded and false
		 *  if it failed. */
		bool	setPattern(const char *pattern, uint32_t options);

		/** Returns the regular expression of this instance. */
		const char	*getPattern();

		/** Studies the current pattern so it can be executed faster.
		 *  If you plan on calling match() several times on this
		 *  pattern, studying it may be worthwhile.  If not, the
		 *  studying the pattern may take longer than the time saved
		 *  by studying it.
		 * 
		 *  Returns true if the study succeeded
		 *  and false if if failed. */
		bool	study();

		/** Matches "str" against the regular expression
		 *  compiled earlier using the compile method.
		 *
		 *  Only the first match in "str" is reported.  Call
		 *  matchNext() afterward to walk the rest of them.
		 *
		 *  Returns true if the match was successful and
		 *  false if it was not. */
		bool	match(const char *str);

		/** Matches the first "length" character of "str" against the
		 *  regular expression compiled earlier using the compile
		 *  method.
		 *
		 *  Returns true if the match was successful and
		 *  false if it was not. */
		bool	match(const char *str, size_t length);

		/** Matches the first "length" characters of "str" against
		 *  the regular expression compiled earlier using the
		 *  compile method, starting at "offset" characters into
		 *  "str" rather than at the beginning.
		 *
		 *  The part of "str" before "offset" is not matched
		 *  against, but the expression is still evaluated as
		 *  though it were there.  So ^ does not match at "offset",
		 *  and a lookbehind can see the characters before it.
		 *  This is what a global substitution needs, where each
		 *  match resumes where the previous one left off.
		 *
		 *  Offsets reported by getSubstringStartOffset() and
		 *  getSubstringEndOffset() remain relative to the start of
		 *  "str", not to "offset".
		 *
		 *  Note that on platforms without PCRE, this class falls
		 *  back to a POSIX regular expression engine, and only
		 *  the ^ anchor is handled correctly there.  Those
		 *  engines have no lookbehind at all, and they compute a
		 *  word boundary as though the subject began at
		 *  "offset", so \b can match there when it should not.
		 *
		 *  Returns true if the match was successful and
		 *  false if it was not, including if "offset" is negative
		 *  or past "length". */
		bool	match(const char *str, size_t length, int32_t offset);

		/** Matches the regular expression against the subject
		 *  given to the last successful call to match(),
		 *  resuming after the previous match rather than at the
		 *  start of the subject.
		 *
		 *  Calling match() and then matchNext() over and over
		 *  reports every match in the subject, in order, rather
		 *  than only the first.  It continues the subject given
		 *  to any of the match() methods that take one, and
		 *  takes no subject of its own, so it cannot be handed
		 *  one that disagrees with the walk in progress.  The
		 *  substring methods report the new match each time.
		 *
		 *  A match that is empty is reported like any other,
		 *  including one at the very end of the subject, and
		 *  the walk resumes one character past where an empty
		 *  match started, so the same one is never reported
		 *  twice.  Note that this is not what
		 *  charstring::replace() does with a regular expression
		 *  - it skips an empty match rather than replacing it,
		 *  which is a substitution policy rather than a
		 *  disagreement about where the matches are.  Both are
		 *  deliberate.
		 *
		 *  The subject given to match() must still be valid and
		 *  unchanged.  This class never copies it, and a walk
		 *  holds it across every call.  One instance can walk
		 *  only one subject at a time, so calling match() on
		 *  this instance from inside the walk ends the walk and
		 *  starts a new one.
		 *
		 *  Note that a walk advances by one character over an
		 *  empty match, and that setPattern() compiles patterns
		 *  without UTF support, so a character is a byte.
		 *
		 *  Calling setPattern() ends the walk.
		 *
		 *  Returns true if another match was found and false if
		 *  it was not, including if there was no successful call
		 *  to match() to continue from.  When it returns false
		 *  the substring methods report nothing, just as they do
		 *  after a failed match(), and calling it again keeps
		 *  returning false.  Call match() to start over. */
		bool	matchNext();

		/** Returns the number of substrings that the last
		 *  successful call to match() or matchNext() can report,
		 *  or 0 if that call failed.
		 *
		 *  This is one for the whole match, plus one for each
		 *  capture group in the pattern, whether or not the
		 *  group participated in the match.  A group that did
		 *  not participate is still a valid index, but reports
		 *  no substring. */
		int32_t	getSubstringCount();

		/** Returns the "index"'th matching substring, or NULL
		 *  if index is invalid or if the capture group at
		 *  "index" did not participate in the match. */
		const char	*getSubstringStart(int32_t index);

		/** Returns the data directly after the "index"'th
		 *  matching substring, or NULL if index is invalid or
		 *  if the capture group at "index" did not participate
		 *  in the match. */
		const char	*getSubstringEnd(int32_t index);

		/** Returns the offset of the "index"'th matching
		 *  substring, or -1 if index is invalid or if the
		 *  capture group at "index" did not participate in the
		 *  match. */
		int32_t	getSubstringStartOffset(int32_t index);

		/** Returns the offset of the data directly after the
		 *  "index"'th matching substring, or -1 if index is
		 *  invalid or if the capture group at "index" did not
		 *  participate in the match. */
		int32_t	getSubstringEndOffset(int32_t index);

		/** Returns the index of the capture group named "name",
		 *  or -1 if the pattern has no group by that name.
		 *
		 *  A pattern names a capture group by writing it as
		 *  (?\<name\>...) and refers to it later as \\k\<name\>.
		 *
		 *  Named capture groups are a PCRE feature.  The POSIX
		 *  regular expression engines that this class falls back
		 *  to, on platforms without PCRE, do not have them, so
		 *  this method returns -1 on those platforms.  It also
		 *  returns -1 if more than one group has the name, since
		 *  there is then no single index to return.
		 *
		 *  Note that passing a literal NULL to the methods below
		 *  that take a "name" is ambiguous with the ones that
		 *  take an "index" on some compilers, and picks the index
		 *  0 - the whole match - on others.  Cast it to
		 *  const char * if you must pass one. */
		int32_t	getSubstringIndex(const char *name);

		/** Returns the substring matched by the capture group
		 *  named "name", or NULL if the pattern has no group by
		 *  that name or if the group did not participate in the
		 *  match. */
		const char	*getSubstringStart(const char *name);

		/** Returns the data directly after the substring matched
		 *  by the capture group named "name", or NULL if the
		 *  pattern has no group by that name or if the group did
		 *  not participate in the match. */
		const char	*getSubstringEnd(const char *name);

		/** Returns the offset of the substring matched by the
		 *  capture group named "name", or -1 if the pattern has
		 *  no group by that name or if the group did not
		 *  participate in the match. */
		int32_t	getSubstringStartOffset(const char *name);

		/** Returns the offset of the data directly after the
		 *  substring matched by the capture group named "name",
		 *  or -1 if the pattern has no group by that name or if
		 *  the group did not participate in the match. */
		int32_t	getSubstringEndOffset(const char *name);

	#include <rudiments/private/regularexpression.h>
};

#endif
