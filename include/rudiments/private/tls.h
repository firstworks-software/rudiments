// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		static void	initTLS();

	private:
		tls(tls &t) {};
		tls	&operator=(tls &t) { return *this; };
		
		static threadmutex	_tlsmutex;
		static bool		_initialized;
