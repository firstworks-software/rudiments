// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		jsondom(jsondom &x);
		jsondom	&operator=(jsondom &x);

		virtual ssize_t	writeJson();
		virtual ssize_t	writeJson(bool indent);
		virtual ssize_t	writeJson(output *out);
		virtual ssize_t	writeJson(output *out, bool indent);

	protected:
		void	construct(bool stringcacheenabled);
		bool	reset();

		ssize_t	writeNode(domnode *dn,
					output *out,
					bool indent,
					uint16_t *indentlevel);

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

		jsondomprivate	*pvt;
