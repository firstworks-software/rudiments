// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	buildList(const char ***vars, const char ***vals,
							namevaluepairs *nvp);
		void	cleanUp(const char ***vars, const char ***vals,
							namevaluepairs *nvp);

		typedef enum {
			get_request,
			head_request,
			post_request
		} httprequestmethod;

		virtual void	parseQueryString(httprequestmethod method);

		virtual void	buildAllVariables();

		bool		dirtyallvars;
		uint64_t	allvariablecount;
		const char	**allvars;
		const char	**allvals;

		httpserverapi	*sapi;

	private:

		void	initCookies();
		void	initParameters();
		void	initFileNames();
		void	parseMultipart();
		void	getNewNames(stringbuffer **name, 
					stringbuffer **filename,
					stringbuffer **mimetype);
		void	getTempFile(const char *filename, 
					file **tempfile, 
					char **tempfilename);
		stringbuffer	*getName();
		stringbuffer	*getFileName();
		stringbuffer	*getSomeKindOfName(char c);
		stringbuffer	*getMimeType();

		void	cleanParameters();
		void	cleanFiles();
		void	cleanCookies();

		void	buildParameterList();
		void	buildFileNamesList();
		void	buildCookieList();
		void	buildFileNameList();


		namevaluepairs	parameters;
		bool		dirtyparameters;
		const char	**parametervars;
		const char	**parametervals;

		size_t		tmpdirlen;
		const char	*boundary;
		dictionary<char *, fileparameter *>	fileparameters;
		bool		dirtyfilenames;
		const char	**filenames;

		namevaluepairs	cookies;
		bool		dirtycookies;
		const char	**cookievars;
		const char	**cookievals;
