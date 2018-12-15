// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		// FIXME: should be possible to copy a mutex
			threadmutex(const threadmutex &m);
		threadmutex	&operator=(const threadmutex &m);

		threadmutexprivate	*pvt;
