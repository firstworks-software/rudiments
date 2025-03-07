// Copyright (c) David Muse
// See the COPYING file for more information.

		md5(md5 &m) {}
		md5	&operator=(md5 &m) { return *this; }

	private:
		void	setError(int32_t err);

		md5private	*pvt;
