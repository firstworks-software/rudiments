// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INPUT_H
#define RUDIMENTS_INPUT_H

#include <rudiments/private/inputincludes.h>

/** The input class provides a base class for things from which input can be
 *  taken (file descriptors, files, sockets, etc.) */
class RUDIMENTS_DLLSPEC input : virtual public object {
	public:

		/** Creates an instance of the input class. */
		input();

		/** Deletes this instance of the input class. */
		virtual	~input();


		/** Reads "size" bytes from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(byte_t *buffer, size_t size)=0;

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(char *buffer, size_t length)=0;

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(char *buffer)=0;

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(wchar_t *buffer, size_t length)=0;

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(wchar_t *buffer)=0;

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	readUcs2(ucs2_t *buffer, size_t length)=0;

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	readUcs2(ucs2_t *buffer)=0;

		/** Reads a 16-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(int16_t *buffer)=0;

		/** Reads a 32-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(int32_t *buffer)=0;

		/** Reads a 64-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(int64_t *buffer)=0;

		/** Reads a byte from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	read(byte_t *buffer)=0;

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		virtual	ssize_t	read(uint16_t *buffer)=0;

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		virtual	ssize_t	read(uint32_t *buffer)=0;

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		virtual	ssize_t	read(uint64_t *buffer)=0;

		/** Reads a floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		virtual	ssize_t	read(float *buffer)=0;

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		virtual	ssize_t	read(double *buffer)=0;

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char **buffer, const char *terminator);

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read, RESULT_ERROR if
		 *  an error occurred or RESULT_MAX if maxbytes were read
		 *  before the terminator was encountered.
		 *  
		 *  Setting maxbytes to 0 disables it. */
		ssize_t	read(char **buffer,
				const char *terminator, size_t maxbytes);

		/** Reads "size" bytes from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(byte_t *buffer, size_t size,
						int32_t sec, int32_t usec);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(char *buffer, size_t length,
						int32_t sec, int32_t usec);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(char *buffer,
						int32_t sec, int32_t usec);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(wchar_t *buffer, size_t length,
						int32_t sec, int32_t usec);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(wchar_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	readUcs2(ucs2_t *buffer, size_t length,
						int32_t sec, int32_t usec);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	readUcs2(ucs2_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads a 16-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(int16_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads a 32-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(int32_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads a 64-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(int64_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads a byte from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(byte_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(uint16_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(uint32_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(uint64_t *buffer,
						int32_t sec, int32_t usec);

		/** Reads a floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(float *buffer,
						int32_t sec, int32_t usec);

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred.
		 *
		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	read(double *buffer,
						int32_t sec, int32_t usec);

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered with a timeout of "sec" seconds
		 *  and "usec" microseconds.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char **buffer, const char *terminator,
					int32_t sec, int32_t usec);

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered with a timeout of "sec" seconds
		 *  and "usec" microseconds.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Returns the number of bytes that were read, RESULT_ERROR if
		 *  an error occurred or RESULT_MAX if maxbytes were read
		 *  before the terminator was encountered.
		 *  
		 *  Setting maxbytes to 0 disables it. */
		ssize_t	read(char **buffer,
				const char *terminator, size_t maxbytes,
				int32_t sec, int32_t usec);

		/** Reads from the file desciptor into "buffer" until
		 *  "terminator" is encountered, using "escapechar" as an
		 *  escape character, with a timeout of "sec" seconds
		 *  and "usec" microseconds.
		 *
		 *  Note that "buffer" is allocated internally and must be freed
		 *  by the calling program.
		 *
		 *  Setting "escapechar" to '\0' disables it.
		 *
		 *  Returns the number of bytes that were read, RESULT_ERROR if
		 *  an error occurred or RESULT_MAX if maxbytes were read
		 *  before the terminator was encountered.
		 *  
		 *  Setting maxbytes to 0 disables it. */
		ssize_t	read(char **buffer,
				const char *terminator, size_t maxbytes,
				char escapechar, int32_t sec, int32_t usec);

	#include <rudiments/private/input.h>
};

#include <rudiments/private/inputinlines.h>

#endif
