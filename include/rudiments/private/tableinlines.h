// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
table<valuetype>::table() :
	tablecollection<valuetype>(),
	cols(0),
	rows(0) {
}

template <class valuetype>
inline
table<valuetype>::table(const table<valuetype> &a) :
				tablecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
table<valuetype>::table(const tablecollection<valuetype> &a) :
				tablecollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
table<valuetype> &table<valuetype>::operator=(const table<valuetype> &a) {
	if (this!=&a) {
		clear();
		tablecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
table<valuetype> &table<valuetype>::operator=(
					const tablecollection<valuetype> &a) {
	if (this!=&a) {
		clear();
		tablecollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void table<valuetype>::clone(const tablecollection<valuetype> *t) {
	cols=0;
	rows=0;
	for (uint64_t col=0; col<t->getColumnCount(); col++) {
		setColumnName(col,t->getColumnName(col));
	}
	for (uint64_t row=0;
		!(t->getAllRowsAvailable() && row==t->getRowCount());
		row++) {
		for (uint64_t col=0; col<t->getColumnCount(); col++) {
			setValue(row,col,
				node_duplicate_value(t->getValue(row,col),
						this->getManageValues(),
						this->getManageArrayValues()));
		}
	}
}

template <class valuetype>
inline
table<valuetype>::~table() {
	if (this->copycolumnnames) {
		for (uint64_t i=0; i<cols; i++) {
			delete[] columnnames[i];
		}
	}
}

template <class valuetype>
inline
void table<valuetype>::setColumnName(uint64_t col, const char *name) {
	if (this->copycolumnnames) {
		if (cols && col<cols-1) {
			delete[] columnnames[col];
		}
		columnnames[col]=charstring::duplicate(name);
	} else {
		columnnames[col]=(char *)name;
	}
	if (col>=cols) {
		cols=col+1;
	}
}

template <class valuetype>
inline
const char *table<valuetype>::getColumnName(uint64_t col) const {
	return (col<cols)?columnnames[col]:NULL;
}

template <class valuetype>
inline
uint64_t table<valuetype>::getColumnCount() const {
	return cols;
}

template <class valuetype>
inline
void table<valuetype>::setValue(uint64_t row, uint64_t col, valuetype value) {
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
valuetype table<valuetype>::getValue(uint64_t row, uint64_t col) const {
	return (row<rows && col<cols)?values[row][col]:((valuetype)0);
}

template <class valuetype>
inline
valuetype table<valuetype>::getValue(uint64_t row, const char *colname) const {
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
uint64_t table<valuetype>::getRowCount() const {
	return rows;
}

template <class valuetype>
inline
bool table<valuetype>::getAllRowsAvailable() const {
	return true;
}

template <class valuetype>
inline
void table<valuetype>::clear() {
	for (uint64_t i=0; i<rows; i++) {
		values[i].clear();
	}
	values.clear();
	cols=0;
	rows=0;
}
