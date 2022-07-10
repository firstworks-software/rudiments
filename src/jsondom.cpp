// Copyright (c) 1999-2018 David Muse
// See the COPYING file for more information

#include <rudiments/jsondom.h>
#include <rudiments/charstring.h>
//#define DEBUG_MESSAGES
#include <rudiments/debugprint.h>

class jsondomprivate {
	friend class jsondom;
	private:
		domnode			*_current;
		linkedlist<bool>	_inarray;

		#ifdef DEBUG_MESSAGES
		uint16_t	_indent;
		#endif
};

#ifdef DEBUG_MESSAGES
void indent(uint16_t ind) {
	if (ind>128) {
		stdoutput.printf("warning! ind=%d\n",ind);
		return;
	}
	for (uint16_t i=0; i<ind; i++) {
		stdoutput.write(' ');
	}
}
#endif

jsondom::jsondom() : jsonsax(), dom() {
	init(true);
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
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

const char *jsondom::getType() const {
	return "jsondom";
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
	bool	retval=jsonsax::parseFile(filename);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
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
	bool	retval=jsonsax::parseString(string);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
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
#ifdef DEBUG_MESSAGES
	if (pvt->_indent) {
		debugPrintf("\n");
	}
	indent(pvt->_indent);
	debugPrintf("objectStart {\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode()->appendTag("r");
	} else {
		pvt->_current=whichNode();
	}
	pvt->_current->setAttributeValue("t","o");
	return true;
}

bool jsondom::memberStart() {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("memberStart {\n");
	pvt->_indent+=2;
#endif
	return true;
}

bool jsondom::memberName(const char *name) {
#ifdef DEBUG_MESSAGES
	indent(pvt->_indent);
	debugPrintf("\"%s\" : ",name);
#endif
	pvt->_current=pvt->_current->appendTag(name);
	return true;
}

bool jsondom::valueStart() {
#ifdef DEBUG_MESSAGES
	const char	*t=pvt->_current->getAttributeValue("t");
	if (t && t[0]=='a') {
		indent(pvt->_indent);
	}
#endif
	return true;
}

bool jsondom::stringValue(const char *value) {
	debugPrintf("\"%s\"",value);
	domnode	*node=whichNode();
	node->setAttributeValue("t","s");
	node->setAttributeValue("v",value);
	return true;
}

bool jsondom::numberValue(const char *value) {
	debugPrintf("%s",value);
	domnode	*node=whichNode();
	node->setAttributeValue("t","n");
	node->setAttributeValue("v",value);
	return true;
}

bool jsondom::trueValue() {
	debugPrintf("true");
	whichNode()->setAttributeValue("t","t");
	return true;
}

bool jsondom::falseValue() {
	debugPrintf("false");
	whichNode()->setAttributeValue("t","f");
	return true;
}

bool jsondom::nullValue() {
	debugPrintf("null");
	whichNode()->setAttributeValue("t","u");
	return true;
}

bool jsondom::arrayStart() {
#ifdef DEBUG_MESSAGES
	debugPrintf("\n");
	indent(pvt->_indent);
	debugPrintf("[\n");
	pvt->_indent+=2;
#endif
	if (getRootNode()->isNullNode()) {
		createRootNode();
		pvt->_current=getRootNode()->appendTag("r");
	} else {
		pvt->_current=whichNode();
	}
	pvt->_current->setAttributeValue("t","a");
	return true;
}

bool jsondom::arrayEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("]");
#endif
	return true;
}

bool jsondom::valueEnd() {
#ifdef DEBUG_MESSAGES
	if (pvt->_current->getAttributeValue("t")[0]!='o') {
		debugPrintf("\n");
	}
#endif
	return true;
}

bool jsondom::memberEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("} %s->",pvt->_current->getName());
#endif
	pvt->_current=pvt->_current->getParent();
#ifdef DEBUG_MESSAGES
	stdoutput.printf("%s\n",pvt->_current->getName());
#endif
	return true;
}

bool jsondom::objectEnd() {
#ifdef DEBUG_MESSAGES
	pvt->_indent-=2;
	indent(pvt->_indent);
	debugPrintf("}\n");
#endif
	domnode		*parent=pvt->_current->getParent();
	const char	*t=parent->getAttributeValue("t");
	if (t && t[0]=='a') {
		pvt->_current=pvt->_current->getParent();
	}
	return true;
}

domnode *jsondom::whichNode() {
	// if we're in an array then append a "v" tag and use it,
	// otherwise just use the current tag
	const char	*t=pvt->_current->getAttributeValue("t");
	return (t && t[0]=='a')?
			pvt->_current->appendTag("v"):
			pvt->_current;
}

bool jsondom::writeNode(const domnode *dn, output *out,
			bool indent, uint16_t *indentlevel) const {

	if (dn->getType()!=TAG_DOMNODETYPE && dn->getType()!=ROOT_DOMNODETYPE) {
		return true;
	}

	bool	inarray=(pvt->_inarray.getLength() &&
				pvt->_inarray.getLast()->getValue());
	if (indentlevel && *indentlevel && !inarray) {
		if (indent) {
			if (!writeIndent(out,*indentlevel)) {
				return false;
			}
		}
		if (out->write('"')<1) {
			return false;
		}
		const char	*name=dn->getName();
		ssize_t		len=charstring::length(name);
		if (out->write(name,len)<len) {
			return false;
		}
		if (out->write('"')<1) {
			return false;
		}
		if (indent) {
			if (out->write(' ')<1) {
				return false;
			}
		}
		if (out->write(':')<1) {
			return false;
		}
		if (indent) {
			if (out->write(' ')<1) {
				return false;
			}
		}
	}

	const char	*type=getType(dn);
	switch (*type) {
		case 'o':
			{
			if (indent) {
				if (*indentlevel && !inarray) {
					if (out->write('\n')<1) {
						return false;
					}
				}
				if (!inarray) {
					if (!writeIndent(out,*indentlevel)) {
						return false;
					}
				}
			}
			out->write('{');
			if (indent) {
				if (out->write('\n')<1) {
					return false;
				}
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
					if (out->write(',')<1) {
						return false;
					}
					if (indent) {
						if (out->write('\n')<1) {
							return false;
						}
					}
				}
				if (!writeNode(child,out,indent,indentlevel)) {
					return false;
				}
			}
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				if (!first) {
					if (out->write('\n')<1) {
						return false;
					}
				}
				*indentlevel=*indentlevel-2;
				if (!writeIndent(out,*indentlevel)) {
					return false;
				}
			}
			if (out->write('}')<1) {
				return false;
			}
			}
			break;
		case 's':
			{
			if (out->write('"')<1) {
				return false;
			}
			const char *val=getValue(dn);
			if (val) {
				for (;;) {
					if (*val=='\0') {
						break;
					} else if (*val=='\b') {
						if (out->write("\\b")<1) {
							return false;
						}
					} else if (*val=='\f') {
						if (out->write("\\f")<1) {
							return false;
						}
					} else if (*val=='\n') {
						if (out->write("\\n")<1) {
							return false;
						}
					} else if (*val=='\r') {
						if (out->write("\\r")<1) {
							return false;
						}
					} else if (*val=='\t') {
						if (out->write("\\t")<1) {
							return false;
						}
					} else {
						if (out->write(*val)<1) {
							return false;
						}
					}
					val++;
				}
			}
			if (out->write('"')<1) {
				return false;
			}
			}
			break;
		case 'n':
			{
			const char	*v=getValue(dn);
			if (!charstring::isNullOrEmpty(v)) {
				ssize_t	len=charstring::length(v);
				if (out->write(v,len)<len) {
					return false;
				}
			} else {
				if (out->write('0')<1) {
					return false;
				}
			}
			}
			break;
		case 't':
			if (out->write("true")<4) {
				return false;
			}
			break;
		case 'f':
			if (out->write("false")<5) {
				return false;
			}
			break;
		case 'u':
			if (out->write("null")<4) {
				return false;
			}
			break;
		case 'a':
			{
			if (indent) {
				if (*indentlevel) {
					if (out->write('\n')<1) {
						return false;
					}
				}
				if (!writeIndent(out,*indentlevel)) {
					return false;
				}
			}
			if (out->write('[')<1) {
				return false;
			}
			if (indent) {
				if (out->write('\n')<1) {
					return false;
				}
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
					if (out->write(',')<1) {
						return false;
					}
					if (indent) {
						if (out->write('\n')<1) {
							return false;
						}
					}
				}
				if (indent) {
					if (!writeIndent(out,*indentlevel)) {
						return false;
					}
				}
				if (!writeNode(child,out,indent,indentlevel)) {
					return false;
				}
			}
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				if (!first) {
					if (out->write('\n')<1) {
						return false;
					}
				}
				*indentlevel=*indentlevel-2;
				if (!writeIndent(out,*indentlevel)) {
					return false;
				}
			}
			if (out->write(']')<1) {
				return false;
			}
			}
			break;
		case 'r':
			if (!writeNode(dn->getFirstTagChild(),
						out,indent,indentlevel)) {
				return false;
			}
			break;
	}
	return true;
}

const char *jsondom::getType(const domnode *dn) const {

	// return nothing for the root node
	if (dn->getType()==ROOT_DOMNODETYPE) {
		return "r";
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
