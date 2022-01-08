// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		void	clone(const dictionary<keytype,valuetype> *a);
		void	clone(const dictionarycollection<keytype,valuetype> *a);
		dictionarypair<keytype,valuetype>
					*getNode(keytype key) const;
		treenode< dictionarypair<keytype,valuetype> *>
					*find(keytype key) const;

		bool	trackinsertionorder;

		avltree<dictionarypair<keytype,valuetype> *>	tree;
		linkedlist<dictionarypair<keytype,valuetype> *>	list;
		mutable linkedlist<keytype>			*keylist;
		dictionarypaircomparator<keytype,valuetype>	*comp;
