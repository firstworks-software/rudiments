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
		encryption() {};

		/** Deletes this instance of the encryption class. */
		virtual	~encryption() {};

		/** Appends "length" bytes of "data" to the data to be
		 *  encrypted.  Returns true on success or false if an error
		 *  occurred. */
		virtual	bool	append(const unsigned char *data,
							uint32_t length)=0;

		/** Returns the current encrypted on success or NULL if an error
		 *  occurred.  Note that the current encryption will be an empty
		 *  string if no data has been appended yet. */
		virtual	const unsigned char	*getEncryptedData()=0;

		/** Returns the length of the encrypted data in bytes. */
		virtual	uint64_t	getEncryptedDataLength()=0;

		/** Clears the current encrypted data and any data that had
 		 *  previously been appended.  Returns true on success or
 		 *  false if an error occurred. */
		virtual	bool	clear()=0;

		/** Returns the most recent error. */
		virtual	encryptionerror_t	getError()=0;
};

#endif
