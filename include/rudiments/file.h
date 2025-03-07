// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILE_H
#define RUDIMENTS_FILE_H

#include <rudiments/private/fileincludes.h>

/** The file class provides methods for interacting with files and for 
 *  discovering the properties associated with a file such as permissions,
 *  ownership, size, etc.
 * 
 *  Many of the static methods are just provided for convenience.  Use them
 *  if you need to get a a single property or perform a single operation.
 * 
 *  If you need to get multiple properties or perform multiple operations,
 *  create an instance of the class, call open(), then use the non-static
 *  methods. */
class RUDIMENTS_DLLSPEC file : public filedescriptor {
	public:

		/** Creates an instance of the file class. */
		file();

		/** Deletes this instance of the file class. */
		virtual ~file();

		/** Opens the file "name" using "flags".
		 *
		 *  "flags" may contain one more more of the following flags,
		 *  or'ed together:
		 *
		 *  O_RDONLY - Open the file in read-only mode.
		 *
		 *  O_WRONLY - Open the file in write-only mode.
		 *
		 *  O_RDWR - Open the file in write-only mode.
		 *
		 *  O_APPEND - All writes will occur at end of the file.
		 *  (NOTE: This flag conflicts with buffering.  Don't use when
		 *  also using buffering.)
		 *
		 *  O_TRUNC - Truncate the file.  Requires O_WRONLY or O_RDWR.
		 *
		 *  Many platforms support additional, platform-specific flags.
		 *
		 *  Note that since this method doesn't allow file permissions
		 *  to be set, attempts to open the file with the O_CREAT flag
		 *  (which creates the file if it doesn't exist) will fail.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	open(const char *name, int32_t flags);

		/** Opens the file "name" using "flags".
		 *
		 *  "flags" may contain one more more of the following flags,
		 *  or'ed together:
		 *
		 *  O_RDONLY - Open the file in read-only mode.
		 *
		 *  O_WRONLY - Open the file in write-only mode.
		 *
		 *  O_RDWR - Open the file in write-only mode.
		 *
		 *  O_APPEND - All writes will occur at end of the file.
		 *  (NOTE: This flag conflicts with buffering.  Don't use when
		 *  also using buffering.)
		 *
		 *  O_TRUNC - Truncate the file.  Requires O_WRONLY or O_RDWR.
		 *
		 *  O_CREAT - Creates the file if it doesn't exist.  See O_EXCL.
		 *
		 *  O_EXCL - Requires O_CREAT.  Causes O_CREAT to fail if the
		 *  		file already exists.  Without O_EXCL, O_CREAT
		 *  		will succeed, and just open the file, if the
		 *  		file already exists.
		 *
		 *  Many platforms support additional, platform-specific flags.
		 *
		 *  If "flags" contains O_CREAT and the file doesn't already
		 *  exist, then it will be created with permissions "perms".
		 *  Note that if "perms" is 0 then the method will fail.
		 *
		 *  Returns true on success and false on failure. */
		virtual bool	open(const char *name,
						int32_t flags,
						mode_t perms);

		/** Creates the file "name" with permissions "perms".  If the
		 *  file already exists, it will be truncated.  Returns true on
		 *  success and false on failure.
		 *
		 *  Note that if "perms" is 0 then the method will fail.
		 *
		 *  Equivalent to calling:
		 *  open(name,O_CREAT|O_WRONLY|O_TRUNC|O_BINARY,perms) */
		virtual bool	create(const char *name, mode_t perms);


		/** Allocates a string large enough to accommodate the
		 *  contents of the currently opened file, reads the
		 *  contents of the file into the string and returns the
		 *  string.  The string must be freed by the calling
		 *  program.
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  If the file contains no data, then an empty string is
		 *  returned.  If an error occurs then a NULL is returned. */
		virtual char	*getContents();

		/** Reads the contents of the currently open file into
		 *  "buffer" of size "buffersize".  If the file is
		 *  larger than "buffersize" then only "buffersize" bytes
		 *  of the file will be read into "buffer".
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		virtual ssize_t	getContents(byte_t *buffer,
							size_t buffersize);

		/** Allocates "buffer" large enough to accommodate the
		 *  contents of the currently opened file, reads the contents
		 *  of the file into "buffer" and sets "buffersize" to the
		 *  size of the buffer.  "buffer" must be freed by the calling
		 *  program.
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		virtual ssize_t	getContents(byte_t **buffer,
							size_t *buffersize);

		/** Truncates all data in the file, resulting in a file of
		 * zero bytes.  Returns true on success and false on
		 * failure. */
		virtual bool	truncate();

		/** Truncates all data beyond the first "size" bytes,
		 *  resulting in a file of "size" bytes.  Returns true on
		 *  success and false on failure. */
		virtual bool	truncate(off64_t size);


		/** Attempts to lock the entire file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK).  Returns true if the lock can
		 *  be established immediately or false otherwise. */
		virtual bool	tryLockFile(int16_t type);

		/** Attempts to lock the entire file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Return true on success or false
		 *  otherwise. */
		virtual bool	lockFile(int16_t type);

		/** Checks to see if the entire file can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockFile(int16_t type,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Attempts to release a previosly established lock and returns
		 *  true on success or false otherwise. */
		virtual bool	unlockFile();

		/** Attempts to lock "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		virtual bool	tryLockRegion(int16_t type,
						off64_t start,
						off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		virtual bool	lockRegion(int16_t type,
						off64_t start,
						off64_t len);

		/** Checks to see if "len" bytes of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRegion(int16_t type,
						off64_t start,
						off64_t len,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at position "start" (relative to
		 *  the beginning of the file).  Returns true on success or
		 *  false otherwise. */
		virtual bool	unlockRegion(off64_t start,
						off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK).  Returns true if the lock can
		 *  be established immediately or false otherwise. */
		virtual bool	tryLockFromCurrent(int16_t type,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		virtual bool	tryLockFromCurrent(int16_t type,
							off64_t start,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) and blocks until the lock can
		 *  be established.  Returns true on success or false
		 *  otherwise. */
		virtual bool	lockFromCurrent(int16_t type,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		virtual bool	lockFromCurrent(int16_t type,
							off64_t start,
							off64_t len);

		/** Checks to see if "len" bytes of the file starting at
		 *  the current position in the file can be locked or not using
		 *  the specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockFromCurrent(int16_t type,
							off64_t len,
							int16_t *conftype,
							int16_t *confwhence,
							off64_t *confstart,
							off64_t *conflen);

		/** Checks to see if "len" bytes of the file starting at
		 *  the current position in the file plus "start" bytes
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockFromCurrent(int16_t type,
							off64_t start,
							off64_t len,
							int16_t *conftype,
							int16_t *confwhence,
							off64_t *confstart,
							off64_t *conflen);

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the current position in the
		 *  file.  Returns true on success or false otherwise. */
		virtual bool	unlockFromCurrent(off64_t len);

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the current position in the
		 *  file plus "start" bytes.  Returns true on success or
		 *  false otherwise. */
		virtual bool	unlockFromCurrent(off64_t start,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		virtual bool	tryLockFromEnd(int16_t type,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the the end of the file plus "start" bytes using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK).  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise. */
		virtual bool	tryLockFromEnd(int16_t type,
							off64_t start,
							off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can
		 *  be established.  Returns true on success or false
		 *  otherwise. */
		virtual bool	lockFromEnd(int16_t type, off64_t len);

		/** Attempts to lock "len" bytes of the file starting at
		 *  the end of the file plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Returns true on success or false
		 *  otherwise. */
		virtual bool	lockFromEnd(int16_t type,
							off64_t start,
							off64_t len);

		/** Checks to see if "len" bytes of the file starting at
		 *  the end of the file can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockFromEnd(int16_t type,
							off64_t len,
							int16_t *conftype,
							int16_t *confwhence,
							off64_t *confstart,
							off64_t *conflen);

		/** Checks to see if "len" bytes of the file starting at
		 *  the end of the file plus "start" bytes can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockFromEnd(int16_t type,
							off64_t start,
							off64_t len,
							int16_t *conftype,
							int16_t *confwhence,
							off64_t *confstart,
							off64_t *conflen);

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the end of the file.
		 *  Returns true on success or false otherwise. */
		virtual bool	unlockFromEnd(off64_t len);

		/** Attempts to release a previosly established lock of "len"
		 *  bytes of the file starting at the end of the file plus
		 *  "start" bytes.  Returns true on success or false
		 *  otherwise. */
		virtual bool	unlockFromEnd(off64_t start,
							off64_t len);

		/** Attempts to lock the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK).
		 *  Returns true if the lock can be established immediately or
		 *  false otherwise. */
		virtual bool	tryLockRemainder(int16_t type,
							off64_t start);

		/** Attempts to lock the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) and
		 *  blocks until the lock can be established.  Returns true on
		 *  success or false otherwise. */
		virtual bool	lockRemainder(int16_t type,
							off64_t start);

		/** Checks to see if the remainder of the file starting at
		 *  position "start" (relative to the beginning of the file)
		 *  can be locked or not using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) but does not actually attempt to lock
		 *  the file.  Returns true if the lock can be established
		 *  immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRemainder(int16_t type,
							off64_t start,
							int16_t *conftype,
							int16_t *confwhence,
							off64_t *confstart,
							off64_t *conflen);

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at position "start"
		 *  (relative to the begining of the file).  Returns true on
		 *  success or false otherwise. */
		virtual bool	unlockRemainder(off64_t start);

		/** Attempts to lock the remainder of the file starting at
		 *  the current position using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		virtual bool	tryLockRemainderFromCurrent(int16_t type);

		/** Attempts to lock the remainder of the file starting at
		 *  the current position plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK).  Returns true if the lock
		 *  can be established immediately or false otherwise. */
		virtual bool	tryLockRemainderFromCurrent(int16_t type,
							off64_t start);

		/** Attempts to lock the remainder of the file starting at
		 *  the current position using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		virtual bool	lockRemainderFromCurrent(int16_t type);

		/** Attempts to lock the remainder of the file starting at
		 *  the current position plus "start" bytes using the specified
		 *  lock "type" (F_RDLCK or F_WRLCK) and blocks until the lock
		 *  can be established.  Returns true on success or false
		 *  otherwise. */
		virtual bool	lockRemainderFromCurrent(int16_t type,
							off64_t start);

		/** Checks to see if the remainder of the file starting at
		 *  the current position can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRemainderFromCurrent(
						int16_t type,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Checks to see if the remainder of the file starting at
		 *  the current position plus "start" bytes can be locked or not
		 *  using the specified lock "type" (F_RDLCK or F_WRLCK) but
		 *  does not actually attempt to lock the file.  Returns true
		 *  if the lock can be established immediately or false 
		 *  otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRemainderFromCurrent(
						int16_t type,
						off64_t start,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the current position.
		 *  Returns true on success or false otherwise. */
		virtual bool	unlockRemainderFromCurrent();

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the current position
		 *  plus "start" bytes.  Returns true on success or false
		 *  otherwise. */
		virtual bool	unlockRemainderFromCurrent(off64_t start);

		/** Attempts to lock the remainder of the file starting at
		 *  the end using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		virtual bool	tryLockRemainderFromEnd(int16_t type);

		/** Attempts to lock the remainder of the file starting at
		 *  the end plus "start" bytes using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK).  Returns true if the lock can be
		 *  established immediately or false otherwise. */
		virtual bool	tryLockRemainderFromEnd(int16_t type,
							off64_t start);

		/** Attempts to lock the remainder of the file starting at
		 *  the end using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		virtual bool	lockRemainderFromEnd(int16_t type);

		/** Attempts to lock the remainder of the file starting at
		 *  the end plus "start" bytes using the specified lock "type"
		 *  (F_RDLCK or F_WRLCK) and blocks until the lock can be
		 *  established.  Returns true on success or false otherwise. */
		virtual bool	lockRemainderFromEnd(int16_t type,
							off64_t start);

		/** Checks to see if the remainder of the file starting at
		 *  the end can be locked or not using the specified lock
		 *  "type" (F_RDLCK or F_WRLCK) but does not actually attempt
		 *  to lock the file.  Returns true if the lock can be
		 *  established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRemainderFromEnd(
						int16_t type,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Checks to see if the remainder of the file starting at
		 *  the end plus "start" bytes can be locked or not using the
		 *  specified lock "type" (F_RDLCK or F_WRLCK) but does not
		 *  actually attempt to lock the file.  Returns true if the
		 *  lock can be established immediately or false otherwise.
		 *
		 *  If the lock cannot be established then the "conf" parameters
		 *  will be populated with the details about a conflicting lock
		 *  which would prevent the file from being locked.  "conftype"
		 *  will contain the type of the lock (F_RDLCK or F_WRLCK)
		 *  "confwhence" will contain the offset type of the lock
		 *  (SEEK_SET, SEEK_CUR or SEEK_END), "confstart" will contain
		 *  the starting offset for the lock and "conflen" will contain
		 *  the number of bytes that are locked.  */
		virtual bool	checkLockRemainderFromEnd(
						int16_t type,
						off64_t start,
						int16_t *conftype,
						int16_t *confwhence,
						off64_t *confstart,
						off64_t *conflen);

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the end.
		 *  Returns true on success or false otherwise. */
		virtual bool	unlockRemainderFromEnd();

		/** Attempts to release a previosly established lock on the
		 *  remainder of the file, starting at the end plus "start"
		 *  bytes.  Returns true on success or false otherwise. */
		virtual bool	unlockRemainderFromEnd(off64_t start);


		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, sequentially so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseSequentialAccess(off64_t start,
							size_t len);

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, randomly so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseRandomAccess(off64_t start, size_t len);

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, only one time so the kernel can perform some
		 *  optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseOnlyOnce(off64_t start, size_t len);

		/** Advises the kernel that you are going to access the region
		 *  of the currently open file begining at byte "start", for
		 *  "len" bytes, in the near future so the kernel can perform
		 *  some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseWillNeed(off64_t start, size_t len);

		/** Advises the kernel that you are not going to access the
		 *  region of the currently open file begining at byte "start",
		 *  for "len" bytes, in the near future so the kernel can
		 *  perform some optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseWontNeed(off64_t start, size_t len);

		/** Advises the kernel that you are going to access the
		 *  region of the currently open file begining at byte "start",
		 *  for "len" bytes, normally so the kernel can undo any
		 *  previously applied optimizations.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  On operating systems don't support this method, it
		 *  returns true but don't actually do anything. */
		virtual bool	adviseNormalAccess(off64_t start, size_t len);


		/** Reserves space on the storage medium such that a
		 *  write to the region starting at "start" and
		 *  continuing for "len" bytes will not fail due to
		 *  lack of storage space.
		 * 
		 *  Returns true if the region can be reserved and
		 *  false otherwise. */
		virtual bool	reserve(off64_t start, size_t len);


		/** On most filesystems, changes to files are often cached in
		 *  system ram, this methods causes those changes (including
		 *  changes to file metadata such as last-access and
		 *  last-modifiction times) to be immediately copied to the
		 *  storage medium that the file resides on.
		 * 
		 *  Returns true on success and false on failure.
		 * 
		 *  Note that if a hard disk has on-board write cache
		 *  enabled, this method only assures that the changes
		 *  has been copied into the disk's write cache, not
		 *  necessarily to the disk itself. */
		virtual bool	sync();

		/** On most filesystems, changes to files are often cached in
		 *  system ram, this methods causes those changes to be
		 *  immediately copied to the storage medium that the file
		 *  resides on.
		 *
		 *  This method differs from sync() in that it does not
		 *  wait until file metadata such as last-access or
		 *  last-modification times to be written before it returns.
		 * 
		 *  Returns true on success and false on failure.
		 * 
		 *  Note that if a hard disk has on-board write cache
		 *  enabled, this method only assures that the changes
		 *  has been copied into the disk's write cache, not
		 *  necessarily to the disk itself. */
		virtual bool	syncData();


		/** If "getcurrentpropertiesonopen" is true, then open() and
		 *  create() methods will call getCurrentProperties()
		 *  internally so other methods of this class such as getSize()
		 *  will return valid data about the file immediately.  
		 *
		 *  If "getcurrentpropertiesonopen" is false, then open() and
		 *  create() methods will not call getCurrentProperties()
		 *  internally.  This offers a small performance improvement
		 *  which can become significant if you intend to open lots of
		 *  files but don't need to know the size or any other
		 *  properties about them.  If this method is called with false
		 *  and a method such as getSize() is called afterwards, then
		 *  it will call getCurrentProperties() internally.
		 *
		 *  Defaults to true. */
		virtual void	setGetCurrentPropertiesOnOpen(bool set);

		/** Refreshes the current file properties as returned by
		 *  getSize(), getPermissions(), etc.
		 *  
		 *  Returns true on success and false on failure. */
		virtual bool	getCurrentProperties();

		/** Returns the permissions settings for the file. */
		virtual mode_t		getPermissions();

		/** Returns the user id of the file's owner. */
		virtual uid_t		getOwnerUserId();

		/** Returns the group id of the file's owner. */
		virtual gid_t		getOwnerGroupId();

		/** Returns the number of bytes in the file. */
		virtual off64_t		getSize();

		/** Returns the blocksize to use for most
		 *  efficient I/O with the file or -1 for
		 *  systems don't support this. */
		virtual blksize_t	getBlockSize();

		/** Returns the number of blocks allocated for
		 *  the file or -1 for systems don't support this. */
		virtual blkcnt_t	getBlockCount();

		/** Returns 1 if the file is a socket,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isSocket();

		/** Returns 1 if the file is a symbolic link,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isSymbolicLink();

		/** Returns 1 if the file is a regular file,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isRegularFile();

		/** Returns 1 if the file is a block device,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isBlockDevice();

		/** Returns 1 if the file is a directory,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isDirectory();

		/** Returns 1 if the file is a character device,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isCharacterDevice();

		/** Returns 1 if the file is a fifo,
		 *  0 if it's not or -1 on error. */
		virtual int32_t		isFifo();

		/** Returns the time of last access of the file. */
		virtual time_t		getLastAccessTime();

		/** Returns the time of last modification of the file.
		 *
		 *  Modification time IS NOT affected by changing file
		 *  permissions, ownership or creating links. */
		virtual time_t		getLastModificationTime();

		/** Returns the time of last change of the file.
		 *
		 *  Change time IS affected by changing file
		 *  permissions, ownership or creating links. */
		virtual time_t		getLastChangeTime();

		/** Returns the device of the file. */
		virtual dev_t		getDevice();

		/** Returns the device type of the file. */
		virtual dev_t		getDeviceType();

		/** Returns the base inode of the file. */
		virtual uint64_t	getInode();

		/** Returns the number of hard links to the file. */
		virtual nlink_t		getNumberOfHardLinks();


		/** Sets the permissions of the file to "perms".
		 *  Returns true on success and false on failure. */
		virtual bool	setPermissions(mode_t perms);


		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		virtual bool	changeOwner(const char *newuser,
						const char *newgroup);

		/** Changes the user that owns the file.
		 *  Returns true on success and false on failure. */
		virtual bool	changeOwner(uid_t uid, gid_t gid);

		/** Returns true if any of the changeOwner()
		 *  methods are allowed on "filename" and
		 *  false otherwise. */
		virtual bool	getCanChangeOwner();

		/** Returns the maximum number of links that can be
		 *  created to "filename". */
		virtual int64_t	getMaxLinks();


		/** Returns a pointer to the file stats structure
		 *  used internally.  (struct stat * on unix) */
		virtual void 	*getInternalFileStatisticsStructure();


		/** Create a file named "name" with permissions "perms".  If
		 *  the file already exists, it will be truncated.
		 *  Returns true on success and false on failure. */
		static bool	createFile(const char *name, mode_t perms);

		/** Creates a fifo called "filename" with permissions "perms".
		 *  Returns true on success and false on failure. */
		static bool	createFifo(const char *filename, mode_t perms);

		/** Creates a (presumably) temporary file using
		 *  "templatefilename" as a template.  The last 6 characters of
		 *  "templatefilename" must be XXXXXX and
		 *  "templatefilename" will be modified to contain the
		 *  name of the file that was actually created.
		 * 
		 *  Opens and returns the file descriptor handle of the
		 *  file on success and -1 on failure. */
		static int32_t	createTemporaryFile(char *templatefilename);

		/** Creates a temporary file using "templatefilename"
		 *  as a template.  The last 6 characters of
		 *  "templatefilename" must be XXXXXX and
		 *  "templatefilename" will be modified to contain the
		 *  name of the file that was actually created.
		 *
		 *  Sets the permissions of the file to "perms".  If
		 *  permissions cannot be set then the file is removed and
		 *  the call fails.
		 * 
		 *  Opens and returns the file descriptor handle of the
		 *  file on success and -1 on failure. */
		static int32_t	createTemporaryFile(char *templatefilename,
								mode_t perms);

		/** Creates a hard link between "oldpath" and "newpath".
		 *  Returns true on success and false on failure. */
		static bool	createHardLink(const char *oldpath,
							const char *newpath);

		/** Creates a symbolic link between "oldpath" and
		 *  "newpath". Returns true on success and false on
		 *  failure. */
		static bool	createSymbolicLink(const char *oldpath,
							const char *newpath);

		/** Returns the pathname of the file that the symbolic
		 *  link "filename" points to.  Returns NULL on failure.
		 * 
		 *  Note that this method allocates a buffer for the
		 *  path internally and returns it.  The calling program
		 *  must deallocate this buffer. */
		static char	*resolveSymbolicLink(const char *filename);


		/** Renames "oldpath" to "newpath".  Returns true on
		 *  success and false on failure. */
		static bool	rename(const char *oldpath,
					const char *newpath);

		/** Removes file "filename".  Returns true on success
		 *  and false on failure.  (Will not remove a directory). */
		static bool	remove(const char *filename);


		/** Truncates the file. Returns true on success and false
		 *  on failure. */
		static	bool	truncate(const char *filename);

		/** Truncates the file after the first "size" bytes.
		 *  Returns true on success and false on failure. */
		static	bool	truncate(const char *filename, off64_t size);


		/** Allocates a string large enough to accommodate the
		 *  contents of the file "name" , reads the contents of
		 *  the file into the string, terminates it with a NULL
		 *  and returns the string.  The string must be freed by
		 *  the calling program.
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  If the file contains no data, then an empty string is
		 *  returned.  If an error occurs then a NULL
		 *  is returned. */
		static	char	*getContents(const char *name);

		/** Reads the contents of the file "name" into
		 *  "buffer" of size "buffersize".  If the file is
		 *  larger than "buffersize" then only "buffersize" bytes
		 *  of the file will be read into "buffer".
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		static	ssize_t	getContents(const char *name,
						byte_t *buffer,
						size_t buffersize);

		/** Allocates "buffer" large enough to accommodate the
		 *  contents of the file "name", reads the contents of the
		 *  file into "buffer" and sets "buffersize" to the size of
		 *  the buffer.  "buffer" must be freed by the calling program.
		 *
		 *  Note: The contents returned by this method are as-is.
		 *  No end-of-line translation is performed.
		 * 
		 *  Returns the number of bytes copied into "buffer" or
		 *  -1 on error. */
		virtual ssize_t	getContents(const char *name,
						byte_t **buffer,
						size_t *buffersize);


		/** Returns true if the file exists and false otherwise. */
		static bool	exists(const char *filename);

		/** Returns true if "filename" is readable by the user
		 *  or false otherwise. */
		static bool	isReadable(const char *filename);

		/** Returns true if "filename" is writeable by the user
		 *  or false otherwise. */
		static bool	isWriteable(const char *filename);

		/** Returns true if "filename" is executable by the user
		 *  or false otherwise. */
		static bool	isExecutable(const char *filename);

		/** Checks to see if "filename" exists, is readable,
		 *  is writeable and/or is executable by the user, based
		 *  on the value of "mode".  Mode should be an or'ed
		 *  combination of F_OK (exists), R_OK (readable),
		 *  W_OK (writable) and X_OK (executable) flags.
		 * 
		 *  Returns true if the file meets the conditions set
		 *  by the mode and false otherwise. */
		static bool	isAccessible(const char *filename,
							int32_t mode);

		/** Sets the permissions of the file to "perms".
		 *  Returns true on success and false on failure. */
		static bool	setPermissions(const char *filename,
							mode_t perms);

		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		static bool	changeOwner(const char *filename,
						const char *newuser,
						const char *newgroup);

		/** Changes the user and/or group that owns the file.
		 *  Returns true on success and false on failure. */
		static bool	changeOwner(const char *filename,
						uid_t uid, gid_t gid);

		/** Returns true if any of the changeOwner methods are
 		 *  allowed on "filename" and false otherwise. */
		static bool	getCanChangeOwner(const char *filename);


		/** Overrides the last access time of the file, setting it to
		 *  "lastaccesstime".  Returns true on success and false on
		 *  failure. */
		static bool	setLastAccessTime(const char *filename,
							time_t lastaccesstime);

		/** Overrides the last modification time of the file, setting
		 *  it to "lastaccesstime".  Returns true on success and false
		 *  on failure. */
		static bool	setLastModificationTime(const char *filename,
							time_t lastmodtime);

		/** Overrides the last access and modification times of the
		 *  file, setting them to "lastaccesstime" and "lastmodtime"
		 *  respectively.  Returns true on success and false on
		 *  failure. */
		static bool	setLastAccessAndModificationTimes(
							const char *filename,
							time_t lastaccesstime,
							time_t lastmodtime);

		/** Overrides the last access and modification times of the
		 *  file, setting them both to the current system time. 
		 * Returns true on success and false on failure. */
		static bool	setLastAccessAndModificationTimes(
							const char *filename);


		/** Returns the directory portion of "filename".
		 *  This method allocates a buffer internally
		 *  and returns it.  The calling program must
		 *  deallocate the buffer. */
		static char	*getDirName(const char *filename);

		/** Returns the non-directory portion of "filename".
		 *  This method allocates a buffer internally and returns it.
		 *  The calling program must deallocate the buffer. */
		static char	*getBaseName(const char *filename);

		/** Returns the non-directory portion of
		 *  "filename", truncating "ext".
		 *  This method allocates a buffer internally
		 *  and returns it.  The calling program must
		 *  deallocate the buffer. */
		static char	*getBaseName(const char *filename,
						const char *ext);

		/** Returns the portion of "filename" after the last dot,
 		 *  including the dot, or an empty string if the filename
 		 *  contains no dot.  The output should be suitable to be
 		 *  passed in as the "ext" parameter when calling basename().*/
		static char	*getExtension(const char *filename);

		/** Translates the basename of "filename" to an 8.3 format,
		 *  performing substitutions in the same way that Windows
		 *  does.  This method allocates a buffer internally and
		 *  returns it.  The calling program must deallocate the
		 *  buffer. */
		static char	*getEightDotThree(const char *filename);


		/** Generates a key based on "filename" and the
		 *  least signifigant 8 bits of id (which must
		 *  be non-zero) suitable for use with
		 *  semaphores, shared memory segments and
		 *  message queues. */
		static key_t	generateKey(const char *filename, int32_t id);

		/** Returns the maximum number of links that
		 *  can be created to "filename". */
		static int64_t	getMaxLinks(const char *filename);

		/** Populates "matches" with the set of files whos names match
 		 *  "pattern".
 		 *
 		 *  Storage for the strings appended to "matches" is allocated
 		 *  internally and must be deleted by the calling program.
 		 *
 		 *  Returns true on success or false on failure.
 		 *
 		 *  Note that on failure, "matches" may be partially
 		 *  populated. */
		static bool	getMatchingFileNames(
					const char *pattern,
					linkedlist<char *> *matches);

		/** Populates "matches" with the set of files whos names match
 		 *  any of the patterns provided in the NULL-terminated array
 		 *  "pattern".
 		 *
 		 *  Storage for the strings appended to "matches" is allocated
 		 *  internally and must be deleted by the calling program.
 		 *
 		 *  Returns true on success or false on failure.
 		 *
 		 *  Note that on failure, "matches" may be partially
 		 *  populated. */
		static bool	getMatchingFileNames(
					const char * const *patterns,
					linkedlist<char *> *matches);

		/** Populates "matches" with the set of files whos names match
 		 *  any of the patterns provided in the linked list "pattern".
 		 *
 		 *  Storage for the strings appended to "matches" is allocated
 		 *  internally and must be deleted by the calling program.
 		 *
 		 *  Returns true on success or false on failure.
 		 *
 		 *  Note that on failure, "matches" may be partially
 		 *  populated. */
		static bool	getMatchingFileNames(
					linkedlist<const char *> *patterns,
					linkedlist<char *> *matches);

		/** Populates "matches" with the set of files whos names match
 		 *  any of the patterns provided in the linked list "pattern".
 		 *
 		 *  Storage for the strings appended to "matches" is allocated
 		 *  internally and must be deleted by the calling program.
 		 *
 		 *  Returns true on success or false on failure.
 		 *
 		 *  Note that on failure, "matches" may be partially
 		 *  populated. */
		static bool	getMatchingFileNames(
					linkedlist<char *> *patterns,
					linkedlist<char *> *matches);

	#include <rudiments/private/file.h>
};

#endif
