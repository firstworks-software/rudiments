// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		xmldom(xmldom &x);
		xmldom	&operator=(xmldom &x);

	friend class domnode;
	protected:
		void	construct(bool stringcacheenabled);
		bool	reset();

	private:
		bool	parse(input *in, domnode *parent,
					uint64_t position);
		bool	parseFile(const char *string,
					domnode *parent,
					uint64_t position);
		bool	parseString(const char *string,
					domnode *parent,
					uint64_t position);
		void	insertChild(domnode *child);

		xmldomprivate	*pvt;
