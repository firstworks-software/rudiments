// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/jsondom.h>
#include <rudiments/charstring.h>
#include <rudiments/xmldom.h>

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
	if (getRootNode()->isNullNode()) {
		createRootNode();
	}
	pvt->_currentparent=pvt->_currentparent->appendTag("o");
	return true;
}

bool jsondom::memberStart() {
	pvt->_currentparent=pvt->_currentparent->appendTag("m");
	return true;
}

bool jsondom::memberName(const char *name) {
	pvt->_currentparent->setAttributeValue("n",name);
	return true;
}

bool jsondom::stringValue(const char *value) {
	domnode	*valuetag=pvt->_currentparent->appendTag("s");
	valuetag->setAttributeValue("v",value);
	return true;
}

bool jsondom::numberValue(const char *value) {
	domnode	*valuetag=pvt->_currentparent->appendTag("n");
	valuetag->setAttributeValue("v",value);
	return true;
}

bool jsondom::trueValue() {
	pvt->_currentparent->appendTag("t");
	return true;
}

bool jsondom::falseValue() {
	pvt->_currentparent->appendTag("f");
	return true;
}

bool jsondom::nullValue() {
	pvt->_currentparent->appendTag("u");
	return true;
}

bool jsondom::arrayStart() {
	pvt->_currentparent=pvt->_currentparent->appendTag("a");
	return true;
}

bool jsondom::arrayEnd() {
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool jsondom::memberEnd() {
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool jsondom::objectEnd() {
	pvt->_currentparent=pvt->_currentparent->getParent();
	return true;
}

bool jsondom::write(output *out) const {
	return dom::write(out,true);
}

void jsondom::write(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	if (dn->getType()==ROOT_DOMNODETYPE) {
		write(dn->getFirstTagChild(),out,indent,indentlevel);
		return;
	} else if (dn->getType()!=TAG_DOMNODETYPE) {
		return;
	}

	const char	*name=dn->getName();
	switch (*name) {
		case 'o':
			{
			if (indent) {
				if (*indentlevel) {
					out->write('\n');
				}
				writeIndent(out,*indentlevel);
			}
			out->write('{');
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel+2;
			}
			bool	first=true;
			for (domnode *child=dn->getFirstTagChild();
					!child->isNullNode();
					child=child->getNextTagSibling()) {
				if (first) {
					first=false;
				} else {
					out->write(',');
					if (indent) {
						out->write('\n');
					}
				}
				write(child,out,indent,indentlevel);
			}
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel-2;
				writeIndent(out,*indentlevel);
			}
			out->write('}');
			}
			break;
		case 'm':
			if (indent) {
				writeIndent(out,*indentlevel);
			}
			out->write('"');
			out->write(dn->getAttributeValue("n"));
			out->write('"');
			if (indent) {
				out->write(' ');
			}
			out->write(':');
			if (indent) {
				out->write(' ');
			}
			write(dn->getFirstTagChild(),out,indent,indentlevel);
			break;
		case 's':
			out->write('"');
			out->write(dn->getAttributeValue("v"));
			out->write('"');
			break;
		case 'n':
			out->write(dn->getAttributeValue("v"));
			break;
		case 't':
			out->write("true");
			break;
		case 'f':
			out->write("false");
			break;
		case 'u':
			out->write("null");
			break;
		case 'a':
			if (indent) {
				out->write('\n');
				writeIndent(out,*indentlevel);
			}
			out->write('[');
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel+2;
			}
			bool	first=true;
			for (domnode *child=dn->getFirstTagChild();
					!child->isNullNode();
					child=child->getNextTagSibling()) {
				if (first) {
					first=false;
				} else {
					out->write(',');
					if (indent) {
						out->write('\n');
					}
				}
				if (indent) {
					writeIndent(out,*indentlevel);
				}
				write(child,out,indent,indentlevel);
			}
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel-2;
				writeIndent(out,*indentlevel);
			}
			out->write(']');
			break;
	}
}
