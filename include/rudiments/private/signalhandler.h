// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		signalhandler(signalhandler &s) {};
		signalhandler	&operator=(signalhandler &s) { return *this; };

		signalhandlerprivate	*pvt;
