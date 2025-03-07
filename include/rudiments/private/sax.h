// Copyright (c) David Muse
// See the COPYING file for more information.

		sax(sax &s) {}
		sax	&operator=(sax &s) { return *this; };

	protected:
		bool	reset();
		void	close();

		virtual bool	parse()=0;

		char	skipWhitespace(char current);
		char	getCharacter();
		void	parseFailed(const char *thing, const char *why);

	private:
		bool	parseLocalFile(const char *filename);
		bool	parseRemoteFile(const char *filename);

		char	getCharacter(bool processignores);
		char	getCharacterBackwards();
		void	ignoreHeaderLines();
		void	ignoreFooterLines();
		bool	mapFile();

		saxprivate	*pvt;
