// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
			jsonsax(jsonsax &x) {};
			jsonsax	&operator=(jsonsax &x) { return *this; };

			bool	parse();
			bool	parseObject(char current, char *next);
			bool	parseMember(char current, char *next);
			bool	parseStr(stringbuffer *str,
						char current, char *next);
			bool	parseValue(char current, char *next);
			bool	parseArray(char current, char *next);
			bool	parseNumber(stringbuffer *str,
						char current, char *next);
			bool	parseLiteral(const char *literal,
						char current, char *next);

			jsonsaxprivate	*pvt;
