// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		unixsocketutil(unixsocketutil &u) {};
		unixsocketutil	&operator=(unixsocketutil &u) { return *this; };

	protected:
		sockaddr_un	*getSun();

	private:
		unixsocketutilprivate	*pvt;
