// Copyright (c) David Muse
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
		virtual bool	supportsBlockingAndNonBlockingModes();

		/** If "nonblockingmode" is true then the socket put into in
		 *  non-blocking mode.  If "nonblockingmode" is false then
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
		virtual bool	setNonBlockingMode(bool nonblockingmode);

		/** Returns true if the socket is in non-blocking mode and
		 *  false otherwise. */
		virtual bool	getNonBlockingMode();

		/** Use the ioctl() system call to perform various low-level
		 *  file descriptor operations. */
		virtual int32_t	ioCtl(int32_t command, void *arg);


		/** If "enable" is set true then the socket will stay open for
		 *  "timeout" seconds even after close() is called to allow it
		 *  to receive any data that may still be buffered.
		 *
		 *  If "enable" is set false then the socket will close
		 *  normally, possibly discarding any buffered data.
		 *
		 *  The default is not to linger on close.
		 *  The default timeout is 0.
		 *
		 *  Note that while the "enabled" option might seen redundant,
		 *  as a timeout of 0 would seemingly have the same effect, in
		 *  fact setLingerOnClose(true,0) and setLingerOnClose(false,0)
		 *  exhibit different behaviors on different platforms.
		 *  Similarly, negative timeouts exhibit different behaviors on
		 *  different platforms.   For the sake of completeness, this
		 *  method provides both flags and allows a negative timeout
		 *  to be provided.
		 * 
		 *  Returns true on success and false on failure. */
		bool	setLingerOnClose(bool enable, int32_t timeout);

		/** Returns true if the socket is configured to linger on close
 		 *  and false otherwise. */
		bool	getLingerOnClose();

		/** Returns the current linger-on-close timeout. */
		int32_t	getLingerOnCloseTimeout();

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
