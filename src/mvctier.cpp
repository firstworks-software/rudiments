// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvc.h>
#include <rudiments/private/inttypes.h>

class mvctierprivate {
	friend class mvctier;
	private:
		mvcproperties	*_prop;
		mvcsecurity	*_sec;
};

mvctier::mvctier() : object() {
	pvt=new mvctierprivate;
	pvt->_prop=NULL;
	pvt->_sec=NULL;
}

mvctier::~mvctier() {
	delete pvt;
}

const char *mvctier::getType() const {
	return "mvctier";
}

void mvctier::setProperties(mvcproperties *prop) {
	pvt->_prop=prop;
}

mvcproperties *mvctier::getProperties() {
	return pvt->_prop;
}

void mvctier::setSecurity(mvcsecurity *sec) {
	pvt->_sec=sec;
}

mvcsecurity *mvctier::getSecurity() {
	return pvt->_sec;
}
