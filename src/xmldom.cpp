// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/xmldom.h>
#include <rudiments/charstring.h>
#include <rudiments/file.h>
#include <rudiments/filesystem.h>
#include <rudiments/sys.h>

class xmldomprivate {
	friend class xmldom;
	private:
		domnode		*_currentparent;
		domnode		*_currentattribute;

		domnode		*_top;
		uint64_t	_topposition;
};

xmldom::xmldom() : xmlsax(), dom() {
	init(true);
}

xmldom::xmldom(bool stringcacheenabled) : xmlsax(), dom(stringcacheenabled) {
	init(stringcacheenabled);
}

xmldom::xmldom(const xmldom &x) : xmlsax(), dom() {
	init(stringCacheEnabled());
}

xmldom &xmldom::operator=(const xmldom &x) {
	if (this!=&x) {
		reset();
	}
	return *this;
}

void xmldom::init(bool stringcacheenabled) {
	pvt=new xmldomprivate;
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
}

xmldom::~xmldom() {
	delete pvt;
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
	return xmlsax::parseFile(filename);
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
	return xmlsax::parseString(string);
}

void xmldom::reset() {
	pvt->_currentparent=NULL;
	pvt->_currentattribute=NULL;
	pvt->_top=NULL;
	pvt->_topposition=0;
	dom::reset();
	xmlsax::reset();
}

void xmldom::createRootNode() {
	dom::createRootNode();
	pvt->_currentparent=getRootNode();
}

bool xmldom::writeFile(const char *filename, mode_t perms) const {
	filesystem	fs;
	off64_t	optblocksize;
	if (fs.open(filename)) {
		optblocksize=fs.getOptimumTransferBlockSize();
	} else {
		optblocksize=sys::getPageSize();
	}
	file	fl;
	if (!fl.open(filename,O_WRONLY|O_CREAT|O_TRUNC,perms)) {
		return false;
	}
	fl.setWriteBufferSize(optblocksize);
	stringbuffer	*xml=getRootNode()->xml();
	xml->append('\n');
	bool	retval=true;
	ssize_t	length=charstring::length(xml->getString());
	if (fl.write(xml->getString(),length)!=length) {
		retval=false;
	}
	fl.flushWriteBuffer(-1,-1);
	if (!fl.close()) {
		retval=false;
	}
	delete xml;
	return retval;
}

bool xmldom::tagStart(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	domnode	*tagnode=new domnode(this,getNullNode(),
						TAG_DOMNODETYPE,
						ns,name,NULL);
	insertChild(tagnode);
	pvt->_currentparent=tagnode;
	return true;
}

bool xmldom::attributeName(const char *name) {
	pvt->_currentattribute=new domnode(this,getNullNode(),
						ATTRIBUTE_DOMNODETYPE,
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
	insertChild(new domnode(this,getNullNode(),
					TEXT_DOMNODETYPE,
					NULL,"text",string));
	return true;
}

bool xmldom::tagEnd(const char *ns, const char *name) {
	pvt->_currentattribute=NULL;
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool xmldom::comment(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,getNullNode(),
					COMMENT_DOMNODETYPE,
					NULL,"comment",string));
	return true;
}

bool xmldom::cdata(const char *string) {
	pvt->_currentattribute=NULL;
	insertChild(new domnode(this,getNullNode(),
					CDATA_DOMNODETYPE,
					NULL,"cdata",string));
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
