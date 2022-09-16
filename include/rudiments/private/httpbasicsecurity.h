// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

	private:
		httpbasicsecurity(httpbasicsecurity &h) {};
		httpbasicsecurity	&operator=(httpbasicsecurity &h) {
			return *this;
		};

		httpbasicsecurityprivate	*pvt;
