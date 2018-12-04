// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/sha256.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/sha.h>
#endif

class sha256private {
	friend class sha256;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			SHA256_CTX	_context;
			uint8_t		_hash[SHA256_DIGEST_LENGTH];
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
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		pvt->_err=HASH_ERROR_UNSUPPORTED;
		return false;
	#endif
}

const unsigned char *sha256::getHash() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (!SHA256_Final(pvt->_hash,&pvt->_context)) {
			// FIXME: set error...
			return NULL;
		}
		return pvt->_hash;
	#else
		pvt->_err=HASH_ERROR_UNSUPPORTED;
		return NULL;
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
	bytestring::zero(pvt->_hash,sizeof(pvt->_hash));
	#if defined(RUDIMENTS_HAS_SSL)
		if (!SHA256_Init(&pvt->_context)) {
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		pvt->_err=HASH_ERROR_UNSUPPORTED;
		return false;
	#endif
}

hasherror_t sha256::getError() {
	return pvt->_err;
}

void sha256::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
	#else
		switch (err) {
			case shaNull:
				pvt->_err=HASH_ERROR_NULL;
			case shaInputTooLong:
				pvt->_err=HASH_ERROR_INPUT_TOO_LONG;
			case shaStateError:
				pvt->_err=HASH_ERROR_STATE_ERROR;
			default:
				pvt->_err=HASH_ERROR_SUCCESS;
		}
	#endif
}
