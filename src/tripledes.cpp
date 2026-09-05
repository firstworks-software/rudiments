// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/tripledes.h>
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
	#include <openssl/err.h>
#else
	#include "desfips463.cpp"
	#define EVP_MAX_BLOCK_LENGTH 0
#endif

// OpenSSL doesn't provide a block-size macro for DES
#define DES_BLOCK_SIZE 8

class tripledesprivate {
	friend class tripledes;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			EVP_CIPHER_CTX		*_context;
		#else
			struct des3_key_s	_context;
			uint8_t			_cbc[DES_BLOCK_SIZE];
		#endif
		byte_t	_out[DES_BLOCK_SIZE+EVP_MAX_BLOCK_LENGTH];
		int	_outlen;
};

tripledes::tripledes() : encryption() {
	pvt=new tripledesprivate;
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_context=NULL;
	#else
		freeContext();
	#endif
}

tripledes::~tripledes() {
	freeContext();
	delete pvt;
}

size_t tripledes::getKeySize() {
	return 24;
}

size_t tripledes::getIvSize() {
	return DES_BLOCK_SIZE;
}

const byte_t *tripledes::getEncryptedData() {
	return getData(true);
}

const byte_t *tripledes::getDecryptedData() {
	return getData(false);
}

const byte_t *tripledes::getData(bool encrypt) {

	// reset the error
	encryption::setError(ENCRYPTION_ERROR_SUCCESS);

	// without padding, the data must already be a
	// multiple of DES_BLOCK_SIZE in length
	if (!getUsePadding() && (getIn()->getSize()%DES_BLOCK_SIZE)) {
		encryption::setError(ENCRYPTION_ERROR_INVALID_PADDING);
		return NULL;
	}

	#if !defined(RUDIMENTS_HAS_SSL)
	// Our implementation decrypts whole blocks, whether padding is on or
	// off, and the padding-truncation below assumes a block-aligned output
	// buffer, so on decryption the input must be a multiple of
	// DES_BLOCK_SIZE in length either way.
	if (!encrypt && (getIn()->getSize()%DES_BLOCK_SIZE)) {
		encryption::setError(ENCRYPTION_ERROR_INVALID_PADDING);
		return NULL;
	}
	#endif

	// set the dirty flag true if we're doing a different operation
	// (encryption vs. decryption) than we're currently configured to do
	if (getEncrypted()!=encrypt) {
		setDirty(true);
	}

	// set the current operation
	setEncrypted(encrypt);

	// if the dirty flag isn't set then we can just return the
	// existing output buffer
	if (!getDirty()) {
		return getOut()->getBuffer();
	}

	// re-init if the dirty flag is set

	freeContext();

	// decide on a block cipher mode
	#if defined(RUDIMENTS_HAS_SSL)
		const EVP_CIPHER	*mode=NULL;
		switch (getBlockCipherMode()) {
			#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_CBC
			case BLOCK_CIPHER_MODE_CBC:
				mode=EVP_des_ede3_cbc();
				break;
			#endif
			#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_CFB64
			// DES has a single, 64-bit-wide CFB mode
			case BLOCK_CIPHER_MODE_CFB128:
				mode=EVP_des_ede3_cfb64();
				break;
			#endif
			#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_ECB
			case BLOCK_CIPHER_MODE_ECB:
				mode=EVP_des_ede3_ecb();
				break;
			#endif
			#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_OFB
			case BLOCK_CIPHER_MODE_OFB:
				mode=EVP_des_ede3_ofb();
				break;
			#endif
			default:
				encryption::setError(
					ENCRYPTION_ERROR_UNSUPPORTED);
				return NULL;
		}
	#else
		// OpenSSL supports a bunch of block cipher modes,
		// but our non-OpenSSL implementation only supports CBC.
		if (getBlockCipherMode()!=BLOCK_CIPHER_MODE_CBC) {
			encryption::setError(ENCRYPTION_ERROR_UNSUPPORTED);
			return NULL;
		}
	#endif

	newContext();
	#if defined(RUDIMENTS_HAS_SSL)
		if (!EVP_CipherInit_ex(pvt->_context,
					mode,
					NULL,
					getKey(),
					getIv(),
					(encrypt)?1:0)) {
			freeContext();
			setError(ERR_GET_REASON(ERR_get_error()));
			return NULL;
		}

		// enable/disable padding
		EVP_CIPHER_CTX_set_padding(pvt->_context,(getUsePadding())?1:0);
	#endif

	#if !defined(RUDIMENTS_HAS_SSL)
	// reset the cbc buffer
	bytestring::copy(pvt->_cbc,getIv(),getIvSize());
	#endif

	// clear the output buffer
	getOut()->clear();


	// encrypt/decrypt the data...
	// The OpenSSL implementation defaults to CBC and CMS padding, so we'll
	// use that in the non-OpenSSL implementation as well.  OpenSSL allows
	// us to manually pad (and not have to "finalize") but who knows,
	// OpenSSL may be optimized in some way (including maybe even using
	// crypto hardware, if available), so we'll let OpenSSL do as much as
	// possible.


	// encrypt/decrypt the data in DES_BLOCK_SIZE-sized blocks
	const byte_t	*in=getIn()->getBuffer();
	uint32_t	inremaining=getIn()->getSize();
	#if !defined(RUDIMENTS_HAS_SSL)
	byte_t		padbytes;
	#endif
	for (;;) {

		#if defined(RUDIMENTS_HAS_SSL)
		// Bail if there's nothing left to read.  Do this here for
		// OpenSSL.  Our non-ssl implementation needs to run through
		// at least one iteration, even for 0-byte cases.  Modern
		// OpenSSL implementations can handle 0-byte cases, but old
		// enough versions throw an assertion if EVP_CipherUpdate
		// is called with readsize==0.
		if (!inremaining) {
			break;
		}
		#else
		// without padding there's no pad block, so 0 bytes
		// of input produce 0 bytes of output
		if (!getUsePadding() && !inremaining) {
			break;
		}
		#endif

		// figure out how much to read from the input
		uint32_t	readsize=(inremaining>=DES_BLOCK_SIZE)?
						DES_BLOCK_SIZE:inremaining;

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
				// initialization vector.
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
				for (byte_t i=0; i<readsize; i++) {
					pvt->_cbc[i]^=in[i];
				}

				// CMS (Cryptographic Message Syntax) padding
				// (PKCS#5/#7)
				//
				// The last block of data may not be
				// DES_BLOCK_SIZE bytes in length.  In that
				// case, we need to fill the remaining bytes
				// with some kind of padding.
				//
				// CMS-padding calls for each remaining byte
				// to be filled with the number of padded bytes.
				// Eg. if we need to fill 4 bytes with padding,
				// then we fill each byte with a 4.
				//
				// Figure out how many bytes of padding we need
				// and XOR the remaining bytes of the CBC buffer
				// against that number.
				if (getUsePadding()) {
					padbytes=DES_BLOCK_SIZE-readsize;
					for (byte_t i=readsize;
							i<DES_BLOCK_SIZE; i++) {
						pvt->_cbc[i]^=padbytes;
					}
				}

				// encrypt the CBC'ed data into the out buffer
				des3_encrypt(pvt->_cbc,&pvt->_context,
								pvt->_out);

				// re-seed the CBC buffer from the out buffer
				bytestring::copy(pvt->_cbc,
						pvt->_out,DES_BLOCK_SIZE);

			} else {
				// decrypt the in buffer
				des3_decrypt(in,&pvt->_context,pvt->_out);

				// un-CBC the output (see CBC description above)
				for (byte_t i=0; i<DES_BLOCK_SIZE; i++) {
					pvt->_out[i]^=pvt->_cbc[i];
				}

				// re-seed the CBC buffer from the in buffer
				bytestring::copy(pvt->_cbc,in,DES_BLOCK_SIZE);
			}

			// outlen will always be DES_BLOCK_SIZE for the
			// non-OpenSSL implementation
			pvt->_outlen=DES_BLOCK_SIZE;
		#endif

		// append the encrypted/decrypted data
		getOut()->append(pvt->_out,pvt->_outlen);

		// advance the input pointer, decrement the input-remaining
		in+=readsize;
		inremaining-=readsize;

		#if !defined(RUDIMENTS_HAS_SSL)
		// Bail if there's nothing left to read.  Do this here for
		// our non-ssl implementation, which needs to run through
		// at least one iteration, even for 0-byte cases.  For the
		// OpenSSL implementation, it's handled at the top of the
		// loop.
		if (!inremaining) {
			break;
		}
		#endif
	}

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
			// DES_BLOCK_SIZE, then we have to include a full block
			// of padding at the end.  That way, when decrypting,
			// we can just grab the last byte and be guaranteed that
			// it's the number of padded bytes.
			//
			// The way the loop above works, if the input is
			// 0-bytes, then this will naturally happen, but it
			// won't happen for other multiples of DES_BLOCK_SIZE,
			// so we need to do it here.
			//
			// Append a full block of 8's (DES_BLOCK_SIZE).
			uint64_t	insize=getIn()->getSize();
			if (getUsePadding() && insize &&
					!(insize%DES_BLOCK_SIZE)) {
				for (byte_t i=0; i<DES_BLOCK_SIZE; i++) {
					pvt->_cbc[i]^=DES_BLOCK_SIZE;
				}
				des3_encrypt(pvt->_cbc,&pvt->_context,
								pvt->_out);
				getOut()->append(pvt->_out,DES_BLOCK_SIZE);
			}

		} else if (getUsePadding()) {

			// Truncate padding...
			//
			// Grab the last byte, which is guaranteed to be a
			// pad-byte (see above).  Then move back that many
			// bytes from the end and truncate the buffer there.
			getOut()->setPositionRelativeToEnd(-1);
			getOut()->read(&padbytes,sizeof(padbytes));
			if (padbytes>DES_BLOCK_SIZE) {
				encryption::setError(
					ENCRYPTION_ERROR_INVALID_PADDING);
				getOut()->clear();
				return NULL;
			}
			getOut()->setPositionRelativeToEnd(-padbytes);
			getOut()->truncate();
		}
	#endif

	// reset the dirty flag
	setDirty(false);

	return getOut()->getBuffer();
}

void tripledes::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		encryption::setError(ENCRYPTION_ERROR_NULL);
		// FIXME: implement this...
		// (currently just clears the queue)
		while (ERR_get_error()) {}
	#else
		encryption::setError(ENCRYPTION_ERROR_NULL);
	#endif
}

void tripledes::newContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		pvt->_context=EVP_CIPHER_CTX_new();
	#else
		des3_key_setup(getKey(),&pvt->_context);
	#endif
}

void tripledes::freeContext() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_context) {
			EVP_CIPHER_CTX_free(pvt->_context);
		}
		pvt->_context=NULL;
	#else
		bytestring::zero(&pvt->_context,sizeof(pvt->_context));
	#endif
}

bool tripledes::isSupported() {
	#if defined(RUDIMENTS_HAS_SSL)
		// only the modes that getData() can map to an EVP cipher
		bool	supported=false;
		#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_CBC
		supported=supported ||
			(getBlockCipherMode()==BLOCK_CIPHER_MODE_CBC);
		#endif
		#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_CFB64
		// DES has a single, 64-bit-wide CFB mode
		supported=supported ||
			(getBlockCipherMode()==BLOCK_CIPHER_MODE_CFB128);
		#endif
		#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_ECB
		supported=supported ||
			(getBlockCipherMode()==BLOCK_CIPHER_MODE_ECB);
		#endif
		#ifdef RUDIMENTS_HAS_EVP_DES_EDE3_OFB
		supported=supported ||
			(getBlockCipherMode()==BLOCK_CIPHER_MODE_OFB);
		#endif
		return supported;
	#else
		// our own implementation only supports CBC
		return getBlockCipherMode()==BLOCK_CIPHER_MODE_CBC;
	#endif
}
