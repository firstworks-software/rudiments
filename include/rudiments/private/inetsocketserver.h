// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		inetsocketserver(inetsocketserver &i) {};
		inetsocketserver	&operator=(inetsocketserver &i) {
			return *this;
		}

		inetsocketserverprivate	*pvt;
