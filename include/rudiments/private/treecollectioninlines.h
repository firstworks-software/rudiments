// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

template <class valuetype>
inline
void treecollection<valuetype>::insert(valuetype *values, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		insert(values[i]);
	}
}

template <class valuetype>
inline
ssize_t treecollection<valuetype>::write() const {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t treecollection<valuetype>::write(output *out) const {
	uint16_t	indentlevel=0;
	return writeNode(out,getTop(),"top",&indentlevel);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml(output *out) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
