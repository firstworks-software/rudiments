// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>
#include <rudiments/private/nodeinlines.h>

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray() : arraycollection<valuetype>() {
	data=new valuetype[length];
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray(
				staticarray<valuetype,length> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[length];
	clone(v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray(
				arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[length];
	clone(v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length> &staticarray<valuetype,length>::operator=(
				staticarray<valuetype,length> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(v);
	}
	return *this;
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length> &staticarray<valuetype,length>::operator=(
					arraycollection<valuetype> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(v);
	}
	return *this;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::clone(arraycollection<valuetype> &v) {
	for (uint64_t i=0; i<length && i<v.getLength(); i++) {
		data[i]=node_duplicate_value(&(v[i]),
					this->getManageValues(),
					this->getManageArrayValues());
	}
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::~staticarray() {
	deleteManagedValues();
	delete[] data;
}

template< class valuetype, uint64_t length >
inline
valuetype &staticarray<valuetype,length>::operator[](uint64_t index) {
	// I once had (semi-clever) bounds-checking code here like:
	//
	// if (index>=length) {
	//	return *((valuetype *)NULL);
	// }
	//
	// which would successfully return a NULL/0 if valuetype was a pointer,
	// but would throw warnings on some platforms if valuetype isn't a
	// pointer, and would probably crash on those platforms if it managed
	// to run.
	//
	// It's (apparently) conventional for the operator[] not to do any
	// bounds checking, not even throw any exceptions, and and just attempt
	// to access and return the requested index, possibly crashing in the
	// process.  When a program accesses via the [] opertator it's telling
	// the compiler "trust me, I know what I'm doing".
	//
	// So, for now, I removed that code above and we'll just let whatever
	// happens, happen.
	return data[index];
}

template< class valuetype, uint64_t length >
inline
uint64_t staticarray<valuetype,length>::getLength() {
	return length;
}

template< class valuetype, uint64_t length >
inline
bool staticarray<valuetype,length>::clear() {
	deleteManagedValues();
	for (uint64_t i=0; i<length; i++) {
		((valuetype *)&data[i])->~valuetype();
		new(&data[i]) valuetype;
	}
	return true;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::deleteManagedValues() {
	if (this->getManageValues() || this->getManageArrayValues()) {
		for (uint64_t i=0; i<length; i++) {
			node_delete_value(&(data[i]),
						this->getManageValues(),
						this->getManageArrayValues());
			node_zero_value(&(data[i]));
		}
	}
}
