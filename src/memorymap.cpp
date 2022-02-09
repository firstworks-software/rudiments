// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/memorymap.h>
#include <rudiments/sys.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#if defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
	#include <rudiments/filedescriptor.h>
#endif
#include <rudiments/sys.h>

#include <stdio.h>
#ifdef RUDIMENTS_HAVE_SYS_MMAN_H
	#include <sys/mman.h>
#endif

#ifdef RUDIMENTS_HAVE_WINDOWS_H
	#include <windows.h>
#endif
#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif

#ifdef RUDIMENTS_HAVE_MUNMAP_CADDR_T
	#define MUNMAP_ADDRCAST caddr_t
#else
	#define MUNMAP_ADDRCAST void *
#endif

#ifdef RUDIMENTS_HAVE_MSYNC_CADDR_T
	#define MSYNC_ADDRCAST caddr_t
#else
	#define MSYNC_ADDRCAST void *
#endif

#ifndef MAP_FAILED
	#define MAP_FAILED ((void *)-1)
#endif

class memorymapprivate {
	friend class memorymap;
	private:
		void	*_data;
		size_t	_size;
		#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
			HANDLE	_map;
		#endif
};

memorymap::memorymap() : object() {
	pvt=new memorymapprivate;
	pvt->_data=NULL;
	pvt->_size=0;
	#ifdef RUDIMENTS_HAVE_CREATE_FILE_MAPPING
		pvt->_map=NULL;
	#endif
}

memorymap::~memorymap() {
	detach();
	delete pvt;
}

bool memorymap::attach(int32_t fd, off64_t offset, size_t size,
					int32_t protection, int32_t flags) {
	pvt->_size=size;
	#if defined(RUDIMENTS_HAVE_MMAP)
		error::clearError();
		do {
			pvt->_data=mmap(NULL,size,protection,flags,fd,offset);
		} while (pvt->_data==(void *)MAP_FAILED &&
				error::getErrorNumber()==EINTR &&
				!process::getShutDownFlag());
		return (pvt->_data!=(void *)MAP_FAILED);
	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)

		// calculate max mapping size and offset
		DWORD	offsethigh=(((uint64_t)offset)>>32);
		DWORD	offsetlow=(((uint64_t)offset)&0x00000000FFFFFFFF);

		// determine map and view protection
		DWORD	mapprot=0;
		DWORD	viewprot=0;
		DWORD	viewprotwrite=(flags&MAP_PRIVATE)?
					FILE_MAP_COPY:FILE_MAP_WRITE;
		if (protection&PROT_EXEC) {
			if (protection&PROT_READ) {
				if (protection&PROT_WRITE) {
					mapprot=PAGE_EXECUTE_READWRITE;
					viewprot=viewprotwrite|
						#ifdef FILE_MAP_EXECUTE
							FILE_MAP_EXECUTE;
						#else
							SECTION_MAP_EXECUTE;
						#endif
				} else {
					mapprot=PAGE_EXECUTE_READ;
					viewprot=FILE_MAP_READ|
						#ifdef FILE_MAP_EXECUTE
							FILE_MAP_EXECUTE;
						#else
							SECTION_MAP_EXECUTE;
						#endif
				}
			}
		} else if (protection&PROT_READ) {
			if (protection&PROT_WRITE) {
				mapprot=PAGE_READWRITE;
				viewprot=viewprotwrite;
			} else {
				mapprot=PAGE_READONLY;
				viewprot=FILE_MAP_READ;
			}
		}

		// create the file mapping
		pvt->_map=CreateFileMapping((HANDLE)filedescriptor::
						getHandleFromFileDescriptor(fd),
						NULL,mapprot,
						0,0,
						NULL);
		if (!pvt->_map) {
			return false;
		}

		// create a view of the file mapping
		pvt->_data=MapViewOfFile(pvt->_map,
						viewprot,
						offsethigh,offsetlow,
						size);
		if (!pvt->_data) {
			CloseHandle(pvt->_map);
			return false;
		}
		return true;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool memorymap::detach() {
	#if defined(RUDIMENTS_HAVE_MMAP) || \
		defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		#if defined(RUDIMENTS_HAVE_MMAP)
			int32_t	result;
			error::clearError();
			do {
				result=munmap(
					reinterpret_cast<MUNMAP_ADDRCAST>
					(pvt->_data),pvt->_size);
			} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
			bool	retval=!result;
		#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
			bool	retval=(UnmapViewOfFile(pvt->_data)==TRUE &&
						CloseHandle(pvt->_map)==TRUE);
		#endif
		pvt->_data=NULL;
		pvt->_size=0;
		return retval;
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

void *memorymap::getData() {
	return pvt->_data;
}

size_t memorymap::getSize() {
	return pvt->_size;
}

bool memorymap::sync(bool immediate, bool invalidate) {
	return sync(0,pvt->_size,immediate,invalidate);
}

bool memorymap::sync(off64_t offset, size_t size,
			bool immediate, bool invalidate) {
	#ifdef RUDIMENTS_HAVE_MSYNC
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		int32_t	result;
		error::clearError();
		do {
			result=msync(reinterpret_cast<MSYNC_ADDRCAST>(ptr),size,
					((immediate)?MS_SYNC:MS_ASYNC)|
						((invalidate)?MS_INVALIDATE:0));
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		return !result;
	#elif defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		unsigned char	*ptr=
			(static_cast<unsigned char *>(pvt->_data))+offset;
		return (FlushViewOfFile(
			reinterpret_cast<void *>(ptr),size)==TRUE);
	#else
		RUDIMENTS_SET_ENOSYS
		return true;
	#endif
}

bool memorymap::supported() {
	#if defined(_SYLLABLE)
		// Syllable has mmap, but it appears that it can only map
		// from byte-0 of the file.  Attempts to map further into
		// the file just result in re-mapping at byte-0.
		return false;
	#elif defined(RUDIMENTS_HAVE_MMAP) || \
		defined(RUDIMENTS_HAVE_CREATE_FILE_MAPPING)
		return true;
	#else
		return false;
	#endif
}
