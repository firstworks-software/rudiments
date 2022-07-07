// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::write() const {
	return write(&stdoutput);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::write(output *out) const {
	ssize_t	retval=0;
	for (listnode<keytype> *node=getKeys()->getFirst();
		node &&
		incOrErr(&retval,
			this->writeValue(out,node->getValue())) &&
		incOrErr(&retval,out->write(':')) &&
		incOrErr(&retval,
			this->writeValue(out,getValue(node->getValue()))) &&
		incOrErr(&retval,out->write('\n'));
		node=node->getNext()) {
	}
	return retval;
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson() const {
	return writeJson(&stdoutput,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(output *out) const {
	return writeJson(out,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(output *out,
							bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeXml() const {
	return writeXml(&stdoutput,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeXml(output *out) const {
	return writeXml(out,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeXml(output *out,
							bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
