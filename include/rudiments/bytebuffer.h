// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_BYTEBUFFER_H
#define RUDIMENTS_BYTEBUFFER_H

#include <rudiments/private/bytebufferincludes.h>

/** The bytebuffer class can be used to store binary data of arbitrary length.
 *
 *  It is generally more efficient to reuse the same bytebuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC bytebuffer : public output {
	public:

		/** Creates an instance of the bytebuffer class with
 		 *  default initial size and increment parameters. */
		bytebuffer();

		/** Creates an instance of the bytebuffer class with
		 *  the specified "initialsize". */
		bytebuffer(size_t initialsize);

		/** Creates an instance of the bytebuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program. */
		bytebuffer(byte_t *initialcontents, size_t initialsize);

		/** Creates an instance of the bytebuffer class
		 *  that is a copy of "v". */
		bytebuffer(bytebuffer &v);

		/** Makes this instance of the bytebuffer class
		 *  identical to "v". */
		bytebuffer	&operator=(bytebuffer &v);

		/** Deletes this instance of the bytebuffer class. */
		virtual	~bytebuffer();

		/** Sets the position (relative to the beginning of the buffer)
		 *  at which the next read or write will occur to "offset".
		 *  If the position is set beyond the end of the buffer, the
		 *  buffer will grow but the data between the current end of
		 *  the buffer and the new position will be undefined. */
		off64_t	setPositionRelativeToBeginning(off64_t offset);

		/** Advances the position at which the next read or write will
		 *  occur by "offset" bytes.  If the position is set beyond the
		 *  end of the buffer, the buffer will grow but the data between
		 *  the current end of the buffer and the new position will be
		 *  undefined. */
		off64_t	setPositionRelativeToCurrent(off64_t offset);

		/** Sets the position at which the next read or write will
		 *  occur to the end of the buffer plus "offset" bytes.
		 *  Generally, offset will be negative.  If it is positive
		 *  then the buffer will grow, but the data between the current
		 *  buffer will grow but the data between the current end of
		 *  the buffer and the new position will be undefined. */
		off64_t	setPositionRelativeToEnd(off64_t offset);

		/** Returns the position in the buffer at which
		 *  the next write will occur. */
		size_t	getPosition();

		/** Writes the first "size" bytes of "data" to the
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(const byte_t *data, size_t size);

		/** Writes the first "length" characters of "string" to the
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(const char *string, size_t length);

		/** Writes "string" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(const char *string);

		/** Writes the first "length" characters of "string" to the
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(const wchar_t *string, size_t length);

		/** Writes "string" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(const wchar_t *string);

		/** Writes the first "length" characters of "string" to the
		 *  bytebuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	writeUcs2(const ucs2_t *string, size_t length);

		/** Writes "string" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	writeUcs2(const ucs2_t *string);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(char character);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(wchar_t character);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	writeUcs2(ucs2_t character);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(int16_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(int32_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(int64_t number);

		/** Writes "character" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(byte_t character);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(uint16_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(uint32_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(uint64_t number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(float number);

		/** Writes "number" to the bytebuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written. */
		ssize_t	write(double number);

		/** Appends the first "size" bytes of "data" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const byte_t *data, size_t size);

		/** Appends the first "length" characters of "string" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const char *string, size_t length);

		/** Appends "string" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const char *string);

		/** Appends the first "length" characters of "string" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const wchar_t *string, size_t length);

		/** Appends "string" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(const wchar_t *string);

		/** Appends the first "length" characters of "string" to the
		 *  bytebuffer, growing the internal buffer as necessary
		 *  to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*appendUcs2(const ucs2_t *string,
							size_t length);

		/** Appends "string" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*appendUcs2(const ucs2_t *string);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(char character);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(wchar_t character);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*appendUcs2(ucs2_t character);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int16_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int32_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(int64_t number);

		/** Appends "character" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(byte_t character);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint16_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint32_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(uint64_t number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(float number);

		/** Appends "number" to the bytebuffer, growing the
		 *  internal buffer as necessary to accommodate the new data. 
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *  	vb->append("numbers: ")->append(5)->append(5.5); */
		bytebuffer	*append(double number);

		/** Appends "..." to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules.
		 *
		 *  Returns NULL if an error occurred. */
		bytebuffer	*appendFormatted(const char *format, ...);

		/** Appends "argp" to the byte buffer using "format"
		 *  which should comply with standard printf formatting
		 *  rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns NULL if an error occurred. */
		bytebuffer	*appendFormatted(const char *format,
							va_list *argp);

		/** Appends "..." to the byte buffer using "format"
		 *  which should comply with standard wprintf formatting
		 *  rules.
		 *
		 *  Returns NULL if an error occurred.
		 *
		 *  NOTE: This method is unsupported on platforms where
		 *  wcharstring::supportsPrintf() returns false.  On those
		 *  platforms this method returns -1 and sets ENOSYS. */
		bytebuffer	*appendFormatted(const wchar_t *format, ...);

		/** Appends "argp" to the byte buffer using "format"
		 *  which should comply with standard wprintf formatting
		 *  rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns NULL if an error occurred.
		 *
		 *  NOTE: This method is unsupported on platforms where
		 *  wcharstring::supportsPrintf() returns false.  On those
		 *  platforms this method returns -1 and sets ENOSYS. */
		bytebuffer	*appendFormatted(const wchar_t *format,
							va_list *argp);

		/** Appends "..." to the byte buffer using "format"
		 *  which should comply with standard wprintf formatting
		 *  rules.
		 *
		 *  Returns NULL if an error occurred. */
		bytebuffer	*appendUcs2Formatted(const ucs2_t *format, ...);

		/** Appends "argp" to the byte buffer using "format"
		 *  which should comply with standard wprintf formatting
		 *  rules.
		 *
		 *  Note that argp is a pointer to a va_list, not just a
		 *  va_list.
		 *
		 *  Returns NULL if an error occurred. */
		bytebuffer	*appendUcs2Formatted(const ucs2_t *format,
								va_list *argp);

		/** Truncates the bytebuffer at position "pos". */
		virtual void	truncate(size_t pos);

		/** Truncates the bytebuffer at the current position. */
		virtual void	truncate();


		/** Reads "size" bytes from the bytebuffer at the
		 *  current position into "data".  Also increments the
		 *  current position by "size" bytes.  Returns the number of
		 *  bytes read. */
		ssize_t	read(byte_t *data, size_t size);

		/** Empties the bytebuffer.
		 *  
		 *  Always returns true. */
		bool	clear();

		/** Empties the bytebuffer and resets the specified
		 *  "initialsize" parameter.
		 *  
		 *  Always returns true. */
		bool	clear(size_t initialsize);

		/** Empties the bytebuffer and attaches the buffer
		 *  "initialcontents" to the instance as the initial contents.
		 *  This buffer should not be freed by the calling program.
		 *  Also resets the specified "initialsize".
		 *
		 *  Always returns true. */
		bool	clear(byte_t *initialcontents, size_t initialsize);

		/** Returns the current data stored in the bytebuffer. */
		const byte_t	*getBuffer();

		/** Returns the number of bytes currently stored in the
		 *  bytebuffer. */
		size_t		getSize();

		/** Returns the actual number of bytes of internal storage
		 *  used by the bytebuffer, which may be larger than the value
		 *  returned by getSize() since the buffer grows in extents. */
		size_t		getActualSize();

		/** Returns the number of bytes in the original initial
		 *  exent. */
		size_t		getInitialSize();

		/** Returns a pointer to the buffer currently
		 *  stored in the bytebuffer, then resets
		 *  the bytebuffer such that it will no
		 *  longer use that buffer.
		 * 
		 *  The calling program must deallocate the
		 *  buffer returned from this method. */
		byte_t	*detachBuffer();

	#include <rudiments/private/bytebuffer.h>
};

#endif
