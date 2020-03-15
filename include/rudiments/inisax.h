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

		virtual	bool	whitespaceStart();
		virtual	bool	whitespace(const char *w);
		virtual	bool	whitespaceEnd();

		virtual	bool	semicolonCommentStart();
		virtual	bool	semicolonComment(const char *c);
		virtual	bool	semicolonCommentEnd();

		virtual	bool	poundCommentStart();
		virtual	bool	poundComment(const char *c);
		virtual	bool	poundCommentEnd();

		virtual	bool	sectionStart();
		virtual	bool	section(const char *s);
		virtual	bool	sectionEnd();

		virtual	bool	keyStart();
		virtual	bool	key(const char *k);
		virtual	bool	keyEnd();

		virtual	bool	valueStart();
		virtual	bool	value(const char *v);
		virtual	bool	valueEnd();

	#include <rudiments/private/inisax.h>
};

#endif
