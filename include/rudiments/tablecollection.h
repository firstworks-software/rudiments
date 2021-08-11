// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TABLECOLLECTION_H
#define RUDIMENTS_TABLECOLLECTION_H

#include <rudiments/nodecollection.h>

/** The tablecollection class is the parent class for all rudiments table
 *  collections. */
template <class valuetype>
class RUDIMENTS_DLLSPEC tablecollection : public nodecollection {
	public:

		/** Creates an instance of the tablecollection class. */
		tablecollection() : nodecollection() {};

		/** Creates an instance of the tablecollection class. */
		tablecollection(const collection &c) : nodecollection(c) {};

		/** Deletes this instance of the tablecollection class. */
		virtual	~tablecollection() {};

		/** Sets the name of column "col" to "name" */
		virtual	void		setColumnName(uint64_t col,
							const char *name)=0;

		/** Returns the name of column "col". */
		virtual	const char	*getColumnName(uint64_t col)=0;

		/** Returns the number of columns in the table. */
		virtual	uint64_t	getColCount()=0;
		
		/** Sets the value at "row", "col" to "value". */
		virtual	void		setValue(uint64_t row,
							uint64_t col,
							valuetype value)=0;

		/** Returns the value at "row", "col".  Returns NULL or 0 if
		 *  there is no value at that address. */
		virtual	valuetype	getValue(uint64_t row,
							uint64_t col)=0;

		/** Returns the value at "row", "colname".  Returns NULL or 0
		 *  if there is no value at that address. */
		virtual	valuetype	getValue(uint64_t row,
							const char * colname)=0;

		/** Returns the current number of rows in the table.
		 *
		 *  Note that some implementations may not load all rows
		 *  immediately.  getRowCount() only returns the total number
		 *  of rows in the table when allRowsAvailable() returns
		 *  true. */
		virtual	uint64_t	getRowCount()=0;

		/** Returns true of getRowCount() will return the total number
		 *  of rows in the table or false if getRowCount() will only
		 *  return the current number of rows in the table. */
		virtual	bool		allRowsAvailable()=0;

		/** Removes all values currently stored in the table, such that
		 *  getValue() will return NULL or 0. */
		virtual	void		clear()=0;
};

#endif
