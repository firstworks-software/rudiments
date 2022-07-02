// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OUTPUT_H
#define RUDIMENTS_OUTPUT_H

#include <rudiments/private/outputincludes.h>

class RUDIMENTS_DLLSPEC output : virtual public object {
	public:
		virtual ~output();

		virtual	ssize_t	write(const unsigned char *string,
							size_t size)=0;
		virtual	ssize_t	write(const char *string)=0;
		virtual	ssize_t	write(const char *string, size_t length)=0;
		virtual	ssize_t	write(char character)=0;
		virtual	ssize_t	write(const wchar_t *string)=0;
		virtual	ssize_t	write(const wchar_t *string, size_t length)=0;
		virtual	ssize_t	write(wchar_t character)=0;
		virtual	ssize_t	write(int16_t number)=0;
		virtual	ssize_t	write(int32_t number)=0;
		virtual	ssize_t	write(int64_t number)=0;
		virtual	ssize_t	write(unsigned char character)=0;
		virtual	ssize_t	write(uint16_t number)=0;
		virtual	ssize_t	write(uint32_t number)=0;
		virtual	ssize_t	write(uint64_t number)=0;
		virtual	ssize_t	write(float number)=0;
		virtual	ssize_t	write(double number)=0;

		virtual	ssize_t	write(const unsigned char *string, size_t size,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const char *string,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const char *string, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(char character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const wchar_t *string,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(const wchar_t *string, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(wchar_t character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int16_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int32_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int64_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(unsigned char character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint16_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint32_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(uint64_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(float number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(double number,
						int32_t sec, int32_t usec);

		/** Writes "..." using "format" which should comply with
		 *  standard printf formatting rules.
		 *
		 *  Returns the number of bytes written or -1 if an error
		 *  occurred. */
		virtual	ssize_t	printf(const char *format, ...);

		/** Writes "argp" using "format" which should comply with
		 *  standard printf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of bytes written. */
		virtual	ssize_t	printf(const char *format, va_list *argp);

		/** Writes "..." using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Returns the number of bytes written or -1 if an error
		 *  occurred. */
		virtual	ssize_t	printf(const wchar_t *format, ...);

		/** Writes "argp" using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of bytes written. */
		virtual	ssize_t	printf(const wchar_t *format, va_list *argp);

	#include <rudiments/private/output.h>
};

#include <rudiments/private/outputinlines.h>

#endif
