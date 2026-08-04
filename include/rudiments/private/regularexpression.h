// Copyright (c) David Muse
// See the COPYING file for more information.

		regularexpression(regularexpression &r) {};
		regularexpression	&operator=(regularexpression &r) {
			return *this;
		};

	private:
		void	construct();

		regularexpressionprivate	*pvt;
