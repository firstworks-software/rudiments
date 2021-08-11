// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/scalar.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
void scalar<valuetype>::setValue(valuetype v) {
	this->v=v;
}

template <class valuetype>
inline
valuetype scalar<valuetype>::getValue() const {
	return v;
}

template <class valuetype>
inline
void scalar<valuetype>::clear() {
	if (collection::managevalues) {
		node_delete_value(v);
	} else if (collection::managearrayvalues) {
		node_delete_array_value(v);
	}
	v=0;
}
