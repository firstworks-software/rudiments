// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/hash.h>
#include <rudiments/bytestring.h>
#include <rudiments/randomnumber.h>
#include <rudiments/stdio.h>

class hashprivate {
	friend class hash;
	private:
		bytebuffer	_in;
		bytebuffer	_out;
		byte_t		*_salt;
		bool		_dirty;
		hasherror_t	_err;
};

hash::hash() : object() {
	pvt=new hashprivate;
	construct();
}

void hash::construct() {
	pvt->_salt=NULL;
	pvt->_dirty=true;
	pvt->_err=HASH_ERROR_SUCCESS;
}

hash::~hash() {
	delete[] pvt->_salt;
	delete pvt;
}

bool hash::setSalt(const byte_t *salt, size_t saltsize) {
	initSalt();
	if (saltsize!=getSaltSize()) {
		setError(HASH_ERROR_INVALID_SALT_SIZE);
		return false;
	}
	delete[] pvt->_salt;
	pvt->_salt=(byte_t *)bytestring::duplicate(salt,saltsize);
	pvt->_dirty=true;
	return true;
}

const byte_t *hash::getSalt() {
	initSalt();
	return pvt->_salt;
}

size_t hash::getSaltSize() {
	return 0;
}

void hash::initSalt() {
	if (!pvt->_salt) {
		pvt->_salt=new byte_t[getSaltSize()];
		bytestring::zero(pvt->_salt,getSaltSize());
	}
}

bool hash::append(const byte_t *data, uint32_t size) {
	pvt->_in.append(data,size);
	pvt->_dirty=true;
	return true;
}

bytebuffer *hash::getIn() {
	return &pvt->_in;
}

bytebuffer *hash::getOut() {
	return &pvt->_out;
}

uint64_t hash::getHashSize() {
	getHash();
	return pvt->_out.getSize();
}

hasherror_t hash::getError() {
	return pvt->_err;
}

void hash::setError(hasherror_t err) {
	pvt->_err=err;
}

void hash::setDirty(bool dirty) {
	pvt->_dirty=dirty;
}

bool hash::getDirty() {
	return pvt->_dirty;
}

bool hash::clear() {
	pvt->_in.clear();
	pvt->_out.clear();
	pvt->_dirty=true;
	pvt->_err=HASH_ERROR_SUCCESS;
	return true;
}

bool hash::reset() {
	pvt->_in.clear();
	pvt->_out.clear();
	delete[] pvt->_salt;
	construct();
	return true;
}
