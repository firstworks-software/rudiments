// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		sha1(sha1 &s) {};
		sha1	&operator=(sha1 &s) { return *this; };

		void	setError(int32_t err);

		sha1private	*pvt;
