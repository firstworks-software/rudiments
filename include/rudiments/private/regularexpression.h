// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void	init();
		// FIXME: it should be possible to copy a regularexpression,
		// but how do you copy *expr and *extra?
				regularexpression(const regularexpression &r);
		regularexpression	&operator=(const regularexpression &r);

		regularexpressionprivate	*pvt;
