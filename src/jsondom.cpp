// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/jsondom.h>
#include <rudiments/charstring.h>

class jsondomprivate {
	friend class jsondom;
	private:
		domnode		*_currentparent;
};

jsondom::jsondom() : jsonsax(), dom() {
	init(true);
}

jsondom::jsondom(bool stringcacheenabled) : jsonsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

jsondom::jsondom(const jsondom &x) : jsonsax(), dom(x) {
	init(stringCacheEnabled());
}

jsondom &jsondom::operator=(const jsondom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void jsondom::init(bool stringcacheenabled) {
	pvt=new jsondomprivate;
	pvt->_currentparent=NULL;
}

jsondom::~jsondom() {
	delete pvt;
}

bool jsondom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool jsondom::parseFile(const char *filename,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
	} else {
		reset();
	}
	return jsonsax::parseFile(filename);
}

bool jsondom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool jsondom::parseString(const char *string,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
	} else {
		reset();
	}
	return jsonsax::parseString(string);
}

void jsondom::reset() {
	pvt->_currentparent=NULL;
	dom::reset();
	jsonsax::reset();
}

void jsondom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

bool jsondom::objectStart() {
	// FIXME: implement this
	return false;
}

bool jsondom::memberStart() {
	// FIXME: implement this
	return false;
}

bool jsondom::memberName(const char *name) {
	// FIXME: implement this
	return false;
}

bool jsondom::valueStart() {
	// FIXME: implement this
	return false;
}

bool jsondom::stringValue(const char *value) {
	// FIXME: implement this
	return false;
}

bool jsondom::numberValue(const char *value) {
	// FIXME: implement this
	return false;
}

bool jsondom::trueValue() {
	// FIXME: implement this
	return false;
}

bool jsondom::falseValue() {
	// FIXME: implement this
	return false;
}

bool jsondom::nullValue() {
	// FIXME: implement this
	return false;
}

bool jsondom::arrayStart() {
	// FIXME: implement this
	return false;
}

bool jsondom::arrayEnd() {
	// FIXME: implement this
	return false;
}

bool jsondom::valueEnd() {
	// FIXME: implement this
	return false;
}

bool jsondom::memberEnd() {
	// FIXME: implement this
	return false;
}

bool jsondom::objectEnd() {
	// FIXME: implement this
	return false;
}

void jsondom::insertChild(domnode *child) {
}

void jsondom::write(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {
}
