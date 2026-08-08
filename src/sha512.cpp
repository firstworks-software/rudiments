// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/sha512.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
	#include <openssl/sha.h>
	#include <openssl/err.h>
#else
	#include "sha512rfc6234.cpp"
#endif

class sha512private {
	friend class sha512;
	private:
		#if defined(RUDIMENTS_HAS_SSL) && \
			defined(RUDIMENTS_HAS_SHA512_CTX)
			SHA512_CTX	_context;
			uint8_t		_hash[SHA512_DIGEST_LENGTH];
		#else
			sha512_ctx	_context;
			sha512_t	_hash;
		#endif
};

sha512::sha512() : hash() {
	pvt=new sha512private;
	clear();
}

sha512::~sha512() {
	delete pvt;
}

bool sha512::append(const byte_t *data, uint32_t size) {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
		#if defined(RUDIMENTS_HAS_SHA512_INIT_RETURNING_INT)
			if (!SHA512_Update(&pvt->_context,data,size)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA512_Update(&pvt->_context,data,size);
		#endif
		return true;
	#else
		sha512_update(&pvt->_context,data,size);
		return true;
	#endif
}

const byte_t *sha512::getHash() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
		#if defined(RUDIMENTS_HAS_SHA512_INIT_RETURNING_INT)
			if (!SHA512_Final(pvt->_hash,&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return NULL;
			}
		#else
			SHA512_Final(pvt->_hash,&pvt->_context);
		#endif
		return pvt->_hash;
	#else
		sha512_done(&pvt->_context,&pvt->_hash);
		return pvt->_hash.u.u8;
	#endif
}

uint64_t sha512::getHashSize() {
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
		return SHA512_DIGEST_LENGTH;
	#else
		return 64;
	#endif
}

bool sha512::clear() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
		bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
		#if defined(RUDIMENTS_HAS_SHA512_INIT_RETURNING_INT)
			if (!SHA512_Init(&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA512_Init(&pvt->_context);
		#endif
		return true;
	#else
		bytestring::zero(pvt->_hash.u.u8,sizeof(pvt->_hash.u.u8));
		sha512_init(&pvt->_context);
		return true;
	#endif
}

void sha512::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL) && defined(RUDIMENTS_HAS_SHA512_CTX)
		// FIXME: implement this...
		hash::setError(HASH_ERROR_NULL);
		// clear the queue
		while (ERR_get_error()) {}
	#else
		hash::setError(HASH_ERROR_NULL);
	#endif
}

bool sha512::isSupported() {
	return true;
}
