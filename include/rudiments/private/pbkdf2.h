// Copyright (c) David Muse
// See the COPYING file for more information.

		pbkdf2(pbkdf2 &s) {};
		pbkdf2	&operator=(pbkdf2 &s) { return *this; };

	private:
		void	setError(int32_t err);

		pbkdf2private	*pvt;
