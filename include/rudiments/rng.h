// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_RNG_H
#define RUDIMENTS_RNG_H

#include <rudiments/private/rngincludes.h>

/** The rng class defines an interface for generating random numbers.
 *
 *  It says nothing about whether an implementation is cryptographically
 *  secure or reproducible from a seed - see prng and csprng for two very
 *  different implementations. */
class RUDIMENTS_DLLSPEC rng : virtual public object {
	public:

		/** Creates an instance of the rng class. */
		rng();

		/** Deletes this instance of the rng class. */
		virtual	~rng();

		/** Sets the initial seed for this instance. */
		virtual bool	setSeed(uint32_t seed)=0;

		/** Generates a random number and sets "result" to this
		 *  number.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generate(uint32_t *result)=0;

		/** Generates a random number between "lower" and "upper"
		 *  and sets "result" to this number.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generate(int32_t *result, int32_t lower,
							int32_t upper)=0;

		/** Allocates a new buffer of "size" bytes, fills it with
		 *  random bytes and returns it.  The caller is responsible
		 *  for calling delete[] on the returned buffer.
		 *
		 *  Returns NULL on failure. */
		virtual byte_t	*generateBytes(size_t size)=0;

		/** Fills the first "size" bytes of "buffer" (which is
		 *  "buffersize" bytes long) with random bytes.
		 *
		 *  Returns false, without modifying "buffer", if "size" is
		 *  greater than "buffersize".  Returns true on success and
		 *  false on failure. */
		virtual bool	generateBytes(byte_t *buffer, size_t buffersize,
							size_t size)=0;

		/** Fills "buffer", which is "buffersize" bytes long, entirely
		 *  with random bytes.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generateBytes(byte_t *buffer, size_t buffersize)=0;

		/** Appends "size" random bytes to "buffer".
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	generateBytes(bytebuffer *buffer, size_t size)=0;

	#include <rudiments/private/rng.h>
};

#endif
