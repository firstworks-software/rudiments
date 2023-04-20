// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_OUTPUT_H
#define RUDIMENTS_OUTPUT_H

#include <rudiments/private/outputincludes.h>

/** The output class provides a base class for things to which output can be
 *  directed (file descriptors, files, sockets, string buffers, etc.) */
class RUDIMENTS_DLLSPEC output : virtual public object {
	public:

		/** Creates an instance of the output class. */
		output();

		/** Deletes this instance of the output class. */
		virtual ~output();


		/** Sets the position (relative to the beginning of the file
		 *  descriptor) at which the next write will occur to "offset".
		 *  Returns that position on success or -1 on failure.
		 *
		 *  This implementation of the method ignores "offset" and
		 *  always returns -1, however a child class might implement
		 *  this method to actually set the position. */
		virtual off64_t	setPositionRelativeToBeginning(off64_t offset);

		/** Advances the position at which the next write will occur by
		 *  "offset" bytes.  Returns that position on success or -1 on
		 *  failure.
		 *
		 *  This implementation of the method ignores "offset" and
		 *  always returns -1, however a child class might implement
		 *  this method to actually set the position. */
		virtual off64_t	setPositionRelativeToCurrent(off64_t offset);

		/** Sets the position at which the next write will occur to the
		 *  end of the file plus "offset" bytes.  Generally, "offset"
		 *  will be negative.  Returns the position on success or -1 on
		 *  failure.
		 *
		 *  This implementation of the method ignores "offset" and
		 *  always returns -1, however a child class might implement
		 *  this method to actually set the position. */
		virtual off64_t	setPositionRelativeToEnd(off64_t offset);

		/** Returns the position at which the next write will occur or
		 *  -1 on failure.
		 *
		 *  This implementation of the method ignores "offset" and
		 *  always returns -1, however a child class might implement
		 *  this method to actually set the position. */
		virtual off64_t getPosition();


		/** Writes "size" bytes of "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(const byte_t *string, size_t size)=0;

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(const char *string)=0;

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		virtual	ssize_t	write(const char *string, size_t length)=0;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(char character)=0;

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(const wchar_t *string)=0;

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		virtual	ssize_t	write(const wchar_t *string, size_t length)=0;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(wchar_t character)=0;

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		virtual	ssize_t	writeUcs2(const ucs2_t *string)=0;

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		virtual	ssize_t	writeUcs2(const ucs2_t *string,
							size_t length)=0;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	writeUcs2(ucs2_t character)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(int16_t number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(int32_t number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(int64_t number)=0;

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		virtual	ssize_t	write(byte_t character)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(uint16_t number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(uint32_t number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(uint64_t number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(float number)=0;

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		virtual	ssize_t	write(double number)=0;


		/** Writes "size" bytes of "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(const byte_t *string, size_t size,
						int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(const char *string,
						int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(const char *string, size_t length,
						int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(char character,
						int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(const wchar_t *string,
						int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(const wchar_t *string, size_t length,
						int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(wchar_t character,
						int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	writeUcs2(const ucs2_t *string,
						int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	writeUcs2(const ucs2_t *string, size_t length,
						int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	writeUcs2(ucs2_t character,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(int16_t number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(int32_t number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(int64_t number,
						int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(byte_t character,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(uint16_t number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(uint32_t number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(uint64_t number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
		virtual	ssize_t	write(float number,
						int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred.
 		 * 
 		 *  In this implementation of the method, "sec" and "usec" are
		 *  ignored, however a child class might implement a timeout
		 *  using them.  In that case, the method would return
		 *  RESULT_TIMEOUT if a timeout occurred. */
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

		/** Prints a hex/character representation "size" bytes of
		 *  "data" to the output. */
		void	printHex(const byte_t *data, uint64_t size);

		/** Prints a hex/character representation "size" bytes of
		 *  "data" to the output, indented by "indent" tabs. */
		void	printHex(const byte_t *data,
					uint64_t size, uint16_t indent);

	#include <rudiments/private/output.h>
};

#include <rudiments/private/outputinlines.h>

#endif
