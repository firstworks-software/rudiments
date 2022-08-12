// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

			void	clear();

			virtual ssize_t	writeJson() const;
			virtual ssize_t	writeJson(bool indent) const;
			virtual ssize_t	writeJson(output *out) const;
			virtual ssize_t	writeJson(output *out,
							bool indent) const;

	friend class domnode;
	protected:
			virtual void	init(bool stringcacheenabled);
			virtual void	reset();

			const char	*cacheString(const char *string);
			void		unCacheString(const char *string);

			virtual ssize_t	writeNode(const domnode *dn,
						output *out,
						bool indent,
						uint16_t *indentlevel) const;
			virtual	ssize_t	writeIndent(output *out,
						uint16_t indent) const;
			virtual	ssize_t	safeWrite(output *out,
						const char *str) const;

				dom(const dom &x);
			dom	&operator=(const dom &x);

	private:
			domprivate	*pvt;
