// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICTABLE_H
#define RUDIMENTS_DYNAMICTABLE_H

#include <rudiments/private/dynamictableincludes.h>

/** The dynamictable class allows you to store an arbitrary number of values
 *  in a table.  Since the dynamictable class is template-based, you can 
 *  store arbitrary types of values.
 *
 *  Read-write, monolithic, random-access. */
template <class valuetype>
class dynamictable : public tablecollection<valuetype> {
	public:

		/** Creates an instance of the dynamictable class. */
		dynamictable();

		/** Creates an instance of the dynmaictable class
		 *  that is a copy of "a". */
		dynamictable(const dynamictable<valuetype> &a);

		/** Makes this instance of the dynamictable class
		 *  identical to "a". */
		dynamictable<valuetype>	&operator=(
					const dynamictable<valuetype> &a);

		/** Deletes this instance of the dynamictable class. */
		~dynamictable();

		/** Sets the name of column "col" to "name" */
		void		setColumnName(uint64_t col, const char *name);

		/** Returns the name of column "col". */
		const char	*getColumnName(uint64_t col) const;

		/** Returns the current number of columns in the table.
		 *  
		 *  Returns larger and larger values as calls to setColumnName()
		 *  or setValue() extend the table. */
		uint64_t	getColCount() const;

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

	#include <rudiments/private/dynamictable.h>
};

#include <rudiments/private/dynamictableinlines.h>

#endif
