// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/sha1.h>
#include <rudiments/bytestring.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/sha.h>
	#include <openssl/err.h>
#else
	#include "sha1rfc3174.cpp"
#endif

class sha1private {
	friend class sha1;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			SHA_CTX		_context;
			uint8_t		_hash[SHA_DIGEST_LENGTH];
		#else
			SHA1Context	_context;
			uint8_t		_hash[SHA1HashSize];
		#endif
};

sha1::sha1() : hash() {
	pvt=new sha1private;
	clear();
}

sha1::~sha1() {
	delete pvt;
}

bool sha1::append(const byte_t *data, uint32_t size) {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_SHA1_INIT_RETURNING_INT)
			if (!SHA1_Update(&pvt->_context,data,size)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA1_Update(&pvt->_context,data,size);
		#endif
		return true;
	#else
		int	result=SHA1Input(&pvt->_context,data,size);
		setError(result);
		return (result==shaSuccess);
	#endif
}

const byte_t *sha1::getHash() {
	hash::setError(HASH_ERROR_SUCCESS);
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_SHA1_INIT_RETURNING_INT)
			if (!SHA1_Final(pvt->_hash,&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return NULL;
			}
		#else
			SHA1_Final(pvt->_hash,&pvt->_context);
		#endif
		return pvt->_hash;
	#else
		int	result=SHA1Result(&pvt->_context,pvt->_hash);
		setError(result);
		if (result==shaSuccess) {
			return pvt->_hash;
		}
		return NULL;
	#endif
}

uint64_t sha1::getHashSize() {
	#if defined(RUDIMENTS_HAS_SSL)
		return SHA_DIGEST_LENGTH;
	#else
		return SHA1HashSize;
	#endif
}

bool sha1::clear() {
	hash::setError(HASH_ERROR_SUCCESS);
	bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
	#if defined(RUDIMENTS_HAS_SSL)
		#if defined(RUDIMENTS_HAS_SHA1_INIT_RETURNING_INT)
			if (!SHA1_Init(&pvt->_context)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}
		#else
			SHA1_Init(&pvt->_context);
		#endif
		return true;
	#else
		int	result=SHA1Reset(&pvt->_context);
		setError(result);
		return (result==shaSuccess);
	#endif
}

void sha1::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
		hash::setError(HASH_ERROR_NULL);
		// clear the queue
		while (ERR_get_error()) {}
	#else
		switch (err) {
			case shaNull:
				hash::setError(HASH_ERROR_NULL);
			case shaInputTooLong:
				hash::setError(HASH_ERROR_INPUT_TOO_LONG);
			case shaStateError:
				hash::setError(HASH_ERROR_STATE_ERROR);
			default:
				hash::setError(HASH_ERROR_SUCCESS);
		}
	#endif
}
