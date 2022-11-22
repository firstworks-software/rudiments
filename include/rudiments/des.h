// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_DES_H
#define RUDIMENTS_DES_H

#include <rudiments/private/desincludes.h>

/** The des class provides the encryption method commonly used on unix and
 *  unix-like platforms for for password encryption. */
class RUDIMENTS_DLLSPEC des : public encryption {
	public:
		/** Creates an intance of the des class. */
		des();

		/** Deletes this instance of the des class. */
		~des();

		/** Returns the number of bytes in the initialization
		 *  vector (salt) - 2 bytes. */
		uint32_t	getIvSize();

		/** Interprets the current data as unencrypted.  Encrypts
		 *  current data using the des algorithm, and the initilization
		 *  vector as the "salt" (which should be a 2 character string
		 *  from the set [a-zA-Z0-9./].  Returns the encrypted data on
		 *  success or NULL if an error occurred.
		 *
		 *  Note that since des is typically used to encrypt
		 *  passwords, and that the encrypted data that results from
		 *  this type of encryption is always printable ascii, this
		 *  method NULL-terminates the encrypted data, and the result
		 *  can be interpreted as a NULL-terminated string.
		 *
		 *  Note that the encrypted data returned will be an empty
		 *  string if no data has been appended yet. */
		const unsigned char	*getEncryptedData();

		/** Returns the number of bytes of encrypted data, not
		 *  including the NULL-terminator.
		 *
		 *  Note that since the encrypted data that results from this
		 *  type of encryption is always printable ascii, this size is
		 *  equivalent to the string length. */
		uint64_t	getEncryptedDataSize();

		/** If your system doesn't support a reentrany crypt function
		 *  then this class needs a mutex to assure thread safety.
		 * 
		 *  This method returns true if this class needs a mutex
		 *  to operate safely in a threaded environment and false
		 *  otherwise. */
		bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  If your application is not multithreaded, then
		 *  there is no need to supply a mutex. */
		void	setMutex(threadmutex *mtx);

	#include <rudiments/private/des.h>
};

#endif
