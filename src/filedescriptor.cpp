// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

//#define DEBUG_PASSFD 1
//#define DEBUG_WRITE 1
//#define DEBUG_READ 1
//#define DEBUG_BUFFERING 1

#if defined(DEBUG_PASSFD) || defined(DEBUG_WRITE) || \
	defined(DEBUG_READ) || defined(DEBUG_BUFFERING)
#ifdef _MSC_VER
	#define debugPrintf(ARGS,...) if (this!=&stdoutput) { stdoutput.printf(ARGS,__VA_ARGS__); }
#else
	#define debugPrintf(ARGS...) if (this!=&stdoutput) { stdoutput.printf(ARGS); }
#endif
#define debugSafePrint(string) if (this!=&stdoutput) { stdoutput.safePrint(string); }
#endif

#include <rudiments/filedescriptor.h>
#include <rudiments/listener.h>
#include <rudiments/charstring.h>
#include <rudiments/character.h>
#include <rudiments/bytestring.h>
#include <rudiments/stringbuffer.h>
#include <rudiments/thread.h>
#include <rudiments/semaphoreset.h>
#include <rudiments/file.h>
#include <rudiments/memorymap.h>
#include <rudiments/permissions.h>
#include <rudiments/sys.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_IO_H
	#include <io.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_TIME_H
	#include <sys/time.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif
#ifdef RUDIMENTS_HAVE_FCNTL_H
	#include <fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_FCNTL_H
	#include <sys/fcntl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif

#include <stdio.h>
#if defined(RUDIMENTS_HAVE_VASPRINTF) && defined(RUDIMENTS_HAVE_STDLIB_H)
	#include <stdlib.h>
#endif
#ifdef RUDIMENTS_HAVE_WCHAR_H
	#include <wchar.h>
#endif

// NOTE: These next two headers must be included in this order or LITTLE_ENDIAN
// will be multiply-defined on linux libc4 systems.  Other systems are
// unaffected.
#ifdef RUDIMENTS_HAVE_NETINET_TCP_H
	// some libc5 systems need this extern "C" wrapper
	extern "C" {
	#include <netinet/tcp.h>
	}
#endif
#ifdef RUDIMENTS_HAVE_NETINET_IN_H
	#include <netinet/in.h>
#endif


#ifdef RUDIMENTS_HAVE_SYS_UIO_H
	#include <sys/uio.h>
#endif
#ifdef RUDIMENTS_HAVE_LIMITS_H
	#include <limits.h>
#endif
#ifdef RUDIMENTS_HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#endif
#ifdef RUDIMENTS_HAVE_BYTESWAP_H
	#ifdef RUDIMENTS_HAVE_SAFE_BYTESWAP_H_AFTER_NETINET_IN_H
		#include <byteswap.h>
	#endif
#endif
#ifdef RUDIMENTS_HAVE_MACHINE_ENDIAN_H
	#include <machine/endian.h>
#endif
#ifdef RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64
	#include <libkern/OSByteOrder.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_BYTEORDER_H
	#include <sys/byteorder.h>
#endif
#ifdef RUDIMENTS_HAVE_OS_SUPPORT_BYTEORDER_H
	#include <os/support/ByteOrder.h>
#endif

// apparently on windows, there are no byte-order macros of any kind
#ifdef _WIN32
	#define __LITTLE_ENDIAN	1234
	#define __BIG_ENDIAN	4321
	#define __BYTE_ORDER	__LITTLE_ENDIAN
#endif

// On solaris (and probably others), BYTE_ORDER is undefined (even with
// underscore prefixes.  Either _BIG_ENDIAN or _LITTLE_ENDIAN is
// defined, but it's just "defined", not set to any value.
#if !defined(__BYTE_ORDER) && \
	!defined(_BYTE_ORDER) && \
	!defined(BYTE_ORDER) && \
	(defined(_BIG_ENDIAN) || defined(_LITTLE_ENDIAN))

	#define __LITTLE_ENDIAN 1234
	#define __BIG_ENDIAN	4321

	#ifdef _LITTLE_ENDIAN
		#define __BYTE_ORDER	__LITTLE_ENDIAN
	#else
		#define __BYTE_ORDER	__BIG_ENDIAN
	#endif
#endif

#ifndef __BYTE_ORDER
	#if defined(BYTE_ORDER)
		#define __BYTE_ORDER BYTE_ORDER
	#elif defined (_BYTE_ORDER)
		#define __BYTE_ORDER _BYTE_ORDER
	#endif
#endif

#ifndef __BIG_ENDIAN
	#if defined(BIG_ENDIAN)
		#define __BIG_ENDIAN BIG_ENDIAN
	#elif defined(_BIG_ENDIAN)
		#define __BIG_ENDIAN _BIG_ENDIAN
	#endif
#endif

#ifndef __LITTLE_ENDIAN
	#if defined(LITTLE_ENDIAN)
		#define __LITTLE_ENDIAN LITTLE_ENDIAN
	#elif defined(_LITTLE_ENDIAN)
		#define __LITTLE_ENDIAN _LITTLE_ENDIAN
	#endif
#endif

// for FD_SET (macro that uses memset) on solaris
#ifdef RUDIMENTS_HAVE_STRING_H
	#include <string.h>
#endif

#ifdef RUDIMENTS_NEED_XNET_PROTOTYPES
extern ssize_t __xnet_recvmsg (int, struct msghdr *, int);
extern ssize_t __xnet_sendmsg (int, const struct msghdr *, int);
#endif

// some platforms (solaris <= 9) don't have these macros
#ifndef CMSG_LEN
	#ifndef CMSG_ALIGN
		#define CMSG_ALIGN(len)	\
				(((len) + sizeof(size_t)-1) & \
				(size_t)~(sizeof(size_t)-1))
	#endif
	#define CMSG_LEN(len)	(CMSG_ALIGN(sizeof(struct cmsghdr))+(len))
#endif

// SCO OpenServer < 5.0.7 has an error in the sys/socket.h header.
// Internally, libc supports accrights/accrightslen but the header defines
// the struct as having control/controllen components.  This hack works
// around the problem.
// http://www.linuxmisc.com/9-unix-programmer/af8e2f1e03a2b913.htm
#ifdef RUDIMENTS_HAVE_BAD_SCO_MSGHDR
	#undef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
	#define RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS 1
	#define msg_accrights msg_control
	#define msg_accrightslen msg_controllen
#endif

// if SSIZE_MAX is undefined...
#ifndef SSIZE_MAX
	#if defined(_WIN32)
		#define SSIZE_MAX LONG_MAX
	#else
		// a good safe value that should even work on 16-bit systems
		#define SSIZE_MAX 16383
	#endif
#endif

// most platforms FILE struct have a member for the file descriptor,
// try to find it...
// some platforms, like solaris 11.2 hide it altogether
#undef FD
#if defined(RUDIMENTS_HAVE_FILE_FILENO)
	#define FD f->_fileno
#elif defined(RUDIMENTS_HAVE_FILE_FILE)
	#ifdef __VMS
		#define FD ((struct _iobuf *)f)->_file
	#else
		#define FD f->_file
	#endif
#elif defined(RUDIMENTS_HAVE_FILE__FILE)
	#define FD f->__file
#elif defined(RUDIMENTS_HAVE_FILE_FILEDES)
	#define FD f->__filedes
#elif defined(RUDIMENTS_HAVE_FILE__FD)
	#define FD f->_fd
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_SENDMSG
extern "C" ssize_t sendmsg(int, const struct msghdr *,int);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_RECVMSG
extern "C" ssize_t recvmsg(int, struct msghdr *,int);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_GETPEERNAME
extern "C" int getpeername(int, struct sockaddr *,socklen_t *);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_GETSOCKOPT
extern "C" int getsockopt(int, int, int, void *, socklen_t *);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_SETSOCKOPT
extern "C" int setsockopt(int, int, int, const void *, socklen_t);
#endif

class filedescriptorprivate {
	friend class filedescriptor;
	private:
		bool	_retryinterruptedreads:1;
		bool	_retryinterruptedwrites:1;
		bool	_retryinterruptedwaits:1;
		bool	_retryinterruptedfcntl:1;
		bool	_retryinterruptedioctl:1;
		bool	_allowshortreads:1;
		bool	_allowshortwrites:1;
		bool	_translatebyteorder:1;

		int32_t	_fd;

		socketlayer	*_socklr;

		const char	*_type;

		listener	*_lstnr;

		bool		_isstream;
		off64_t		_offset;
		off64_t		_writeblockoffset;
		ssize_t		_writeblocksize;
		ssize_t 	(filedescriptor::*_readPtr)(
						unsigned char *,ssize_t,
						int32_t,int32_t);
		ssize_t 	(filedescriptor::*_writePtr)(
						const unsigned char *,ssize_t,
						int32_t,int32_t);

		memorymap	*_writebuffermap;
		unsigned char	*_writebufferunaligned;
		unsigned char	*_writebuffer;
		unsigned char	*_writebufferhead;
		unsigned char	*_writebuffertail;
		unsigned char	*_writebufferend;
		ssize_t		_writebufferreadavail;
		ssize_t		_writebufferwriteavail;
		bool		_writebufferdirty;
		bool		_writebuffermmapenabled;

		unsigned char	*_readbuffer;
		unsigned char	*_readbufferhead;
		unsigned char	*_readbuffertail;
		unsigned char	*_readbufferend;
};

filedescriptor::filedescriptor() : input(), output() {
	pvt=new filedescriptorprivate;
	filedescriptorInit();
}

filedescriptor::filedescriptor(int32_t fd) : input(), output() {
	pvt=new filedescriptorprivate;
	filedescriptorInit();
	setFileDescriptor(fd);
}

filedescriptor::filedescriptor(const filedescriptor &f) : input(f), output(f) {
	pvt=new filedescriptorprivate;
	filedescriptorClone(f);
}

filedescriptor &filedescriptor::operator=(const filedescriptor &f) {
	if (this!=&f) {
		if (pvt->_writebuffermap) {
			delete pvt->_writebuffermap;
		} else {
			delete[] pvt->_writebufferunaligned;
		}
		filedescriptorClone(f);
	}
	return *this;
}

void filedescriptor::filedescriptorInit() {
	setFileDescriptor(-1);
	pvt->_retryinterruptedreads=false;
	pvt->_retryinterruptedwrites=false;
	pvt->_retryinterruptedwaits=true;
	pvt->_retryinterruptedfcntl=true;
	pvt->_retryinterruptedioctl=true;
	pvt->_allowshortreads=false;
	pvt->_allowshortwrites=false;
	pvt->_translatebyteorder=false;
	pvt->_socklr=NULL;
	pvt->_type="filedescriptor";
	pvt->_lstnr=NULL;
	pvt->_offset=0;
	pvt->_writeblockoffset=0;
	pvt->_writeblocksize=0;
	pvt->_isstream=true;
	pvt->_readPtr=&filedescriptor::unBufferedRead;
	pvt->_writePtr=&filedescriptor::unBufferedWrite;
	pvt->_writebuffermap=NULL;
	pvt->_writebufferunaligned=NULL;
	pvt->_writebuffer=NULL;
	pvt->_writebufferhead=NULL;
	pvt->_writebuffertail=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferreadavail=0;
	pvt->_writebufferwriteavail=0;
	pvt->_writebufferdirty=false;
	pvt->_writebuffermmapenabled=false;
	pvt->_readbuffer=NULL;
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
	pvt->_readbufferend=NULL;
}

void filedescriptor::filedescriptorClone(const filedescriptor &f) {
	setFileDescriptor(f.pvt->_fd);
	pvt->_retryinterruptedreads=f.pvt->_retryinterruptedreads;
	pvt->_retryinterruptedwrites=f.pvt->_retryinterruptedwrites;
	pvt->_retryinterruptedwaits=f.pvt->_retryinterruptedwaits;
	pvt->_retryinterruptedfcntl=f.pvt->_retryinterruptedfcntl;
	pvt->_retryinterruptedioctl=f.pvt->_retryinterruptedioctl;
	pvt->_allowshortreads=f.pvt->_allowshortreads;
	pvt->_allowshortwrites=f.pvt->_allowshortwrites;
	pvt->_translatebyteorder=f.pvt->_translatebyteorder;
	pvt->_socklr=f.pvt->_socklr;
	pvt->_type="filedescriptor";
	pvt->_lstnr=NULL;
	pvt->_offset=f.pvt->_offset;
	pvt->_writeblockoffset=f.pvt->_writeblockoffset;
	pvt->_writeblocksize=f.pvt->_writeblocksize;
	pvt->_isstream=f.pvt->_isstream;
	// FIXME: clone function pointers
	pvt->_readPtr=&filedescriptor::unBufferedRead;
	pvt->_writePtr=&filedescriptor::unBufferedWrite;
	// FIXME: clone buffere
	pvt->_writebuffermap=NULL;
	pvt->_writebufferunaligned=NULL;
	pvt->_writebuffer=NULL;
	pvt->_writebufferhead=NULL;
	pvt->_writebuffertail=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferreadavail=0;
	pvt->_writebufferwriteavail=0;
	pvt->_writebufferdirty=false;
	pvt->_writebuffermmapenabled=f.pvt->_writebuffermmapenabled;
	pvt->_readbuffer=NULL;
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
}

filedescriptor::~filedescriptor() {

	// see NOTE in ~threadmutex()

	if (!pvt) {
		return;
	}

	// clean up read buffer
	unsigned char	*tmpbuffer=pvt->_readbuffer;
	pvt->_readbuffer=NULL;
	delete[] tmpbuffer;

	// clean up write buffer
	if (pvt->_writebuffermap) {
		memorymap	*tmpmap=pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
		delete[] tmpmap;
	} else {
		tmpbuffer=pvt->_writebufferunaligned;
		pvt->_writebufferunaligned=NULL;
		delete[] tmpbuffer;
	}

	// clean up listener
	listener	*tmplstnr=pvt->_lstnr;
	pvt->_lstnr=NULL;
	delete tmplstnr;

	// close
	close();

	// clean up pvt
	filedescriptorprivate	*tmppvt=pvt;
	pvt=NULL;
	delete tmppvt;
}

bool filedescriptor::setWriteBufferSize(ssize_t size) const {

	// finagle size
	if (size>SSIZE_MAX) {
		size=SSIZE_MAX;
	}
	if (size<0) {
		size=0;
	}

	// set/unset stream/storage buffers, as appropriate
	return (size)?
		((pvt->_isstream)?
			setStreamWriteBufferSize(size):
			setStorageWriteBufferSize(size)):
		((pvt->_isstream)?
			unsetStreamWriteBuffer():
			unsetStorageWriteBuffer());
}

bool filedescriptor::setStreamWriteBufferSize(ssize_t size) const {

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: setStreamWriteBufferSize(%d,attempting %d bytes",
			(int)process::getProcessId(),(int)pvt->_fd,(int)size);
	#endif

	// invalidate stuff that's only used
	// when buffering storage
	pvt->_offset=0;
	pvt->_writeblockoffset=0;

	// update write block size
	pvt->_writeblocksize=size;

	// clean up
	if (pvt->_writebuffermap) {
		delete pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
	} else {
		delete[] pvt->_writebufferunaligned;
	}

	// create a new buffer
	allocateWriteBuffer(size);

	// update the write buffer pointers and counts
	pvt->_writebufferhead=pvt->_writebuffer;
	pvt->_writebuffertail=pvt->_writebuffer;
	pvt->_writebufferend=pvt->_writebuffer+size;
	pvt->_writebufferreadavail=size;
	pvt->_writebufferwriteavail=size;

	// mark the write buffer not dirty
	pvt->_writebufferdirty=false;

	// set the write method to use
	pvt->_writePtr=&filedescriptor::streamBufferedWrite;

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

bool filedescriptor::setStorageWriteBufferSize(ssize_t size) const {

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: setStorageWriteBufferSize(%d,attempting %d bytes",
			(int)process::getProcessId(),(int)pvt->_fd,(int)size);
	#endif

	// figure out the current offset...
	//
	// If we haven't been buffering then we need to set pvt->_offset from
	// the current position in the file.
	//
	// If we have been buffering then pvt->_offset ought to already be
	// valid.
	if (!pvt->_writebuffer) {
		off64_t	pos=getCurrentPosition();
		if (pos==-1) {
			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf(",error: lseek failed)\n");
			#endif
			return false;
		}
		pvt->_offset=pos;
	}

	// update write block offset and write block size
	pvt->_writeblockoffset=pvt->_offset/size*size;
	pvt->_writeblocksize=size;

	// clean up but don't create a new write buffer
	// the new buffer will be created during the first call to
	// realignWriteBuffer()
	if (pvt->_writebuffermap) {
		delete pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
	} else {
		delete[] pvt->_writebufferunaligned;
		pvt->_writebufferunaligned=NULL;
	}

	// update the write buffer pointers and counts
	pvt->_writebuffer=NULL;
	pvt->_writebufferhead=NULL;
	pvt->_writebuffertail=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferreadavail=0;
	pvt->_writebufferwriteavail=0;

	// mark the write buffer not dirty
	pvt->_writebufferdirty=false;

	// set the read and write methods to use
	// (for storage, if we're buffering writes
	// then we have to buffer reads too)
	pvt->_readPtr=&filedescriptor::storageBufferedRead;
	pvt->_writePtr=&filedescriptor::storageBufferedWrite;

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

bool filedescriptor::unsetStreamWriteBuffer() const {

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: unsetStreamWriteBuffer(%d,",
			(int)process::getProcessId(),(int)pvt->_fd);
	#endif

	// invalidate stuff that's only used
	// when buffering storage
	pvt->_offset=0;
	pvt->_writeblockoffset=0;

	// update write block size
	pvt->_writeblocksize=0;

	// clean up
	if (pvt->_writebuffermap) {
		delete pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
	} else {
		delete[] pvt->_writebufferunaligned;
		pvt->_writebufferunaligned=NULL;
	}

	// update the write buffer pointers and counts
	pvt->_writebuffer=NULL;
	pvt->_writebufferhead=NULL;
	pvt->_writebuffertail=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferreadavail=0;
	pvt->_writebufferwriteavail=0;

	// mark the write buffer not dirty
	pvt->_writebufferdirty=false;

	// set the write method to use
	pvt->_writePtr=&filedescriptor::unBufferedWrite;

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

bool filedescriptor::unsetStorageWriteBuffer() const {

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: unsetStorageWriteBuffer(%d,",
			(int)process::getProcessId(),(int)pvt->_fd);
	#endif

	// If we were buffering previously then the current file position is
	// probably pvt->_writeblockoffset.  Go to pvt->_offset instead.
	if (pvt->_writebuffer) {
		if (lseek(pvt->_offset,SEEK_SET)!=pvt->_offset) {
			#if defined(DEBUG_BUFFERING)
			debugPrintf("lseek failed)\n");
			#endif
			return false;
		}
	}

	// update write block offset and write block size
	pvt->_writeblockoffset=0;
	pvt->_writeblocksize=0;

	// clean up
	if (pvt->_writebuffermap) {
		delete pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
	} else {
		delete[] pvt->_writebufferunaligned;
		pvt->_writebufferunaligned=NULL;
	}

	// update the write buffer pointers and counts
	pvt->_writebuffer=NULL;
	pvt->_writebufferhead=NULL;
	pvt->_writebuffertail=NULL;
	pvt->_writebufferend=NULL;
	pvt->_writebufferreadavail=0;
	pvt->_writebufferwriteavail=0;

	// mark the write buffer not dirty
	pvt->_writebufferdirty=false;

	// set the read and write methods to use
	// (for storage, if we're not buffering writes then we
	// can't buffer reads either)
	pvt->_readPtr=&filedescriptor::unBufferedRead;
	pvt->_writePtr=&filedescriptor::unBufferedWrite;

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

void filedescriptor::allocateWriteBuffer(ssize_t size) const {

	// If the file was opened with O_DIRECT then we can only copy data to
	// an address that is aligned to a multiple of the fllesystem's block
	// size.  We'll assume that "size" is a multiple of the filesystem's
	// block size and align to that.

	// Allocate pvt->_writebufferunaligned with enough space that a pointer
	// inside of it can be aligned to an address that is a multiple of
	// "size".
	ssize_t	extra=size-1;
	pvt->_writebufferunaligned=new unsigned char[size+extra];

	// Set pvt->_writebuffer to the first address that is a multiple of
	// "size" within pvt->_writebufferunaligned.
	pvt->_writebuffer=(unsigned char *)
		((((ssize_t)pvt->_writebufferunaligned)+extra)&~extra);
}

ssize_t filedescriptor::getWriteBufferSize() const {
	return pvt->_writeblocksize;
}

bool filedescriptor::setReadBufferSize(ssize_t size) const {

	// finagle size
	if (size>SSIZE_MAX) {
		size=SSIZE_MAX;
	}
	if (size<0) {
		size=0;
	}

	// set/unset stream/storage buffers, as appropriate
	return (size)?
		((pvt->_isstream)?
			setStreamReadBufferSize(size):
			setStorageReadBufferSize(size)):
		((pvt->_isstream)?
			unsetStreamReadBuffer():
			unsetStorageReadBuffer());
}

bool filedescriptor::setStreamReadBufferSize(ssize_t size) const {

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: setStreamReadBufferSize(%d,attempting %d bytes",
			(int)process::getProcessId(),(int)pvt->_fd,(int)size);
	#endif

	// clean up and create a new buffer
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=new unsigned char[size];

	// update the read buffer pointers
	pvt->_readbufferhead=pvt->_readbuffer;
	pvt->_readbuffertail=pvt->_readbuffer;
	pvt->_readbufferend=pvt->_readbuffer+size;

	// set the read method to use
	pvt->_readPtr=&filedescriptor::streamBufferedRead;

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

bool filedescriptor::setStorageReadBufferSize(ssize_t size) const {

	// for storage filedescriptors, we only
	// use one buffer, the write buffer

	// clean up but don't create a new read buffer
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=NULL;

	// update the read buffer pointers
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
	pvt->_readbufferend=NULL;

	// set the write buffer size
	return setWriteBufferSize(size);
}

bool filedescriptor::unsetStreamReadBuffer() const {

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: unsetStreamReadBufferSize(%d,",
			(int)process::getProcessId(),(int)pvt->_fd);
	#endif

	// clean up but don't create a new read buffer
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=NULL;

	// update the read buffer pointers
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
	pvt->_readbufferend=NULL;
	
	// set the read method to use
	pvt->_readPtr=&filedescriptor::unBufferedRead;

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf(",success)\n");
	#endif
	return true;
}

bool filedescriptor::unsetStorageReadBuffer() const {

	// for storage filedescriptors, we only
	// use one buffer, the write buffer

	// clean up but don't create a new read buffer
	delete[] pvt->_readbuffer;
	pvt->_readbuffer=NULL;

	// update the read buffer pointers
	pvt->_readbufferhead=NULL;
	pvt->_readbuffertail=NULL;
	pvt->_readbufferend=NULL;

	// unset the write buffer
	return unsetStorageWriteBuffer();
}

ssize_t filedescriptor::getReadBufferSize() const {
	return (pvt->_isstream)?
			(pvt->_readbufferend-pvt->_readbuffer):
			pvt->_writeblocksize;
}

void filedescriptor::setMmapBufferingEnabled(bool enabled) {
	pvt->_writebuffermmapenabled=(enabled && memorymap::supported());
}

bool filedescriptor::getMmapBufferingEnabled() {
	return pvt->_writebuffermmapenabled;
}

bool filedescriptor::getIsCurrentBlockMmapBuffered() {
	return pvt->_writebuffermap;
}

off64_t filedescriptor::getCurrentBlockOffset() {
	return pvt->_writeblockoffset;
}

ssize_t filedescriptor::getBytesBuffered() {
	return (pvt->_writebuffertail && pvt->_writebuffer)?
			pvt->_writebuffertail-pvt->_writebuffer:0;
}

int32_t filedescriptor::getFileDescriptor() const {
	return pvt->_fd;
}

void filedescriptor::setFileDescriptor(int32_t filedesc) {
	pvt->_fd=filedesc;
}

bool filedescriptor::setIsStream(bool isstream) {
	if (pvt->_readbuffer || pvt->_writebuffer) {
		return false;
	}
	pvt->_isstream=isstream;
	return true;
}

bool filedescriptor::getIsStream() {
	return pvt->_isstream;
}

int32_t filedescriptor::duplicate() const {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__DUP)
			result=_dup(pvt->_fd);
		#elif defined(RUDIMENTS_HAVE_DUP)
			result=dup(pvt->_fd);
		#else
			#error no dup or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return result;
}

bool filedescriptor::duplicate(int32_t newfd) const {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__DUP2)
			result=_dup2(pvt->_fd,newfd);
		#elif defined(RUDIMENTS_HAVE_DUP2)
			result=dup2(pvt->_fd,newfd);
		#else
			#error no dup2 or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return (result==newfd);
}

void filedescriptor::setSocketLayer(socketlayer *layer) {
	pvt->_socklr=layer;
}

socketlayer *filedescriptor::getSocketLayer() {
	return pvt->_socklr;
}

bool filedescriptor::supportsBlockingNonBlockingModes() {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return true;
	#else
		return false;
	#endif
}

bool filedescriptor::useNonBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return (fCntl(F_SETFL,fCntl(F_GETFL,0)|O_NONBLOCK)!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::useBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && \
		defined(F_SETFL) && defined (F_GETFL)
		return (fCntl(F_SETFL,fCntl(F_GETFL,0)&(~O_NONBLOCK))!=-1);
	#else
		return false;
	#endif
}

bool filedescriptor::isUsingNonBlockingMode() const {
	#if defined(RUDIMENTS_HAVE_FCNTL) && defined(F_GETFL)
		return (fCntl(F_GETFL,0)&O_NONBLOCK);
	#else
		return false;
	#endif
}

off64_t filedescriptor::setPositionRelativeToBeginning(off64_t offset) const {
	return setPosition(offset,SEEK_SET);
}

off64_t filedescriptor::setPositionRelativeToCurrent(off64_t offset) const {
	return setPosition(offset,SEEK_CUR);
}

off64_t filedescriptor::setPositionRelativeToEnd(off64_t offset) const {
	return setPosition(offset,SEEK_END);
}

off64_t filedescriptor::setPosition(off64_t offset, int32_t whence) const {

	// for stream filedescriptors, we can't set the position, just return
	// the current offset, which should always be 0
	if (pvt->_isstream) {
		return 0;
	}

	// for non-stream (storage) filedescriptors...

	// if we're not buffering then seek to the requested offset
	if (!pvt->_writeblocksize) {
		return lseek(offset,whence);
	}

	// if we are buffering, then just set the offset, the first call to
	// realignWriteBuffer() will actually position us in the file
	if (whence==SEEK_CUR) {
		offset=pvt->_offset+offset;
	} else if (whence==SEEK_END) {
		off64_t	size=getSize();
		if (size<0) {
			return size;
		}
		offset=size+offset;
	}
	pvt->_offset=offset;

	#if defined(DEBUG_BUFFERING)
	debugPrintf("%d: setPosition(%d,offset=%08x,",
		(int)process::getProcessId(),(int)pvt->_fd,offset);
	#endif

	// If we have an existing pvt->_writebuffer (eg. we didn't just open the
	// file, enable buffering and immediately call setPosition()) then...
	if (pvt->_writebuffer) {

		// If the offset is inside of the current block, then
		// offset-blockoffset<blocksize.  Also, if the offset is way
		// past the current block, then offset-blockoffset>blocksize.
		// Those work naturally.
		//
		// We use a trick to handle cases where the offset is before
		// the current block though.  In that case, offset-blockoffset
		// ends up being a negative number, but since we convert it to
		// an unsigned integer, it becomes a large positive number, so
		// still we have a case where offset-blockoffset>blocksize.
		//
		// I belive that since all numbers involved are signed, there
		// should be no way for offset-blockoffset to result in a large
		// enough negative number that it wraps back around and ends
		// up < blocksize.
		if ((uint64_t)(offset-pvt->_writeblockoffset)<
					(uint64_t)pvt->_writeblocksize) {

			#if defined(DEBUG_BUFFERING)
			debugPrintf("inside current block");
			#endif

			// if we just moved around inside of the current block
			// then we need to adjust the head and read/write
			// avails (see note at end of realignWriteBuffer() for
			// why the read/write avails are different)
			pvt->_writebufferhead=pvt->_writebuffer+
						(offset-pvt->_writeblockoffset);
			pvt->_writebufferreadavail=pvt->_writebuffertail-
						pvt->_writebufferhead;
			pvt->_writebufferwriteavail=pvt->_writebufferend-
						pvt->_writebufferhead;

		} else {

			// Otherwise we'll just set the read/write avail to 0
			// so that realignWriteBuffer() will get called during
			// the next read/write and figure everything out at that
			// time.
			pvt->_writebufferwriteavail=0;
			pvt->_writebufferreadavail=0;

			#if defined(DEBUG_BUFFERING)
			debugPrintf("outside current block");
			#endif
		}
	}
	#if defined(DEBUG_BUFFERING)
	else {

		// In this case, read/write avail will be 0, which will
		// trigger realignWriteBuffer() to be called during the next
		// read/write and to figure everything out at that time.
		debugPrintf("no existing write buffer");
	}

	debugPrintf(")\n");
	#endif
	return offset;
}

off64_t filedescriptor::getSize() const {
	file	f;
	f.setFileDescriptor(pvt->_fd);
	bool	success=f.getCurrentProperties();
	f.setFileDescriptor(-1);
	if (!success) {
		return -1;
	}
	return f.getSize();
}

off64_t filedescriptor::getCurrentPosition() const {

	// for stream filedescriptors, the position is always 0
	if (pvt->_isstream) {
		return 0;
	}

	// for storage filedescriptors...

	// if we're not buffering then we need to read the offset from the file
	if (!pvt->_writeblocksize) {
		return lseek(0,SEEK_CUR);
	}

	// if we are buffering then return the offset that we've been tracking
	return pvt->_offset;
}

off64_t filedescriptor::lseek(off64_t offset, int32_t whence) const {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE__LSEEK)
			result=_lseek(fd(),offset,whence);
		#elif defined(RUDIMENTS_HAVE_LSEEK)
			result=::lseek(fd(),offset,whence);
		#else
			#error no lseek or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return result;
}

ssize_t filedescriptor::realignWriteBuffer(int32_t sec, int32_t usec) {

	// At this point...
	//
	// pvt->_writeblockoffset is set to the offset of the beginning of the
	// current block.  That block may or may not have been actually
	// buffered yet though.
	//
	// pvt->_offset is set to the offset that we want to read/write at,
	// which may or may not be in the current block.
	//
	// There's a lot to figure out...

	#if defined(DEBUG_BUFFERING)
	debugPrintf("%d: realignWriteBuffer(%d,current blockoffset=%08x",
						(int)process::getProcessId(),
						(int)pvt->_fd,
						pvt->_writeblockoffset);
	#endif

	// un-buffer the current block...
	if (pvt->_writebuffermap) {

		// if the current block is mmapped...

		#if defined(DEBUG_BUFFERING)
		debugPrintf(",current block mmapped");
		#endif

		// detach the map from the current block
		pvt->_writebuffermap->detach();

		// the write buffer pointers will be invalid here, so
		// formally invalidate them and mark the buffer clean
		pvt->_writebuffer=NULL;
		pvt->_writebufferhead=NULL;
		pvt->_writebuffertail=NULL;
		pvt->_writebufferend=NULL;
		pvt->_writebufferreadavail=0;
		pvt->_writebufferwriteavail=0;
		pvt->_writebufferdirty=false;

	} else if (pvt->_writebuffer) {

		// if the current block is traditionally-buffered...

		#if defined(DEBUG_BUFFERING)
		debugPrintf(",current block traditional...\n");
		#endif

		// flush the contents of the write buffer to storage
		if (!flushWriteBuffer(sec,usec)) {
			#if defined(DEBUG_BUFFERING)
			debugPrintf(",failed)\n");
			#endif
			// we haven't actually changed anything yet,
			// so it's safe to just return error here,
			// the operation will fail gracefully and is
			// retryable
			return RESULT_ERROR;
		}

		// At this point, the traditional write buffer
		// is empty but still exists and all of the pointers
		// to it are valid.
		//
		// The writebuffer and writebufferend pointers still
		// point to good locations.  The flush above will have
		// reset the writebuffertail and marked the buffer
		// clean.

		#if defined(DEBUG_BUFFERING)
		debugPrintf("...");
		#endif
	}

	// calculate the new block offset
	pvt->_writeblockoffset=
		pvt->_offset/pvt->_writeblocksize*pvt->_writeblocksize;

	#if defined(DEBUG_BUFFERING)
	debugPrintf(",new blockoffset=%08x,blocksize=%lld",
				pvt->_writeblockoffset,pvt->_writeblocksize);
	#endif

	// If we're here, then any data that was previously buffered has been
	// un-buffered and pvt->_writeblockoffset is aligned to the new block.
	//
	// We're ready to buffer that new block.
	//
	// We would like to just mmap it, which we can do if it's somewhere in
	// the middle of the file.  If it's the last block in the file though,
	// we can run into trouble if we map less than a full block or map
	// a full block that runs off of the end of the file.
	//
	// So we can only mmap it if it's not the last block, or if it is, but
	// it's a full block (i.e. it's exactly pvt->_writeblocksize bytes),
	// so...

	bool	canmmap=false;
	if (pvt->_writebuffermmapenabled) {

		// get the size of the file and
		// determine if we can mmap the block
		off64_t	filesize=getSize();
		if (filesize<0) {
			#if defined(DEBUG_BUFFERING)
			debugPrintf(",error getting file size)\n");
			#endif
			// It's safe to just return error here.  The operation
			// will fail gracefully and all buffer pointers are set
			// such that a retry will fall through safely to this
			// point again.
			return RESULT_ERROR;
		}

		#if defined(DEBUG_BUFFERING)
		if (filesize/pvt->_writeblocksize==
				pvt->_writeblockoffset/pvt->_writeblocksize) {
			debugPrintf(",is last block");
			if (filesize-pvt->_writeblockoffset==
						pvt->_writeblocksize) {
				debugPrintf(",is full block,"
						"can mmap");
			} else {
				debugPrintf(",is partial block,"
						"can't mmap...\n");
			}
		} else {
			debugPrintf(",can mmap");
		}
		#endif

		canmmap=(
			// not the last block
			!(filesize/pvt->_writeblocksize==
					pvt->_writeblockoffset/
					pvt->_writeblocksize) ||
			// is a full block
			(filesize-pvt->_writeblockoffset==
					pvt->_writeblocksize));

	}
	#if defined(DEBUG_BUFFERING)
	else {
		debugPrintf(",mmap disabled,can't mmap...\n");
	}
	#endif


	// if we can mmap the block...
	if (canmmap) {

		// We need to know if the current buffer is an mmap, so we
		// can determine whether we need to delete it or hang on to
		// it below.  Determine that here.
		bool	previouslymmapped=pvt->_writebuffermap;

		// make sure we actually have a memorymap
		if (!pvt->_writebuffermap) {
			pvt->_writebuffermap=new memorymap();
		}

		// attempt to attach the memorymap to the current block
		if (pvt->_writebuffermap->attach(
					pvt->_fd,
					pvt->_writeblockoffset,
					pvt->_writeblocksize,
					PROT_WRITE|PROT_READ,MAP_SHARED)) {

			// success...

			// if the previous buffer wasn't mmapped,
			// then delete the traditional buffer
			if (!previouslymmapped) {
				delete[] pvt->_writebufferunaligned;
				pvt->_writebufferunaligned=NULL;
			}

			// update the write buffer pointers and counts
			pvt->_writebuffer=(unsigned char *)
					pvt->_writebuffermap->getData();
			pvt->_writebufferhead=pvt->_writebuffer+
					(pvt->_offset-pvt->_writeblockoffset);
			pvt->_writebuffertail=
					pvt->_writebuffer+pvt->_writeblocksize;
			pvt->_writebufferend=pvt->_writebuffertail;
			pvt->_writebufferreadavail=pvt->_writeblocksize;
			pvt->_writebufferwriteavail=pvt->_writeblocksize;

			#if defined(DEBUG_BUFFERING)
			debugPrintf(",mapped %d bytes)",
					(int)pvt->_writeblocksize);
			#endif

			// return non-error
			return 0;
		}

		// if the previous buffer was mmapped then we need to set
		// it NULL so a traditional buffer will be created below
		if (previouslymmapped) {
			pvt->_writebuffer=NULL;
		}

		#if defined(DEBUG_BUFFERING)
		char	*err=error::getErrorString();
		debugPrintf(",mmap failed: %s,"
			"attempting traditional buffer...\n",err);
		delete[] err;
		#endif
	}

	// if we couldn't mmap the block, either because we're in the last
	// block of the file and it's a partial block, or just because the
	// attempt failed, then buffer this block traditionally...

	// move to the beginning of the current block
	if (lseek(pvt->_writeblockoffset,SEEK_SET)!=pvt->_writeblockoffset) {
		#if defined(DEBUG_BUFFERING)
		debugPrintf("...,error)\n");
		#endif
		// FIXME: ??? graceful/retryable
		return RESULT_ERROR;
	}

	// delete any existing memorymap, we don't want to have a memory map
	// and a traditional buffer, just one or the other
	delete pvt->_writebuffermap;
	pvt->_writebuffermap=NULL;

	// we may or may not already have a buffer, depending on too many
	// conditions to list here, but if we don't, then create one
	// (and align it to a pvt->_writeblocksize boundary)
	if (!pvt->_writebuffer) {
		allocateWriteBuffer(pvt->_writeblocksize);
	}

	// update the write buffer pointers
	pvt->_writebufferhead=pvt->_writebuffer+
				(pvt->_offset-pvt->_writeblockoffset);
	pvt->_writebuffertail=pvt->_writebuffer;
	pvt->_writebufferend=pvt->_writebuffer+pvt->_writeblocksize;

	// attempt to fill the buffer
	// (temporarily not allowing short reads)
	bool	saveasr=pvt->_allowshortreads;
	pvt->_allowshortreads=false;
	ssize_t	result=unBufferedRead(pvt->_writebuffer,
					pvt->_writeblocksize,sec,usec);
	pvt->_allowshortreads=saveasr;

	// if an error, timeout, abort, max-read, etc.
	// occurred then return that
	if (result<0) {
		#if defined(DEBUG_BUFFERING)
		debugPrintf("...error reading: %d)\n",result);
		#endif
		// FIXME: ??? graceful/retryable
		return result;
	}

	#if defined(DEBUG_BUFFERING)
	if (!result) {
		debugPrintf("...EOF\n");
	} else if (result!=pvt->_writeblocksize) {
		debugPrintf("...short read\n");
	} else {
		debugPrintf("...success\n");
	}
	#endif

	// all went well enough...

	// update the write buffer tail
	pvt->_writebuffertail=pvt->_writebuffer+result;

	// update the number of bytes available in the buffer
	//
	// Note that these could be different for read/write.  The number of
	// bytes available to read is how ever many we were able to copy in
	// to the buffer.  The number of bytes available to write is the full
	// size of the buffer.  These might be different if this is the last
	// block of the file and it's a partial block.
	pvt->_writebufferreadavail=result;
	pvt->_writebufferwriteavail=pvt->_writeblocksize;

	#if defined(DEBUG_BUFFERING)
	debugPrintf(",read %d bytes)\n",(int)result);
	#endif

	// return non-error
	return 0;
}

ssize_t filedescriptor::write(uint16_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(uint16_t));
}

ssize_t filedescriptor::write(uint32_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(uint32_t));
}

ssize_t filedescriptor::write(uint64_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(uint64_t));
}

ssize_t filedescriptor::write(int16_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint16_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(int16_t));
}

ssize_t filedescriptor::write(int32_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint32_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(int32_t));
}

ssize_t filedescriptor::write(int64_t number) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint64_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,sizeof(int64_t));
}

ssize_t filedescriptor::write(uint16_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(uint16_t),sec,usec);
}

ssize_t filedescriptor::write(uint32_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(uint32_t),sec,usec);
}

ssize_t filedescriptor::write(uint64_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet(number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(uint64_t),sec,usec);
}

ssize_t filedescriptor::write(int16_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint16_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(int16_t),sec,usec);
}

ssize_t filedescriptor::write(int32_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint32_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(int32_t),sec,usec);
}

ssize_t filedescriptor::write(int64_t number, int32_t sec, int32_t usec) {
	if (pvt->_translatebyteorder) {
		number=hostToNet((uint64_t)number);
	}
	return highLevelWrite((const unsigned char *)&number,
					sizeof(int64_t),sec,usec);
}

ssize_t filedescriptor::read(uint16_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(uint16_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint32_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(uint32_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint64_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(uint64_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int16_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(int16_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint16_t)*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int32_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(int32_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint32_t)*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int64_t *buffer) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,sizeof(int64_t));
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint64_t)*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint16_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(uint16_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint32_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(uint32_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(uint64_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(uint64_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost(*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int16_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(int16_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint16_t)*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int32_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(int32_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint32_t)*buffer);
	}
	return retval;
}

ssize_t filedescriptor::read(int64_t *buffer,
				int32_t sec, int32_t usec) {
	ssize_t	retval=highLevelRead((unsigned char *)buffer,
					sizeof(int64_t),sec,usec);
	if (pvt->_translatebyteorder) {
		*buffer=netToHost((uint64_t)*buffer);
	}
	return retval;
}

bool filedescriptor::close() {

	// reset offsets
	pvt->_offset=0;
	pvt->_writeblockoffset=0;

	// empty the read buffer...
	if (pvt->_readbuffer) {

		// preserve the buffer so it can be reused if we open another
		// file, but mark it empty by setting the head and tail to the
		// beginning
		pvt->_readbufferhead=pvt->_readbuffer;
		pvt->_readbuffertail=pvt->_readbuffer;
	}

	// empty the write buffer...
	if (pvt->_writebuffermap) {

		// if we've been mmapping...

		// don't preserve the buffer, we'll let realignWriteBuffer()
		// figure out what to do the next time it's called
		delete pvt->_writebuffermap;
		pvt->_writebuffermap=NULL;
		pvt->_writebuffer=NULL;
		pvt->_writebufferhead=NULL;
		pvt->_writebuffertail=NULL;
		pvt->_writebufferend=NULL;

	} else if (pvt->_writebuffer) {

		// if we haven't been mmapping...

		// preserve the buffer so it can be reused if we open another
		// file, but mark it empty by setting the head and tail to the
		// beginning
		pvt->_writebufferhead=pvt->_writebuffer;
		pvt->_writebuffertail=pvt->_writebuffer;
		pvt->_writebufferreadavail=0;
	}

	// mark the write buffer not dirty
	pvt->_writebufferdirty=false;

	// close the actual file
	if (pvt->_fd!=-1) {

		// do a low level close
		int32_t	result;
		error::clearError();
		do {
			result=lowLevelClose();
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		if (result==-1) {
			return false;
		}

		// reset the file descriptor
		setFileDescriptor(-1);
	}
	return true;
}

int32_t filedescriptor::lowLevelClose() {
	#if defined(RUDIMENTS_HAVE__CLOSE)
		return _close(pvt->_fd);
	#elif defined(RUDIMENTS_HAVE_CLOSE)
		return ::close(pvt->_fd);
	#else
		#error no close or anything like it
	#endif
}

void filedescriptor::retryInterruptedReads() {
	pvt->_retryinterruptedreads=true;
}

void filedescriptor::dontRetryInterruptedReads() {
	pvt->_retryinterruptedreads=false;
}

bool filedescriptor::getRetryInterruptedReads() const {
	return pvt->_retryinterruptedreads;
}

void filedescriptor::retryInterruptedWrites() {
	pvt->_retryinterruptedwrites=true;
}

void filedescriptor::dontRetryInterruptedWrites() {
	pvt->_retryinterruptedwrites=false;
}

bool filedescriptor::getRetryInterruptedWrites() const {
	return pvt->_retryinterruptedwrites;
}

void filedescriptor::retryInterruptedWaits() {
	pvt->_retryinterruptedwaits=true;
}

void filedescriptor::dontRetryInterruptedWaits() {
	pvt->_retryinterruptedwaits=false;
}

bool filedescriptor::getRetryInterruptedWaits() const {
	return pvt->_retryinterruptedwaits;
}

void filedescriptor::retryInterruptedFcntl() {
	pvt->_retryinterruptedfcntl=true;
}

void filedescriptor::dontRetryInterruptedFcntl() {
	pvt->_retryinterruptedfcntl=true;
}

bool filedescriptor::getRetryInterruptedFcntl() const {
	return pvt->_retryinterruptedfcntl;
}

void filedescriptor::retryInterruptedIoctl() {
	pvt->_retryinterruptedioctl=true;
}

void filedescriptor::dontRetryInterruptedIoctl() {
	pvt->_retryinterruptedioctl=true;
}

bool filedescriptor::getRetryInterruptedIoctl() const {
	return pvt->_retryinterruptedioctl;
}

void filedescriptor::allowShortReads() {
	pvt->_allowshortreads=true;
}

void filedescriptor::dontAllowShortReads() {
	pvt->_allowshortreads=false;
}

void filedescriptor::allowShortWrites() {
	pvt->_allowshortwrites=true;
}

void filedescriptor::dontAllowShortWrites() {
	pvt->_allowshortwrites=false;
}

ssize_t filedescriptor::highLevelRead(unsigned char *buf, ssize_t count) {
	return (this->*pvt->_readPtr)(buf,count,-1,-1);
}

ssize_t filedescriptor::highLevelRead(unsigned char *buf, ssize_t count,
						int32_t sec, int32_t usec) {
	return (this->*pvt->_readPtr)(buf,count,sec,usec);
}

ssize_t filedescriptor::streamBufferedRead(unsigned char *buf, ssize_t count,
						int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!count) {
		return 0;
	}

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf("streamBufferedRead of %d bytes\n",(int)count);
	#endif

	// do a bufffered read...
	ssize_t		bytesread=0;
	ssize_t		bytesunread=count;
	for (;;) {

		// copy out what we can from the buffer
		ssize_t	bytesavailable=pvt->_readbuffertail-
						pvt->_readbufferhead;
		if (bytesavailable) {

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("%d bytes in read buffer\n",
						(int)bytesavailable);
			#endif

			// calculate how many bytes to copy out
			ssize_t	bytestocopy=(bytesavailable<bytesunread)?
						bytesavailable:bytesunread;

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("copying %d bytes out of read buffer\n",
							(int)bytestocopy);
			#endif

			// copy out those bytes
			bytestring::copy(buf,pvt->_readbufferhead,bytestocopy);

			// advance various pointers
			buf+=bytestocopy;
			bytesread+=bytestocopy;
			pvt->_readbufferhead+=bytestocopy;
			bytesunread-=bytestocopy;

			// return if we've copied out
			// enough to satisfy the request
			if (bytesread==count) {
				#if defined(DEBUG_READ) && \
					 defined(DEBUG_BUFFERING)
				debugPrintf("yay, we're done reading\n");
				#endif
				return bytesread;
			}

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("need to read %d more bytes\n",
							(int)bytesunread);
			#endif
		}

		// if we've emptied the buffer, then fill it again
		// FIXME: I think if we're here then we must have copied out
		// the entire buffer, and pvt->_readbufferhead will always
		// equal pvt->_readbuffertail, so this test is redundant
		if (pvt->_readbufferhead==pvt->_readbuffertail) {

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("attempting to fill read buffer, "
					"reading %d bytes...\n",
						(int)(pvt->_readbufferend-
							pvt->_readbuffer));
			#endif

			// reset ptr and tail
			pvt->_readbufferhead=pvt->_readbuffer;
			pvt->_readbuffertail=pvt->_readbuffer;

			// attempt to fill the buffer
			// (temporarily allowing short reads)
			bool	saveasr=pvt->_allowshortreads;
			pvt->_allowshortreads=true;
			ssize_t	result=unBufferedRead(pvt->_readbuffer,
							pvt->_readbufferend-
							pvt->_readbuffer,
							sec,usec);
			pvt->_allowshortreads=saveasr;

			// if we encountered an EOF...
			if (!result) {

				// if short reads are allowed, then just
				// return the number of bytes that we were
				// able to read prior to hitting the EOF
				if (pvt->_allowshortreads) {
					#if defined(DEBUG_READ) && \
						defined(DEBUG_BUFFERING)
					debugPrintf("EOF\n");
					#endif
					return bytesread;
				}

				#if defined(DEBUG_READ) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("still need %d bytes, "
							"reading...\n",
							(int)bytesunread);
				#endif

				// if short reads are not allowed, then
				// try again, this time not allowing further
				// short reads
				result=unBufferedRead(pvt->_readbuffer,
							bytesunread,
							sec,usec);

				// if we still got short read (I think in this
				// case, it could only have been an EOF) then
				// return the number of bytes that we were able
				// to read prior to hitting the EOF
				if (result>-1 && result!=bytesunread) {
					#if defined(DEBUG_READ) && \
						defined(DEBUG_BUFFERING)
					debugPrintf("EOF\n");
					#endif
					pvt->_readbuffertail=
						pvt->_readbuffer+result;
					return bytesread;
				}
			}

			// if an error, timeout, abort, max-read, etc. occurred
			// during any of the above then return that
			if (result<0) {
				#if defined(DEBUG_READ) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("error reading: %d...\n",result);
				#endif
				return result;
			}

			// all went well
			pvt->_readbufferhead=pvt->_readbuffer;
			pvt->_readbuffertail=pvt->_readbuffer+result;

			#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
			debugPrintf("read %d bytes\n",(int)result);
			#endif
		}
	}
}

ssize_t filedescriptor::storageBufferedRead(unsigned char *buf, ssize_t count,
						int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!count) {
		return 0;
	}

	#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: storageBufferedRead(%d,attempting %d bytes...\n",
			(int)process::getProcessId(),(int)pvt->_fd,(int)count);
	#endif

	// do a bufffered read...
	ssize_t		result;
	ssize_t		bytesread=0;
	ssize_t		bytestocopy;
	for (;;) {

		if (!pvt->_writebufferreadavail) {

			// realign/fill the write buffer
			result=realignWriteBuffer(sec,usec);
			if (result) {
				// if an error, timeout, abort, max-read,
				// etc. occurred then return that
				#if defined(DEBUG_READ) && \
						defined(DEBUG_BUFFERING)
				debugPrintf("...,error: %d)\n",result);
				#endif
				return result;
			}

			// bail on EOF
			if (!pvt->_writebufferreadavail) {
				#if defined(DEBUG_WRITE) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("...0 bytes available!)\n");
				#endif
				return bytesread;
			}
		}

		// calculate how many bytes to actually copy out
		bytestocopy=(pvt->_writebufferreadavail<count)?
					pvt->_writebufferreadavail:count;

		#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
		debugPrintf("...%d bytes available,copying out %d bytes\n",
					(int)pvt->_writebufferreadavail,
					(int)bytestocopy);
		#endif

		// copy out those bytes
		bytestring::copy(buf,pvt->_writebufferhead,bytestocopy);

		// adjust positions and counts
		pvt->_writebufferhead+=bytestocopy;
		pvt->_offset+=bytestocopy;
		bytesread+=bytestocopy;
		count-=bytestocopy;
		pvt->_writebufferreadavail-=bytestocopy;
		pvt->_writebufferwriteavail-=bytestocopy;

		// return if we've copied out enough to satisfy the request
		if (!count) {
			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf(")\n");
			#endif
			return bytesread;
		}

		// adjust buf
		buf+=bytestocopy;

		#if defined(DEBUG_READ) && defined(DEBUG_BUFFERING)
		debugPrintf(",need to read %d more bytes",(int)count);
		#endif
	}
}

ssize_t filedescriptor::unBufferedRead(unsigned char *buf, ssize_t count,
						int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_READ
	debugPrintf("%d: unBufferedRead(%d,attempting %d bytes,sec=%d,usec=%d",
		(int)process::getProcessId(),(int)pvt->_fd,(int)count,sec,usec);
	#endif

	// read from the file descriptor:
	// * timing out, if necessary
	// * doing multiple individual reads, if necessary
	// * limiting individual reads to SSIZE_MAX,
	//   (or whatever maximum size is set by the socketlayer)
	// * retrying interrupted reads, if necessary

	ssize_t	totalread=0;
	ssize_t	sizetoread;
	ssize_t	actualread;
	ssize_t	sizemax=(pvt->_socklr)?pvt->_socklr->getSizeMax():SSIZE_MAX;
	bool	isusingnonblockingmode=isUsingNonBlockingMode();
	while (totalread<count) {

		// limit size of individual reads
		sizetoread=count-totalread;
		if (sizetoread>sizemax) {
			sizetoread=sizemax;
		}

		// wait if necessary
		if (sec>-1 && usec>-1) {

			int32_t	waitresult=waitForNonBlockingRead(sec,usec);

			// return error or timeout
			if (waitresult<0) {
				#ifdef DEBUG_READ
				debugPrintf(",ERROR/TIMEOUT)\n");
				#endif
				return waitresult;
			}
		}

		// set a pointer to the position in the buffer that we need
		// to read data into
		void	*ptr=(void *)(buf+totalread);

		// read into that position...
		error::clearError();
		if (pvt->_socklr) {
			#ifdef DEBUG_READ
			debugPrintf(",(SocketLayer)");
			#endif
			actualread=pvt->_socklr->read(ptr,sizetoread);
		} else {
			actualread=lowLevelRead(ptr,sizetoread);
		}

		#ifdef DEBUG_READ
		debugPrintf(",");
		for (int32_t i=0; i<actualread; i++) {
			debugSafePrint(((unsigned char *)ptr)[i]);
		}
		debugPrintf(",%ld bytes",(long)actualread);
		if (actualread==-1) {
			char	*e=error::getErrorString();
			debugPrintf(",%s",e);
			delete[] e;
		}
		stdoutput.flush();
		#endif

		// if we didn't read the number of bytes we expected to,
		// handle that...
		if (actualread!=sizetoread) {
			if (isusingnonblockingmode &&
				error::getErrorNumber()==EAGAIN) {
				#ifdef DEBUG_READ
				debugPrintf(",EAGAIN");
				#endif
				// if we got an EAGAIN, and we're in
				// non-blocking mode, then try again
				break;
			} else if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_READ
				debugPrintf(",EINTR");
				#endif
				// if we got an EINTR, then we may need to
				// retry the read
				if (process::getShutDownFlag()) {
					return RESULT_ERROR;
				} else if (pvt->_retryinterruptedreads) {
					continue;
				} else {
					totalread=totalread+actualread;
					break;
				}
			} else if (actualread==0 &&
					error::getErrorNumber()==0) {
				// eof condition
				#ifdef DEBUG_READ
				debugPrintf(",EOF");
				#endif
				break;
			} else if (actualread==-1) {
				// error condition
				#ifdef DEBUG_READ
				debugPrintf(",ERROR)\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalread=totalread+actualread;

		// if short reads are allowed, then break out here
		if (pvt->_allowshortreads) {
			#ifdef DEBUG_READ
			debugPrintf(",SHORTREAD");
			#endif
			break;
		}
	}

	#ifdef DEBUG_READ
	debugPrintf(",%d)\n",(int)totalread);
	#endif

	// return the total number of bytes that we were able to read
	return totalread;
}

ssize_t filedescriptor::lowLevelRead(void *buf, ssize_t count) {
	#if defined(RUDIMENTS_HAVE__READ)
		return _read(pvt->_fd,buf,count);
	#elif defined(RUDIMENTS_HAVE_READ)
		return ::read(pvt->_fd,buf,count);
	#else
		#error no read or anything like it
	#endif
}

ssize_t filedescriptor::highLevelWrite(
			const unsigned char *buf, ssize_t count) {
	return (this->*pvt->_writePtr)(buf,count,-1,-1);
}

ssize_t filedescriptor::highLevelWrite(
			const unsigned char *buf, ssize_t count,
			int32_t sec, int32_t usec) {
	return (this->*pvt->_writePtr)(buf,count,sec,usec);
}

ssize_t filedescriptor::streamBufferedWrite(
				const unsigned char *buf, ssize_t count,
				int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!count) {
		return 0;
	}

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("streamBufferedWrite of %d bytes\n",(int)count);
	#endif

	// do a bufffered write...
	ssize_t	initialwritebuffersize=pvt->_writebuffertail-pvt->_writebuffer;
	bool	first=true;
	ssize_t	byteswritten=0;
	ssize_t	bytesunwritten=count;
	while (byteswritten<count) {

		// calculate the number of bytes currently buffered
		ssize_t	writebuffersize=pvt->_writebuffertail-
						pvt->_writebuffer;

		// detemine the number of bytes of space remaining in the
		// buffer after the data
		ssize_t	writebufferspace=pvt->_writebufferend-
						pvt->_writebuffertail;

		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("	writebuffersize=%d\n",
						(int)writebuffersize);
		debugPrintf("	writebufferspace=%d\n",
						(int)writebufferspace);
		debugPrintf("	byteswritten=%d\n",
						(int)byteswritten);
		debugPrintf("	bytesunwritten=%d\n",
						(int)bytesunwritten);
		#endif

		// if we have enough space to just buffer the data...
		if (bytesunwritten<=writebufferspace) {

			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf("buffering %d bytes\n",
						(int)bytesunwritten);
			#endif

			// copy the data into the buffer
			bytestring::copy(pvt->_writebuffertail,
						buf,bytesunwritten);
			pvt->_writebuffertail+=bytesunwritten;
			byteswritten+=bytesunwritten;

		// if we don't have enough space to just buffer the data...
		} else {

			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf("just buffering %d bytes\n",
						(int)writebufferspace);
			#endif
			#if defined(DEBUG_BUFFERING)
			debugPrintf("auto-flush write buffer: %d bytes\n",
				(int)(writebuffersize+writebufferspace));
			#endif

			// copy what we can of the data into the buffer
			bytestring::copy(pvt->_writebuffertail,
						buf,writebufferspace);

			// attempt to write the contents of the buffer
			// (temporarily allowing short writes)
			bool	saveasw=pvt->_allowshortwrites;
			pvt->_allowshortwrites=true;
			ssize_t	result=unBufferedWrite(pvt->_writebuffer,
					writebuffersize+writebufferspace,
					sec,usec);
			pvt->_allowshortwrites=saveasw;

			// if we got an EOF, short write, error, timeout,
			// abort, or max-write, then return that
			if (result!=writebuffersize+writebufferspace) {
				return result;
			}

			// reset the buffer tail to the start of the buffer
			pvt->_writebuffertail=pvt->_writebuffer;

			// The first time the buffer is written, the number of
			// bytes that were already in the buffer need to be
			// taken into account when calculating byteswritten,
			// bytesunwritten and buf.
			ssize_t	adjustment=(first)?initialwritebuffersize:0;
			if (first) {
				first=false;
			}

			// update stats
			byteswritten+=result-adjustment;
			bytesunwritten-=result-adjustment;

			// update the position in the buf that we're writing
			buf+=result-adjustment;
		}
	}

	// return the total number of bytes that we were able to write
	return byteswritten;
}

ssize_t filedescriptor::storageBufferedWrite(
				const unsigned char *buf,ssize_t count,
				int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!count) {
		return 0;
	}

	#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
	debugPrintf("%d: storageBufferedWrite(%d,attempting %d bytes...\n",
			(int)process::getProcessId(),(int)pvt->_fd,(int)count);
	#endif

	// do a bufffered write...
	ssize_t		result;
	ssize_t		byteswritten=0;
	ssize_t		bytestocopy;
	for (;;) {

		if (!pvt->_writebufferwriteavail) {

			// realign/fill the write buffer
			result=realignWriteBuffer(sec,usec);
			if (result) {
				// if an error, timeout, abort, max-read,
				// etc. occurred then return that
				#if defined(DEBUG_WRITE) && \
						defined(DEBUG_BUFFERING)
				debugPrintf("...,error: %d)\n",result);
				#endif
				return result;
			}

			// bail on out of file system space or simmilar
			if (!pvt->_writebufferwriteavail) {
				#if defined(DEBUG_WRITE) && \
					defined(DEBUG_BUFFERING)
				debugPrintf("...0 bytes available!)\n");
				#endif
				return byteswritten;
			}
		}

		// calculate how many bytes to actually copy in
		bytestocopy=(pvt->_writebufferwriteavail<count)?
					pvt->_writebufferwriteavail:count;

		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf("...%d bytes available,copying in %d bytes",
					(int)pvt->_writebufferwriteavail,
					(int)bytestocopy);
		#endif

		// copy in those bytes
		bytestring::copy(pvt->_writebufferhead,buf,bytestocopy);

		// advance various pointers
		pvt->_writebufferhead+=bytestocopy;
		pvt->_offset+=bytestocopy;
		byteswritten+=bytestocopy;
		count-=bytestocopy;
		pvt->_writebufferwriteavail-=bytestocopy;

		// If the buffer head catches up to the buffer tail, then
		// adjust the buffer tail.  This can happen if we're
		// appending to the last block of a file, and it was a
		// partial block.
		if (pvt->_writebufferhead>pvt->_writebuffertail) {
			pvt->_writebuffertail=pvt->_writebufferhead;
		}

		// reset the read avail
		pvt->_writebufferreadavail=pvt->_writebuffertail-
						pvt->_writebufferhead;

		// mark the buffer dirty
		pvt->_writebufferdirty=true;

		// return if we've copied in enough to satisfy the request
		if (!count) {
			#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
			debugPrintf(")\n");
			#endif
			return byteswritten;
		}

		// adjust buf
		buf+=bytestocopy;

		#if defined(DEBUG_WRITE) && defined(DEBUG_BUFFERING)
		debugPrintf(",need to write %d more bytes",(int)count);
		#endif
	}
}

bool filedescriptor::flushWriteBuffer(int32_t sec, int32_t usec) {

	// FIXME: this can actually fail in various ways and really ought to
	// return the number of bytes flushed, one of the various error
	// conditions, or EOF rather than just true/false

	// degenerate case, bail immediately
	if (!pvt->_writebuffer) {
		return true;
	}

	// if this isn't a streaming filedescriptor (i.e. is a storage file
	// descriptor) and the buffer isn't dirty (i.e. hasn't had anything
	// actually written to it), or we're using a memorymap, then just reset
	// the buffer tail and mark the buffer clean
	if ((!pvt->_isstream && !pvt->_writebufferdirty) ||
					pvt->_writebuffermap ) {
		pvt->_writebuffertail=pvt->_writebuffer;
		pvt->_writebufferreadavail=0;
		pvt->_writebufferdirty=false;
		return true;
	}

	// calculate how much to write
	ssize_t	writebuffersize=pvt->_writebuffertail-pvt->_writebuffer;
	#if defined(DEBUG_BUFFERING)
	debugPrintf("%d: flushWriteBuffer(%d,attempting %d bytes",
					(int)process::getProcessId(),
					(int)pvt->_fd,(int)writebuffersize);
	#endif

	// set the position in the file to write to
	// (unless this is a streaming filedescriptor)
	// FIXME: this appears to be necessary, though I need to figure out
	// in what situations we're not just naturally at this position, as
	// we may be able to avoid doing this sometimes
	if (!pvt->_isstream) {
		#if defined(DEBUG_BUFFERING)
		debugPrintf(",current pos=%08x,seeking to %08x",
						lseek(0,SEEK_CUR),
						pvt->_writeblockoffset);
		#endif
		if (lseek(pvt->_writeblockoffset,SEEK_SET)!=
						pvt->_writeblockoffset) {
			#if defined(DEBUG_BUFFERING)
			debugPrintf(",lseek failed)\n");
			#endif
			return false;
		}
	}

	#if defined(DEBUG_BUFFERING)
	debugPrintf(",...\n");
	#endif

	// write the contents of the buffer
	// (temporarily not allowing short writes)
	bool	saveasw=pvt->_allowshortwrites;
	pvt->_allowshortwrites=false;
	ssize_t	result=unBufferedWrite(pvt->_writebuffer,
					writebuffersize,sec,usec);
	pvt->_allowshortwrites=saveasw;

	#if defined(DEBUG_BUFFERING)
	debugPrintf("...,%d bytes,%s)\n",(int)result,
			(result==writebuffersize)?"true":"false");
	#endif

	// If the write succeeded, then reset the buffer tail and mark the
	// buffer clean.  If not, then don't do either of those things so
	// the buffer state will be the same as it was prior to the attempt,
	// and the flush can be retried.
	if (result) {
		// reset the buffer tail and mark the buffer clean
		pvt->_writebuffertail=pvt->_writebuffer;
		pvt->_writebufferreadavail=0;
		pvt->_writebufferdirty=false;
		return true;
	}
	return false;
}

ssize_t filedescriptor::unBufferedWrite(
				const unsigned char *buf, ssize_t count,
				int32_t sec, int32_t usec) {

	// degenerate case, bail immediately
	if (!buf) {
		return 0;
	}

	#ifdef DEBUG_WRITE
	debugPrintf("%d: unBufferedWrite(%d,attempting %d bytes,sec=%d,usec=%d",
		(int)process::getProcessId(),(int)pvt->_fd,count,sec,usec);
	#endif

	// write to the file descriptor:
	// * timing out, if necessary
	// * doing multiple individual writes, if necessary
	// * limiting individual writes to SSIZE_MAX,
	//   (or whatever maximum size is set by the socketlayer)
	// * retrying interrupted writes, if necessary

	int32_t	olderrno=error::getErrorNumber();

	ssize_t	totalwrite=0;
	ssize_t	sizetowrite;
	ssize_t	actualwrite;
	ssize_t	sizemax=(pvt->_socklr)?pvt->_socklr->getSizeMax():SSIZE_MAX;
	bool	isusingnonblockingmode=isUsingNonBlockingMode();
	while (totalwrite<count) {

		// limit size of individual writes
		sizetowrite=count-totalwrite;
		if (sizetowrite>sizemax) {
			sizetowrite=sizemax;
		}

		// wait if necessary
		if (sec>-1 && usec>-1) {

			int32_t	waitresult=waitForNonBlockingWrite(sec,usec);

			// return error or timeout
			if (waitresult<0) {
				#ifdef DEBUG_WRITE
				debugPrintf(",ERROR/TIMEOUT)\n");
				#endif
				return waitresult;
			}
		}

		// set a pointer to the position in the buffer that we need
		// to write data from
		const void	*ptr=
			(const void *)(buf+totalwrite);

		error::clearError();
		if (pvt->_socklr) {
			#ifdef DEBUG_WRITE
			debugPrintf(",(SocketLayer)");
			#endif
			actualwrite=pvt->_socklr->write(ptr,sizetowrite);
		} else {
			actualwrite=lowLevelWrite(ptr,sizetowrite);
		}

		#ifdef DEBUG_WRITE
		debugPrintf(",");
		for (int32_t i=0; i<actualwrite; i++) {
			debugSafePrint(((const unsigned char *)(ptr))[i]);
		}
		debugPrintf(",%ld bytes",(long)actualwrite);
		if (actualwrite==-1) {
			char	*e=error::getErrorString();
			debugPrintf(",%s",e);
			delete[] e;
		}
		stdoutput.flush();
		#endif

		// if we didn't write the number of bytes we expected to,
		// handle that...
		if (actualwrite!=sizetowrite) {
			if (isusingnonblockingmode &&
				error::getErrorNumber()==EAGAIN) {
				#ifdef DEBUG_READ
				debugPrintf(",EAGAIN");
				#endif
				// if we got an EAGAIN, and we're in
				// non-blocking mode, then try again
				break;
			} else if (error::getErrorNumber()==EINTR) {
				#ifdef DEBUG_WRITE
				debugPrintf(",EINTR");
				#endif
				// if we got an EINTR, then we may need to
				// retry the write
				if (process::getShutDownFlag()) {
					return RESULT_ERROR;
				} else if (pvt->_retryinterruptedwrites) {
					continue;
				} else {
					totalwrite=totalwrite+actualwrite;
					break;
				}
			} else if (actualwrite==0 &&
					error::getErrorNumber()==0) {
				// eof condition
				#ifdef DEBUG_WRITE
				debugPrintf(",EOF");
				#endif
				break;
			} else if (actualwrite==-1) {
				// error condition
				#ifdef DEBUG_WRITE
				debugPrintf(",ERROR)\n");
				#endif
				return RESULT_ERROR;
			}
		}

		totalwrite=totalwrite+actualwrite;

		// if short writes are allowed, then break out here
		if (pvt->_allowshortwrites) {
			#ifdef DEBUG_WRITE
			debugPrintf(",SHORTWRITE");
			#endif
			break;
		}
	}

	#ifdef DEBUG_WRITE
	debugPrintf(",%d)\n",(int)totalwrite);
	#endif
	error::setErrorNumber(olderrno);

	// return the total number of bytes that we were able to write
	return totalwrite;
}

ssize_t filedescriptor::lowLevelWrite(const void *buf, ssize_t count) {
	#if defined(RUDIMENTS_HAVE__WRITE)
		return _write(pvt->_fd,buf,count);
	#elif defined(RUDIMENTS_HAVE_WRITE)
		return ::write(pvt->_fd,buf,count);
	#else
		#error no write or anything like it
	#endif
}

int32_t filedescriptor::waitForNonBlockingRead(
				int32_t sec, int32_t usec) const {
	if (!pvt->_lstnr) {
		pvt->_lstnr=new listener();
	} else {
		pvt->_lstnr->removeAllFileDescriptors();
	}
	pvt->_lstnr->addReadFileDescriptor((filedescriptor *)this);
	return pvt->_lstnr->listen(sec,usec);
}

int32_t filedescriptor::waitForNonBlockingWrite(
				int32_t sec, int32_t usec) const {
	if (!pvt->_lstnr) {
		pvt->_lstnr=new listener();
	} else {
		pvt->_lstnr->removeAllFileDescriptors();
	}
	pvt->_lstnr->addWriteFileDescriptor((filedescriptor *)this);
	return pvt->_lstnr->listen(sec,usec);
}

void filedescriptor::translateByteOrder() {
	pvt->_translatebyteorder=true;
}

void filedescriptor::dontTranslateByteOrder() {
	pvt->_translatebyteorder=false;
}

bool filedescriptor::createPipe(filedescriptor *readfd,
				filedescriptor *writefd) {
	int32_t	result;
	int	fd[2];
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_PIPE)
			result=pipe(fd);
		#elif defined(RUDIMENTS_HAVE__PIPE)
			result=_pipe(fd,1024,0);
		#else
			#error no pipe or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (!result) {
		if (readfd) {
			readfd->setFileDescriptor(fd[0]);
		}
		if (writefd) {
			writefd->setFileDescriptor(fd[1]);
		}
	}
	return !result;
}

uint16_t filedescriptor::hostToNet(uint16_t value) {
	return htons(value);
}

uint32_t filedescriptor::hostToNet(uint32_t value) {
	return htonl(value);
}

uint64_t filedescriptor::hostToNet(uint64_t value) {
	#if defined(RUDIMENTS_HAVE_HTONLL)
		return htonll(value);
	#elif __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#elif defined(RUDIMENTS_HAVE_BSWAP_64)
		return bswap_64(value);
	#elif defined(RUDIMENTS_HAVE___BSWAP64)
		return __bswap64(value);
	#elif defined(RUDIMENTS_HAVE_BSWAP64)
		return bswap64(value);
	#elif defined(RUDIMENTS_HAVE_SWAP64)
		return swap64(value);
	#elif defined(RUDIMENTS_HAVE_SWAP_INT64)
		return __swap_int64(value);
	#elif defined(RUDIMENTS_HAVE_OSSWAPHOSTTOLITTLEINT64)
		return OSSwapHostToLittleInt64(value);
	#else
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			return
			(((uint64_t)hostToNet(
				(uint32_t)(value&0x00000000FFFFFFFFLL)))<<32)|
			((uint64_t)hostToNet(
				(uint32_t)((value&0xFFFFFFFF00000000LL)>>32)));
		#else
			return htonl(value);
		#endif
	#endif
}

uint16_t filedescriptor::netToHost(uint16_t value) {
	return ntohs(value);
}

uint32_t filedescriptor::netToHost(uint32_t value) {
	return ntohl(value);
}

uint64_t filedescriptor::netToHost(uint64_t value) {
	#if defined(RUDIMENTS_HAVE_NTOHLL)
		return ntohll(value);
	#elif __BYTE_ORDER == __BIG_ENDIAN
		return value;
	#elif defined(RUDIMENTS_HAVE_BSWAP_64)
		return bswap_64(value);
	#elif defined(RUDIMENTS_HAVE___BSWAP64)
		return __bswap64(value);
	#elif defined(RUDIMENTS_HAVE_BSWAP64)
		return bswap64(value);
	#elif defined(RUDIMENTS_HAVE_SWAP64)
		return swap64(value);
	#elif defined(RUDIMENTS_HAVE_SWAP_INT64)
		return __swap_int64(value);
	#elif defined(RUDIMENTS_HAVE_OSSWAPLITTLETOHOSTINT64)
		return OSSwapLittleToHostInt64(value);
	#else
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			return
			(((uint64_t)netToHost(
				(uint32_t)(value&0x00000000FFFFFFFFLL)))<<32)|
			((uint64_t)netToHost(
				(uint32_t)((value&0xFFFFFFFF00000000LL)>>32)));
		#else
			return ntohl(value);
		#endif
	#endif
}

uint16_t filedescriptor::hostToLittleEndian(uint16_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		return (((value&0x00FF)<<8)|((value&0xFF00)>>8));
	#endif
}

uint32_t filedescriptor::hostToLittleEndian(uint32_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		return (((value&0x000000FF)<<24)|
			((value&0x0000FF00)<<8)|
			((value&0x00FF0000)>>8)|
			((value&0xFF000000)>>24));
	#endif
}

uint64_t filedescriptor::hostToLittleEndian(uint64_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			uint32_t	low=(uint32_t)
					(value&0x00000000FFFFFFFFLL);
			uint32_t	high=(uint32_t)
					((value&0xFFFFFFFF00000000LL)>>32);
			low=(((low&0x000000FF)<<24)|
				((low&0x0000FF00)<<8)|
				((low&0x00FF0000)>>8)|
				((low&0xFF000000)>>24));
			high=(((high&0x000000FF)<<24)|
				((high&0x0000FF00)<<8)|
				((high&0x00FF0000)>>8)|
				((high&0xFF000000)>>24));
			return (((uint64_t)high)|(((uint64_t)low)<<32));
		#else
			return hostToLittleEndian((uint32_t)value);
		#endif
	#endif
}

uint16_t filedescriptor::littleEndianToHost(uint16_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		return (((value&0x00FF)<<8)|((value&0xFF00)>>8));
	#endif
}

uint32_t filedescriptor::littleEndianToHost(uint32_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		return (((value&0x000000FF)<<24)|
			((value&0x0000FF00)<<8)|
			((value&0x00FF0000)>>8)|
			((value&0xFF000000)>>24));
	#endif
}

uint64_t filedescriptor::littleEndianToHost(uint64_t value) {
	#if __BYTE_ORDER == __LITTLE_ENDIAN
		return value;
	#else
		#ifdef RUDIMENTS_HAVE_LONG_LONG
			uint32_t	low=(uint32_t)
					(value&0x00000000FFFFFFFFLL);
			uint32_t	high=(uint32_t)
					((value&0xFFFFFFFF00000000LL)>>32);
			low=(((low&0x000000FF)<<24)|
				((low&0x0000FF00)<<8)|
				((low&0x00FF0000)>>8)|
				((low&0xFF000000)>>24));
			high=(((high&0x000000FF)<<24)|
				((high&0x0000FF00)<<8)|
				((high&0x00FF0000)>>8)|
				((high&0xFF000000)>>24));
			return (((uint64_t)high)|(((uint64_t)low)<<32));
		#else
			return littleEndianToHost((uint32_t)value);
		#endif
	#endif
}

int32_t filedescriptor::fCntl(int32_t cmd, long arg) const {
	#ifdef RUDIMENTS_HAVE_FCNTL
		int32_t	result;
		error::clearError();
		do {
			result=fcntl(pvt->_fd,cmd,arg);
		} while (pvt->_retryinterruptedfcntl && result==-1 &&
					error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
		return result;
	#else
		return -1;
	#endif
}

int32_t filedescriptor::ioCtl(int32_t cmd, void *arg) const {
	#ifdef RUDIMENTS_HAVE_IOCTL
		int32_t	result;
		error::clearError();
		do {
			result=ioctl(pvt->_fd,cmd,arg);
		} while (pvt->_retryinterruptedioctl && result==-1 &&
					error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
		return result;
	#else
		return -1;
	#endif
}

bool filedescriptor::passFileDescriptor(int32_t fd) {

#if (defined(RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN) && \
		defined(RUDIMENTS_HAVE_CMSGHDR)) || \
		defined(RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS)

	// have to use sendmsg to pass a file descriptor. 
	// sendmsg can only send a msghdr
	struct	msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// must send at least 1 iovector with 1 byte of real data
	struct iovec	iovector[1];
	iovector[0].iov_base=(RUDIMENTS_IOV_BASE_TYPE)" ";
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	// use other parts of the msghdr structure to send the descriptor
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN

		// new-style: the descriptor is passed in the msg_control...

		// On OS X 10.7, CMSG_LEN ultimately makes a functon call, so
		// this array must be dynamically allocated.
		unsigned char	*control=new unsigned char[
						CMSG_LEN(sizeof(int32_t))];
		messageheader.msg_control=(caddr_t)control;
		messageheader.msg_controllen=CMSG_LEN(sizeof(int32_t));

		struct cmsghdr	*cmptr=CMSG_FIRSTHDR(&messageheader);
		cmptr->cmsg_level=SOL_SOCKET;
		cmptr->cmsg_type=SCM_RIGHTS;
		cmptr->cmsg_len=CMSG_LEN(sizeof(int32_t));

		bytestring::copy((int32_t *)CMSG_DATA(cmptr),
						&fd,sizeof(int32_t));
	#else
		// old-style: the descriptor is passed in the accrights...
		messageheader.msg_accrights=(caddr_t)&fd;
		messageheader.msg_accrightslen=sizeof(int32_t);
	#endif

	// finally, send the msghdr
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_SENDMSG) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_SENDMSG)
			result=sendmsg(pvt->_fd,&messageheader,0);
		#else
			#error no sendmsg or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					pvt->_retryinterruptedwrites &&
					!process::getShutDownFlag());

	// clean up
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
		delete[] control;
	#endif

	return (result!=-1);

#elif defined(RUDIMENTS_HAVE_DUPLICATEHANDLE)

	// get a handle to this process
	// Apparently we can't just use GetCurrentProcess() or
	// we'll just get the pseudo-handle for our process.
	HANDLE	localprocesshandle=OpenProcess(PROCESS_DUP_HANDLE,FALSE,
						(DWORD)GetCurrentProcessId());
	if (!localprocesshandle) {
		return false;
	}

	// tell the other process to go
	if (write(true)!=sizeof(bool)) {
		return false;
	}

	// read the process id from the other side
	uint32_t	otherpid;
	if (read(&otherpid)!=sizeof(uint32_t)) {
		return false;
	}

	// get a handle to that process
	bool	success=true;
	HANDLE	otherprocesshandle=OpenProcess(PROCESS_DUP_HANDLE,
						FALSE,(DWORD)otherpid);
	if (!otherprocesshandle) {
		success=false;
	}

	// get the handle from the fd
	HANDLE	localhandle=INVALID_HANDLE_VALUE;
	if (success) {
		localhandle=(HANDLE)getHandleFromFileDescriptor(fd);
		success=(localhandle!=INVALID_HANDLE_VALUE);
	}

	// duplicate the handle
	HANDLE	otherhandle=INVALID_HANDLE_VALUE;
	if (success) {
		success=(DuplicateHandle(localprocesshandle,
						localhandle,
						otherprocesshandle,
						&otherhandle,
						0,TRUE,
						DUPLICATE_SAME_ACCESS)==TRUE);
	}

	// send otherhandle to other process
	bool	retval=(write((uint64_t)otherhandle)==sizeof(uint64_t));

	// close the other process handle
	CloseHandle(otherprocesshandle);

	// send done flag
	return retval;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::receiveFileDescriptor(int32_t *fd) {

#if (defined(RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN) && \
		defined(RUDIMENTS_HAVE_CMSGHDR)) || \
		defined(RUDIMENTS_HAVE_MSGHDR_MSG_ACCRIGHTS)

	// have to use recvmsg to receive a file descriptor. 
	// recvmsg can only send a msghdr
	struct msghdr	messageheader;

	// these must be null for stream sockets
	messageheader.msg_name=NULL;
	messageheader.msg_namelen=0;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_FLAGS
	// initialize flags to 0
	messageheader.msg_flags=0;
	#endif

	// the process that's going to handoff it's socket will also 
	// send a single iovector with a single byte of data in it, 
	// so we'll receive that too
	struct iovec	iovector[1];
	char		ptr;
	iovector[0].iov_base=(RUDIMENTS_IOV_BASE_TYPE)&ptr;
	iovector[0].iov_len=sizeof(char);
	messageheader.msg_iov=iovector;
	messageheader.msg_iovlen=1;

	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
		// new-style: the descriptor is passed in the msg_control...

		// On OS X 10.7, CMSG_LEN ultimately makes a functon call, so
		// this array must be dynamically allocated.
		unsigned char	*control=new unsigned char[
						CMSG_LEN(sizeof(int32_t))];
		messageheader.msg_control=(caddr_t)control;
		messageheader.msg_controllen=CMSG_LEN(sizeof(int32_t));
	#else
		// old-style: the descriptor is received in the accrights...
		int32_t	newfd;
		messageheader.msg_accrights=(caddr_t)&newfd;
		messageheader.msg_accrightslen=sizeof(int32_t);
	#endif

	// receive the msghdr
	int32_t	result;
	error::clearError();
	do {
		// wait 120 seconds for data to come in
		// FIXME: this should be configurable
		bool	oldwaits=pvt->_retryinterruptedwaits;
		pvt->_retryinterruptedwaits=pvt->_retryinterruptedreads;
		result=waitForNonBlockingRead(120,0);
		pvt->_retryinterruptedwaits=oldwaits;
		if (result==RESULT_TIMEOUT) {
			#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
				delete[] control;
			#endif
			return false;
		}
		if (result>-1) {
			#if defined(RUDIMENTS_HAVE_RECVMSG) || \
				defined(RUDIMENTS_HAVE_UNDEFINED_RECVMSG)
				result=recvmsg(pvt->_fd,&messageheader,0);
			#else
				#error no recvmsg or anything like it
			#endif
		}
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					pvt->_retryinterruptedreads &&
					!process::getShutDownFlag());
	if (result==-1) {
		#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN
			delete[] control;
		#endif
		return false;
	}


	// if we got valid data, set the passed-in descriptor to the 
	// descriptor we received and return success
	#ifdef RUDIMENTS_HAVE_MSGHDR_MSG_CONTROLLEN

		struct cmsghdr  *cmptr=CMSG_FIRSTHDR(&messageheader);
		if (cmptr && cmptr->cmsg_len==CMSG_LEN(sizeof(int32_t)) &&
				cmptr->cmsg_level==SOL_SOCKET &&
				cmptr->cmsg_type==SCM_RIGHTS) {

			bytestring::copy(fd,(int32_t *)CMSG_DATA(cmptr),
							sizeof(int32_t));

			delete[] control;
			return true;
		}
		#ifdef DEBUG_PASSFD
		else {

			// if we got bad data, be specific about what was
			// wrong, this will help debug problems with different
			// platforms
			if (!cmptr) {
				debugPrintf("%d: ",
						(int)process::getProcessId());
				debugPrintf("null cmptr\n");
			} else {
				if (cmptr->cmsg_level!=SOL_SOCKET) {
					debugPrintf("%d: ",
						(int)process::getProcessId());
					debugPrintf("got cmsg_level=%ld",
						(long)(cmptr->cmsg_level));
					debugPrintf(" instead of %ld",
						(long)(SOL_SOCKET));
					debugPrintf("\n");
				}
				if (cmptr->cmsg_type!=SCM_RIGHTS) {
					debugPrintf("%d: ",
						(int)process::getProcessId());
					debugPrintf("got cmsg_type=%ld",
						(long)(cmptr->cmsg_type));
					debugPrintf(" instead of %ld",
						(long)(SCM_RIGHTS));
					debugPrintf("\n");
				}
			}
		}
		#endif

		delete[] control;
	#else
		if (messageheader.msg_accrightslen==sizeof(int32_t)) {
			*fd=newfd;
			return true;
		}
	#endif

	// if we're here then we must have received some bad data
	return false;

#elif defined(RUDIMENTS_HAVE_DUPLICATEHANDLE)

	// wait for the other process to tell us to go
	bool	go;
	if (read(&go)!=sizeof(bool)) {
		return false;
	}

	// send our process id
	uint32_t	pid=process::getProcessId();
	if (write(pid)!=sizeof(uint32_t)) {
		return false;
	}

	// get the handle from the other process
	uint64_t	handle;
	if (read(&handle)!=sizeof(uint64_t)) {
		return false;
	}

	// get file descriptor from handle
	if ((HANDLE)handle!=INVALID_HANDLE_VALUE) {
		*fd=_open_osfhandle((long)handle,0);
		return true;
	}
	return false;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::supportsPassReceiveFileDescriptor() {

        // not supported on Cygwin, Linux < 2.2, syllable, IRIX, mac os 10.0,
        // FreeBSD, NetBSD, or OpenBSD
        char    *os=sys::getOperatingSystemName();
        char    *rel=sys::getOperatingSystemRelease();
        double  ver=charstring::toFloat(rel);
	delete[] rel;
	bool	supported=!(
			!charstring::compare(os,"CYGWIN",6) ||
			(!charstring::compare(os,"Linux",5) && ver<2.2) ||
			!charstring::compare(os,"syllable",8) ||
			!charstring::compare(os,"IRIX",4) ||
               		(!charstring::compare(os,"Darwin",6) && ver<1.4) ||
               		!charstring::compare(os,"FreeBSD",7) ||
               		!charstring::compare(os,"NetBSD",6) ||
               		!charstring::compare(os,"OpenBSD",7)
			);
	delete[] os;
	return supported;
}

bool filedescriptor::passSocket(int32_t sock) {

#if defined(RUDIMENTS_HAVE_WSADUPLICATESOCKET)

	// tell the other process to go
	if (write(true)!=sizeof(bool)) {
		return false;
	}

	// read the process id from the other side
	uint32_t	otherpid;
	if (read(&otherpid)!=sizeof(uint32_t)) {
		return false;
	}

	// duplicate the socket
	WSAPROTOCOL_INFO	wpinfo;
	if (WSADuplicateSocket((SOCKET)sock,otherpid,&wpinfo)) {
		return false;
	}

	// write the wsaprotocol_info to the other side
	if (write((void *)&wpinfo,sizeof(wpinfo))!=sizeof(wpinfo)) {
		return false;
	}

	// get result from the other process
	bool	result;
	if (read(&result)!=sizeof(bool)) {
		return false;
	}
	return result;
#else
	return passFileDescriptor(sock);
#endif
}

bool filedescriptor::receiveSocket(int32_t *sock) {

#if defined(RUDIMENTS_HAVE_WSADUPLICATESOCKET)

	// wait for the other process to tell us to go
	bool	go;
	if (read(&go)!=sizeof(bool)) {
		return false;
	}

	// send our process id
	uint32_t	pid=process::getProcessId();
	if (write(pid)!=sizeof(uint32_t)) {
		return false;
	}

	// read a wsaprotocol_info from the other side
	WSAPROTOCOL_INFO	wpinfo;
	if (read((void *)&wpinfo,sizeof(wpinfo))!=sizeof(wpinfo)) {
		return false;
	}

	// create the socket
	*sock=WSASocket(AF_INET,SOCK_STREAM,IPPROTO_TCP,
					&wpinfo,0,WSA_FLAG_OVERLAPPED);
	bool	result=(*sock!=INVALID_SOCKET);

	// tell the other process how it went
	if (write(result)!=sizeof(bool)) {
		return false;
	}
	return result;
#else
	return receiveFileDescriptor(sock);
#endif
}

bool filedescriptor::supportsPassReceiveSocket() {
	// so far, any platform that supports passing file
	// descriptors also supports passing sockets
	return supportsPassReceiveFileDescriptor();
}

bool filedescriptor::useNaglesAlgorithm() {
	return setNoDelay(0);
}

bool filedescriptor::dontUseNaglesAlgorithm() {
	return setNoDelay(1);
}

bool filedescriptor::setNoDelay(int32_t onoff) {
#ifdef TCP_NODELAY
	int32_t	value=onoff;
	return !setSockOpt(IPPROTO_TCP,TCP_NODELAY,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
				(socklen_t)sizeof(int));
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::getSocketWriteBufferSize(int32_t *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_SNDBUF,
				(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)size,
				&intsize)!=-1;
}

bool filedescriptor::setSocketWriteBufferSize(int32_t size) {
	return !setSockOpt(SOL_SOCKET,SO_SNDBUF,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&size,
				(socklen_t)sizeof(int));
}

bool filedescriptor::getSocketReadBufferSize(int32_t *size) {
	socklen_t	intsize=sizeof(int);
	return getSockOpt(SOL_SOCKET,SO_RCVBUF,
				(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)size,
				&intsize)!=-1;
}

bool filedescriptor::setSocketReadBufferSize(int32_t size) {
	return setSockOpt(SOL_SOCKET,SO_RCVBUF,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&size,
				(socklen_t)sizeof(int))!=-1;
}

bool filedescriptor::disableIPv4() {
#ifdef IPV6_V6ONLY
	int32_t	no=0;
	return setSockOpt(IPPROTO_IPV6,IPV6_V6ONLY,
				(void *)&no,
				(socklen_t)sizeof(int32_t))!=-1;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

bool filedescriptor::enableIPv4() {
#ifdef IPV6_V6ONLY
	int32_t	yes=1;
	return setSockOpt(IPPROTO_IPV6,IPV6_V6ONLY,
				(void *)&yes,
				(socklen_t)sizeof(int32_t))!=-1;
#else
	RUDIMENTS_SET_ENOSYS
	return false;
#endif
}

const char *filedescriptor::getType() const {
	return pvt->_type;
}

char *filedescriptor::getPeerAddress() const {

	// initialize a socket address structure
	struct sockaddr_in		clientsin;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(clientsin);
	bytestring::zero(&clientsin,sizeof(clientsin));

	// get the peer address
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_GETPEERNAME) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_GETPEERNAME)
			result=getpeername(pvt->_fd,
						(struct sockaddr *)&clientsin,
						&size);
		#else
			#error no getpeername or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());

	// if getpeername was successful and the peer was an inet socket,
	// convert the address to a string and return a copy of it,
	// otherwise return NULL
	if (result!=-1 && ((struct sockaddr *)&clientsin)->sa_family==AF_INET) {
		return charstring::duplicate(inet_ntoa(clientsin.sin_addr));
	}
	return NULL;
}

int32_t filedescriptor::getSockOpt(int32_t level, int32_t optname,
				void *optval, socklen_t *optlen) {
	int32_t	result;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	tempoptlen;
	if (optlen) {
		tempoptlen=*optlen;
	}
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_GETSOCKOPT) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_GETSOCKOPT)
			result=getsockopt(pvt->_fd,level,optname,
				(RUDIMENTS_GETSOCKOPT_OPTVAL_TYPE)optval,
				&tempoptlen);
		#else
			#error no getsockopt or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (optlen) {
		*optlen=tempoptlen;
	}
	return result;
}

int32_t filedescriptor::setSockOpt(int32_t level, int32_t optname,
				const void *optval, socklen_t optlen) {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_SETSOCKOPT) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_SETSOCKOPT)
			result=setsockopt(pvt->_fd,level,optname,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)optval,
				optlen);
		#else
			#error no setsockopt or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return result;
}

const char *filedescriptor::type() const {
	return pvt->_type;
}

void filedescriptor::type(const char *tp) {
	pvt->_type=tp;
}

int32_t filedescriptor::fd() const {
	return pvt->_fd;
}

void filedescriptor::fd(int32_t filedes) {
	setFileDescriptor(filedes);
}

socketlayer *filedescriptor::socklr() {
	return pvt->_socklr;
}

bool filedescriptor::closeOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)|FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				HANDLE_FLAG_INHERIT,0)!=0;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::dontCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return !fCntl(F_SETFD,fCntl(F_GETFD,FD_CLOEXEC)&(~FD_CLOEXEC));
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		return SetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				HANDLE_FLAG_INHERIT,
				HANDLE_FLAG_INHERIT)!=0;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

bool filedescriptor::getCloseOnExec() {
	#if defined(RUDIMENTS_HAVE_FD_CLOEXEC)
		return fCntl(F_GETFD,FD_CLOEXEC);
	#elif defined(RUDIMENTS_HAVE_HANDLE_FLAG_INHERIT)
		DWORD	inherit;
		if (GetHandleInformation(
				(HANDLE)getHandleFromFileDescriptor(pvt->_fd),
				&inherit)) {
			return (bool)(inherit&HANDLE_FLAG_INHERIT);
		}
		return false;
	#else
		#error no FD_CLOEXEC or anything like it
	#endif
}

ssize_t filedescriptor::printfDelegate(const char *format, va_list *argp) {

	ssize_t	size=0;

	// If we're not buffering writes...
	if (!pvt->_writebuffer) {

		#ifdef RUDIMENTS_HAVE_VDPRINTF

			// use vdprintf if it's available
			// (on some platforms (redhat 9), it tends to set
			// errno=ESPIPE, even on success, so save/restore
			// errno too)
			int32_t	olderr=error::getErrorNumber();
			int	result=vdprintf(pvt->_fd,format,*argp);
			if (result>-1) {
				error::setErrorNumber(olderr);
			}
			return result;

		#else

			// otherwise use vfprintf, if we can
			FILE	*f=NULL;
			if (pvt->_fd==0) {
				f=stdin;
			} else if (pvt->_fd==1) {
				f=stdout;
			} else if (pvt->_fd==2) {
				f=stderr;
			}

			// Use fdopen if it's available.  Unfortunately we
			// can't (reliably) on Windows because it won't work
			// if the filedescriptor is a socket.
			#if defined(RUDIMENTS_HAVE_FDOPEN) && \
				defined(FD) && !defined(_WIN32)
			else {
				f=fdopen(pvt->_fd,"a");

				// Some platforms (Unixware) don't like "a"
				// with some types of file descriptors, so if
				// "a" fails, then try "w".
				if (!f) {
					f=fdopen(pvt->_fd,"w");
				}
			}
			#endif

			if (f) {
				size=vfprintf(f,format,*argp);
				fflush(f);

				#if defined(RUDIMENTS_HAVE_FDOPEN) && \
					defined(FD) && !defined(_WIN32)
				if (f!=stdin && f!=stdout && f!=stderr) {

					// We need to free f but we don't want
					// fclose() to close pvt->_fd.  There's
					// no standard way of doing this though.
					//
					// Setting f's file descriptor member
					// to -1 is generally reliable, though
					// that's tricky too...

					// The size and signedness of
					// FD varies a bit.  This
					// is the only way to handle
					// all variations without the
					// compiler throwing errors.
					if (sizeof(FD)==1) {
						int8_t	i8=-1;
						bytestring::copy(&(FD),&i8,1);
					} else if (sizeof(FD)==2) {
						int16_t i16=-1;
						bytestring::copy(&(FD),&i16,2);
					} else if (sizeof(FD)==4) {
						int32_t i32=-1;
						bytestring::copy(&(FD),&i32,4);
					} else if (sizeof(FD)==8) {
						int64_t i64=-1;
						bytestring::copy(&(FD),&i64,8);
					}

					// ok, now close f
					fclose(f);
				}
				#endif

				return size;
			}
		#endif
	}

	// If we are buffering writes though, don't use the above because it
	// would bypass the buffer.

	// write the formatted data to a buffer
	char	*buffer=NULL;
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		size=vasprintf(&buffer,format,*argp);
	#else
		size=charstring::printf(&buffer,format,argp);
	#endif

	// write the buffer to the file descriptor
	write(buffer,size);

	// clean up
	#ifdef RUDIMENTS_HAVE_VASPRINTF
		free(buffer);
	#else
		delete[] buffer;
	#endif

	return size;
}

ssize_t filedescriptor::printfDelegate(const wchar_t *format, va_list *argp) {

	ssize_t	size=0;

	#ifdef RUDIMENTS_HAVE_VFWPRINTF

	// If we're not buffering writes...
	if (!pvt->_writebuffer) {

		// otherwise use vfwprintf, if we can
		FILE	*f=NULL;
		if (pvt->_fd==0) {
			f=stdin;
		} else if (pvt->_fd==1) {
			f=stdout;
		} else if (pvt->_fd==2) {
			f=stderr;
		}

		// Use fdopen if it's available.  Unfortunately we
		// can't (reliably) on Windows because it won't work
		// if the filedescriptor is a socket.
		#if defined(RUDIMENTS_HAVE_FDOPEN) && \
			defined(FD) && !defined(_WIN32)
		else {
			f=fdopen(pvt->_fd,"a");

			// Some platforms (Unixware) don't like "a"
			// with some types of file descriptors, so if
			// "a" fails, then try "w".
			if (!f) {
				f=fdopen(pvt->_fd,"w");
			}
		}
		#endif

		if (f) {
			size=vfwprintf(f,format,*argp);
			fflush(f);

			#if defined(RUDIMENTS_HAVE_FDOPEN) && \
				defined(FD) && !defined(_WIN32)
			if (f!=stdin && f!=stdout && f!=stderr) {

				// We need to free f but we don't want
				// fclose() to close pvt->_fd.  There's
				// no standard way of doing this though.
				//
				// Setting f's file descriptor member
				// to -1 is generally reliable, though
				// that's tricky too...

				// The size and signedness of
				// FD varies a bit.  This
				// is the only way to handle
				// all variations without the
				// compiler throwing errors.
				if (sizeof(FD)==1) {
					int8_t	i8=-1;
					bytestring::copy(&(FD),&i8,1);
				} else if (sizeof(FD)==2) {
					int16_t i16=-1;
					bytestring::copy(&(FD),&i16,2);
				} else if (sizeof(FD)==4) {
					int32_t i32=-1;
					bytestring::copy(&(FD),&i32,4);
				} else if (sizeof(FD)==8) {
					int64_t i64=-1;
					bytestring::copy(&(FD),&i64,8);
				}

				// ok, now close f
				fclose(f);
			}
			#endif

			return size;
		}
	}

	#endif

	// If we are buffering writes though, don't use the above because it
	// would bypass the buffer.

	// write the formatted data to a buffer
	wchar_t	*buffer=NULL;
	size=wcharstring::printf(&buffer,format,argp);

	// write the buffer to the file descriptor
	write(buffer,size);

	// clean up
	delete[] buffer;

	return size;
}

void filedescriptor::safePrint(char c) {
	safePrint((unsigned char)c);
}

void filedescriptor::safePrint(const char *string, int32_t length) {
	safePrint((const unsigned char *)string,length);
}

void filedescriptor::safePrint(const char *string) {
	safePrint((const unsigned char *)string);
}

static char hex[17]="0123456789ABCDEF";

void filedescriptor::safePrint(unsigned char c) {
	if (c=='\r') {
		printf("\\r");
	} else if (c=='\n') {
		printf("\\n");
	} else if (c=='	') {
		printf("\\t");
	} else if (c>=' ' && c<='~') {
		printf("%c",c);
	} else {
		unsigned int	uintc=(unsigned char)c;
		printf("(0x%c%c|%d)",hex[((c>>4)&0x0F)],hex[(c&0x0F)],uintc);
	}
}

void filedescriptor::safePrint(const unsigned char *string, int32_t length) {
	for (int32_t i=0; i<length; i++) {
		safePrint(*string);
		string++;
	}
}

void filedescriptor::safePrint(const unsigned char *string) {
	safePrint(string,charstring::length((const char *)string));
}

void filedescriptor::printBits(unsigned char value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(uint16_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(uint32_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(uint64_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(char value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(int16_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(int32_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(int64_t value) {
	printBits((const unsigned char *)&value,sizeof(value));
}

void filedescriptor::printBits(const unsigned char *bits, uint64_t size) {
	for (uint64_t i=0; i<size; i++) {
		unsigned char byte=bits[i];
		for (int8_t j=7; j>=0; j--) {
			printf("%d",(byte>>j)&0x01);
		}
	}
}

#if defined(_WIN32) && defined(RUDIMENTS_HAVE_LONG_LONG)
static void invalidParameterHandler(const wchar_t *expression,
					const wchar_t *function,
					const wchar_t *file,
					unsigned int line,
					uintptr_t preserved) {
	// don't do anything
}
#endif

void *filedescriptor::getHandleFromFileDescriptor(int32_t fd) {
	#if defined(_WIN32)
		#if defined(RUDIMENTS_HAVE_LONG_LONG)
		if (fd<0) {
			return INVALID_HANDLE_VALUE;
		}
		_invalid_parameter_handler	oldiph=
			_set_invalid_parameter_handler(invalidParameterHandler);
		intptr_t	handle=_get_osfhandle(fd);
		_set_invalid_parameter_handler(oldiph);
		return (void *)handle;
		#else
		// this is dangerous, and can crash if fd is invalid,
		// but I'm not sure what else to do
		return (void *)_get_osfhandle(fd);
		#endif
	#else
		return NULL;
	#endif
}
