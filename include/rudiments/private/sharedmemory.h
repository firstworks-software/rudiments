// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		sharedmemory(sharedmemory &s) {};
		sharedmemory	&operator=(sharedmemory &s) { return *this; };

	private:
		bool	forceRemove();

		sharedmemoryprivate	*pvt;
