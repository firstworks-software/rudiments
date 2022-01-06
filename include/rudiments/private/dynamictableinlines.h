// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
dynamictable<valuetype>::dynamictable() :
	tablecollection<valuetype>(),
	cols(0),
	rows(0) {
}

template <class valuetype>
inline
dynamictable<valuetype>::dynamictable(const dynamictable<valuetype> &a) :
						tablecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
dynamictable<valuetype> &dynamictable<valuetype>::operator=(
					const dynamictable<valuetype> &a) {
	if (this!=&a) {
		clear();
		tablecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void dynamictable<valuetype>::clone(const dynamictable<valuetype> *table) {
	columnnames=&(table->columnnames);
	values=&(table->values);
	cols=table->cols;
	rows=table->rows;
}

template <class valuetype>
inline
dynamictable<valuetype>::~dynamictable() {
	for (uint64_t i=0; i<cols; i++) {
		delete[] columnnames[i];
	}
}

template <class valuetype>
inline
void dynamictable<valuetype>::setColumnName(uint64_t col, const char *name) {
	if (cols && col<cols-1) {
		delete[] columnnames[col];
	}
	columnnames[col]=charstring::duplicate(name);
	if (col>=cols) {
		cols=col+1;
	}
}

template <class valuetype>
inline
const char *dynamictable<valuetype>::getColumnName(uint64_t col) {
	return (col<cols)?columnnames[col]:NULL;
}

template <class valuetype>
inline
uint64_t dynamictable<valuetype>::getColCount() {
	return cols;
}

template <class valuetype>
inline
void dynamictable<valuetype>::setValue(uint64_t row, uint64_t col, valuetype value) {
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
valuetype dynamictable<valuetype>::getValue(uint64_t row, uint64_t col) {
	return (row<rows && col<cols)?values[row][col]:((valuetype)0);
}

template <class valuetype>
inline
valuetype dynamictable<valuetype>::getValue(uint64_t row, const char *colname) {
	// FIXME: inefficient
	for (uint64_t i=0; i<cols; i++) {
		if (!charstring::compare(colname,columnnames[i])) {
			return getValue(row,i);
		}
	}
	return (valuetype)0;
}

template <class valuetype>
inline
uint64_t dynamictable<valuetype>::getRowCount() {
	return rows;
}

template <class valuetype>
inline
bool dynamictable<valuetype>::getAllRowsAvailable() {
	return true;
}

template <class valuetype>
inline
void dynamictable<valuetype>::clear() {
	for (uint64_t i=0; i<rows; i++) {
		values[i].clear();
	}
	values.clear();
	cols=0;
	rows=0;
}
