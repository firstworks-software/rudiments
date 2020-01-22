// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	buildAllVariables();

	private:
		void	initSkinVariables();
		void	parseSkinVariableFile(const char *skinvarfilename);
		void	cleanSkinVariables();
		void	buildSkinVariableList();
		void	initUrlVariables();

		const char	*urlvars[URL_VARIABLE_COUNT+1];
		const char	*urlvals[URL_VARIABLE_COUNT+1];

		uint64_t	urlvariablecount;
		stringbuffer	applicationstr;
		stringbuffer	applicationpath;
		stringbuffer	applicationurl;
		stringbuffer	programstr;
		stringbuffer	programpath;
		stringbuffer	programurl;
		stringbuffer	skinstr;
		stringbuffer	skinpath;
		stringbuffer	skinurl;
		stringbuffer	modulestr;
		stringbuffer	modulepath;
		stringbuffer	moduleurl;
		stringbuffer	pagestr;
		stringbuffer	pagepath;
		stringbuffer	pageurl;

		namevaluepairs	skinvars;
		bool		dirtyskinvariables;
		const char	**skinvariablevars;
		const char	**skinvariablevals;
