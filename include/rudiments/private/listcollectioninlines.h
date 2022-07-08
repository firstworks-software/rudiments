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
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(output *out, bool indent) const {
	ssize_t			retval=0;
	uint64_t		i=0;
	listnode<valuetype>	*node=getFirst();
	this->incOrErr(&retval,out->write('['));
	while (node && retval>-1) {
		((i)?this->incOrErr(&retval,out->write(',')):true) &&
		((indent)?this->incOrErr(&retval,out->write("\n	")):true) &&
		this->incOrErr(&retval,
			this->writeJsonValue(out,node->getValue()));
		node=node->getNext();
		i++;
	}
	((indent)?this->incOrErr(&retval,out->write('\n')):true) &&
	this->incOrErr(&retval,out->write(']')) &&
	((indent)?this->incOrErr(&retval,out->write('\n')):true);
	return retval;
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml(output *out) const {
	return writeXml(out,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeXml(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
