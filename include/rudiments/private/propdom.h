// Copyright (c) David Muse
// See the COPYING file for more information.

		propdom(propdom &x);
		propdom	&operator=(propdom &x);

	protected:
		void	construct(bool stringcacheenabled);
		bool	reset();

		ssize_t	writeNode(domnode *dn,
					output *out,
					bool indent,
					uint16_t *indentlevel);
		ssize_t	writeAndEscape(output *out,
					const char *value);

	friend class domnode;
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

		propdomprivate	*pvt;
