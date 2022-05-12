// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_TREECOLLECTIONINLINES_H
#define RUDIMENTS_TREECOLLECTIONINLINES_H

template <class valuetype>
inline
void treecollection<valuetype>::insert(valuetype *values, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		insert(values[i]);
	}
}

#endif
