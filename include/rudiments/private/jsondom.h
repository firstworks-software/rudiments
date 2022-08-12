// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class domnode;

			virtual ssize_t	writeJson() const;
			virtual ssize_t	writeJson(bool indent) const;
			virtual ssize_t	writeJson(output *out) const;
			virtual ssize_t	writeJson(output *out,
							bool indent) const;

	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			ssize_t	writeNode(const domnode *dn,
						output *out,
						bool indent,
						uint16_t *indentlevel) const;

				jsondom(const jsondom &x);
			jsondom	&operator=(const jsondom &x);

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
			const char	*getType(const domnode *dn) const;
			const char	*getValue(const domnode *dn) const;

			jsondomprivate	*pvt;
