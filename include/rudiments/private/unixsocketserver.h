// Copyright (c) David Muse
// See the COPYING file for more information.

		unixsocketserver(unixsocketserver &u) {};
		unixsocketserver	&operator=(unixsocketserver &u) {
			return *this;
		}

	private:
		unixsocketserverprivate	*pvt;
