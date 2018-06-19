// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			virtual bool	tagStart(const char *ns,
							const char *name);
			virtual bool	attributeName(const char *name);
			virtual bool	attributeValue(const char *value);
			virtual bool	text(const char *string);
			virtual bool	tagEnd(const char *ns,
							const char *name);
			virtual bool	comment(const char *string);
			virtual bool	cdata(const char *string);

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

			xmldomprivate	*pvt;
