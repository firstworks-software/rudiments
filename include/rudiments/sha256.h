// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SHA256_H
#define RUDIMENTS_SHA256_H

#include <rudiments/private/sha256includes.h>

/** The sha256 class provides a simple sha256 implementation. */
class RUDIMENTS_DLLSPEC sha256 : public hash {
	public:
		/** Creates an instance of the sha256 class. */
		sha256();

		/** Deletes this instance of the sha256 class. */
		virtual	~sha256();

		/** Appends "length" bytes of "data" to the data to be
		 *  hashed.  Returns true on success or false if an error
		 *  occurred. */
		bool	append(const unsigned char *data, uint32_t length);

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		const unsigned char	*getHash();

		/** Returns the length of the hash in bytes. */
		uint64_t	getHashLength();

		/** Clears the current hash and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		bool	clear();

		/** Returns the most recent error. */
		hasherror_t	getError();

	#include <rudiments/private/sha256.h>
};

#endif
