// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SOCKETLAYER_H
#define RUDIMENTS_SOCKETLAYER_H

#include <rudiments/object.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

class	filedescriptor;

class RUDIMENTS_DLLSPEC socketlayer : virtual public object {
	public:
		virtual	void	setFileDescriptor(filedescriptor *fd)=0;
		virtual	filedescriptor	*getFileDescriptor()=0;

		virtual	bool	connect()=0;
		virtual	bool	accept()=0;
		virtual	ssize_t	read(void *buf, ssize_t size)=0;
		virtual	ssize_t	write(const void *buf, ssize_t size)=0;
		virtual	ssize_t pending()=0;
		virtual	bool	close()=0;
		virtual ssize_t	getSizeMax()=0;
};

#endif
