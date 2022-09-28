// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			void	construct(bool stringcacheenabled);
			bool	reset();

				xmldom(xmldom &x);
			xmldom	&operator=(xmldom &x);

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
