// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		virtual	bool	connect()=0;
		virtual	bool	accept()=0;
		virtual	ssize_t	read(void *buf, ssize_t size)=0;
		virtual	ssize_t	write(const void *buf, ssize_t size)=0;
		virtual	ssize_t pending()=0;
		virtual	bool	close()=0;
		virtual ssize_t	getSizeMax()=0;
