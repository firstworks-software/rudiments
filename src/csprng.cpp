// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/csprng.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/device.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

#if defined(RUDIMENTS_HAS_SSL)
	#include <openssl/rand.h>
#elif defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
	#ifdef RUDIMENTS_HAVE_WINDOWS_H
		#include <windows.h>
	#endif
#endif

class csprngprivate {
	friend class csprng;
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

csprng::csprng() : rng() {
	pvt=new csprngprivate;

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
		// prng::getSeed() uses
		pvt->opened=pvt->dev.open("/dev/urandom",O_RDONLY);
	#endif
}

csprng::~csprng() {
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

bool csprng::setSeed(uint32_t seed) {
	// ignore the seed, a CSPRNG has no seed concept - this method exists
	// only to satisfy the rng interface
	return true;
}

bool csprng::generate(uint32_t *result) {
	return generateBytes((byte_t *)result,sizeof(uint32_t),
						sizeof(uint32_t));
}

bool csprng::generate(int32_t *result, int32_t lower, int32_t upper) {
	uint32_t	res;
	if (!generate(&res)) {
		return false;
	}
	*result=scale(res,lower,upper);
	return true;
}

byte_t *csprng::generateBytes(size_t size) {
	byte_t	*buffer=new byte_t[size];
	if (!generateBytes(buffer,size,size)) {
		delete[] buffer;
		return NULL;
	}
	return buffer;
}

bool csprng::generateBytes(byte_t *buffer, size_t buffersize,
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

bool csprng::generateBytes(byte_t *buffer, size_t buffersize) {
	return generateBytes(buffer,buffersize,buffersize);
}

bool csprng::generateBytes(bytebuffer *buffer, size_t size) {
	byte_t	*bytes=generateBytes(size);
	if (!bytes) {
		return false;
	}
	buffer->append(bytes,size);
	delete[] bytes;
	return true;
}

uint32_t csprng::getSeed() {
	// a CSPRNG has no seed to report, this is just a placeholder for
	// interface parity with prng
	return 0;
}

uint32_t csprng::generate() {
	csprng		r;
	uint32_t	result;
	return (r.generate(&result))?result:0;
}

int32_t csprng::generate(int32_t lower, int32_t upper) {
	csprng		r;
	int32_t		result;
	return (r.generate(&result,lower,upper))?result:0;
}

// the seeded statics below ignore "seed" and draw a real
// cryptographically secure value instead

uint32_t csprng::generate(uint32_t seed) {
	return generate();
}

int32_t csprng::generate(uint32_t seed, int32_t lower, int32_t upper) {
	return generate(lower,upper);
}

byte_t *csprng::generateBytes(uint32_t seed, size_t size) {
	csprng	r;
	return r.generateBytes(size);
}

bool csprng::generateBytes(uint32_t seed, byte_t *buffer, size_t buffersize,
								size_t size) {
	csprng	r;
	return r.generateBytes(buffer,buffersize,size);
}

bool csprng::generateBytes(uint32_t seed, byte_t *buffer, size_t buffersize) {
	csprng	r;
	return r.generateBytes(buffer,buffersize);
}

bool csprng::generateBytes(uint32_t seed, bytebuffer *buffer, size_t size) {
	csprng	r;
	return r.generateBytes(buffer,size);
}

int32_t csprng::scale(uint32_t number, int32_t lower, int32_t upper) {
	// the same approach prng::scale() uses, but against a fixed
	// 2^32 range, since generate(uint32_t*) always spans the full range
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

uint32_t csprng::getRandMax() {
	return 4294967295U;
}

bool csprng::getNeedsMutex() {
	// none of the backends hold shared mutable state, so no mutex is
	// ever needed
	return false;
}

void csprng::setMutex(threadmutex *mtx) {
	// no mutex is ever needed (see getNeedsMutex()), so ignore "mtx"
}

bool csprng::isSupported() {
	// the arms above are an exhaustive #if/#elif/#else, so one
	// backend is always compiled in - whether it actually
	// initializes for a given instance is a separate, runtime
	// question that generateBytes() answers instead
	return true;
}
