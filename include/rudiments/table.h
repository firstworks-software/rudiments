// Copyright (c) David Muse
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
		 *  that is a copy of "a".
		 *
		 *  Values are read from "a" using getValue(), in row
		 *  order.
		 *
		 *  The copy takes the manage-values, manage-array-values,
		 *  and copy-column-names flags from "a".  If no manage flag
		 *  is set on "a", a copy of pointer values gets the same
		 *  pointers as "a", valid only while the data they point to
		 *  exists.  Unless "a" copies column names, the copy also
		 *  shares its column name pointers.  See the collection class
		 *  description. */
		table(table<valuetype> &a);

		/** Creates an instance of the table class
		 *  that is a copy of "a".
		 *
		 *  Values are read from "a" using getValue(), in row
		 *  order.  If not all of "a"'s rows are available
		 *  (getAllRowsAvailable() returns false), then rows are read
		 *  until getAllRowsAvailable() returns true or reading past
		 *  the current block doesn't make the row exist.
		 *
		 *  The copy takes the manage-values, manage-array-values,
		 *  and copy-column-names flags from "a".  If "a" is
		 *  block-based, then a copy of pointer values gets pointers
		 *  into the blocks of "a".  These can be invalid immediately,
		 *  because the copy loads later blocks.  Call
		 *  setManageArrayValues(true) and setCopyColumnNames(true)
		 *  on "a" first to duplicate the strings and column names.
		 *  On a table, these flags also make "a" delete its own
		 *  values or column names.  See the tablecollection class
		 *  description for how to set them safely. */
		table(tablecollection<valuetype> &a);

		/** Makes this instance of the table class
		 *  identical to "a".
		 *
		 *  Values are read from "a" using getValue(), in row
		 *  order.
		 *
		 *  This table takes the manage-values, manage-array-values,
		 *  and copy-column-names flags from "a", and flags that were
		 *  set on this table have no effect.  If no manage flag is
		 *  set on "a", a copy of pointer values gets the same
		 *  pointers as "a", valid only while the data they point to
		 *  exists.  Unless "a" copies column names, the copy also
		 *  shares its column name pointers.  See the collection class
		 *  description. */
		table<valuetype>	&operator=(table<valuetype> &a);

		/** Makes this instance of the table class
		 *  identical to "a".
		 *
		 *  Values are read from "a" using getValue(), in row
		 *  order.  If not all of "a"'s rows are available
		 *  (getAllRowsAvailable() returns false), then rows are read
		 *  until getAllRowsAvailable() returns true or reading past
		 *  the current block doesn't make the row exist.
		 *
		 *  This table takes the manage-values, manage-array-values,
		 *  and copy-column-names flags from "a", and flags that were
		 *  set on this table have no effect.  If "a" is block-based,
		 *  then pointer values get pointers into the blocks of "a".
		 *  These can be invalid immediately, because this table loads
		 *  later blocks.  Call setManageArrayValues(true) and
		 *  setCopyColumnNames(true) on "a" first to duplicate the
		 *  strings and column names.  On a table, these flags also
		 *  make "a" delete its own values or column names.  See the
		 *  tablecollection class description for how to set them
		 *  safely. */
		table<valuetype>	&operator=(
					tablecollection<valuetype> &a);

		/** Deletes this instance of the table class.
		 *
		 *  Also deletes the values that are stored in the table if
		 *  setManageValues(true) or setManageArrayValues(true) was
		 *  called, and the copies of the column names if
		 *  setCopyColumnNames(true) was called. */
		~table();

		/** Indicates whether or not this instance of the
		 *  tablecollection class should make copies the values that
		 *  are passed in to setColumnName().
		 *
		 *  The copies are deleted when the column name is set again,
		 *  when clear() is called, and when the instance is deleted.
		 *
		 *  Defaults to "false".
		 *
		 *  Call this before any column names are set.  Changing it
		 *  after column names are set is unsafe, as the instance would
		 *  then delete names that it didn't copy, or leak names that it
		 *  did copy. */
		void	setCopyColumnNames(bool copy);

		/** Returns whether or not this instance of the tablecollection
 		 *  class is configured to make copies of the values that are
 		 *  passed in to setColumnName().
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getCopyColumnNames();

		/** Sets the name of column "col" to "name" */
		void		setColumnName(uint64_t col, const char *name);

		/** Returns the name of column "col", or NULL if column "col"
		 *  has no name. */
		const char	*getColumnName(uint64_t col);

		/** Returns the current number of columns in the table.
		 *  
		 *  Returns larger and larger values as calls to setColumnName()
		 *  or setValue() extend the table. */
		uint64_t	getColumnCount();

		/** Indicates whether or not this instance of the table
		 *  class should delete the values that are stored at each
		 *  location in the table when clear() is called.
		 *
		 *  Setting "manage" to true causes the instance to delete the
		 *  values that are stored at each location in the table
		 *  when clear() is called and when the instance is deleted.
		 *
		 *  Setting "manage" to false causes the instance not to delete
		 *  the values that are stored at each location in the
		 *  table when clear() is called or when the instance is
		 *  deleted.
		 *
		 *  Defaults to "false".
		 *
		 *  Note that setting this true implies
		 *  setManageArrayValues(false); */
		void	setManageValues(bool manage);

		/** Returns whether or not this instance of the table class
		 *  is configured to delete the values that are stored at each
		 *  location in the table when clear() is called and when the
		 *  instance is deleted.
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getManageValues();

		/** Indicates whether or not this instance of the table
		 *  class should array-delete the values that are stored at each
		 *  location in the table when clear() is called.
		 *
		 *  Setting "manage" to true causes the instance to
		 *  array-delete the values that are stored at each location in
		 *  the table when clear() is called and when the instance is
		 *  deleted.
		 *
		 *  Setting "manage" to false causes the instance not to
		 *  array-delete the values that are stored at each location in
		 *  the table when clear() is called or when the instance is
		 *  deleted.
		 *
		 *  Defaults to "false".
		 *
		 *  When a table that manages array values is copied, an array
		 *  of char, wchar_t, or byte_t is duplicated (a byte_t array
		 *  only up to its first zero byte).  An array of any
		 *  other type can't be duplicated without its length, so the
		 *  copy gets NULL for it.
		 *
		 *  Note that setting this true implies
		 *  setManageValues(false); */
		void	setManageArrayValues(bool manage);

		/** Returns whether or not this instance of the table class
		 *  is configured to array-delete the values that are stored at
		 *  each location in the table when clear() is called and when
		 *  the instance is deleted.
		 *
		 *  Returns true if it is and false if it is not. */
		bool	getManageArrayValues();

		/** Sets the value at "row", "col" to "value".
		 *
		 *  Note that this doesn't delete the value that was previously
		 *  stored at "row", "col", even if setManageValues(true) or
		 *  setManageArrayValues(true) was called.  The caller must
		 *  delete it, if necessary. */
		void		setValue(uint64_t row, uint64_t col,
							valuetype value);

		/** Returns the value at "row", "col".  Returns NULL or 0 if
		 *  there is no value at that address. */
		valuetype	getValue(uint64_t row,
						uint64_t col);

		/** Returns a reference to the value at "row", "col". */
		valuetype	&getReference(uint64_t row,
						uint64_t col);

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		valuetype	getValue(uint64_t row,
						const char *colname);

		/** Returns the current number of rows in the table.
		 *
		 *  Returns larger and larger values as calls to setValue()
		 *  extend the table. */
		uint64_t	getRowCount();

		/** Returns true. */
		bool		getAllRowsAvailable();

		/** Removes all values and column names currently stored in the
		 *  table, such that getValue() and getColumnName() will return
		 *  NULL or 0.  Deletes the values and column names as
		 *  configured by setManageValues(), setManageArrayValues(),
		 *  and setCopyColumnNames().  Always returns true. */
		bool	clear();

	#include <rudiments/private/table.h>
};

#include <rudiments/private/tableinlines.h>

#endif
