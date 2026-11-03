// Copyright (c) David Muse
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
 *  succeed, but a subsequent attempt to get a value from row 7 would fail.
 *
 *  In a block-based implementation, a pointer value, such as a const char *,
 *  can point into the buffers of the current block.  Then the pointer is
 *  valid only while that block is loaded.
 *
 *  A table that is created from, or assigned from, a block-based
 *  implementation gets the same pointers, not copies of the data.  The table
 *  loads later blocks as it reads the rows.  So pointers from earlier blocks
 *  can be invalid immediately.  To prevent this, set flags on the source
 *  before the table reads it:
 *  - setManageArrayValues(true) makes the table duplicate the strings.
 *    setManageValues(true) does not duplicate strings.
 *  - setCopyColumnNames(true) makes the table duplicate the column names.
 *
 *  On a table source, these flags also make the source delete its own values
 *  or column names on clear() and destruction.  If the source does not own
 *  its values, then set the manage flag just before the copy, and set it back
 *  to false just after the copy.  But setCopyColumnNames() is unsafe on a
 *  table after its column names are set.  So set it before the names are
 *  set, and do not change it after.
 *
 *  The table takes these flags from the source.  Flags that are set on the
 *  table before an assignment have no effect. */
template <class valuetype>
class tablecollection : public collection {
	public:

		/** Creates an instance of the tablecollection class. */
		tablecollection();

		/** Creates an instance of the tablecollection class that
		 *  is a copy of "c".
		 *
		 *  Copies only the flags of "c", such as the manage-values
		 *  and copy-column-names flags, not the rows or columns.  A
		 *  table copy of a block-based "c" gets pointers into the
		 *  blocks of "c" unless flags are set on "c" first.  These
		 *  pointers are valid only while that block is loaded.  See
		 *  the class description. */
		tablecollection(tablecollection &c);

		/** Makes this instance of the tablecollection class
		 *  identical to "c".
		 *
		 *  Copies only the flags of "c", such as the manage-values
		 *  and copy-column-names flags, not the rows or columns.  A
		 *  table assignment from a block-based "c" gets pointers into
		 *  the blocks of "c" unless flags are set on "c" first.  These
		 *  pointers are valid only while that block is loaded.  See
		 *  the class description. */
		tablecollection &operator=(tablecollection &c);

		/** Deletes this instance of the tablecollection class. */
		virtual	~tablecollection();

		/** Returns "table". */
		virtual const char	*getType();

		/** Returns the number of elements in the table.
		 *
		 *  In a block-based implementation, counts only the elements
		 *  in the rows through the end of the current block. */
		virtual uint64_t	getCount();

		/** In a read-write implementation, sets the name of column
		 *  "col" to "name".
		 *
		 *  In a read-only implementation, has no effect. */
		virtual	void		setColumnName(uint64_t col,
							const char *name)=0;

		/** Returns the name of column "col" or NULL if column "col"
		 *  has no name. */
		virtual	const char	*getColumnName(uint64_t col)=0;

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
		virtual	bool	getCopyColumnNames();

		/** Returns the current number of columns in the table.
		 *
		 *  In a read-write implementation, returns larger and larger
		 *  values as calls to setColumnName() or setValue() extend the
		 *  table. */
		virtual	uint64_t	getColumnCount()=0;
		
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
						uint64_t col)=0;

		/** Returns a reference to the value at "row", "col". */
		virtual	valuetype	&getReference(uint64_t row,
							uint64_t col)=0;

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		virtual	valuetype	getValue(uint64_t row,
						const char * colname)=0;

		/** Returns the current number of rows in the table.
		 *
		 *  In a read-write implementation, returns larger and larger
		 *  values as calls to setValue() extend the table.
		 *
		 *  In a block-based implementation, returns the total number
		 *  of rows through the end of the current block, which is only
		 *  the total number of rows in the table when
		 *  getAllRowsAvailable() returns true. */
		virtual	uint64_t	getRowCount()=0;

		/** Always returns true for monolithic implementations.  Only
		 *  returns true in a block-based implementation if the current
		 *  block contains the last row in the table.
		 *
		 *  A block-based implementation should load the next block
		 *  when getValue() is called for row getRowCount(), so that
		 *  getRowCount() increases if there are more rows.
		 *
		 *  See the class description for the limits of a table copy
		 *  of a block-based implementation. */
		virtual	bool		getAllRowsAvailable()=0;

		/** Writes a representation of the tablecollection to standard
		 *  output.
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	write();

		/** Writes a representation of the tablecollection to "out".
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	write(output *out);

		/** Writes a JSON representation of the tablecollection to
		 *  standard output.
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson();

		/** Writes a JSON representation of the tablecollection to
		 *  standard output.
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the collection
		 *  is written without indentation.
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(bool indent);

		/** Writes a JSON representation of the tablecollection to
		 *  "out".
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(output *out);

		/** Writes a JSON representation of the tablecollection to
		 *  "out".
		 *  
		 *  If "indent" is true, then the output is automatically
		 *  indented.  If "indent" is false, then the collection
		 *  is written without indentation.
		 *
		 *  Rows are read in order.  If getAllRowsAvailable() returns
		 *  false, then later blocks are loaded by calls to getValue().
		 *  In a sequential implementation, the rows that were written
		 *  can not be read again after this call.
		 *
		 *  If the table has zero columns, only the rows of the first
		 *  block are written, as empty arrays.
		 *
		 *  Returns the number of bytes written, or -1 if an error
		 *  occurred.  A short write returns -1.  Any other error from
		 *  the output, such as a timeout or abort, is returned
		 *  unchanged.  Nothing more is written after an error. */
		virtual	ssize_t	writeJson(output *out, bool indent);

	#include <rudiments/private/tablecollection.h>
};

#include <rudiments/private/tablecollectioninlines.h>

#endif
