// Copyright (c) David Muse
// See the COPYING file for more information.

		sha512(sha512 &s) {};
		sha512	&operator=(sha512 &s) { return *this; };

	private:
		void	setError(int32_t err);

		sha512private	*pvt;
