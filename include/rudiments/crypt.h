// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_CRYPT_H
#define RUDIMENTS_CRYPT_H

#include <rudiments/private/cryptincludes.h>

/** The crypt class provides a static encryption method that is commonly used
 *  for password encryption. */
class RUDIMENTS_DLLSPEC crypt : public encryption {
	public:
		/** Creates an intance of the crypt class. */
		crypt();

		/** Deletes this instance of the crypt class. */
		~crypt();

		/** Returns the number of bytes in the initialization
		 *  vector (salt) - 2 bytes. */
		uint32_t	getIvSize();

		/** Interprets the current data as unencrypted.  Encrypts
		 *  current data using the des algorithm, and the initilization
		 *  vector as the "salt" (which should be a 2 character string
		 *  from the set [a-zA-Z0-9./].  Returns the encrypted data on
		 *  success or NULL if an error occurred.  Note that the
		 *  encrypted data returned will be an empty string if no data
		 *  has been appended yet. */
		const unsigned char	*getEncryptedData();

		/** If your system doesn't support crypt_r() then this
		 *  class needs a mutex to assure thread safety.
		 * 
		 *  This method returns true if this class needs a mutex
		 *  to operate safely in a threaded environment and false
		 *  otherwise. */
		bool	needsMutex();

		/** Allows you to supply a mutex is the class needs it.
		 *  If your application is not multithreaded, then
		 *  there is no need to supply a mutex. */
		void	setMutex(threadmutex *mtx);

	#include <rudiments/private/crypt.h>
};

#endif
