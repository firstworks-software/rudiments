// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_JSONSAX_H
#define RUDIMENTS_JSONSAX_H

#include <rudiments/private/jsonsaxincludes.h>

/** The jsonsax class is a base class for an SAX-style parser for JSON data.
 *  To use this class, you should create a class that inherits from it and
 *  implements its protected virtual methods. */
class RUDIMENTS_DLLSPEC jsonsax : public sax {
	public:

		/** Creates an instance of the jsonsax class. */
		jsonsax();

		/** Deletes this instance of the jsonsax class. */
		virtual	~jsonsax();

	protected:

		/** Gets called when an object start is parsed. */
		virtual	bool	objectStart();

		/** Gets called when a member start is parsed. */
		virtual	bool	memberStart();

		/** Gets called when a member name is parsed. */
		virtual	bool	memberName(const char *name);

		/** Gets called when a value start is parsed. */
		virtual	bool	valueStart();

		/** Gets called when a string value is parsed. */
		virtual	bool	stringValue(const char *value);

		/** Gets called when a number value is parsed. */
		virtual	bool	numberValue(const char *value);

		/** Gets called when a true value is parsed. */
		virtual	bool	trueValue();

		/** Gets called when a false value is parsed. */
		virtual	bool	falseValue();

		/** Gets called when a null value is parsed. */
		virtual	bool	nullValue();

		/** Gets called when an array start is parsed. */
		virtual	bool	arrayStart();

		/** Gets called when an array end is parsed. */
		virtual	bool	arrayEnd();

		/** Gets called when an value end is parsed. */
		virtual	bool	valueEnd();

		/** Gets called when a member end is parsed. */
		virtual	bool	memberEnd();

		/** Gets called when an object end is parsed. */
		virtual	bool	objectEnd();

	#include <rudiments/private/jsonsax.h>
};

#endif
