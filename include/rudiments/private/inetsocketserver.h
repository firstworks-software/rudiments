// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		inetsocketserver(inetsocketserver &i) {};
		inetsocketserver	&operator=(inetsocketserver &i) {
			return *this;
		}

	private:
		inetsocketserverprivate	*pvt;
