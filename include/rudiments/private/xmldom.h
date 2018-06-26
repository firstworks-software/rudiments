// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			void	write(const domnode *dn,
						output *out,
						bool indent,
						uint16_t *indentlevel) const;

				xmldom(const xmldom &x);
			xmldom	&operator=(const xmldom &x);

	private:
			bool	parseFile(const char *string,
						domnode *parent,
						uint64_t position);
			bool	parseString(const char *string,
						domnode *parent,
						uint64_t position);
			void	insertChild(domnode *child);
			void	safeWrite(output *out, const char *str) const;

			xmldomprivate	*pvt;
