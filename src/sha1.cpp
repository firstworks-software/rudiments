// Copyright (c) 2016 David Muse
// See the COPYING file for more information

#include <rudiments/sha1.h>
#include <rudiments/bytestring.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/sha.h>
#else
	#include "sha1rfc3174.cpp"
#endif

class sha1private {
	friend class sha1;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			SHA_CTX		_context;
			uint8_t		_result[SHA_DIGEST_LENGTH+1];
		#else
			SHA1Context	_context;
			uint8_t		_result[SHA1HashSize+1];
		#endif
		hasherror_t	_err;
};

sha1::sha1() : hash() {
	pvt=new sha1private;
	clear();
}

sha1::~sha1() {
	delete pvt;
}

bool sha1::append(const unsigned char *data, uint32_t length) {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (!SHA1_Update(&pvt->_context,data,length)) {
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		int	result=SHA1Input(&pvt->_context,data,length);
		setError(result);
		return (result==shaSuccess);
	#endif
}

const unsigned char *sha1::getHash() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (SHA1_Final(pvt->_result,&pvt->_context)) {
			pvt->_result[SHA_DIGEST_LENGTH]='\0';
		} else {
			pvt->_result[0]='\0';
		}
		return pvt->_result;
	#else
		int	result=SHA1Result(&pvt->_context,pvt->_result);
		setError(result);
		if (result==shaSuccess) {
			pvt->_result[SHA1HashSize]='\0';
			return pvt->_result;
		}
		return NULL;
	#endif
}

uint32_t sha1::getHashLength() {
	#if defined(RUDIMENTS_HAS_SSL)
		return SHA_DIGEST_LENGTH;
	#else
		return SHA1HashSize;
	#endif
}

bool sha1::clear() {
	pvt->_err=HASH_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(pvt->_result,sizeof(pvt->_result));
		if (!SHA1_Init(&pvt->_context)) {
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		int	result=SHA1Reset(&pvt->_context);
		setError(result);
		bytestring::zero(pvt->_result,sizeof(pvt->_result));
		return (result==shaSuccess);
	#endif
}

hasherror_t sha1::getError() {
	return pvt->_err;
}

void sha1::setError(int32_t err) {
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
