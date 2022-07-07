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
	for (uint64_t i=0;
		i<getLength() &&
		incOrErr(&retval,out->printf("%lld: ",i)) &&
		incOrErr(&retval,this->writeValue(out,(*this)[i])) &&
		incOrErr(&retval,out->write('\n'));
		i++) {
	}
	return retval;
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml(output *out) const {
	return writeXml(out,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeXml(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
