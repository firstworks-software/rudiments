// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/scalar.h>
#include <rudiments/private/nodeinlines.h>

template <class valuetype>
inline
scalar<valuetype>::scalar() : scalarcollection<valuetype>() {
}

template <class valuetype>
inline
scalar<valuetype>::scalar(const scalar<valuetype> &a) :
				scalarcollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
scalar<valuetype> &scalar<valuetype>::operator=(const scalar<valuetype> &a) {
	if (this!=&a) {
		clear();
		scalarcollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void scalar<valuetype>::clone(const scalar<valuetype> *a) {
	if (this->collection::managevalues) {
		v=node_duplicate_value(a->getValue());
	} else if (this->collection::managearrayvalues) {
		v=node_duplicate_array_value(a->getValue());
	} else {
		v=a->getValue();
	}
}

template <class valuetype>
inline
scalar<valuetype>::~scalar() {
	if (this->collection::managevalues) {
		node_delete_value(v);
	} else if (this->collection::managearrayvalues) {
		node_delete_array_value(v);
	}
}

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
	if (this->collection::managevalues) {
		node_delete_value(v);
	} else if (this->collection::managearrayvalues) {
		node_delete_array_value(v);
	}
	v=0;
}
