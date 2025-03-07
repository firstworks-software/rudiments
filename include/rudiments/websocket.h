// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_TLS_H
#define RUDIMENTS_TLS_H

#include <rudiments/private/websocketincludes.h>

/** The websocket class implements the socketlayer interface for Web Sockets. */
class RUDIMENTS_DLLSPEC websocket : public socketlayer {
	public:
		/** Creates a new instance of the websocket class. */
		websocket();

		/** Deletes this instance of the websocket class. */
		virtual	~websocket();

		/** Sets the file descriptor that will be used to communicate
		 *  with the peer during the next call to connect() or
		 *  accept(). */
		void		setFileDescriptor(filedescriptor *fd);

		/** Returns the file descrptor that will be used to communicate
 		 *  with the peer during the next call to connect() or
 		 *  accept(). */
		filedescriptor	*getFileDescriptor();


		/** Initiates a security context with the server with whom
		 *  a connection is already established across the
		 *  filedescriptor previously set using setFileDescriptor().
		 *
		 *  Note that if this instance is set as the current security
		 *  context of a child of the socketclient class, then this
		 *  method is called implicitly during a successful call to
		 *  connect().
		 *
		 *  Returns true on success and false on failure. */
		bool	connect();


		/** Accepts a security context from the client with whom
		 *  a connection is already established across the
		 *  filedescriptor previously set using setFileDescriptor().
		 *
		 *  Note that if this instance is set as the current GSS
		 *  context of a child of the socketserver class, then this
		 *  method is called implicitly during a successful call to
		 *  accept().
		 *
		 *  Returns true on success and false on failure. */
		bool	accept();

		/** Reads from the file descriptor previously configured by
		 *  setFileDescriptor() into "buf" until "size" bytes have
		 *  been read.  Returns the number of bytes that were written
		 *  to "buf" or RESULT_ERROR if an error occurred. */
		ssize_t	read(void *buf, size_t size);

		/** Writes "size" bytes from "buf" to the file descriptor
		 *  previously configured by setFileDescriptor().
		 *  Returns the number of bytes that were written or
		 *  RESULT_ERROR if an error occurred. */
		ssize_t	write(const void *buf, size_t size);

		/** Pings the peer with "size" bytes of "buf" data.  Returns
		 *  true if the peer responds with the same data or false if
		 *  the peer doesn't respond or responds with different
		 *  data. */
		bool ping(const byte_t *buf, size_t size);

		/** Returns the number of bytes that are buffered and available
		 *  for immediate read. */
		size_t getPendingSize();

		/** Releases any security context established during the
		 *  previous call to connect() or accept(). */
		bool	close();


		/** Returns the maximum number of bytes that can be written or
		 *  read in a single call to write() or read(). */
		size_t	getSizeMax();

	#include <rudiments/private/websocket.h>
};

#endif
