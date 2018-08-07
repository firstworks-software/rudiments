// Copyright (c) 2004 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_MEMORYMAP_H
#define RUDIMENTS_MEMORYMAP_H

#include <rudiments/private/memorymapincludes.h>

/** The memorymap class provides methods for mapping a file (or file descriptor)
 *  to a region of memory and manipulating the mapped region. */
class RUDIMENTS_DLLSPEC memorymap {
	public:
		
		/** Creates an instance of the memorymap class. */
		memorymap();

		/** Deletes this instance of the memorymap class, calling
 		 *  detach() if necessary. */
		~memorymap();

		/** Attaches the memorymap to file descriptor "fd" at
		 *  "offset" for "len" bytes.
		 * 
		 *  "protection" may be:
		 * 	PROT_NONE - pages may not be accessed
		 *  or an or'ed combination of the following:
		 *  	PROT_EXEC - pages may be excuted
		 * 	PROT_READ - pages may be read
		 * 	PROT_WRITE - pages may be written
		 *  "protection" may not conflict with the open mode
		 *  of the file. (eg. if the file was opened readonly
		 *  then PROT_WRITE may not be used).
		 * 
		 *  flags may be:
		 * 	MAP_SHARED
		 * 	or
		 * 	MAP_PRIVATE
		 *  and an or'ed combination of the following:
		 * 	MAP_FIXED 
		 *  	MAP_DENYWRITE
		 *  	MAP_EXECUTABLE
		 *  	MAP_NORESERVE
		 * 	MAP_LOCKED
		 * 	MAP_GROWSDOWN
		 * 	MAP_ANONYMOUS
		 * 	MAP_ANON
		 * 	MAP_32BIT
		 * 	MAP_AUTOGROW
		 * 	MAP_AUTORESRV
		 * 	MAP_COPY
		 * 	MAP_LOCAL
		 *
		 *  Note that not all filesystems support all of those options.
		 * 
		 *  Returns true on success and false on failure. */
		bool	attach(int32_t fd, off64_t offset, size_t len,
					int32_t protection, int32_t flags);

		/** detaches the memory map from the file descriptor
		 * 
		 *  Returns true on success and false on failure. */
		bool	detach();

		/** Returns a pointer to the region of memory that the
		 *  file is mapped into. */
		void	*getData();

		/** Returns the length of the region of memory that the
		 *  file is mapped into. */
		size_t	getLength();

		/** Make sure that changes made to the memory map have
		 *  been copied to the storage mediam that the mapped
		 *  file resides on.
		 * 
		 *  If "immediate" is true, the method will not return
		 *  until the synchronization has finished, if it is
		 *  false, the synchronization will occur in the
		 *  background.
		 * 
		 *  If "invalidate" is true, all other mappings of the
		 *  file will be invalidated.
		 * 
		 *  Returns true on success and false on failure. */
		bool	sync(bool immediate, bool invalidate);

		/** Make sure that changes made to the memory map for
		 *  "len" bytes, starting at "offset" have been copied
		 *  to the storage mediam that the mapped file resides
		 *  on.
		 * 
		 *  If "immediate" is true, the method will not return
		 *  until the synchronization has finished, if it is
		 *  false, the synchronization will occur in the
		 *  background.
		 * 
		 *  If "invalidate" is true, all other mappings of the
		 *  file will be invalidated.
		 * 
		 *  Returns true on success and false on failure. */
		bool	sync(off64_t offset, size_t len,
				bool immediate, bool invalidate);


		/** Returns true if the platform supports
		 *  memory mapping and false otherwise. */
		static bool	supported();

	#include <rudiments/private/memorymap.h>
};

#endif
