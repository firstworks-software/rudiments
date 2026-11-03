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
	stringbuffer		buf;
	if (!incOrErr(&retval,out->write('{'),1) && retval>-1) {
		retval=RESULT_ERROR;
	}
	while (node && retval>-1) {
		if (!(((i)?incOrErr(&retval,out->write(','),1):true) &&
			((indent)?incOrErr(&retval,
					out->write("\n	",2),2):true) &&
			this->writeJsonValue(&retval,out,&buf,
						node->getValue()) &&
			incOrErr(&retval,out->write(':'),1) &&
			this->writeJsonValue(&retval,out,&buf,
					getValue(node->getValue()))) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
 		node=node->getNext();
		i++;
	}
	if (!(retval>-1 &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true) &&
		incOrErr(&retval,out->write('}'),1) &&
		((indent)?incOrErr(&retval,out->write('\n'),1):true)) &&
		retval>-1) {
		retval=RESULT_ERROR;
	}
	return retval;
}
