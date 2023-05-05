// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		uint64_t	getLength();

		bool	clear();

		virtual ssize_t	writeJson();
		virtual ssize_t	writeJson(bool indent);
		virtual ssize_t	writeJson(output *out);
		virtual ssize_t	writeJson(output *out, bool indent);

	friend class domnode;
	protected:
		virtual void	construct(bool stringcacheenabled);
		virtual bool	reset();

		const char	*cacheString(const char *string);
		void		unCacheString(const char *string);

		virtual ssize_t	writeNode(domnode *dn,
					output *out,
					bool indent,
					uint16_t *indentlevel);
		virtual	ssize_t	writeIndent(output *out,
					uint16_t indent);
		virtual	ssize_t	safeWrite(output *out,
					const char *str);

		dom(dom &x);
		dom	&operator=(dom &x);

	private:
		domprivate	*pvt;
