// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>
#include <rudiments/private/nodeinlines.h>
#include <rudiments/bytestring.h>

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count>::staticarray() : arraycollection<valuetype>() {
	data=new valuetype[count];
}

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count>::staticarray(
				staticarray<valuetype,count> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[count];
	clone(v);
}

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count>::staticarray(
				arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[count];
	clone(v);
}

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count> &staticarray<valuetype,count>::operator=(
				staticarray<valuetype,count> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(v);
	}
	return *this;
}

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count> &staticarray<valuetype,count>::operator=(
					arraycollection<valuetype> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(v);
	}
	return *this;
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::clone(arraycollection<valuetype> &v) {
	for (uint64_t i=0; i<count && i<v.getCount(); i++) {
		data[i]=node_duplicate_value(&(v[i]),
					this->getManageValues(),
					this->getManageArrayValues());
	}
}

template< class valuetype, uint64_t count >
inline
staticarray<valuetype,count>::~staticarray() {
	deleteManagedValues();
	delete[] data;
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::zero() {
	set(0,0,count);
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::zero(uint64_t zerocount) {
	set(0,0,zerocount);
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::zero(uint64_t start, uint64_t zerocount) {
	set(0,start,zerocount);
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::set(byte_t value) {
	set(value,0,count);
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::set(byte_t value, uint64_t setcount) {
	set(value,0,setcount);
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::set(byte_t value,
				uint64_t start, uint64_t setcount) {
	bytestring::set(data,value+start,sizeof(valuetype)*setcount);
}

template< class valuetype, uint64_t count >
inline
valuetype &staticarray<valuetype,count>::operator[](uint64_t index) {
	// I once had (semi-clever) bounds-checking code here like:
	//
	// if (index>=count) {
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

template< class valuetype, uint64_t count >
inline
uint64_t staticarray<valuetype,count>::getCount() {
	return count;
}

template< class valuetype, uint64_t count >
inline
bool staticarray<valuetype,count>::clear() {
	deleteManagedValues();
	for (uint64_t i=0; i<count; i++) {
		((valuetype *)&data[i])->~valuetype();
		new(&data[i]) valuetype;
	}
	return true;
}

template< class valuetype, uint64_t count >
inline
void staticarray<valuetype,count>::deleteManagedValues() {
	if (this->getManageValues() || this->getManageArrayValues()) {
		for (uint64_t i=0; i<count; i++) {
			node_delete_value(&(data[i]),
						this->getManageValues(),
						this->getManageArrayValues());
			node_zero_value(&(data[i]));
		}
	}
}
