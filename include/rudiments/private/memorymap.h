// Copyright (c) David Muse
// See the COPYING file for more information.

		memorymap(memorymap &m) {};
		memorymap	&operator=(memorymap &m) { return *this; };

	private:
		bool	mAdvise(byte_t *start, size_t length, int32_t advice);

		static	bool	mLockAll(int32_t flags);

		memorymapprivate	*pvt;
