// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/crypt.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/error.h>

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

class cryptprivate {
	friend class crypt;
	private:
		#if !defined(RUDIMENTS_HAVE_CRYPT_R)
			threadmutex	*_cryptmutex;
		#endif
};

crypt::crypt() : encryption() {
	pvt=new cryptprivate;
}

crypt::~crypt() {
	delete pvt;
}

uint32_t crypt::getKeySize() {
	return 0;
}

uint32_t crypt::getIvSize() {
	return 2;
}

const unsigned char *crypt::getEncryptedData() {

	#if defined(RUDIMENTS_HAVE_CRYPT_R) || defined(RUDIMENTS_HAVE_CRYPT)

		// reset the error
		setError(ENCRYPTION_ERROR_SUCCESS);
		error::clearError();

		// if the dirty flag isn't set then we can just
		// return the existing output buffer
		if (!getDirty()) {
			return getOut()->getBuffer();

		}

		// clear the output buffer
		getOut()->clear();

		// get the salt as a string
		char	salt[3];
		bytestring::copy(salt,getIv(),2);
		salt[2]='\0';

		// get the input buffer as a string
		const unsigned char	*in=getIn()->getBuffer();
		size_t			insize=getIn()->getSize();
		char	*password=new char[insize+1];
		bytestring::copy(password,in,insize);
		password[insize]='\0';

		// encrypt
		#if defined(RUDIMENTS_HAVE_CRYPT_R)
			crypt_data	cd;
			bytestring::zero(&cd,sizeof(cd));
			char	*encryptedpassword=crypt_r(password,salt,&cd);
			if (!error::getErrorNumber()) {
				getOut()->append(encryptedpassword);
				getOut()->append('\0');
			}
		#elif defined(RUDIMENTS_HAVE_CRYPT)
			if (_cryptmutex && !_cryptmutex->lock()) {
				return NULL;
			}
			char	*encryptedpassword=::crypt(password,salt);
			if (!error::getErrorNumber()) {
				getOut()->append(encryptedpassword);
				getOut()->append('\0');
			}
			if (_cryptmutex) {
				_cryptmutex->unlock();
			}
		#endif

		// clean up
		delete[] password;

		// reset the dirty flag
		setDirty(false);

		if (error::getErrorNumber()) {
			setError(ENCRYPTION_ERROR_INPUT_TOO_LONG);
			return NULL;
		}
		return getOut()->getBuffer();

	#else
		setError(ENCRYPTION_ERROR_UNSUPPORTED);
		RUDIMENTS_SET_ENOSYS
		return NULL;
	#endif
}

const unsigned char *crypt::getDecryptedData() {
	return NULL;
}

bool crypt::needsMutex() {
	#if defined(RUDIMENTS_HAVE_CRYPT_R)
		return false;
	#elif !defined(RUDIMENTS_HAVE_CRYPT)
		return true;
	#else
		return false;
	#endif
}

void crypt::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_CRYPT_R)
		pvt->_cryptmutex=mtx;
	#endif
}
