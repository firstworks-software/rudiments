// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MD5_H
#define RUDIMENTS_MD5_H

#include <rudiments/private/md5includes.h>

/** The md5 class provides a simple md5 implementation. */
class RUDIMENTS_DLLSPEC md5 : public hash {
	public:
		/** Creates an instance of the md5 class. */
		md5();

		/** Deletes this instance of the md5 class. */
		virtual	~md5();

		/** Appends "size" bytes of "data" to the data to be
		 *  hashed.  Returns true on success or false if an error
		 *  occurred. */
		bool	append(const byte_t *data, uint32_t size);

		/** Returns the current hash on success or NULL if an error
		 *  occurred.  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		const byte_t	*getHash();

		/** Returns the number of bytes in the hash. */
		uint64_t	getHashSize();

		/** Clears the current hash and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		bool	clear();

		/** Returns true if this platform supports md5 and false
		 *  otherwise. */
		bool	isSupported();

	#include <rudiments/private/md5.h>
};

#endif
