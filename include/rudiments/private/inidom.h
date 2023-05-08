// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		inidom(inidom &x);
		inidom	&operator=(inidom &x);

	friend class domnode;
	protected:
		void	construct(bool stringcacheenabled);
		bool	reset();

		ssize_t	writeNode(domnode *dn,
					output *out,
					bool indent,
					uint16_t *indentlevel);

	private:
		bool	parse(input *in, domnode *parent,
					uint64_t position);
		bool	parseFile(const char *string,
					domnode *parent,
					uint64_t position);
		bool	parseString(const char *string,
					domnode *parent,
					uint64_t position);
		domnode	*whichNode();
		const char	*getType(domnode *dn);
		const char	*getValue(domnode *dn);

		inidomprivate	*pvt;
