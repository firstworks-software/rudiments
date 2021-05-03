// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::dictionary() :
				dictionarycollection<keytype,valuetype>() {
	trackinsertionorder=true;
}

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::~dictionary() {
	clear();
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::setTrackInsertionOrder(
						bool trackinsertionorder) {
	if (!tree.getLength()) {
		this->trackinsertionorder=trackinsertionorder;
		return true;
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::getTrackInsertionOrder() {
	return trackinsertionorder;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValue(keytype key, valuetype value) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		dnode->setValue(value);
	} else {
		dnode=new dictionarynode<keytype,valuetype>(key,value);
		tree.insert(dnode);
		if (trackinsertionorder) {
			list.append(dnode);
		}
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValues(keytype *keys,
						valuetype *values) {
	keytype		*key=keys;
	valuetype	*value=values;
	while (*key) {
		setValue(*key,*value);
		key++;
		value++;
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValues(keytype const *keys,
						valuetype const *values) {
	if (keys && values) {
		keytype const	*key=keys;
		valuetype const	*value=values;
		while (*key) {
			setValue(*key,*value);
			key++;
			value++;
		}
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValues(keytype *keys,
						valuetype *values,
						uint64_t count) {
	if (keys && values) {
		keytype		*key=keys;
		valuetype	*value=values;
		for (uint64_t i=0; i<count; i++) {
			setValue(*key,*value);
			key++;
			value++;
		}
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValues(keytype const *keys,
						valuetype const *values,
						uint64_t count) {
	if (keys && values) {
		keytype const	*key=keys;
		valuetype const	*value=values;
		for (uint64_t i=0; i<count; i++) {
			setValue(*key,*value);
			key++;
			value++;
		}
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::setValues(
					dictionary<keytype,valuetype> *dict) {
	if (dict) {
		for (listnode<dictionarynode<keytype,valuetype> *>
					*node=dict->getList()->getFirst();
					node; node=node->getNext()) {
			setValue(node->getValue()->getKey(),
					node->getValue()->getValue());
		}
	}
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::getValue(keytype key,valuetype *value) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		*value=dnode->getValue();
		return true;
	}
	return false;
}

template <class keytype, class valuetype>
inline
valuetype dictionary<keytype,valuetype>::getValue(keytype key) {
	valuetype	value;
	if (getValue(key,&value)) {
		return value;
	}
	return (valuetype)0;
}

template <class keytype, class valuetype>
inline
dictionarynode<keytype,valuetype> *dictionary<keytype,valuetype>::
							getNode(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		return tnode->getValue();
	}
	return NULL;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::remove(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDelete(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDelete(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDeleteKey(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDeleteKey(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDeleteValue(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDeleteValue(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::
			removeAndDeleteKeyAndArrayDeleteValue(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::
			removeAndArrayDeleteKeyAndDeleteValue(keytype key) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::remove(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDelete(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDelete(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDeleteKey(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDeleteKey(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndDeleteKeyAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue()->getKey();
		delete[] tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::removeAndArrayDeleteKeyAndDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode<dictionarynode<keytype,valuetype> *> *tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete[] tnode->getValue()->getKey();
		delete tnode->getValue()->getValue();
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clear() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDelete() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDelete() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteKeys() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteKeys() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteValues() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteValues() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteKeysAndArrayDeleteValues() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteKeysAndDeleteValues() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

template <class keytype, class valuetype>
inline
linkedlist<keytype> *dictionary<keytype,valuetype>::getKeys() {
	linkedlist<keytype>	*keys=new linkedlist<keytype>();
	for (listnode<dictionarynode<keytype,valuetype> *>
		*node=getList()->getFirst(); node; node=node->getNext()) {
		keys->append(node->getValue()->getKey());
	}
	return keys;
}

template <class keytype, class valuetype>
inline
avltree<dictionarynode<keytype,valuetype> *>
			*dictionary<keytype,valuetype>::getTree() {
	return &tree;
}

template <class keytype, class valuetype>
inline
linkedlist<dictionarynode<keytype,valuetype> *>
			*dictionary<keytype,valuetype>::getList() {
	if (!trackinsertionorder) {
		list.clear();
		for (avltreenode<dictionarynode<keytype,valuetype> *>
			*node=tree.getFirst(); node; node=node->getNext()) {
			list.append(node->getValue());
		}
	}
	return &list;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::print() {
	for (listnode<dictionarynode<keytype,valuetype> *> *node=
				list.getFirst(); node; node=node->getNext()) {
		node->getValue()->print();
		stdoutput.printf("\n");
	}
}

template <class keytype, class valuetype>
inline
avltreenode<dictionarynode<keytype,valuetype> *>
			*dictionary<keytype,valuetype>::find(keytype key) {
	dictionarynode<keytype,valuetype>	fnode(key,(valuetype)0);
	return tree.find(&fnode);
}

template <class keytype, class valuetype>
inline
dictionarynode<keytype,valuetype>::dictionarynode(keytype key,
						valuetype value) : object() {
	this->key=key;
	this->value=value;
}

template <class keytype, class valuetype>
inline
dictionarynode<keytype,valuetype>::~dictionarynode() {}

template <class keytype, class valuetype>
inline
void dictionarynode<keytype,valuetype>::setKey(keytype key) {
	this->key=key;
}

template <class keytype, class valuetype>
inline
void dictionarynode<keytype,valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class keytype, class valuetype>
inline
keytype dictionarynode<keytype,valuetype>::getKey() const {
	return key;
}

template <class keytype, class valuetype>
inline
valuetype dictionarynode<keytype,valuetype>::getValue() const {
	return value;
}

template <class keytype, class valuetype>
inline
int32_t dictionarynode<keytype,valuetype>::compare(keytype testkey) const {
	return node_compare(key,testkey);
}

template <class keytype, class valuetype>
inline
int32_t dictionarynode<keytype,valuetype>::compare(
		dictionarynode<keytype,valuetype> *testnode) const {
	return node_compare(key,testnode->key);
}

template <class keytype, class valuetype>
inline
void dictionarynode<keytype,valuetype>::print() const {
	node_print(key);
	stdoutput.printf(":");
	node_print(value);
}


template <class keytype, class valuetype>
inline
int32_t node_compare(dictionarynode<keytype,valuetype> *value1,
			dictionarynode<keytype,valuetype> *value2) {
	return node_compare(value1->getKey(),value2->getKey());
}

template <class keytype, class valuetype>
inline
void node_compare(dictionarynode<keytype,valuetype> *value) {
	node_print(value);
}
