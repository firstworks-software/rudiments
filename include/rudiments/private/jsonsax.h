// Copyright (c) David Muse
// See the COPYING file for more information.

		jsonsax(jsonsax &x) {};
		jsonsax	&operator=(jsonsax &x) { return *this; };

	private:
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
