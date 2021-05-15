// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class keytype, class valuetype>
class dictionarynode : public object {
	public:
		dictionarynode(keytype key, valuetype value);
		virtual	~dictionarynode();

		void	setKey(keytype key);
		void	setValue(valuetype value);
		keytype		getKey() const;
		valuetype	getValue() const;
		int32_t	compare(keytype testkey) const;
		int32_t	compare(
			dictionarynode<keytype,valuetype> *testnode) const;
		void	print() const;

	private:
		keytype		key;
		valuetype	value;
};

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::dictionary() :
				dictionarycollection<keytype,valuetype>() {
	trackinsertionorder=true;
	keylist=NULL;
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
		if (dict->trackinsertionorder) {
			for (listnode<dictionarynode<keytype,valuetype> *>
						*node=dict->list.getFirst();
						node; node=node->getNext()) {
				setValue(node->getValue()->getKey(),
						node->getValue()->getValue());
			}
		} else {
			for (avltreenode<dictionarynode<keytype,valuetype> *>
						*node=dict->tree.getFirst();
						node; node=node->getNext()) {
				setValue(node->getValue()->getKey(),
						node->getValue()->getValue());
			}
		}
	}
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::getValue(keytype key, valuetype *value) {
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
bool dictionary<keytype,valuetype>::getKey(keytype key, keytype *k) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		*k=dnode->getKey();
		return true;
	}
	return false;
}

template <class keytype, class valuetype>
inline
keytype dictionary<keytype,valuetype>::getKey(keytype key) {
	keytype	k;
	if (getKey(key,&k)) {
		return k;
	}
	return (keytype)0;
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
linkedlist<keytype> *dictionary<keytype,valuetype>::getKeys() {
	delete keylist;
	keylist=new linkedlist<keytype>();
	if (trackinsertionorder) {
		for (listnode<dictionarynode<keytype,valuetype> *>
			*node=list.getFirst(); node; node=node->getNext()) {
			keylist->append(node->getValue()->getKey());
		}
	} else {
		for (avltreenode<dictionarynode<keytype,valuetype> *>
			*node=tree.getFirst(); node; node=node->getNext()) {
			keylist->append(node->getValue()->getKey());
		}
	}
	return keylist;
}

template <class keytype, class valuetype>
inline
uint64_t dictionary<keytype,valuetype>::getLength() {
	return tree.getLength();
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
void dictionary<keytype,valuetype>::clear() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDelete() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDelete() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteKeys() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteKeys() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteValues() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteValues() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndDeleteKeysAndArrayDeleteValues() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clearAndArrayDeleteKeysAndDeleteValues() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::print() {
	for (avltreenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
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
void node_print(dictionarynode<keytype,valuetype> *value) {
	node_print(value);
}
