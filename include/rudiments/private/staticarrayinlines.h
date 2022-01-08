// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray() : arraycollection<valuetype>() {
	len=length;
	data=new valuetype[len];
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray(
				const staticarray<valuetype,length> &v) :
						arraycollection<valuetype>(v) {
	len=length;
	data=new valuetype[len];
	clone(&v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::staticarray(
				const arraycollection<valuetype> &v) :
						arraycollection<valuetype>(v) {
	len=length;
	data=new valuetype[len];
	clone(&v);
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length> &staticarray<valuetype,length>::operator=(
				const staticarray<valuetype,length> &v) {
	if (this!=&v) {
		clear();
		arraycollection<valuetype>::operator=(v);
		clone(&v);
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
		clone(&v);
	}
	return *this;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::clone(const arraycollection<valuetype> *v) {
	for (uint64_t i=0; i<len && i<v->getLength(); i++) {

		// Why not just:
		//	*data[i]=(*v)[i];
		//
		// Some compilers get confused and think that
		//	*data[i]=(*v)[i]
		//		means
		//	*((data[i])->operator=((*v)[i]))
		// and no carefully placed parentheses help.
		//
		// This silliness sorts out the problem.
		valuetype	*a=&(data[i]);
		valuetype	b=(*v)[i];
		*a=b;
	}
}

template< class valuetype, uint64_t length >
inline
staticarray<valuetype,length>::~staticarray() {
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
	return len;
}

template< class valuetype, uint64_t length >
inline
void staticarray<valuetype,length>::clear() {
	for (uint64_t i=0; i<len; i++) {
		((valuetype *)&data[i])->~valuetype();
		new(&data[i]) valuetype;
	}
}
