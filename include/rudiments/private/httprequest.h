// Copyright (c) David Muse
// See the COPYING file for more information

		httprequest(httprequest &h) {};
		httprequest	&operator=(httprequest &h) { return *this; };

	protected:
		void	buildList(const char ***vars, const char ***vals,
					dictionary<char *, char *> *nvp);
		void	cleanUp(const char ***vars, const char ***vals,
					dictionary<char *, char *> *nvp);

		typedef enum {
			get_request,
			head_request,
			post_request
		} httprequestmethod;

		virtual void	parseQueryString(httprequestmethod method);

		virtual void	buildAllVariables();

		bool		dirtyAllVars();
		void		dirtyAllVars(bool dirtyallvars);
		uint64_t	allVariableCount();
		void		allVariableCount(uint64_t allvariablecount);
		const char	***allVars();
		const char	***allVals();

	private:
		void	initCookies();
		void	initParameters();
		void	initFileNames();
		void	parseMultipart();
		void	parseJsonOrXml();
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

		bool	isThingAllowed(const char *thing,
					bool allowemptything,
					const char *deniedthings,
					const char *allowedthings);

		httprequestprivate	*pvt;
