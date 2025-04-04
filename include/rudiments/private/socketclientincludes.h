// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/config.h>
#include <sys/types.h>

// for socklen_t
#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	// some platforms don't have ifdef wrappers inside of sys/socket.h
	#ifndef RUDIMENTS_SYS_SOCKET_H
		#define RUDIMENTS_SYS_SOCKET_H 1
		#include <sys/socket.h>
	#endif
#endif
#ifdef RUDIMENTS_HAVE_WINSOCK2_H
	#include <winsock2.h>
#endif

#include <rudiments/client.h>

class socketclientprivate;
