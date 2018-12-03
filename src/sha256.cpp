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
			uint8_t		_result[SHA256_DIGEST_LENGTH+1];
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
		if (SHA256_Final(pvt->_result,&pvt->_context)) {
			pvt->_result[SHA256_DIGEST_LENGTH]='\0';
		} else {
			pvt->_result[0]='\0';
		}
		return pvt->_result;
	#else
		pvt->_err=HASH_ERROR_UNSUPPORTED;
		return NULL;
	#endif
}

uint32_t sha256::getHashLength() {
	#if defined(RUDIMENTS_HAS_SSL)
		return SHA256_DIGEST_LENGTH;
	#else
		return 32;
	#endif
}

bool sha256::clear() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(pvt->_result,sizeof(pvt->_result));
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
