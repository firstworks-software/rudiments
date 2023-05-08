// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		regularexpression(regularexpression &r) {};
		regularexpression	&operator=(regularexpression &r) {
			return *this;
		};

		bool	clear();

	private:
		void	construct();

		regularexpressionprivate	*pvt;
