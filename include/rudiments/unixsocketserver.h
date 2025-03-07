// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETSERVER_H
#define RUDIMENTS_UNIXSOCKETSERVER_H

#include <rudiments/private/unixsocketserverincludes.h>

/** The unixsocketserver class allows you to write programs that can talk to
 *  other programs on the same machine over TCP stream sockets.
 * 
 *  Inet sockets (see the inetsocketserver class) can be used by clients and
 *  servers on the same machine as well, but Unix sockets generally perform
 *  better.
 *
 *  Some platforms (line Windows) don't support Unix sockets.  For those
 *  platforms, Unix sockets are simulated using Inet sockets on localhost
 *  by translating the socket filename to a port.  See
 *  unixsocketutil::filenameToPort() for details.
 * 
 *  The unixsocketserver class provides methods for setting up sockets and
 *  accepting client connections.  Its ultimate parent class: transport,
 *  provides methods for reading and writing data and closing connections.
 *  An immediate parent class: unixsocket provides methods for passing file
 *  descriptors between connected instances of unixservertsocket's and
 *  unixsocketclient's.
 * 
 *  If you need to listen on more than 1 socket at a time, you should use the 
 *  unixsocketserver class (and possibly the unixsocketserver class) in
 *  conjunction with the listener class. */
class RUDIMENTS_DLLSPEC unixsocketserver :
			public socketserver, private unixsocketutil {
	public:

		/** Creates an instance of the unixsocketserver class. */
		unixsocketserver();

		/** Deletes this instance of the unixsocketserver class. */
		virtual		~unixsocketserver();

		/** Convenience method that calls open(), bind() and listen().
		 *  If you need to set socket options or do anything else
		 *  special between those discrete steps then you should use
		 *  the methods individually.
		 *
		 *  Listen on "filename" and allow "backlog" connections to
		 *  pile up before refusing them.  Set the permissions on
		 *  "filename" using umask "mask".
		 * 
		 *  Returns true on success and false on failure. */
		bool	listen(const char *filename,
					mode_t mask,
					uint32_t backlog);

		/** Sets the umask that will be used when associating the
		 *  socket with a file. */
		void	setMask(mode_t mask);

		/** Returns the umask that will be used when associating the
		 *  socket with a file. */
		mode_t	getMask();

		/** Creates the actual socket and initializes the instance to
		 *  use "filename" when bind() is called.  The permissions on
		 *  "filename" will be set using umask "mask".
		 * 
		 *  Returns true on success and false on failure. */
		bool	open();

		/** Associates the socket with a file and sets permissions on
 		 *  that file.
		 * 
		 *  Returns true on success and false on failure. */
		bool	bind();

		/** Waits until a client connects then places that connection
		 *  in queue.  Up to "backlog" connections may be queued before
		 *  future conenctions are refused.
		 * 
		 *  Returns true on success and false on failure. */
		bool	listen();

		/** Removes the client connection from the queue and associates
		 *  a new socket with that connection.  Communication with the
		 *  client may be done over this new socket.  
		 *
		 *  Returns an unixsocketclient on success and NULL on
		 *  failure. */
		filedescriptor	*accept();

		/** Closes the socket.
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/unixsocketserver.h>
};

#endif
