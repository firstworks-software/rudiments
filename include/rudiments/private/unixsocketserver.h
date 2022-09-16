// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		unixsocketserver(unixsocketserver &u) {};
		unixsocketserver	&operator=(unixsocketserver &u) {
			return *this;
		}

		unixsocketserverprivate	*pvt;
