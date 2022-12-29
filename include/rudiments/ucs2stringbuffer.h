// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UCS2STRINGBUFFER_H
#define RUDIMENTS_UCS2STRINGBUFFER_H

#include <rudiments/private/ucs2stringbufferincludes.h>

/** The ucs2stringbuffer class can be used to store strings of arbitrary length.
 *
 *  It is generally more efficient to reuse the same ucs2stringbuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC ucs2stringbuffer : public bytebuffer {
	public:

		/** Creates an instance of the ucs2stringbuffer class with
		 *  default initial size. */
		ucs2stringbuffer();

		/** Creates an instance of the ucs2stringbuffer class with the
		 *  specified "initialsize". */
		ucs2stringbuffer(size_t initialsize);

		/** Creates an instance of the ucs2stringbuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size may be specified by the "initialsize" parameter. */
		ucs2stringbuffer(ucs2_t *initialcontents, size_t initialsize);

		/** Creates an instance of the ucs2stringbuffer class
		 *  that is a copy of "s". */
		ucs2stringbuffer(ucs2stringbuffer &s);

		/** Makes this instance of the ucs2stringbuffer class
		 *  identical to "s". */
		ucs2stringbuffer	&operator=(ucs2stringbuffer &s);

		/** Deletes this instance of the ucs2stringbuffer class. */
		~ucs2stringbuffer();

		/** Sets the position in the internal buffer at which the next
		 *  write will occur to "pos".  If the position is set beyond
		 *  the end of the buffer, the buffer will grow but the data
		 *  between the current end of the buffer and the new position
		 *  will be undefined. */
		void	setPosition(size_t pos);

		/** Returns the position in the internal buffer at which
		 *  the next write will occur. */
		size_t	getPosition();

		/** Writes "string" to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const byte_t *string);

		/** Writes the first "size" bytes of "string" to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const byte_t *string, size_t size);

		/** Writes "string" to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const ucs2_t *string);

		/** Writes the first "length" characters of "string" to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const ucs2_t *string, size_t length);

		/** Writes "character" to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(ucs2_t character);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int16_t number);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int32_t number);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the inernal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int64_t number);

		/** Writes "character" to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(byte_t character);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint16_t number);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint32_t number);

		/** Converts "number" to a string and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint64_t number);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the ucs2stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the
		 *  ucs2stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number, uint16_t precision,
							uint16_t scale);


		/** Appends "string" to the ucs2stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(const byte_t *string);

		/** Appends the first "size" bytes of "string" to the
		 *  ucs2stringbuffer, growing the internall buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(const byte_t *string,
								size_t size);

		/** Appends "string" to the ucs2stringbuffer, growing the
 		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(const ucs2_t *string);

		/** Appends the first "length" characters of "string" to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(const ucs2_t *string,
							size_t length);

		/** Appends "character" to the ucs2stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(ucs2_t character);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int16_t number);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int32_t number);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int64_t number);

		/** Appends "character" to the ucs2stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(byte_t character);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint16_t number);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint32_t number);

		/** Converts "number" to a string and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint64_t number);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int16_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int32_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(int64_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint16_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint32_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the ucs2stringbuffer, growing
		 *  the internal buffer as necessary to accommodate the new
		 *  data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(uint64_t number,
						uint16_t zeropadding);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the ucs2stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the ucs2stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(float number,
							uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the ucs2stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the ucs2stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the
		 *  ucs2stringbuffer, growing the internal buffer as necessary
		 *  to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		ucs2stringbuffer	*append(double number,
							uint16_t precision,
							uint16_t scale);

		/** Trucnates the ucs2stringbuffer at position "pos". */
		void	truncate(size_t pos);

		/** Trucnates the ucs2stringbuffer at the current position. */
		void	truncate();

		/** Empties the ucs2stringbuffer.
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
		 *  Also resets the specified "initialsize" parameter.
		 *
		 *  Always returns true. */
		bool	clear(ucs2_t *initialcontents, size_t initialsize);

		/** Returns the string currently stored in the
		 *  ucs2stringbuffer. */
		const ucs2_t	*getString();

		/** Returns a pointer to the string currently stored
		 *  in the ucs2stringbuffer, then resets the ucs2stringbuffer
		 *  such that it will no longer use that string.
		 * 
		 *  The calling program must deallocate the string
		 *  returned from this method. */
		ucs2_t	*detachString();

		/** Returns the length of the string currently stored
		 *  in the ucs2stringbuffer. */
		size_t	getStringLength();
};

#include <rudiments/private/ucs2stringbufferinlines.h>

#endif
