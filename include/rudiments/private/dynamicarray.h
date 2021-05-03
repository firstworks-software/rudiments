// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

	private:
		void		init(uint64_t initiallength,
						uint64_t increment);
		void		dynamicarrayClone(
					const dynamicarray<valuetype> &v);
		void		extend(uint64_t len);
		valuetype	&find(uint64_t index);
		void		clearExtentList();

		linkedlist< valuetype *>	extents;
		listnode< valuetype *>		*curext;
		uint64_t	curind;
		uint64_t	initial;
		uint64_t	extlength;
		uint64_t	len;
		uint64_t	length;
