// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_XMLSAX_H
#define RUDIMENTS_XMLSAX_H

#include <rudiments/private/xmlsaxincludes.h>

/** The xmlsax class is a base class for an SAX-style parser for XML data.  To
 *  use this class, you should create a class that inherits from it and
 *  implements its protected virtual methods. */
class RUDIMENTS_DLLSPEC xmlsax : public sax {
	public:

		/** Creates an instance of the xmlsax class. */
		xmlsax();

		/** Deletes this instance of the xmlsax class. */
		virtual	~xmlsax();

	protected:

		/** Gets called when a start tag is parsed. */
		virtual	bool	tagStart(const char *ns, const char *name);

		/** Gets called when an attribute name is parsed. */
		virtual	bool	attributeName(const char *name);

		/** Gets called when an attribute value is parsed. */
		virtual	bool	attributeValue(const char *value);

		/** Gets called when a block of text is parsed. */
		virtual	bool	text(const char *string);

		/** Gets called when an end tag is parsed. */
		virtual	bool	tagEnd(const char *ns, const char *name);

		/** Gets called when a comment is parsed. */
		virtual	bool	comment(const char *string);

		/** Gets called when cdata is parsed. */
		virtual	bool	cdata(const char *string);

	#include <rudiments/private/xmlsax.h>
};

#endif
