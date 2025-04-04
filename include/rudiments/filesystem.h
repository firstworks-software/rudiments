// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_FILESYSTEM_H
#define RUDIMENTS_FILESYSTEM_H

#include <rudiments/private/filesystemincludes.h>

/** The filesystem class provides methods for discovering the properties
 *  associated with a filesystem such as type, total and free blocks, total and
 *  free inodes, etc.
 *
 *  The native methods for accessing filesystem data vary greatly between
 *  platforms and many of these methods only work at all on certain platforms.
 *  */
class RUDIMENTS_DLLSPEC filesystem : virtual public object {
	public:
		/** Creates an instance of the filesystem class. */
		filesystem();

		/** Deletes this instance of the filesystem class. */
		virtual		~filesystem();

		/** Opens the filesystem containing file or directory "path".
		 *  If "path" is NULL or an empty string then the instance
		 *  is initialized using the current working directoy.
		 *
		 *  Returns true on success and false on failure. */
		bool	open(const char *path);

		/** Initializes the instance to use already open file
		 *  descriptor "fd".
		 *
		 *  Returns true on success and false on failure. */
		bool	open(int32_t fd);

		/** Closes the currently open filesystem. */
		bool	close();

		/** Refreshes the current filesystem properties as returned
		 *  by the various methods of this class.
		 *  Returns true on success and false on failure. */
		bool	getCurrentProperties();

		/** Returns a number representing the filesystem type. */
		int64_t		getType();

		/** Returns the fundamental block size of the filesystem. */
		int64_t		getBlockSize();

		/** Returns the optimum transfer block size. */
		int64_t		getOptimumTransferBlockSize();

		/** Returns the optimum transfer block size for the filesystem
		 *  containing file or directory "path".  If "path" is NULL or
		 *  an empty string then the current working directory is used.
		 *  Returns sys::getPageSize() if "path" cannot be opened. */
		static int64_t	getOptimumTransferBlockSize(const char *path);

		/** Returns the total number of blocks allocated for the
		 *  filesystem. */
		int64_t		getTotalBlocks();

		/** Returns the number of free blocks in the filesystem. */
		int64_t		getFreeBlocks();

		/** Returns the number of blocks available to non-superuser
		 *  accounts on the filesystem. */
		int64_t		getAvailableBlocks();

		/** Returns the number of blocks reserved for superuser
		 *  accounts on the filesystem. */
		int64_t		getReservedBlocks();

		/** Returns the number of file nodes on the filesystem. */
		int64_t		getTotalFileNodes();

		/** Returns the number of free file nodes. */
		int64_t		getFreeFileNodes();

		/** Returns the number of file nodes available to 
		 *  non-superuser accounts. */
		int64_t		getAvailableFileNodes();

		/** Returns the number of file nodes reserved for 
		 *  superuser accounts. */
		int64_t		getReservedFileNodes();

		/** Returns the file system id. */
		int64_t		getFileSystemId();

		/** Returns the maximum character length of filenames on the
		 *  filesystem. */
		int64_t		getMaximumFileNameLength();

		/** Returns the id of the user that mounted the filesystem. */
		uid_t		getOwner();

		/** Returns the number of synchronous writes that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getSyncWrites(); 

		/** Returns the number of asynchronous writes that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getAsyncWrites();

		/** Returns the name of the filesystem type. */
		const char	*getTypeName();

		/** Returns the mount point of the filesystem. */
		const char	*getMountPoint();

		/** Returns the number of synchronous reads that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getSyncReads();

		/** Returns the number of asynchronous reads that
		 *  have occurred since the filesytem was mounted. */
		int64_t		getAsyncReads();

		/** Returns the name of the device file for the filesystem. */
		const char	*getDeviceName();

		/** Returns the filesystem-specific string. */
		const char	*getFilesystemSpecificString();

		/** Returns a pointer to the filesystem stats structure
		 *  used internally.  (This is VERY system-specific and could
		 *  be struct statfs *, struct statvfs * or something else.) */
		void	*getInternalFilesystemStatisticsStructure();

	#include <rudiments/private/filesystem.h>
};

#endif
