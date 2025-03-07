// Copyright (c) David Muse
// See the COPYING file for more information.

template <class valuetype>
inline
const char *scalarcollection<valuetype>::getType() {
	return "scalar";
}
template <class valuetype>
inline
uint64_t scalarcollection<valuetype>::getCount() {
	return 1;
}

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write() {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson() {
	return writeJson(&stdoutput,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(bool indent) {
	return writeJson(&stdoutput,indent);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(output *out) {
	return writeJson(out,true);
}

template< class valuetype >
inline
ssize_t scalarcollection<valuetype>::writeJson(output *out, bool indent) {
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
