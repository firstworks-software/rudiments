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
	return writeJson(out,true);
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
	ssize_t			retval=0;
	uint64_t		i=0;
	listnode<keytype>	*node=getKeys()->getFirst();
	incOrErr(&retval,out->write('{'));
	while (node && retval>-1) {
		((i)?incOrErr(&retval,out->write(',')):true) &&
		((indent)?incOrErr(&retval,out->write("\n	")):true) &&
		incOrErr(&retval,
			this->writeJsonValue(out,node->getValue())) &&
		incOrErr(&retval,out->write(':')) &&
		incOrErr(&retval,
			this->writeJsonValue(out,getValue(node->getValue())));
 		node=node->getNext();
		i++;
	}
	((indent)?incOrErr(&retval,out->write('\n')):true) &&
	incOrErr(&retval,out->write('}')) &&
	((indent)?incOrErr(&retval,out->write('\n')):true);
	return retval;
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
