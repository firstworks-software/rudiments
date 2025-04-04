// Copyright (c) David Muse
// See the COPYING file for more information.

#include <rudiments/object.h>

#include <sys/types.h>

// some systems need this for key_t
#ifdef RUDIMENTS_HAVE_SYS_IPC_H
	#ifndef RUDIMENTS_SYS_IPC_H
	#define RUDIMENTS_SYS_IPC_H
		#include <sys/ipc.h>
	#endif
#endif

class sharedmemoryprivate;
