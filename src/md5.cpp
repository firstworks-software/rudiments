// Copyright (c) 2016  David Muse
// See the file COPYING for more information

#include <rudiments/md5.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/md5.h>
#else
	#include "md5rfc1321.cpp"
#endif

class md5private {
	friend class md5;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			MD5_CTX		_context;
		#else
			MD5_CONTEXT	_context;
			MD5		_md5;
		#endif
		unsigned char	*_hash;
		unsigned char	_buff[16];
};

md5::md5() {
	pvt=new md5private;
	pvt->_hash=NULL;
	clear();
}

md5::~md5() {
	delete pvt;
}

bool md5::append(const unsigned char *data, uint32_t length) {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Update(&pvt->_context,data,length)) {
			// FIXME: set error...
			return false;
		}
		return true;
	#else
		pvt->_md5.MD5Update(&pvt->_context,data,length);
		return true;
	#endif
}

const unsigned char *md5::getHash() {
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Final(pvt->_buff,&pvt->_context)) {
			// FIXME: set error...
			return (const unsigned char *)"";
		}
	#else
		pvt->_md5.MD5Final(pvt->_buff,&pvt->_context);
	#endif
	delete[] pvt->_hash;
	pvt->_hash=new unsigned char[sizeof(pvt->_buff)*2+1];
	uint16_t	hi=0;
	for (uint16_t bi=0; bi<sizeof(pvt->_buff); bi++) {
		charstring::printf((char *)&pvt->_hash[hi],3,
					"%02x",pvt->_buff[bi]);
		hi+=2;
	}
	pvt->_hash[sizeof(pvt->_buff)*2]='\0';
	return pvt->_hash;
}

uint32_t md5::getHashLength() {
	return charstring::length(pvt->_hash);
}

bool md5::clear() {
	delete[] pvt->_hash;
	pvt->_hash=(unsigned char *)charstring::duplicate("");
	bytestring::zero(pvt->_buff,sizeof(pvt->_buff));
	#if defined(RUDIMENTS_HAS_SSL)
		if (!MD5_Init(&pvt->_context)) {
			// FIXME: set error
			return false;
		}
	#else
		pvt->_md5.MD5Init(&pvt->_context);
	#endif
	return true;
}

hasherror_t md5::getError() {
	return HASH_ERROR_SUCCESS;
}
