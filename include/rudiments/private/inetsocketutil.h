// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	protected:
		const char	*_address();
		uint16_t	*_port();
		sockaddr_in	*_sin();
	private:
		inetsocketutilprivate	*pvt;
