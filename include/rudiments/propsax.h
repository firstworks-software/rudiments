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

		virtual	bool	whitespaceStart();
		virtual	bool	whitespace(const char *w);
		virtual	bool	whitespaceEnd();

		virtual	bool	exclamationCommentStart();
		virtual	bool	exclamationComment(const char *c);
		virtual	bool	exclamationCommentEnd();

		virtual	bool	poundCommentStart();
		virtual	bool	poundComment(const char *c);
		virtual	bool	poundCommentEnd();

		virtual	bool	keyStart();
		virtual	bool	key(const char *k);
		virtual	bool	keyEnd();

		virtual	bool	equals(const char *e);

		virtual	bool	valueStart();
		virtual	bool	value(const char *v);
		virtual	bool	valueEnd();

	#include <rudiments/private/propsax.h>
};

#endif
