// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PROPSAX_H
#define RUDIMENTS_PROPSAX_H

#include <rudiments/private/propsaxincludes.h>

/** The propsax class is a base class for an SAX-style parser for data in
 *  property-file format.  To use this class, you should create a class that
 *  inherits from it and implements its protected virtual methods. */
class RUDIMENTS_DLLSPEC propsax : public sax {
	public:

		/** Creates an instance of the propsax class. */
		propsax();

		/** Deletes this instance of the propsax class. */
		virtual	~propsax();

	protected:

		/** Gets called when the start of whitespace (outside of a
		 *  comment, section name, key, or value) is encountered. */
		virtual	bool	whitespaceStart();

		/** Gets called when whitespace (outside of a comment, section
		 *  name, key, or value) is parsed. */
		virtual	bool	whitespace(const char *w);

		/** Gets called when the end of whitespace (outside of a
		 *  comment, section name, key, or value) is encountered. */
		virtual	bool	whitespaceEnd();

		/** Gets called when the start of an exclamation-delimited
		 *  comment is encountered. */
		virtual	bool	exclamationCommentStart();

		/** Gets called when an exclamation-delimited comment is
		 *  parsed. */
		virtual	bool	exclamationComment(const char *c);

		/** Gets called when the end of an exclamation-delimited
		 *  comment is encountered. */
		virtual	bool	exclamationCommentEnd();

		/** Gets called when the start of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentStart();

		/** Gets called when a pound-delimited comment is parsed. */
		virtual	bool	poundComment(const char *c);

		/** Gets called when the end of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentEnd();

		/** Gets called when the start of a key is encountered. */
		virtual	bool	keyStart();

		/** Gets called when a key name is parsed. */
		virtual	bool	key(const char *k);

		/** Gets called when the end of a key is encountered. */
		virtual	bool	keyEnd();

		/** Gets called when the equality delimiter between a key and
		 *  value is parsed. */
		virtual	bool	equals(const char *e);

		/** Gets called when the start of a value is encountered. */
		virtual	bool	valueStart();

		/** Gets called when a value name is parsed. */
		virtual	bool	value(const char *v);

		/** Gets called when the end of a value is encountered. */
		virtual	bool	valueEnd();

	#include <rudiments/private/propsax.h>
};

#endif
