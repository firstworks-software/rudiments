// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class valuetype >
inline
const char *arraycollection<valuetype>::getType() {
	return "array";
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::write() {
	return write(&stdoutput);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::write(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t arraycollection<valuetype>::writeJson(output *out, bool indent) {
	ssize_t		retval=0;
	stringbuffer	buf;
	if (!incOrErr(&retval,out->write('['),1) && retval>-1) {
		retval=RESULT_ERROR;
	}
	for (uint64_t i=0; i<getCount() && retval>-1; i++) {
		if (!(((i)?this->incOrErr(&retval,out->write(','),1):true) &&
			((indent)?this->incOrErr(&retval,
					out->write("\n	",2),2):true) &&
			this->writeJsonValue(&retval,out,&buf,(*this)[i])) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
	}
	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true) &&
		incOrErr(&retval,out->write(']'),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}
	return retval;
}
