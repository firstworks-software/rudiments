// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

template <class valuetype>
inline
const char *scalarcollection<valuetype>::getType() const {
	return "scalar";
}

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write() const {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson() const {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(bool indent) const {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(output *out) const {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(output *out, bool indent) const {
	ssize_t	retval=0;
	incOrErr(&retval,out->write('{'),1) &&
	((indent)?incOrErr(&retval,out->write("\n	",2),2):true) &&
	incOrErr(&retval,out->write("\"v\":",4),4) &&
	((indent)?incOrErr(&retval,out->write(' '),1):true) &&
	incOrErr(&retval,this->writeJsonValue(out,getValue())) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true) &&
	incOrErr(&retval,out->write('}'),1) &&
	((indent)?incOrErr(&retval,out->write('\n'),1):true);
	return retval;
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeXml() const {
	return writeXml(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeXml(bool indent) const {
	return writeXml(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeXml(output *out) const {
	return writeXml(out,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeXml(output *out, bool indent) const {
	// FIXME: implement this
	return RESULT_ERROR;
}
