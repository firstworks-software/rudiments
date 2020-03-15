// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
			bool	parse();

			bool	parseWhitespace(char current, char *next);
			bool	parseExclamationComment(
						char current, char *next);
			void	parseRestOfLine(char ch,
						char *next, bool invalue);
			bool	parsePoundComment(char current, char *next);
			bool	parseKey(char current, char *next);
			bool	parseValue(char ch, char *next);

				propsax(const propsax &x);
			propsax	&operator=(const propsax &x);

			propsaxprivate	*pvt;
