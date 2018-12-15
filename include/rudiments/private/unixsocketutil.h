// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_filename();
		sockaddr_un	*_sun();
	private:
		unixsocketutilprivate	*pvt;
