// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			void	write(const domnode *dn,
						output *out,
						bool indent,
						uint16_t *indentlevel) const;
			void	writeAndEscape(output *out,
						const char *value) const;

				propdom(const propdom &x);
			propdom	&operator=(const propdom &x);

	private:
			bool	parseFile(const char *string,
						domnode *parent,
						uint64_t position);
			bool	parseString(const char *string,
						domnode *parent,
						uint64_t position);
			domnode	*whichNode();
			const char	*getType(const domnode *dn) const;
			const char	*getValue(const domnode *dn) const;

			propdomprivate	*pvt;
