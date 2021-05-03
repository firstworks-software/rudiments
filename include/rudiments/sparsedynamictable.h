// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SPARSEDYNAMICTABLE_H
#define RUDIMENTS_SPARSEDYNAMICTABLE_H

#include <rudiments/private/sparsedynamictableincludes.h>

template <class valuetype>
class RUDIMENTS_DLLSPEC sparsedynamictable : public tablecollection<valuetype> {
	public:

		/** Creates an instance of the sparsedynamictable class. */
		sparsedynamictable();

		/** Deletes this instance of the sparsedynamictable class. */
		~sparsedynamictable();

		uint64_t	getRowCount();
		uint64_t	getColCount();
		void		setValue(uint64_t row, uint64_t col,
							valuetype value);
		valuetype	getValue(uint64_t row, uint64_t col);

		void		setColumnName(uint64_t col, const char *name);
		const char	*getColumnName(uint64_t col);
		void		clear();

	#include <rudiments/private/sparsedynamictable.h>
};

#include <rudiments/private/sparsedynamictableinlines.h>

#endif
