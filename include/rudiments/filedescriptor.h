// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILEDESCRIPTOR_H
#define RUDIMENTS_FILEDESCRIPTOR_H

#include <rudiments/private/filedescriptorincludes.h>

class listener;

/** The filedescriptor class extends the input and output classes, providing
 *  a base class for child classes that utilize file descriptors. */
class RUDIMENTS_DLLSPEC filedescriptor : public input, public output {
	public:
		/** Creates an instance of the filedescriptor class. */
		filedescriptor();

		/** Deletes this instance of the filedescriptor class.
		 *  Calls close() if it hasn't already been called. */
		virtual	~filedescriptor();

		/** Closes the file descriptor.
		 *  Returns true on success and false on failure. */
		virtual	bool	close();

		/** Returns the file descriptor. */
		int32_t	getFileDescriptor();

		/** Sets the file descriptor associated with
		 *  the class to "filedesc". */
		void	setFileDescriptor(int32_t filedesc);

		/** Set "isstream" to true (the default) if this filedescriptor
		 *  is a stream such as a socket, serial port, fifo, etc. or
		 *  false if this filedescriptor is storage such as a file or
		 *  raw block device.  This setting impacts how buffering
		 *  works, as well as how the various setPosition*() and
		 *  getPosition() methods work.
		 *
		 *  Note that if the file descriptor is a stream, but this is
		 *  set false, or vice versa, then unexpected results may
		 *  occur if buffering is used, and/or the
		 *  setPosition*()/getPosition() methods are called.
		 *
		 *  Note that this method to have any effect, it must be called
		 *  when buffering is disabled.  It must be called immedaiately
		 *  upon creation of the instance, when buffering is disabled
		 *  implicitly, or after a set of calls to disable buffering.
		 *  For example:
		 *
		 *  setReadBufferSize(0);
		 *  setWriteBufferSize(0);
		 *  setIsStream(false);
		 *
		 *  Returns true on success or false if called while buffering
		 *  is enabled. */
		bool	setIsStream(bool isstream);

		/** Returns true if this filedescriptor was set to be a stream
		 *  using setIsStream() or false otherwise. */
		bool	getIsStream();

		/** Duplicates the file descriptor and returns
		 *  the handle of the duplicate descriptor.  The
		 *  old and new descriptors may be used
		 *  interchangeably, they share locks, position
		 *  pointers, flags (except the close-on-exec
		 *  flag), etc.
		 * 
		 *  Returns the lowest-numbered unused descriptor
		 *  on success or -1 on failure. */
		int32_t	duplicate();

		/** Sets file descriptor handle "newfd" to be a
		 *  duplicate of this file descriptor.  If
		 *  "newfd" is already open, it will be closed
		 *  first.
		 * 
		 *  Returns true on success and false on failure. */
		bool	duplicate(int32_t newfd);

		/** Associates a socketlayer "layer" with the filedescriptor.
		 *  To remove the current layer, pass in a NULL for
		 *  "layer". */
		void	setSocketLayer(socketlayer *layer);

		/** Returns the socketlayer currently associated
		 *  with the filedescriptor or NULL if none
		 *  is currently associated. */
		socketlayer	*getSocketLayer();

		/** Returns true if the system supports blocking/nonblocking
		 *  modes and false otherwise. */
		virtual bool	supportsBlockingAndNonBlockingModes();

		/** If "usenonblockingmode" is true then the file descriptor
		 *  put into in non-blocking mode.  If "usenonblockingmode" is
		 *  false then the file descriptor is put into blocking mode.
		 *
		 *  The default for most file descriptors is to be in blocking
		 *  mode, however this is not guaranteed.  It is good practice
		 *  to specifically set the mode.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  Returns false if the system doesn't support
		 *  blocking/nonblocking modes. */
		virtual bool	setNonBlockingMode(bool usenonblockingmode);

		/** Returns true if the file descriptor is in
		 *  non-blocking mode and false otherwise. */
		virtual bool	getNonBlockingMode();

		/** Returns the position (relative to the beginning of the
		 *  file descriptor) at which the next read or write will
		 *  occur, or -1 on failure. */
		virtual off64_t	getPosition();

		/** Sets the position (relative to the beginning of the file
		 *  descriptor) at which the next read or write will occur to
		 *  "offset".  Returns that position on success or -1 on
		 *  failure. */
		virtual off64_t	setPositionRelativeToBeginning(off64_t offset);

		/** Advances the position at which the next read or write will
 		 *  occur by "offset" bytes.  Returns that position on success
 		 *  or -1 on failure. */
		virtual off64_t	setPositionRelativeToCurrent(off64_t offset);

		/** Sets the position at which the next read or write will
		 *  occur to the end of the file plus "offset" bytes.
		 *  Generally, "offset" will be negative though most filesystems
		 *  allow the creation of files with holes in them and that can
		 *  be accomplished by using a positive "offset" and then
		 *  writing data at that position.  Returns the position on
		 *  success or -1 on failure. */
		virtual off64_t	setPositionRelativeToEnd(off64_t offset);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint16_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint32_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(uint64_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int16_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int32_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(int64_t number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(float number);

		/** Writes "number" to the file descriptor.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred. */
		ssize_t	write(double number);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(byte_t character);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(char character);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(wchar_t character);

		/** Writes "character" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	writeUcs2(ucs2_t character);

		/** Writes "value" to the file descriptor.  Returns the
		 *  number of bytes that were successfully written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(bool value);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const byte_t *string);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const char *string);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const wchar_t *string);

		/** Writes NULL-terminated "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	writeUcs2(const ucs2_t *string);

		/** Writes "size" bytes of "string" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const byte_t *string, size_t size);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		ssize_t	write(const char *string, size_t length);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		ssize_t	write(const wchar_t *string, size_t length);

		/** Writes "length" characters of "string" to the file
 		 *  descriptor.  Returns the number of bytes that were
 		 *  successfully written or RESULT_ERROR if an error occurred.
 		 *  Note that it is possible to write beyond the string's NULL
 		 *  terminator using this method.  */
		ssize_t	writeUcs2(const ucs2_t *string, size_t length);

		/** Writes "size" bytes of "buffer" to the file descriptor.
 		 *  Returns the number of bytes that were successfully written
 		 *  or RESULT_ERROR if an error occurred. */
		ssize_t	write(const void *buffer, size_t size);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint16_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint32_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(uint64_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int16_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int32_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(int64_t number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(float number, int32_t sec, int32_t usec);

		/** Writes "number" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(double number, int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(byte_t character, int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(char character, int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(wchar_t character, int32_t sec, int32_t usec);

		/** Writes "character" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	writeUcs2(ucs2_t character, int32_t sec, int32_t usec);

		/** Writes "value" to the file descriptor with a timeout of
		 *  "sec" seconds and "usec" microseconds.  Returns the number
		 *  of bytes that were successfully written or RESULT_ERROR if
		 *  an error occurred or RESULT_TIMEOUT if a timeout
		 *  occurred. */
		ssize_t	write(bool value, int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const byte_t *string, int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const char *string,
					int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const wchar_t *string,
					int32_t sec, int32_t usec);

		/** Writes NULL-terminated "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	writeUcs2(const ucs2_t *string,
					int32_t sec, int32_t usec);

		/** Writes "size" bytes of "string" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred.  Note that it is possible to write beyond
		 *  the string's NULL terminator using this method.  */
		ssize_t	write(const byte_t *string, size_t size,
					int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
		 *  descriptor with a timeout of "sec" seconds and "usec"
		 *  microseconds.  Returns the number of bytes that were
		 *  successfully written or RESULT_ERROR if an error occurred
		 *  or RESULT_TIMEOUT if a timeout occurred.  Note that it is
		 *  possible to write beyond the string's NULL terminator using
		 *  this method.  */
		ssize_t	write(const char *string, size_t length,
					int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
		 *  descriptor with a timeout of "sec" seconds and "usec"
		 *  microseconds.  Returns the number of bytes that were
		 *  successfully written or RESULT_ERROR if an error occurred
		 *  or RESULT_TIMEOUT if a timeout occurred.  Note that it is
		 *  possible to write beyond the string's NULL terminator using
		 *  this method.  */
		ssize_t	write(const wchar_t *string, size_t length,
					int32_t sec, int32_t usec);

		/** Writes "length" characters of "string" to the file
		 *  descriptor with a timeout of "sec" seconds and "usec"
		 *  microseconds.  Returns the number of bytes that were
		 *  successfully written or RESULT_ERROR if an error occurred
		 *  or RESULT_TIMEOUT if a timeout occurred.  Note that it is
		 *  possible to write beyond the string's NULL terminator using
		 *  this method.  */
		ssize_t	writeUcs2(const ucs2_t *string, size_t length,
						int32_t sec, int32_t usec);

		/** Writes "size" bytes of "buffer" to the file descriptor with
		 *  a timeout of "sec" seconds and "usec" microseconds.
		 *  Returns the number of bytes that were successfully written
		 *  or RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	write(const void *buffer, size_t size,
					int32_t sec, int32_t usec);

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint16_t *buffer);

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint32_t *buffer);

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(uint64_t *buffer);

		/** Reads a 16-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int16_t *buffer);

		/** Reads a 32-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int32_t *buffer);

		/** Reads a 64-bit integer from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(int64_t *buffer);

		/** Reads a floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(float *buffer);

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer".  Returns the number of bytes that
		 *  were successfully read or RESULT_ERROR if an error
		 *  occurred. */
		ssize_t	read(double *buffer);

		/** Reads a byte from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(byte_t *buffer);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(char *buffer);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(wchar_t *buffer);

		/** Reads a character from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	readUcs2(ucs2_t *buffer);

		/** Reads a boolean value from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(bool *buffer);

		/** Reads "size" bytes from the file descriptor into "buffer".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(byte_t *buffer, size_t size);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(char *buffer, size_t length);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	read(wchar_t *buffer, size_t length);

		/** Reads "length" characters from the file descriptor into
		 *  "buffer".  Returns the number of bytes that were
		 *  successfully read or RESULT_ERROR if an error occurred. */
		ssize_t	readUcs2(ucs2_t *buffer, size_t length);

		/** Reads "size" bytes from the file descriptor into "buf".
		 *  Returns the number of bytes that were successfully read or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	read(void *buf, size_t size);

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

		/** Reads an unsigned 16-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint16_t *buffer, int32_t sec, int32_t usec);

		/** Reads an unsigned 32-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint32_t *buffer, int32_t sec, int32_t usec);

		/** Reads an unsigned 64-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(uint64_t *buffer, int32_t sec, int32_t usec);

		/** Reads a 16-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int16_t *buffer, int32_t sec, int32_t usec);

		/** Reads a 32-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int32_t *buffer, int32_t sec, int32_t usec);

		/** Reads a 64-bit integer from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(int64_t *buffer, int32_t sec, int32_t usec);

		/** Reads a floating point number from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(float *buffer, int32_t sec, int32_t usec);

		/** Reads a double-precision floating point number from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(double *buffer, int32_t sec, int32_t usec);

		/** Reads a byte from the file descriptor into "buffer" with a
		 *  timeout of "sec" seconds and "usec" microseconds.  Returns
		 *  the number of bytes that were successfully read,
		 *  RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
		 *  timeout occurred. */
		ssize_t	read(byte_t *buffer, int32_t sec, int32_t usec);

		/** Reads a character from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(char *buffer, int32_t sec, int32_t usec);

		/** Reads a character from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(wchar_t *buffer, int32_t sec, int32_t usec);

		/** Reads a character from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	readUcs2(ucs2_t *buffer, int32_t sec, int32_t usec);

		/** Reads a boolean value from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(bool *buffer, int32_t sec, int32_t usec);

		/** Reads "size" bytes from the file descriptor into "buffer"
 		 *  with a timeout of "sec" seconds and "usec" microseconds.
 		 *  Returns the number of bytes that were successfully read,
 		 *  RESULT_ERROR if an error occurred or RESULT_TIMEOUT if a
 		 *  timeout occurred. */
		ssize_t	read(byte_t *buffer, size_t size,
					int32_t sec, int32_t usec);

		/** Reads "length" characters from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(char *buffer, size_t length,
					int32_t sec, int32_t usec);

		/** Reads "length" characters from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(wchar_t *buffer, size_t length,
					int32_t sec, int32_t usec);

		/** Reads "length" characters from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	readUcs2(ucs2_t *buffer, size_t length,
					int32_t sec, int32_t usec);

		/** Reads "size" bytes from the file
		 *  descriptor into "buffer" with a timeout of "sec" seconds
		 *  and "usec" microseconds.  Returns the number of bytes that
		 *  were successfully read, RESULT_ERROR if an error
		 *  occurred or RESULT_TIMEOUT if a timeout occurred. */
		ssize_t	read(void *buf, size_t size,
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


		/** Causes the application to wait until a read()
		 *  will proceed without blocking or until "sec"
		 *  seconds and "usec" microseconds have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately unless a
		 *  data is immediately available.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be
		 *  read from.
		 * 
		 *  This will usually be 1, indicating that the
		 *  file descriptor represented by the class is
		 *  ready to be read from. */
		int32_t	waitForNonBlockingRead(int32_t sec, int32_t usec);

		/** Causes the application to wait until a
		 *  write() will proceed without blocking or
		 *  until "sec" seconds and "usec" microseconds
		 *  have elapsed.
		 * 
		 *  Entering -1 for either parameter causes the
		 *  method to wait indefinitely.  
		 * 
		 *  Entering 0 for both parameters causes the
		 *  method to fall through immediately unless a
		 *  data is immediately available.
		 * 
		 *  Returns RESULT_ERROR on error, RESULT_TIMEOUT
		 *  on timeout and otherwise returns the number
		 *  of file descriptors that are ready to be
		 *  written to.
		 * 
		 *  This will usually be 1, indicating that the
		 *  file descriptor represented by the class is
		 *  ready to be written to. */
		int32_t	waitForNonBlockingWrite(int32_t sec, int32_t usec);


		/** If "retry" is true then a read will be automatically
		 *  retried if it is interrupted by a signal.  If "retry" is
		 *  false then, if a read is interrupted by a signal, it
		 *  will fail and the system error will be set to EINTR.
		 *
		 *  Defaults to false. */
		void	setRetryInterruptedReads(bool retry);

		/** Returns true if interrupted reads will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedReads();

		/** If "retry" is true then a write will be automatically
		 *  retried if it is interrupted by a signal.  If "retry" is
		 *  false then, if a write is interrupted by a signal, it
		 *  will fail and the system error will be set to EINTR.
		 *
		 *  Defaults to false. */
		void	setRetryInterruptedWrites(bool retry);

		/** Returns true if interrupted writes will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedWrites();

		/** If "retry" is true then a wait operation will be
		 *  automatically retried if it is interrupted by a signal.  If
		 *  "retry" is false then, if a wait operation is interrupted by
		 *  a signal, it will fail and the system error will
		 *  be set to EINTR.
		 *
		 *  Defaults to true. */
		void	setRetryInterruptedWaits(bool retry);

		/** Returns true if interrupted waits will be retried and false
		 *  otherwise. */
		bool	getRetryInterruptedWaits();

		/** If "retry" is true then a fcntl operation will be
		 *  automatically retried if it is interrupted by a signal.  If
		 *  "retry" is false then, if a fcntl operation is interrupted
		 *  by a signal, it will fail and the system error will be set
		 *  to EINTR.
		 *
		 *  Defaults to true. */
		void	setRetryInterruptedFcntl(bool retry);

		/** Returns true if interrupted calls to fcntl will be retried
		 *  and false otherwise. */
		bool	getRetryInterruptedFcntl();

		/** If "retry" is true then an ioctl operation will be
		 *  automatically retried if it is interrupted by a signal.  If
		 *  "retry" is false then, if an ioctl operation is interrupted
		 *  by a signal, it will fail and the system error will be set
		 *  to EINTR.
		 *
		 *  Defaults to true. */
		void	setRetryInterruptedIoctl(bool retry);

		/** Returns true if interrupted calls to ioctl will be retried
		 *  and false otherwise. */
		bool	getRetryInterruptedIoctl();


		/** If "allow" is set true then read() will only attempt a
		 *  single pass and return the number of bytes that were read
		 *  during that  pass whether the specified number of bytes
		 *  were read or not.
		 *  
		 *  If "allow" is set false then read() will attempt to read
		 *  the specified number of bytes from the file descriptor,
		 *  even if several passes are necessary.
		 *
		 *  Defaults to false. */
		void	setAllowShortReads(bool allow);

		/** Returns true if read()s will attempt to read the specified
		 *  number of bytes from the file descriptor, even if several
		 *  passes are necessary, and false otherwise. */
		bool	getAllowShortReads();

		/** If "allow" is set true then write() will only attempt a
		 *  single pass and return the number of bytes that were written
		 *  during that pass whether the specified number of bytes
		 *  were written or not.
		 *  
		 *  If "allow" is set false then write() will attempt to write
		 *  the specified number of bytes to the file descriptor,
		 *  even if several passes are necessary.
		 *
		 *  Defaults to false. */
		void	setAllowShortWrites(bool allow);

		/** Returns true if read()s will attempt to read the specified
		 *  number of bytes from the file descriptor, even if several
		 *  passes are necessary, and false otherwise. */
		bool	getAllowShortWrites();


		/** Sends file descriptor "fd" to the file descriptor.  This
		 *  is useful for passing an open file descriptor from one
		 *  process to another over a unix socket, for example. */
		virtual bool	passFileDescriptor(int32_t fd);

		/** Receives a file descriptor into buffer "fd".  This
		 *  is useful for receiving an open file descriptor passed
		 *  from another process over a unix socket, for example. */
		virtual bool	receiveFileDescriptor(int32_t *fd);

		/** Returns true if the platform supports passing and receiving
		 *  file descriptors or false otherwise. */
		static bool	supportsPassAndReceiveFileDescriptor();


		/** Sends socket "sock" to the file descriptor.  This
		 *  is useful for passing an open socket from one
		 *  process to another over a unix socket, for example. */
		virtual bool	passSocket(int32_t sock);

		/** Receives a socket into buffer "sock".  This is useful for
		 *  receiving an open file descriptor passed from another
		 *  process over a unix socket, for example. */
		virtual bool	receiveSocket(int32_t *sock);

		/** Returns true if the platform supports passing and receiving
		 *  sockets or false otherwise. */
		static bool	supportsPassAndReceiveSocket();


		/** If "translate" is set true then integers will be translated
		 *  from native byte order to network byte order during writes
		 *  and vice-versa during reads.
		 *  
		 *  If "translate" is set false then no translation will be
		 *  done.
		 *
		 *  Defaults to false. */
		void	setTranslateByteOrder(bool translate);

		/** Returns true if integers will be translated from native
		 *  byte order to network byte order during writes and
		 *  vice-versa during reads, and false otherwise. */
		bool	getTranslateByteOrder();

		/** Use the fcntl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	fCntl(int32_t command, long arg);

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg);

		/** If "enabled" is set true then small write()s will be
		 *  collected up and sent together when either the kernel's
		 *  write buffer is full or when a maximum of 0.2 seconds has
		 *  gone by, per Nagle's algorithm.
		 * 
		 *  If "enabled" is set false then writes will be sent
		 *  immediately.
		 *
		 *  Defaults to true.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setNaglesAlgorithmEnabled(bool enabled);

		/** Returns true if small write()s will be collected up and
		 *  sent together when either the kernel's write buffer is full
		 *  or when a maximum of 0.2 seconds has gone by, per Nagle's
		 *  algorithm, or false if writes will be sent immediately. */
		bool	getNaglesAlgorithmEnabled();

		/** Sets the size of the kernel's socket write buffer to "size"
		 *  bytes.  This is only useful for socket file descriptors.
		 *  Returns true on success and false on failure. */
		bool	setSocketWriteBufferSize(int32_t size);

		/** Fetches the size of the kernel's socket write buffer
		 *  (in bytes) into buffer "size".  This is only useful for
		 *  socket file descriptors.  Returns true on success and false
		 *  on failure. */
		bool	getSocketWriteBufferSize(int32_t *size);

		/** Sets the size of the kernel's socket read buffer to "size"
		 *  bytes.  This is only useful for socket file descriptors.
		 *  Returns true on success and false on failure. */
		bool	setSocketReadBufferSize(int32_t size);

		/** Fetches the size of the kernel's socket read buffer
		 *  (in bytes) into buffer "size".  This is only useful for
		 *  socket file descriptors.  Returns true on success and false
		 *  on failure. */
		bool	getSocketReadBufferSize(int32_t *size);

		/** Disables IPv4 on this file descriptor, only IPv6 will be
		 *  used.  This is only useful for TCP Socket file descriptors.
		 *  Returns true on success and false on failure. */
		bool	disableIPv4();

		/** Enables IPv4 (in addition to IPv6) on this file descriptor
		 *  (the default).  This is only useful for TCP Socket file
		 *  descriptors.  Returns true on success and false on
		 *  failure. */
		bool	enableIPv4();

		/** Returns a string representing the type of the
		 *  filedescriptor.  This is "filedescriptor" by default but
		 *  a child class may override this method and return
		 *  something else. */
		const char	*getType();

		/** Returns the IP address of the client at the other end of the
		 *  connection if the filedescriptor is an inet socket or NULL
		 *  otherwise.
		 * 
		 *  Note that the buffer for the address is allocated internally
		 *  and must be freed by the calling program. */
		char	*getPeerAddress();


		/** If an application performs many small writes, the overhead
		 *  of the system calls can slow the application down
		 *  substantially.  To address this, the filedescriptor class
		 *  supports traditional and mmap write buffering.
		 *
		 *  This method sets the write buffer size to "size" bytes.
		 *  A size of 0 (or less) means not to buffer writes at all.
		 *
		 *
		 *  Traditional buffering is enabled by default, but mmap
		 *  buffering can be enabled by calling:
		 *  setMmapBufferingEnabled(true).
		 *
		 *
		 *  When the underlying file descriptor is storage, such as a
		 *  file or raw block device, "size" should be a multiple of
		 *  the block size of the underlying file system or device so
		 *  that block-aligned writes can be performed.
		 *
		 *  When the underlying file descriptor is a stream, such as a
		 *  socket, any "size" can be used.
		 *
		 *
		 *  When traditional buffering is enabled, data passed in to
		 *  any of the write() methods is buffered in user-space, and
		 *  system calls are only made when the buffer is full or when
		 *  it's flushed manually.  Note that this also means that no
		 *  data is actually written to the file descriptor until the
		 *  buffer is full or until it's flushed manually.
		 *
		 *  When using sockets, note that the user-space buffer
		 *  described above is not the same as the tcp write buffer.
		 *  The tcp write buffer is analogous, but resides in kernel
		 *  space, is populated by the write() system call, and is used
		 *  to minimize network latency.
		 *
		 *
		 *  When mmap buffering is enabled, the underlying storage is
		 *  memory mapped, and data is written to memory (and thus to
		 *  the file descriptor) without making system calls.
		 *
		 *  Note the following...
		 *
		 *  Mmap buffering can typically only be used with storage file
		 *  descriptors such as file or raw block devices, and cannot
		 *  typically be used with streaming file descriptors such as
		 *  sockets.  See setIsStream()/getIsStream().
		 *
		 *  When mmap buffering is enabled, the read and write buffer
		 *  sizes must be the same.  Calling this method sets both.
		 *
		 *  On platforms with modern file caching, traditional buffering
		 *  tends to outperform mmap buffering except in very specific
		 *  situations such as when O_DIRECT is used to bypass the
		 *  cache, when using large buffer sizes, or when randomly
		 *  accessing small parts of large files.
		 *
		 *  Sometimes a block cannot be memory-mapped.  For example:
		 *  * on file systems that don't support mmap
		 *  * if "size" is not a multiple of the underlying file
		 *    system's block size
		 *  In these and other cases, the class will fall back to
		 *  traditional buffering for any blocks that cannot be mmaped
		 *  and return to mmap buffering when possible.
		 *
		 *
		 *  Returns true on success and false on failure. */
		bool	setWriteBufferSize(size_t size);

		/** Returns the current size of the write buffer. */
		size_t	getWriteBufferSize();

		/** When write buffering enabled, this method causes the
		 *  contents of the write buffer to be written to the file
		 *  descriptor immediately.  See setWriteBufferSize().
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  If the buffer cannot be flushed within "sec" seconds and
		 *  "usec" microseconds, then the method will fail.  Set either
		 *  "sec" or "usec" to -1 to disable the timeout and allow the
		 *  method to block until the buffer has been flushed. */
		bool	flushWriteBuffer(int32_t sec, int32_t usec);


		/** If an application performs many small reads, the overhead
		 *  of the system calls can slow the application down
		 *  substantially.  To address this, the filedescriptor class
		 *  supports traditional and mmap read buffering.
		 *
		 *  This method sets the read buffer size to "size" bytes.
		 *  A size of 0 (or less) means not to buffer reads at all.
		 *
		 *
		 *  Traditional buffering is enabled by default, but mmap
		 *  buffering can be enabled by calling:
		 *  setMmapBufferingEnabled(true).
		 *
		 *
		 *  When the underlying file descriptor is storage, such as a
		 *  file or raw block device, "size" should be a multiple of
		 *  the block size of the underlying file system or device so
		 *  that block-aligned reads can be performed.
		 *
		 *  When the underlying file descriptor is a stream, such as a
		 *  socket, any "size" can be used.
		 *
		 *
		 *  When traditional buffering is enabled, the class maintains
		 *  a buffer in user-space and attempts to keep it full.  When
		 *  the buffer is empty, th first read will attempt to read
		 *  "size" bytes into the buffer but only return the number of
		 *  bytes specified in the read.  Subsequent reads will just
		 *  return data from the buffer without doing additional system
		 *  calls unless the buffer is empty.
		 *
		 *  When using sockets, note that the user-space buffer
		 *  described above is not the same as the tcp read buffer.
		 *  The tcp read buffer is analogous, but resides in kernel
		 *  space, is populated by the read() system call, and is used
		 *  to minimize network latency.
		 *
		 *
		 *  When mmap buffering is enabled, the underlying storage is
		 *  memory mapped, and data is read from memory (and thus to
		 *  the file descriptor) without making system calls.
		 *
		 *  Note the following...
		 *
		 *  Mmap buffering can typically only be used with storage file
		 *  descriptors such as file or raw block devices, and cannot
		 *  typically be used with streaming file descriptors such as
		 *  sockets.  See setIsStream()/getIsStream().
		 *
		 *  When mmap buffering is enabled, the read and write buffer
		 *  sizes must be the same.  Calling this method sets both.
		 *
		 *  On platforms with modern file caching, traditional buffering
		 *  tends to outperform mmap buffering except in very specific
		 *  situations such as when O_DIRECT is used to bypass the
		 *  cache, when using large buffer sizes, or when randomly
		 *  accessing small parts of large files.
		 *
		 *  Sometimes a block cannot be memory-mapped.  For example:
		 *  * on file systems that don't support mmap
		 *  * if "size" is not a multiple of the underlying file
		 *    system's block size
		 *  * if the last block of a file is smaller than the block
		 *    size of the underlying filesystem, and the file is opened
		 *    read-only
		 *  In these and other cases, the class will fall back to
		 *  traditional buffering for any blocks that cannot be mmaped
		 *  and return to mmap buffering when possible.
		 *
		 *
		 *  Returns true on success and false on failure. */
		bool	setReadBufferSize(size_t size);

		/** Returns the current size of the read buffer. */
		size_t	getReadBufferSize();

		/** If "enabled" is set true then mmap will be used when
		 *  buffering a storage filedescriptor such as a file or raw
		 *  block device.  If set false, then traditional buffering
		 *  will be used.  See setWriteBufferSize()/setReadBufferSize().
		 *
		 *  Note that on platforms with modern file caching,
		 *  traditional buffering tends to outperform mmap buffering
		 *  except in very specific situations such as when O_DIRECT is
		 *  used to bypass the cache, when using large buffer sizes, or
		 *  when randomly accessing small parts of large files.
		 *
		 *  NOTE: if memorymap::isSupported() returns false, then
		 *  calling this method with "enabled" set to true has no
		 *  effect.
		 *
		 *  Defaults to false. */
		void	setMmapBufferingEnabled(bool enabled);

		/** Returns true if mmap will be used when buffering a storage
		 *  filedescriptor such as a file or raw block device, or
		 *  false otherwise.  See setWriteBufferSize()/
		 *  setReadBufferSize(). */
		bool	getMmapBufferingEnabled();

		/** Returns true if this is a storage filedescriptor such as a
		 *  file, raw block device, etc. and the current block is
		 *  buffered using mmap, or false otherwise. */
		bool	isCurrentBlockMmapBuffered();

		/** If buffering is enabled, and this is a storage
		 *  filedescriptor such as a file or raw block device. then
		 *  this returns the offset of the currently buffered block, or
		 *  0 otherwise. */
		off64_t	getCurrentBlockOffset();

		/** If "close" is true then the file descriptor will remain
		 *  open across a process::exec() call.  If "close" is false
		 *  then the file descriptor will be closed during a
		 *  process::exec() call.
		 *
		 *  Defaults to false. */
		bool	setCloseOnExec(bool close);

		/** Returns true if the file descriptor will remain open across
		 *  a process::exec() call and false otherwise. */
		bool	getCloseOnExec();

		/** Creates a pipe and sets "readfd" to the read side and
		 *  "writefd" to the write side. */
		static bool	createPipe(filedescriptor *readfd,
						filedescriptor *writefd);

		/** Converts an unsigned 16-bit integer from host byte order to
		 *  network byte order (big-endian). */
		static	uint16_t	convertHostToNet(uint16_t value);

		/** Converts an unsigned 32-bit integer from host byte order to
		 *  network byte order (big-endian). */
		static	uint32_t	convertHostToNet(uint32_t value);

		/** Converts an unsigned 64-bit integer from host byte order to
		 *  network byte order (big-endian). */
		static	uint64_t	convertHostToNet(uint64_t value);

		/** Converts an unsigned 16-bit integer from network byte order
		 * (bit-endian) to host byte order. */
		static	uint16_t	convertNetToHost(uint16_t value);

		/** Converts an unsigned 32-bit integer from network byte order
		 * (bit-endian) to host byte order. */
		static	uint32_t	convertNetToHost(uint32_t value);

		/** Converts an unsigned 64-bit integer from network byte order
		 * (bit-endian) to host byte order. */
		static	uint64_t	convertNetToHost(uint64_t value);

		/** Converts an unsigned 16-bit integer from host byte order to
		 *  little-endian byte order. */
		static	uint16_t	convertHostToLittleEndian(
							uint16_t value);

		/** Converts an unsigned 32-bit integer from host byte order to
		 *  little-endian byte order. */
		static	uint32_t	convertHostToLittleEndian(
							uint32_t value);

		/** Converts an unsigned 64-bit integer from host byte order to
		 *  little-endian byte order. */
		static	uint64_t	convertHostToLittleEndian(
							uint64_t value);

		/** Converts an unsigned 16-bit integer from little-endian byte
		 *  order to host byte order. */
		static	uint16_t	convertLittleEndianToHost(
							uint16_t value);

		/** Converts an unsigned 32-bit integer from little-endian byte
		 *  order to host byte order. */
		static	uint32_t	convertLittleEndianToHost(
							uint32_t value);

		/** Converts an unsigned 64-bit integer from little-endian byte
		 *  order to host byte order. */
		static	uint64_t	convertLittleEndianToHost(
							uint64_t value);

	#include <rudiments/private/filedescriptor.h>
};

#include <rudiments/private/filedescriptorinlines.h>

#endif
