// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_UNIXSOCKETUTIL_H
#define RUDIMENTS_UNIXSOCKETUTIL_H

#include <rudiments/private/unixsocketutilincludes.h>

/** The unixsocketutil class is just a base class for other classes that use
 *  unix sockets.  It is unlikely to need to be used directly. */
class RUDIMENTS_DLLSPEC unixsocketutil {
	public:

		/** Creates an instance of the unixsocketutil class. */
		unixsocketutil();

		/** Deletes this instance of the unixsocketutill class. */
		virtual	~unixsocketutil();

		/** Sets the filename that connect() will attempt to connect
		 *  to, or that listen() will listen on.
		 *
		 *  This is not directly useful within this instance but child
		 *  classes may use this value. */
		void	setFileName(const char *filename);

		/** Returns the filename that connect() will attempt to connect
		 *  to, or that listen() will listen on. */
		const char	*getFileName();

		/** Converts "filename" to a port number for faking unix
		 *  sockets.  If the filename starts with a number, then
		 *  that number is used.  Otherwise it starts with 10240 and
		 *  adds the ascii values of each character in the name.
		 *  The port number is not guaranteed to be unique for unique
		 *  names unless those names are based on unique numbers. */
		uint16_t	fileNameToPort(const char *filename);

	#include <rudiments/private/unixsocketutil.h>
};

#endif
