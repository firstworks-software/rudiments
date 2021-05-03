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

		virtual	uint64_t	getRowCount()=0;
		virtual	uint64_t	getColCount()=0;
		virtual	void		setValue(uint64_t row,
							uint64_t col,
							valuetype value)=0;
		virtual	valuetype	getValue(uint64_t row, uint64_t col)=0;

		virtual	void		setColumnName(uint64_t col,
							const char *name)=0;
		virtual	const char	*getColumnName(uint64_t col)=0;
		virtual	void		clear()=0;
};

#endif
