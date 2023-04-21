// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class url;
	protected:
		int32_t	connect(const struct sockaddr *addr,
				socklen_t addrlen, int32_t sec, int32_t usec);
		ssize_t	lowLevelRead(void *buf, size_t count);
		ssize_t	lowLevelWrite(const void *buf, size_t count);
		int32_t	lowLevelClose();

	private:
		socketclient(socketclient &c) {};
		socketclient	&operator=(socketclient &c) { return *this; };

		socketclientprivate	*pvt;
