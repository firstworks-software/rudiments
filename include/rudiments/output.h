// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OUTPUT_H
#define RUDIMENTS_OUTPUT_H

#include <rudiments/private/outputincludes.h>

class RUDIMENTS_DLLSPEC output : virtual public object {
	public:
		output();
		virtual ~output();

		virtual off64_t	setPositionRelativeToBeginning(off64_t offset);
		virtual off64_t	setPositionRelativeToCurrent(off64_t offset);
		virtual off64_t	setPositionRelativeToEnd(off64_t offset);

		virtual	ssize_t	write(const byte_t *string, size_t size)=0;
		virtual	ssize_t	write(const char *string)=0;
		virtual	ssize_t	write(const char *string, size_t length)=0;
		virtual	ssize_t	write(char character)=0;
		virtual	ssize_t	write(const wchar_t *string)=0;
		virtual	ssize_t	write(const wchar_t *string, size_t length)=0;
		virtual	ssize_t	write(wchar_t character)=0;
		virtual	ssize_t	writeUcs2(const ucs2_t *string)=0;
		virtual	ssize_t	writeUcs2(const ucs2_t *string,
							size_t length)=0;
		virtual	ssize_t	writeUcs2(ucs2_t character)=0;
		virtual	ssize_t	write(int16_t number)=0;
		virtual	ssize_t	write(int32_t number)=0;
		virtual	ssize_t	write(int64_t number)=0;
		virtual	ssize_t	write(byte_t character)=0;
		virtual	ssize_t	write(uint16_t number)=0;
		virtual	ssize_t	write(uint32_t number)=0;
		virtual	ssize_t	write(uint64_t number)=0;
		virtual	ssize_t	write(float number)=0;
		virtual	ssize_t	write(double number)=0;

		virtual	ssize_t	write(const byte_t *string, size_t size,
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
		virtual	ssize_t	writeUcs2(const ucs2_t *string,
						int32_t sec, int32_t usec);
		virtual	ssize_t	writeUcs2(const ucs2_t *string, size_t length,
						int32_t sec, int32_t usec);
		virtual	ssize_t	writeUcs2(ucs2_t character,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int16_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int32_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(int64_t number,
						int32_t sec, int32_t usec);
		virtual	ssize_t	write(byte_t character,
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
		 *  occurred.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printf(const char *format, ...);

		/** Writes "argp" using "format" which should comply with
		 *  standard printf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of bytes written.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printf(const char *format, va_list *argp);

		/** Writes "..." using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Returns the number of bytes written or -1 if an error
		 *  occurred.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printf(const wchar_t *format, ...);

		/** Writes "argp" using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of bytes written.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printf(const wchar_t *format, va_list *argp);

		/** Writes "..." using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Returns the number of bytes written or -1 if an error
		 *  occurred.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printfUcs2(const ucs2_t *format, ...);

		/** Writes "argp" using "format" which should comply with
		 *  standard wprintf formatting rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns the number of bytes written.
		 *
		 *  NOTE: The various implemententations of this method
		 *  use whatever underlying v(d/s/f)printf() functions are
		 *  available, which, as it turns out, yield fairly
		 *  inconsistent results between each other, especially across
		 *  platforms, when printing wide characters.  Don't be
		 *  surprised if you get different results, when printf()ing
		 *  wide strings to a stringbuffer vs. filedescriptor, or to a
		 *  buffered vs. unbuffered file descriptor. */
		virtual	ssize_t	printfUcs2(const ucs2_t *format, va_list *argp);

		/** Prints "c" to the output, however if "c" is non-printing
		 *  character then it is printed as a hex value of the format:
		 *  (0x0a).  Carriage returns, line feeds and tabs are printed
		 *  as \n, \r and \t. */
		void	safePrint(byte_t c);

		/** Prints "str" to the output, however all non-printing
		 *  characters are printed as hex values of the format: (0x0a)
		 *  and carriage returns, line feeds and tabs are printed as
		 *  \n, \r and \t. */
		void	safePrint(const byte_t *str);

		/** Prints "length" characters of "str" to the output,
		 *  however all non-printing characters are printed as hex
		 *  values of the format: (0x0a) and carriage returns, line
		 *  feeds and tabs are printed as \n, \r and \t. */
		void	safePrint(const byte_t *str, int32_t length);

		/** Prints "c" to the output, however if "c" is non-printing
		 *  character then it is printed as a hex value of the format:
		 *  (0x0a).  Carriage returns, line feeds and tabs are printed
		 *  as \n, \r and \t. */
		void	safePrint(char c);

		/** Prints "str" to the output, however all non-printing
		 *  characters are printed as hex values of the format: (0x0a)
		 *  and carriage returns, line feeds and tabs are printed as
		 *  \n, \r and \t. */
		void	safePrint(const char *str);

		/** Prints "length" characters of "str" to the output, however
		 *  all non-printing characters are printed as hex values of the
		 *  format: (0x0a) and carriage returns, line feeds and tabs
		 *  are printed as \n, \r and \t. */
		void	safePrint(const char *str, int32_t length);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(byte_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(uint16_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(uint32_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(uint64_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(char value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(int16_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(int32_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(int64_t value);

		/** Prints "value" to the output as a string of ones and
		 *  zeros. */
		void	printBits(const byte_t *bits, uint64_t size);

		void	printHex(const byte_t *data, uint64_t size);

		void	printHex(const byte_t *data,
					uint64_t size, uint16_t indent);

	#include <rudiments/private/output.h>
};

#include <rudiments/private/outputinlines.h>

#endif
