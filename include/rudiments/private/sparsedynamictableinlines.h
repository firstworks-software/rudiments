// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
sparsedynamictable<valuetype>::sparsedynamictable() :
				tablecollection<valuetype>() {
	cols=0;
	rows=0;
}

template <class valuetype>
inline
sparsedynamictable<valuetype>::~sparsedynamictable() {
	for (uint64_t i=0; i<cols; i++) {
		delete[] columnnames[i];
	}
}

template <class valuetype>
inline
void sparsedynamictable<valuetype>::setColumnName(uint64_t col,
							const char *name) {
	delete[] columnnames[col];
	columnnames[col]=charstring::duplicate(name);
	if (col>=cols) {
		cols=col+1;
	}
}

template <class valuetype>
inline
const char *sparsedynamictable<valuetype>::getColumnName(uint64_t col) {
	return columnnames[col];
}

template <class valuetype>
inline
uint64_t sparsedynamictable<valuetype>::getColCount() {
	return cols;
}

template <class valuetype>
inline
void sparsedynamictable<valuetype>::setValue(uint64_t row,
						uint64_t col,
						valuetype value) {
	values[row][col]=value;
	if (row>=rows) {
		rows=row+1;
	}
	if (col>=cols) {
		cols=col+1;
	}
}

template <class valuetype>
inline
valuetype sparsedynamictable<valuetype>::getValue(uint64_t row, uint64_t col) {
	return values[row][col];
}

template <class valuetype>
inline
uint64_t sparsedynamictable<valuetype>::getRowCount() {
	return rows;
}

template <class valuetype>
inline
bool sparsedynamictable<valuetype>::allRowsAvailable() {
	return true;
}

template <class valuetype>
inline
void sparsedynamictable<valuetype>::clear() {
	for (uint64_t i=0; i<rows; i++) {
		values[i].clear();
	}
	values.clear();
	cols=0;
	rows=0;
}
