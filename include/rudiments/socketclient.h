// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETCLIENT_H
#define RUDIMENTS_SOCKETCLIENT_H

#include <rudiments/private/socketclientincludes.h>

/** The socketclient class provides a base class for classes that implement
 *  the client side of a client-server communication model over a socket. */
class RUDIMENTS_DLLSPEC socketclient : public client {
	public:

		/** Creates an instance of the socketclient class. */
		socketclient();

		/** Deletes this instance of the socketclient class. */
		virtual	~socketclient();

		/** Returns true if the client socket supports
		 *  blocking/nonblocking modes and false otherwise. */
		virtual bool	supportsBlockingNonBlockingModes();

		/** Puts the client socket in non-blocking mode.  Returns true
		 *  on success and false on failure.  Returns false if the
		 *  system doesn't support blocking/nonblocking modes. */
		virtual bool	useNonBlockingMode();

		/** Puts the client socket in blocking mode.  Returns true on
		 *  success and false on failure.  Returns false if the system
		 *  doesn't support blocking/nonblocking modes. */
		virtual bool	useBlockingMode();

		/** Returns true if the file descriptor is in
		 *  non-blocking mode and false otherwise. */
		virtual bool	isUsingNonBlockingMode();

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg);

	#include <rudiments/private/socketclient.h>
};

#endif
