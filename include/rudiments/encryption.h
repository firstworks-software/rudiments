// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ENCRYPTION_H
#define RUDIMENTS_ENCRYPTION_H

#include <rudiments/private/encryptionincludes.h>

/** encryption errors */
enum encryptionerror_t {
	ENCRYPTION_ERROR_SUCCESS=0,
	ENCRYPTION_ERROR_NULL,
	ENCRYPTION_ERROR_INPUT_TOO_LONG,
	ENCRYPTION_ERROR_STATE_ERROR,
	ENCRYPTION_ERROR_UNSUPPORTED
};

/** The encryption class provides a simple encryption interface. */
class RUDIMENTS_DLLSPEC encryption {
	public:
		/** Creates an instance of the encryption class. */
		encryption();

		/** Deletes this instance of the encryption class. */
		virtual	~encryption();

		/** Sets the key used for encryption/decryption to the first
		 *  "keysize" bytes of "key".  Returns true on success and
		 *  false on failure.  The most common failure is that "keysize"
		 *  is not the correct number of bytes.  The method getKeySize()
		 *  returns the correct number of bytes for the key. */
		virtual bool	setKey(const unsigned char *key,
							size_t keysize);

		/** Sets the key used for encryption/decryption to random
		 *  data.  The data used can be retrived using getKey(). */
		virtual	void	setRandomKey();

		/** Returns the key currently used for encryption/decryption.
		 *  If no key has been set, then this will be a block of 0's. */
		virtual unsigned char	*getKey();

		/** Returns the number of bytes in the key. */
		virtual uint32_t	getKeySize()=0;

		/** Sets the initialization vector used for
		 *  encryption/decryption to the first "ivsize" bytes of "iv".
		 *  Returns true on success and false on failure.  The most
		 *  common failure is that "ivsize" is not the correct number
		 *  of bytes.  The method getKeySize() returns the correct
		 *  number of bytes for the key. */
		virtual bool	setIv(const unsigned char *iv, size_t ivsize);

		/** Sets the initialization vector used encryption/decryption
		 *  to random data.  The data used can be retrived using
		 *  getKey(). */
		virtual	void	setRandomIv();

		/** Returns the initialization vector currently used for
		 *  encryption/decryption.  If no initialization vector has
		 *  been set, then this will be a block of 0's. */
		virtual unsigned char	*getIv();

		/** Returns the number of bytes in the initialization vector. */
		virtual uint32_t	getIvSize()=0;

		/** Appends "length" bytes of "data" to the data to be
		 *  encrypted/decrypted.  Returns true on success or false if
		 *  an error occurred. */
		virtual bool	append(const unsigned char *data,
							uint32_t length);

		/** Interprets the current data as unencrypted.  Encrypts the
		 *  current data.  Returns the encrypted data on success or
		 *  NULL if an error occurred.  Note that the encrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		virtual const unsigned char	*getEncryptedData()=0;

		/** Interprets the current data as encrypted.  Decrypts the
		 *  current data.  Returns the decrypted data on success or
		 *  NULL if an error occurred.  Note that the decrypted data
		 *  returned will be an empty string if no data has been
		 *  appended yet. */
		virtual const unsigned char	*getDecryptedData()=0;

		/** Returns the length of the encrypted/decrypted data in
		 *  bytes. */
		virtual	uint64_t	getDataLength();

		/** Clears the current encrypted/decrypted data and any data
		 *  that had previously been appended.  Does not clear the 
		 *  key or initialization vector.  Returns true on success or
 		 *  false if an error occurred. */
		virtual	bool	clear();

		/** Returns the most recent error. */
		virtual	encryptionerror_t	getError();
	
	#include <rudiments/private/encryption.h>
};

#endif
