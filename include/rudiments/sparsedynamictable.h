// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SPARSEDYNAMICTABLE_H
#define RUDIMENTS_SPARSEDYNAMICTABLE_H

#include <rudiments/private/sparsedynamictableincludes.h>

/** The sparsedynamictable class allows you to store an arbitrary number of
 *  values in a table.  Since the sparsedynamictable class is template-based,
 *  you can store arbitrary types of values.
 *
 *  This class is similar to dynamictable, but uses less memory and many of
 *  its operations are faster when the table is sparsely populated.
 *
 *  However, it performs more poorly than dynamictable for general purpose
 *  tables.  Unless your application intends to sparsely populate the table,
 *  then you should consider using the dynamictable class instead.
 *
 *  Read-write, monolithic, random-access. */
template <class valuetype>
class sparsedynamictable : public tablecollection<valuetype> {
	public:

		/** Creates an instance of the sparsedynamictable class. */
		sparsedynamictable();

		/** Deletes this instance of the sparsedynamictable class. */
		~sparsedynamictable();

		/** Sets the name of column "col" to "name" */
		void		setColumnName(uint64_t col, const char *name);

		/** Returns the name of column "col". */
		const char	*getColumnName(uint64_t col);

		/** Returns the current number of columns in the table.
		 *  
		 *  Returns larger and larger values as calls to setColumnName()
		 *  or setValue() extend the table. */
		uint64_t	getColCount();

		/** Sets the value at "row", "col" to "value". */
		void		setValue(uint64_t row, uint64_t col,
							valuetype value);

		/** Returns the value at "row", "col".  Returns NULL or 0 if
		 *  there is no value at that address. */
		valuetype	getValue(uint64_t row, uint64_t col);

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		valuetype	getValue(uint64_t row, const char *colname);

		/** Returns the current number of rows in the table.
		 *
		 *  Returns larger and larger values as calls to setValue()
		 *  extend the table. */
		uint64_t	getRowCount();

		/** Returns true. */
		bool		getAllRowsAvailable();

		/** Removes all values currently stored in the table, such that
		 *  getValue() will return NULL or 0. */
		void		clear();

	#include <rudiments/private/sparsedynamictable.h>
};

#include <rudiments/private/sparsedynamictableinlines.h>

#endif
