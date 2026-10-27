// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_WSTRINGBUFFER_H
#define RUDIMENTS_WSTRINGBUFFER_H

#include <rudiments/private/wstringbufferincludes.h>

/** The wstringbuffer class can be used to store wide strings of arbitrary
 *  length.
 *
 *  It is generally more efficient to reuse the same wstringbuffer over and
 *  over than to allocate a new one for each operation. */
class RUDIMENTS_DLLSPEC wstringbuffer : public bytebuffer {
	public:

		/** Creates an instance of the wstringbuffer class with
		 *  default initial size. */
		wstringbuffer();

		/** Creates an instance of the wstringbuffer class with the
		 *  specified "initialsize", in bytes. */
		wstringbuffer(size_t initialsize);

		/** Creates an instance of the wstringbuffer class.
		 *  The buffer "initialcontents" will be attached to the
		 *  instance and used as the initial contents.  This buffer
		 *  should not be freed by the calling program.  The initial
		 *  size, in characters, may be specified by the "initialsize"
		 *  parameter.  If "initialsize" characters would take more
		 *  than SSIZE_MAX bytes then the initial size is 0. */
		wstringbuffer(wchar_t *initialcontents, size_t initialsize);

		/** Creates an instance of the wstringbuffer class
		 *  that is a copy of "s". */
		wstringbuffer(wstringbuffer &s);

		/** Makes this instance of the wstringbuffer class
		 *  identical to "s". */
		wstringbuffer	&operator=(wstringbuffer &s);

		/** Deletes this instance of the wstringbuffer class. */
		~wstringbuffer();

		/** Sets the position (relative to the beginning of the buffer)
		 *  at which the next read or write will occur to "offset"
		 *  characters.  If the position is set beyond the end of the
		 *  buffer, the buffer will grow but the data between the
		 *  current end of the buffer and the new position will be
		 *  undefined.
		 *
		 *  Returns the new position in characters or -1 if the
		 *  offset, in bytes, does not fit in an off64_t or the new
		 *  position would be before the start of the buffer.  In that
		 *  case the position does not change. */
		off64_t	setPositionRelativeToBeginning(off64_t pos);

		/** Advances the position at which the next read or write will
		 *  occur by "offset" characters.  If the position is set
		 *  beyond the end of the buffer, the buffer will grow but the
		 *  data between the current end of the buffer and the new
		 *  position will be undefined.
		 *
		 *  Returns the new position in characters or -1 if the
		 *  offset, in bytes, does not fit in an off64_t or the new
		 *  position would be before the start of the buffer.  In that
		 *  case the position does not change. */
		off64_t	setPositionRelativeToCurrent(off64_t pos);

		/** Sets the position at which the next read or write will
		 *  occur to the end of the buffer plus "offset" characters.
		 *  Generally, offset will be negative.  If it is positive
		 *  then the buffer will grow, but the data between the current
		 *  buffer will grow but the data between the current end of
		 *  the buffer and the new position will be undefined.
		 *
		 *  Returns the new position in characters or -1 if the
		 *  offset, in bytes, does not fit in an off64_t or the new
		 *  position would be before the start of the buffer.  In that
		 *  case the position does not change. */
		off64_t	setPositionRelativeToEnd(off64_t pos);

		/** Returns the position in the internal buffer at which
		 *  the next write will occur, in characters. */
		off64_t	getPosition();

		/** Writes "string" to the wstringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written or -1 on error. */
		ssize_t	write(const wchar_t *string);

		/** Writes the first "length" characters of "string" to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns the number of bytes written or -1 on error. */
		ssize_t	write(const wchar_t *string, size_t length);

		/** Writes "character" to the wstringbuffer at the current
		 *  position and increments the current position to the next
		 *  byte after the data that was written.  If necessary, the
		 *  internal buffer will grow to accommodate the new data.
		 *
		 *  Returns the number of bytes written or -1 on error. */
		ssize_t	write(wchar_t character);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(int16_t number);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(int32_t number);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the inernal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(int64_t number);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(uint16_t number);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(uint32_t number);

		/** Converts "number" to a string and writes it to the
		 *  wstringbuffer at the current position and increments the
		 *  current position to the next byte after the data that was
		 *  written.  If necessary, the internal buffer will grow to
		 *  accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(uint64_t number);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the wstringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the interna
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the wstringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the wstringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and writes it to the wstringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and writes it to the wstringbuffer at the current position
		 *  and increments the current position to the next byte after
		 *  the data that was written.  If necessary, the internal
		 *  buffer will grow to accommodate the new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and writes it to the wstringbuffer
		 *  at the current position and increments the current position
		 *  to the next byte after the data that was written.  If
		 *  necessary, the internal buffer will grow to accommodate the
		 *  new data.
		 *
		 *  Returns -1 on error. */
		ssize_t	write(double number, uint16_t precision,
							uint16_t scale);


		/** Appends "string" to the wstringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(const wchar_t *string);

		/** Appends the first "length" characters of "string" to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(const wchar_t *string, size_t length);

		/** Appends "character" to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(wchar_t character);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int16_t number);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int32_t number);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int64_t number);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint16_t number);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint32_t number);

		/** Converts "number" to a string and appends it to the
		 *  wstringbuffer, growing the internal buffer as necessary to
		 *  accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint64_t number);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(int64_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint16_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint32_t number, uint16_t zeropadding);

		/** Converts "number" to a string with the specified amount of
		 *  zeropadding and appends it to the wstringbuffer, growing the
		 *  internal buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(uint64_t number, uint16_t zeropadding);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the wstringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(float number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the wstringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(float number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the wstringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(float number, uint16_t precision,
							uint16_t scale);

		/** Converts "number" to a string (using a default scale of 4)
		 *  and appends it to the wstringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(double number);

		/** Converts "number" to a string using the specified "scale"
		 *  and appends it to the wstringbuffer, growing the internal
		 *  buffer as necessary to accommodate the new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(double number, uint16_t scale);

		/** Converts "number" to a string using the specified
		 *  "precision" and "scale" and appends it to the wstringbuffer,
		 *  growing the internal buffer as necessary to accommodate the
		 *  new data.
		 *
		 *  Returns a pointer to "this" to enable chaining such as:
		 *	sb->append("numbers: ")->append(5)->append(5.5); */
		wstringbuffer	*append(double number, uint16_t precision,
							uint16_t scale);

		/** Truncates the wstringbuffer at position "pos", in
		 *  characters.  If "pos" is not less than the current size,
		 *  the size is unchanged. */
		void	truncate(size_t pos);

		/** Truncates the wstringbuffer at the current position.  If the
		 *  position is not less than the current size, the size is
		 *  unchanged. */
		void	truncate();

		/** Empties the wstringbuffer.
 		 *  
 		 *  Always returns true. */
		bool	clear();

		/** Empties the bytebuffer and resets the specified
		 *  "initialsize" parameter, in bytes.
		 *  
		 *  Always returns true. */
		bool	clear(size_t initialsize);

		/** Empties the bytebuffer and attaches the buffer
		 *  "initialcontents" to the instance as the initial contents.
		 *  This buffer should not be freed by the calling program.
		 *  Also resets the specified "initialsize" parameter, in
		 *  characters.  If "initialsize" characters would take more
		 *  than SSIZE_MAX bytes then the initial size is 0.
		 *
		 *  Always returns true. */
		bool	clear(wchar_t *initialcontents, size_t initialsize);

		/** Returns the string currently stored in the wstringbuffer. */
		const wchar_t	*getString();

		/** Returns a pointer to the string currently stored
		 *  in the wstringbuffer, then resets the wstringbuffer
		 *  such that it will no longer use that string.
		 * 
		 *  The calling program must deallocate the string
		 *  returned from this method. */
		wchar_t	*detachString();

		/** Returns the length of the string currently stored
		 *  in the wstringbuffer. */
		size_t	getStringLength();
};

#include <rudiments/private/wstringbufferinlines.h>

#endif
