// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

	protected:
		dictionarynode<keytype,valuetype>	*getNode(keytype key);
		treenode< dictionarynode<keytype,valuetype> *>
							*find(keytype key);

		avltree<dictionarynode<keytype,valuetype> *>	tree;
		linkedlist<dictionarynode<keytype,valuetype> *>	list;
		linkedlist<keytype>				*keylist;

		bool	trackinsertionorder;
