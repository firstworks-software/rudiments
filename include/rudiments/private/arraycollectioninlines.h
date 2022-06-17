// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
bool arraycollection<valuetype>::write(output *out) const {
	for (uint64_t i=0; i<getLength(); i++) {
		out->printf("%lld: ",i);
		this->getWriter()->write((*this)[i]);
		out->write('\n');
	}
	return true;
}
