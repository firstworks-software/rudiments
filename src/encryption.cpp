// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/encryption.h>
#include <rudiments/bytestring.h>
#include <rudiments/bytebuffer.h>

class encryptionprivate {
	friend class encryption;
	private:
		unsigned char		*_key[16];
		unsigned char		*_iv[AES_BLOCK_SIZE];
		bytebuffer		_in;
		unsigned char		*_out;
		uint32_t		_outlen;
		encryptionerror_t	_err;
};

encryption::encryption() {
	pvt=new encrpytionprivate;
	pvt->_key=new unsigned char[getKeySize()];
	bytestring::zero(pvt->_key,sizeof(pvt->_key));
	pvt->_iv=new unsigned char[getIvSize()];
	bytestring::zero(pvt->_iv,sizeof(pvt->_iv));
	pvt->_out=NULL;
	pvt->_outlen=0;
	pvt->_dirty=true;
	pvt->_encrypted=true;
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
}

encryption::~encryption() {
	delete[] pvt->_key;
	delete[] pvt->_iv;
	delete[] pvt->_out;
	delete pvt;
}

bool encryption::setKey(const unsigned char *key, size_t keysize) {
	if (keysize!=sizeof(pvt->_key)) {
		// FIXME: set wrong key-size error
		return false;
	}
	bytestring::copy(pvt->_key,key,keysize);
	pvt->_dirty=true;
	return true;
}

unsigned char *encryption::getKey() {
	return pvt->_key;
}

uint32_t encryption::getKeySize() {
	return sizeof(pvt->_key);
}

bool encryption::setIv(const unsigned char *iv, size_t ivsize) {
	if (ivsize!=sizeof(pvt->_iv)) {
		// FIXME: set wrong iv-size error
		return false;
	}
	bytestring::copy(pvt->_iv,iv,sizeof(pvt->_iv));
	pvt->_dirty=true;
	return true;
}

unsigned char *encryption::getIv() {
	return pvt->_iv;
}

uint32_t encryption::getIvSize() {
	return sizeof(pvt->_iv);
}

bool encryption::append(const unsigned char *data, uint32_t length) {
	pvt->_in.append(data,length);
	return true;
}

bytebuffer *encryption::getIn() {
	return pvt->_in;
}

unsigned char *encryption::getOut() {
	return pvt->_out;
}

uint32_t *encryption::getOutLengthPointer() {
	return &pvt->_outlen;
}

void encryption::reallocateOut(uint32_t size) {
	delete[] pvt->_out;
	pvt->_out=new unsigned char[size];
}

uint64_t encryption::getDataLength() {
	return pvt->_outlen;
}

bool encryption::clear() {
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	pvt->_in.clear();
	delete[] pvt->_out;
	pvt->_out=NULL;
	pvt->_outlen=0;
	pvt->_dirty=true;
	return true;
}

encryptionerror_t encryption::getError() {
	return pvt->_err;
}

void encryption::setError(int32_t err) {
	pvt->_err=err;
}
