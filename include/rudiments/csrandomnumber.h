// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CSRANDOMNUMBER_H
#define RUDIMENTS_CSRANDOMNUMBER_H

#include <rudiments/private/csrandomnumberincludes.h>

/** The csrandomnumber class provides methods for generating cryptographically
 *  secure random numbers.
 *
 *  Unlike randomnumber, csrandomnumber offers no way to seed or otherwise
 *  reproduce its output.  A seeded, deterministic sequence is exactly what
 *  "cryptographically secure" rules out, so there is no setSeed(),
 *  seed-driven generate(), or getSeed() here.  Use randomnumber instead if
 *  you need a reproducible sequence (eg. for scaling, jitter, or test
 *  fixtures).
 *
 *  csrandomnumber is intended for secrets - keys, tokens, salts, nonces,
 *  session ids, and anything else that must be unpredictable even to
 *  someone who can observe other output from the same process.
 *
 *  csrandomnumber uses openssl's RAND_bytes() when available, the Windows
 *  CryptGenRandom() API when building for Windows, and /dev/urandom
 *  otherwise. */
class RUDIMENTS_DLLSPEC csrandomnumber : public object {
	public:

		/** Creates a new instance of the csrandomnumber class. */
		csrandomnumber();

		/** Deletes this instance of the csrandomnumber class. */
		~csrandomnumber();

		/** Generates a cryptographically secure random number between
		 *  0 and 2^32-1 and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		bool	generate(uint32_t *result);

		/** Generates a cryptographically secure random number between
		 *  "lower" and "upper" and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		bool	generate(int32_t *result, int32_t lower, int32_t upper);

		/** Allocates a new buffer of "size" bytes, fills it with
		 *  cryptographically secure random bytes and returns it.
		 *  The caller is responsible for calling delete[] on the
		 *  returned buffer.
		 *
		 *  Returns NULL on failure. */
		byte_t	*generateBytes(size_t size);

		/** Fills the first "size" bytes of "buffer" (which is
		 *  "buffersize" bytes long) with cryptographically secure
		 *  random bytes.
		 *
		 *  Returns false, without modifying "buffer", if "size" is
		 *  greater than "buffersize".  Returns true on success and
		 *  false on failure. */
		bool	generateBytes(byte_t *buffer, size_t buffersize,
							size_t size);

		/** Fills "buffer", which is "buffersize" bytes long, entirely
		 *  with cryptographically secure random bytes.
		 *
		 *  Returns true on success and false on failure. */
		bool	generateBytes(byte_t *buffer, size_t buffersize);

		/** Appends "size" cryptographically secure random bytes to
		 *  "buffer".
		 *
		 *  Returns true on success and false on failure. */
		bool	generateBytes(bytebuffer *buffer, size_t size);

		// There is no static generateBytes().  Unlike static
		// generate() below, a static generateBytes() would need the
		// same parameter list as the instance overloads above, since
		// there's no seed to distinguish them by, and C++ doesn't
		// allow that.  Construct a short-lived csrandomnumber instead.

		/** Generates a cryptographically secure random number between
		 *  0 and 2^32-1 and returns it, or 0 on failure.
		 *
		 *  Unlike randomnumber::generate(seed), this is not seeded and
		 *  not reproducible - each call is an independent
		 *  cryptographically secure draw.  Each call also opens and
		 *  closes the underlying source, so code making many draws in
		 *  a loop is better off using an instance of csrandomnumber
		 *  directly. */
		static	uint32_t	generate();

		/** Generates a cryptographically secure random number between
		 *  "lower" and "upper" and returns it, or 0 on failure.
		 *
		 *  Unlike randomnumber::generate(seed,lower,upper), this is
		 *  not seeded and not reproducible - each call is an
		 *  independent cryptographically secure draw.  Each call also
		 *  opens and closes the underlying source, so code making many
		 *  draws in a loop is better off using an instance of
		 *  csrandomnumber directly. */
		static	int32_t	generate(int32_t lower, int32_t upper);

		/** Scales "number" to be between "lower" and "upper" and
		 *  returns it. */
		static	int32_t	scale(uint32_t number, int32_t lower,
								int32_t upper);

		/** Returns the largest number generate(uint32_t*) could
		 *  produce.  Unlike randomnumber::getRandMax(), this is not
		 *  backend-dependent - generate(uint32_t*) always spans the
		 *  full 0..2^32-1 range, so this always returns 4294967295U. */
		static	uint32_t	getRandMax();

		/** Returns true if a cryptographically secure backend
		 *  (openssl, CryptGenRandom, or /dev/urandom) is available
		 *  and false otherwise. */
		static	bool	isSupported();

	#include <rudiments/private/csrandomnumber.h>
};

#endif
