// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wastebasket.h>
#include <rudiments/linkedlist.h>

class wastebasketprivate {
	friend class wastebasket;
	private:
		linkedlist<object *>		*_objects;
		linkedlist<collection *>	*_cwithvalues;
		linkedlist<collection *>	*_cwitharrayvalues;
		linkedlist<collection *>	*_cwithkeys;
		linkedlist<collection *>	*_cwitharraykeys;
		linkedlist<collection *>	*_cwitharraykeysandvalues;
		linkedlist<collection *>	*_cwithkeysandarrayvalues;
};

wastebasket::wastebasket() : object() {
	pvt=new wastebasketprivate;
	pvt->_objects=NULL;
	pvt->_cwithvalues=NULL;
	pvt->_cwitharrayvalues=NULL;
	pvt->_cwithkeys=NULL;
	pvt->_cwitharraykeys=NULL;
	pvt->_cwitharraykeysandvalues=NULL;
	pvt->_cwithkeysandarrayvalues=NULL;
}

wastebasket::~wastebasket() {
	empty();
	delete pvt;
}

void wastebasket::attachObject(object *o) {
	if (!pvt->_objects) {
		pvt->_objects=new linkedlist<object *>();
	}
	pvt->_objects->append(o);
}

void wastebasket::attachCollectionWithValues(collection *c) {
	if (!pvt->_cwithvalues) {
		pvt->_cwithvalues=new linkedlist<collection *>();
	}
	pvt->_cwithvalues->append(c);
}

void wastebasket::attachCollectionWithArrayValues(collection *c) {
	if (!pvt->_cwitharrayvalues) {
		pvt->_cwitharrayvalues=new linkedlist<collection *>();
	}
	pvt->_cwitharrayvalues->append(c);
}

void wastebasket::attachCollectionWithKeys(collection *c) {
	if (!pvt->_cwithkeys) {
		pvt->_cwithkeys=new linkedlist<collection *>();
	}
	pvt->_cwithkeys->append(c);
}

void wastebasket::attachCollectionWithArrayKeys(collection *c) {
	if (!pvt->_cwitharraykeys) {
		pvt->_cwitharraykeys=new linkedlist<collection *>();
	}
	pvt->_cwitharraykeys->append(c);
}

void wastebasket::attachCollectionWithArrayKeysAndValues(collection *c) {
	if (!pvt->_cwitharraykeysandvalues) {
		pvt->_cwitharraykeysandvalues=new linkedlist<collection *>();
	}
	pvt->_cwitharraykeysandvalues->append(c);
}

void wastebasket::attachCollectionWithKeysAndArrayValues(collection *c) {
	if (!pvt->_cwithkeysandarrayvalues) {
		pvt->_cwithkeysandarrayvalues=new linkedlist<collection *>();
	}
	pvt->_cwithkeysandarrayvalues->append(c);
}

void wastebasket::empty() {
	if (pvt->_objects) {
		pvt->_objects->clearAndDelete();
		delete pvt->_objects;
		pvt->_objects=NULL;
	}
	if (pvt->_cwithvalues) {
		for (listnode<collection *> *node=
				pvt->_cwithvalues->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwithvalues->clearAndDelete();
		delete pvt->_cwithvalues;
		pvt->_cwithvalues=NULL;
	}
	if (pvt->_cwitharrayvalues) {
		for (listnode<collection *> *node=
				pvt->_cwitharrayvalues->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwitharrayvalues->clearAndArrayDelete();
		delete pvt->_cwitharrayvalues;
		pvt->_cwitharrayvalues=NULL;
	}
	if (pvt->_cwithkeys) {
		for (listnode<collection *> *node=
				pvt->_cwithkeys->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwithkeys->clearAndDelete();
		delete pvt->_cwithkeys;
		pvt->_cwithkeys=NULL;
	}
	if (pvt->_cwitharraykeys) {
		for (listnode<collection *> *node=
				pvt->_cwitharraykeys->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwitharraykeys->clearAndArrayDelete();
		delete pvt->_cwitharraykeys;
		pvt->_cwitharrayvalues=NULL;
	}
	if (pvt->_cwitharraykeysandvalues) {
		for (listnode<collection *> *node=
				pvt->_cwitharraykeysandvalues->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwitharraykeysandvalues->clearAndDelete();
		delete pvt->_cwitharraykeysandvalues;
		pvt->_cwitharraykeysandvalues=NULL;
	}
	if (pvt->_cwithkeysandarrayvalues) {
		for (listnode<collection *> *node=
				pvt->_cwithkeysandarrayvalues->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_cwithkeysandarrayvalues->clearAndDelete();
		delete pvt->_cwithkeysandarrayvalues;
		pvt->_cwithkeysandarrayvalues=NULL;
	}
}
