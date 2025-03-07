// Copyright (c) David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>

class xmldomprivate {
	friend class xmldom;
	private:
		domnode		*_currentparent;
		domnode		*_currentattribute;

		domnode		*_top;
		uint64_t	_topposition;
};

xmldom::xmldom() : xmlsax(), dom() {
	construct(true);
}

xmldom::xmldom(bool stringcacheenabled) : xmlsax(), dom(stringcacheenabled) {
	construct(stringcacheenabled);
}

xmldom::xmldom(xmldom &x) : xmlsax(), dom(x) {
	construct(getStringCacheEnabled());
}

xmldom &xmldom::operator=(xmldom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void xmldom::construct(bool stringcacheenabled) {
	pvt=new xmldomprivate;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
}

xmldom::~xmldom() {
	delete pvt;
}

const char *xmldom::getType() {
	return "xmldom";
}

bool xmldom::parse(input *in) {
	return parse(in,NULL,0);
}

bool xmldom::parse(input *in, domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return sax::parse(in);
}

bool xmldom::parseFile(const char *filename) {
	return parseFile(filename,NULL,0);
}

bool xmldom::parseFile(const char *filename,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return sax::parseFile(filename);
}

bool xmldom::parseString(const char *string) {
	return parseString(string,NULL,0);
}

bool xmldom::parseString(const char *string,
				domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_currentparent=parent;
		pvt->_currentattribute=NULL;
		pvt->_top=parent;
		pvt->_topposition=position;
	} else {
		reset();
	}
	return sax::parseString(string);
}

bool xmldom::reset() {
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
	return dom::reset() && xmlsax::reset();
}

void xmldom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

bool xmldom::tagStart(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*tagnode=new domnode(this,TAG_DOMNODETYPE,ns,name,NULL);
	insertChild(tagnode);
	pvt->_currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(const char *name) {
	pvt->_currentattribute=new domnode(this,ATTRIBUTE_DOMNODETYPE,
						NULL,name,NULL);
	pvt->_currentparent->insertAttribute(pvt->_currentattribute,
				pvt->_currentparent->getAttributeCount());
	return true;
}

bool xmldom::attributeValue(const char *value) {
	if (!pvt->_currentattribute) {
		if (!attributeName(value)) {
			return false;
		}
	}
	pvt->_currentattribute->setValue(value);
	pvt->_currentattribute=NULL;
	return true;
}

bool xmldom::text(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,TEXT_DOMNODETYPE,NULL,"text",string));
	return true;
}

bool xmldom::tagEnd(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool xmldom::comment(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,COMMENT_DOMNODETYPE,
					NULL,"comment",string));
	return true;
}

bool xmldom::cdata(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,CDATA_DOMNODETYPE,NULL,"cdata",string));
	return true;
}

void xmldom::insertChild(domnode *child) {
	uint64_t	pos=pvt->_currentparent->getChildCount();
	if (pvt->_currentparent==pvt->_top) {
		pos=pvt->_topposition;
		pvt->_topposition++;
	}
	pvt->_currentparent->insertChild(child,pos);
}
