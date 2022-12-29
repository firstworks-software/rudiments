// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INPUT_H
#define RUDIMENTS_INPUT_H

#include <rudiments/private/inputincludes.h>

class RUDIMENTS_DLLSPEC input : virtual public object {
	public:
		input();
		virtual	~input();

		virtual	ssize_t	read(byte_t *buffer, size_t size)=0;
		virtual	ssize_t	read(char *buffer, size_t length)=0;
		virtual	ssize_t	read(char *character)=0;
		virtual	ssize_t	read(wchar_t *buffer, size_t length)=0;
		virtual	ssize_t	read(wchar_t *character)=0;
		virtual	ssize_t	read(char16_t *buffer, size_t length)=0;
		virtual	ssize_t	read(char16_t *character)=0;
		virtual	ssize_t	read(int16_t *number)=0;
		virtual	ssize_t	read(int32_t *number)=0;
		virtual	ssize_t	read(int64_t *number)=0;
		virtual	ssize_t	read(byte_t *character)=0;
		virtual	ssize_t	read(uint16_t *number)=0;
		virtual	ssize_t	read(uint32_t *number)=0;
		virtual	ssize_t	read(uint64_t *number)=0;
		virtual	ssize_t	read(float *number)=0;
		virtual	ssize_t	read(double *number)=0;

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

		virtual	ssize_t	read(byte_t *buffer, size_t size,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char *buffer, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(wchar_t *buffer, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(wchar_t *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char16_t *buffer, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(char16_t *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int16_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int32_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(int64_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(byte_t *character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint16_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint32_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(uint64_t *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(float *number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	read(double *number,
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
