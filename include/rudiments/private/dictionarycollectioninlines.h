// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class keytype, class valuetype >
inline
ssize_t dictionarycollection<keytype,valuetype>::write(output *out) const {
	ssize_t	retval=0;
	for (listnode<keytype> *node=getKeys()->getFirst();
					node; node=node->getNext()) {
		retval+=this->writeDelegate(out,node->getValue());
		retval+=out->write(':');
		retval+=this->writeDelegate(out,getValue(node->getValue()));
		retval+=out->write('\n');
	}
	return retval;
}
