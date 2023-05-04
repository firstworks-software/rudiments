// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		sharedmemory(sharedmemory &s) {};
		sharedmemory	&operator=(sharedmemory &s) { return *this; };

		bool	forceRemove();

		sharedmemoryprivate	*pvt;
