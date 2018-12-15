// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SAX_H
#define RUDIMENTS_SAX_H

#include <rudiments/private/saxincludes.h>

/** The sax class is a base class for a SAX-style parser.  To use this class,
 *  you should create a class that inherits from one if its immediate children
 *  and implements the protected virtual methods of that class. */
class RUDIMENTS_DLLSPEC sax {
	public:

		/** Creates an instance of the sax class. */
		sax();

		/** Deletes this instance of the sax class. */
		virtual	~sax();


		/** Parses file "filename" and calls the appropriate callback
		 *  when tags, attributes, text, etc. are encountered.
		 *
		 *  Returns true on success or false otherwise. */
		virtual bool	parseFile(const char *filename);

		/** Parses "string"  and calls the appropriate callback
		 *  when tags, attributes, text, etc. are encountered.
		 *
		 *  Returns true on success or false otherwise. */
		virtual bool	parseString(const char *string);

		/** If parseFile() or parseString() fails, this method
		 *  returns the error that caused the failure. */
		const char	*getError();

	#include <rudiments/private/sax.h>
};

#endif
