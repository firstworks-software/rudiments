// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		inetsocketclient(inetsocketclient &i) {};
		inetsocketclient	&operator=(inetsocketclient &i) {
			return *this;
		}

	private:
		inetsocketclientprivate	*pvt;
