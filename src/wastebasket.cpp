// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wastebasket.h>
#include <rudiments/linkedlist.h>

class wastebasketprivate {
	friend class wastebasket;
	private:
		linkedlist<object *>		*_objects;
};

wastebasket::wastebasket() : object() {
	pvt=new wastebasketprivate;
	pvt->_objects=NULL;
}

wastebasket::~wastebasket() {
	empty();
	delete pvt;
}

void wastebasket::attachObject(object *o) {
	if (!pvt->_objects) {
		pvt->_objects=new linkedlist<object *>();
		pvt->_objects->setManageValues(true);
	}
	pvt->_objects->append(o);
}

void wastebasket::empty() {
	if (pvt->_objects) {
		delete pvt->_objects;
		pvt->_objects=NULL;
	}
}
