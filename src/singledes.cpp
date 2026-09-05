// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/singledes.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/stdio.h>

// OpenSSL 3 moved single DES to the legacy provider.  Loading that provider
// affects every other OpenSSL user in the process, and it isn't available at
// all in FIPS-configured builds, so we always use our own implementation here.
#include "desfips463.cpp"

#define DES_BLOCK_SIZE 8

class singledesprivate {
	friend class singledes;
	private:
		uint32_t	_ek[32];
		uint32_t	_dk[32];
		uint8_t		_cbc[DES_BLOCK_SIZE];
		byte_t		_out[DES_BLOCK_SIZE];
		int		_outlen;
};

singledes::singledes() : encryption() {
	pvt=new singledesprivate;
	freeContext();
}

singledes::~singledes() {
	freeContext();
	delete pvt;
}

size_t singledes::getKeySize() {
	return 8;
}

size_t singledes::getIvSize() {
	return DES_BLOCK_SIZE;
}

const byte_t *singledes::getEncryptedData() {
	return getData(true);
}

const byte_t *singledes::getDecryptedData() {
	return getData(false);
}

const byte_t *singledes::getData(bool encrypt) {

	// reset the error
	encryption::setError(ENCRYPTION_ERROR_SUCCESS);

	// The input must be a multiple of DES_BLOCK_SIZE in length, except
	// when encrypting with padding on, which is the case where the pad
	// bytes are added.  Decryption always operates on whole blocks, and
	// the padding-truncation below assumes a block-aligned output buffer.
	if ((getIn()->getSize()%DES_BLOCK_SIZE) &&
				(!encrypt || !getUsePadding())) {
		encryption::setError(ENCRYPTION_ERROR_INVALID_PADDING);
		return NULL;
	}

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

	// our implementation only supports CBC
	if (getBlockCipherMode()!=BLOCK_CIPHER_MODE_CBC) {
		encryption::setError(ENCRYPTION_ERROR_UNSUPPORTED);
		return NULL;
	}

	newContext();

	// reset the cbc buffer
	bytestring::copy(pvt->_cbc,getIv(),getIvSize());

	// clear the output buffer
	getOut()->clear();

	// encrypt/decrypt the data in DES_BLOCK_SIZE-sized blocks
	const byte_t	*in=getIn()->getBuffer();
	uint32_t	inremaining=getIn()->getSize();
	byte_t		padbytes;
	for (;;) {

		// without padding there's no pad block, so 0 bytes
		// of input produce 0 bytes of output
		if (!getUsePadding() && !inremaining) {
			break;
		}

		// figure out how much to read from the input
		uint32_t	readsize=(inremaining>=DES_BLOCK_SIZE)?
						DES_BLOCK_SIZE:inremaining;

		if (getEncrypted()) {

			// CBC (Cipher block chaining)
			//
			// The CBC buffer (pvt->_cbc) is seeded with the
			// initializtion vector.
			//
			// XOR whatever's in the CBC buffer with the input to
			// create CBC'ed data.  We'll encrypt this, rather than
			// encrypting the input data directly.
			//
			// Later, we'll use the encrypted data as the next seed.
			//
			// This is analogous to seeding a random number
			// generator, then using the generated number as the
			// next seed.
			for (byte_t i=0; i<readsize; i++) {
				pvt->_cbc[i]^=in[i];
			}

			// CMS (Cryptographic Message Syntax) padding
			// (PKCS#5/#7)
			//
			// The last block of data may not be DES_BLOCK_SIZE
			// bytes in length.  In that case, we need to fill the
			// remaining bytes with some kind of padding.
			//
			// CMS-padding calls for each remaining byte to be
			// filled with the number of padded bytes.  Eg. if we
			// need to fill 4 bytes with padding, then we fill each
			// byte with a 4.
			//
			// Figure out how many bytes of padding we need and XOR
			// the remaining bytes of the CBC buffer against that
			// number.
			if (getUsePadding()) {
				padbytes=DES_BLOCK_SIZE-readsize;
				for (byte_t i=readsize;
						i<DES_BLOCK_SIZE; i++) {
					pvt->_cbc[i]^=padbytes;
				}
			}

			// encrypt the CBC'ed data into the out buffer
			des_block_encrypt(pvt->_cbc,pvt->_ek,pvt->_out);

			// re-seed the CBC buffer from the out buffer
			bytestring::copy(pvt->_cbc,pvt->_out,DES_BLOCK_SIZE);

		} else {

			// decrypt the in buffer
			des_block_decrypt(in,pvt->_dk,pvt->_out);

			// un-CBC the output (see CBC description above)
			for (byte_t i=0; i<DES_BLOCK_SIZE; i++) {
				pvt->_out[i]^=pvt->_cbc[i];
			}

			// re-seed the CBC buffer from the in buffer
			bytestring::copy(pvt->_cbc,in,DES_BLOCK_SIZE);
		}

		// outlen will always be DES_BLOCK_SIZE
		pvt->_outlen=DES_BLOCK_SIZE;

		// append the encrypted/decrypted data
		getOut()->append(pvt->_out,pvt->_outlen);

		// advance the input pointer, decrement the input-remaining
		in+=readsize;
		inremaining-=readsize;

		// Bail if there's nothing left to read.  Do this here, rather
		// than at the top of the loop, because we need to run through
		// at least one iteration, even for 0-byte cases.
		if (!inremaining) {
			break;
		}
	}

	if (encrypt) {

		// When CMS-padding, if the input is a multiple of
		// DES_BLOCK_SIZE, then we have to include a full block of
		// padding at the end.  That way, when decrypting, we can just
		// grab the last byte and be guaranteed that it's the number of
		// padded bytes.
		//
		// The way the loop above works, if the input is 0-bytes, then
		// this will naturally happen, but it won't happen for other
		// multiples of DES_BLOCK_SIZE, so we need to do it here.
		//
		// Append a full block of 8's (DES_BLOCK_SIZE).
		uint64_t	insize=getIn()->getSize();
		if (getUsePadding() && insize && !(insize%DES_BLOCK_SIZE)) {
			for (byte_t i=0; i<DES_BLOCK_SIZE; i++) {
				pvt->_cbc[i]^=DES_BLOCK_SIZE;
			}
			des_block_encrypt(pvt->_cbc,pvt->_ek,pvt->_out);
			getOut()->append(pvt->_out,DES_BLOCK_SIZE);
		}

	} else if (getUsePadding()) {

		// Truncate padding...
		//
		// Grab the last byte, which is guaranteed to be a pad-byte
		// (see above).  Then move back that many bytes from the end
		// and truncate the buffer there.
		getOut()->setPositionRelativeToEnd(-1);
		getOut()->read(&padbytes,sizeof(padbytes));
		if (padbytes>DES_BLOCK_SIZE) {
			encryption::setError(ENCRYPTION_ERROR_INVALID_PADDING);
			getOut()->clear();
			return NULL;
		}
		getOut()->setPositionRelativeToEnd(-padbytes);
		getOut()->truncate();
	}

	// reset the dirty flag
	setDirty(false);

	return getOut()->getBuffer();
}

void singledes::setError(int32_t err) {
	encryption::setError(ENCRYPTION_ERROR_NULL);
}

void singledes::newContext() {
	des_key_setup(getKey(),pvt->_ek,pvt->_dk);
}

void singledes::freeContext() {
	bytestring::zero(pvt->_ek,sizeof(pvt->_ek));
	bytestring::zero(pvt->_dk,sizeof(pvt->_dk));
}

bool singledes::isSupported() {
	// our own implementation only supports CBC
	return getBlockCipherMode()==BLOCK_CIPHER_MODE_CBC;
}
