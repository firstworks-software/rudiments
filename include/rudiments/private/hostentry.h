// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		hostentry(hostentry &h) {};
		hostentry	&operator=(hostentry &h) { return *this; };

		bool	initialize(const char *hostname,
					const char *address,
					int32_t len, int32_t type);
		hostentryprivate	*pvt;
