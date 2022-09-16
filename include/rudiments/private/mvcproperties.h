// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		mvcproperties(mvcproperties &m) {};
		mvcproperties	&operator=(mvcproperties &m) { return *this; };

		void	buildDictionary();

		mvcpropertiesprivate	*pvt;
