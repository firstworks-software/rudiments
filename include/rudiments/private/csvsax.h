// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

			csvsax(csvsax &x);
			csvsax	&operator=(csvsax &x);

			void	reset();
			bool	parse();

	private:
			csvsaxprivate	*pvt;
