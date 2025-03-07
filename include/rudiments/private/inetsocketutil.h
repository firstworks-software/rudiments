// Copyright (c) David Muse
// See the COPYING file for more information.

		inetsocketutil(inetsocketutil &i) {};
		inetsocketutil	&operator=(inetsocketutil &i) { return *this; };

	protected:
		sockaddr_in	*getSin();

	private:
		inetsocketutilprivate	*pvt;
