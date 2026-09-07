// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSPRNG_H
#define RUDIMENTS_CSPRNG_H

#include <rudiments/private/csprngincludes.h>

/** The csprng class provides methods for generating cryptographically
 *  secure random numbers.
 *
 *  Unlike prng, csprng offers no way to seed or otherwise reproduce its
 *  output.  A seeded, deterministic sequence is exactly what
 *  "cryptographically secure" rules out.  Use prng instead if you need a
 *  reproducible sequence (eg. for scaling, jitter, or test fixtures).
 *
 *  csprng does provide setSeed(), getSeed(), and the seeded static
 *  generate() and generateBytes() methods, but only so that it presents the
 *  same interface as rng and prng, and can be used interchangeably with
 *  them.  They are degenerate - the seed is ignored, and each call draws a
 *  fresh, cryptographically secure, non-reproducible value.  See each
 *  method for details.
 *
 *  csprng is intended for secrets - keys, tokens, salts, nonces,
 *  session ids, and anything else that must be unpredictable even to
 *  someone who can observe other output from the same process.
 *
 *  csprng uses openssl's RAND_bytes() when available, the Windows
 *  CryptGenRandom() API when building for Windows, and /dev/urandom
 *  otherwise.  If /dev/urandom cannot be opened, csprng falls back
 *  further to an internal prng instance, which still produces output
 *  but is NOT cryptographically secure.  Call
 *  isCryptographicallySecure() if you need a guarantee that the output
 *  is suitable for cryptographic use. */
class RUDIMENTS_DLLSPEC csprng : public rng {
	public:

		/** Creates a new instance of the csprng class. */
		csprng();

		/** Deletes this instance of the csprng class. */
		virtual	~csprng();

		/** Does nothing and returns true.
		 *
		 *  A CSPRNG has no seed concept, so "seed" is ignored.  This
		 *  method exists only so that csprng satisfies the rng
		 *  interface. */
		virtual bool	setSeed(uint32_t seed);

		/** Generates a cryptographically secure random number between
		 *  0 and 2^32-1 and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generate(uint32_t *result);

		/** Generates a cryptographically secure random number between
		 *  "lower" and "upper" and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generate(int32_t *result, int32_t lower,
							int32_t upper);

		/** Allocates a new buffer of "size" bytes, fills it with
		 *  cryptographically secure random bytes and returns it.
		 *  The caller is responsible for calling delete[] on the
		 *  returned buffer.
		 *
		 *  Returns NULL on failure. */
		virtual byte_t	*generateBytes(size_t size);

		/** Fills the first "size" bytes of "buffer" (which is
		 *  "buffersize" bytes long) with cryptographically secure
		 *  random bytes.
		 *
		 *  Returns false, without modifying "buffer", if "size" is
		 *  greater than "buffersize".  Returns true on success and
		 *  false on failure. */
		virtual bool	generateBytes(byte_t *buffer, size_t buffersize,
							size_t size);

		/** Fills "buffer", which is "buffersize" bytes long, entirely
		 *  with cryptographically secure random bytes.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generateBytes(byte_t *buffer, size_t buffersize);

		/** Appends "size" cryptographically secure random bytes to
		 *  "buffer".
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generateBytes(bytebuffer *buffer, size_t size);

		/** Returns true if this instance is actually backed by a
		 *  cryptographically secure source - openssl's RAND_bytes(),
		 *  the Windows CryptGenRandom() API, or a successfully
		 *  opened /dev/urandom - and false otherwise.
		 *
		 *  False means this instance has fallen back to an internal
		 *  prng instance, because /dev/urandom could not be opened.
		 *  It still generates output, but that output is not
		 *  cryptographically secure.  See the prng class for what
		 *  that fallback provides. */
		virtual bool	isCryptographicallySecure();

		/** Returns 0.
		 *
		 *  A CSPRNG has no seed to report, so this is a meaningless
		 *  placeholder.  It exists only so that csprng presents the
		 *  same interface as prng. */
		static	uint32_t	getSeed();

		/** Generates a cryptographically secure random number between
		 *  0 and 2^32-1 and returns it, or 0 on failure.
		 *
		 *  Unlike prng::generate(seed), this is not seeded and
		 *  not reproducible - each call is an independent
		 *  cryptographically secure draw.  Each call also opens and
		 *  closes the underlying source, so code making many draws in
		 *  a loop is better off using an instance of csprng
		 *  directly. */
		static	uint32_t	generate();

		/** Generates a cryptographically secure random number between
		 *  "lower" and "upper" and returns it, or 0 on failure.
		 *
		 *  Unlike prng::generate(seed,lower,upper), this is
		 *  not seeded and not reproducible - each call is an
		 *  independent cryptographically secure draw.  Each call also
		 *  opens and closes the underlying source, so code making many
		 *  draws in a loop is better off using an instance of
		 *  csprng directly. */
		static	int32_t	generate(int32_t lower, int32_t upper);

		// The seeded static methods below ignore "seed" entirely.
		// They exist only so that csprng presents the same static
		// interface as prng, and can be swapped in for it.  Unlike
		// prng's seeded statics, which are reproducible, each of these
		// draws a fresh cryptographically secure value.

		/** Generates a cryptographically secure random number between
		 *  0 and 2^32-1 and returns it, or 0 on failure.
		 *
		 *  "seed" is ignored.  Unlike prng::generate(seed), this is
		 *  not reproducible - each call is an independent
		 *  cryptographically secure draw. */
		static	uint32_t	generate(uint32_t seed);

		/** Generates a cryptographically secure random number between
		 *  "lower" and "upper" and returns it, or 0 on failure.
		 *
		 *  "seed" is ignored.  Unlike
		 *  prng::generate(seed,lower,upper), this is not reproducible
		 *  - each call is an independent cryptographically secure
		 *  draw. */
		static	int32_t	generate(uint32_t seed, int32_t lower,
							int32_t upper);

		/** Allocates a new buffer of "size" bytes, fills it with
		 *  cryptographically secure random bytes and returns it.  The
		 *  caller is responsible for calling delete[] on the returned
		 *  buffer.
		 *
		 *  "seed" is ignored.  Unlike prng::generateBytes(seed,size),
		 *  this is not reproducible - each call draws independent
		 *  cryptographically secure bytes.
		 *
		 *  Returns NULL on failure. */
		static	byte_t	*generateBytes(uint32_t seed, size_t size);

		/** Fills the first "size" bytes of "buffer" (which is
		 *  "buffersize" bytes long) with cryptographically secure
		 *  random bytes.
		 *
		 *  "seed" is ignored.  Unlike prng's seeded static
		 *  generateBytes(), this is not reproducible - each call draws
		 *  independent cryptographically secure bytes.
		 *
		 *  Returns false, without modifying "buffer", if "size" is
		 *  greater than "buffersize".  Returns true on success and
		 *  false on failure. */
		static	bool	generateBytes(uint32_t seed, byte_t *buffer,
							size_t buffersize,
							size_t size);

		/** Fills "buffer", which is "buffersize" bytes long, entirely
		 *  with cryptographically secure random bytes.
		 *
		 *  "seed" is ignored.  Unlike prng's seeded static
		 *  generateBytes(), this is not reproducible - each call draws
		 *  independent cryptographically secure bytes.
		 *
		 *  Returns true on success and false on failure. */
		static	bool	generateBytes(uint32_t seed, byte_t *buffer,
							size_t buffersize);

		/** Appends "size" cryptographically secure random bytes to
		 *  "buffer".
		 *
		 *  "seed" is ignored.  Unlike prng's seeded static
		 *  generateBytes(), this is not reproducible - each call draws
		 *  independent cryptographically secure bytes.
		 *
		 *  Returns true on success and false on failure. */
		static	bool	generateBytes(uint32_t seed, bytebuffer *buffer,
							size_t size);

		/** Scales "number" to be between "lower" and "upper" and
		 *  returns it. */
		static	int32_t	scale(uint32_t number, int32_t lower,
								int32_t upper);

		/** Returns the largest number generate(uint32_t*) could
		 *  produce.  Unlike prng::getRandMax(), this is not
		 *  backend-dependent - generate(uint32_t*) always spans the
		 *  full 0..2^32-1 range, so this always returns 4294967295U. */
		static	uint32_t	getRandMax();

		/** Returns true if this class needs a mutex to operate safely
		 *  in a threaded environment and false otherwise.
		 *
		 *  The cryptographically secure backends hold no shared
		 *  mutable state - openssl's RAND_bytes() is thread safe, and
		 *  CryptGenRandom() and /dev/urandom are each reached through
		 *  a per-instance handle or device - so when one of those is
		 *  in use, this returns false.
		 *
		 *  When neither openssl nor CryptGenRandom is compiled in and
		 *  /dev/urandom cannot be opened, csprng falls back to prng,
		 *  whose backends sometimes share process-wide state.  In that
		 *  case this returns prng::getNeedsMutex(). */
		static	bool	getNeedsMutex();

		/** Allows you to supply a mutex if the class needs it
		 *  (see getNeedsMutex()).  If your application is not
		 *  multithreaded, then there is no need to supply a mutex.
		 *
		 *  "mtx" is ignored unless csprng is using the prng fallback,
		 *  in which case it is passed along to prng::setMutex(). */
		static	void	setMutex(threadmutex *mtx);

		/** Returns true if this platform supports csprng and false
		 *  otherwise.  Since a fallback implementation is built in,
		 *  this method always returns true.
		 *
		 *  csprng always produces output, but that output is only
		 *  cryptographically secure when a secure backend is
		 *  available.  Call isCryptographicallySecure() on an
		 *  instance to find out whether it is. */
		static	bool	isSupported();

	#include <rudiments/private/csprng.h>
};

#endif
