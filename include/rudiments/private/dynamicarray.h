// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void		init(uint64_t initiallength,
						uint64_t increment);
		void		clone(const dynamicarray<valuetype> *v);
		void		clone(const arraycollection<valuetype> *v);
		void		extend(uint64_t length);
		size_t		findExtentStartIndex(uint64_t index) const;
		valuetype	&find(uint64_t index);
		valuetype	find(uint64_t index) const;

		linkedlist<valuetype *>	extents;

		mutable listnode<valuetype *>	*curext;
		mutable uint64_t		curind;

		uint64_t	initlen;
		uint64_t	inclen;
		uint64_t	totallen;
		uint64_t	lastlen;
