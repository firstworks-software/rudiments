// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		regularexpression(regularexpression &r) {};
		regularexpression	&operator=(regularexpression &r) {
			return *this;
		};

		void	init();
		void	clear();

		regularexpressionprivate	*pvt;
