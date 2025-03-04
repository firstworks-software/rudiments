// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/private/dll.h>
#include <rudiments/private/inttypes.h>

#include <rudiments/input.h>
#include <rudiments/output.h>
#include <rudiments/socketlayer.h>
#include <rudiments/resultcodes.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

#ifdef RUDIMENTS_HAVE_SYS_SOCKET_H
	// IRIX needs _XOPEN_SOURCE but OpenBSD can't
	// find select() if it's still defined later
	#ifndef _XOPEN_SOURCE
		#define _XOPEN_SOURCE
		#define UN_XOPEN_SOURCE
	#endif
        // some platforms don't have ifdef wrappers inside of sys/socket.h
	#ifndef RUDIMENTS_SYS_SOCKET_H
		#define RUDIMENTS_SYS_SOCKET_H
		#include <sys/socket.h>
	#endif
	#ifdef UN_XOPEN_SOURCE
		#undef _XOPEN_SOURCE
	#endif
#endif

#ifdef RUDIMENTS_HAVE_STDARG_H
	#include <stdarg.h>
#endif

class filedescriptorprivate;
