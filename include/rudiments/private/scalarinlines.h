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
scalar<valuetype>::scalar(scalar<valuetype> &a) :
				scalarcollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
scalar<valuetype>::scalar(scalarcollection<valuetype> &a) :
				scalarcollection<valuetype>(a) {
	clone(&a);
}

template <class valuetype>
inline
scalar<valuetype> &scalar<valuetype>::operator=(
				scalar<valuetype> &a) {
	if (this!=&a) {
		clear();
		scalarcollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
scalar<valuetype> &scalar<valuetype>::operator=(
				scalarcollection<valuetype> &a) {
	if (this!=&a) {
		clear();
		scalarcollection<valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class valuetype>
inline
void scalar<valuetype>::clone(scalarcollection<valuetype> *a) {
	v=node_duplicate_value(&(a->getReference()),
				this->getManageValues(),
				this->getManageArrayValues());
}

template <class valuetype>
inline
scalar<valuetype>::~scalar() {
	node_delete_value(&v,this->getManageValues(),
					this->getManageArrayValues());
}

template <class valuetype>
inline
void scalar<valuetype>::setValue(valuetype v) {
	this->v=v;
}

template <class valuetype>
inline
valuetype scalar<valuetype>::getValue() {
	return v;
}

template <class valuetype>
inline
valuetype &scalar<valuetype>::getReference() {
	return v;
}

template <class valuetype>
inline
bool scalar<valuetype>::clear() {
	node_delete_value(&v,this->getManageValues(),
					this->getManageArrayValues());
	v=0;
	return true;
}
