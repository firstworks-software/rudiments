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

		void	setKey(const unsigned char *key);
		unsigned char	*getKey();
		uint32_t	getKeySize();

		void	setIv(const unsigned char *iv);
		unsigned char	*getIv();
		uint32_t	getIvSize();

		/** Appends "length" bytes of "data" to the data to be
		 *  encrypted.  Returns true on success or false if an error
		 *  occurred. */
		bool	append(const unsigned char *data, uint32_t length);

		/** Returns the current encrypted data on success or NULL if an
		 *  error occurred.  Note that the current encrypted data will
		 *  be an empty string if no data has been appended yet. */
		const unsigned char	*getEncryptedData();

		/** Returns the length of the encrypted data in bytes. */
		uint64_t	getEncryptedDataLength();

		/** Clears the current encrypted data and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		bool	clear();

		/** Returns the most recent error. */
		encryptionerror_t	getError();

	#include <rudiments/private/aes128.h>
};

#endif
