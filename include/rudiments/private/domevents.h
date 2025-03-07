// Copyright (c) David Muse
// See the COPYING file for more information.

		domevents(domevents &d) {};
		domevents	&operator=(domevents &d) { return *this; };

	private:
		domnode	*findEvent(domnode *codetreenode);

		domeventsprivate	*pvt;
