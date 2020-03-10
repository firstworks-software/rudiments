// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvcproperties.h>

class mvcpropertiesprivate {
	friend class mvcproperties;
	private:
		dictionary<const char *,const char *>	dict;
};

mvcproperties::mvcproperties() {
	pvt=new mvcpropertiesprivate;
}

mvcproperties::~mvcproperties() {
	delete pvt;
}

bool mvcproperties::parseFile(const char *filename) {
	// FIXME: implement this...
	return true;
}

bool mvcproperties::parseString(const char *string) {
	// FIXME: implement this...
	return true;
}

const char *mvcproperties::getValue(const char *parameter) {
	return pvt->dict.getValue(parameter);
}
