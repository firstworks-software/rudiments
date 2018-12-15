// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		static void	initTLS();

	private:
		static threadmutex	_tlsmutex;
		static bool		_initialized;
