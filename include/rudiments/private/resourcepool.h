// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		resourcepool(resourcepool &r) {};
		resourcepool	&operator=(resourcepool &r) { return *this; };

	protected:
		virtual valuetype	*createResource();

		void	clearDelegate();

		uint64_t	minval;
		uint64_t	maxval;
		uint64_t	growby;
		uint64_t	total;
		bool		initialized;

		linkedlist<valuetype *>	initiallist;
		avltree<valuetype *>	initialtree;
		linkedlist<valuetype *>	ondemandlist;
		avltree<valuetype *>	ondemandtree;

		threadmutex		*mtx;
