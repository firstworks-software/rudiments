// Copyright (c) 2002 David Muse
// See the COPYING file for more information.

			virtual void	reset();
			virtual void	close();
			virtual bool	parse()=0;
			virtual bool	parseLocalFile(const char *filename);
			virtual bool	parseRemoteFile(const char *filename);
			virtual char	skipWhitespace(char current);
			virtual char	getCharacter();
			virtual bool	mapFile();

			stringbuffer	*getErrorString();
			uint32_t	getLine();
	private:
				sax(const sax &x);
			sax	&operator=(const sax &x);

			saxprivate	*pvt;
