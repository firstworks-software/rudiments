// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/md5.h>
#else
	#include "md5rfc1321.cpp"
#endif

class md5private {
	friend class md5;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			MD5_CTX		_context;
		#else
			MD5_CONTEXT	_context;
			MD5		_md5;
		#endif
		char		*_string;
		unsigned char	_hash[16];
};

md5::md5() {
	pvt=new md5private;
	pvt->_string=NULL;
	clear();
}

md5::~md5() {
	delete pvt;
}

bool md5::append(const unsigned char *data, uint32_t length) {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Update(&pvt->_context,data,length)) {
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		pvt->_md5.MD5Update(&pvt->_context,data,length);
		return true;
	#endif
}

const unsigned char *md5::getHash() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Final(pvt->_hash,&pvt->_context)) {
			// FIXME: set error...
			return (const unsigned char *)"";
		}
	#else
		pvt->_md5.MD5Final(pvt->_hash,&pvt->_context);
	#endif
	return pvt->_hash;
	//delete[] pvt->_string;
	//pvt->_string=charstring::hexEncode(pvt->_hash,sizeof(pvt->_hash));
	//return pvt->_string;
}

uint64_t md5::getHashLength() {
	return sizeof(pvt->_hash);
}

bool md5::clear() {
	delete[] pvt->_string;
	pvt->_string=charstring::duplicate("");
	bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Init(&pvt->_context)) {
			// FIXME: set error
			return false;
		}
	#else
		pvt->_md5.MD5Init(&pvt->_context);
	#endif
	return true;
}

hasherror_t md5::getError() {
	return HASH_ERROR_SUCCESS;
}
