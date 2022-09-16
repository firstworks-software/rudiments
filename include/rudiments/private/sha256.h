// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		sha256(sha256 &s) {};
		sha256	&operator=(sha256 &s) { return *this; };

		void	setError(int32_t err);

		sha256private	*pvt;
