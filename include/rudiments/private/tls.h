// Copyright (c) David Muse
// See the COPYING file for more information.

		static void	initTLS();

	private:
		static threadmutex	_tlsmutex;
		static bool		_initialized;
