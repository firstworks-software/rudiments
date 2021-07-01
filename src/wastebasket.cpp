// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wastebasket.h>
#include <rudiments/linkedlist.h>

class wastebasketprivate {
	friend class wastebasket;
	private:
		linkedlist<object *>		*_objects;
		linkedlist<collection *>	*_deletecollections;
		linkedlist<collection *>	*_arraydeletecollections;
};

wastebasket::wastebasket() : object() {
	pvt=new wastebasketprivate;
	pvt->_objects=NULL;
	pvt->_deletecollections=NULL;
	pvt->_arraydeletecollections=NULL;
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

void wastebasket::attachDeleteCollection(collection *c) {
	if (!pvt->_deletecollections) {
		pvt->_deletecollections=new linkedlist<collection *>();
	}
	pvt->_deletecollections->append(c);
}

void wastebasket::empty() {
	if (pvt->_objects) {
		pvt->_objects->clearAndDelete();
		delete pvt->_objects;
		pvt->_objects=NULL;
	}
	if (pvt->_deletecollections) {
		for (listnode<collection *> *node=
				pvt->_deletecollections->getFirst();
				node; node=node->getNext()) {
			node->getValue()->clearAndDelete();
		}
		pvt->_deletecollections->clearAndDelete();
		delete pvt->_deletecollections;
		pvt->_deletecollections=NULL;
	}
}
