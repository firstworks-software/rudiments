// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_HASH_H
#define RUDIMENTS_HASH_H

#include <rudiments/private/hashincludes.h>

/** hash errors */
enum hasherror_t {
	HASH_ERROR_SUCCESS=0,
	HASH_ERROR_NULL,
	HASH_ERROR_INVALID_SALT_SIZE,
	HASH_ERROR_INPUT_TOO_LONG,
	HASH_ERROR_STATE_ERROR,
	HASH_ERROR_UNSUPPORTED
};

/** The hash class defines an interface for hashing. */
class RUDIMENTS_DLLSPEC hash : virtual public object {
	public:
		/** Creates an instance of the hash class. */
		hash();

		/** Deletes this instance of the hash class. */
		virtual	~hash();

		/** Sets the salt. */
		virtual bool	setSalt(const byte_t *salt, size_t size);

		/** Returns the salt set by a previous call to setSalt() or
		 *  NULL if no salt has been set. */
		virtual const byte_t	*getSalt();

		/** Returns the number of bytes in the salt. */
		virtual size_t	getSaltSize();

		/** Appends "size" bytes of "data" to the data to be
		 *  hashed.  Returns true on success or false if an error
		 *  occurred. */
		virtual	bool	append(const byte_t *data, uint32_t size);

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		virtual	const byte_t	*getHash()=0;

		/** Returns the number of bytes in the hash. */
		virtual	uint64_t	getHashSize();

		/** Returns the most recent error. */
		virtual	hasherror_t	getError();

		/** Clears the current hash and any data that had
 		 *  previously been appended.
 		 *
 		 *  Does not clear the salt.
 		 *
 		 *  This implementation always returns true, but a child class
 		 *  may override this method to return false if something
 		 *  fails, and possibly set an error. */
		virtual	bool	clear();

		/** Clears the current hash and any data that had
 		 *  previously been appended, and the salt.
 		 *
 		 *  This implementation always returns true, but a child class
 		 *  may override this method to return false if something
 		 *  fails, and possibly set an error. */
		virtual	bool	reset();

	#include <rudiments/private/hash.h>
};

#endif
