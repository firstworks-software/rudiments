// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/inetsocketserver.h>
#include <rudiments/inetsocketclient.h>
#include <rudiments/charstring.h>
#include <rudiments/bytestring.h>
#include <rudiments/process.h>
#include <rudiments/error.h>

#include <rudiments/private/winsock.h>

#ifdef RUDIMENTS_HAVE_ARPA_INET_H
	#include <arpa/inet.h>
#endif
#ifdef RUDIMENTS_HAVE_NETDB_H
	#include <netdb.h>
#endif

#ifndef RUDIMENTS_HAVE_IN_ADDR_T
typedef unsigned long	in_addr_t;
#endif

#ifndef INADDR_NONE
	#define INADDR_NONE ((in_addr_t)-1)
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_SOCKET
	extern "C" int socket(int,int,int);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_BIND
	extern "C" int bind(int,struct sockaddr *,int);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_GETSOCKNAME
	extern "C" int getsockname(int,struct sockaddr *,int *);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_LISTEN
	extern "C" int listen(int,int);
#endif

#ifdef RUDIMENTS_HAVE_UNDEFINED_ACCEPT
	extern "C" int accept(int,struct sockaddr *,int *);
#endif

class inetsocketserverprivate {
	friend class inetsocketserver;
	private:
};

inetsocketserver::inetsocketserver() : socketserver(), inetsocketutil() {
	pvt=new inetsocketserverprivate;
	setTranslateByteOrder(true);
	type("inetsocketserver");
}

inetsocketserver::~inetsocketserver() {
	// filedescriptor's destructor calls close(), why the close() call here?
	// Destructors don't always call overridden methods, but rather the
	// version defined in that class.  In this case, lowLevelClose() needs
	// to be called from this class, especially on Windows where
	// closesocket() must be called rather than close() to prevent a crash.
	// If close() is called here, it will eventually call this method's
	// lowLevelClose() rather than filedescriptor::lowLevelClose().
	close();
	delete pvt;
}

bool inetsocketserver::listen(const char *address, uint16_t port,
							uint32_t backlog) {
	setHost(address);
	setPort(port);
	setBacklog(backlog);
	return open() && setReuseAddresses(true) && bind() && listen();
}

bool inetsocketserver::open() {

	close();

	uint16_t	port=getPort();
	const char	*address=getHost();

	// initialize a socket address structure
	bytestring::zero(getSin(),sizeof(sockaddr_in));
	getSin()->sin_family=AF_INET;
	getSin()->sin_port=convertHostToNet(port);

	// if a specific address was passed in, bind to it only,
	// otherwise bind to all addresses
	if (!charstring::isNullOrEmpty(address) &&
			charstring::compare(address,"0.0.0.0")) {
		#if defined(RUDIMENTS_HAVE_INET_ATON)
			in_addr	ia;
			if (!inet_aton(address,&ia)) {
				return false;
			}
			getSin()->sin_addr.s_addr=ia.s_addr;
		#elif defined(RUDIMENTS_HAVE_INET_ADDR)
			in_addr_t	saddr=inet_addr(address);
			if (saddr==INADDR_NONE) {
				return false;
			}
			getSin()->sin_addr.s_addr=saddr;
		#else
			#error no inet_aton or anything like it
		#endif
	} else {
		getSin()->sin_addr.s_addr=convertHostToNet((uint32_t)INADDR_ANY);
	}

	// create the socket
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_SOCKET) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_SOCKET)
			fd(::socket(AF_INET,SOCK_STREAM,0));
		#else
			#error no socket or anything like it
		#endif
	} while (fd()==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (fd()==-1) {
		return false;
	}

	// Put the socket in blocking mode.  Most platforms create sockets in
	// blocking mode by default but OpenBSD doesn't appear to (at least in
	// version 4.9) so we'll force it to blocking-mode to be consistent.
	if (!setNonBlockingMode(false) &&
			error::getErrorNumber()
			#ifdef ENOTSUP
			&& error::getErrorNumber()!=ENOTSUP
			#endif
			#ifdef EOPNOTSUPP
			&& error::getErrorNumber()!=EOPNOTSUPP
			#endif
			) {
		close();
		return false;
	}

	return true;
}

bool inetsocketserver::bind() {

	// bind the socket
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_BIND) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_BIND)
			result=::bind(fd(),
					(struct sockaddr *)getSin(),
					sizeof(sockaddr_in));
		#else
			#error no bind or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	if (result==-1) {
		return false;
	}

	// get the actual port number if an arbitrary port was requested
	if (!getPort()) {

		// initialize a socket address structure
		sockaddr_in			socknamesin;
		RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(socknamesin);
		bytestring::zero(&socknamesin,sizeof(socknamesin));

		int32_t	result;
		error::clearError();
		do {
			#if defined(RUDIMENTS_HAVE_GETSOCKNAME) || \
				defined(RUDIMENTS_HAVE_UNDEFINED_GETSOCKNAME)
				result=getsockname(fd(),
					(struct sockaddr *)&socknamesin,&size);
			#else
				#error no getsockname or anything like it
			#endif
		} while (result==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
		if (result!=-1) {
			setPort((uint16_t)ntohs(socknamesin.sin_port));
		}
	}
	return true;
}

bool inetsocketserver::listen() {
	int32_t	result;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_LISTEN) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_LISTEN)
			result=::listen(fd(),(int)getBacklog());
		#else
			#error no listen or anything like it
		#endif
	} while (result==-1 && error::getErrorNumber()==EINTR &&
					!process::getShutDownFlag());
	return !result;
}

filedescriptor *inetsocketserver::accept() {

	// initialize a socket address structure
	sockaddr_in			clientsin;
	RUDIMENTS_SOCKLEN_OR_SIZE_T	size=sizeof(clientsin);
	bytestring::zero(&clientsin,sizeof(clientsin));

	// accept on the socket
	int32_t	clientsock;
	error::clearError();
	do {
		#if defined(RUDIMENTS_HAVE_ACCEPT) || \
			defined(RUDIMENTS_HAVE_UNDEFINED_ACCEPT)
			clientsock=::accept(fd(),
				(struct sockaddr *)&clientsin,&size);
		#else
			#error no accept or anything like it
		#endif

	} while (clientsock==-1 && error::getErrorNumber()==EINTR &&
						!process::getShutDownFlag());
	if (clientsock==-1) {
		return NULL;
	}

	inetsocketclient	*returnsock=new inetsocketclient;
	returnsock->setFileDescriptor(clientsock);

	// set the client socket to the same blocking/non-blocking
	// mode as the server socket
	if (!(returnsock->setNonBlockingMode(getNonBlockingMode())) &&
				error::getErrorNumber()
				#ifdef ENOTSUP
				&& error::getErrorNumber()!=ENOTSUP
				#endif
				#ifdef EOPNOTSUPP
				&& error::getErrorNumber()!=EOPNOTSUPP
				#endif
				) {
		delete returnsock;
		return NULL;
	}

	// handle socketlayer-accept if necessary
	if (!socketLayerAccept(returnsock)) {
		delete returnsock;
		return NULL;
	}

	return returnsock;
}
