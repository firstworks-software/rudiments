// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/csrandomnumber.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/device.h>
#include <rudiments/error.h>

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/rand.h>
#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
#endif

class csrandomnumberprivate {
	friend class csrandomnumber;
	private:
		#if defined(RUDIMENTS_HAS_SSL)
			// RAND_bytes() is stateless, nothing to keep
		#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
			HCRYPTPROV	hprov;
			BOOL		acquired;
		#else
			device		dev;
			bool		opened;
		#endif
};

csrandomnumber::csrandomnumber() : object() {
	pvt=new csrandomnumberprivate;

	#if defined(RUDIMENTS_HAS_SSL)
		// nothing to init
	#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		pvt->hprov=NULL;
		pvt->acquired=CryptAcquireContext(
					&pvt->hprov,NULL,
					MS_DEF_PROV,PROV_RSA_FULL,
					CRYPT_VERIFYCONTEXT
					#if _WIN32_WINNT>0x0400
					|CRYPT_SILENT
					#endif
					);
	#else
		// read straight from /dev/urandom, the same source
		// randomnumber::getSeed() uses
		pvt->opened=pvt->dev.open("/dev/urandom",O_RDONLY);
	#endif
}

csrandomnumber::~csrandomnumber() {
	#if defined(RUDIMENTS_HAS_SSL)
		// nothing to clean up
	#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		if (pvt->acquired) {
			CryptReleaseContext(pvt->hprov,0);
		}
	#else
		if (pvt->opened) {
			pvt->dev.close();
		}
	#endif
	delete pvt;
}

bool csrandomnumber::generate(uint32_t *result) {
	return generateBytes((byte_t *)result,sizeof(uint32_t),
						sizeof(uint32_t));
}

bool csrandomnumber::generate(int32_t *result, int32_t lower, int32_t upper) {
	uint32_t	res;
	if (!generate(&res)) {
		return false;
	}
	*result=scale(res,lower,upper);
	return true;
}

byte_t *csrandomnumber::generateBytes(size_t size) {
	byte_t	*buffer=new byte_t[size];
	if (!generateBytes(buffer,size,size)) {
		delete[] buffer;
		return NULL;
	}
	return buffer;
}

bool csrandomnumber::generateBytes(byte_t *buffer, size_t buffersize,
							size_t size) {
	if (size>buffersize) {
		return false;
	}
	#if defined(RUDIMENTS_HAS_SSL)
		return RAND_bytes((unsigned char *)buffer,(int)size)==1;
	#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		return pvt->acquired &&
			CryptGenRandom(pvt->hprov,(DWORD)size,(BYTE *)buffer);
	#else
		return pvt->opened &&
			pvt->dev.read(buffer,size)==(ssize_t)size;
	#endif
}

bool csrandomnumber::generateBytes(byte_t *buffer, size_t buffersize) {
	return generateBytes(buffer,buffersize,buffersize);
}

bool csrandomnumber::generateBytes(bytebuffer *buffer, size_t size) {
	byte_t	*bytes=generateBytes(size);
	if (!bytes) {
		return false;
	}
	buffer->append(bytes,size);
	delete[] bytes;
	return true;
}

uint32_t csrandomnumber::generate() {
	csrandomnumber	r;
	uint32_t	result;
	return (r.generate(&result))?result:0;
}

int32_t csrandomnumber::generate(int32_t lower, int32_t upper) {
	csrandomnumber	r;
	int32_t		result;
	return (r.generate(&result,lower,upper))?result:0;
}

int32_t csrandomnumber::scale(uint32_t number, int32_t lower, int32_t upper) {
	// scale "number" from its full 0..2^32-1 range to lower..upper, the
	// same approach randomnumber::scale() uses, but against a fixed
	// 2^32 range since generate(uint32_t*) always spans the full
	// range, regardless of which backend is in use
	float	originalrange=(int64_t)getRandMax()+1;
	float	newrange=(float)abs(upper-lower)+1.0f;
	float	shrunk=((float)number)/originalrange;
	float	expanded=shrunk*newrange;
	int32_t	shifted=lower+(int32_t)expanded;
	// float only carries 24 bits of precision, so for number near
	// UINT32_MAX, shrunk can round up to exactly 1.0 and push shifted
	// one past upper - clamp it back into range
	return (shifted>upper)?upper:shifted;
}

uint32_t csrandomnumber::getRandMax() {
	return 4294967295U;
}

bool csrandomnumber::isSupported() {
	// openssl and CryptGenRandom are both real CSPRNGs, and when
	// neither is available, the fallback below reads /dev/urandom,
	// which is itself a legitimate cryptographically secure source on
	// POSIX systems (not a degraded fallback like randomnumber would
	// be), so a backend is always available
	return true;
}
