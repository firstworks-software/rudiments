// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_ICONVERT_H
#define RUDIMENTS_ICONVERT_H

#include <rudiments/private/iconvertincludes.h>

/** The iconvert class implements character encoding converter. */
class RUDIMENTS_DLLSPEC iconvert : public object {
	public:

		/** Creates a new instance of the iconvert class. */
		iconvert();

		/** Crates an instance of the iconvert class with the same
		 *  from and to encodings as "i". */
		iconvert(iconvert &i);

		/** Closes this instance of the inconvert class and sets its
		 *  from and to encodings to the same as "i". */
		iconvert	&operator=(iconvert &i);

		/** Deletes this instance of the iconvert class. */
		virtual	~iconvert();

		/** Sets the "from encoding" of the instance.  This is the
		 *  encoding that characters in the "from buffer" are expected
		 *  to have. */
		void	setFromEncoding(const char *fromencoding);

		/** Returns the "from encoding" of the instance as set by
		 *  a previous call to setFromEncoding(), the copy constructor,
		 *  or = operator. */
		const char	*getFromEncoding();

		/** Sets the "to encoding" of the instance.  This is the
		 *  encoding that characters in the "to buffer" are expected
		 *  to have. */
		void	setToEncoding(const char *fromencoding);

		/** Returns the "to encoding" of the instance as set by
		 *  a previous call to setToEncoding(), the copy constructor,
		 *  or = operator. */
		const char	*getToEncoding();

		/** Sets the "from buffer" - the buffer that characters will
		 *  be read from during the call to convert().  Specifically,
		 *  "buffer" should be a pointer to the first byte that will
		 *  be read when convert() is called. */
		void	setFromBuffer(const byte_t *buffer);

		/** Returns the "from buffer" as previously set by
		 *  setFromBuffer(), or as set by a call to convert(). */
		const byte_t	*getFromBuffer();

		/** Sets the number of bytes in the "from buffer".  The call
		 *  to convert() won't attempt to process more bytes than
		 *  this. */
		void	setFromBufferSize(size_t buffersize);

		/** Returns the number of bytes in the "from buffer", as set
		 *  by a previous call to setFromBufferSize(). */
		size_t	getFromBufferSize();

		/** Sets the "to buffer" - the buffer that characters will be
		 *  written to during a call to convert(). */
		void	setToBuffer(byte_t *buffer);

		/** Returns the "to buffer" as set by a previous call to
		 *  setToBuffer(). */
		byte_t	*getToBuffer();

		/** Sets the number of bytes in the "to buffer".  The call
		 *  to convert() won't attempt to write more bytes than this. */
		void	setToBufferSize(size_t buffersize);

		/** Returns the number of bytes in the "to buffer" as set
		 *  by a previous call to setToBufferSize(). */
		size_t	getToBufferSize();

		/** Processes characters from the "from buffer", converting
		 *  them from the "from encoding" to the "to encoding" and
		 *  writing the converted characters to the "to buffer".
		 *
		 *  Sets the "from buffer position" to a pointer to the first
		 *  byte of the "from buffer" after the last character that was
		 *  processed.  Sets the "from buffer remaining" to the number
		 *  of bytes in the "from buffer" that were not processed.
		 *
		 *  Sets the "to buffer position" to a pointer to the first
		 *  byte in the "to buffer" after the last character that was
		 *  written.  Sets the "to buffer remaining" to the number of
		 *  bytes in the "to buffer" that were not written to.
		 *
		 *  Returns true on successful conversion.
		 *
		 *  Returns false if one of the following errors occur.
		 *
		 *  If conversion from the "from encoding" to the "to encoding"
		 *  isn't supported then the error number will be set to ENOSYS.
		 *
		 *  If the "to buffer size" isn't sufficient to store the
		 *  converted characters (eg. because we're converting to a
		 *  multi-byte encoding and there aren't enough bytes to store
		 *  the multi-byte character) then the error number will be set
		 *  to E2BIG.
		 *
		 *  If an invalid multibyte sequence was encountered in the 
		 *  "from buffer" then the error number will be set to EILSEQ.
		 *
		 *  If an incomplete multibyte sequence was encountered in the
		 *  "from buffer" then the error number will be set to EINVAL.
		 *
		 *  In each of these error cases, the "from buffer position"
		 *  will be set to the "from buffer" and the "from buffer
		 *  remaining" will be set to the "from buffer size".  The "to
		 *  buffer position" will be set to the "to buffer" and the "to
		 *  buffer remaining" will be set to the "to buffer size". */
		bool	convert();

		/** Returns the "from buffer position" - the position in the
		 *  "from buffer" set by a previous call to convert(). */
		const byte_t	*getFromBufferPosition();

		/** Returns the "from buffer remaining" - the number of bytes
		 *  remaining in "from buffer", as set by a previous call to
		 *  convert(). */
		size_t	getFromBufferRemaining();

		/** Returns the "to buffer position" - the position in the
		 *  "to buffer" set by a previous call to convert(). */
		const byte_t	*getToBufferPosition();

		/** Returns the "to buffer remaining" - the number of bytes
		 *  remaining in "to buffer", as set by a previous call to
		 *  convert(). */
		size_t	getToBufferRemaining();

		/** Returns the maximum number of bytes that a multibyte
		 *  character can contain.
		 *
		 *  Can be used, for example, when sizing the "to buffer", when
		 *  converting to a multibyte encoding. */
		static	uint16_t	getMaxMultiByteSize();

		/** Returns true if convert() needs to be protected with a
		 *  mutex (eg. because it might need to use non-reentrant
		 *  functions) or false if it does not (eg. because it only
		 *  uses reentrant functions). */
		static	bool	getNeedsMutex();

	#include <rudiments/private/iconvert.h>
};

#endif
