// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information
#ifndef RUDIMENTS_LISTCOLLECTIONINLINES_H
#define RUDIMENTS_LISTCOLLECTIONINLINES_H

template <class valuetype>
inline
void listcollection<valuetype>::prepend(valuetype *values, uint64_t count) {
	for (uint64_t i=count; i>0; i--) {
		prepend(values[i-1]);
	}
}

template <class valuetype>
inline
void listcollection<valuetype>::append(valuetype *values, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		append(values[i]);
	}
}

template <class valuetype>
inline
void listcollection<valuetype>::insertBefore(listnode<valuetype> *node,
					valuetype *values, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		insertBefore(node,values[i]);
	}
}

template <class valuetype>
inline
void listcollection<valuetype>::insertAfter(listnode<valuetype> *node,
					valuetype *values, uint64_t count) {
	for (uint64_t i=count; i>0; i--) {
		insertAfter(node,values[i-1]);
	}
}

template <class valuetype>
inline
bool listcollection<valuetype>::write(output *out) const {
	uint64_t	i=0;
	for (listnode<valuetype> *current=getFirst();
				current; current=current->getNext()) {
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			out->printf("index %lld: ",(long long)i);
		#else
			out->printf("index %ld: ",(long)i);
		#endif
		this->getWriter()->write(out,current->getValue());
		out->write('\n');
		i++;
	}
	return true;
}

#endif
