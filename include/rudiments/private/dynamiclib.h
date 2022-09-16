// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	private:
		dynamiclib(dynamiclib &d) {};
		dynamiclib	&operator=(dynamiclib &d) { return *this; };

		dynamiclibprivate	*pvt;
