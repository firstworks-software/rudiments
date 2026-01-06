// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/pbkdf2.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/evp.h>
	#include <openssl/err.h>
#endif

class pbkdf2private {
	friend class pbkdf2;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			pbkdf2algorithm_t	_alg;
			uint32_t		_iterations;
			uint32_t		_keysize;
		#endif
};

pbkdf2::pbkdf2() : hash() {
	pvt=new pbkdf2private;
	pvt->_alg=PBKDF2_ALGORITHM_SHA256;
	pvt->_iterations=10000;
	pvt->_keysize=0;
}

pbkdf2::~pbkdf2() {
	delete pvt;
}

void pbkdf2::setIterations(uint32_t iterations) {
	pvt->_iterations=iterations;
}

uint32_t pbkdf2::getIterations() {
	return pvt->_iterations;
}

void pbkdf2::setAlgorithm(pbkdf2algorithm_t alg) {
	pvt->_alg=alg;
}

pbkdf2algorithm_t pbkdf2::getAlgorithm() {
	return pvt->_alg;
}

void pbkdf2::setKeySize(uint32_t keysize) {
	pvt->_keysize=keysize;
}

uint32_t pbkdf2::getKeySize() {
	return pvt->_keysize;
}

const byte_t *pbkdf2::getHash() {
	#if defined(RUDIMENTS_HAS_SSL)

		// reset the error
		hash::setError(HASH_ERROR_SUCCESS);
		error::clearError();

		// if the dirty flag isn't set then we can just
		// return the existing output buffer
		if (!getDirty()) {
			return getOut()->getBuffer();
		}

		// clear the output buffer
		getOut()->clear();

		// decide on hash algorithm
		const EVP_MD	*alg=NULL;
		switch (pvt->_alg) {
			case PBKDF2_ALGORITHM_SHA1:
				alg=EVP_sha1();
				break;
			case PBKDF2_ALGORITHM_SHA256:
				alg=EVP_sha256();
				break;
			case PBKDF2_ALGORITHM_SHA512:
				alg=EVP_sha512();
				break;
			default:
				hash::setError(HASH_ERROR_UNSUPPORTED);
				RUDIMENTS_SET_ENOSYS;
				return NULL;
		}

		// create a buffer to store the key
		unsigned char	*key=new unsigned char[getKeySize()];

		// hash
		if (!PKCS5_PBKDF2_HMAC((const char *)getIn()->getBuffer(),
				getIn()->getSize(),
				getSalt(),
				getSaltSize(),
				getIterations(),
				alg,
				getKeySize(),
				key)) {
			// FIXME: setError(...)
			delete[] key;
			return NULL;
		}

		// copy out the key
		getOut()->append(key,getKeySize());

		// clean up
		delete[] key;

		// reset the dirty flag
		setDirty(false);

		return getOut()->getBuffer();

	#else
		hash::setError(HASH_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS;
		return NULL;
	#endif
}

bool pbkdf2::reset() {
	if (!clear()) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_alg=PBKDF2_ALGORITHM_SHA256;
		pvt->_iterations=10000;
		pvt->_keysize=0;
		return true;
	#else
		hash::setError(HASH_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS;
		return false;
	#endif
}

void pbkdf2::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this
		hash::setError(HASH_ERROR_NULL);
		// clear the queue
		while(ERR_get_error()) {}
	#else
		hash::setError(HASH_ERROR_NULL);
	#endif
}

bool pbkdf2::isSupported() {
	#if defined(RUDIMENTS_HAS_SSL)
		return true;
	#else
		return false;
	#endif
}
