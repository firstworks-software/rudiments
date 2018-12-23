// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/stdio.h>
#include <rudiments/private/nodeinlines.h>

#define DICTIONARY_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARY_CLASS \
	dictionary<keytype,valuetype>

DICTIONARY_TEMPLATE
inline
DICTIONARY_CLASS::dictionary() {
	trackinsertionorder=true;
}

DICTIONARY_TEMPLATE
inline
DICTIONARY_CLASS::~dictionary() {
	clear();
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::setTrackInsertionOrder(bool trackinsertionorder) {
	if (!tree.getLength()) {
		this->trackinsertionorder=trackinsertionorder;
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::getTrackInsertionOrder() {
	return trackinsertionorder;
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValue(keytype key, valuetype value) {
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

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValues(keytype *keys, valuetype *values) {
	keytype		*key=keys;
	valuetype	*value=values;
	while (*key) {
		setValue(*key,*value);
		key++;
		value++;
	}
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValues(keytype const *keys, valuetype const *values) {
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

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValues(keytype *keys, valuetype *values,
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

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValues(keytype const *keys, valuetype const *values,
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

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::setValues(dictionary<keytype,valuetype> *dict) {
	if (dict) {
		for (linkedlistnode< dictionarynode< keytype, valuetype > *>
					*node=dict->getList()->getFirst();
					node; node=node->getNext()) {
			setValue(node->getValue()->getKey(),
					node->getValue()->getValue());
		}
	}
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::getValue(keytype key, valuetype *value) {
	dictionarynode<keytype,valuetype>	*dnode=getNode(key);
	if (dnode) {
		*value=dnode->getValue();
		return true;
	}
	return false;
}

DICTIONARY_TEMPLATE
inline
valuetype DICTIONARY_CLASS::getValue(keytype key) {
	valuetype	value;
	if (getValue(key,&value)) {
		return value;
	}
	return (valuetype)0;
}

DICTIONARY_TEMPLATE
inline
dictionarynode<keytype,valuetype> *DICTIONARY_CLASS::getNode(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		return tnode->getValue();
	}
	return NULL;
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::remove(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDelete(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDelete(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteKey(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteKey(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteValue(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteValue(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteKeyAndArrayDeleteValue(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteKeyAndDeleteValue(keytype key) {
	avltreenode< dictionarynode<keytype,valuetype> *> *tnode=find(key);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::remove(dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
	if (tnode) {
		if (trackinsertionorder) {
			list.remove(tnode->getValue());
		}
		delete tnode->getValue();
		return tree.remove(tnode);
	}
	return false;
}

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDelete(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDelete(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteKey(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteKey(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndDeleteKeyAndArrayDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
bool DICTIONARY_CLASS::removeAndArrayDeleteKeyAndDeleteValue(
				dictionarynode<keytype,valuetype> *node) {
	avltreenode< dictionarynode<keytype,valuetype> *>
					*tnode=tree.find(node);
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

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clear() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndDelete() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndArrayDelete() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndDeleteKeys() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndArrayDeleteKeys() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndDeleteValues() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndArrayDeleteValues() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndDeleteKeysAndArrayDeleteValues() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete node->getValue()->getKey();
		delete[] node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::clearAndArrayDeleteKeysAndDeleteValues() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		delete[] node->getValue()->getKey();
		delete node->getValue()->getValue();
		delete node->getValue();
	}
	tree.clear();
	list.clear();
}

DICTIONARY_TEMPLATE
inline
linkedlist<keytype> *DICTIONARY_CLASS::getKeys() {
	linkedlist<keytype>	*keys=new linkedlist<keytype>();
	for (linkedlistnode< dictionarynode< keytype, valuetype > *>
		*node=getList()->getFirst(); node; node=node->getNext()) {
		keys->append(node->getValue()->getKey());
	}
	return keys;
}

DICTIONARY_TEMPLATE
inline
avltree< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::getTree() {
	return &tree;
}

DICTIONARY_TEMPLATE
inline
linkedlist< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::getList() {
	if (!trackinsertionorder) {
		list.clear();
		for (avltreenode< dictionarynode< keytype, valuetype > *>
			*node=tree.getFirst(); node; node=node->getNext()) {
			list.append(node->getValue());
		}
	}
	return &list;
}

DICTIONARY_TEMPLATE
inline
void DICTIONARY_CLASS::print() {
	for (linkedlistnode< dictionarynode< keytype, valuetype > *> *node=
				list.getFirst(); node; node=node->getNext()) {
		node->getValue()->print();
		stdoutput.printf("\n");
	}
}

DICTIONARY_TEMPLATE
inline
avltreenode< dictionarynode<keytype,valuetype> *> *DICTIONARY_CLASS::
							find(keytype key) {
	dictionarynode<keytype,valuetype>	fnode(key,(valuetype)0);
	return tree.find(&fnode);
}

#define DICTIONARYNODE_TEMPLATE \
	template <class keytype, class valuetype>

#define DICTIONARYNODE_CLASS \
	dictionarynode<keytype,valuetype>

DICTIONARYNODE_TEMPLATE
inline
DICTIONARYNODE_CLASS::dictionarynode(keytype key, valuetype value) {
	this->key=key;
	this->value=value;
}

DICTIONARYNODE_TEMPLATE
inline
DICTIONARYNODE_CLASS::~dictionarynode() {}

DICTIONARYNODE_TEMPLATE
inline
void DICTIONARYNODE_CLASS::setKey(keytype key) {
	this->key=key;
}

DICTIONARYNODE_TEMPLATE
inline
void DICTIONARYNODE_CLASS::setValue(valuetype value) {
	this->value=value;
}

DICTIONARYNODE_TEMPLATE
inline
keytype DICTIONARYNODE_CLASS::getKey() const {
	return key;
}

DICTIONARYNODE_TEMPLATE
inline
valuetype DICTIONARYNODE_CLASS::getValue() const {
	return value;
}

DICTIONARYNODE_TEMPLATE
inline
int32_t DICTIONARYNODE_CLASS::compare(keytype testkey) const {
	return node_compare(key,testkey);
}

DICTIONARYNODE_TEMPLATE
inline
int32_t DICTIONARYNODE_CLASS::compare(
		dictionarynode<keytype,valuetype> *testnode) const {
	return node_compare(key,testnode->key);
}

DICTIONARYNODE_TEMPLATE
inline
void DICTIONARYNODE_CLASS::print() const {
	node_print(key);
	stdoutput.printf(":");
	node_print(value);
}



DICTIONARYNODE_TEMPLATE
inline
int32_t node_compare(
			dictionarynode<keytype,valuetype> *value1,
			dictionarynode<keytype,valuetype> *value2) {
	return node_compare(value1->getKey(),value2->getKey());
}

DICTIONARYNODE_TEMPLATE
inline
void node_compare(dictionarynode<keytype,valuetype> *value) {
	node_print(value);
}
