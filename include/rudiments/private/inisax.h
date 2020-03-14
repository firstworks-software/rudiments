// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
			bool	parse();

			bool	parseWhitespace(char current, char *next);
			bool	parseSemicolonComment(char current, char *next);
			void	parseRestOfLine(char *next);
			bool	parsePoundComment(char current, char *next);
			bool	parseSection(char *next);
			void	parseRestOfSectionName(char *next);
			bool	parseKey(char current, char *next);
			bool	parseValue(char *next);

				inisax(const inisax &x);
			inisax	&operator=(const inisax &x);

			inisaxprivate	*pvt;
