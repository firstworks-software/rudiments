// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::write(output *out) const {
	ssize_t	retval=0;
	for (uint64_t i=0; i<getLength(); i++) {
		retval+=out->printf("%lld: ",i);
		retval+=this->writeDelegate(out,(*this)[i]);
		retval+=out->write('\n');
	}
	return retval;
}
