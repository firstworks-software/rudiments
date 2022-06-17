// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

template <class valuetype>
inline
bool scalarcollection<valuetype>::write(output *out) const {
	this->getWriter()->write(getValue());
	out->write('\n');
	return true;
}
