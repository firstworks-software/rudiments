// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_STRINGBUFFER_H
#define RUDIMENTS_STRINGBUFFER_H

#include <rudiments/private/stringbufferincludes.h>

/** The stringbuffer class can be used to store strings of arbitrary length.
 *
 *  It is generally more efficient to reuse the same stringbuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC stringbuffer : public bytebuffer {
	public:

		/** Creates an instance of the stringbuffer class with
		 *  default initial size. */
		stringbuffer();

		/** Creates an instance of the stringbuffer class with the
		 *  specified "initialsize". */
		stringbuffer(size_t initialsize);

		/** Creates an instance of the stringbuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size may be specified by the "initialsize" parameter. */
		stringbuffer(char *initialcontents, size_t initialsize);

		/** Creates an instance of the stringbuffer class
		 *  that is a copy of "s". */
		stringbuffer(stringbuffer &s);

		/** Makes this instance of the stringbuffer class
		 *  identical to "s". */
		stringbuffer	&operator=(stringbuffer &s);

		/** Deletes this instance of the stringbuffer class. */
		~stringbuffer();

		/** Sets the position (relative to the beginning of the buffer)
		 *  at which the next read or write will occur to "offset".
		 *  If the position is set beyond the end of the buffer, the
		 *  buffer will grow but the data between the current end of
		 *  the buffer and the new position will be undefined. */
		off64_t	setPositionRelativeToBeginning(off64_t pos);

		/** Advances the position at which the next read or write will
		 *  occur by "offset" bytes.  If the position is set beyond the
		 *  end of the buffer, the buffer will grow but the data between
		 *  the current end of the buffer and the new position will be
		 *  undefined. */
		off64_t	setPositionRelativeToCurrent(off64_t pos);

		/** Sets the position at which the next read or write will
		 *  occur to the end of the buffer plus "offset" bytes.
		 *  Generally, offset will be negative.  If it is positive
		 *  then the buffer will grow, but the data between the current
		 *  buffer will grow but the data between the current end of
		 *  the buffer and the new position will be undefined. */
		off64_t	setPositionRelativeToEnd(off64_t pos);

		/** Returns the position in the internal buffer at which
		 *  the next write will occur. */
		off64_t	getPosition();

		/** Writes "string" to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
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

		/** Writes "string" to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const char *string);

		/** Writes the first "length" characters of "string" to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(const char *string, size_t length);

		/** Writes "character" to the stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(char character);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int16_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int32_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the inernal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(int64_t number);

		/** Writes "character" to the stringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(byte_t character);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint16_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint32_t number);

		/** Converts "number" to a string and writes it to the
		 *  stringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(uint64_t number);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the interna
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the stringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the stringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the stringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->write("numbers: ")->write(5)->write(5.5); */
		ssize_t	write(double number, uint16_t precision,
							uint16_t scale);


		/** Appends "string" to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const byte_t *string);

		/** Appends the first "size" bytes of "string" to the
		 *  stringbuffer, growing the internall buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const byte_t *string, size_t size);

		/** Appends "string" to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const char *string);

		/** Appends the first "length" characters of "string" to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(const char *string, size_t length);

		/** Appends "character" to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(char character);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int16_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int32_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int64_t number);

		/** Appends "character" to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(byte_t character);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint16_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint32_t number);

		/** Converts "number" to a string and appends it to the
		 *  stringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint64_t number);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(int64_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the stringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(uint64_t number, uint16_t zeropadding);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the stringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the stringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the stringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		stringbuffer	*append(double number, uint16_t precision,
							uint16_t scale);

		/** Trucnates the stringbuffer at position "pos". */
		void	truncate(size_t pos);

		/** Trucnates the stringbuffer at the current position. */
		void	truncate();

		/** Empties the stringbuffer.
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
		bool	clear(char *initialcontents, size_t initialsize);

		/** Returns the string currently stored in the stringbuffer. */
		const char	*getString();

		/** Returns a pointer to the string currently stored
		 *  in the stringbuffer, then resets the stringbuffer
		 *  such that it will no longer use that string.
		 * 
		 *  The calling program must deallocate the string
		 *  returned from this method. */
		char	*detachString();

		/** Returns the length of the string currently stored
		 *  in the stringbuffer. */
		size_t	getStringLength();
};

#include <rudiments/private/stringbufferinlines.h>

#endif
