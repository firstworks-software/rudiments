// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_AES256_H
#define RUDIMENTS_AES256_H

#include <rudiments/private/aes256includes.h>

/** The aes256 class provides a simple aes256 implementation. */
class RUDIMENTS_DLLSPEC aes256 : public encryption {
	public:
		/** Creates an instance of the aes256 class. */
		aes256();

		/** Deletes this instance of the aes256 class. */
		~aes256();

		/** Returns the number of bytes in the key. */
		size_t	getKeySize();

		/** Returns the number of bytes in the initialization vector. */
		size_t	getIvSize();

		/** Interprets the current data as unencrypted.  Encrypts the
		 *  current data.  Returns the encrypted data on success or
		 *  NULL if an error occurred.  Note that the encrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		const byte_t	*getEncryptedData();

		/** Interprets the current data as encrypted.  Decrypts the
		 *  current data.  Returns the decrypted data on success or
		 *  NULL if an error occurred.  Note that the decrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		const byte_t	*getDecryptedData();

		/** Returns true if the platforms supports aes256, with the
		 *  block cipher mode specified by setBlockCipherMode() (or the
		 *  default of CBC) and false otherwise. */
		bool	isSupported();

	#include <rudiments/private/aes256.h>
};

#endif
