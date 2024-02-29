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

		/** Gets called when an object start is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	objectStart();

		/** Gets called when a member start is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	memberStart();

		/** Gets called when a member name is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	memberName(const char *name);

		/** Gets called when a value start is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	valueStart();

		/** Gets called when a string value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	stringValue(const char *value);

		/** Gets called when a number value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	numberValue(const char *value);

		/** Gets called when a true value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	trueValue();

		/** Gets called when a false value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	falseValue();

		/** Gets called when a null value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	nullValue();

		/** Gets called when an array start is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	arrayStart();

		/** Gets called when an array end is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	arrayEnd();

		/** Gets called when an value end is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	valueEnd();

		/** Gets called when a member end is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	memberEnd();

		/** Gets called when an object end is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	objectEnd();

	#include <rudiments/private/jsonsax.h>
};

#endif
