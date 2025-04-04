// Copyright (c) David Muse
// See the COPYING file for more information.

		xmlsax(xmlsax &x) {};
		xmlsax	&operator=(xmlsax &x) { return *this; };

	private:
		bool	parse();
		bool	parseTag(char current, char *next);
		bool	parseTagName(char current,
					stringbuffer *ns,
					stringbuffer *name,
					char *next);
		char	parseComment(char current);
		char	parseCData(char current);
		char	parseAttribute(char current, char standalone);
		int32_t	getGeneralEntity(char breakchar);
		bool	parseText(char current, char *next);

		xmlsaxprivate	*pvt;
