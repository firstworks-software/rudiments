// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_INETSOCKETUTIL_H
#define RUDIMENTS_INETSOCKETUTIL_H

#include <rudiments/private/inetsocketutilincludes.h>

/** The inetsocketutil class is just a base class for other classes that use
 *  inet sockets.  It is unlikely to need to be used directly. */
class RUDIMENTS_DLLSPEC inetsocketutil {
	public:

		/** Creates an instance of the inetsocketutil class. */
		inetsocketutil();

		/** Deletes this instance of the inetsocketutil class. */
		virtual	~inetsocketutil();

		/** Sets the host (or address) that connect() will attempt
		 *  to connect to or the address that listen() will listen on.
		 *
		 *  This is not directly useful within this instance but child
		 *  classes may use this values. */
		void	setHost(const char *host);

		/** Sets the port that connect() will attempt to connect to or
		 *  that listen() wil listen on.
		 *
		 *  This is not directly useful within this instance but child
		 *  classes may use this value. */
		void	setPort(uint16_t port);

		/** Returns the host (or address) that connect() will attempt
		 *  to connect to or the address that listen() will listen
		 *  on. */
		const char	*getHost();

		/** Returns the port that connect() will attempt to connect
		 *  to or that listen() will listen on. */
		uint16_t	getPort();

	#include <rudiments/private/inetsocketutil.h>
};

#endif
