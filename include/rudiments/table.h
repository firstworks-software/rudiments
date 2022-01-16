// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TABLE_H
#define RUDIMENTS_TABLE_H

#include <rudiments/private/tableincludes.h>

/** The table class allows you to store an arbitrary number of values
 *  in a table.  Since the table class is template-based, you can 
 *  store arbitrary types of values.
 *
 *  Read-write, monolithic, random-access. */
template <class valuetype>
class table : public tablecollection<valuetype> {
	public:

		/** Creates an instance of the table class. */
		table();

		/** Creates an instance of the table class
		 *  that is a copy of "a". */
		table(const table<valuetype> &a);

		/** Creates an instance of the table class
		 *  that is a copy of "a". */
		table(const tablecollection<valuetype> &a);

		/** Makes this instance of the table class
		 *  identical to "a". */
		table<valuetype>	&operator=(const table<valuetype> &a);

		/** Makes this instance of the table class
		 *  identical to "a". */
		table<valuetype>	&operator=(
					const tablecollection<valuetype> &a);

		/** Deletes this instance of the table class. */
		~table();

		/** Indicates whether or not this instance of the
		 *  tablecollection class should make copies the values that
		 *  are passed in to setColumnName().
		 *
		 *  Defaults to "false". */
		void	setCopyColumnNames(bool copy);

		/** Returns whether or not this instance of the tablecollection
 		 *  class is configured to make copies of the values that are
 		 *  passed in to setColumnName().
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getCopyColumnNames() const;

		/** Sets the name of column "col" to "name" */
		void		setColumnName(uint64_t col, const char *name);

		/** Returns the name of column "col". */
		const char	*getColumnName(uint64_t col) const;

		/** Returns the current number of columns in the table.
		 *  
		 *  Returns larger and larger values as calls to setColumnName()
		 *  or setValue() extend the table. */
		uint64_t	getColumnCount() const;

		/** Indicates whether or not this instance of the table
		 *  class should delete the values that are stored at each
		 *  location in the table when clear() is called.
		 *
		 *  Setting "manage" to true causes the instance to delete the
		 *  values that are stored at each location in the table
		 *  when clear() is called.
		 *
		 *  Setting "manage" to false causes the instance not to delete
		 *  the values that are stored at each location in the
		 *  table when clear() is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageArrayValues(false); */
		void	setManageValues(bool manage);

		/** Returns whether or not this instance of the table class
		 *  is configured to delete the values that are stored at each
		 *  location in the table when clear() is called.
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getManageValues() const;

		/** Indicates whether or not this instance of the table
		 *  class should array-delete the values that are stored at each
		 *  location in the table when clear() is called.
		 *
		 *  Setting "manage" to true causes the instance to
		 *  array-delete the values that are stored at each location in
		 *  the table when clear() is called.
		 *
		 *  Setting "manage" to false causes the instance not to
		 *  array-delete the values that are stored at each location in
		 *  the table when clear() is called.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageValues(false); */
		void	setManageArrayValues(bool manage);

		/** Returns whether or not this instance of the table class
		 *  is configured to array-delete the values that are stored at
		 *  each location in the table when clear() is called.
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getManageArrayValues() const;

		/** Sets the value at "row", "col" to "value". */
		void		setValue(uint64_t row, uint64_t col,
							valuetype value);

		/** Returns the value at "row", "col".  Returns NULL or 0 if
		 *  there is no value at that address. */
		valuetype	getValue(uint64_t row,
						uint64_t col) const;

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		valuetype	getValue(uint64_t row,
						const char *colname) const;

		/** Returns the current number of rows in the table.
		 *
		 *  Returns larger and larger values as calls to setValue()
		 *  extend the table. */
		uint64_t	getRowCount() const;

		/** Returns true. */
		bool		getAllRowsAvailable() const;

		/** Removes all values currently stored in the table, such that
		 *  getValue() will return NULL or 0. */
		void		clear();

	#include <rudiments/private/table.h>
};

#include <rudiments/private/tableinlines.h>

#endif
