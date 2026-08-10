// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/hash.h>
#include <rudiments/bytestring.h>
#include <rudiments/prng.h>
#include <rudiments/stdio.h>

class hashprivate {
	friend class hash;
	private:
		bytebuffer	_in;
		bytebuffer	_out;
		byte_t		*_salt;
		size_t		_saltsize;
		bool		_dirty;
		hasherror_t	_err;
};

hash::hash() : object() {
	pvt=new hashprivate;
	construct();
}

void hash::construct() {
	pvt->_salt=NULL;
	pvt->_saltsize=0;
	pvt->_dirty=true;
	pvt->_err=HASH_ERROR_SUCCESS;
}

hash::~hash() {
	delete[] pvt->_salt;
	delete pvt;
}

size_t hash::getRequiredSaltSize() {
	return 0;
}

bool hash::setSalt(const byte_t *salt, size_t saltsize) {
	delete[] pvt->_salt;
	pvt->_saltsize=saltsize;
	pvt->_salt=(byte_t *)bytestring::duplicate(salt,saltsize);
	pvt->_dirty=true;
	return true;
}

const byte_t *hash::getSalt() {
	return pvt->_salt;
}

size_t hash::getSaltSize() {
	return pvt->_saltsize;
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
	return clear();
}
