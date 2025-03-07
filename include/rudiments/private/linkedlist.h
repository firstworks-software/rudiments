// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		void	clone(nodecollection<valuetype> *coll);
		void	move(listnode<valuetype> *node,
				listnode<valuetype> *nodetomove,
				bool before);

		listnode<valuetype>	*first;
		listnode<valuetype>	*last;
		uint64_t		count;
