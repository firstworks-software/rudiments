// Copyright (c) David Muse
// See the COPYING file for more information

	protected:
		void	clone(dictionary<keytype,valuetype> *a);
		void	clone(dictionarycollection<keytype,valuetype> *a);
		dictionarypair<keytype,valuetype>
					*getNode(keytype key);
		treenode< dictionarypair<keytype,valuetype> *>
					*find(keytype key);

		bool	trackinsertionorder;

		avltree<dictionarypair<keytype,valuetype> *>	tree;
		linkedlist<dictionarypair<keytype,valuetype> *>	list;
		linkedlist<keytype>				*keylist;
		dictionarypaircomparator<keytype,valuetype>	*comp;

