// Copyright (c) David Muse
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
	construct(true);
	#ifdef DEBUG_MESSAGES
	pvt->_indent=0;
	#endif
}

jsondom::jsondom(bool stringcacheenabled) : jsonsax(), dom(stringcacheenabled) {
	construct(stringcacheenabled);
}

jsondom::jsondom(jsondom &x) : jsonsax(), dom(x) {
	construct(getStringCacheEnabled());
}

jsondom &jsondom::operator=(jsondom &x) {
	if (this!=&x) {
		reset();
		dom::operator=(x);
	}
	return *this;
}

void jsondom::construct(bool stringcacheenabled) {
	pvt=new jsondomprivate;
	pvt->_current=NULL;
}

jsondom::~jsondom() {
	delete pvt;
}

const char *jsondom::getType() {
	return "jsondom";
}

bool jsondom::parse(input *in) {
	return parse(in,NULL,0);
}

bool jsondom::parse(input *in, domnode *parent, uint64_t position) {
	if (parent) {
		pvt->_current=parent;
	} else {
		reset();
	}
	bool	retval=sax::parse(in);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
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
	bool	retval=sax::parseFile(filename);
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
	bool	retval=sax::parseString(string);
#ifdef DEBUG_MESSAGES
	stdoutput.write("\ndom:\n");
	uint16_t	indentlevel=0;
	dom::writeNode(getRootNode(),&stdoutput,true,&indentlevel);
#endif
	return retval;
}

bool jsondom::reset() {
	pvt->_current=NULL;
	return dom::reset() && jsonsax::reset();
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

ssize_t jsondom::writeNode(domnode *dn, output *out,
				bool indent, uint16_t *indentlevel) {

	if (dn->getType()!=TAG_DOMNODETYPE && dn->getType()!=ROOT_DOMNODETYPE) {
		return 0;
	}

	ssize_t	retval=0;

	bool	inarray=(pvt->_inarray.getCount() &&
				pvt->_inarray.getLast()->getValue());
	if (dn->getType()!=ROOT_DOMNODETYPE &&
			dn->getParent()->getType()!=ROOT_DOMNODETYPE &&
			!inarray) {
		if (indent) {
			if (!incOrErr(&retval,writeIndent(out,*indentlevel))) {
				return retval;
			}
		}
		if (!incOrErr(&retval,out->write('"'),1)) {
			return retval;
		}
		const char	*name=dn->getName();
		ssize_t		len=charstring::getLength(name);
		if (!incOrErr(&retval,out->write(name,len),len)) {
			return retval;
		}
		if (!incOrErr(&retval,out->write('"'),1)) {
			return retval;
		}
		if (indent) {
			if (!incOrErr(&retval,out->write(' '),1)) {
				return retval;
			}
		}
		if (!incOrErr(&retval,out->write(':'),1)) {
			return retval;
		}
		if (indent) {
			if (!incOrErr(&retval,out->write(' '),1)) {
				return retval;
			}
		}
	}

	const char	*type=getType(dn);
	switch (*type) {
		case 'o':
			{
			if (indent) {
				if (*indentlevel && !inarray) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
				if (!inarray) {
					if (!incOrErr(&retval,
						writeIndent(out,
							*indentlevel))) {
						return retval;
					}
				}
			}
			if (!incOrErr(&retval,out->write('{'),1)) {
				return retval;
			}
			if (indent) {
				if (!incOrErr(&retval,out->write('\n'),1)) {
					return retval;
				}
				*indentlevel=*indentlevel+2;
			}
			pvt->_inarray.append(false);
			bool	first=true;
			for (domnode *child=dn->getFirstTagChild();
					!child->isNullNode();
					child=child->getNextTagSibling()) {
				if (first) {
					first=false;
				} else {
					if (!incOrErr(&retval,
							out->write(','),1)) {
						return retval;
					}
					if (indent) {
						if (!incOrErr(&retval,
							out->write('\n'),1)) {
							return retval;
						}
					}
				}
				if (!incOrErr(&retval,
						writeNode(child,out,
							indent,indentlevel))) {
					return retval;
				}
			}
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				if (!first) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
				*indentlevel=*indentlevel-2;
				if (!incOrErr(&retval,writeIndent(out,
							*indentlevel))) {
					return retval;
				}
			}
			if (!incOrErr(&retval,out->write('}'),1)) {
				return retval;
			}
			}
			break;
		case 's':
			{
			if (!incOrErr(&retval,out->write('"'),1)) {
				return retval;
			}
			const char *val=getValue(dn);
			if (val) {
				for (;;) {
					if (*val=='\0') {
						break;
					} else if (*val=='\b') {
						if (!incOrErr(&retval,
							out->write("\\b",2),
							2)) {
							return retval;
						}
					} else if (*val=='\f') {
						if (!incOrErr(&retval,
							out->write("\\f",2),
							2)) {
							return retval;
						}
					} else if (*val=='\n') {
						if (!incOrErr(&retval,
							out->write("\\n",2),
							2)) {
							return retval;
						}
					} else if (*val=='\r') {
						if (!incOrErr(&retval,
							out->write("\\r",2),
							2)) {
							return retval;
						}
					} else if (*val=='\t') {
						if (!incOrErr(&retval,
							out->write("\\t",2),
							2)) {
							return retval;
						}
					} else {
						if (!incOrErr(&retval,
							out->write(*val),1)) {
							return retval;
						}
					}
					val++;
				}
			}
			if (!incOrErr(&retval,out->write('"'),1)) {
				return retval;
			}
			}
			break;
		case 'n':
			{
			const char	*v=getValue(dn);
			if (!charstring::isNullOrEmpty(v)) {
				ssize_t	len=charstring::getLength(v);
				if (!incOrErr(&retval,out->write(v,len),len)) {
					return retval;
				}
			} else {
				if (!incOrErr(&retval,out->write('0'),1)) {
					return retval;
				}
			}
			}
			break;
		case 't':
			if (!incOrErr(&retval,out->write("true",4),4)) {
				return retval;
			}
			break;
		case 'f':
			if (!incOrErr(&retval,out->write("false",5),5)) {
				return retval;
			}
			break;
		case 'u':
			if (!incOrErr(&retval,out->write("null",4),4)) {
				return retval;
			}
			break;
		case 'a':
			{
			if (indent) {
				if (*indentlevel) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
				if (!incOrErr(&retval,
					writeIndent(out,*indentlevel))) {
					return retval;
				}
			}
			if (!incOrErr(&retval,out->write('['),1)) {
				return retval;
			}
			if (indent) {
				if (!incOrErr(&retval,out->write('\n'),1)) {
					return retval;
				}
				*indentlevel=*indentlevel+2;
			}
			pvt->_inarray.append(true);
			bool	first=true;
			for (domnode *child=dn->getFirstTagChild();
					!child->isNullNode();
					child=child->getNextTagSibling()) {
				if (first) {
					first=false;
				} else {
					if (!incOrErr(&retval,
							out->write(','),1)) {
						return retval;
					}
					if (indent) {
						if (!incOrErr(&retval,
							out->write('\n'),1)) {
							return retval;
						}
					}
				}
				if (indent) {
					if (!incOrErr(&retval,
						writeIndent(out,
							*indentlevel))) {
						return retval;
					}
				}
				if (!incOrErr(&retval,
						writeNode(child,out,
							indent,indentlevel))) {
					return retval;
				}
			}
			pvt->_inarray.remove(pvt->_inarray.getLast());
			if (indent) {
				if (!first) {
					if (!incOrErr(&retval,
							out->write('\n'),1)) {
						return retval;
					}
				}
				*indentlevel=*indentlevel-2;
				if (!incOrErr(&retval,
					writeIndent(out,*indentlevel))) {
					return retval;
				}
			}
			if (!incOrErr(&retval,out->write(']'),1)) {
				return retval;
			}
			}
			break;
		case 'r':
			if (!incOrErr(&retval,
				writeNode(dn->getFirstTagChild(),
						out,indent,indentlevel))) {
				return retval;
			}
			break;
	}
	return false;
}

const char *jsondom::getType(domnode *dn) {

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

const char *jsondom::getValue(domnode *dn) {

	// first try attribute "v"
	const char	*value=dn->getAttributeValue("v");

	// then try attribute "value"
	if (charstring::isNullOrEmpty(value)) {
		value=dn->getAttributeValue("value");
	}

	// FIXME: then fall back to the immediate text node
	return value;
}

ssize_t jsondom::writeJson() {
	return write(&stdoutput,true);
}

ssize_t jsondom::writeJson(bool indent) {
	return write(&stdoutput,indent);
}

ssize_t jsondom::writeJson(output *out) {
	return write(out,true);
}

ssize_t jsondom::writeJson(output *out, bool indent) {
	return write(out,true);
}
