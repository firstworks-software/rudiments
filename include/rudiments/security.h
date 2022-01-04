// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#ifndef RUDIMENTS_SECURITY_H
#define RUDIMENTS_SECURITY_H

#include <rudiments/object.h>

#ifdef RUDIMENTS_HAVE_SYS_TYPES_H
	#include <sys/types.h>
#endif

class	filedescriptor;

class RUDIMENTS_DLLSPEC securitycontext : virtual public object {
	public:
		securitycontext() {};
		virtual ~securitycontext() {};
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
