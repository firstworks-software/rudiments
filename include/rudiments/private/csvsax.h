// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		bool	reset();
		bool	parse();

		csvsax(csvsax &x) {};
		csvsax	&operator=(csvsax &x) { return *this; };

	private:
		csvsaxprivate	*pvt;
