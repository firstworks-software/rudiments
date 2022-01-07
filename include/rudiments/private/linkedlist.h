// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	clone(const listcollection<valuetype> *list);
		void	move(listnode<valuetype> *node,
				listnode<valuetype> *nodetomove,
				bool before);

		listnode<valuetype>	*first;
		listnode<valuetype>	*last;
		uint64_t		length;
