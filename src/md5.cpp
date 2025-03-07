// Copyright (c) David Muse
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
		byte_t		_hash[16];
};

md5::md5() : hash() {
	pvt=new md5private;
	clear();
}

md5::~md5() {
	delete pvt;
}

bool md5::append(const byte_t *data, uint32_t size) {
	hash::setError(HASH_ERROR_SUCCESS);
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

const byte_t *md5::getHash() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_MD5_INIT_RETURNING_INT)
			if (!MD5_Final(pvt->_hash,&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return (const byte_t *)"";
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
	hash::setError(HASH_ERROR_SUCCESS);
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

void md5::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
		hash::setError(HASH_ERROR_NULL);
		// clear the queue
		while (ERR_get_error()) {}
	#else
		hash::setError(HASH_ERROR_NULL);
	#endif
}
