// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		memorymap(memorymap &m) {};
		memorymap	&operator=(memorymap &m) { return *this; };

		bool	mAdvise(unsigned char *start,
					size_t length, int32_t advice);

		static	bool	mLockAll(int32_t flags);

		memorymapprivate	*pvt;
