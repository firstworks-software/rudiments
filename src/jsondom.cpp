// Copyright (c) 2018 David Muse
// See the COPYING file for more information

#include <rudiments/jsondom.h>
#include <rudiments/charstring.h>

class jsondomprivate {
	friend class jsondom;
	private:
		domnode			*_current;
		linkedlist<bool>	_inarray;
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
	pvt->_current=NULL;
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
		pvt->_current=parent;
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
		pvt->_current=parent;
	} else {
		reset();
	}
#if 0
	bool	retval=jsonsax::parseString(string);
	uint16_t	indentlevel=0;
	dom::write(getRootNode(),&stdoutput,true,&indentlevel);
	return retval;
#endif
	return jsonsax::parseString(string);
}

void jsondom::reset() {
	pvt->_current=NULL;
	dom::reset();
	jsonsax::reset();
}

void jsondom::createRootNode() {
	dom::createRootNode();
	pvt->_current=getRootNode();
}

bool jsondom::objectStart() {
	if (getRootNode()->isNullNode()) {
		createRootNode();
	} else {
		pvt->_current=whichNode();
		pvt->_current->setAttributeValue("t","o");
	}
	return true;
}

bool jsondom::memberStart() {
	return true;
}

bool jsondom::memberName(const char *name) {
	pvt->_current=pvt->_current->appendTag(name);
	return true;
}

bool jsondom::stringValue(const char *value) {
	domnode	*node=whichNode();
	node->setAttributeValue("t","s");
	node->setAttributeValue("v",value);
	return true;
}

bool jsondom::numberValue(const char *value) {
	domnode	*node=whichNode();
	node->setAttributeValue("t","n");
	node->setAttributeValue("v",value);
	return true;
}

bool jsondom::trueValue() {
	whichNode()->setAttributeValue("t","t");
	return true;
}

bool jsondom::falseValue() {
	whichNode()->setAttributeValue("t","f");
	return true;
}

bool jsondom::nullValue() {
	whichNode()->setAttributeValue("t","u");
	return true;
}

bool jsondom::arrayStart() {
	whichNode()->setAttributeValue("t","a");
	return true;
}

bool jsondom::arrayEnd() {
	return true;
}

bool jsondom::memberEnd() {
	pvt->_current=pvt->_current->getParent();
	return true;
}

bool jsondom::objectEnd() {
	if (pvt->_current!=getRootNode()) {
		pvt->_current=pvt->_current->getParent();
	}
	return true;
}

bool jsondom::write(output *out) const {
	return dom::write(out,true);
}

domnode *jsondom::whichNode() {
	// if we're in an array then append a "v" tag and use it,
	// otherwise just use the current tag
	const char	*t=pvt->_current->getAttributeValue("t");
	return (t && t[0]=='a')?
			pvt->_current->appendTag("v"):
			pvt->_current;
}

void jsondom::write(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	if (dn->getType()!=TAG_DOMNODETYPE && dn->getType()!=ROOT_DOMNODETYPE) {
		return;
	}

	if (pvt->_inarray.getLength() && !pvt->_inarray.getLast()->getValue()) {
		if (indent) {
			writeIndent(out,*indentlevel);
		}
		out->write('"');
		out->write(dn->getName());
		out->write('"');
		if (indent) {
			out->write(' ');
		}
		out->write(':');
		if (indent) {
			out->write(' ');
		}
	}

	const char	*type=getType(dn);
	switch (*type) {
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
			pvt->_inarray.append(false);
			bool	first=true;
			// NOTE: this loop uses getFirstChild/getNextSibling
			// and ignores non-tags rather than just calling
			// getFirstTagChild/getNextTagSibling to work correctly
			// with cursordomnodes
			for (domnode *child=dn->getFirstChild();
					!child->isNullNode();
					child=child->getNextSibling()) {
				if (child->getType()!=TAG_DOMNODETYPE) {
					continue;
				}
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
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel-2;
				writeIndent(out,*indentlevel);
			}
			out->write('}');
			}
			break;
		case 's':
			{
			out->write('"');
			const char *val=getValue(dn);
			for (;;) {
				if (*val=='\0') {
					break;
				} else if (*val=='\b') {
					out->write("\\b");
				} else if (*val=='\f') {
					out->write("\\f");
				} else if (*val=='\n') {
					out->write("\\n");
				} else if (*val=='\r') {
					out->write("\\r");
				} else if (*val=='\t') {
					out->write("\\t");
				} else {
					out->write(*val);
				}
				val++;
			}
			out->write('"');
			}
			break;
		case 'n':
			{
			const char	*v=getValue(dn);
			out->write((!charstring::isNullOrEmpty(v))?v:"0");
			}
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
			pvt->_inarray.append(true);
			bool	first=true;
			// NOTE: this loop uses getFirstChild/getNextSibling
			// and ignores non-tags rather than just calling
			// getFirstTagChild/getNextTagSibling to work correctly
			// with cursordomnodes
			for (domnode *child=dn->getFirstChild();
					!child->isNullNode();
					child=child->getNextSibling()) {
				if (child->getType()!=TAG_DOMNODETYPE) {
					continue;
				}
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
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				out->write('\n');
				*indentlevel=*indentlevel-2;
				writeIndent(out,*indentlevel);
			}
			out->write(']');
			break;
	}
}

const char *jsondom::getType(const domnode *dn) const {

	// if this is the root node then it's an object
	if (dn->getType()==ROOT_DOMNODETYPE) {
		return "o";
	}

	// first try attribute "t"
	const char	*type=dn->getAttributeValue("t");

	// then try attribute "type"
	if (charstring::isNullOrEmpty(type)) {
		type=dn->getAttributeValue("type");
	}

	// we use 'u' to represent a null because 'n' represents a number,
	// so if we find type="null" then bump the return value to the 'u'
	if (!charstring::compare(type,"null")) {
		type++;
	}

	// if the type isn't provided, then try to derive it from the value
	if (charstring::isNullOrEmpty(type)) {
		const char	*v=getValue(dn);
		if (charstring::isNullOrEmpty(v)) {
			// FIXME: actually, it could also be an array,
			// see if there are multiple children with the
			// same name...
			// of course, it could be an array with 1 member or
			// an empty array...
			type="o";
		} else if (!charstring::compare(v,"true")) {
			type="t";
		} else if (!charstring::compare(v,"false")) {
			type="f";
		} else if (!charstring::compare(v,"null")) {
			type="u";
		} else if (charstring::isNumber(v)) {
			type="n";
		} else {
			type="s";
		}
	}

	return type;
}

const char *jsondom::getValue(const domnode *dn) const {

	// first try attribute "v"
	const char	*value=dn->getAttributeValue("v");

	// then try attribute "value"
	if (charstring::isNullOrEmpty(value)) {
		value=dn->getAttributeValue("value");
	}

	// FIXME: then fall back to the immediate text node
	return value;
}
