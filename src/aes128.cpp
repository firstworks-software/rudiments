// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/aes128.h>
#include <rudiments/bytestring.h>

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
			bool		_first;
			EVP_CIPHER_CTX	*_context;
			unsigned char	_key[16];
			unsigned char	_iv[AES_BLOCK_SIZE];
			unsigned char	*_out;
			int		_outlen;
		#else
			SHA1Context	_context;
			uint8_t	_hash[SHA1HashSize];
		#endif
		encryptionerror_t	_err;
};

aes128::aes128() : encryption() {
	pvt=new aes128private;
	pvt->_first=true;
	pvt->_context=NULL;
	bytestring::zero(pvt->_key,sizeof(pvt->_key));
	pvt->_out=NULL;
	pvt->_outlen=0;
	clear();
}

aes128::~aes128() {
	if (pvt->_context) {
		EVP_CIPHER_CTX_free(pvt->_context);
	}
	delete[] pvt->_out;
	delete pvt;
}

void aes128::setKey(const unsigned char *key) {
	bytestring::copy(pvt->_key,key,sizeof(pvt->_key));
}

unsigned char *aes128::getKey() {
	return pvt->_key;
}

uint32_t aes128::getKeySize() {
	return sizeof(pvt->_key);
}

void aes128::setIv(const unsigned char *iv) {
	bytestring::copy(pvt->_iv,iv,sizeof(pvt->_iv));
}

unsigned char *aes128::getIv() {
	return pvt->_iv;
}

uint32_t aes128::getIvSize() {
	return sizeof(pvt->_iv);
}

bool aes128::append(const unsigned char *data, uint32_t length) {

	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (pvt->_first) {

			pvt->_context=EVP_CIPHER_CTX_new();

			if (!EVP_CipherInit_ex(pvt->_context,
						EVP_aes_128_cbc(),
						NULL,
						pvt->_key,
						pvt->_iv,
						1)) {
				EVP_CIPHER_CTX_free(pvt->_context);
				setError(ERR_GET_REASON(ERR_get_error()));
				return false;
			}

			pvt->_first=false;
		}

		// grow the output buffer as necessary
		// FIXME: use a memorypool?
		if (!pvt->_out) {
			pvt->_out=new unsigned char
						[length+EVP_MAX_BLOCK_LENGTH];
		} else {
			unsigned char	*temp=new unsigned char
						[pvt->_outlen+length+
						EVP_MAX_BLOCK_LENGTH];
			bytestring::copy(temp,pvt->_out,pvt->_outlen);
			delete[] pvt->_out;
			pvt->_out=temp;
		}

		if (!EVP_CipherUpdate(pvt->_context,
					pvt->_out,&pvt->_outlen,
					data,length)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return false;
		}
		return true;
	#else
		int	result=SHA1Input(pvt->_context,data,length);
		setError(result);
		return (result==shaSuccess);
	#endif
}

const unsigned char *aes128::getEncryptedData() {
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	#if defined(RUDIMENTS_HAS_SSL)
		if (!EVP_CipherFinal_ex(pvt->_context,
					pvt->_out,&pvt->_outlen)) {
			setError(ERR_GET_REASON(ERR_get_error()));
			return NULL;
		}
		return pvt->_out;
	#else
		int	result=SHA1Result(&pvt->_context,pvt->_hash);
		setError(result);
		if (result==shaSuccess) {
			return pvt->_hash;
		}
		return NULL;
	#endif
}

uint64_t aes128::getEncryptedDataLength() {
	#if defined(RUDIMENTS_HAS_SSL)
		return pvt->_outlen;
	#else
		return SHA1HashSize;
	#endif
}

bool aes128::clear() {
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	delete[] pvt->_out;
	pvt->_out=NULL;
	pvt->_outlen=0;
	#if defined(RUDIMENTS_HAS_SSL)
		return true;
	#else
		int	result=SHA1Reset(&pvt->_context);
		setError(result);
		return (result==shaSuccess);
	#endif
}

encryptionerror_t aes128::getError() {
	return pvt->_err;
}

void aes128::setError(int32_t err) {
	#if defined(RUDIMENTS_HAS_SSL)
		// FIXME: implement this...
		pvt->_err=ENCRYPTION_ERROR_NULL;
		// clear the queue
		while (ERR_get_error()) {}
	#else
		switch (err) {
			case shaNull:
				pvt->_err=ENCRYPTION_ERROR_NULL;
			case shaInputTooLong:
				pvt->_err=ENCRYPTION_ERROR_INPUT_TOO_LONG;
			case shaStateError:
				pvt->_err=ENCRYPTION_ERROR_STATE_ERROR;
			default:
				pvt->_err=ENCRYPTION_ERROR_SUCCESS;
		}
	#endif
}
