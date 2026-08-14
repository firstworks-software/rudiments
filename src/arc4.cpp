// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/arc4.h>
#include <rudiments/bytestring.h>

// Without RC4 in libcrypto, the built-in implementation is used instead.
#if !defined(RUDIMENTS_HAS_RC4)
	#undef RUDIMENTS_HAS_SSL
#endif

// The low-level RC4_set_key()/RC4() functions are used rather than
// EVP_rc4().  EVP_rc4() requires the legacy provider to be loaded on OpenSSL
// 3.0 and up, but the low-level functions are direct libcrypto C
// implementations that the provider split doesn't apply to.  They are
// deprecated, but rudiments builds with -Wno-deprecated-declarations.
#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/rc4.h>
#else
	#include "arc4scratch.cpp"
#endif

class arc4private {
	friend class arc4;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			RC4_KEY		_rc4key;
		#else
			unsigned char	_state[256];
			unsigned char	_s1;
			unsigned char	_s2;
		#endif
		bool	_haskey;
};

arc4::arc4() {
	pvt=new arc4private;
	pvt->_haskey=false;
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(&pvt->_rc4key,sizeof(pvt->_rc4key));
	#else
		bytestring::zero(pvt->_state,sizeof(pvt->_state));
		pvt->_s1=0;
		pvt->_s2=0;
	#endif
}

arc4::arc4(const byte_t *key, size_t keysize) {
	pvt=new arc4private;
	pvt->_haskey=false;
	setKey(key,keysize);
}

// The key schedule is key material, so it's zeroed on the way out.
arc4::~arc4() {
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(&pvt->_rc4key,sizeof(pvt->_rc4key));
	#else
		bytestring::zero(pvt->_state,sizeof(pvt->_state));
		pvt->_s1=0;
		pvt->_s2=0;
	#endif
	pvt->_haskey=false;
	delete pvt;
}

bool arc4::setKey(const byte_t *key, size_t keysize) {

	// discard the previous keystream state
	pvt->_haskey=false;
	#if defined(RUDIMENTS_HAS_SSL)
		bytestring::zero(&pvt->_rc4key,sizeof(pvt->_rc4key));
	#else
		bytestring::zero(pvt->_state,sizeof(pvt->_state));
		pvt->_s1=0;
		pvt->_s2=0;
	#endif

	if (!key || !keysize || keysize>256) {
		return false;
	}

	// initialize the keystream from the key
	#if defined(RUDIMENTS_HAS_SSL)
		RC4_set_key(&pvt->_rc4key,
				(int)keysize,
				(const unsigned char *)key);
	#else
		arc4scratchksa(pvt->_state,&pvt->_s1,&pvt->_s2,key,keysize);
	#endif

	pvt->_haskey=true;
	return true;
}

bool arc4::crypt(byte_t *buffer, size_t size) {

	if (!pvt->_haskey) {
		return false;
	}

	if (!size) {
		return true;
	}

	if (!buffer) {
		return false;
	}

	#if defined(RUDIMENTS_HAS_SSL)
		RC4(&pvt->_rc4key,
			(size_t)size,
			(const unsigned char *)buffer,
			(unsigned char *)buffer);
	#else
		arc4scratchprga(pvt->_state,&pvt->_s1,&pvt->_s2,buffer,size);
	#endif

	return true;
}

bool arc4::isSupported() {
	return true;
}
