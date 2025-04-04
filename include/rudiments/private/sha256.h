// Copyright (c) David Muse
// See the COPYING file for more information.

		sha256(sha256 &s) {};
		sha256	&operator=(sha256 &s) { return *this; };

	private:
		void	setError(int32_t err);

		sha256private	*pvt;
