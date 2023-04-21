// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		bool	socketLayerAccept(filedescriptor *sock);

		ssize_t	lowLevelRead(void *buf, size_t count);
		ssize_t	lowLevelWrite(const void *buf, size_t count);
		int32_t	lowLevelClose();

	private:
		socketserver(socketserver &s) {};
		socketserver	&operator=(socketserver &s) { return *this; };

		bool	setLingerOnClose(int32_t timeout, int32_t onoff);
		bool	setReuseAddresses(int32_t onoff);

		socketserverprivate	*pvt;
