// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/new.h>

template< class keytype, class valuetype >
inline
bool dictionarycollection<keytype,valuetype>::write(output *out) const {
	for (listnode<keytype> *node=getKeys()->getFirst();
					node; node=node->getNext()) {
		this->getWriter()->write(node->getValue());
		out->write(':');
		this->getWriter()->write(getValue(node->getValue()));
		out->write('\n');
	}
	return true;
}
