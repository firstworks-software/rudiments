// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		virtual valuetype	*createResource();

	private:
		void	clone(const resourcepool<valuetype> *r);

		uint64_t	min;
		uint64_t	max;
		uint64_t	growby;
		uint64_t	total;
		bool		initialized;

		linkedlist<valuetype *>	initiallist;
		avltree<valuetype *>	initialtree;
		linkedlist<valuetype *>	ondemandlist;
		avltree<valuetype *>	ondemandtree;

		threadmutex		*mtx;
