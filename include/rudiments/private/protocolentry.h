// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		protocolentry(protocolentry &p) {};
		protocolentry	&operator=(protocolentry &p) { return *this; };

	private:
		bool	open(const char *protocolname, int32_t number);

		protocolentryprivate	*pvt;
