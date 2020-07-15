// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/aes128.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/evp.h>
	#include <openssl/aes.h>
	#include <openssl/err.h>
#else
	#include "aes128rfcsomething.cpp"
#endif

class aes128private {
	friend class aes128;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			EVP_CIPHER_CTX	*_context;
		#else
			#error implement this...
		#endif
};

aes128::aes128() : encryption() {
	pvt=new aes128private;
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_context=NULL;
	#else
		#error implement this...
	#endif
}

aes128::~aes128() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_context) {
			EVP_CIPHER_CTX_free(pvt->_context);
		}
	#else
		#error implement this...
	#endif
}

uint32_t aes128::getKeySize() {
	return 16;
}

uint32_t aes128::getIvSize() {
	#if defined(RUDIMENTS_HAS_SSL)
		return AES_BLOCK_SIZE;
	#else
		#error implement this...
	#endif
}

const unsigned char *aes128::getEncryptedData() {
	return getData(true);
}

const unsigned char *aes128::getDecryptedData() {
	return getData(false);
}

const unsigned char *aes128::getData(bool encrypt) {

	// set the dirty flag true if we're doing a different operation
	// (encryption vs. decryption) than we're currently configured to do
	setDirty(getDirty() || (getEncrypted() && !encrypt));
stdoutput.printf("encrypt=%d\n",encrypt);
stdoutput.printf("dirty=%d\n",getDirty());

	// reset the error
	setError(ENCRYPTION_ERROR_SUCCESS);

	// re-init if the dirty flag is set
	if (getDirty()) {

		#if defined(RUDIMENTS_HAS_SSL)
			pvt->_context=EVP_CIPHER_CTX_new();

			if (!EVP_CipherInit_ex(pvt->_context,
						EVP_aes_128_cbc(),
						NULL,
						getKey(),
						getIv(),
						(encrypt)?1:0)) {
				EVP_CIPHER_CTX_free(pvt->_context);
				setError(ERR_GET_REASON(ERR_get_error()));
				return NULL;
			}
		#else
			#error implement this...
		#endif

		setDirty(false);
		setEncrypted(encrypt);
	}

	// allocate the output buffer as necessary
	reallocateOut(getIn()->getSize()+
			#if defined(RUDIMENTS_HAS_SSL)
				EVP_MAX_BLOCK_LENGTH
			#else
				#error implement this...
			#endif
			);
	

	// encrypt and finalize the data
	#if defined(RUDIMENTS_HAS_SSL)
		if (!EVP_CipherUpdate(pvt->_context,
						getOut(),
						(int *)getOutLengthPointer(),
						getIn()->getBuffer(),
						getIn()->getSize()) ||
			!EVP_CipherFinal_ex(pvt->_context,
						getOut(),
						(int *)getOutLengthPointer())) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return NULL;
		}
		return getOut();
	#else
		#error implement this...
	#endif
}

void aes128::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		encryption::setError(ENCRYPTION_ERROR_NULL);
		// FIXME: implement this...
		// (currently just clears the queue)
		while (ERR_get_error()) {}
	#else
		#error implement this...
	#endif
}
