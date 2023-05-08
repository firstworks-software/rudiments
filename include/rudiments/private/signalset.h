// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		signalset(signalset &s) {};
		signalset	&operator=(signalset &s) { return *this; };

	private:
		signalsetprivate	*pvt;
