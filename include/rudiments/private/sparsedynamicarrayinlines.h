// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
sparsedynamicarray<valuetype>::sparsedynamicarray() :
					arraycollection<valuetype>() {
}

template< class valuetype >
inline
sparsedynamicarray<valuetype>::~sparsedynamicarray() {
}

template< class valuetype >
inline
valuetype &sparsedynamicarray<valuetype>::operator[](uint64_t index) {
	// FIXME: return a default value if not found
	return values.getValue(index);
}

template< class valuetype >
inline
uint64_t sparsedynamicarray<valuetype>::getLength() const {
	return values.getTree()->getLength();
}

template< class valuetype >
inline
void sparsedynamicarray<valuetype>::clear() {
	values.clear();
}
