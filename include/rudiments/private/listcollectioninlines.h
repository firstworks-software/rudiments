// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

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
ssize_t listcollection<valuetype>::write() const {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t listcollection<valuetype>::write(output *out) const {
	ssize_t		retval=0;
	uint64_t	i=0;
	for (listnode<valuetype> *current=getFirst();
				current; current=current->getNext()) {
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			retval+=out->printf("index %lld: ",(long long)i);
		#else
			retval+=out->printf("index %ld: ",(long)i);
		#endif
		retval+=this->writeDelegate(out,current->getValue());
		retval+=out->write('\n');
		i++;
	}
	return retval;
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
