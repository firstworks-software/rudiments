// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		dictionarypair<keytype,valuetype>
					*getNode(keytype key) const;
		treenode< dictionarypair<keytype,valuetype> *>
					*find(keytype key) const;

		avltree<dictionarypair<keytype,valuetype> *>	tree;
		linkedlist<dictionarypair<keytype,valuetype> *>	list;
		mutable linkedlist<keytype>			*keylist;
		dictionarypaircomparator<keytype,valuetype>	*comp;

		bool	trackinsertionorder;
