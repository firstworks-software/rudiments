// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ENCRYPTION_H
#define RUDIMENTS_ENCRYPTION_H

#include <rudiments/private/encryptionincludes.h>

/** encryption errors */
enum encryptionerror_t {
	ENCRYPTION_ERROR_SUCCESS=0,
	ENCRYPTION_ERROR_NULL,
	ENCRYPTION_ERROR_INVALID_KEY_SIZE,
	ENCRYPTION_ERROR_INVALID_IV_SIZE,
	ENCRYPTION_ERROR_INPUT_TOO_LONG,
	ENCRYPTION_ERROR_INVALID_PADDING,
	ENCRYPTION_ERROR_UNSUPPORTED
};

/** block cipher modes */
enum blockciphermode_t {
	BLOCK_CIPHER_MODE_CBC=0,
	BLOCK_CIPHER_MODE_CFB1,
	BLOCK_CIPHER_MODE_CFB8,
	BLOCK_CIPHER_MODE_CFB128,
	BLOCK_CIPHER_MODE_ECB,
	BLOCK_CIPHER_MODE_OFB,
	BLOCK_CIPHER_MODE_GCM,
	BLOCK_CIPHER_MODE_CCM,
	BLOCK_CIPHER_MODE_CTR
};

/** The encryption class provides a simple encryption interface. */
class RUDIMENTS_DLLSPEC encryption : virtual public object {
	public:
		/** Creates an instance of the encryption class. */
		encryption();

		/** Deletes this instance of the encryption class. */
		virtual	~encryption();

		/** Sets the key used for encryption/decryption to the first
		 *  "keysize" bytes of "key".
		 *
		 *  Returns true on success and false if an error occurred.
		 *
		 *  The most common error is that "keysize" is not the
		 *  correct number of bytes.  The method getKeySize() returns
		 *  the correct number of bytes for the key. */
		virtual bool	setKey(const byte_t *key, size_t keysize);

		/** Sets the key used for encryption/decryption to random
		 *  data.  The data used can be retrived using getKey(). */
		virtual	void	setRandomKey();

		/** Returns the key currently used for encryption/decryption.
		 *  If no key has been set, then this will be a block of 0's. */
		virtual byte_t	*getKey();

		/** Returns the number of bytes in the key. */
		virtual size_t	getKeySize()=0;

		/** Sets the initialization vector used for
		 *  encryption/decryption to the first "ivsize" bytes of "iv".
		 *
		 *  Returns true on success and false if an error occurred.
		 *
		 *  The most common error is that "ivsize" is not the correct
		 *  number of bytes.  The method getKeySize() returns the
		 *  correct number of bytes for the key. */
		virtual bool	setIv(const byte_t *iv, size_t ivsize);

		/** Sets the initialization vector used encryption/decryption
		 *  to random data.  The data used can be retrived using
		 *  getKey(). */
		virtual	void	setRandomIv();

		/** Returns the initialization vector currently used for
		 *  encryption/decryption.  If no initialization vector has
		 *  been set, then this will be a block of 0's. */
		virtual byte_t	*getIv();

		/** Returns the number of bytes in the initialization vector. */
		virtual size_t	getIvSize()=0;

		/** Sets the block cipher mode to "mode".  Defaults to
		 *  BLOCK_CIPHER_MODE_CBC. */
		virtual void	setBlockCipherMode(blockciphermode_t mode);

		/** Returns the current block cipher mode. */
		virtual blockciphermode_t	getBlockCipherMode();

		/** Appends "size" bytes of "data" to the data to be
		 *  encrypted/decrypted.
		 *
		 *  Returns true on success or false if an error occurred. */
		virtual bool	append(const byte_t *data, uint32_t size);

		/** Interprets the current data as unencrypted.  Encrypts the
		 *  current data.
		 *
		 *  Returns the encrypted data on success or NULL if an error
		 *  occurred.
		 *
		 *  Note that the encrypted data returned will be an empty
		 *  string if no data has been appended yet. */
		virtual const byte_t	*getEncryptedData()=0;

		/** Returns the number of bytes of encrypted data. */
		virtual	uint64_t	getEncryptedDataSize();

		/** Interprets the current data as encrypted.  Decrypts the
		 *  current data.
		 *
		 *  Returns the decrypted data on success or NULL if an error
		 *  occurred.
		 *
		 *  Note that the decrypted data returned will be an empty
		 *  string if no data has been appended yet. */
		virtual const byte_t	*getDecryptedData()=0;

		/** Returns the number of bytes of decrypted data. */
		virtual	uint64_t	getDecryptedDataSize();

		/** Returns the most recent error. */
		virtual	encryptionerror_t	getError();

		/** Clears the current encrypted/decrypted data and any data
		 *  that had been appended.
		 *
		 *  Does not clear the key or initialization vector.
		 *
		 *  This implementation always returns true, but a child class
		 *  may override this method to return false if something
		 *  fails, and possibly set an error. */
		virtual	bool	clear();

		/** Clears the current encrypted/decrypted data, any data
		 *  that had been appended, any current error, key, and
		 *  initialization vector.
		 *
		 *  This implementation always returns true, but a child class
		 *  may override this method to return false if something
		 *  fails, and possibly set an error. */
		virtual	bool	reset();

		/** Returns true if the platforms supports this encryption type
		 *  and false otherwise. */
		virtual bool	isSupported()=0;
	
	#include <rudiments/private/encryption.h>
};

#endif
