// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/socketserver.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#include <rudiments/stdio.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_SYS_IOCTL_H
	#include <sys/ioctl.h>
#endif
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	#include <sys/socket.h>
#endif
#ifdef RUDIMENTS_HAVE_UNISTD_H
	#include <unistd.h>
#endif

class socketserverprivate {
	friend class socketserver;
	private:
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		bool	_nonblockingmode;
		#endif
};

socketserver::socketserver() : server() {
	pvt=new socketserverprivate;
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
	pvt->_nonblockingmode=false;
	#endif
	type("socketserver");
	winsock::initWinsock();
}

socketserver::~socketserver() {
	delete pvt;
}

bool socketserver::supportsBlockingAndNonBlockingModes() {
	#if defined(FIONBIO) && !defined(RUDIMENTS_DISABLE_FIONBIO)
		return true;
	#else
		return filedescriptor::
			supportsBlockingAndNonBlockingModes();
	#endif
}

bool socketserver::setUseNonBlockingMode(bool usenonblockingmode) {
	// The posix way of setting blocking/non-blocking mode is to use
	// fcntl(), which is what the filedescriptor class does, but this
	// doesn't work for sockets on all platforms.  If FIONBIO is defined,
	// then we'll try that with an ioctl first, and fall back to fcntl() if
	// that fails with one of:
	// * "Inappropriate ioctl for device"
	// * "Device not configured"
	// * "Operation not supported"
	#if defined(FIONBIO)
		int32_t	nonblocking=(usenonblockingmode)?1:0;
		bool	retval=(ioCtl(FIONBIO,&nonblocking)!=-1);
		if (!retval && (error::getErrorNumber()==ENOTTY
				#if defined(ENXIO)
				|| error::getErrorNumber()==ENXIO
				#endif
				#if defined(EOPNOTSUPP)
				|| error::getErrorNumber()==EOPNOTSUPP
				#endif
				)) {
			retval=filedescriptor::setUseNonBlockingMode(
							usenonblockingmode);
		}
		#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		if (retval) {
			pvt->_nonblockingmode=usenonblockingmode
		}
		#endif
		return retval;
	#else
		return filedescriptor::setUseNonBlockingMode(
						usenonblockingmode);
	#endif
}

bool socketserver::getIsUsingNonBlockingMode() {
	// There is no way to determine the blocking mode using ioctl's and
	// FIONBIO.  On posix platforms, independent of whether blocking mode
	// was set using an ioctl or fcntl, you can use an fcntl to get the
	// blocking mode.  On other platforms, you just have to keep track of
	// what mode you set it to and hope that the program only uses methods
	// from this class to set the mode.
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		return pvt->_nonblockingmode;
	#else
		return filedescriptor::getIsUsingNonBlockingMode();
	#endif
}

int32_t socketserver::ioCtl(int32_t cmd, void *arg) {
	#if defined(RUDIMENTS_HAVE_IOCTLSOCKET)
		int32_t	result;
		error::clearError();
		do {
			result=ioctlsocket(fd(),cmd,(u_long *)arg);
		} while (getRetryInterruptedIoctl() &&
				result==-1 &&
				error::getErrorNumber()==EINTR &&
				!process::getShutDownFlag());
		return result;
	#else
		return filedescriptor::ioCtl(cmd,arg);
	#endif
}

bool socketserver::lingerOnClose(int32_t timeout) {
	return setLingerOnClose(timeout,1);
}

bool socketserver::dontLingerOnClose() {
	return setLingerOnClose(0,0);
}

bool socketserver::setReuseAddresses(bool reuse) {
	return setReuseAddresses((reuse)?1:0);
}

bool socketserver::setLingerOnClose(int32_t timeout, int32_t onoff) {
	#ifdef SO_LINGER
		struct	linger	ling;
		ling.l_onoff=onoff;
		ling.l_linger=timeout;
		return !setSockOpt(SOL_SOCKET,SO_LINGER,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&ling,
					sizeof(struct linger));
	#else
		RUDIMENTS_SET_ENOSYS
		return false;
	#endif
}

bool socketserver::setReuseAddresses(int32_t onoff) {
	int32_t	value=onoff;
	return !setSockOpt(SOL_SOCKET,SO_REUSEADDR,
				(RUDIMENTS_SETSOCKOPT_OPTVAL_TYPE)&value,
					(socklen_t)sizeof(int));
}

bool socketserver::listen(int32_t backlog) {
	int32_t	result;
	error::clearError();
	do {
		result=::listen(fd(),backlog);
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return !result;
}

bool socketserver::socketLayerAccept(filedescriptor *sock) {
	if (socklr()) {
		sock->setSocketLayer(socklr());
		socklr()->setFileDescriptor(sock);
		if (!socklr()->accept()) {
			sock->setSocketLayer(NULL);
			socklr()->setFileDescriptor(NULL);
			return false;
		}
	}
	return true;
}

ssize_t socketserver::lowLevelRead(void *buf, size_t count) {
	return ::recv(fd(),
			#ifdef RUDIMENTS_HAVE_RECV_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

ssize_t socketserver::lowLevelWrite(const void *buf, size_t count) {
	return ::send(fd(),
			#ifdef RUDIMENTS_HAVE_SEND_WITH_VOID
			buf,
			#else
			(char *)buf,
			#endif
			count,0);
}

int32_t socketserver::lowLevelClose() {
	#if defined(RUDIMENTS_HAVE_CLOSESOCKET)
		return closesocket(fd());
	#elif defined(RUDIMENTS_HAVE__CLOSE)
		return _close(fd());
	#elif defined(RUDIMENTS_HAVE_CLOSE)
		return ::close(fd());
	#else
		#error no closesocket, close or anything like it
	#endif
}
