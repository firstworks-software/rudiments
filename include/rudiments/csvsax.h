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

		/** Gets called when a column name is parsed.  "namelength" is
		 *  the exact number of bytes in "name" and may be greater than
		 *  charstring::getLength(name) if "name" contains embedded
		 *  null bytes.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation calls column(name,quoted), which
		 *  truncates "name" at the first embedded null byte.  A child
		 *  class that needs to preserve embedded null bytes should
		 *  override this method instead. */
		virtual	bool	column(const char *name, size_t namelength,
								bool quoted);

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

		/** Gets called when a field is parsed.  "valuelength" is the
		 *  exact number of bytes in "value" and may be greater than
		 *  charstring::getLength(value) if "value" contains embedded
		 *  null bytes.
		 *
		 *  Returns true on success and false if an error occurred.
		 *  Parsing stops if this method returns false.
		 *
		 *  This implementation calls field(value,quoted), which
		 *  truncates "value" at the first embedded null byte.  A child
		 *  class that needs to preserve embedded null bytes should
		 *  override this method instead. */
		virtual	bool	field(const char *value, size_t valuelength,
								bool quoted);

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
