// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_AES128_H
#define RUDIMENTS_AES128_H

#include <rudiments/private/aes128includes.h>

/** The aes128 class provides a simple aes128 implementation. */
class RUDIMENTS_DLLSPEC aes128 : public encryption {
	public:
		/** Creates an instance of the aes128 class. */
		aes128();

		/** Deletes this instance of the aes128 class. */
		virtual	~aes128();

		/** Interprets the current data as unencrypted.  Encrypts the
		 *  current data.  Returns the encrypted data on success or
		 *  NULL if an error occurred.  Note that the encrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		const unsigned char	*getEncryptedData();

		/** Interprets the current data as encrypted.  Decrypts the
		 *  current data.  Returns the decrypted data on success or
		 *  NULL if an error occurred.  Note that the decrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		const unsigned char	*getDecryptedData();

		/** Clears the current encrypted data and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		bool	clear();

	#include <rudiments/private/aes128.h>
};

#endif
