// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TABLECOLLECTION_H
#define RUDIMENTS_TABLECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The tablecollection class is the parent class for all rudiments table
 *  collections.
 *
 *  Tables have rows and columns.  Columns may have names.
 *
 *  Tables may be read-write or read-only, monolithic or block-based,
 *  and random-access or sequential-access.
 *
 *  In a monolithic implemenatation, all rows are immediately available at all
 *  times.
 *
 *  In a block-based implementation, rows are only immediately available in
 *  blocks.  Eg. If the table has a block size of 10, then getting a value in
 *  row 6 would cause rows 0-9 to be loaded.  Getting a value in row 22 would
 *  cause rows 20-29 to be loaded.
 *
 *  In a random implementation, values from any row may be accessed at any
 *  time.  If the implementation is also block-based, then it may be more
 *  efficient to access values from adjacent rows within the current block than
 *  to access values from rows outside of the current block.
 *
 *  In a sequential implementation, values may only be accessed from rows within
 *  the current block or subsequent blocks.  Eg. If the table has a block size
 *  of 10, then getting a value in row 6, followed by a value in row 22 would
 *  succeed, but a subsequent attempt to get a value from row 7 would fail. */
template <class valuetype>
class tablecollection : public collection {
	public:

		/** Creates an instance of the tablecollection class. */
		tablecollection();

		/** Creates an instance of the tablecollection class that
		 *  is a copy of "c". */
		tablecollection(const tablecollection &c);

		/** Makes this instance of the tablecollection class
		 *  identical to "c". */
		tablecollection &operator=(const tablecollection &c);

		/** Deletes this instance of the tablecollection class. */
		virtual	~tablecollection();

		/** Returns "table". */
		virtual const char	*getType() const;

		/** In a read-write implementation, sets the name of column
		 *  "col" to "name".
		 *
		 *  In a read-only implementation, has no effect. */
		virtual	void		setColumnName(uint64_t col,
							const char *name)=0;

		/** Returns the name of column "col" or NULL if column "col"
		 *  has no name. */
		virtual	const char	*getColumnName(uint64_t col) const=0;

		/** Indicates whether or not this instance of the
		 *  tablecollection class should make copies the values that
		 *  are passed in to setColumnName().
		 *
		 *  Defaults to "false". */
		virtual	void	setCopyColumnNames(bool copy);

		/** Returns whether or not this instance of the tablecollection
 		 *  class is configured to make copies of the values that are
 		 *  passed in to setColumnName().
		 *
		 *  Returns true if it is and false if it is not. */
		virtual	bool	getCopyColumnNames() const;

		/** Returns the current number of columns in the table.
		 *
		 *  In a read-write implementation, returns larger and larger
		 *  values as calls to setColumnName() or setValue() extend the
		 *  table. */
		virtual	uint64_t	getColumnCount() const=0;
		
		/** In a read-write implementation, sets the value at "row",
		 *  "col" to "value".
		 *
		 *  In a read-only implementation, has no effect. */
		virtual	void		setValue(uint64_t row,
							uint64_t col,
							valuetype value)=0;

		/** Returns the value at "row", "col".  Returns NULL or 0 if
		 *  there is no value at that address. */
		virtual	valuetype	getValue(uint64_t row,
						uint64_t col) const=0;

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		virtual	valuetype	getValue(uint64_t row,
						const char * colname) const=0;

		/** Returns the current number of rows in the table.
		 *
		 *  In a read-write implementation, returns larger and larger
		 *  values as calls to setValue() extend the table.
		 *
		 *  In a block-based implementation, returns the total number
		 *  of rows through the end of the current block, which is only
		 *  the total number of rows in the table when
		 *  getAllRowsAvailable() returns true. */
		virtual	uint64_t	getRowCount() const=0;

		/** Always returns true for monolithic implementations.  Only
		 *  returns true in a block-based implementation if the current
		 *  block contains the last row in the table. */
		virtual	bool		getAllRowsAvailable() const=0;

		/** Writes a representation of the tablecollection to standard
		 *  output. */
		virtual	ssize_t	write() const;

		/** Writes a representation of the tablecollection to "out". */
		virtual	ssize_t	write(output *out) const;

		/** Writes a JSON representation of the tablecollection to
		 *  standard output. */
		virtual	ssize_t	writeJson() const;

		/** Writes a JSON representation of the tablecollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(bool indent) const;

		/** Writes a JSON representation of the tablecollection to
		 *  "out". */
		virtual	ssize_t	writeJson(output *out) const;

		/** Writes a JSON representation of the tablecollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the tree is written
		 *  without indentation. */
		virtual	ssize_t	writeJson(output *out, bool indent) const;

	#include <rudiments/private/tablecollection.h>
};

#include <rudiments/private/tablecollectioninlines.h>

#endif
