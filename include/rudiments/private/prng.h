// Copyright (c) David Muse
// See the COPYING file for more information.

		prng(prng &r) {};
		prng	&operator=(prng &r) { return *this; };

	private:
		prngprivate	*pvt;
