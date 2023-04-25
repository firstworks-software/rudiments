// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETSERVER_H
#define RUDIMENTS_SOCKETSERVER_H

#include <rudiments/private/socketserverincludes.h>

/** The socketserver class provides a base class for classes that implement
 *  the server side of a client-server communication model over a socket. */
class RUDIMENTS_DLLSPEC socketserver : public server {
	public:

		/** Creates an instance of the socketserver class. */
		socketserver();

		/** Deletes this instance of the socketserver class. */
		virtual		~socketserver();

		/** Returns true if the system supports blocking/nonblocking
		 *  modes and false otherwise. */
		virtual bool	getBlockingAndNonBlockingModesAreSupported();

		/** If "usenonblockingmode" is true then the socket put into in
		 *  non-blocking mode.  If "usenonblockingmode" is false then
		 *  the socket is put into blocking mode.
		 *
		 *  The default for most sockets is to be in blocking mode,
		 *  however this is not guaranteed.  It is good practice to
		 *  specifically set the mode.
		 *
		 *  Returns true on success and false on failure.
		 *
		 *  Returns false if the system doesn't support
		 *  blocking/nonblocking modes. */
		virtual bool	setUseNonBlockingMode(bool usenonblockingmode);

		/** Returns true if the socket is in non-blocking mode and
		 *  false otherwise. */
		virtual bool	getIsUsingNonBlockingMode();

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg);


		/** Instructs the socket to stay open for
		 *  "timeout" seconds even after close() is
		 *  called to allow it to receive any data that
		 *  may still be buffered.  The default is not
		 *  to linger on close.
		 * 
		 *  Returns true on success and false on failure. */
		bool	lingerOnClose(int32_t timeout);

		/** Instructs the socket to close immediately
		 *  when close() is called, dumping any data
		 *  that may still be buffered but that it
		 *  may not have received.  This is the default.
		 * 
		 *  Returns true on success and false on failure. */
		bool	dontLingerOnClose();

		/** If "reuse" is true then sockets in the TIME_WAIT state are
		 *  allowed to be reused.  If "reuse" is false, then sockets in
		 *  the TIME_WAIT state are not reused.  Defaults to false.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setReuseAddresses(bool reuse);

		/** Waits until a client connects then places
		 *  that connection in queue.  Up to "backlog"
		 *  connections may be queued before future
		 *  conenctions are refused.
		 * 
		 *  Returns true on success and false on failure. */
		virtual	bool	listen(int32_t backlog);

	#include <rudiments/private/socketserver.h>
};

#endif
