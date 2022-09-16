// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		domevents(domevents &d) {};
		domevents	&operator=(domevents &d) { return *this; };

		domnode	*findEvent(domnode *codetreenode);

		domeventsprivate	*pvt;
