// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DES_H
#define RUDIMENTS_DES_H

#include <rudiments/private/desincludes.h>

/** The des class provides the hash method commonly used on unix and unix-like
 *  platforms for for password encryption. */
class RUDIMENTS_DLLSPEC des : public hash {
	public:
		/** Creates an intance of the des class. */
		des();

		/** Deletes this instance of the des class. */
		~des();

		/** Returns the number of bytes in the salt (2). */
		size_t	getSaltSize();

		/** Returns the current hash on success or NULL if an error
		 *  occurred.
		 *
		 *  Data is hashed using the supplied salt, which should be a
		 *  2 character string from the set [a-zA-Z0-9./].
		 *
		 *  Note that since des is typically used to encrypt passwords,
		 *  and that the data that results from this type of hash is
		 *  always printable ascii, this method NULL-terminates the
		 *  hash, and the result can be interpreted as a
		 *  NULL-terminated string.
		 *
		 *  Note that the current hash will be an empty
		 *  string if no data has been appended yet. */
		const byte_t	*getHash();

		/** Returns the number of bytes in the hash, including the
		 *  NULL-terminator.
		 *
		 *  Note that since the resulting hash is always printable
		 *  ascii, this size is equivalent to the string length,
		 *  plus 1. */
		uint64_t	getHashSize();

		/** If your system doesn't support a reentrany crypt function
		 *  then this class needs a mutex to assure thread safety.
		 * 
		 *  This method returns true if this class needs a mutex
		 *  to operate safely in a threaded environment and false
		 *  otherwise. */
		bool	getNeedsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  If your application is not multithreaded, then
		 *  there is no need to supply a mutex. */
		void	setMutex(threadmutex *mtx);

		/** Returns true if this platform supports des and false
		 *  otherwise. */
		bool	isSupported();

	#include <rudiments/private/des.h>
};

#endif
