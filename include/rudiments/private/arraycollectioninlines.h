// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::write() const {
	return write(&stdoutput);
}

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

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
