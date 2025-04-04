// Copyright (c) David Muse
// See the COPYING file for more information.

		unixsocketclient(unixsocketclient &u) {};
		unixsocketclient	&operator=(unixsocketclient &u) {
			return *this;
		}

	private:
		unixsocketclientprivate	*pvt;
