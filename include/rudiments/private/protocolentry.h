// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		protocolentry(protocolentry &p) {};
		protocolentry	&operator=(protocolentry &p) { return *this; };

		bool	initialize(const char *protocolname, int32_t number);
		protocolentryprivate	*pvt;
