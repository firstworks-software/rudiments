// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/sha256.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/sha.h>
	#include <openssl/err.h>
#else
	#include "sha256rfc4642.cpp"
#endif

class sha256private {
	friend class sha256;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			SHA256_CTX	_context;
			uint8_t		_hash[SHA256_DIGEST_LENGTH];
		#else
			sha256_ctx	_context;
			sha256_t	_hash;
		#endif
		hasherror_t	_err;
};

sha256::sha256() : hash() {
	pvt=new sha256private;
	clear();
}

sha256::~sha256() {
	delete pvt;
}

bool sha256::append(const unsigned char *data, uint32_t length) {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (!SHA256_Update(&pvt->_context,data,length)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return false;
		}
		return true;
	#else
		sha256_update(&pvt->_context,data,length);
		return false;
	#endif
}

const unsigned char *sha256::getHash() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (!SHA256_Final(pvt->_hash,&pvt->_context)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return NULL;
		}
		return pvt->_hash;
	#else
		sha256_done(&pvt->_context,&pvt->_hash);
		return pvt->_hash.u.u8;
	#endif
}

uint64_t sha256::getHashLength() {
	#if defined(RUDIMENTS_HAS_SSL)
		return SHA256_DIGEST_LENGTH;
	#else
		return 32;
	#endif
}

bool sha256::clear() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
		if (!SHA256_Init(&pvt->_context)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return false;
		}
		return true;
	#else
		bytestring::zero(pvt->_hash.u.u8,sizeof(pvt->_hash.u.u8));
		sha256_init(&pvt->_context);
		return false;
	#endif
}

hasherror_t sha256::getError() {
	return pvt->_err;
}

void sha256::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
		pvt->_err=HASH_ERROR_NULL;
		// clear the queue
		while (ERR_get_error()) {}
	#else
		pvt->_err=HASH_ERROR_NULL;
	#endif
}
