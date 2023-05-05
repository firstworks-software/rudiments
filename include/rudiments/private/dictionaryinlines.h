// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

template <class keytype, class valuetype>
class dictionarypair : public object {
	public:
		dictionarypair(keytype key, valuetype value);

		void	setKey(keytype key);
		void	setValue(valuetype value);
		keytype		getKey();
		keytype		&getKeyRef();
		valuetype	getValue();
		valuetype	&getValueRef();

	private:
		keytype		key;
		valuetype	value;
};

template <class keytype, class valuetype>
class dictionarypaircomparator : public comparator {
	public:
		int32_t compare(object *value1, object *value2);
};

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::dictionary() :
	dictionarycollection<keytype,valuetype>(),
	trackinsertionorder(true),
	keylist(NULL) {

	comp=new dictionarypaircomparator<keytype,valuetype>();
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
dictionary<keytype,valuetype>::dictionary(
			dictionary<keytype,valuetype> &a) :
			dictionarycollection<keytype,valuetype>(a),
			keylist(NULL) {

	comp=new dictionarypaircomparator<keytype,valuetype>();
	clone(&a);

#ifdef DARWIN_GCC_2952_HACKS
	return;

	// see note above
	keylist=getKeys();
	setValues(NULL);
#endif
}

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype>::dictionary(
			dictionarycollection<keytype,valuetype> &a) :
			dictionarycollection<keytype,valuetype>(a),
			keylist(NULL) {

	comp=new dictionarypaircomparator<keytype,valuetype>();
	clone(&a);

#ifdef DARWIN_GCC_2952_HACKS
	return;

	// see note above
	keylist=getKeys();
	setValues(NULL);
#endif
}

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype> &dictionary<keytype,valuetype>::
			operator=(dictionary<keytype,valuetype> &a) {
	if (this!=&a) {
		clear();
		dictionarycollection<keytype,valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class keytype, class valuetype>
inline
dictionary<keytype,valuetype> &dictionary<keytype,valuetype>::
			operator=(dictionarycollection<keytype,valuetype> &a) {
	if (this!=&a) {
		clear();
		dictionarycollection<keytype,valuetype>::operator=(a);
		clone(&a);
	}
	return *this;
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clone(dictionary<keytype,valuetype> *a) {

	trackinsertionorder=a->getTrackInsertionOrder();

	// comp should already exist at this point
	tree.setComparator(comp);

	// We can't just set tree=a->tree and list=a->list because
	// we don't have a node_duplicate_value() for a
	// dictionarypair.  We can't just implement one because it
	// would need to know whether to duplicate the key and value
	// individually.
	//
	// So, we'll manually build both by running through a.tree and
	// adding its key/value pairs to ourself.
	for (treenode<dictionarypair<keytype,valuetype> *>
			*node=a->tree.getFirst(); node; node=node->getNext()) {
		setValue(node_duplicate_value(
				&(node->getValue()->getKeyRef()),
				this->getManageKeys(),
				this->getManageArrayKeys()),
			node_duplicate_value(
				&(node->getValue()->getValueRef()),
				this->getManageValues(),
				this->getManageArrayValues()));
	}

	// if a's keylist was already built then go ahead and build ours too
	if (a->keylist) {
		getKeys();
	}
}

template <class keytype, class valuetype>
inline
void dictionary<keytype,valuetype>::clone(
				dictionarycollection<keytype,valuetype> *a) {

	trackinsertionorder=a->getTrackInsertionOrder();

	// comp should already exist at this point
	tree.setComparator(comp);

	for (listnode<keytype> *node=a->getKeys()->getFirst();
					node; node=node->getNext()) {
		keytype		key=node->getValue();
		valuetype	value=a->getValue(key);
		setValue(node_duplicate_value(&key,
				this->getManageKeys(),
				this->getManageArrayKeys()),
			node_duplicate_value(&value,
				this->getManageValues(),
				this->getManageArrayValues()));
	}

	// a's keylist may have been built as a result of the getKeys() call
	// above, so go ahead and build ours too
	getKeys();
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
	if (!tree.getCount()) {
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
	dictionarypair<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		dnode->setValue(value);
	} else {
		dnode=new dictionarypair<keytype,valuetype>(key,value);
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
			for (listnode<dictionarypair<keytype,valuetype> *>
						*node=dict->list.getFirst();
						node; node=node->getNext()) {
				setValue(node->getValue()->getKey(),
						node->getValue()->getValue());
			}
		} else {
			for (treenode<dictionarypair<keytype,valuetype> *>
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
bool dictionary<keytype,valuetype>::getValue(keytype key,
						valuetype *value) {
	dictionarypair<keytype,valuetype>	*dnode=getNode(key);
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
	dictionarypair<keytype,valuetype>	*dnode=getNode(key);
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
dictionarypair<keytype,valuetype>
		*dictionary<keytype,valuetype>::getNode(keytype key) {
	treenode<dictionarypair<keytype,valuetype> *> *tnode=find(key);
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
		for (listnode<dictionarypair<keytype,valuetype> *>
			*node=list.getFirst(); node; node=node->getNext()) {
			keylist->append(node->getValue()->getKey());
		}
	} else {
		for (treenode<dictionarypair<keytype,valuetype> *>
			*node=tree.getFirst(); node; node=node->getNext()) {
			keylist->append(node->getValue()->getKey());
		}
	}
	return keylist;
}

template <class keytype, class valuetype>
inline
uint64_t dictionary<keytype,valuetype>::getCount() {
	return tree.getCount();
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::remove(keytype key) {
	treenode<dictionarypair<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		node_delete_value(&(tnode->getValue()->getKeyRef()),
						this->getManageKeys(),
						this->getManageArrayKeys());
		node_delete_value(&(tnode->getValue()->getValueRef()),
						this->getManageValues(),
						this->getManageArrayValues());
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

template <class keytype, class valuetype>
inline
bool dictionary<keytype,valuetype>::clear() {
	for (treenode<dictionarypair<keytype,valuetype> *> *node=
				tree.getFirst(); node; node=node->getNext()) {
		node_delete_value(&(node->getValue()->getKeyRef()),
					this->getManageKeys(),
					this->getManageArrayKeys());
		node_delete_value(&(node->getValue()->getValueRef()),
					this->getManageValues(),
					this->getManageArrayValues());
		delete node->getValue();
	}
	tree.clear();
	list.clear();
	delete keylist;
	keylist=NULL;
	return true;
}

template <class keytype, class valuetype>
inline
treenode<dictionarypair<keytype,valuetype> *>
		*dictionary<keytype,valuetype>::find(keytype key) {
	dictionarypair<keytype,valuetype>	fnode(key,(valuetype)0);
	return tree.find(&fnode);
}

template <class keytype, class valuetype>
inline
dictionarypair<keytype,valuetype>::dictionarypair(keytype key,
							valuetype value) :
	object(),
	key(key),
	value(value) {
}

template <class keytype, class valuetype>
inline
void dictionarypair<keytype,valuetype>::setKey(keytype key) {
	this->key=key;
}

template <class keytype, class valuetype>
inline
void dictionarypair<keytype,valuetype>::setValue(valuetype value) {
	this->value=value;
}

template <class keytype, class valuetype>
inline
keytype dictionarypair<keytype,valuetype>::getKey() {
	return key;
}

template <class keytype, class valuetype>
inline
keytype &dictionarypair<keytype,valuetype>::getKeyRef() {
	return key;
}

template <class keytype, class valuetype>
inline
valuetype dictionarypair<keytype,valuetype>::getValue() {
	return value;
}

template <class keytype, class valuetype>
inline
valuetype &dictionarypair<keytype,valuetype>::getValueRef() {
	return value;
}

template <class keytype, class valuetype>
inline
int32_t dictionarypaircomparator<keytype,valuetype>::compare(
							object *value1,
							object *value2) {
	dictionarypair<keytype,valuetype> *v1=
		(dictionarypair<keytype,valuetype> *)value1;
	dictionarypair<keytype,valuetype> *v2=
		(dictionarypair<keytype,valuetype> *)value2;
	return comparator::compare(v1->getKey(),v2->getKey());
}
