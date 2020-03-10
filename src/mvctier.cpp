// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvctier.h>
#include <rudiments/private/inttypes.h>

class mvctierprivate {
	friend class mvctier;
	private:
		mvcproperties	*prop;
		mvcsecurity	*sec;
};

mvctier::mvctier() {
	pvt=new mvctierprivate;
	pvt->prop=NULL;
	pvt->sec=NULL;
}

mvctier::~mvctier() {
	delete pvt;
}

void mvctier::setProperties(mvcproperties *prop) {
	pvt->prop=prop;
}

mvcproperties *mvctier::getProperties() {
	return pvt->prop;
}

void mvctier::setSecurity(mvcsecurity *sec) {
	pvt->sec=sec;
}

mvcsecurity *mvctier::getSecurity() {
	return pvt->sec;
}
