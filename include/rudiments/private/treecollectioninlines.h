// Copyright (c) David Muse
// See the COPYING file for more information

template <class valuetype>
inline
treecollection<valuetype>::treecollection() : nodecollection<valuetype>() {
}

template <class valuetype>
inline
treecollection<valuetype>::treecollection(nodecollection<valuetype> &n) :
						nodecollection<valuetype>(n) {
}

template <class valuetype>
inline
const char *treecollection<valuetype>::getType() {
	return "tree";
}

template <class valuetype>
inline
void treecollection<valuetype>::insert(valuetype *values, uint64_t count) {
	for (uint64_t i=0; i<count; i++) {
		insert(values[i]);
	}
}

template <class valuetype>
inline
ssize_t treecollection<valuetype>::write() {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t treecollection<valuetype>::write(output *out) {
	if (!getTop()) {
		ssize_t	retval=0;
		if (!this->incOrErr(&retval,out->write("<t/>\n",5),5) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
		return retval;
	}
	uint16_t	indentlevel=0;
	return writeNodeXml(out,getTop(),"t",&indentlevel,true,true);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeJson(output *out, bool indent) {
	if (!getTop()) {
		ssize_t	retval=0;
		if (!(this->incOrErr(&retval,out->write('['),1) &&
			((indent)?this->incOrErr(&retval,
					out->write('\n'),1):true) &&
			this->incOrErr(&retval,out->write(']'),1) &&
			((indent)?this->incOrErr(&retval,
					out->write('\n'),1):true)) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
		return retval;
	}
	uint16_t	indentlevel=0;
	return writeNodeJson(out,getTop(),&indentlevel,indent);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml() {
	return writeXml(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml(bool indent) {
	return writeXml(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml(output *out) {
	return writeXml(out,true);
}

template< class valuetype >
inline
ssize_t treecollection<valuetype>::writeXml(output *out, bool indent) {
	if (!getTop()) {
		ssize_t	retval=0;
		if (!this->incOrErr(&retval,out->write("<t/>\n",5),5) &&
			retval>-1) {
			retval=RESULT_ERROR;
		}
		return retval;
	}
	uint16_t	indentlevel=0;
	return writeNodeXml(out,getTop(),"t",&indentlevel,false,indent);
}
