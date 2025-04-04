// Copyright (c) David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETLAYER_H
#define RUDIMENTS_SOCKETLAYER_H

#include <rudiments/private/socketlayerincludes.h>

/** The socketlayer class defines an interface for protocols that sit on top
 *  of TCP stream sockets, such as TLS, GSS and Web Sockets. */
class RUDIMENTS_DLLSPEC socketlayer : virtual public object {
	public:

		/** Sets the file desecriptor to be used by the sockeylayer. */
		virtual	void	setFileDescriptor(filedescriptor *fd)=0;

		/** Returns the file descriptor set by a previous call to
		 *  setFileDescriptor() or NULL if none has been set. */
		virtual	filedescriptor	*getFileDescriptor()=0;

	#include <rudiments/private/socketlayer.h>
};

#endif
