// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/sha256.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
	#include <openssl/sha.h>
	#include <openssl/err.h>
#else
	#include "sha256rfc4642.cpp"
#endif

class sha256private {
	friend class sha256;
	private:
		#if defined(RUDIMENTS_HAS_SSL) && \
			defined(RUDIMENTS_HAS_SHA256_CTX)
			SHA256_CTX	_context;
			uint8_t		_hash[SHA256_DIGEST_LENGTH];
		#else
			sha256_ctx	_context;
			sha256_t	_hash;
		#endif
};

sha256::sha256() : hash() {
	pvt=new sha256private;
	clear();
}

sha256::~sha256() {
	delete pvt;
}

bool sha256::append(const byte_t *data, uint32_t size) {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
		#if defined(RUDIMENTS_HAS_SHA256_INIT_RETURNING_INT)
			if (!SHA256_Update(&pvt->_context,data,size)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA256_Update(&pvt->_context,data,size);
		#endif
		return true;
	#else
		sha256_update(&pvt->_context,data,size);
		return true;
	#endif
}

const byte_t *sha256::getHash() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
		#if defined(RUDIMENTS_HAS_SHA256_INIT_RETURNING_INT)
			if (!SHA256_Final(pvt->_hash,&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return NULL;
			}
		#else
			SHA256_Final(pvt->_hash,&pvt->_context);
		#endif
		return pvt->_hash;
	#else
		sha256_done(&pvt->_context,&pvt->_hash);
		return pvt->_hash.u.u8;
	#endif
}

uint64_t sha256::getHashSize() {
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
		return SHA256_DIGEST_LENGTH;
	#else
		return 32;
	#endif
}

bool sha256::clear() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
		bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
		#if defined(RUDIMENTS_HAS_SHA256_INIT_RETURNING_INT)
			if (!SHA256_Init(&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA256_Init(&pvt->_context);
		#endif
		return true;
	#else
		bytestring::zero(pvt->_hash.u.u8,sizeof(pvt->_hash.u.u8));
		sha256_init(&pvt->_context);
		return true;
	#endif
}

void sha256::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA256_CTX)
		// FIXME: implement this...
		hash::setError(HASH_ERROR_NULL);
		// clear the queue
		while (ERR_get_error()) {}
	#else
		hash::setError(HASH_ERROR_NULL);
	#endif
}

bool sha256::isSupported() {
	return true;
}
