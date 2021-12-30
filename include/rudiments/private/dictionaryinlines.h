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
		void	print() const;

	private:
		keytype		key;
		valuetype	value;
};

template <class keytype, class valuetype>
class dictionarynodecomparator : public comparator {
	public:
		int32_t compare(object *value1, object *value2);
};

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::dictionary() :
				dictionarycollection<keytype,valuetype>() {
	trackinsertionorder=true;
	keylist=NULL;
	comp=new dictionarynodecomparator<keytype,valuetype>();
	tree.setComparator(comp);

#ifdef DARWIN_GCC_2952_HACKS
	return;

	// Various methods here use linkedlist and linkedlistnode, but if the
	// calling app doens't happen to call one of them, then they get
	// declared but the code that defines them never gets pulled in.
	// On Darwin platforms, when using gcc 2.95.2 (and possibly other
	// versions) this results in various undefined symbols.  Adding some
	// calls here, after the return causes the code to be included and the
	// symbols to be defined.  It's called here, after the return so that
	// it never actually gets executed.  The old compiler doesn't complain
	// about that.
	keylist=getKeys();
	setValues(NULL);
#endif
}

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::~dictionary() {
	clear();
	delete comp;
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
			for (treenode<dictionarynode<keytype,valuetype> *>
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
	treenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
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
		for (treenode<dictionarynode<keytype,valuetype> *>
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
	treenode<dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		if (this->collection::managekeys) {
			node_delete_value(tnode->getValue()->getKey());
		} else if (this->collection::managearraykeys) {
			node_delete_array_value(tnode->getValue()->getKey());
		}
		if (this->collection::managevalues) {
			node_delete_value(tnode->getValue()->getValue());
		} else if (this->collection::managearrayvalues) {
			node_delete_array_value(tnode->getValue()->getValue());
		}
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clear() {
	for (treenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		if (this->collection::managekeys) {
			node_delete_value(node->getValue()->getKey());
		} else if (this->collection::managearraykeys) {
			node_delete_array_value(node->getValue()->getKey());
		}
		if (this->collection::managevalues) {
			node_delete_value(node->getValue()->getValue());
		} else if (this->collection::managearrayvalues) {
			node_delete_array_value(node->getValue()->getValue());
		}
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
	for (treenode<dictionarynode<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		node_print(node->getValue());
		stdoutput.printf("\n");
	}
}

template <class keytype, class valuetype>
inline
treenode<dictionarynode<keytype,valuetype> *>
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
void node_print(dictionarynode<keytype,valuetype> *value) {
	node_print(value->getKey());
	stdoutput.printf(":");
	node_print(value->getValue());
}

template <class keytype, class valuetype>
inline
void dictionarynode<keytype,valuetype>::print() const {
	node_print(this);
}

template <class keytype, class valuetype>
inline
int32_t dictionarynodecomparator<keytype,valuetype>::compare(
							object *value1,
							object *value2) {
	dictionarynode<keytype,valuetype> *v1=
		(dictionarynode<keytype,valuetype> *)value1;
	dictionarynode<keytype,valuetype> *v2=
		(dictionarynode<keytype,valuetype> *)value2;
	return comparator::compare(v1->getKey(),v2->getKey());
}
