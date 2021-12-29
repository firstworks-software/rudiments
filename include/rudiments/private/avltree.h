// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	insert(treenode<valuetype> *top,
				treenode<valuetype> *node,
				treenode<valuetype> **treetop);

		treenode<valuetype>	*top;
		treenode<valuetype>	*first;
		treenode<valuetype>	*last;
		uint64_t		length;
