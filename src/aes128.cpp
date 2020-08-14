// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/aes128.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/stdio.h>

// If we don't have ECP_CIPHER_CTX_new then there's a whole different API that
// we have to use: EVP_CIPHER_CTX_init/cleanup, EVP_Encrypt/Decryipt_init, and
// EVP_Encrypt/DecryptUpdate/Final.  Maybe later...
#if !defined(RUDIMENTS_HAS_EVP_CIPHER_CTX_NEW)
	#undef RUDIMENTS_HAS_SSL
#endif

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/evp.h>
	#include <openssl/aes.h>
	#include <openssl/err.h>
#else
	#include "aes128rfc3602.cpp"
	#define AES_BLOCK_SIZE 16
	#define EVP_MAX_BLOCK_LENGTH 0
#endif

class aes128private {
	friend class aes128;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			EVP_CIPHER_CTX	*_context;
		#else
			void		*_context;
			uint8_t		_cbc[AES_BLOCK_SIZE];
		#endif
		unsigned char	_out[AES_BLOCK_SIZE+EVP_MAX_BLOCK_LENGTH];
		int		_outlen;
};

aes128::aes128() : encryption() {
	pvt=new aes128private;
	pvt->_context=NULL;
}

aes128::~aes128() {
	freeContext();
}

uint32_t aes128::getKeySize() {
	return 16;
}

uint32_t aes128::getIvSize() {
	return AES_BLOCK_SIZE;
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
	if (getEncrypted()!=encrypt) {
		setDirty(true);
	}

	// set the current operation
	setEncrypted(encrypt);

	// FIXME: arguably, we need 2 dirty's - one if the key/iv has changed,
	// and another if data has been appended.  If data has been appended,
	// we don't need to reinit the context, we just need to
	// reencrypt/decrypt the data.

	if (!getDirty()) {

		// if the dirty flag isn't set then we can just return the
		// existing output buffer
		return getOut()->getBuffer();

	} else {

		// re-init if the dirty flag is set

		// reset the error
		setError(ENCRYPTION_ERROR_SUCCESS);

		freeContext();
		newContext();
		#if defined(RUDIMENTS_HAS_SSL)
			if (!EVP_CipherInit_ex(pvt->_context,
						EVP_aes_128_cbc(),
						NULL,
						getKey(),
						getIv(),
						(encrypt)?1:0)) {
				freeContext();
				setError(ERR_GET_REASON(ERR_get_error()));
				return NULL;
			}
		#else
			if (!pvt->_context) {
				// FIXME: set error
				return NULL;
			}
		#endif

		// reset the dirty flag
		setDirty(false);
	}

	#if !defined(RUDIMENTS_HAS_SSL)
	// reset the cbc buffer
	bytestring::copy(pvt->_cbc,getIv(),getIvSize());
	#endif

	// clear the output buffer
	getOut()->clear();


	// encrypt/decrypt the data...
	// The OpenSSL implementation uses CBC and CMS padding, so we'll use
	// that in the non-OpenSSL implementation as well.  OpenSSL allows us
	// to manually pad (and not have to "finalize") but who knows, OpenSSL
	// may be optimized in some way (including maybe even using crypto
	// hardware, if available), so we'll let OpenSSL do as much as possible.


	// encrypt/decrypt the data in AES_BLOCK_SIZE-sized blocks
	const unsigned char	*in=getIn()->getBuffer();
	uint32_t		inremaining=getIn()->getSize();
	#if !defined(RUDIMENTS_HAS_SSL)
	unsigned char		padbytes;
	#endif
	do {

		// figure out how much to read from the input
		uint32_t	readsize=(inremaining>=AES_BLOCK_SIZE)?
						AES_BLOCK_SIZE:inremaining;

		// encrypt/decrypt a block of data
		#if defined(RUDIMENTS_HAS_SSL)
			if (!EVP_CipherUpdate(pvt->_context,
							pvt->_out,
							&pvt->_outlen,
							in,
							readsize)) {
				setError(ERR_GET_REASON(ERR_get_error()));
				getOut()->clear();
				return NULL;
			}
		#else
			if (getEncrypted()) {

				// OpenSSL implements CBC and CMS-padding
				// internally, but we have to implement it
				// ourselves here.

				// CBC (Cipher block chaining)
				//
				// The CBC buffer (pvt->_cbc) is seeded with the
				// initializtion vector.
				//
				// XOR whatever's in the CBC buffer with the
				// input to create CBC'ed data.  We'll encrypt
				// this, rather than encrypting the input data
				// directly.
				//
				// Later, we'll use the encrypted data as the
				// next seed.
				//
				// This is analogous to seeding a random number
				// generator, then using the generated number
				// as the next seed.
				for (unsigned char i=0; i<readsize; i++) {
					pvt->_cbc[i]^=in[i];
				}

				// CMS (Cryptographic Message Syntax) padding
				// (PKCS#5/#7)
				//
				// The last block of data may not be
				// AES_BLOCK_SIZE bytes in length.  In that
				// case, we need to fill the remaining bytes
				// with some kind of padding.
				//
				// CMS-padding calls for each remaining byte
				// to be filled with the number of padded bytes.
				// Eg. if we need to fill 8 bytes with padding,
				// then we fill each byte with an 8.
				//
				// Figure out how many bytes of padding we need
				// and XOR the remaining bytes of the CBC buffer
				// against that number.
				padbytes=AES_BLOCK_SIZE-readsize;
				for (unsigned char i=readsize;
							i<AES_BLOCK_SIZE; i++) {
					pvt->_cbc[i]^=padbytes;
				}

				// encrypt the CBC'ed data into the out buffer
				aes_encrypt(pvt->_context,pvt->_cbc,pvt->_out);

				// re-seed the CBC buffer from the out buffer
				bytestring::copy(pvt->_cbc,
						pvt->_out,AES_BLOCK_SIZE);

			} else {
				// decrypt the in buffer
				aes_decrypt(pvt->_context,in,pvt->_out);

				// un-CBC the output (see CBC description above)
				for (unsigned char i=0; i<AES_BLOCK_SIZE; i++) {
					pvt->_out[i]^=pvt->_cbc[i];
				}

				// re-seed the CBC buffer from the in buffer
				bytestring::copy(pvt->_cbc,in,AES_BLOCK_SIZE);
			}

			// outlen will always be AES_BLOCK_SIZE for the
			// non-OpenSSL implementation
			pvt->_outlen=AES_BLOCK_SIZE;
		#endif

		// append the encrypted/decrypted data
		getOut()->append(pvt->_out,pvt->_outlen);

		// advance the input pointer, decrement the input-remaining
		in+=readsize;
		inremaining-=readsize;

	} while (inremaining);

	#if defined(RUDIMENTS_HAS_SSL)
		// finalize
		if (!EVP_CipherFinal_ex(pvt->_context,
						pvt->_out,
						&pvt->_outlen)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			getOut()->clear();
			return NULL;
		}

		// append the finalized data
		getOut()->append(pvt->_out,pvt->_outlen);
	#else
		if (encrypt) {

			// When CMS-padding, if the input is a multiple of
			// AES_BLOCK_SIZE, then we have to include a full block
			// of padding at the end.  That way, when decrypting,
			// we can just grab the last byte and be guaranteed that
			// it's the number of padded bytes.
			//
			// The way the loop above works, if the input is
			// 0-bytes, then this will naturally happen, but it
			// won't happen for other multiples of AES_BLOCK_SIZE,
			// so we need to do it here.
			//
			// Append a full block of 16's (AES_BLOCK_SIZE).
			uint64_t	insize=getIn()->getSize();
			if (insize && !(insize%AES_BLOCK_SIZE)) {
				for (unsigned char i=0; i<AES_BLOCK_SIZE; i++) {
					pvt->_cbc[i]^=AES_BLOCK_SIZE;
				}
				aes_encrypt(pvt->_context,pvt->_cbc,pvt->_out);
				getOut()->append(pvt->_out,AES_BLOCK_SIZE);
			}

		} else {

			// Truncate padding...
			//
			// Grab the last byte, which is guaranteed to be a
			// pad-byte (see above).  Then move back that many
			// bytes from the end and truncate the buffer there.
			getOut()->setPosition(getOut()->getSize()-1);
			getOut()->read(&padbytes,sizeof(padbytes));
			if (padbytes>AES_BLOCK_SIZE) {
				setError(ENCRYPTION_ERROR_INVALID_PADDING);
				getOut()->clear();
				return NULL;
			}
			getOut()->setPosition(getOut()->getSize()-padbytes);
			getOut()->truncate();
		}
	#endif

	return getOut()->getBuffer();
}

void aes128::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		encryption::setError(ENCRYPTION_ERROR_NULL);
		// FIXME: implement this...
		// (currently just clears the queue)
		while (ERR_get_error()) {}
	#else
		encryption::setError((encryptionerror_t)err);
	#endif
}

void aes128::newContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_context=EVP_CIPHER_CTX_new();
	#else
		if (getEncrypted()) {
			pvt->_context=aes_encrypt_init(getKey(),getKeySize());
		} else {
			pvt->_context=aes_decrypt_init(getKey(),getKeySize());
		}
	#endif
}

void aes128::freeContext() {
	if (pvt->_context) {
		#if defined(RUDIMENTS_HAS_SSL)
			EVP_CIPHER_CTX_free(pvt->_context);
		#else
			if (getEncrypted()) {
				aes_encrypt_deinit(pvt->_context);
			} else {
				aes_decrypt_deinit(pvt->_context);
			}
		#endif
	}
	pvt->_context=NULL;
}
