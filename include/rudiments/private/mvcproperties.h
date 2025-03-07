// Copyright (c) David Muse
// See the COPYING file for more information.

		mvcproperties(mvcproperties &m) {};
		mvcproperties	&operator=(mvcproperties &m) { return *this; };

	private:
		void	buildDictionary();

		mvcpropertiesprivate	*pvt;
