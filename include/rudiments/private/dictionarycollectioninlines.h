// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class keytype, class valuetype >
inline
const char *dictionarycollection<keytype,valuetype>::getType() {
	return "dictionary";
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::write() {
	return write(&stdoutput);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::write(output *out) {
	return writeJson(out,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::writeJson(output *out,
							bool indent) {
	ssize_t			retval=0;
	uint64_t		i=0;
	listnode<keytype>	*node=getKeys()->getFirst();
	incOrErr(&retval,out->write('{'),1);
	while (node && retval>-1) {
		((i)?incOrErr(&retval,out->write(','),1):true) &&
		((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
		incOrErr(&retval,
			this->writeJsonValue(out,node->getValue())) &&
		incOrErr(&retval,out->write(':'),1) &&
		incOrErr(&retval,
			this->writeJsonValue(out,getValue(node->getValue())));
 		node=node->getNext();
		i++;
	}
	((indent)?incOrErr(&retval,out->write('\n'),1):true) &&
	incOrErr(&retval,out->write('}'),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);
	return retval;
}
