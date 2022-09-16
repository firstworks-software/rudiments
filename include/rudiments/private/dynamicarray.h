// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void		construct(uint64_t initiallength,
						uint64_t increment);
		void		clone(dynamicarray<valuetype> &v);
		void		clone(arraycollection<valuetype> &v);
		void		extend(uint64_t length);
		size_t		findExtentStartIndex(uint64_t index);
		valuetype	&find(uint64_t index);
		void		deleteManagedValues();

		linkedlist<valuetype *>	extents;
		listnode<valuetype *>	*curext;
		uint64_t		curind;

		uint64_t	initlen;
		uint64_t	inclen;
		uint64_t	totallen;
		uint64_t	lastlen;
