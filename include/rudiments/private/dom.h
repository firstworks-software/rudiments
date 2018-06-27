// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			virtual void	init(bool stringcacheenabled);
			virtual void	reset();

			const char	*cacheString(const char *string);
			void		unCacheString(const char *string);

			virtual void	write(const domnode *dn,
						output *out,
						bool indent,
						uint16_t *indentlevel) const;
			virtual	void	writeIndent(output *out,
						uint16_t indent) const;

				dom(const dom &x);
			dom	&operator=(const dom &x);

	private:
			domprivate	*pvt;
