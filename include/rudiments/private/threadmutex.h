// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		threadmutex(threadmutex &m) {};
		threadmutex	&operator=(threadmutex &m) { return *this; };

		threadmutexprivate	*pvt;
