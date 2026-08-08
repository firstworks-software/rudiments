// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/encryption.h>
#include <rudiments/bytestring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

class encryptionprivate {
	friend class encryption;
	private:
		bytebuffer		_in;
		bytebuffer		_out;
		byte_t			*_key;
		byte_t			*_iv;
		blockciphermode_t	_mode;
		bool			_padding;
		bool			_dirty;
		bool			_encrypted;
		encryptionerror_t	_err;
};

encryption::encryption() : object() {
	pvt=new encryptionprivate;
	construct();
}

void encryption::construct() {
	pvt->_key=NULL;
	pvt->_iv=NULL;
	pvt->_mode=BLOCK_CIPHER_MODE_CBC;
	pvt->_padding=true;
	pvt->_dirty=true;
	pvt->_encrypted=true;
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
}

encryption::~encryption() {
	delete[] pvt->_key;
	delete[] pvt->_iv;
	delete pvt;
}

bool encryption::setKey(const byte_t *key, size_t keysize) {
	initKey();
	if (keysize!=getKeySize()) {
		setError(ENCRYPTION_ERROR_INVALID_KEY_SIZE);
		return false;
	}
	bytestring::copy(pvt->_key,key,keysize);
	pvt->_dirty=true;
	return true;
}

byte_t *encryption::getKey() {
	initKey();
	return pvt->_key;
}

void encryption::initKey() {
	if (!pvt->_key) {
		pvt->_key=new byte_t[getKeySize()];
		bytestring::zero(pvt->_key,getKeySize());
	}
}

bool encryption::setIv(const byte_t *iv, size_t ivsize) {
	initIv();
	if (ivsize!=getIvSize()) {
		setError(ENCRYPTION_ERROR_INVALID_IV_SIZE);
		return false;
	}
	bytestring::copy(pvt->_iv,iv,ivsize);
	pvt->_dirty=true;
	return true;
}

void encryption::setRandomKey() {
	initKey();
	setRandomBuffer(pvt->_key,getKeySize());
}

void encryption::setRandomIv() {
	initIv();
	setRandomBuffer(pvt->_iv,getIvSize());
}

void encryption::setRandomBuffer(byte_t *buffer, size_t buffersize) {
	randomnumber	r;
	// seed explicitly, some platforms don't self-seed on construction
	r.setSeed(randomnumber::getSeed());
	r.generateBytes(buffer,buffersize);
}

byte_t *encryption::getIv() {
	initIv();
	return pvt->_iv;
}

void encryption::initIv() {
	if (!pvt->_iv) {
		pvt->_iv=new byte_t[getIvSize()];
		bytestring::zero(pvt->_iv,getIvSize());
	}
}

void encryption::setBlockCipherMode(blockciphermode_t mode) {
	pvt->_mode=mode;
}

blockciphermode_t encryption::getBlockCipherMode() {
	return pvt->_mode;
}

void encryption::setPadding(bool padding) {
	pvt->_padding=padding;
	pvt->_dirty=true;
}

bool encryption::getPadding() {
	return pvt->_padding;
}

bool encryption::append(const byte_t *data, uint32_t size) {
	pvt->_in.append(data,size);
	pvt->_dirty=true;
	return true;
}

bytebuffer *encryption::getIn() {
	return &pvt->_in;
}

bytebuffer *encryption::getOut() {
	return &pvt->_out;
}

uint64_t encryption::getEncryptedDataSize() {
	getEncryptedData();
	return pvt->_out.getSize();
}

uint64_t encryption::getDecryptedDataSize() {
	getDecryptedData();
	return pvt->_out.getSize();
}

encryptionerror_t encryption::getError() {
	return pvt->_err;
}

void encryption::setError(encryptionerror_t err) {
	pvt->_err=err;
}

void encryption::setDirty(bool dirty) {
	pvt->_dirty=dirty;
}

bool encryption::getDirty() {
	return pvt->_dirty;
}

void encryption::setEncrypted(bool encrypted) {
	pvt->_encrypted=encrypted;
}

bool encryption::getEncrypted() {
	return pvt->_encrypted;
}

bool encryption::clear() {
	pvt->_in.clear();
	pvt->_out.clear();
	pvt->_dirty=true;
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	return true;
}

bool encryption::reset() {
	pvt->_in.clear();
	pvt->_out.clear();
	delete[] pvt->_key;
	delete[] pvt->_iv;
	construct();
	return true;
}
