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
				const staticarray<valuetype,length> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[length];
	clone(v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray(
				const arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	data=new valuetype[length];
	clone(v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length> &staticarray<valuetype,length>::operator=(
				const staticarray<valuetype,length> &v) {
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
					const arraycollection<valuetype> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(v);
	}
	return *this;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::clone(const arraycollection<valuetype> &v) {
	for (uint64_t i=0; i<length && i<v.getLength(); i++) {
		data[i]=node_duplicate_value(v[i],
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
	return data[index];
}

template< class valuetype, uint64_t length >
inline
valuetype staticarray<valuetype,length>::operator[](uint64_t index) const {
	return data[index];
}

template< class valuetype, uint64_t length >
inline
uint64_t staticarray<valuetype,length>::getLength() const {
	return length;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::clear() {
	deleteManagedValues();
	for (uint64_t i=0; i<length; i++) {
		((valuetype *)&data[i])->~valuetype();
		new(&data[i]) valuetype;
	}
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
