// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/encryption.h>
#include <rudiments/bytestring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

class encryptionprivate {
	friend class encryption;
	private:
		unsigned char		*_key;
		unsigned char		*_iv;
		bytebuffer		_in;
		bytebuffer		_out;
		bool			_dirty;
		bool			_encrypted;
		encryptionerror_t	_err;
};

encryption::encryption() {
	pvt=new encryptionprivate;
	pvt->_key=NULL;
	pvt->_iv=NULL;
	pvt->_dirty=true;
	pvt->_encrypted=true;
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
}

encryption::~encryption() {
	delete[] pvt->_key;
	delete[] pvt->_iv;
	delete pvt;
}

bool encryption::setKey(const unsigned char *key, size_t keysize) {
	initKey();
	if (keysize!=getKeySize()) {
		// FIXME: set wrong key-size error
		return false;
	}
	bytestring::copy(pvt->_key,key,keysize);
	pvt->_dirty=true;
	return true;
}

unsigned char *encryption::getKey() {
	initKey();
	return pvt->_key;
}

void encryption::initKey() {
	if (!pvt->_key) {
		pvt->_key=new unsigned char[getKeySize()];
		bytestring::zero(pvt->_key,getKeySize());
	}
}

bool encryption::setIv(const unsigned char *iv, size_t ivsize) {
	initIv();
	if (ivsize!=getIvSize()) {
		// FIXME: set wrong iv-size error
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

void encryption::setRandomBuffer(unsigned char *buffer, size_t buffersize) {
	randomnumber	r;
	uint32_t	seed=randomnumber::getSeed();
	size_t		remaining=buffersize;
	unsigned char	*ptr=buffer;
	for (;;) {
		seed=randomnumber::generateNumber(seed);
		size_t	blocksize=remaining%sizeof(uint32_t);
		if (!blocksize) {
			blocksize=sizeof(uint32_t);
		}
		bytestring::copy(ptr,(unsigned char *)&seed,blocksize);
		if (blocksize<sizeof(uint32_t)) {
			break;
		}
		ptr+=sizeof(uint32_t);
		remaining-=sizeof(uint32_t);
		if (!remaining) {
			break;
		}
	}
}

unsigned char *encryption::getIv() {
	initIv();
	return pvt->_iv;
}

void encryption::initIv() {
	if (!pvt->_iv) {
		pvt->_iv=new unsigned char[getIvSize()];
		bytestring::zero(pvt->_iv,getIvSize());
	}
}

bool encryption::append(const unsigned char *data, uint32_t length) {
	pvt->_in.append(data,length);
	pvt->_dirty=true;
	return true;
}

bytebuffer *encryption::getIn() {
	return &pvt->_in;
}

bytebuffer *encryption::getOut() {
	return &pvt->_out;
}

uint64_t encryption::getEncryptedDataLength() {
	getEncryptedData();
	return pvt->_out.getSize();
}

uint64_t encryption::getDecryptedDataLength() {
	getDecryptedData();
	return pvt->_out.getSize();
}

bool encryption::clear() {
	pvt->_err=ENCRYPTION_ERROR_SUCCESS;
	pvt->_in.clear();
	pvt->_out.clear();
	pvt->_dirty=true;
	return true;
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
