// Copyright (c) David Muse
// See the COPYING file for more information.

		socketclient(socketclient &c) {};
		socketclient	&operator=(socketclient &c) { return *this; };

	protected:
		int32_t	connect(const struct sockaddr *addr,
				socklen_t addrlen, int32_t sec, int32_t usec);
		ssize_t	lowLevelRead(void *buf, size_t count);
		ssize_t	lowLevelWrite(const void *buf, size_t count);
		int32_t	lowLevelClose();

	friend class url;
	private:
		socketclientprivate	*pvt;
