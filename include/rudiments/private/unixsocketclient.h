// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		unixsocketclient(unixsocketclient &u) {};
		unixsocketclient	&operator=(unixsocketclient &u) {
			return *this;
		}

	private:
		unixsocketclientprivate	*pvt;
