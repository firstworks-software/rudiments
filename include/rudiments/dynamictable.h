// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DYNAMICTABLE_H
#define RUDIMENTS_DYNAMICTABLE_H

#include <rudiments/private/dynamictableincludes.h>

template <class valuetype>
class RUDIMENTS_DLLSPEC dynamictable : public tablecollection<valuetype> {
	public:

		/** Creates an instance of the dynamictable class. */
		dynamictable();

		/** Deletes this instance of the dynamictable class. */
		~dynamictable();

		uint64_t	getRowCount();
		uint64_t	getColCount();
		void		setValue(uint64_t row, uint64_t col,
							valuetype value);
		valuetype	getValue(uint64_t row, uint64_t col);

		void		setColumnName(uint64_t col, const char *name);
		const char	*getColumnName(uint64_t col);
		void		clear();

	#include <rudiments/private/dynamictable.h>
};

#include <rudiments/private/dynamictableinlines.h>

#endif
