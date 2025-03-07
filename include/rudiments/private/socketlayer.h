// Copyright (c) David Muse
// See the COPYING file for more information.

		virtual	bool	connect()=0;
		virtual	bool	accept()=0;
		virtual	ssize_t	read(void *buf, size_t size)=0;
		virtual	ssize_t	write(const void *buf, size_t size)=0;
		virtual	size_t	getPendingSize()=0;
		virtual	bool	close()=0;
		virtual size_t	getSizeMax()=0;
