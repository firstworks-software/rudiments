// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETCLIENT_H
#define RUDIMENTS_UNIXSOCKETCLIENT_H

#include <rudiments/private/unixsocketclientincludes.h>

/** The unixsocketclient class allows you to write programs that can talk to
 *  other programs on the same machine over TCP stream sockets.
 * 
 *  Inet sockets (see the inetsocketclient class) can be used by clients and
 *  servers on the same machine as well, but Unix sockets generally perform
 *  better.
 *
 *  Some platforms (line Windows) don't support Unix sockets.  For those
 *  platforms, Unix sockets are simulated using Inet sockets on localhost
 *  by translating the socket filename to a port.  See
 *  unixsocketutil::filenameToPort() for details.
 * 
 *  The unixsocketclient class provides methods for connecting to servers.
 *  Its ultimate parent class: filedescriptor provides methods for reading and 
 *  writing data and closing connections. */
class RUDIMENTS_DLLSPEC unixsocketclient :
			public socketclient, public unixsocketutil {
	public:

		/** Creates an instance of the unixsocketclient class. */
		unixsocketclient();

		/** Deletes this instance of unixsocketclient. */
		virtual		~unixsocketclient();

		/** Queries "cd" for "filename", "timeoutsec", "timeoutusec",
		 *  "retrywait" and "tries" and configures this instancec to
		 *  use them when connect() is called. */
		void	setParameters(
				dictionary<const char *, const char *> *cd);

		/** Attempts to connect to the "filename" set earlier.
		 *  If the connection fails, it will retry, for a total of
		 *  "tries" attempts, waiting "retrywait" seconds between
		 *  each attempt.
		 * 
		 *  Setting "tries" to 0 will cause it to try to 
		 *  connect indefinitely.  Setting "retrywait" to 0
		 *  will cause it to try to connect over and over
		 *  as fast as possible (not recommended).
		 * 
		 *  Each attempt to connect will wait "timeoutsec"
		 *  seconds and "timeoutusec" microseconds for the
		 *  connect to succeed.  Specifying -1 for either
		 *  parameter will cause the attempt to wait until the
		 *  underlying protocol times out which may be up to 2
		 *  minutes.
		 * 
		 *  Returns RESULT_SUCCESS on success and RESULT_ERROR
		 *  on failure. */
		int32_t	connect();

		/** Closes the socket.
		 *  Returns true on success and false on failure. */
		bool	close();

	#include <rudiments/private/unixsocketclient.h>
};

#endif
