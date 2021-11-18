// Copyright (c) 1999-2018 David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/md5.h>
	#include <openssl/err.h>
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
		unsigned char	_hash[16];
		hasherror_t	_err;
};

md5::md5() : hash() {
	pvt=new md5private;
	clear();
}

md5::~md5() {
	delete pvt;
}

bool md5::append(const unsigned char *data, uint32_t size) {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_MD5_INIT_RETURNING_INT)
			if (!MD5_Update(&pvt->_context,data,size)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			MD5_Update(&pvt->_context,data,size);
		#endif
		return true;
	#else
		pvt->_md5.MD5Update(&pvt->_context,data,size);
		return true;
	#endif
}

const unsigned char *md5::getHash() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_MD5_INIT_RETURNING_INT)
			if (!MD5_Final(pvt->_hash,&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return (const unsigned char *)"";
			}
		#else
			MD5_Final(pvt->_hash,&pvt->_context);
		#endif
	#else
		pvt->_md5.MD5Final(pvt->_hash,&pvt->_context);
	#endif
	return pvt->_hash;
}

uint64_t md5::getHashSize() {
	return sizeof(pvt->_hash);
}

bool md5::clear() {
	pvt->_err=HASH_ERROR_SUCCESS;
	bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_MD5_INIT_RETURNING_INT)
			if (!MD5_Init(&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			MD5_Init(&pvt->_context);
		#endif
	#else
		pvt->_md5.MD5Init(&pvt->_context);
	#endif
	return true;
}

hasherror_t md5::getError() {
	return pvt->_err;
}

void md5::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
		pvt->_err=HASH_ERROR_NULL;
		// clear the queue
		while (ERR_get_error()) {}
	#else
		// FIXME: implement this...
	#endif
}
