// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	private:
			bool	parse();
			bool	parseTag(char current, char *next);
			void	parseTagFailed();
			bool	parseTagName(char current,
						stringbuffer *ns,
						stringbuffer *name,
						char *next);
			char	parseComment(char current);
			char	parseCData(char current);
			char	parseAttribute(char current, char standalone);
			int32_t	getGeneralEntity(char breakchar);
			void	parseAttributeFailed();
			bool	parseText(char current, char *next);
			void	parseTextFailed();

				xmlsax(const xmlsax &x);
			xmlsax	&operator=(const xmlsax &x);

			xmlsaxprivate	*pvt;
