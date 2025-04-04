// Copyright (c) David Muse
// See the COPYING file for more information.

		signalset(signalset &s) {};
		signalset	&operator=(signalset &s) { return *this; };

	private:
		signalsetprivate	*pvt;
