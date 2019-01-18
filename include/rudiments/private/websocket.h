// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:

		bool	acceptInternal();
		ssize_t	readInternal(void *buf, ssize_t count);
		ssize_t	writeInternal(const void *buf, ssize_t count);
		ssize_t	copyOut(void *buf, ssize_t count);

		websocketprivate	*pvt;
