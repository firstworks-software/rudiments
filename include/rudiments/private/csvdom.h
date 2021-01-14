// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	friend class csvdomnode;
	protected:
			void	init(bool stringcacheenabled);
			void	reset();

			domnode	*getColumn(uint64_t position);
			domnode	*getColumn(const char *name);

			bool	caseColumn(domnode *c, bool upper);
			bool	caseColumns(bool upper);

			void	setValue(domnode *node,
					const char *name, bool quoted);

			bool	renameColumn(const char *oldname,
					uint64_t position, bool useposition,
					const char *name, bool quoted);

			bool	insertColumnAt(uint64_t position,
						const char *name, bool quoted);

			bool	insertRowAt(uint64_t position);

			void	writeValue(output *out, domnode *value) const;

				csvdom(const csvdom &x);
			csvdom	&operator=(const csvdom &x);

	private:
			csvdomprivate	*pvt;
