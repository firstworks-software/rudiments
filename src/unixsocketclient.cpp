// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/unixsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/snooze.h>
#include <rudiments/process.h>
#include <rudiments/error.h>
#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	#include <rudiments/inetsocketclient.h>
#endif

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_UNDEFINED_SOCKET
	extern "C" int socket(int,int,int);
#endif

class unixsocketclientprivate {
	friend class unixsocketclient;
	private:
		#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
			inetsocketclient	_isc;
		#endif
};

unixsocketclient::unixsocketclient() : socketclient(), unixsocketutil() {
	pvt=new unixsocketclientprivate;
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		translateByteOrder();
	#endif
	type("unixsocketclient");
}

unixsocketclient::~unixsocketclient() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	close();
	delete pvt;
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		fd(-1);
	#endif
}

void unixsocketclient::setParameters(
			dictionary<const char *, const char *> *cd) {

	if (cd) {
		const char	*filename=NULL;
		cd->getValue("filename",&filename);

		setFilename((filename)?filename:"");
		client::setParameters(cd);
	}
}

int32_t unixsocketclient::connect() {

	close();

#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
	pvt->_isc.setHost("127.0.0.1");
	pvt->_isc.setPort(filenameToPort(getFilename()));
	pvt->_isc.setSocketLayer(getSocketLayer());
	pvt->_isc.setTimeoutSeconds(getTimeoutSeconds());
	pvt->_isc.setTimeoutMicroeconds(getTimeoutMicroeconds());
	pvt->_isc.setRetryWait(getRetryWait());
	pvt->_isc.setTries(getTries());
	int32_t	result=pvt->_isc.connect();
	if (result>-1) {
		fd(pvt->_isc.getFileDescriptor());
	}
	return result;
#else
	// set the filename to connect to
	getSun()->sun_family=AF_UNIX;
	charstring::copy(getSun()->sun_path,getFilename());

	// create a unix socket
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_SOCKET) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_SOCKET)
			fd(::socket(AF_UNIX,SOCK_STREAM,0));
		#else
			#error no socket or anything like it
		#endif
	} while (fd()==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (fd()==-1) {
		return RESULT_ERROR;
	}

	// Put the socket in blocking mode.  Most platforms create sockets in
	// blocking mode by default but OpenBSD doesn't appear to (at least in
	// version 4.9) so we'll force it to blocking-mode to be consistent.
	if (!useBlockingMode() &&
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		close();
		return RESULT_ERROR;
	}

	int32_t	retval=RESULT_ERROR;

	// try to connect, over and over for the specified number of times
	for (uint32_t counter=0; counter<getTries() || !getTries(); counter++) {

		if (process::getShutDownFlag()) {
			return RESULT_ERROR;
		}

		// wait the specified amount of time between reconnect tries
		// unless we're on the very first try
		if (counter) {
			snooze::macrosnooze(getRetryWait());
		}

		// attempt to connect
		retval=socketclient::connect(
			(struct sockaddr *)getSun(),sizeof(sockaddr_un),
			getTimeoutSeconds(),getTimeoutMicroseconds());
		if (retval==RESULT_SUCCESS) {
			return RESULT_SUCCESS;
		}
	}

	// if we're here, the connect failed...

	// save the error, as a successful call to close() will mask it
	int32_t	err=error::getErrorNumber();

	close();

	// restore the error
	if (err) {
		error::setErrorNumber(err);
	}

	return retval;
#endif
}

bool unixsocketclient::close() {
	#if defined(_WIN32) || defined(__VMS) || defined(_SYLLABLE)
		pvt->_isc.close();
		fd(-1);
	#endif
	return filedescriptor::close();
}
