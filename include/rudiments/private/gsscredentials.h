// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

		void	setPackageSpecificData(void *psd);

		gsscredentials(gsscredentials &g) {};
		gsscredentials	&operator=(gsscredentials &g) { return *this; };

	private:
		bool	acquire(const char *name,
					size_t namelen,
					const void *nametype);
		void	setStatus(uint32_t status, int32_t type);

		gsscredentialsprivate	*pvt;
