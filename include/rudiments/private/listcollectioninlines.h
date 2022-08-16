// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

template <class valuetype>
inline
listcollection<valuetype>::listcollection() : nodecollection<valuetype>() {
}

template <class valuetype>
inline
listcollection<valuetype>::listcollection(nodecollection<valuetype> &n) :
						nodecollection<valuetype>(n) {
}

template <class valuetype>
inline
const char *listcollection<valuetype>::getType() {
	return "list";
}

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
ssize_t listcollection<valuetype>::write() {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t listcollection<valuetype>::write(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t listcollection<valuetype>::writeJson(output *out, bool indent) {
	ssize_t			retval=0;
	uint64_t		i=0;
	listnode<valuetype>	*node=getFirst();
	this->incOrErr(&retval,out->write('['),1);
	while (node && retval>-1) {
		((i)?this->incOrErr(&retval,out->write(','),1):true) &&
		((indent)?this->incOrErr(&retval,out->write("\n	",2),2):true) &&
		this->incOrErr(&retval,
			this->writeJsonValue(out,node->getValue()));
		node=node->getNext();
		i++;
	}
	((indent)?this->incOrErr(&retval,out->write('\n'),1):true) &&
	this->incOrErr(&retval,out->write(']'),1) &&
	((indent)?this->incOrErr(&retval,out->write('\n'),1):true);
	return retval;
}
