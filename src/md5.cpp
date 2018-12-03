// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#include "md5rfc1321.cpp"

class md5private {
	friend class md5;
	private:
		MD5_CONTEXT	_ctx;
		MD5		_md5;
		unsigned char	*_hash;
		unsigned char	_buff[16];
};

md5::md5() {
	pvt=new md5private;
	pvt->_md5.MD5Init(&pvt->_ctx);
	pvt->_hash=(unsigned char *)charstring::duplicate("");
	bytestring::zero(pvt->_buff,sizeof(pvt->_buff));
}

md5::~md5() {
	delete pvt;
}

bool md5::append(const unsigned char *data, uint32_t length) {
	pvt->_md5.MD5Update(&pvt->_ctx,data,length);
	return true;
}

const unsigned char *md5::getHash() {
	pvt->_md5.MD5Final(pvt->_buff,&pvt->_ctx);
	delete[] pvt->_hash;
	pvt->_hash=(unsigned char *)pvt->_md5.MD5ConvertToAscii(pvt->_buff);
	return pvt->_hash;
}

uint32_t md5::getHashLength() {
	return charstring::length(pvt->_hash);
}

bool md5::clear() {
	pvt->_md5.MD5Final(pvt->_buff,&pvt->_ctx);
	pvt->_md5.MD5Init(&pvt->_ctx);
	delete[] pvt->_hash;
	pvt->_hash=(unsigned char *)charstring::duplicate("");
	bytestring::zero(pvt->_buff,sizeof(pvt->_buff));
	return true;
}

hasherror_t md5::getError() {
	return HASH_ERROR_SUCCESS;
}
