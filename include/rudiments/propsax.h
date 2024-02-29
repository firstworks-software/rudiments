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
		 *  comment, section name, key, or value) is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	whitespaceStart();

		/** Gets called when whitespace (outside of a comment, section
		 *  name, key, or value) is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	whitespace(const char *w);

		/** Gets called when the end of whitespace (outside of a
		 *  comment, section name, key, or value) is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	whitespaceEnd();

		/** Gets called when the start of an exclamation-delimited
		 *  comment is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	exclamationCommentStart();

		/** Gets called when an exclamation-delimited comment is
		 *  parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	exclamationComment(const char *c);

		/** Gets called when the end of an exclamation-delimited
		 *  comment is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	exclamationCommentEnd();

		/** Gets called when the start of a pound-delimited comment
		 *  is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	poundCommentStart();

		/** Gets called when a pound-delimited comment is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	poundComment(const char *c);

		/** Gets called when the end of a pound-delimited comment
		 *  is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	poundCommentEnd();

		/** Gets called when the start of a key is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	keyStart();

		/** Gets called when a key name is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	key(const char *k);

		/** Gets called when the end of a key is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	keyEnd();

		/** Gets called when the equality delimiter between a key and
		 *  value is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	equals(const char *e);

		/** Gets called when the start of a value is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	valueStart();

		/** Gets called when a value name is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	value(const char *v);

		/** Gets called when the end of a value is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	valueEnd();

	#include <rudiments/private/propsax.h>
};

#endif
