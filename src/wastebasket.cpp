// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information.

#include <rudiments/wastebasket.h>
#include <rudiments/linkedlist.h>

class wastebasketprivate {
	friend class wastebasket;
	private:
		linkedlist<object *>		_objects;
		linkedlist<object **>		_objectarrays;
};

wastebasket::wastebasket() : object() {
	pvt->_objects.setManageValues(true);
	pvt->_objectarrays.setManageArrayValues(true);
}

wastebasket::~wastebasket() {
	delete pvt;
}

void wastebasket::attachObject(object *o) {
	pvt->_objects.append(o);
}

void wastebasket::attachObjectArray(object **o) {
	pvt->_objectarrays.append(o);
}

void wastebasket::empty() {
	pvt->_objects.clear();
	pvt->_objectarrays.clear();
}
