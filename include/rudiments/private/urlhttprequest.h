// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		urlhttprequest(urlhttprequest &u) : httprequest(NULL) {};
		urlhttprequest	&operator=(urlhttprequest &u) { return *this; };

	protected:
		void	buildAllVariables();

	private:
		void	initSkinVariables();
		void	parseSkinVariableFile(const char *skinvarfilename);
		void	cleanSkinVariables();
		void	buildSkinVariableList();
		void	initUrlVariables();

		urlhttprequestprivate	*pvt;
