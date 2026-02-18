// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/des.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#if defined(RUDIMENTS_HAVE_CRYPT_R) && !defined(__USE_GNU)
	#define __USE_GNU
#endif
#if defined(RUDIMENTS_HAVE_CRYPT_H)
	#include <crypt.h>
#elif defined(RUDIMENTS_HAVE_UNISTD_H)
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#ifdef RUDIMENTS_HAVE_UNDECLARED_CRYPT
	#define RUDIMENTS_HAVE_CRYPT 1
	extern "C" {
		char *crypt(const char *key, const char *salt);
	}
#endif

class desprivate {
	friend class des;
	private:
		#if !defined(RUDIMENTS_HAVE_CRYPT_R)
			threadmutex	*_desmutex;
		#endif
};

des::des() : hash() {
	pvt=new desprivate;
	#if !defined(RUDIMENTS_HAVE_CRYPT_R)
	pvt->_desmutex=NULL;
	#endif
}

des::~des() {
	delete pvt;
}

size_t des::getRequiredSaltSize() {
	return 2;
}

bool des::setSalt(const byte_t *salt, size_t size) {
	if (size!=getRequiredSaltSize()) {
		setError(HASH_ERROR_INVALID_SALT_SIZE);
		return false;
	}
	return hash::setSalt(salt,size);
}

const byte_t *des::getHash() {

	#if defined(RUDIMENTS_HAVE_CRYPT_R) || defined(RUDIMENTS_HAVE_CRYPT)

		// reset the error
		setError(HASH_ERROR_SUCCESS);
		error::clearError();

		// if the dirty flag isn't set then we can just
		// return the existing output buffer
		if (!getDirty()) {
			return getOut()->getBuffer();
		}

		// clear the output buffer
		getOut()->clear();

		// get the salt as a string, handle degenerate salt
		char	salt[3];
		if (!getSalt()) {
			bytestring::zero(salt,3);
		} else {
			bytestring::copy(salt,getSalt(),getRequiredSaltSize());
			salt[getRequiredSaltSize()]='\0';
		}

		// get the input buffer as a string
		const byte_t	*in=getIn()->getBuffer();
		size_t		insize=getIn()->getSize();
		char		*password=new char[insize+1];
		bytestring::copy(password,in,insize);
		password[insize]='\0';

		// encrypt
		char	*encryptedpassword=NULL;
		#if defined(RUDIMENTS_HAVE_CRYPT_R)
			crypt_data	cd;
			bytestring::zero(&cd,sizeof(cd));
			encryptedpassword=crypt_r(password,salt,&cd);
			if (encryptedpassword) {
				getOut()->append(encryptedpassword);
				getOut()->append('\0');
			}
		#elif defined(RUDIMENTS_HAVE_CRYPT)
			if (pvt->_desmutex && !pvt->_desmutex->lock()) {
				return NULL;
			}
			encryptedpassword=::crypt(password,salt);
			if (encryptedpassword) {
				getOut()->append(encryptedpassword);
				getOut()->append('\0');
			}
			if (pvt->_desmutex) {
				pvt->_desmutex->unlock();
			}
		#endif

		// clean up
		delete[] password;

		// return error
		if (!encryptedpassword) {
			switch (error::getErrorNumber()) {
				case ERANGE:
				case ENOMEM:
					setError(
					HASH_ERROR_INPUT_TOO_LONG);
					break;
				case EINVAL:
				default:
					setError(
					HASH_ERROR_UNSUPPORTED);
			};
			return NULL;
		}

		// reset the dirty flag
		setDirty(false);

		return getOut()->getBuffer();

	#else
		setError(HASH_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

uint64_t des::getHashSize() {
	getHash();
	return getOut()->getSize()-1;
	
}

bool des::getNeedsMutex() {
	#if defined(RUDIMENTS_HAVE_CRYPT_R)
		return false;
	#elif !defined(RUDIMENTS_HAVE_CRYPT)
		return true;
	#else
		return false;
	#endif
}

void des::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_CRYPT_R)
		pvt->_desmutex=mtx;
	#endif
}

bool des::isSupported() {
	#if defined(RUDIMENTS_HAVE_CRYPT_R) || defined(RUDIMENTS_HAVE_CRYPT)
		return true;
	#else
		return false;
	#endif
}
