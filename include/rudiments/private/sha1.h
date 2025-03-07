// Copyright (c) David Muse
// See the COPYING file for more information.

		sha1(sha1 &s) {};
		sha1	&operator=(sha1 &s) { return *this; };

	private:
		void	setError(int32_t err);

		sha1private	*pvt;
