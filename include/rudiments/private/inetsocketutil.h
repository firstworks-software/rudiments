// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		sockaddr_in	*getSin();

	private:
		inetsocketutil(inetsocketutil &i) {};
		inetsocketutil	&operator=(inetsocketutil &i) { return *this; };

		inetsocketutilprivate	*pvt;
