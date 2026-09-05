// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SINGLEDES_H
#define RUDIMENTS_SINGLEDES_H

#include <rudiments/private/singledesincludes.h>

/** The singledes class provides a simple single DES implementation. */
class RUDIMENTS_DLLSPEC singledes : public encryption {
	public:
		/** Creates an instance of the singledes class. */
		singledes();

		/** Deletes this instance of the singledes class. */
		~singledes();

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

		/** Returns true if the block cipher mode specified by
		 *  setBlockCipherMode() (or the default of CBC) is supported
		 *  and false otherwise. */
		bool	isSupported();

	#include <rudiments/private/singledes.h>
};

#endif
