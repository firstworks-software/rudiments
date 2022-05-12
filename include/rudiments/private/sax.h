// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

			void	reset();
			void	close();

			virtual bool	parse()=0;

			char	skipWhitespace(char current);
			char	getCharacter();
			void	parseFailed(const char *thing, const char *why);
	private:
				sax(const sax &x);
			sax	&operator=(const sax &x);

			bool	parseLocalFile(const char *filename);
			bool	parseRemoteFile(const char *filename);

			char	getCharacter(bool processignores);
			char	getCharacterBackwards();
			void	ignoreHeaderLines();
			void	ignoreFooterLines();
			bool	mapFile();

			saxprivate	*pvt;
