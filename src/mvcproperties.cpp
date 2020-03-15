// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more informations.

#include <rudiments/mvcproperties.h>
#include <rudiments/propdom.h>
#include <rudiments/dictionary.h>

class mvcpropertiesprivate {
	friend class mvcproperties;
	private:
		propdom	_p;
		dictionary<const char *,const char *>	_dict;
};

mvcproperties::mvcproperties() {
	pvt=new mvcpropertiesprivate;
}

mvcproperties::~mvcproperties() {
	delete pvt;
}

bool mvcproperties::parseFile(const char *filename) {
	pvt->_dict.clear();
	if (!pvt->_p.parseFile(filename)) {
		return false;
	}
	buildDictionary();
	return true;
}

bool mvcproperties::parseString(const char *string) {
	pvt->_dict.clear();
	if (!pvt->_p.parseString(string)) {
		return false;
	}
	buildDictionary();
	return true;
}

void mvcproperties::buildDictionary() {
	for (domnode *node=pvt->_p.getRootNode()->getFirstTagChild("k");
				!node->isNullNode();
				node=node->getNextTagSibling("k")) {
		pvt->_dict.setValue(node->getAttributeValue("k"),
					node->getAttributeValue("v"));
	}
}

const char *mvcproperties::getValue(const char *parameter) {
	return pvt->_dict.getValue(parameter);
}
