// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_PBKDF2_H
#define RUDIMENTS_PBKDF2_H

#include <rudiments/private/pbkdf2includes.h>

/** algorithms */
enum pbkdf2algorithm_t {
	PBKDF2_ALGORITHM_SHA1=0,
	PBKDF2_ALGORITHM_SHA256,
	PBKDF2_ALGORITHM_SHA512
};

/** The pbkdf2 class provides a simple PBKDF2 (Password-Based Key Derivation
 *  Function 2) implementation. */
class RUDIMENTS_DLLSPEC pbkdf2 : public hash {
	public:
		/** Creates an instance of the pbkdf2 class. */
		pbkdf2();

		/** Deletes this instance of the pbkdf2 class. */
		virtual	~pbkdf2();

		/** Sets the number of times to apply the hash algorithm to
		 *  "iterations". */
		void	setIterations(uint32_t iterations);

		/** Returns the number of times to apply the hash algorithm
		 *  set by a previous call to setIterations() or 10000 (the
		 *  default) if no number of iterations has been set. */
		uint32_t	getIterations();

		/** Sets the hash algorithm to use to "alg". */
		void	setAlgorithm(pbkdf2algorithm_t alg);

		/** Returns the algorithm set by a previous call to
		 *  setAlgorithm() or PBKDF2_ALGORITHM_SHA256 (the default)
		 *  if no algorithm has been set. */
		pbkdf2algorithm_t getAlgorithm();

		/** Sets the size of the key to generate to "keysize". */
		void	setKeySize(uint32_t keysize);

		/** Returns the size of the key to generate as set by a
		 *  previous call to setKeySize() or 0 if no key size has
		 *  been set. */
		uint32_t	getKeySize();

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		const byte_t	*getHash();

		/** Clears the current hash, and any data that had previously
		 *  been appended, and sets the salt and the number of
		 *  iterations to defaults, and the algorithm.  Returns true on
		 *  success or false if an error occurred. */
		bool	reset();

		/** Returns true if this platform supports pbkdf2 and false
		 *  otherwise. */
		bool	isSupported();

	#include <rudiments/private/pbkdf2.h>
};

#endif
