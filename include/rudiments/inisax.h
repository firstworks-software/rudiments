// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INISAX_H
#define RUDIMENTS_INISAX_H

#include <rudiments/private/inisaxincludes.h>

/** The inisax class is a base class for an SAX-style parser for data in
 *  .ini-file format.  To use this class, you should create a class that
 *  inherits from it and implements its protected virtual methods. */
class RUDIMENTS_DLLSPEC inisax : public sax {
	public:

		/** Creates an instance of the inisax class. */
		inisax();

		/** Deletes this instance of the inisax class. */
		virtual	~inisax();

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

		/** Gets called when the start of a semicolon-delimited comment
		 *  is encountered. */
		virtual	bool	semicolonCommentStart();

		/** Gets called when a semicolon-delimited comment is parsed. */
		virtual	bool	semicolonComment(const char *c);

		/** Gets called when the end of a semicolon-delimited comment
		 *  is encountered. */
		virtual	bool	semicolonCommentEnd();

		/** Gets called when the start of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentStart();

		/** Gets called when a pound-delimited comment is parsed. */
		virtual	bool	poundComment(const char *c);

		/** Gets called when the end of a pound-delimited comment
		 *  is encountered. */
		virtual	bool	poundCommentEnd();

		/** Gets called when the start of a section is encountered. */
		virtual	bool	sectionStart();

		/** Gets called when the name of a section is parsed. */
		virtual	bool	section(const char *s);

		/** Gets called when the end of a section is encountered. */
		virtual	bool	sectionEnd();

		/** Gets called when the start of a key is encountered. */
		virtual	bool	keyStart();

		/** Gets called when a key name is parsed. */
		virtual	bool	key(const char *k);

		/** Gets called when the end of a key is encountered. */
		virtual	bool	keyEnd();

		/** Gets called when the start of a value is encountered. */
		virtual	bool	valueStart();

		/** Gets called when a value name is parsed. */
		virtual	bool	value(const char *v);

		/** Gets called when the end of a value is encountered. */
		virtual	bool	valueEnd();

	#include <rudiments/private/inisax.h>
};

#endif
