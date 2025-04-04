// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		void	clone(nodecollection<valuetype> *coll);
		void	insertBefore(listnode<valuetype> *node,
							valuetype value) {};
		void	insertBefore(listnode<valuetype> *node,
					listnode<valuetype> *newnode) {};
		void	moveBefore(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove) {};

		listnode<valuetype>	*first;
		listnode<valuetype>	*last;
		uint64_t		count;
