// Copyright (c) David Muse
// See the COPYING file for more information

		dynamiclib(dynamiclib &d) {};
		dynamiclib	&operator=(dynamiclib &d) { return *this; };

	private:
		dynamiclibprivate	*pvt;
