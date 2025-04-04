// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSVSAX_H
#define RUDIMENTS_CSVSAX_H

#include <rudiments/private/csvsaxincludes.h>

/** The csvsax class is a base class for an SAX-style parser for CSV data.  To
 *  use this class, you should create a class that inherits from it and
 *  implements its protected virtual methods. */
class RUDIMENTS_DLLSPEC csvsax : public sax {
	public:

		/** Creates an instance of the csvsax class. */
		csvsax();

		/** Deletes this instance of the csvsax class. */
		virtual	~csvsax();

		/** Sets the character that optionally surrounds columns and
		 *  fields to "quote". */
		void	setQuote(char quote);

		/** Returns the character that optionally surrounds columns
		 *  and fields. */
		char	getQuote();

		/** Sets the character that separates columns and fields to
		 *  "delimiter". */
		void	setDelimiter(char delimiter);

		/** Returns the character that separates columns and fields. */
		char	getDelimiter();

	protected:

		/** Gets called when the start of the header is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	headerStart();

		/** Gets called when a column name is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	column(const char *name, bool quoted);

		/** Gets called when the end of the header is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	headerEnd();

		/** Gets called when the start of the body is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	bodyStart();

		/** Gets called when the start of a record is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	recordStart();

		/** Gets called when a field is parsed.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	field(const char *value, bool quoted);

		/** Gets called when the end of a record is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	recordEnd();

		/** Gets called when the end of the body is encountered.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation just returns true by a child class may
		 *  override this method to do something else. */
		virtual	bool	bodyEnd();

	#include <rudiments/private/csvsax.h>
};

#endif
