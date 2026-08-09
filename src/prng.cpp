// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/prng.h>
#include <rudiments/bytebuffer.h>
#include <rudiments/bytestring.h>
#include <rudiments/device.h>
#include <rudiments/datetime.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

#ifdef RUDIMENTS_HAVE_STDLIB_H
	#include <stdlib.h>
#endif

// LAME: not in the class
#if !defined(RUDIMENTS_HAVE_CRYPTGENRANDOM) && \
	!defined(RUDIMENTS_HAVE_ARC4RANDOM) && \
	!defined(RUDIMENTS_HAVE_RANDOM_R) && \
	!defined(RUDIMENTS_HAVE_RAND_R) && \
	!defined(RUDIMENTS_HAVE_LRAND48_R)
static threadmutex	*_rnmutex;
#endif

class prngprivate {
	friend class prng;
	private:
		#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
			HCRYPTPROV	hprov;
			BOOL		acquired;
			uint32_t	seed;
		#elif defined(RUDIMENTS_HAVE_RANDOM_R)
			char		statebuf[64];
			random_data	buffer;
		#elif defined(RUDIMENTS_HAVE_RAND_R) && \
			!defined(RUDIMENTS_HAVE_ARC4RANDOM)
			uint32_t	seed;
		#elif defined(RUDIMENTS_HAVE_LRAND48_R)
			drand48_data	buffer;
		#endif
};

prng::prng() : rng() {
	pvt=new prngprivate;

	#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		pvt->hprov=NULL;
		pvt->acquired=CryptAcquireContext(
					&pvt->hprov,NULL,
					MS_DEF_PROV,PROV_RSA_FULL,
					CRYPT_VERIFYCONTEXT
					#if _WIN32_WINNT>0x0400
					|CRYPT_SILENT
					#endif
					);
		if (!pvt->acquired) {
			return;
		}
	#endif
}

prng::~prng() {

	#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		if (pvt->acquired) {
			CryptReleaseContext(pvt->hprov,0);
		}
	#endif

	delete pvt;
}

bool prng::setSeed(uint32_t seed) {

	#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		pvt->seed=seed;
		return true;
	#elif defined(RUDIMENTS_HAVE_ARC4RANDOM)
		// do nothing, arc4random is seeded automatically
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM_R)
		bytestring::zero(pvt->statebuf,sizeof(pvt->statebuf));
		bytestring::zero(&pvt->buffer,sizeof(pvt->buffer));
		return !initstate_r(seed,pvt->statebuf,
					sizeof(pvt->statebuf),
					&pvt->buffer);
	#elif defined(RUDIMENTS_HAVE_RAND_R)
		pvt->seed=seed;
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48_R)
		bytestring::zero(&pvt->buffer,sizeof(pvt->buffer));
		return !srand48_r(seed,&pvt->buffer);
	#elif defined(RUDIMENTS_HAVE_RANDOM)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srandom(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_RAND)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srand(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		srand48(seed);
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#else
		#error "Couldn't find a suitable replacement for rand/srand"
	#endif
}

bool prng::generate(uint32_t *result) {

	#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM)
		if (pvt->acquired &&
			CryptGenRandom(pvt->hprov,4,(BYTE *)&pvt->seed)) {
			*result=pvt->seed;
			return true;
		}
		return false;
	#elif defined(RUDIMENTS_HAVE_ARC4RANDOM)
		*result=arc4random();
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM_R)
		int32_t	res;
		if (!random_r(&pvt->buffer,&res)) {
			*result=res;
			return true;
		}
		return false;
	#elif defined(RUDIMENTS_HAVE_RAND_R)
		int32_t	res=rand_r(&pvt->seed);
		pvt->seed=res;
		*result=pvt->seed;
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48_R)
		long	res;
		if (lrand48_r(&pvt->buffer,&res)) {
			return false;
		}
		*result=res;
		return true;
	#elif defined(RUDIMENTS_HAVE_RANDOM)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=random();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_RAND)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=rand();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#elif defined(RUDIMENTS_HAVE_LRAND48)
		if (_rnmutex && !_rnmutex->lock()) {
			return false;
		}
		*result=lrand48();
		if (_rnmutex) {
			_rnmutex->unlock();
		}
		return true;
	#else
		#error "Couldn't find a suitable replacement for rand/srand"
	#endif
}

bool prng::generate(int32_t *result, int32_t lower, int32_t upper) {
	uint32_t	res;
	if (!generate(&res)) {
		return false;
	}
	*result=scale(res,lower,upper);
	return true;
}

byte_t *prng::generateBytes(size_t size) {
	byte_t	*buffer=new byte_t[size];
	if (!generateBytes(buffer,size,size)) {
		delete[] buffer;
		return NULL;
	}
	return buffer;
}

bool prng::generateBytes(byte_t *buffer, size_t buffersize,
							size_t size) {
	if (size>buffersize) {
		return false;
	}
	byte_t	*ptr=buffer;
	size_t	remaining=size;
	while (remaining) {
		uint32_t	chunk;
		if (!generate(&chunk)) {
			return false;
		}
		size_t	blocksize=(remaining<sizeof(uint32_t))?
					remaining:sizeof(uint32_t);
		bytestring::copy(ptr,(byte_t *)&chunk,blocksize);
		ptr+=blocksize;
		remaining-=blocksize;
	}
	return true;
}

bool prng::generateBytes(byte_t *buffer, size_t buffersize) {
	return generateBytes(buffer,buffersize,buffersize);
}

bool prng::generateBytes(bytebuffer *buffer, size_t size) {
	byte_t	*bytes=generateBytes(size);
	if (!bytes) {
		return false;
	}
	buffer->append(bytes,size);
	delete[] bytes;
	return true;
}

uint32_t prng::getSeed() {

	// first try /dev/urandom
	device	d;
	if (d.open("/dev/urandom",O_RDONLY)) {
		uint32_t	retval=0;
		if (d.read((void *)&retval,sizeof(retval))==sizeof(retval)) {
			return retval;
		}
	}

	// if that fails, use epoch
	datetime	dt;
	dt.initFromSystemDateTime();
	return dt.getEpoch();
}

uint32_t prng::generate(uint32_t seed) {
	prng	r;
	uint32_t	result;
	return (r.setSeed(seed) &&
		r.generate(&result))?result:0;
}

int32_t prng::generate(uint32_t seed, int32_t lower, int32_t upper) {
	prng	r;
	int32_t		result;
	return (r.setSeed(seed) &&
		r.generate(&result,lower,upper))?result:0;
}

byte_t *prng::generateBytes(uint32_t seed, size_t size) {
	prng	r;
	return (r.setSeed(seed))?r.generateBytes(size):NULL;
}

bool prng::generateBytes(uint32_t seed, byte_t *buffer,
						size_t buffersize, size_t size) {
	prng	r;
	return r.setSeed(seed) &&
		r.generateBytes(buffer,buffersize,size);
}

bool prng::generateBytes(uint32_t seed, byte_t *buffer,
						size_t buffersize) {
	prng	r;
	return r.setSeed(seed) &&
		r.generateBytes(buffer,buffersize);
}

bool prng::generateBytes(uint32_t seed, bytebuffer *buffer,
						size_t size) {
	prng	r;
	return r.setSeed(seed) &&
		r.generateBytes(buffer,size);
}

int32_t prng::scale(uint32_t number, int32_t lower, int32_t upper) {
	float	originalrange=(int64_t)getRandMax()+1;
	float	newrange=(float)abs(upper-lower)+1.0;
	float	shrunk=((float)number)/originalrange;
	float	expanded=shrunk*newrange;
	int32_t	shifted=lower+(int32_t)expanded;
	// float only carries 24 bits of precision, so for number near
	// getRandMax(), shrunk can round up to exactly 1.0 and push shifted
	// one past upper - clamp it back into range
	return (shifted>upper)?upper:shifted;
}

uint32_t prng::getRandMax() {
	#if defined(RUDIMENTS_HAVE_CRYPTGENRANDOM) || \
			defined(RUDIMENTS_HAVE_ARC4RANDOM)
		return 4294967295U;
	#else
		return RAND_MAX;
	#endif
}

bool prng::getNeedsMutex() {
	#if !defined(RUDIMENTS_HAVE_CRYPTGENRANDOM) && \
		!defined(RUDIMENTS_HAVE_ARC4RANDOM) && \
		!defined(RUDIMENTS_HAVE_RANDOM_R) && \
		!defined(RUDIMENTS_HAVE_RAND_R) && \
		!defined(RUDIMENTS_HAVE_LRAND48_R)
		return true;
	#else
		return false;
	#endif
}

void prng::setMutex(threadmutex *mtx) {
	#if !defined(RUDIMENTS_HAVE_CRYPTGENRANDOM) && \
		!defined(RUDIMENTS_HAVE_ARC4RANDOM) && \
		!defined(RUDIMENTS_HAVE_RANDOM_R) && \
		!defined(RUDIMENTS_HAVE_RAND_R) && \
		!defined(RUDIMENTS_HAVE_LRAND48_R)
		_rnmutex=mtx;
	#endif
}
