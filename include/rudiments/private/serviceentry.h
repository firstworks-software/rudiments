// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		serviceentry(serviceentry &s) {};
		serviceentry	&operator=(serviceentry &s) { return *this; };

		bool	init(const char *servicename, int32_t port,
							const char *protocol);
		serviceentryprivate	*pvt;
