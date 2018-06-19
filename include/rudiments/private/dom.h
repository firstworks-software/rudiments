// Copyright (c) 2018 David Muse
// See the COPYING file for more information.

	friend class domnode;
	protected:
			virtual void	init(bool stringcacheenabled);
			virtual void	reset();

			const char	*cacheString(const char *string);
			void		unCacheString(const char *string);

	private:

				dom(const dom &x);
			dom	&operator=(const dom &x);

			domprivate	*pvt;
