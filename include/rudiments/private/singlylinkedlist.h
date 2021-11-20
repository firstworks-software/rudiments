// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

		void	insertBefore(listnode<valuetype> *node,
							valuetype value) {};
		void	insertBefore(listnode<valuetype> *node,
					listnode<valuetype> *newnode) {};
		void	moveBefore(listnode<valuetype> *node,
					listnode<valuetype> *nodetomove) {};

	protected:
		listnode<valuetype>	*first;
		listnode<valuetype>	*last;
		uint64_t		length;
