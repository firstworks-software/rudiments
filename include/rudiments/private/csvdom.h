// Copyright (c) David Muse
// See the COPYING file for more information.

	friend class csvdomnode;
	protected:

		csvdom(csvdom &x);
		csvdom	&operator=(csvdom &x);

		void	construct(bool stringcacheenabled);
		bool	reset();

		domnode	*getColumn(uint64_t position);
		domnode	*getColumn(const char *name);

		bool	caseColumn(domnode *c, bool upper);
		bool	caseColumns(bool upper);

		bool	trimNode(domnode *node, bool left, bool right);
		bool	trimColumns(bool left, bool right);

		void	setValue(domnode *node, const char *name, bool quoted);
		void	setValue(domnode *node, const char *name,
					size_t namelength, bool quoted);

		bool	renameColumn(const char *oldname,
				uint64_t position, bool useposition,
				const char *name, bool quoted);

		bool	insertColumnAt(uint64_t position,
					const char *name, bool quoted);

		bool	insertRecordAt(uint64_t position);

		bool	trimField(uint64_t record, uint64_t column,
						bool left, bool right);

		void	trimFields(bool left, bool right);

		ssize_t	writeNode(domnode *dn,
					output *out,
					bool indent,
					uint16_t *indentlevel);
		ssize_t	writeValue(output *out, domnode *value);

	private:
		csvdomprivate	*pvt;
