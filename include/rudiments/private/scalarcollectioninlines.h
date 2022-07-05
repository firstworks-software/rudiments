// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write() const {
	return write(&stdoutput);
}

template <class valuetype>
inline
ssize_t scalarcollection<valuetype>::write(output *out) const {
	ssize_t	retval=0;
	retval+=this->writeDelegate(out,getValue());
	retval+=out->write('\n');
	return retval;
}
