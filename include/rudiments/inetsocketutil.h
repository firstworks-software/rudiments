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
		virtual		~inetsocketutil();

		/** Initializes the class to use "address" and "port".
		 *  This is not directly useful within this class but child
		 *  classes may use these values to decide where to connect to
		 *  or what to listen on. */
		void		initialize(const char *address, uint16_t port);

	#include <rudiments/private/inetsocketutil.h>
};

#endif
