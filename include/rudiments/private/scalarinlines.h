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
	v=node_duplicate_value(a->getValue(),
			this->collection::managevalues,
			this->collection::managearrayvalues);
}

template <class valuetype>
inline
scalar<valuetype>::~scalar() {
	node_delete_value(v,
		this->collection::managevalues,
		this->collection::managearrayvalues);
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
valuetype &scalar<valuetype>::getValue() {
	return v;
}

template <class valuetype>
inline
void scalar<valuetype>::clear() {
	node_delete_value(v,
		this->collection::managevalues,
		this->collection::managearrayvalues);
	v=0;
}
