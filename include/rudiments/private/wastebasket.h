// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		wastebasket(wastebasket &w) {};
		wastebasket	&operator=(wastebasket &w) { return *this; };

		wastebasketprivate	*pvt;
