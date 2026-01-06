// Copyright (c) David Muse
// See the COPYING file for more information.

		dh(dh &s) {};
		dh	&operator=(dh &s) { return *this; };

	private:
		void	setError(int32_t err);
		void	setError(dherror_t err);

		dhprivate	*pvt;
